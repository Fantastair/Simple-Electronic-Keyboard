#include "stm32f10x.h"

/**
  * @brief 初始化NVIC
  *   使用NVIC前，需要先配置优先级分组：NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);    //x 为 0 ~ 4
  *   整个程序只能配置一次优先级分组
  * @param Channel 指定中断通道
  * @param GPIO_PinSourcex 通过 AFIO 连接的 GPIO 线路，x 为 0 ~ 15
  * @param PreemptionPriority 抢占优先级，可以为 0 ~ 15
  * @param SubPriority 响应优先级，可以为 0 ~ 15
  * @param  指定外部中断的触发边沿，x 可以为 Rising、Falling 或 Raising_Falling
  * @retval 无
  **/
void MyNVIC_Init(uint8_t Channel, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Channel;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_Init(&NVIC_InitStructure);
}
