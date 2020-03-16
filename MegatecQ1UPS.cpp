/*
 * UPS.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#define DEBUG

#include <Arduino.h>
#include <VoltageReference.h>
#include <uptime_formatter.h>
#include <uptime.h>

#include "MegatecQ1UPS.h"
#include "MegatecQ1Response.h"
#include "UserCommands.h"

#define TX_EOL '\r'

const char UC_PRFX[] = "@";

MegatecQ1UPS::MegatecQ1UPS(
	String &rx, Print &tx, Configuration &config,
	Display &display, Beeper &beeper)
	: rx(rx)
	, tx(tx)
	, config(config)
	, display(display)
	, beeper(beeper) {
	vRef.begin(config.getVccInternalReference());
#ifdef DEBUG
	tx.println("ups ready");
#endif
}

void MegatecQ1UPS::ups_update() {
	vcc = vRef.readVcc();
	vPwr = analogRead(VPWR_PIN) * vcc / 1024;
	vBat = analogRead(VBAT_PIN) * vcc / 1024;
	vOut = analogRead(VOUT_PIN) * vcc / 1024;
	vTmp = analogRead(TMP0_PIN) * vcc / 1024;
}

void MegatecQ1UPS::ups_parse() {
	static UserCommands userCommands(* this);
	static MegatecQ1Response mq1Responder(* this);
	if (rx == "Q1") {
		// STATUS COMMAND
		// (MMM.M NNN.N PPP.P QQQ RR.R S.SS TT.T b7b6b5b4b3b2b1b0<cr>
		mq1Responder.respondQ1();
	} else if (rx.startsWith("T")) {
		// TEST COMMANDS
		// * T    10 Seconds Test
		// * TL   Test until Battery Low
		// * T<n> Test for Specified Time Period
		response("@");
	} else if (rx == "CT") {
		// CANCEL TEST COMMAND
		response("@");
	} else if (rx == "D") {
		// STATUS INQUIRY DISABLE
		response("@");
	} else if (rx.startsWith(UC_PRFX)) {
		// CUSTOM CODES
		String uc = rx.substring(sizeof(UC_PRFX) - 1);
		userCommands.parse(uc);
	} else {
		// INVALID COMMAND
		response(rx.c_str());
	}
}

void MegatecQ1UPS::response(const char* buf) {
#ifdef DEBUG
	Serial.println();
	Serial.println(buf);
#else
	Serial.print(buf);
	Serial.print(TX_EOL);
#endif
}

void MegatecQ1UPS::response(const String& buf) {
	response(buf.c_str());
}

void MegatecQ1UPS::response(const Printable& buf) {
#ifdef DEBUG
	Serial.println();
	Serial.println(buf);
#else
	Serial.print(buf);
	Serial.print(TX_EOL);
#endif
}

