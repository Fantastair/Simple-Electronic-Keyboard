#include "stm32f10x.h"

/**
 * @brief 获取长度为 16 的二进制数的特定位
 * @param Num 原数字，二进制长度为 16
 * @param pos 指定位序号，0 ~ 15，从高位开始数
 * @retval 0 或 1
 */
uint8_t MyMisc_GetValue16(uint16_t Num, uint8_t pos)
{
    return (Num >> (15 - pos)) % 2;
}
