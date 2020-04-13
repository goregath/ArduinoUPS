/*
 * Beeper.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef BEEPER_H_
#define BEEPER_H_

#include "Configuration.h"

class Beeper {
public:
	Beeper(Configuration &conf);
	virtual ~Beeper();

	void enable();
	void disable();
	bool isActive();
	uint8_t getVolume();
	void setVolume(uint8_t vol);
private:
	uint16_t frequency_hz;
	uint8_t volume = 10;
};

#endif /* BEEPER_H_ */
