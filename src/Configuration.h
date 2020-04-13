/*
 * Configuration.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

#include "Tones.h"

struct config_t {
	uint32_t int_vref = 1126400L;
	uint32_t serial_baud = 2400;
	uint8_t serial_mode = SERIAL_8N1;
	char ups_company[15] = "";
	char ups_model[10] = "";
	char ups_version[10] = "";
	uint32_t tone_pitch = NOTE_C7;
	uint8_t tone_volume = 255;
};

class UserCommands;

class Configuration {
public:
	Configuration();
	virtual ~Configuration();

	bool check();

	uint32_t getVccInternalReference();
	uint32_t getSerialBaudRate();
	uint8_t  getSerialMode();

	uint32_t getTonePitch();
	uint8_t getToneVolume();

	void getUPSCompany(char *buf, size_t len);
	void getUPSModel(char *buf, size_t len);
	void getUPSVersion(char *buf, size_t len);

	friend class UserCommands;
private:
	void defaults(config_t &);
	void load(config_t &);
	bool store(config_t &);
};

#endif /* CONFIGURATION_H_ */
