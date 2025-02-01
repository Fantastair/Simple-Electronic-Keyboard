#ifndef __MYMISC_H
#define __MYMISC_H

#include "stm32f10x.h"


uint8_t MyMisc_GetValue16(uint16_t Num, uint8_t pos);

void MyMisc_DrawNoteSmall(uint8_t Left, uint8_t Top, uint8_t num, uint8_t by);
void MyMisc_DrawNoteBig(uint8_t Left, uint8_t Top, uint8_t num);


#endif
