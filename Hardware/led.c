#include "stm32f10x.h"
#include "led.h"
#include "MyGPIO.h"

uint16_t LED_PIN[8] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN, LED6_PIN, LED7_PIN, LED8_PIN};
MyGPIO *LED_GPIO[8] = {&LED1_GPIO, &LED2_GPIO, &LED3_GPIO, &LED4_GPIO, &LED5_GPIO, &LED6_GPIO, &LED7_GPIO, &LED8_GPIO};

/**
 * @brief  初始化 LED 灯列控制模块
 */
void LED_Init(void)
{
    uint8_t i;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    for (i = 0; i < 8; i ++)
    {
        MyGPIO_Init(LED_GPIO[i], LED_PIN[i], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
        GPIO_WriteBit(LED_GPIO[i]->GPIOx, LED_PIN[i], Bit_RESET);
    }

    MyGPIO_Init(&LED0_GPIO, LED0_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    LED_IndicateOn();
}

/**
 * @brief 打开指定编号的 LED 灯
 * @param num LED 灯编号，0 ~ 7
 */
void LED_On(uint8_t num)
{
    GPIO_WriteBit(LED_GPIO[num]->GPIOx, LED_PIN[num], Bit_SET);
}

/**
 * @brief 关闭指定编号的 LED 灯
 * @param num LED 灯编号，0 ~ 7
 */
void LED_Off(uint8_t num)
{
    GPIO_WriteBit(LED_GPIO[num]->GPIOx, LED_PIN[num], Bit_RESET);
}

/**
 * @brief 判断指定编号的 LED 灯是否打开
 * @param num LED 灯编号，0 ~ 7
 * @retval 1 表示打开，0 表示关闭
 */
uint8_t LED_IsOn(uint8_t num)
{
    return GPIO_ReadOutputDataBit(LED_GPIO[num]->GPIOx, LED_PIN[num]);
}

/**
 * @brief 开启指示灯
 */
void LED_IndicateOn(void)
{
    GPIO_WriteBit(LED0_GPIO.GPIOx, LED0_PIN, Bit_RESET);
}

/**
 * @brief 关闭指示灯
 */
void LED_IndicateOff(void)
{
    GPIO_WriteBit(LED0_GPIO.GPIOx, LED0_PIN, Bit_SET);
}
