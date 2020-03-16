/*
 * Configuration.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <EEPROM.h>
#include "Configuration.h"

struct eeprom_layout {
	char ups_company[15];
	char ups_model[10];
	char ups_version[10];
	uint8_t serial_baud;
	uint8_t serial_mode;
	uint32_t v11;
};

Configuration::Configuration() {
}

Configuration::~Configuration() {}

uint32_t Configuration::getVccInternalReference() {
	uint32_t val = 0;
	EEPROM.get(offsetof(eeprom_layout, v11), val);
	if (! val) {
		return 1126400L;
	}
	return val;
}
