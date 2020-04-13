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

#include "Definitions.h"
#include "UFixedDecimal.h"
#include "Beeper.h"
#include "Pinout.h"
#include "Configuration.h"

#define RV "016.0" /* MMM.M Rating Voltage */
#define RC "004"   /* QQQ   Rating Current */
#define RB "13.40" /* SS.SS Battery Voltage */
#define RF "00.0"  /* RR.R  Rating Frequency */

class MegatecQ1Response;
class UserCommands;
class Display;

enum UPS_STATE {
	RUNNING, SHUTDOWN_WAITING, SHUTDOWN, RESTORE_WAITING
};

class MegatecQ1UPS {
public:
	MegatecQ1UPS(String &rx, Print& tx, Configuration &config, Beeper &beeper);

	virtual ~MegatecQ1UPS() {}

	void update();
	void parse();

private:
	UPS_STATE state = RUNNING;
	const VoltageReference vRef = VoltageReference();

	const Configuration &config;
	const Beeper &beeper;

	const String &rx;
	Print &tx;

	uint16_t vcc = 0;
	uint16_t vOut = 0;
	uint16_t vBat = 0;
	uint16_t vPwr = 0;
	uint16_t vTmp = 0;

	bool vin_fail = false;
	bool bat_low = false;
	bool bat_dead = false;

	void response(const char *);
	void response(const String &);
	void response(const Printable &);

	friend class UserCommands;
	friend class MegatecQ1Response;
	friend class Display;
};

#endif /* MEGATECQ1UPS_H_ */
