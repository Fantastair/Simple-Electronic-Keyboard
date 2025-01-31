#include "stm32f10x.h"

#include "MyOLED.h"
#include "MyOLED_Render.h"

#include "Page.h"
#include "MyTime.h"
#include "Keyboard.h"
#include "MyMisc.h"
#include "Buzzer.h"

void blankfunc1(void) {}    // 空函数，占位用
void blankfunc2(uint8_t a) {}    // 空函数，占位用


void WelcomePage_Init(void);
void WelcomePage_Render(void);
void MainPage_Init(void);
void MainPage_Render(void);
void MainPage_ButtonDown(uint8_t num);
void MainPage_ButtonUp(uint8_t num);

Page WelcomePage = {WelcomePage_Init, WelcomePage_Render, blankfunc2, blankfunc2};
Page MainPage = {MainPage_Init, MainPage_Render, MainPage_ButtonDown, MainPage_ButtonUp};


Page *current_page;    // 当前界面函数指针

/**
 * @brief 设置当前界面
 * @param page 界面结构体指针
 */
void SetPage(Page *page)
{
    current_page = page;
    page->PageInit();
}

void Page_Init(void)
{
    SetPage(&MainPage);
    // SetPage(&WelcomePage);
}


// ============= 开场界面 =============

uint16_t WelcomePage_temp;    // 记录延时

void WelcomePage_Init(void)
{
    MyOLED_Clear_GRAM();
    MyOLED_Blit_Icon_64x64(32);
    MyOLED_Flip();
    WelcomePage_temp = MyTime_GetTick();
}

void WelcomePage_Render(void)
{
    if ((MyTime_GetTick() - WelcomePage_temp) > 320)
    {
        SetPage(&MainPage);
    }
}


// ============= 主界面 =============

void MainPage_Init(void)
{
    int i;

    MyOLED_Clear_GRAM();
    MyOLED_Fill_GRAM_Rect(0, 39, 128, 2);
    for (i = 0; i < 8; i ++)
    {
        MyOLED_Fill_GRAM_Rect(16 + i * 12, 41, 1, 23);
        MyOLED_Fill_GRAM_Rect(27 + i * 12, 41, 1, 23);
        MyMisc_DrawNoteSmall(20 + i * 12, 53, i, 1);
    }
    MyOLED_Fill_GRAM_Rect(0, 0, 1, 64);
    MyOLED_Fill_GRAM_Rect(127, 0, 1, 64);
    MyOLED_Fill_GRAM_Rect(15, 41, 1, 23);
    MyOLED_Fill_GRAM_Rect(112, 41, 1, 23);
    MyOLED_Fill_GRAM_Rect(0, 63, 128, 1);
    MyOLED_Fill_GRAM_Rect(0, 0, 128, 1);
    MyOLED_Flip();
}

void MainPage_Render(void)
{

}

void MainPage_ButtonDown(uint8_t num)
{
    if (num < 8)
    {
        MyOLED_Fill_GRAM_Rect(17 + num * 12, 41, 10, 22);
        MyMisc_DrawNoteSmall(20 + num * 12, 50, num, 0);
        MyOLED_Update(17 + num * 12, 5, 10, 3);
        Buzzer_Play(num);
    }
}

void MainPage_ButtonUp(uint8_t num)
{
    if (num < 8)
    {
        MyOLED_Clear_GRAM_Rect(17 + num * 12, 41, 10, 22);
        MyMisc_DrawNoteSmall(20 + num * 12, 53, num, 1);
        MyOLED_Update(17 + num * 12, 5, 10, 3);
        Buzzer_UnPlay(num);
    }
}
