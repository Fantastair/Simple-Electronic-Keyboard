#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stm32f10x.h"


void Keyboard_Init(void);
uint16_t Keyboard_GetInput(void);


#endif
