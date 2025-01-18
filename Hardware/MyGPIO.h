#ifndef __MYGPIO_H
#define __MYGPIO_H
#include "stm32f10x.h"

typedef struct {
    GPIO_TypeDef * GPIOx;
    uint32_t RCC_APB2Periph_GPIOx;
} MyGPIO;

extern MyGPIO MyGPIOA;
extern MyGPIO MyGPIOB;
    
void MyGPIO_Init(MyGPIO* MyGPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode_x, GPIOSpeed_TypeDef GPIO_Speed_xMHz);

#endif
