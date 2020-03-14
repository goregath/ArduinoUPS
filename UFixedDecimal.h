/*
 * IntegerPritable.h
 *
 *  Created on: Mar 8, 2020
 *      Author: goregath
 */

#ifndef UFIXEDDECIMAL_H_
#define UFIXEDDECIMAL_H_

#include <math.h>
#include <stdint.h>
#include <Print.h>
#include <Printable.h>

#include "int_math.h"

template<uint16_t IP, uint16_t FP>
class UFixedDecimal : public Printable  {
private:
	const uint16_t &_v;
	const float _f;
	char buf[IP + FP + 2];
public:
	UFixedDecimal(uint16_t &v, float f = 1.0)
		: _v(v), _f(f) {
		static_assert(IP < 5 && FP < 5, "invalid range");
		buf[IP] = '.';
		buf[sizeof(buf) - 1] = '\0';
	}
	virtual ~UFixedDecimal() {}
	
	size_t printTo(Print& p) {
		static char digits[] = "0123456789";
		uint16_t in = 9999;
		uint16_t fr = 9999;
		float v = _v * _f;
		if (v < upow10i(IP)) {
			float int_flt = 0;
			float frc_flt = 0;
			frc_flt = modff(v, &int_flt);
			in = (uint16_t) int_flt;
			fr = (uint16_t) ((frc_flt * upow10i(FP)) + 0.5);
		}
		for (int w = IP - 1; w >= 0; in /= 10, w--) {
			// fills up zeros
			buf[w] = digits[in % 10];
		}
		for (uint16_t w = sizeof(buf) - 2; w > IP; fr /= 10, w--) {
			// fills up zeros
			buf[w] = digits[fr % 10];
		}
		return IP + FP + 1;
	}
};

#endif /* UFIXEDDECIMAL_H_ */
