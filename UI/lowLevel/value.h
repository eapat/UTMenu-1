#ifndef __VALUE_H
#define __VALUE_H

#include <stdint.h>
#include <stdbool.h>

enum ValueType {
	VALUE_INT,
	VALUE_FLOAT,
	VALUE_BOOL,
	VALUE_ENUM
};


typedef struct {
	void*			vl;
	enum ValueType	type;
	char**			units;
	float			max;
	float			min;
} Value;

#define MAKE_VALUE(Name, Value, Type, Units, max , min) \
		Value Name = {(void*)&Value, Type, Units, max , min}



#endif /* VALUE_H_ */
