#ifndef __MYNVIC_H
#define __MYNVIC_H
#include "stm32f10x.h"

void MyNVIC_Init(uint8_t Channel, uint8_t PreemptionPriority, uint8_t SubPriority);


#endif
