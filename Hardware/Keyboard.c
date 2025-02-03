#include "stm32f10x.h"
#include "MyGPIO.h"
#include "Keyboard.h"
#include "frame.h"
#include "MyTime.h"

/*
按键编号与实体按键对应关系：
0 ~ 7：8 个琴键
8、9、10、11：操作按键4、3、2、1
12、13：音量+、-
14、15：菜单-、+
*/

uint32_t last_update_time;    // 存储上一次更新输入的时间
uint16_t keyboard_input_temp = 0;    // 存储矩阵键盘输入状态
/**
 * @brief 初始化矩阵键盘
 */
void Keyboard_Init(void)
{
    MyGPIO_Init(&KEYBOARD_R1_GPIO, KEYBOARD_R1_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_R2_GPIO, KEYBOARD_R2_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_R3_GPIO, KEYBOARD_R3_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_R4_GPIO, KEYBOARD_R4_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_C1_GPIO, KEYBOARD_C1_PIN, GPIO_Mode_IPD, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_C2_GPIO, KEYBOARD_C2_PIN, GPIO_Mode_IPD, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_C3_GPIO, KEYBOARD_C3_PIN, GPIO_Mode_IPD, GPIO_Speed_50MHz);
    MyGPIO_Init(&KEYBOARD_C4_GPIO, KEYBOARD_C4_PIN, GPIO_Mode_IPD, GPIO_Speed_50MHz);

    GPIO_ResetBits(KEYBOARD_R1_GPIO.GPIOx, KEYBOARD_R1_PIN);
    GPIO_ResetBits(KEYBOARD_R2_GPIO.GPIOx, KEYBOARD_R2_PIN);
    GPIO_ResetBits(KEYBOARD_R3_GPIO.GPIOx, KEYBOARD_R3_PIN);
    GPIO_ResetBits(KEYBOARD_R4_GPIO.GPIOx, KEYBOARD_R4_PIN);

    ApplyTickFunc(Keyboard_UpdateInput, 0);
    last_update_time = MyTime_GetTick();
}


/** 
 * @brief 更新矩阵键盘的输入
 */
void Keyboard_UpdateInput(uint16_t result)
{
    result = 0;
    uint32_t t = MyTime_GetTick();

    if (t - last_update_time >= 4)
    {
        last_update_time = t;
        GPIO_SetBits(KEYBOARD_R1_GPIO.GPIOx, KEYBOARD_R1_PIN);    // 读取第一行
        result += GPIO_ReadInputDataBit(KEYBOARD_C1_GPIO.GPIOx, KEYBOARD_C1_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C2_GPIO.GPIOx, KEYBOARD_C2_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C3_GPIO.GPIOx, KEYBOARD_C3_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C4_GPIO.GPIOx, KEYBOARD_C4_PIN);
        result <<= 1;
        GPIO_ResetBits(KEYBOARD_R1_GPIO.GPIOx, KEYBOARD_R1_PIN);
        GPIO_SetBits(KEYBOARD_R2_GPIO.GPIOx, KEYBOARD_R2_PIN);    // 读取第二行
        result += GPIO_ReadInputDataBit(KEYBOARD_C1_GPIO.GPIOx, KEYBOARD_C1_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C2_GPIO.GPIOx, KEYBOARD_C2_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C3_GPIO.GPIOx, KEYBOARD_C3_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C4_GPIO.GPIOx, KEYBOARD_C4_PIN);
        result <<= 1;
        GPIO_ResetBits(KEYBOARD_R2_GPIO.GPIOx, KEYBOARD_R2_PIN);
        GPIO_SetBits(KEYBOARD_R3_GPIO.GPIOx, KEYBOARD_R3_PIN);    // 读取第三行
        result += GPIO_ReadInputDataBit(KEYBOARD_C1_GPIO.GPIOx, KEYBOARD_C1_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C2_GPIO.GPIOx, KEYBOARD_C2_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C3_GPIO.GPIOx, KEYBOARD_C3_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C4_GPIO.GPIOx, KEYBOARD_C4_PIN);
        result <<= 1;
        GPIO_ResetBits(KEYBOARD_R3_GPIO.GPIOx, KEYBOARD_R3_PIN);
        GPIO_SetBits(KEYBOARD_R4_GPIO.GPIOx, KEYBOARD_R4_PIN);    // 读取第四行
        result += GPIO_ReadInputDataBit(KEYBOARD_C1_GPIO.GPIOx, KEYBOARD_C1_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C2_GPIO.GPIOx, KEYBOARD_C2_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C3_GPIO.GPIOx, KEYBOARD_C3_PIN);
        result <<= 1;
        result += GPIO_ReadInputDataBit(KEYBOARD_C4_GPIO.GPIOx, KEYBOARD_C4_PIN);
        GPIO_ResetBits(KEYBOARD_R4_GPIO.GPIOx, KEYBOARD_R4_PIN);
        keyboard_input_temp = result;
    }
}

/**
 * @brief 获取键盘输入
 */
uint16_t Keyboard_GetInput(void)
{
    return keyboard_input_temp;
}
