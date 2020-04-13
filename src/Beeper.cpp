/*
 * Beeper.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include <Arduino.h>
#include <toneAC.h>

#include "Beeper.h"

bool active = false;

Beeper::Beeper(Configuration &conf)
	: frequency_hz(conf.getTonePitch())
	, volume(map(conf.getToneVolume(), 0, 255, 0, 10)) {}
Beeper::~Beeper() {}

void Beeper::enable() {
	toneAC(frequency_hz, volume, 0, true);
	active = true;
}

void Beeper::disable() {
	noToneAC();
	active = false;
}

bool Beeper::isActive() {
	return active;
}
