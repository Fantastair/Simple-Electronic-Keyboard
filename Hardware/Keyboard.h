#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stm32f10x.h"

#define KEYBOARD_R1_GPIO MyGPIOA
#define KEYBOARD_R2_GPIO MyGPIOA
#define KEYBOARD_R3_GPIO MyGPIOA
#define KEYBOARD_R4_GPIO MyGPIOA
#define KEYBOARD_C1_GPIO MyGPIOA
#define KEYBOARD_C2_GPIO MyGPIOB
#define KEYBOARD_C3_GPIO MyGPIOB
#define KEYBOARD_C4_GPIO MyGPIOB

#define KEYBOARD_R1_PIN GPIO_Pin_1
#define KEYBOARD_R2_PIN GPIO_Pin_2
#define KEYBOARD_R3_PIN GPIO_Pin_3
#define KEYBOARD_R4_PIN GPIO_Pin_4
#define KEYBOARD_C1_PIN GPIO_Pin_7
#define KEYBOARD_C2_PIN GPIO_Pin_0
#define KEYBOARD_C3_PIN GPIO_Pin_1
#define KEYBOARD_C4_PIN GPIO_Pin_10


void Keyboard_Init(void);
uint16_t Keyboard_GetInput(void);


#endif
