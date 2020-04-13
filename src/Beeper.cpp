/*
 * Beeper.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#include "Definitions.h"

#include <Arduino.h>
#ifdef HAS_BEEPER
#include <toneAC.h>
#endif

#include "Beeper.h"

bool active = false;

Beeper::Beeper(Configuration &conf)
#ifdef HAS_BEEPER
	: frequency_hz(conf.getTonePitch())
	, volume(map(conf.getToneVolume(), 0, 255, 0, 10))
#endif
{}
Beeper::~Beeper() {}

void Beeper::enable() {
#ifdef HAS_BEEPER
	toneAC(frequency_hz, volume, 0, true);
	active = true;
#endif
}

void Beeper::disable() {
#ifdef HAS_BEEPER
	noToneAC();
	active = false;
#endif
}

bool Beeper::isActive() {
#ifdef HAS_BEEPER
	return active;
#else
	return false;
#endif
}
