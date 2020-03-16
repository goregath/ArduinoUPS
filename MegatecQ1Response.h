/*
 * MegatecQ1Response.h
 *
 *  Created on: Mar 16, 2020
 *      Author: goregath
 */

#ifndef MEGATECQ1RESPONSE_H_
#define MEGATECQ1RESPONSE_H_

#include <Arduino.h>
#include "MegatecQ1UPS.h"

class MegatecQ1Response {
public:
	MegatecQ1Response(const MegatecQ1UPS &ups) : ups(ups) {}
	virtual ~MegatecQ1Response();
	friend class MegatecQ1UPS;
private:
	const MegatecQ1UPS &ups;

	/**
	 * Computer: Q1<cr>
	 * UPS: (MMM.M NNN.N PPP.P QQQ RR.R SS.S TT.T b7b6b5b4b3b2b1b0<cr>
	 *   MMM.M Input voltage
	 *   NNN.N Input fault voltage
	 *   PPP.P Output voltage
	 *   QQQ   Output current (percentage)
	 *   RR.R  Input frequency
	 *   SS.S  Battery voltage
	 *   TT.T  Temperature (centigrade)
	 *   UPS Status Byte
	 *     Bit# Description
	 *       7 Utility Fail (Immediate)
	 *       6 Battery Low
	 *       5 Bypass/Boost or Buck Active
	 *       4 UPS Failed
	 *       3 UPS Type is Standby (0 is On_line)
	 *       2 Test in Progress
	 *       1 Shutdown Active
	 *       0 Beeper On
	 */
	void respondQ1();
};

#endif /* MEGATECQ1RESPONSE_H_ */
