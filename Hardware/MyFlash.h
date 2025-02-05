#ifndef __MYFLASH_H
#define __MYFLASH_H


uint32_t MyFlash_ReadData_32(uint32_t Address);
uint16_t MyFlash_ReadData_16(uint32_t Address);
uint8_t MyFlash_ReadData_8(uint32_t Address);
void MyFlash_EraseAllPages(void);
void MyFLASH_ErasePage(uint8_t page);
void MyFlash_WriteData_32(uint32_t Address, uint32_t Data);
void MyFlash_WriteData_16(uint32_t Address, uint16_t Data);
void MyFlash_WriteData_16x(uint32_t Address, uint16_t *Data, uint32_t Length);
void MyFlash_WriteData_32x(uint32_t Address, uint32_t *Data, uint32_t Length);
uint32_t MyFlash_GetPageAddress(uint8_t page);
uint8_t MyFlash_IsEmptyPage(uint8_t page);


#endif
