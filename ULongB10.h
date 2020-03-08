/*
 * IntegerPritable.h
 *
 *  Created on: Mar 8, 2020
 *      Author: goregath
 */

#ifndef ULONGB10_H_
#define ULONGB10_H_

#include <Print.h>
#include <Printable.h>

class ULongB10 : public Printable {
private:
	unsigned long int*_val;
public:
	ULongB10(unsigned long *val) : _val(val){}
	virtual ~ULongB10() {}
	
	size_t printTo(Print& p) const {
		return p.print(*_val, 10);
	}
};

#endif /* ULONGB10_H_ */