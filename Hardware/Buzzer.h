#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"
#include "MyTIM.h"
#include "frame.h"

#define BUZZER1_GPIO MyGPIOA
#define BUZZER2_GPIO MyGPIOA

#define BUZZER1_PIN GPIO_Pin_0
#define BUZZER2_PIN GPIO_Pin_6

#define BUZZER1_TIM MyTIM2
#define BUZZER2_TIM MyTIM3

#define BUZZER1_CH MyTIM_CH1
#define BUZZER2_CH MyTIM_CH1

#define BUZZERSMOOTHONTICK 4
#define BUZZERSMOOTHOFFTICK 18

typedef struct
{
    void (*On)(void);    // 硬开启蜂鸣器
    void (*Off)(void);    // 硬关闭蜂鸣器
    void (*SetFreq)(uint16_t freq);    // 设置蜂鸣器发声频率
    void (*SetVolume)(uint8_t volume);    // 设置蜂鸣器音量
    void (*SmoothOff)(uint16_t time);    // 软关闭蜂鸣器（淡出）
    uint8_t active;    // 记录蜂鸣器状态，0 关闭，1 开启，2 淡出
    uint8_t volume;    // 音量，0 ~ 15
    uint16_t freq;    // 发声频率
    TickFunc * smoothoff_tick;    // 淡出帧函数
} MyBuzzer;    // 蜂鸣器类

extern MyBuzzer Buzzer1, Buzzer2;


void Buzzer_Init(void);

void Buzzer_SetVolume(uint8_t value);
uint8_t Buzzer_GetVolume(void);
void Buzzer_HighVolume(void);
void Buzzer_LowerVolume(void);

void Buzzer_Play(uint8_t note);
void Buzzer_UnPlay(uint8_t note);
void Buzzer_SmoothOn(void);
void Buzzer_SmoothOff(void);

#endif
