#include "stm32f10x.h"
#include "MyTIM.h"
#include "MyPWM.h"

TIM_OCInitTypeDef TIM_OCInitStructure;

/**
 * @brief 仅初始化 PWM 的输出比较单元，不包含相应时基单元的初始化
 *     要输出 PWM 波，还要初始化定时器为内部时钟，以及相关 GPIO 口为 复用推挽输出
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param MyTIM_CHx 结构体指针，包含输出比较单元的相关函数，x 可以为 1 ~ 4
 * @param TIM_OCPolarity_x 输出极性选择，x 可以为 High 或 Low
 * @retval 无
 */
void MyPWM_OCInit(MyTIM* MyTIMx, MyTIM_CH* MyTIM_CHx, uint16_t TIM_OCPolarity_x)
{
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_x;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    MyTIM_CHx->TIM_OCxInit(MyTIMx->TIMx, &TIM_OCInitStructure);
}

/**
 * @brief 设置 PWM 输出比较寄存器的值
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param MyTIM_CHx 结构体指针，包含输出比较单元的相关函数，x 可以为 1 ~ 4
 * @param CCR  输出比较寄存器的值
 * @retval 无
 */
void MyPWM_SetCCR(MyTIM* MyTIMx, MyTIM_CH* MyTIM_CHx, uint16_t CCR)
{
    MyTIM_CHx->TIM_SetComparex(MyTIMx->TIMx, CCR);
}
