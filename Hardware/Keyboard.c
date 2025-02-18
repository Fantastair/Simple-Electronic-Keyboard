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

MyGPIO *Keyboard_GPIO[8];
uint16_t Keyboard_Pin[8] = {KEYBOARD_R1_PIN, KEYBOARD_R2_PIN, KEYBOARD_R3_PIN, KEYBOARD_R4_PIN, KEYBOARD_C1_PIN, KEYBOARD_C2_PIN, KEYBOARD_C3_PIN, KEYBOARD_C4_PIN};
uint32_t last_update_time;    // 存储上一次更新输入的时间
uint16_t keyboard_input_temp = 0;    // 存储矩阵键盘输入状态
/**
 * @brief 初始化矩阵键盘
 */
void Keyboard_Init(void)
{
    uint8_t i;

    Keyboard_GPIO[0] = &KEYBOARD_R1_GPIO;
    Keyboard_GPIO[1] = &KEYBOARD_R2_GPIO;
    Keyboard_GPIO[2] = &KEYBOARD_R3_GPIO;
    Keyboard_GPIO[3] = &KEYBOARD_R4_GPIO;
    Keyboard_GPIO[4] = &KEYBOARD_C1_GPIO;
    Keyboard_GPIO[5] = &KEYBOARD_C2_GPIO;
    Keyboard_GPIO[6] = &KEYBOARD_C3_GPIO;
    Keyboard_GPIO[7] = &KEYBOARD_C4_GPIO;
    
    for (i = 0; i < 4; i++)
    {
        MyGPIO_Init(Keyboard_GPIO[i], Keyboard_Pin[i], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
        GPIO_ResetBits(Keyboard_GPIO[i]->GPIOx, Keyboard_Pin[i]);
        MyGPIO_Init(Keyboard_GPIO[i + 4], Keyboard_Pin[i + 4], GPIO_Mode_IPD, GPIO_Speed_50MHz);
    }
    
    ApplyTickFunc(Keyboard_UpdateInput, 0);
    last_update_time = MyTime_GetTick();
}

/** 
 * @brief 更新矩阵键盘的输入
 */
void Keyboard_UpdateInput(uint16_t tick)
{
    uint8_t i, j;
    uint16_t result;
    uint32_t t = MyTime_GetTick();

    if (t - last_update_time >= 6)
    {
        last_update_time = t;
        result = 0;
        for (i = 0; i < 4; i ++)
        {
            GPIO_SetBits(Keyboard_GPIO[i]->GPIOx, Keyboard_Pin[i]);
            for (j = 0; j < 4; j ++)
            {
                result <<= 1;
                result += GPIO_ReadInputDataBit(Keyboard_GPIO[j + 4]->GPIOx, Keyboard_Pin[j + 4]);
            }
            GPIO_ResetBits(Keyboard_GPIO[i]->GPIOx, Keyboard_Pin[i]);
        }
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
