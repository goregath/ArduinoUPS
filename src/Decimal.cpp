/*
 * UFixedDecimal.cpp
 *
 *  Created on: Apr 13, 2020
 *      Author: goregath
 */

#include "UFixedDecimal.h"
#include "ULongB10.h"

namespace UFixedDecimalHelpers {

	template<uint16_t DIV>
	inline float divide(const uint16_t &v) {
		return v / (float) DIV;
	}

	template float divide<1u>(const uint16_t &);
	template float divide<1000u>(const uint16_t &);
}

inline size_t ULongB10::printTo(Print& p) const {
	return p.print(_val, 10);
}

template<uint16_t IP, uint16_t FP>
inline UFixedDecimal<IP, FP>::UFixedDecimal(const uint16_t& v, ufx_conv_t f)
	: _v(v), _f(f) {
	static_assert(IP < 5 && FP < 5, "invalid range");
}

template<uint16_t IP, uint16_t FP>
inline size_t UFixedDecimal<IP, FP>::printTo(Print& p) const {
	static char digits[] = "0123456789";
	static char buf[FP != 0 ? (IP + FP + 2) : (IP + 1)] = { };
	uint16_t in = 9999;
	uint16_t fr = 9999;
	float v = _f(_v);
	if (v < upow10i(IP)) {
		if (FP == 0) {
			in = (uint16_t)((v + 0.5));
		} else {
			float int_flt = 0;
			float frc_flt = 0;
			frc_flt = modff(v, &int_flt);
			in = (uint16_t)(int_flt);
			fr = (uint16_t)(((frc_flt * upow10i(FP)) + 0.5));
		}
	}
	for (int w = IP - 1; w >= 0; in /= 10, w--) {
		// fills up zeros
		buf[w] = digits[in % 10];
	}
	if (FP != 0) {
		buf[IP] = '.';
		for (uint16_t w = sizeof(buf) - 2; w > IP; fr /= 10, w--) {
			// fills up zeros
			buf[w] = digits[fr % 10];
		}
	}
	return p.print(buf);
}

template class UFixedDecimal<2u,0>;
template class UFixedDecimal<1u,2u>;
template class UFixedDecimal<2u,1u>;
template class UFixedDecimal<3u,1u>;
