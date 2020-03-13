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

template<size_t DP, size_t FP>
class UFixedDecimal : public Printable {
private:
	const uint16_t &_v;
	const float _f;
	char buf[DP + FP + 2];
public:
	UFixedDecimal(uint16_t &v, float f = 1.0)
		: _v(v), _f(f) {}
	virtual ~UFixedDecimal() {}
	
	size_t printTo(Print& p) const {
		float fv = _v * _f;
		float iv = 0;
		float dv = modff(fv, &iv);
		int r = snprintf(buf, sizeof(buf), "%0*u.%u",
				DP,
				(unsigned int) iv,
				(unsigned int) roundf(dv * powf(10,FP))
		);
		Serial.println(r);
		Serial.println(iv);
		Serial.println((unsigned int) (dv * pow(10,FP)));
		if (r <= sizeof(buf)) {
			return p.print(buf);
		} else {
			return p.print('@');
		}
	}
};

#endif /* UFIXEDDECIMAL_H_ */
