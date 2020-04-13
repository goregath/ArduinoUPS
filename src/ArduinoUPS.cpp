#include <Arduino.h>
#include <HardwareSerial.h>

#include "Definitions.h"
#include "MegatecQ1UPS.h"
#include "Display.h"
#include "Tones.h"

#define RX_BUFFER_SIZE 64

String rx = "";
bool rx_ready = false;

Configuration configuration;
Beeper beeper(configuration);
#ifdef HAS_DISPLAY
Display display(configuration);
#endif

void setup() {
	rx.reserve(RX_BUFFER_SIZE);
	if (configuration.check()) {
		Serial.begin(
			configuration.getSerialBaudRate(),
			configuration.getSerialMode()
		);
	} else {
		Serial.begin(SERIAL_BAUD, SERIAL_MODE);
	}
	while (!Serial) {;}
#ifdef HAS_BEEPER
	beeper.enable();
	delay(250);
	beeper.disable();
#endif
#ifdef HAS_DISPLAY
	display.init();
#endif
}

void loop() {
	static MegatecQ1UPS ups(rx, Serial, configuration, beeper);
	ups.update();
	if (rx_ready) {
		ups.parse();
		rx = "";
		rx_ready = false;
	}
#ifdef HAS_DISPLAY
	display.update(ups);
#endif
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
