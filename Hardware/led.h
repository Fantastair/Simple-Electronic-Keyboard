#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "MyGPIO.h"

#define LED1_GPIO MyGPIOA
#define LED2_GPIO MyGPIOA
#define LED3_GPIO MyGPIOA
#define LED4_GPIO MyGPIOB
#define LED5_GPIO MyGPIOB
#define LED6_GPIO MyGPIOB
#define LED7_GPIO MyGPIOB
#define LED8_GPIO MyGPIOB
#define LED1_PIN GPIO_Pin_11
#define LED2_PIN GPIO_Pin_12
#define LED3_PIN GPIO_Pin_15
#define LED4_PIN GPIO_Pin_3
#define LED5_PIN GPIO_Pin_4
#define LED6_PIN GPIO_Pin_5
#define LED7_PIN GPIO_Pin_6
#define LED8_PIN GPIO_Pin_7
#define LED0_GPIO MyGPIOC
#define LED0_PIN GPIO_Pin_13

void LED_Init(void);
void LED_On(uint8_t num);
void LED_Off(uint8_t num);
uint8_t LED_IsOn(uint8_t num);
void LED_IndicateOn(void);
void LED_IndicateOff(void);

#endif
