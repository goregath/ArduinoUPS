/*
 * MegatecQ1Response.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include "MegatecQ1Response.h"

#include "UFixedDecimal.h"

#define SEND(...) ups.tx.print(__VA_ARGS__)
#define FXP_MILLIS UFixedDecimalHelpers::divide<1000>

MegatecQ1Response::~MegatecQ1Response() {
}

void MegatecQ1Response::respondQ1() {
	static UFixedDecimal<3,1> vpwr(ups.vPwr, FXP_MILLIS);
	static UFixedDecimal<3,1> vout(ups.vOut, FXP_MILLIS);
	static UFixedDecimal<2,1> vbat(ups.vBat, FXP_MILLIS);
	static UFixedDecimal<2,1> ctmp(ups.vTmp, FXP_MILLIS);

	// (MMM.M NNN.N PPP.P QQQ RR.R S.SS TT.T b7b6b5b4b3b2b1b0;

	SEND('(');
	SEND(vpwr);   // MMM.M Input voltage
	SEND(' ');
	SEND(vpwr);   // NNN.N Input fault voltage
	SEND(' ');
	SEND(vout);   // PPP.P Output voltage
	SEND(' ');
	SEND("050");  // QQQ Output current (percentage)
	SEND(' ');
	SEND("00.0"); // RR.R  Input frequency
	SEND(' ');
	SEND(vbat);   // SS.S Battery voltage
	SEND(' ');
	// TODO convert to °C
	SEND(ctmp);  // TT.T  Temperature (centigrade)
	// UPS Status Byte

	SEND('\r');

//	Serial.print("(00");
//	Serial.print(Vin, 1);                   // MMM.M Input voltage
//
//	Serial.print(" 00");
//	Serial.print(Vin, 1);                  // NNN.N Input fault voltage
//
//	Serial.print(" 00");
//	Serial.print(Vout, 1);                  // PPP.P Output voltage
//
//	Serial.print(" ");
//	Serial.print("050");                    // QQQ   Output current %
//
//	Serial.print(" ");
//	Serial.print("00.0");                   // RR.R  Input frequency
//
//	Serial.print(" ");
//	Serial.print(Vbat, 2);                  // S.SS  Battery voltage
//
//	Serial.print(" ");
//	Serial.print("21.1");                   // TT.T  Temperature centigrade
//
//	Serial.print(" ");
//
//	//   UPS Status Byte
//	Serial.print(Vin_Fail);          // 7 Utility Fail (Immediate)
//	Serial.print(LowBat);            // 6 Battery Low
//	Serial.print(Vin_Fail);          // 5 Bypass/Boost or Buck Active
//	Serial.print("0");               // 4 UPS Failed
//	Serial.print("1");               // 3 UPS Type is Standby (0 is On_line)
//	Serial.print("0");               // 2 Test in Progress
//
//	if (State == Shutting_Down || State == ShutdownCommand) {
//		Serial.print("1");
//	}  // 1 Shutdown Active
//	else {
//		Serial.print("0");
//	}
//
//	Serial.print(Beeper_Enable);     // 0 Beeper On
//	Serial.print("\r");
}
