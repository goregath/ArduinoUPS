/*
 * IntegerPritable.h
 *
 *  Created on: Mar 8, 2020
 *      Author: goregath
 */

#ifndef UFIXEDDECIMAL_H_
#define UFIXEDDECIMAL_H_

#include <Arduino.h>
#include <Print.h>
#include <Printable.h>

#include "Definitions.h"
#include "IntegerMath.h"

typedef float (& ufx_conv_t)(const uint16_t &);

namespace UFixedDecimalHelpers {
	template<uint16_t DIV>
	inline float divide(const uint16_t &);
}

template<uint16_t IP, uint16_t FP>
class UFixedDecimal : public Printable  {
private:
	const uint16_t &_v;
	const ufx_conv_t _f;
public:
	UFixedDecimal(const uint16_t& v, ufx_conv_t f);
	virtual ~UFixedDecimal() {}
	
	size_t printTo(Print& p) const;
};

#endif /* UFIXEDDECIMAL_H_ */
