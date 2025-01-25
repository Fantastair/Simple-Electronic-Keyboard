#include "stm32f10x.h"
// #include "MyGPIO.h"
#include "MyNVIC.h"
#include "MyTIM.h"


MyTIM MyTIM1 = {TIM1, RCC_APB2Periph_TIM1, TIM1_UP_IRQn, RCC_APB2PeriphClockCmd};
MyTIM MyTIM2 = {TIM2, RCC_APB1Periph_TIM2, TIM2_IRQn, RCC_APB1PeriphClockCmd};
MyTIM MyTIM3 = {TIM3, RCC_APB1Periph_TIM3, TIM3_IRQn, RCC_APB1PeriphClockCmd};
MyTIM MyTIM4 = {TIM4, RCC_APB1Periph_TIM4, TIM4_IRQn, RCC_APB1PeriphClockCmd};

MyTIM_CH MyTIM_CH1 = {TIM_OC1Init, TIM_SetCompare1, TIM_GetCapture1};
MyTIM_CH MyTIM_CH2 = {TIM_OC2Init, TIM_SetCompare2, TIM_GetCapture2};
MyTIM_CH MyTIM_CH3 = {TIM_OC3Init, TIM_SetCompare3, TIM_GetCapture3};
MyTIM_CH MyTIM_CH4 = {TIM_OC4Init, TIM_SetCompare4, TIM_GetCapture4};


TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

/**
 * @brief 时基单元初始化
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param My_ARR 自动重装器数值，0 ~ 65535，注意实际的值是加一的
 * @param My_PSC 预分频器数值，0 ~ 65535，注意实际的值是加一的
 * @retval 无
 */
void MyTIMBase_Init(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC)
{
    MyTIMx->RCC_APBxPeriphClockCmd(MyTIMx->RCC_APBxPeriph_TIMx, ENABLE);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = My_ARR;
    TIM_TimeBaseInitStructure.TIM_Prescaler = My_PSC;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MyTIMx->TIMx, &TIM_TimeBaseInitStructure);

    TIM_Cmd(MyTIMx->TIMx, ENABLE);
}

/**
 * @brief 通用定时器初始化，使用内部时钟
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param My_ARR 自动重装器数值，0 ~ 65535，注意实际的值是加一的
 * @param My_PSC 预分频器数值，0 ~ 65535，注意实际的值是加一的
 * @retval 无
 */
void MyTIM_Init_Internal(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC)
{
    MyTIMBase_Init(MyTIMx, My_ARR, My_PSC);
    TIM_InternalClockConfig(MyTIMx->TIMx);
}

/**
 * @brief 通用定时器初始化，使用外部时钟
 *     需要提前配置相应的 GPIO 作为输入端口，推荐 上拉输入模式
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param My_ARR 自动重装器数值，0 ~ 65535，注意实际的值是加一的
 * @param My_PSC 预分频器数值，0 ~ 65535，注意实际的值是加一的
 * @param TIM_ExtTRGPSC_x 外部输入预分频
 * @param TIM_ExtTRGPolarity_x 输入极性反相或不反相
 * @param Filter 外部触发滤波器，范围 0x00 ~ 0xff，消抖保险可使用 0x0f
 * @retval 无
 */
void MyTIM_Init_ETR(MyTIM* MyTIMx, uint16_t My_ARR, uint16_t My_PSC, uint16_t TIM_ExtTRGPSC_x, uint16_t TIM_ExtTRGPolarity_x, uint16_t Filter)
{
    MyTIMBase_Init(MyTIMx, My_ARR, My_PSC);
    TIM_ETRClockMode2Config(MyTIMx->TIMx, TIM_ExtTRGPSC_x, TIM_ExtTRGPolarity_x, Filter);
}

/**
 * @brief 配置定时器定时中断，需要提前初始化定时器
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param PreemptionPriority 抢占优先级，可以为 0 ~ 15
 * @param SubPriority 响应优先级，可以为 0 ~ 15
 * @retval 无
 */
void MyTIM_NVIC_Config(MyTIM* MyTIMx, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    TIM_ITConfig(MyTIMx->TIMx, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(MyTIMx->TIMx, TIM_FLAG_Update);
    MyNVIC_Init(MyTIMx->TIMx_IRQn, PreemptionPriority, SubPriority);
}

/* 中断函数模板
void TIMx_IRQHandler(void)
{
    if (TIM_GetITStatus(TIMx, TIM_IT_Update) == SET)
    {
         
        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    }
}
*/

/**
 * @brief 获取计数器当前的值
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @retval 计数器当前的值
 */
uint16_t MyTIM_GetCounter(MyTIM* MyTIMx)
{
    return TIM_GetCounter(MyTIMx->TIMx);
}

/**
 * @brief 设置计数器当前的值
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param Value 修改后的数值
 * @retval 无
 */
void MyTIM_SetCounter(MyTIM* MyTIMx, uint16_t Value)
{
    TIM_SetCounter(MyTIMx->TIMx, Value);
}

/**
 * @brief 修改指定定时器的预分频系数
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param MyPSC 修改后的值
 * @param TIM_PSCReloadMode_x 生效模式，x 可以为 Update（下个周期生效）或 Immediate
 * @retval 无
 */
void MyTIM_SetPSC(MyTIM * MyTIMx, uint16_t MyPSC, uint16_t TIM_PSCReloadMode_x)
{
    TIM_PrescalerConfig(MyTIMx->TIMx, MyPSC, TIM_PSCReloadMode_x);
}

/**
 * @brief 修改指定定时器的自动重装系数
 * @param MyTIMx 结构体指针，表示要配置的 TIM 定时器
 * @param MyARR 修改后的值
 * @retval 无
 */
void MyTIM_SetARR(MyTIM * MyTIMx, uint16_t MyARR)
{
    TIM_SetAutoreload(MyTIMx->TIMx, MyARR);
}
