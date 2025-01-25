#include "stm32f10x.h"
#include "Page.h"

#include "MyOLED.h"
#include "MyOLED_Render.h"
#include "MyTime.h"

void blankfunc1(void) {}    // 空函数，占位用
void blankfunc2(uint8_t a) {}    // 空函数，占位用


void WelcomePage_Init(void);
void WelcomePage_Render(void);
Page WelcomePage = {WelcomePage_Init, WelcomePage_Render, blankfunc2, blankfunc2};


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
    SetPage(&WelcomePage);
}


// ============= 开场界面 =============

uint16_t WelcomePage_temp;

void WelcomePage_Init(void)
{
    MyOLED_Blit_Icon_64x64(32);
    MyOLED_Update(32, 0, 64, 8);
    WelcomePage_temp = MyTime_GetTick();
}

void WelcomePage_Render(void)
{
    if ((MyTime_GetTick() - WelcomePage_temp) % 65536 > 3000)
    {
        // SetPage();
    }
}


// ============= 开场界面 =============

void MainPage_Init(void)
{

}

void MainPage_Render(void)
{

}
