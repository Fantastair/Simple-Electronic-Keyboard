#ifndef __MYTIME_H
#define __MYTIME_H

#include "stm32f10x.h"


void MyTime_Init(void);
uint32_t MyTime_GetTick(void);
float MyTime_Tick2Second(uint32_t ticks);



#endif
