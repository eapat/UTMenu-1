#ifndef __TIME_UTILITIES_H
#define __TIME_UTILITIES_H
#include <stdint.h>

#define MAXU32 0xFFFFFFFF

uint32_t TimeUtilities_getDelta32(uint32_t curTime,uint32_t prevTime);

#endif
