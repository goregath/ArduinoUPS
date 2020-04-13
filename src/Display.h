/*
 * Display.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Definitions.h"
#include "Configuration.h"
#include "MegatecQ1UPS.h"

class Display {
public:
	Display(Configuration &conf);
	virtual ~Display();
	void init();
	void update(MegatecQ1UPS &ups);
};

#endif /* DISPLAY_H_ */
