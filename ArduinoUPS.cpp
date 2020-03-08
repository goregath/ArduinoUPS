#include <Arduino.h>
#include <HardwareSerial.h>
#include <uptime_formatter.h>

#include "ULongB10.h"

#define RX_BUFFER_SIZE 64

// overflow in approx. 50 days
unsigned long ticks;
String rx = "";
bool rx_ready = false;

// converters
ULongB10 uLongB10(&ticks);

void ups_parse();
void response(const char *);
void response(const String &);

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;
	}
	rx.reserve(RX_BUFFER_SIZE);
}

void loop() {
	ticks = millis();
}

void response(const char *buf) {
	Serial.print(buf);
	Serial.print('\r');
}

void response(const String &buf) {
	Serial.print(buf);
	Serial.print('\r');
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
	} else if (rx.startsWith("X")) {
		// CUSTOM CODES
		if (rx == "Xhelp") {
			response("help sys.uptime");
		} else if (rx == "Xsys.uptime") {
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
		switch (c) {
		case '\r':
			ups_parse();
			rx = "";
			break;
		default:
			// TODO catch overflow
			rx += c;
			break;
		}
	}
}
