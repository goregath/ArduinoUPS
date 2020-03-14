/*
 * int_math.h
 *
 *  Created on: Mar 14, 2020
 *      Author: goregath
 */

#ifndef INT_MATH_H_
#define INT_MATH_H_

#include <stdint.h>

static inline uint16_t ulog10i_floor(uint16_t v) {
	return (v >= 10000u) ? 4u : (v >= 1000u) ? 3u : (v >= 100u) ? 2u :
			(v >= 10u) ? 1u : 0u;
}

static inline uint16_t ulog10i_ceil(uint16_t v) {
	return (v >= 10000u) ? 5u : (v >= 1000u) ? 4u : (v >= 100u) ? 3u :
			(v >= 10u) ? 2u : (v >= 1u) ? 1u : 0u;
}

static inline uint16_t upow10i(uint16_t n) {
	static int pow10[10] = { 1, 10, 100, 1000, 10000 };
	return pow10[n];
}

#endif /* INT_MATH_H_ */
