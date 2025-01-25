// 前置头文件：MyNVIC.h
#ifndef __MYTIM_H
#define __MYTIM_H
#include "stm32f10x.h"


typedef struct {
    TIM_TypeDef * TIMx;
    uint32_t RCC_APBxPeriph_TIMx;
    enum IRQn TIMx_IRQn;
    void (*RCC_APBxPeriphClockCmd)(uint32_t, FunctionalState);
} MyTIM;

extern MyTIM MyTIM1, MyTIM2, MyTIM3, MyTIM4;


typedef struct {
    void (*TIM_OCxInit)(TIM_TypeDef*, TIM_OCInitTypeDef*);
    void (*TIM_SetComparex)(TIM_TypeDef*, uint16_t);
    uint16_t (*TIM_GetCapturex)(TIM_TypeDef*);
} MyTIM_CH;

extern MyTIM_CH MyTIM_CH1, MyTIM_CH2, MyTIM_CH3, MyTIM_CH4;


void MyTIMBase_Init(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC);
void MyTIM_Init_Internal(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC);
void MyTIM_Init_ETR(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC, uint16_t TIM_ExtTRGPSC_x, uint16_t TIM_ExtTRGPolarity_x, uint16_t Filter);
void MyTIM_NVIC_Config(MyTIM* MyTIMx, uint8_t PreemptionPriority, uint8_t SubPriority);
uint16_t MyTIM_GetCounter(MyTIM* MyTIMx);
void MyTIM_SetCounter(MyTIM* MyTIMx, uint16_t Value);
void MyTIM_SetPSC(MyTIM * MyTIMx, uint16_t MyPSC, uint16_t TIM_PSCReloadMode_x);
void MyTIM_SetARR(MyTIM * MyTIMx, uint16_t MyARR);


#endif
