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

inline float lm335_conv(const uint16_t &v) {
	// FIXME calculate precise temp
	// uncalibrated celsius
	return v / 10.0f - 273.15f;
}

MegatecQ1Response::~MegatecQ1Response() {
}

inline void pad_space(char *buf, size_t len) {
	size_t eos = strlen(buf);
	memset(buf + eos, 0x20, len - eos);
}

void MegatecQ1Response::respondQ1() {
	static UFixedDecimal<3,1> vpwr(ups.vPwr, FXP_MILLIS);
	static UFixedDecimal<3,1> vout(ups.vOut, FXP_MILLIS);
	static UFixedDecimal<2,1> vbat(ups.vBat, FXP_MILLIS);
	static UFixedDecimal<2,1> ctmp(ups.vTmp, lm335_conv);

	// (MMM.M NNN.N PPP.P QQQ RR.R SS.S TT.T b7b6b5b4b3b2b1b0;

	SEND('(');
	SEND(vpwr);   // MMM.M Input voltage
	SEND(' ');
	SEND(vpwr);   // NNN.N Input fault voltage
	              //   For offline UPS: Its purpose is to identify a short
	              //   duration voltage glitch which cause OFF line UPS to go
	              //   to Inverter mode. If this occurs input voltage will
	              //   appear normal at query prior to glitch and will still
	              //   appear normal at next query. The I/P fault voltage will
	              //   hold glitch voltage till next query.
	              //   After query, the I/P fault voltage will be same as
	              //   I/P voltage until next glitch occurs.
	SEND(' ');
	SEND(vout);   // PPP.P Output voltage
	SEND(' ');
	SEND("050");  // QQQ Output current (percentage)
	SEND(' ');
	SEND("00.0"); // RR.R  Input frequency as HZ
	SEND(' ');
	SEND(vbat);   // SS.S Battery voltage
	              //   For standby UPS, voltage is equal to actual battery voltage as SS.S
	              //   For online UPS, voltage is equal to voltage/cell as S.SS
	SEND(' ');
	SEND(ctmp);   // TT.T  Temperature (centigrade)
	SEND(' ');
	// UPS Status Byte
	SEND(ups.vin_fail ? '1' : '0');  // 7 Utility Fail (Immediate)
	SEND(ups.bat_low ? '1' : '0');   // 6 Battery Low
	SEND(ups.vin_fail ? '1' : '0');  // 5 Bypass/Boost or Buck Active
	SEND('0');                       // 4 UPS Failed
	                                 //   A standby UPS, also called an "offline UPS," is the most common type of UPS
	                                 //   found in a computer or office supply store. It draws current from the AC
	                                 //   outlet and switches to battery within a few milliseconds after detecting
	                                 //   a power failure.
	SEND('1');                       // 3 UPS Type is Standby
	SEND('0');                       // 2 Test in Progress
	switch (ups.state) {             // 1 Shutdown Active
	case SHUTDOWN:
	case SHUTDOWN_WAITING:
		SEND('1');
		break;
	default:
		SEND('0');
	}
	SEND(ups.beeper.isActive() ? '1' : '0'); // 0 Beeper On
	SEND('\r');
}

void MegatecQ1Response::respondF() {
	SEND("#" RV " " RC " " RB " " RF "\r");
}

void MegatecQ1Response::respondI() {
	char buf[16] = { '\0' };
	SEND('#');
	memset(buf, '\0', sizeof(buf) - 1);
	ups.config.getUPSCompany(buf, 15);
	pad_space(buf, 15);
	SEND(buf);
	SEND(' ');
	memset(buf, '\0', sizeof(buf) - 1);
	ups.config.getUPSModel(buf, 10);
	pad_space(buf, 10);
	SEND(buf);
	SEND(' ');
	memset(buf, '\0', sizeof(buf) - 1);
	ups.config.getUPSVersion(buf, 10);
	pad_space(buf, 10);
	SEND(buf);
	SEND('\r');
}
