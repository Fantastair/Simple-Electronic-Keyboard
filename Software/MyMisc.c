#include <stdlib.h>
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyADC.h"
#include "MyOLED.h"
#include "MyOLED_Render.h"

/**
 * @brief 获取长度为 16 的二进制数的特定位
 * @param Num 原数字，二进制长度为 16
 * @param pos 指定位序号，0 ~ 15，从高位开始数
 * @retval 0 或 1
 */
uint8_t MyMisc_GetValue16(uint16_t Num, uint8_t pos)
{
    return Num >> (15 - pos) & 1;
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

/**
 * @brief 在指定位置绘制大号音符，尺寸：12x20
 * @param Left 锚点横坐标
 * @param Top 锚点纵坐标
 * @param num 音符编号，0 ~ 7
 */
void MyMisc_DrawNoteBig(uint8_t Left, uint8_t Top, uint8_t num)
{
    switch (num)
    {
    case 0:
        MyOLED_Fill_GRAM_Rect(Left + 4, Top + 2, 4, 15);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 17, 8, 1);
        MyOLED_Blit_Point(Left + 4, Top + 2, 0);
        MyOLED_Blit_Point(Left + 3, Top + 4, 1);
        MyOLED_Blit_Point(Left + 3, Top + 5, 1);
        MyOLED_Blit_Point(Left + 2, Top + 5, 1);
        break;
    case 1:
        MyOLED_Blit_Point(Left + 2, Top + 3, 1);
        MyOLED_Blit_Point(Left + 2, Top + 4, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 8, 1);
        MyOLED_Fill_GRAM_Rect(Left + 8, Top + 3, 2, 8);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 9, 6, 2);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 11, 3, 7);
        MyOLED_Fill_GRAM_Rect(Left + 5, Top + 16, 5, 2);
        break;
    case 2:
        MyOLED_Blit_Point(Left + 2, Top + 3, 1);
        MyOLED_Blit_Point(Left + 2, Top + 4, 1);
        MyOLED_Blit_Point(Left + 2, Top + 15, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 8, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 16, 8, 2);
        MyOLED_Fill_GRAM_Rect(Left + 7, Top + 3, 3, 13);
        MyOLED_Fill_GRAM_Rect(Left + 3, Top + 9, 4, 2);
        break;
    case 3:
        MyOLED_Fill_GRAM_Rect(Left + 7, Top + 2, 3, 16);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 2, 11);
        MyOLED_Fill_GRAM_Rect(Left + 4, Top + 12, 3, 1);
        break;
    case 4:
        MyOLED_Blit_Point(Left + 2, Top + 15, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 8, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 3, 2, 8);
        MyOLED_Fill_GRAM_Rect(Left + 4, Top + 9, 6, 2);
        MyOLED_Fill_GRAM_Rect(Left + 7, Top + 11, 3, 7);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 16, 5, 2);
        break;
    case 5:
        MyOLED_Blit_Point(Left + 9, Top + 3, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 8, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 3, 3, 15);
        MyOLED_Fill_GRAM_Rect(Left + 5, Top + 9, 5, 2);
        MyOLED_Fill_GRAM_Rect(Left + 5, Top + 16, 5, 2);
        MyOLED_Fill_GRAM_Rect(Left + 8, Top + 11, 2, 5);
        break;
    case 6:
        MyOLED_Blit_Point(Left + 2, Top + 4, 1);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 2, 8, 2);
        MyOLED_Fill_GRAM_Rect(Left + 7, Top + 4, 3, 14);
        break;
    case 7:
        MyOLED_Blit_Point(Left + 3, Top + 7, 1);
        MyOLED_Blit_Point(Left + 3, Top + 8, 1);
        MyOLED_Blit_Point(Left + 2, Top + 8, 1);
        MyOLED_Fill_GRAM_Rect(Left + 5, Top + 1, 3, 3);
        MyOLED_Fill_GRAM_Rect(Left + 4, Top + 5, 4, 12);
        MyOLED_Blit_Point(Left + 4, Top + 5, 0);
        MyOLED_Fill_GRAM_Rect(Left + 2, Top + 17, 8, 1);
        break;
    default:
        break;
    }
}

/**
 * @brief 初始化 ADC 通道用于实现随机数播种
 */
void MyMisc_InitRandom(void)
{
    MyGPIO_Init(&MyGPIOA, GPIO_Pin_5, GPIO_Mode_AIN, GPIO_Speed_50MHz);
    MyADC_Init(&MyADC1, ENABLE, DISABLE, 1);
    MyADC_RegularChannelConFig(&MyADC1, ADC_Channel_5, 1, ADC_SampleTime_1Cycles5);
    MyADC_Start(&MyADC1);
}

/**
 * @brief 获取一个随机数
 * @param Start 起始值
 * @param End 结束值
 * @retval 0 ~ 65535
 */
uint16_t MyMisc_RandomInt(uint16_t Start, uint16_t End)
{
    srand(MyADC_GetValue(&MyADC1));
    return rand() % (End - Start + 1) + Start;
}
