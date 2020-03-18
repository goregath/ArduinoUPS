/*
 * UPS.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef MEGATECQ1UPS_H_
#define MEGATECQ1UPS_H_

#define U_MILLIS

#include <Arduino.h>
#include <VoltageReference.h>

#include "UFixedDecimal.h"
#include "Beeper.h"
#include "Pinout.h"
#include "Configuration.h"
#include "Display.h"

class MegatecQ1Response;
class UserCommands;

class MegatecQ1UPS {
public:
	MegatecQ1UPS(String &rx, Print& tx, Configuration &config, Display &display, Beeper &beeper);

	virtual ~MegatecQ1UPS() {}

	void ups_update();
	void ups_parse();

private:
	const VoltageReference vRef = VoltageReference();

	const Configuration &config;
	const Display &display;
	const Beeper &beeper;

	const String &rx;
	Print &tx;

	uint16_t vcc = 0;
	uint16_t vOut = 0;
	uint16_t vBat = 0;
	uint16_t vPwr = 0;
	uint16_t vTmp = 0;

	void response(const char *);
	void response(const String &);
	void response(const Printable &);

	friend class UserCommands;
	friend class MegatecQ1Response;
};

#endif /* MEGATECQ1UPS_H_ */
