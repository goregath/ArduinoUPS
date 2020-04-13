/*
 * UserCommands.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <uptime_formatter.h>
#include <inttypes.h>
#include "UserCommands.h"
#include "Configuration.h"

#define UPS_OK ups.response("ok");
#define UPS_ERR ups.response("error");

#define FXP_MILLIS UFixedDecimalHelpers::divide<1000>

UserCommands::~UserCommands() {}

template<typename T>
void print(Print &p, int idx, T v) {
	p.print("[0x");
	p.print(idx, HEX);
	p.print(':');
	p.print(sizeof(v));
	p.print("]=");
	p.print(v);
}

void print(Print &p, int idx, const char *v, size_t len) {
	p.print("[0x");
	p.print(idx, HEX);
	p.print(':');
	p.print(len);
	p.print("]=");
	for (int i = 0; i < len; ++i) {
		if (isprint(v[i])) {
			p.write(v[i]);
		} else {
			p.write('?');
		}
	}
}

bool parse_addr(const char *buf, size_t &addr) {
	unsigned long lv = strtoul(buf, NULL, 0);
	if (lv >= sizeof(config_t)) {
		return false;
	}
	addr = lv;
	return true;
}

template<typename T>
bool parse_numeric(const char *buf, T &num) {
	unsigned long ul = strtoul(buf, NULL, 0);
	if (ul > ((T) -1)) {
		return false;
	}
	num = (T) ul;
	return true;
}

inline void print_hex(Print &p, const char *buf, size_t len) {
	static const char hex[] = "0123456789abcdef";
	for (unsigned int i = 0; i < len; ++i) {
		p.write((uint8_t) hex[(buf[i] >> 4) & 0xF]);
		p.write((uint8_t) hex[buf[i] & 0xF]);
	}
	p.flush();
}

inline int xtob(char b) {
	if (b >= '0' && b <= '9') {
		b = b - '0';
	} else if (b >= 'a' && b <='f') {
		b = b - 'a' + 10;
	} else if (b >= 'A' && b <='F') {
		b = b - 'A' + 10;
	} else {
		b = 0;
	}
	return b;
}

void UserCommands::parse(const String &uc) {
	static config_t *data = NULL;
	if (uc == "check") {
		if (ups.config.check()) {
			UPS_OK
		} else {
			UPS_ERR
		}
	} else if (uc == "dump") {
		if (data != NULL) {
			print_hex(ups.tx, (char *) data, sizeof(* data));
			UPS_OK
		} else {
			UPS_ERR
		}
	} else if (uc == "get") {
		if (data != NULL) {
			ups.tx.print(F("(vref)"));
			print(ups.tx, offsetof(config_t, int_vref), data->int_vref);
			ups.tx.print(F(",(serial_rate)"));
			print(ups.tx, offsetof(config_t, serial_baud), data->serial_baud);
			ups.tx.print(F(",(serial_mode)"));
			print(ups.tx, offsetof(config_t, serial_mode), data->serial_mode);
			ups.tx.print(F(",(vendor)"));
			print(ups.tx, offsetof(config_t, ups_company), data->ups_company, sizeof(data->ups_company));
			ups.tx.print(F(",(model)"));
			print(ups.tx, offsetof(config_t, ups_model), data->ups_model, sizeof(data->ups_model));
			ups.tx.print(F(",(version)"));
			print(ups.tx, offsetof(config_t, ups_version), data->ups_version, sizeof(data->ups_version));
			ups.tx.print(F(",(tone)"));
			print(ups.tx, offsetof(config_t, tone_pitch), data->tone_pitch);
			ups.tx.print(F(",(volume)"));
			print(ups.tx, offsetof(config_t, tone_volume), data->tone_volume);
			ups.tx.print('\r');
		} else {
			UPS_ERR
		}
		// TODO overthink memory consumption
//	} else if (uc == "help set") {
//		ups.response("set [cbsih],addr,[hex|str]");
//	} else if (uc.startsWith("help")) {
//		ups.response(F("check dump get help load uptime reset set store unload"));
	} else if (uc == "load") {
		if (data == NULL) {
			data = malloc(sizeof(config_t));
		}
		if (data != NULL) {
			ups.config.load(* data);
			UPS_OK
		} else {
			UPS_ERR
		}
	} else if (uc == "uptime") {
		ups.response("up " + uptime_formatter::getUptime());
	} else if (uc.startsWith("set ")) {
		if (data != NULL) {
			char *stmt = malloc(uc.length());
			char *pc;
			strcpy(stmt, uc.c_str());
			strtok(stmt, " ,.-");
			const char *type = strtok(NULL, ",");
			const char *addr = strtok(NULL, ",");
			const char *val = strtok(NULL, ",");
			if (type != NULL && addr != NULL && val != NULL) {
				size_t offset = 0;
				if (parse_addr(addr, offset)) {
					void *daddr = (void *) data;
					switch(type[0]) {
					case 'h': {
							size_t hex_sz = strlen(val);
							size_t buf_sz = min(hex_sz / 2, sizeof(* data) - offset);
							if ((hex_sz % 2) == 0 || buf_sz > 0) {
								uint8_t *buf = calloc(sizeof(char), buf_sz);
								for (int i = 0; i < buf_sz; ++i) {
									buf[i] = (uint8_t) (xtob(val[i*2]) << 4) | xtob(val[i*2+1]);
								}
								memcpy(daddr + offset, buf, buf_sz);
								free(buf);
								UPS_OK
							} else {
								UPS_ERR
							}
						}
						break;
					case 'c':
						memcpy(daddr + offset, val, min(strlen(val), sizeof(* data) - offset));
						UPS_OK
						break;
					case 'b': {
							uint8_t ival = 0;
							if (parse_numeric(val, ival)) {
								*((uint8_t *)(daddr + offset)) = ival;
								UPS_OK
							} else {
								UPS_ERR
							}
						}
						break;
					case 's': {
							uint16_t ival = 0;
							if (parse_numeric(val, ival)) {
								*((uint16_t *)(daddr + offset)) = ival;
								UPS_OK
							} else {
								UPS_ERR
							}
						}
						break;
					case 'i': {
							uint32_t ival = 0;
							if (parse_numeric(val, ival)) {
								*((uint32_t *)(daddr + offset)) = ival;
								UPS_OK
							} else {
								UPS_ERR
							}
						}
						break;
					default:
						UPS_ERR
					}
				} else {
					UPS_ERR
				}
			} else {
				UPS_ERR
			}
			free(stmt);
		} else {
			UPS_ERR
		}
	} else if (uc == "reset") {
		if (data != NULL) {
			ups.config.defaults(* data);
			UPS_OK
		} else {
			UPS_ERR
		}
	} else if (uc == "store") {
		if (data != NULL && ups.config.store(* data)) {
			UPS_OK
		} else {
			UPS_ERR
		}
	} else if (uc == "unload") {
		if (data != NULL) {
			free(data);
			data = NULL;
			UPS_OK
		} else {
			UPS_ERR
		}
	} else {
		ups.response("@");
	}
}
