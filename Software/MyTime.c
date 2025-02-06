// 这是整个程序的计时模块，可以随时获取时间刻以确定时间
#include "stm32f10x.h"
#include "MyTIM.h"

volatile uint32_t time_counter = 0;  // 32位整型变量，用于记录时刻

/**
 * @brief 计时模块初始化
 */
void MyTime_Init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(SystemCoreClock / 128);
}

void SysTick_Handler(void)
{
    time_counter++;
}
/**
 * @brief 获取从程序开始直到当前的时刻
 *     本程序每秒记录 128 个时刻
 * @retval 程序当前时刻
 */
uint32_t MyTime_GetTick(void)
{
    return time_counter;
}

/**
 * @brief 将时刻单位转换成秒
 * @param ticks 时刻数，0 ~ 65535
 * @retval 以秒为单位的浮点型时间
 */
float MyTime_Tick2Second(uint32_t ticks)
{
    return ticks / 128.0;
}
