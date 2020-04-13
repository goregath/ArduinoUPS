#include <Arduino.h>
//#include <Wire.h>
#include <HardwareSerial.h>

#include "MegatecQ1UPS.h"
#include "Display.h"
#include "Tones.h"

#define RX_BUFFER_SIZE 64

String rx = "";
bool rx_ready = false;

Configuration configuration;
Display display(configuration);
Beeper beeper(configuration);

void setup() {
//	Wire.setClock(100000L);
	if (configuration.check()) {
		Serial.begin(
			configuration.getSerialBaudRate(),
			configuration.getSerialMode()
		);
	} else {
		Serial.begin(9600, SERIAL_8N1);
	}
	while (!Serial) {;}
	beeper.enable();
	delay(250);
	beeper.disable();
	display.init();
	rx.reserve(RX_BUFFER_SIZE);
}

void loop() {
	static MegatecQ1UPS ups(rx, Serial, configuration, beeper);
	ups.update();
	if (rx_ready) {
		ups.parse();
		rx = "";
		rx_ready = false;
	}
	display.update(ups);
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
