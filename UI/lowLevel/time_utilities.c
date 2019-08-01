/* Файл реализует функции времени */
#include "time_utilities.h"

/*
 * Функция возвращает разницу во времени с учетом переполнения
 * uint32_t curTime - текущее время
 * uint32_t prevTime - предыдущее время
 */
uint32_t TimeUtilities_getDelta32(uint32_t curTime,uint32_t prevTime){
	if(curTime>prevTime)
		return curTime-prevTime;
	else
		return MAXU32-prevTime+curTime;
}

