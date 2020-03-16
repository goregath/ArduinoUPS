#include <Arduino.h>
#include <HardwareSerial.h>

#include "MegatecQ1UPS.h"

#define DEBUG

#define RX_BUFFER_SIZE 64

String rx = "";
bool rx_ready = false;

Configuration configuration;
Display display;
Beeper beeper;

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;
	}
	rx.reserve(RX_BUFFER_SIZE);
}

void loop() {
	static MegatecQ1UPS ups(rx, Serial, configuration, display, beeper);
	if (rx_ready) {
		ups.ups_update();
		ups.ups_parse();
		rx = "";
		rx_ready = false;
	}
}

void serialEvent() {
	if (rx_ready == true) return;
	while (Serial.available()) {
		char c = (char) Serial.read();
#ifdef DEBUG
		if (isprint(c)) {
			Serial.print(c);
		}
#endif
		switch (c) {
		case '\r':
			rx_ready = true;
			break;
		default:
			if (isprint(c) && rx.length() < RX_BUFFER_SIZE) {
				rx += c;
			}
			break;
		}
	}
}
