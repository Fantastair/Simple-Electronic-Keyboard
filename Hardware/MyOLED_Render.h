// 前置头文件：MyGPIO.h, MyOLED_
#ifndef __MYOLED_RENDER_H
#define __MYOLED_RENDER_H
#include "MyOLED.h"

void MyOLED_Blit_String_ASCII_8x16(uint8_t Left, uint8_t Top, char *String);
void MyOLED_Blit_Char_ASCII_8x16(uint8_t Left, uint8_t Top, char Char);
void MyOLED_Blit_Num_ASCII_8x16(uint8_t Left, uint8_t Top, uint32_t Number, uint8_t Length);
void MyOLED_Blit_SignedNum_ASCII_8x16(uint8_t Left, uint8_t Top, int32_t Number, uint8_t Length);
void MyOLED_Blit_HexNum_ASCII_8x16(uint8_t Left, uint8_t Top, uint32_t Number, uint8_t Length);
void MyOLED_Blit_BinNum_ASCII_8x16(uint8_t Left, uint8_t Top, uint16_t Number, uint8_t Length);
void MyOLED_Blit_FloatNum_ASCII_8x16(uint8_t Left, uint8_t Top, float Number, uint8_t int_Length, uint8_t float_Length);
void MyOLED_DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t bd);

void MyOLED_Blit_Icon_64x64(uint8_t Left);

#endif
