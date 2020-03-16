/*
 * UserCommands.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <uptime_formatter.h>
#include "UserCommands.h"

#define FXP_MILLIS 0.001

UserCommands::~UserCommands() {}

void UserCommands::parse(const String &uc) {
	static UFixedDecimal<3,2> vcc(ups.vcc, FXP_MILLIS);
	if (uc == "help") {
		ups.response("help sys.uptime sys.vcc");
	} else if (uc == "sys.uptime") {
		ups.response("up " + uptime_formatter::getUptime());
	} else if (uc == "sys.vcc") {
		ups.response(vcc);
	} else {
		ups.response("@");
	}
}
