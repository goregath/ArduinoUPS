/*
 * UserCommands.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef USERCOMMANDS_H_
#define USERCOMMANDS_H_

#include "Definitions.h"
#include "MegatecQ1UPS.h"

const char UC_PRFX[] = "@";

class UserCommands {
public:
	UserCommands(const MegatecQ1UPS &ups) : ups(ups) {}
	virtual ~UserCommands();
	void parse(const String &rx);
private:
	const MegatecQ1UPS &ups;
};

#endif /* USERCOMMANDS_H_ */
