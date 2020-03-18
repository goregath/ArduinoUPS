/*
 * Configuration.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

struct config_t {
	uint32_t int_vref = 1126400L;
	uint32_t serial_baud = 9600;
	uint8_t serial_mode = SERIAL_8N1;
	char ups_company[15] = "Arduino UPS";
	char ups_model[10] = "MegatecQ1";
	char ups_version[10] = "ATmega328";
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

	friend class UserCommands;
private:
	void load(config_t &);
	bool store(config_t &);
};

#endif /* CONFIGURATION_H_ */
