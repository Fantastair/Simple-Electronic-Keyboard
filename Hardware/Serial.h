#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"


void Serial_Init(void);

void Serial_SendDataPackage(uint8_t * data, uint8_t Length);
void Serial_HandleOrder(void);


#endif
