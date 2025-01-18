#ifndef __MYTIME_H
#define __MYTIME_H

#include "stm32f10x.h"


void MyTime_Init(void);
uint16_t MyTime_GetTick(void);
float MyTime_Tick2Second(uint16_t ticks);



#endif
