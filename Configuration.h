/*
 * Configuration.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>

class Configuration {
public:
	Configuration();
	virtual ~Configuration();

	uint32_t getVccInternalReference();
};

#endif /* CONFIGURATION_H_ */
