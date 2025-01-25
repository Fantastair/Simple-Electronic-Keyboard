// 前置头文件：MyTIM.h
#ifndef __MYPWM_H
#define __MYPWM_H
#include "MyTIM.h"


void MyPWM_OCInit(MyTIM* MyTIMx, MyTIM_CH* MyTIM_CHx, uint16_t TIM_OCPolarity_x);
void MyPWM_SetCCR(MyTIM* MyTIMx, MyTIM_CH* MyTIM_CHx, uint16_t CCR);


#endif
