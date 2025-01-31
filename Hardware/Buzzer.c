#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTIM.h"
#include "MyPWM.h"
#include "frame.h"
#include "Buzzer.h"

void Buzzer1_On(void);
void Buzzer2_On(void);
void Buzzer1_Off(void);
void Buzzer2_Off(void);
void Buzzer1_SetFreq(uint16_t freq);
void Buzzer2_SetFreq(uint16_t freq);
void Buzzer1_SetVolume(uint8_t volume);
void Buzzer2_SetVolume(uint8_t volume);


MyBuzzer Buzzer1 = {Buzzer1_On, Buzzer1_Off, Buzzer1_SetFreq, Buzzer1_SetVolume, 0, 100, 1};
MyBuzzer Buzzer2 = {Buzzer2_On, Buzzer2_Off, Buzzer2_SetFreq, Buzzer2_SetVolume, 0, 100, 1};

uint16_t FreqMap[] = {523, 587, 659, 698, 784, 880, 988, 1046};
uint8_t VolumeMap[] = {0, 1, 3, 6, 10, 15};

/**
 * @brief 初始化蜂鸣器驱动电路
 */
void Buzzer_Init(void)
{
    MyGPIO_Init(&BUZZER1_GPIO, BUZZER1_PIN, GPIO_Mode_AF_OD, GPIO_Speed_50MHz);
    MyGPIO_Init(&BUZZER2_GPIO, BUZZER2_PIN, GPIO_Mode_AF_OD, GPIO_Speed_50MHz);
    MyTIM_Init_Internal(&BUZZER1_TIM, 100 - 1, 720 - 1);
    MyTIM_Init_Internal(&BUZZER2_TIM, 100 - 1, 720 - 1);
    MyPWM_OCInit(&BUZZER1_TIM, &BUZZER1_CH, TIM_OCPolarity_Low);
    MyPWM_OCInit(&BUZZER2_TIM, &BUZZER2_CH, TIM_OCPolarity_Low);
    Buzzer_SetVolume(1);
    Buzzer1.Off();
    Buzzer2.Off();
}

void Buzzer1_On(void)
{
    MyPWM_SetCCR(&BUZZER1_TIM, &BUZZER1_CH, Buzzer1.volume);
    Buzzer1.active = 1;
}

void Buzzer2_On(void)
{
    MyPWM_SetCCR(&BUZZER2_TIM, &BUZZER2_CH, Buzzer2.volume);
    Buzzer2.active = 1;
}

void Buzzer1_Off(void)
{
    MyPWM_SetCCR(&BUZZER1_TIM, &BUZZER1_CH, 0);
    Buzzer1.active = 0;
}

void Buzzer2_Off(void)
{
    MyPWM_SetCCR(&BUZZER2_TIM, &BUZZER2_CH, 0);
    Buzzer2.active = 0;
}

void Buzzer1_SetFreq(uint16_t freq)
{
    Buzzer1.freq = freq;
    MyTIM_SetPSC(&BUZZER1_TIM, 720000 / freq - 1, TIM_PSCReloadMode_Update);
}

void Buzzer2_SetFreq(uint16_t freq)
{
    Buzzer2.freq = freq;
    MyTIM_SetPSC(&BUZZER2_TIM, 720000 / freq - 1, TIM_PSCReloadMode_Update);
}

void Buzzer1_SetVolume(uint8_t volume)
{
    Buzzer1.volume = volume;
    MyPWM_SetCCR(&BUZZER1_TIM, &BUZZER1_CH, volume);
}

void Buzzer2_SetVolume(uint8_t volume)
{
    Buzzer2.volume = volume;
    MyPWM_SetCCR(&BUZZER2_TIM, &BUZZER2_CH, volume);
}


// ====== 以下是控制蜂鸣器的顶层接口，无需指定哪一个蜂鸣器 ======


/**
 * @brief 设置音量（两个蜂鸣器一起）
 * @param value 音量档位，0 ~ 5
 */
void Buzzer_SetVolume(uint8_t value)
{
    Buzzer1.SetVolume(VolumeMap[value]);
    Buzzer2.SetVolume(VolumeMap[value]);
}

MyBuzzer * last_played = NULL;    // 记录上一次播放使用的蜂鸣器

/**
 * @brief 播放单个音符
 * @param note 音符编号，0 ~ 7
 */
void Buzzer_Play(uint8_t note)
{
    if (Buzzer1.active == 0)    // 蜂鸣器1 空闲
    {
        Buzzer1.SetFreq(FreqMap[note]);
        Buzzer1.On();
        last_played = &Buzzer1;
    }
    else if (Buzzer2.active == 0)    // 蜂鸣器1 不空闲但是 蜂鸣器2 空闲
    {
        Buzzer2.SetFreq(FreqMap[note]);
        Buzzer2.On();
        last_played = &Buzzer2;
    }
    else if (last_played == &Buzzer1)    // 都不空闲且上一个播放的是 蜂鸣器1
    {
        Buzzer2.SetFreq(FreqMap[note]);
        last_played = &Buzzer2;
    }
    else    // 都不空闲且上一个播放的是 蜂鸣器2
    {
        Buzzer1.SetFreq(FreqMap[note]);
        last_played = &Buzzer1;
    }
}

/**
 * @brief 如果存在播放指定音符的蜂鸣器,则关闭
 * @param note 音符编号，0 ~ 7
 */
void Buzzer_UnPlay(uint8_t note)
{
    if (Buzzer1.freq == FreqMap[note])
    {
        Buzzer1.Off();
        if (last_played == &Buzzer1)
            if (Buzzer2.active == 0) last_played = NULL;
            else last_played = &Buzzer2;
    }
    else if (Buzzer2.freq == FreqMap[note])
    {
        Buzzer2.Off();
        if (last_played == &Buzzer2)
            if (Buzzer1.active == 0) last_played = NULL;
            else last_played = &Buzzer1;
    }
}
