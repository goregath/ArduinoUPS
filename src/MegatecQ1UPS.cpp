/*
 * UPS.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

//#define DEBUG

#include <Arduino.h>

#include "MegatecQ1UPS.h"
#include "MegatecQ1Response.h"
#include "UserCommands.h"

#ifdef HAS_VREF
#include <VoltageReference.h>
#endif

MegatecQ1UPS::MegatecQ1UPS(
	String &rx, Print &tx, Configuration &config, Beeper &beeper)
	: rx(rx)
	, tx(tx)
	, config(config)
	, beeper(beeper) {
#ifdef HAS_VREF
	vRef.begin(config.getVccInternalReference());
#endif
#ifdef DEBUG
	tx.println("ups ready");
#endif
}

void MegatecQ1UPS::update() {
#ifdef HAS_VREF
	vcc = vRef.readVcc();
#else
	vcc = 5000;
#endif
	// FIXME overflow
	vPwr = analogRead(VPWR_PIN) * vcc / 1024;
	vBat = analogRead(VBAT_PIN) * vcc / 1024;
	vOut = analogRead(VOUT_PIN) * vcc / 1024;
	vTmp = analogRead(TMP0_PIN) * vcc / 1024;
}

void MegatecQ1UPS::parse() {
#ifdef HAS_USERCOMMANDS
	static UserCommands userCommands(* this);
#endif
	static MegatecQ1Response mq1Responder(* this);
	if (rx == "Q1") {
		// STATUS COMMAND
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
	} else if (rx == "F") {
		// UPS RATING INFORMATION
		mq1Responder.respondF();
	} else if (rx == "I") {
		// UPS INFORMATION COMMAND
		mq1Responder.respondI();
	} else if (rx == "Q") {
		// TURN ON/OFF BEEP - TOGGLE THE UPS BEEPER
		if (beeper.isActive()) {
			beeper.disable();
		} else {
			beeper.enable();
		}
#ifdef HAS_USERCOMMANDS
	} else if (rx.startsWith(UC_PRFX)) {
		// CUSTOM CODES
		String uc = rx.substring(sizeof(UC_PRFX) - 1);
		userCommands.parse(uc);
#endif
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

