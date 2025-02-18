#include "stm32f10x.h"
#include "MyGPIO.h"

MyGPIO MyGPIOA = {GPIOA, RCC_APB2Periph_GPIOA};
MyGPIO MyGPIOB = {GPIOB, RCC_APB2Periph_GPIOB};
MyGPIO MyGPIOC = {GPIOC, RCC_APB2Periph_GPIOC};

/**
  * @brief 初始化 GPIO 的特定端口
  * @param MyGPIOx 指定 GPIO，x 可以为 A ~ G
  * @param GPIO_Pin_x 指定端口号，x 可以为 0 ~ 15
  * @param GPIO_Mode_x 指定端口模式，可以为 GPIOMode_TypeDef 中的一个值
  * @param GPIO_Speed_xMHz 指定端口速度，可以为 GPIOSpeed_TypeDef 中的一个值（x 可以为 2、10、50）
  * @retval 无
  */
void MyGPIO_Init(MyGPIO* MyGPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode_x, GPIOSpeed_TypeDef GPIO_Speed_xMHz)
{
	RCC_APB2PeriphClockCmd(MyGPIOx->RCC_APB2Periph_GPIOx, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_xMHz;   
    GPIO_Init(MyGPIOx->GPIOx, &GPIO_InitStructure);
}
