#include "stm32f10x.h"

#include "MyOLED.h"
// #include "MyOLED_Render.h"

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

/**
 * @brief 在指定位置绘制小号音符，尺寸：4x7（高音 do 除外）
 * @param Left 锚点横坐标
 * @param Top 锚点纵坐标
 * @param num 音符编号，0 ~ 7
 * @param by 指定用 1 还是 0 绘制
 */
void MyMisc_DrawNoteSmall(uint8_t Left, uint8_t Top, uint8_t num, uint8_t by)
{
    switch (num)
    {
    case 0:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 1, by);
        MyOLED_Blit_Point(Left + 2, Top + 2, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 4, by);
        MyOLED_Blit_Point(Left + 2, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 1:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 1, by);
        MyOLED_Blit_Point(Left + 3, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 1, Top + 3, by);
        MyOLED_Blit_Point(Left + 0, Top + 3, by);
        MyOLED_Blit_Point(Left + 0, Top + 4, by);
        MyOLED_Blit_Point(Left + 0, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 2:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 1, by);
        MyOLED_Blit_Point(Left + 3, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 1, Top + 3, by);
        MyOLED_Blit_Point(Left + 0, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 4, by);
        MyOLED_Blit_Point(Left + 3, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 3:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 0, Top + 1, by);
        MyOLED_Blit_Point(Left + 0, Top + 2, by);
        MyOLED_Blit_Point(Left + 0, Top + 3, by);
        MyOLED_Blit_Point(Left + 1, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 1, by);
        MyOLED_Blit_Point(Left + 3, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 4, by);
        MyOLED_Blit_Point(Left + 3, Top + 5, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 4:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 0, Top + 1, by);
        MyOLED_Blit_Point(Left + 0, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 1, Top + 3, by);
        MyOLED_Blit_Point(Left + 0, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 4, by);
        MyOLED_Blit_Point(Left + 3, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 5:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 0, Top + 1, by);
        MyOLED_Blit_Point(Left + 0, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 1, Top + 3, by);
        MyOLED_Blit_Point(Left + 0, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 4, by);
        MyOLED_Blit_Point(Left + 3, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        MyOLED_Blit_Point(Left + 0, Top + 4, by);
        MyOLED_Blit_Point(Left + 0, Top + 5, by);
        break;
    case 6:
        MyOLED_Blit_Point(Left + 0, Top + 1, by);
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 0, by);
        MyOLED_Blit_Point(Left + 3, Top + 1, by);
        MyOLED_Blit_Point(Left + 3, Top + 2, by);
        MyOLED_Blit_Point(Left + 3, Top + 3, by);
        MyOLED_Blit_Point(Left + 3, Top + 4, by);
        MyOLED_Blit_Point(Left + 3, Top + 5, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        break;
    case 7:
        MyOLED_Blit_Point(Left + 0, Top + 0, by);
        MyOLED_Blit_Point(Left + 1, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 0, by);
        MyOLED_Blit_Point(Left + 2, Top + 1, by);
        MyOLED_Blit_Point(Left + 2, Top + 2, by);
        MyOLED_Blit_Point(Left + 2, Top + 3, by);
        MyOLED_Blit_Point(Left + 2, Top + 4, by);
        MyOLED_Blit_Point(Left + 2, Top + 5, by);
        MyOLED_Blit_Point(Left + 0, Top + 6, by);
        MyOLED_Blit_Point(Left + 1, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top + 6, by);
        MyOLED_Blit_Point(Left + 3, Top + 6, by);
        MyOLED_Blit_Point(Left + 2, Top - 2, by);
        break;
    default:
        break;
    }
}
