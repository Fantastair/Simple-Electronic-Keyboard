// 前置头文件：MyI2C.h
#ifndef __MYOLED_H
#define __MYOLED_H
#include "stm32f10x.h"

void MyOLED_Init(void);
void MyOLED_Write_Command(uint8_t Command);
void MyOLED_SetCursor(uint8_t Y, uint8_t X);
void MyOLED_Write_Data(uint8_t Data);
void MyOLED_Flip(void);
void MyOLED_Flip_Reverse(void);
void MyOLED_Update(uint8_t Left, uint8_t Top_Page, uint8_t Width, uint8_t Page_Height);
void MyOLED_Update_Reverse(uint8_t Left, uint8_t Top_Page, uint8_t Width, uint8_t Page_Height);

void MyOLED_Clear_GRAM(void);
void MyOLED_Clear_GRAM_Rect(uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height);
void MyOLED_Fill_GRAM_Rect(uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height);
void MyOLED_Blit_GRAM(const uint8_t data[], uint8_t Left, uint8_t Top, uint8_t Length);
void MyOLED_Blit_Point(uint8_t X, uint8_t Y, uint8_t Bit);

#endif
