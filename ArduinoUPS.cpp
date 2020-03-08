#include <Arduino.h>
#include <HardwareSerial.h>

#define RX_BUFFER_SIZE 64

String rx = "";
bool rx_ready = false;

void ups_parse();
void response(const char *);

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;
	}
	rx.reserve(RX_BUFFER_SIZE);
}

void loop() {
}

void response(const char *buf) {
	Serial.print(buf);
	Serial.print('\r');
}

void ups_parse() {
	if (rx.startsWith("T")) {
		// TEST COMMANDS
		// * T    10 Seconds Test
		// * TL   Test until Battery Low
		// * T<n> Test for Specified Time Period
		Serial.print("@\r");
	} else if (rx == "CT") {
		// CANCEL TEST COMMAND
		response("@");
	} else if (rx == "D") {
		// STATUS INQUIRY DISABLE
		response("@");
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
