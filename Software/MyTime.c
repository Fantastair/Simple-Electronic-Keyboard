// 这是整个程序的计时模块，可以随时获取时间刻以确定时间
#include "stm32f10x.h"
#include "MyTIM.h"

/**
 * @brief 计时模块初始化
 */
void MyTime_Init(void)
{
    MyTIM_Init_Internal(&MyTIM1, 65535, 36000);
}

/**
 * @brief 获取程序当前时刻
 *     本程序每秒记录 2000 个时刻，最长记录 65536 刻（之后循环）
 *     两次函数调用的返回值之差就是两次函数调用的时刻之差
 *     由于循环计数的存在，保险起见，还需要对 65535 取余
 * @retval 程序当前时刻
 */
uint16_t MyTime_GetTick(void)
{
    return MyTIM_GetCounter(&MyTIM1);
}

/**
 * @brief 将时刻单位转换成秒
 * @param ticks 时刻数，0 ~ 65535
 * @retval 以秒为单位的浮点型时间
 */
float MyTime_Tick2Second(uint16_t ticks)
{
    return (float)ticks / 2000;
}
