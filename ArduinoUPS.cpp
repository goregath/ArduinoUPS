#include <ctype.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <uptime_formatter.h>

#include "Config.h"
#include "ULongB10.h"
#include "UFixedDecimal.h"

// overflow in approx. 50 days
unsigned long ticks;
String rx = "";
bool rx_ready = false;

uint16_t vccRef;

// converters
ULongB10 uLongB10(ticks);
UFixedDecimal<3,2> vccRef_3_2(vccRef, 0.001);

void ups_parse();
void response(const char *);
void response(const String &);

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;
	}
	rx.reserve(RX_BUFFER_SIZE);
#ifdef DEBUG
	vccRef = 1234;
	Serial.println(vccRef);
	Serial.println(vccRef_3_2);
	vccRef = 0;
	while(true) {
		Serial.println(vccRef_3_2);
		vccRef++;
	}
	response("ready");
#endif
}

void loop() {
	ticks = millis();
}

void response(const char *buf) {
#ifdef DEBUG
	Serial.println();
	Serial.println(buf);
#else
	Serial.print(buf);
	Serial.print(TX_EOL);
#endif
}

void response(const String &buf) {
	response(buf.c_str());
}

void ups_parse() {
	if (rx.startsWith("T")) {
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
		String userCommand = rx.substring(sizeof(UC_PRFX) - 1);
		if (userCommand == "help") {
			response("help sys.uptime");
		} else if (userCommand == "sys.uptime") {
			response("up " + uptime_formatter::getUptime());
		} else {
			response("@");
		}
	} else {
		// INVALID COMMAND
		response(rx.c_str());
	}
}

void serialEvent() {
	while (Serial.available()) {
		char c = (char) Serial.read();
#ifdef DEBUG
		if (isprint(c)) {
			Serial.print(c);
		}
#endif
		switch (c) {
		case '\r':
			ups_parse();
			rx = "";
			break;
		default:
			if (isprint(c) && rx.length() < RX_BUFFER_SIZE) {
				rx += c;
			}
			break;
		}
	}
}
