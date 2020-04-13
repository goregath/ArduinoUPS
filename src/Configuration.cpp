/*
 * Configuration.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Configuration.h"

#ifdef HAS_EEPROM
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <CRC32.h>
#endif


const config_t config_defaults PROGMEM;

Configuration::Configuration() {}

Configuration::~Configuration() {}

#ifdef HAS_EEPROM
const int eeprom_offset = 0;

struct eeprom_layout {
	uint32_t crc;
	config_t data;
};

uint32_t checksum_over_eeprom() {
	const size_t blk = sizeof(config_t);
	CRC32 chk;
	int eepromStart = eeprom_offset + offsetof(eeprom_layout, data);
	for (uint16_t i = 0; i < blk; ++i) {
		chk.update((uint8_t) EEPROM[eepromStart + i]);
	}
	return chk.finalize();
}

uint32_t calculate_checksum(config_t &data) {
	return CRC32::calculate(&data, 1);
}

uint32_t get_stored_checksum() {
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, crc), val);
	return val;
}
#endif

bool Configuration::check() {
#ifdef HAS_EEPROM
	const uint32_t sto_crc32 = get_stored_checksum();
	const uint32_t cal_crc32 = checksum_over_eeprom();
	return cal_crc32 == sto_crc32;
#else
	return true;
#endif
}

uint32_t Configuration::getVccInternalReference() {
#ifdef HAS_EEPROM
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.int_vref), val);
	if (val == 0) {
#endif
		return config_defaults.int_vref;
#ifdef HAS_EEPROM
	}
	return val;
#endif
}

uint32_t Configuration::getSerialBaudRate() {
#ifdef HAS_EEPROM
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.serial_baud), val);
	if (val == 0) {
#endif
		return config_defaults.serial_baud;
#ifdef HAS_EEPROM
	}
	return val;
#endif
}

uint8_t Configuration::getSerialMode() {
#ifdef HAS_EEPROM
	uint8_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.serial_mode), val);
	if (val == 0) {
#endif
		return config_defaults.serial_mode;
#ifdef HAS_EEPROM
	}
	return val;
#endif
}

void Configuration::getUPSCompany(char *buf, size_t len) {
#ifdef HAS_EEPROM
	decltype(eeprom_layout::data.ups_company) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_company), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
#else
	strcpy(buf, UPS_COMPANY);
#endif
}

void Configuration::getUPSModel(char *buf, size_t len) {
#ifdef HAS_EEPROM
	decltype(eeprom_layout::data.ups_model) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_model), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
#else
	strcpy(buf, UPS_MODEL);
#endif
}

void Configuration::getUPSVersion(char *buf, size_t len) {
#ifdef HAS_EEPROM
	decltype(eeprom_layout::data.ups_version) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_version), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
#else
	strcpy(buf, UPS_VERSION);
#endif
}

uint32_t Configuration::getTonePitch() {
#ifdef HAS_EEPROM
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.tone_pitch), val);
	if (val == 0) {
#endif
		return config_defaults.tone_pitch;
#ifdef HAS_EEPROM
	}
	return val;
#endif
}

uint8_t Configuration::getToneVolume() {
#ifdef HAS_EEPROM
	uint8_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.tone_volume), val);
	if (val == 0) {
#endif
		return config_defaults.tone_volume;
#ifdef HAS_EEPROM
	}
	return val;
#endif
}

void Configuration::defaults(config_t &data) {
#ifdef HAS_EEPROM
	memcpy_P(&data, &config_defaults, sizeof(config_t));
#endif
}

void Configuration::load(config_t &data) {
#ifdef HAS_EEPROM
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data), data);
#endif
}

bool Configuration::store(config_t &data) {
#ifdef HAS_EEPROM
	const uint32_t chk = calculate_checksum(data);
	EEPROM.put(eeprom_offset + offsetof(eeprom_layout, crc), chk);
	EEPROM.put(eeprom_offset + offsetof(eeprom_layout, data), data);
#endif
	return check();
}
