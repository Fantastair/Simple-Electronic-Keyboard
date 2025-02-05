// 前置模块：MyGPIO.h
#ifndef __MYADC_H
#define __MYADC_H
#include "stm32f10x.h"
#include "MyGPIO.h"

typedef struct {
    uint32_t RCC_APB2Periph_ADCx;
    ADC_TypeDef * ADCx;
} MyADC;

extern MyADC MyADC1, MyADC2;


void MyADC_Init(MyADC* MyADCx, FunctionalState ADC_ContinuousConvMode, FunctionalState ADC_ScanConvMode, uint8_t ADC_NbrOfChannel);
void MyADC_RegularChannelConFig(MyADC* MyADCx, uint8_t ADC_Channel_x, uint8_t Rank, uint8_t ADC_SampleTime);
void MyADC_Start(MyADC* MyADCx);
void MyADC_Wait(MyADC* MyADCx);
uint16_t MyADC_GetValue(MyADC * MyADCx);
uint32_t MyADC_GetAddr(MyADC * MyADCx);
void MyADC_StartDMA(MyADC * MyADCx);

#endif
