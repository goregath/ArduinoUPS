/*
 * Configuration.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <CRC32.h>

#include "Configuration.h"

const int eeprom_offset = 0;
const config_t config_defaults PROGMEM;

struct eeprom_layout {
	uint32_t crc;
	config_t data;
};

Configuration::Configuration() {}

Configuration::~Configuration() {}

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

bool Configuration::check() {
	const uint32_t sto_crc32 = get_stored_checksum();
	const uint32_t cal_crc32 = checksum_over_eeprom();
	return cal_crc32 == sto_crc32;
}

uint32_t Configuration::getVccInternalReference() {
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.int_vref), val);
	if (val == 0) {
		return config_defaults.int_vref;
	}
	return val;
}

uint32_t Configuration::getSerialBaudRate() {
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.serial_baud), val);
	if (val == 0) {
		return config_defaults.serial_baud;
	}
	return val;
}

uint8_t Configuration::getSerialMode() {
	uint8_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.serial_mode), val);
	if (val == 0) {
		return config_defaults.serial_mode;
	}
	return val;
}

void Configuration::getUPSCompany(char *buf, size_t len) {
	decltype(eeprom_layout::data.ups_company) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_company), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
}

void Configuration::getUPSModel(char *buf, size_t len) {
	decltype(eeprom_layout::data.ups_model) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_model), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
}

void Configuration::getUPSVersion(char *buf, size_t len) {
	decltype(eeprom_layout::data.ups_version) fbuf;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.ups_version), fbuf);
	memcpy(buf, fbuf, min(len, sizeof(fbuf)));
}

uint32_t Configuration::getTonePitch() {
	uint32_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.tone_pitch), val);
	if (val == 0) {
		return config_defaults.tone_pitch;
	}
	return val;
}

uint8_t Configuration::getToneVolume() {
	uint8_t val = 0;
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data.tone_volume), val);
	if (val == 0) {
		return config_defaults.tone_volume;
	}
	return val;
}

void Configuration::defaults(config_t &data) {
	memcpy_P(&data, &config_defaults, sizeof(config_t));
}

void Configuration::load(config_t &data) {
	EEPROM.get(eeprom_offset + offsetof(eeprom_layout, data), data);
}

bool Configuration::store(config_t &data) {
	const uint32_t chk = calculate_checksum(data);
	EEPROM.put(eeprom_offset + offsetof(eeprom_layout, crc), chk);
	EEPROM.put(eeprom_offset + offsetof(eeprom_layout, data), data);
	return check();
}

