#include <stddef.h>
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTIM.h"
#include "MyPWM.h"
#include "frame.h"
#include "MyTime.h"
#include "Buzzer.h"
#include "MyMisc.h"
#include "MyOLED_Render.h"
#include "Page.h"

void Buzzer1_On(void);
void Buzzer2_On(void);
void Buzzer1_Off(void);
void Buzzer2_Off(void);
void Buzzer1_SetFreq(uint16_t freq);
void Buzzer2_SetFreq(uint16_t freq);
void Buzzer1_SetVolume(uint8_t volume);
void Buzzer2_SetVolume(uint8_t volume);
void Buzzer1_SmoothOff_TickFunc(uint16_t tick);
void Buzzer2_SmoothOff_TickFunc(uint16_t tick);
void Buzzer1_SmoothOff(uint16_t time);
void Buzzer2_SmoothOff(uint16_t time);

MyBuzzer Buzzer1 = {Buzzer1_On, Buzzer1_Off, Buzzer1_SetFreq, Buzzer1_SetVolume, Buzzer1_SmoothOff, 0, 100, 1};
MyBuzzer Buzzer2 = {Buzzer2_On, Buzzer2_Off, Buzzer2_SetFreq, Buzzer2_SetVolume, Buzzer2_SmoothOff, 0, 100, 1};

uint16_t FreqMap[] = {523, 587, 659, 698, 784, 880, 988, 1046};
uint8_t VolumeMap[] = {0, 1, 3, 6, 10, 15};

void LeftVolumeBar_Ani(uint16_t tick);
void RightVolumeBar_Ani(uint16_t tick);

uint32_t lvba_tick;
uint32_t lvba_total;
uint8_t lvba_target;
uint8_t lvba_start = 0;
uint8_t lvba_temp = 0;
TickFunc * lvba_tf;

uint32_t rvba_tick;
uint32_t rvba_total;
uint8_t rvba_target;
uint8_t rvba_start = 0;
uint8_t rvba_temp = 0;
TickFunc * rvba_tf;

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
    Buzzer1.smoothoff_tick = CreateTickFunc(Buzzer1_SmoothOff_TickFunc, 0, False);
    Buzzer2.smoothoff_tick = CreateTickFunc(Buzzer2_SmoothOff_TickFunc, 0, False);
    lvba_tf = CreateTickFunc(LeftVolumeBar_Ani, 0, False);
    rvba_tf = CreateTickFunc(RightVolumeBar_Ani, 0, False);
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

uint32_t Buzzer1TickTemp;    // 记录帧函数开始的时刻
uint32_t Buzzer1TotalTemp;    // 记录帧函数持续时刻
/**
 * @brief 蜂鸣器1 淡出帧函数的执行函数
 */
void Buzzer1_SmoothOff_TickFunc(uint16_t tick)
{
    uint32_t t = MyTime_GetTick() - Buzzer1TickTemp;
    if (t > Buzzer1TotalTemp)
    {
        Buzzer1_Off();
        StopTickFunc(Buzzer1.smoothoff_tick);
    }
    else
    {
        MyPWM_SetCCR(&BUZZER1_TIM, &BUZZER1_CH, (uint16_t)(Buzzer1.volume * (1.0 - (float)t / Buzzer1TotalTemp)));
    }
}

uint32_t Buzzer2TickTemp;    // 记录帧函数开始的时刻
uint32_t Buzzer2TotalTemp;    // 记录帧函数持续时刻
/**
 * @brief 蜂鸣器2 淡出帧函数的执行函数
 */
void Buzzer2_SmoothOff_TickFunc(uint16_t tick)
{
    uint32_t t = MyTime_GetTick() - Buzzer2TickTemp;
    if (t > Buzzer2TotalTemp)
    {
        Buzzer2_Off();
        StopTickFunc(Buzzer2.smoothoff_tick);
    }
    else
    {
        MyPWM_SetCCR(&BUZZER2_TIM, &BUZZER2_CH, Buzzer2.volume * (1.0 - (float)t / Buzzer2TotalTemp));
    }
}

/**
 * @brief 蜂鸣器1 的淡出控制
 * @param time 淡出持续时刻（程序时刻）
 */
void Buzzer1_SmoothOff(uint16_t time)
{
    if (Buzzer1.active == 1)
    {
        Buzzer1.active = 2;
        LaunchTickFunc(Buzzer1.smoothoff_tick);
        Buzzer1TotalTemp = time;
        Buzzer1TickTemp = MyTime_GetTick();
    }
}

/**
 * @brief 蜂鸣器2 的淡出控制
 * @param time 淡出持续时刻（程序时刻）
 */
void Buzzer2_SmoothOff(uint16_t time)
{
    if (Buzzer2.active == 1)
    {
        Buzzer2.active = 2;
        LaunchTickFunc(Buzzer2.smoothoff_tick);
        Buzzer2TotalTemp = time;
        Buzzer2TickTemp = MyTime_GetTick();
    }
}


/**
 * @brief 设置音量（两个蜂鸣器一起）
 * @param value 音量档位，0 ~ 5
 */
void Buzzer_SetVolume(uint8_t value)
{
    Buzzer1.SetVolume(VolumeMap[value]);
    Buzzer2.SetVolume(VolumeMap[value]);
    Buzzer1.Off();
    Buzzer2.Off();
}

/**
 * @brief 获取音量
 * @retval 音量挡位，0 ~ 5
 */
uint8_t Buzzer_GetVolume(void)
{
    int8_t i;

    if (Buzzer2.volume != Buzzer1.volume) Buzzer2.SetVolume(Buzzer1.volume);

    for (i = 5; i >= 0; i --)
    {
        if (Buzzer1.volume >= VolumeMap[i]) return i;
    }
    return 0;
}

/**
 * @brief 提升一档音量
 */
void Buzzer_HighVolume(void)
{
    uint8_t value = Buzzer_GetVolume() + 1;
    if (value > 5) value = 5;
    Buzzer_SetVolume(value);
}

/**
 * @brief 降低一档音量
 */
void Buzzer_LowerVolume(void)
{
    int8_t value = Buzzer_GetVolume() - 1;
    if (value < 0) value = 0;
    Buzzer_SetVolume(value);
}

MyBuzzer * last_played = NULL;    // 记录上一次播放使用的蜂鸣器
/**
 * @brief 播放单个音符
 * @param note 音符编号，0 ~ 7
 */
void Buzzer_Play(uint8_t note)
{
    uint8_t flag;

    if (Buzzer1.active == 0)    // 蜂鸣器1 空闲
    {
        Buzzer1.SetFreq(FreqMap[note]);
        Buzzer1.On();
        last_played = &Buzzer1;
        flag = 0;
    }
    else if (Buzzer2.active == 0)    // 蜂鸣器1 不空闲但是 蜂鸣器2 空闲
    {
        Buzzer2.SetFreq(FreqMap[note]);
        Buzzer2.On();
        last_played = &Buzzer2;
        flag = 1;
    }
    else if (Buzzer1.active == 2)    // 都不空闲但是 蜂鸣器1 正在淡出
    {
        StopTickFunc(Buzzer1.smoothoff_tick);
        Buzzer1.SetFreq(FreqMap[note]);
        Buzzer1.On();
        last_played = &Buzzer1;
        flag = 0;
    }
    else if (Buzzer2.active == 2)    // 都不空闲但是 蜂鸣器2 正在淡出
    {
        StopTickFunc(Buzzer2.smoothoff_tick);
        Buzzer2.SetFreq(FreqMap[note]);
        Buzzer2.On();
        last_played = &Buzzer2;
        flag = 1;
    }
    else if (last_played == &Buzzer1)    // 都不空闲且上一个播放的是 蜂鸣器1
    {
        Buzzer2.SetFreq(FreqMap[note]);
        last_played = &Buzzer2;
        flag = 1;
    }
    else    // 都不空闲且上一个播放的是 蜂鸣器2
    {
        Buzzer1.SetFreq(FreqMap[note]);
        last_played = &Buzzer1;
        flag = 0;
    }
    if (flag == 0 && Buzzer2.volume > 0)
    {
        if (current_page == &MainPage)
        {
            MyOLED_Clear_GRAM_Rect(66, 13, 16, 20);
            MyMisc_DrawNoteBig(68, 13, note);
            MyOLED_Update(66, 1, 16, 3);
        }
        rvba_start = rvba_temp;
        rvba_total = BUZZERSMOOTHONTICK;
        rvba_target = Buzzer_GetVolume() * 18 / 5;
        LaunchTickFunc(rvba_tf);
        rvba_tick = MyTime_GetTick();
    }
    else if (flag == 1 && Buzzer1.volume > 0)
    {
        if (current_page == &MainPage)
        {
            MyOLED_Clear_GRAM_Rect(46, 13, 17, 20);
            MyMisc_DrawNoteBig(48, 13, note);
            MyOLED_Update(46, 1, 17, 3);
        }
        lvba_start = lvba_temp;
        lvba_total = BUZZERSMOOTHONTICK;
        lvba_target = Buzzer_GetVolume() * 18 / 5;
        LaunchTickFunc(lvba_tf);
        lvba_tick = MyTime_GetTick();
    }
}

/**
 * @brief 如果存在播放指定音符的蜂鸣器,则关闭
 * @param note 音符编号，0 ~ 7
 */
void Buzzer_UnPlay(uint8_t note)
{
    if (Buzzer1.active == 1 && Buzzer1.freq == FreqMap[note])
    {
        Buzzer1.SmoothOff(BUZZERSMOOTHOFFTICK);
        if (last_played == &Buzzer1)
            if (Buzzer2.active == 0) last_played = NULL;
            else last_played = &Buzzer2;
        if (current_page == &MainPage)
        {
            MyOLED_Clear_GRAM_Rect(66, 13, 16, 20);
            MyOLED_Fill_GRAM_Rect(70, 22, 8, 4);
            MyOLED_Update(66, 1, 16, 3);
        }
        rvba_start = rvba_temp;
        rvba_total = BUZZERSMOOTHOFFTICK;
        rvba_target = 0;
        LaunchTickFunc(rvba_tf);
        rvba_tick = MyTime_GetTick();
    }
    else if (Buzzer2.active == 1 && Buzzer2.freq == FreqMap[note])
    {
        Buzzer2.SmoothOff(BUZZERSMOOTHOFFTICK);
        if (last_played == &Buzzer2)
            if (Buzzer1.active == 0) last_played = NULL;
            else last_played = &Buzzer1;
        if (current_page == &MainPage)
        {
            MyOLED_Clear_GRAM_Rect(46, 13, 17, 20);
            MyOLED_Fill_GRAM_Rect(50, 22, 8, 4);
            MyOLED_Update(46, 1, 17, 3);
        }
        lvba_start = lvba_temp;
        lvba_total = 24;
        lvba_target = 0;
        LaunchTickFunc(lvba_tf);
        lvba_tick = MyTime_GetTick();
    }
}


void LeftVolumeBar_Ani(uint16_t tick)
{
    uint32_t t = MyTime_GetTick() - lvba_tick;
    uint8_t l;

    if (t > lvba_total)
    {
        StopTickFunc(lvba_tf);
        lvba_start = l = lvba_target;
    }
    else
    {
        l = (uint8_t)(lvba_start + (float)(lvba_target - lvba_start) * (float)t / lvba_total);
    }
    lvba_temp = l;
    if (current_page == &MainPage)
    {
        MyOLED_Clear_GRAM_Rect(3, 43, 12, 20);
        if (l > 0) MyOLED_Fill_GRAM_Rect(2, 61 - l, 11, l);
        MyOLED_Update(3, 5, 12, 3);
    }
}

void RightVolumeBar_Ani(uint16_t tick)
{
    uint32_t t = MyTime_GetTick() - rvba_tick;
    uint8_t l;

    if (t > rvba_total)
    {
        StopTickFunc(rvba_tf);
        rvba_start = l = rvba_target;
    }
    else
    {
        l = (uint8_t)(rvba_start + (float)(rvba_target - rvba_start) * (float)t / rvba_total);
    }
    rvba_temp = l;
    if (current_page == &MainPage)
    {
        MyOLED_Clear_GRAM_Rect(115, 43, 12, 20);
        if (l > 0) MyOLED_Fill_GRAM_Rect(114, 61 - l, 11, l);
        MyOLED_Update(115, 5, 12, 3);
    }
}
