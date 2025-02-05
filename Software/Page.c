#include "stm32f10x.h"

#include "MyOLED.h"
#include "MyOLED_Render.h"

#include "Page.h"
#include "MyTime.h"
#include "Keyboard.h"
#include "MyMisc.h"
#include "Buzzer.h"

#include "ChineseFont.h"

void blankfunc1(void) {}    // 空函数，占位用
void blankfunc2(uint8_t a) {}    // 空函数，占位用


void WelcomePage_Init(void);
void WelcomePage_Render(void);
void MainPage_Init(void);
void MainPage_Render(void);
void MainPage_ButtonDown(uint8_t num);
void MainPage_ButtonUp(uint8_t num);
void AboutPage_Init(void);
void AboutPage_ButtonDown(uint8_t num);
void AboutPage_ButtonUp(uint8_t num);

Page WelcomePage = {WelcomePage_Init, WelcomePage_Render, blankfunc2, blankfunc2};
Page MainPage = {MainPage_Init, MainPage_Render, MainPage_ButtonDown, MainPage_ButtonUp};
Page AboutPage = {AboutPage_Init, blankfunc1, AboutPage_ButtonDown, AboutPage_ButtonUp};

Page *PageList[2];    // 菜单切换界面列表
int8_t PageListIndex;    // 菜单切换界面列表索引
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

/**
 * @brief 切换到下一个界面
 */
void NextPage(void)
{
    PageListIndex ++;
    if (PageListIndex >= sizeof(PageList) / sizeof(PageList[0]))
    {
        PageListIndex = 0;
    }
    SetPage(PageList[PageListIndex]);
}

/**
 * @brief 切换到上一个界面
 */
void PrevPage(void)
{
    PageListIndex --;
    if (PageListIndex < 0)
    {
        PageListIndex = sizeof(PageList) / sizeof(PageList[0]) - 1;
    }
    SetPage(PageList[PageListIndex]);
}

void Page_Init(void)
{
    PageList[0] = &MainPage;
    PageList[1] = &AboutPage;

    // SetPage(&AboutPage);
    // SetPage(&MainPage);
    SetPage(&WelcomePage);
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

/**
 * @brief 绘制音量条
 */
void MainPage_DrawVolumeBar(void)
{
    uint8_t i, j = Buzzer_GetVolume();

    MyOLED_Clear_GRAM_Rect(44, 4, 40, 4);
    MyOLED_Fill_GRAM_Rect(44, 4, 8 * j, 4);
    for (i = 1; i < j; i ++)
    {
        MyOLED_Clear_GRAM_Rect(43 + 8 * i, 4, 2, 4);
    }
    MyOLED_Update(44, 0, 40, 1);
}

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
    MyOLED_Fill_GRAM_Rect(44, 11, 40, 2);
    MyOLED_Fill_GRAM_Rect(44, 11, 2, 24);
    MyOLED_Fill_GRAM_Rect(44, 33, 40, 2);
    MyOLED_Fill_GRAM_Rect(82, 11, 2, 24);
    MyOLED_Fill_GRAM_Rect(63, 11, 2, 24);
    MyOLED_Fill_GRAM_Rect(50, 22, 8, 4);
    MyOLED_Fill_GRAM_Rect(70, 22, 8, 4);
    MyOLED_DrawCircle(20, 20, 15, 2);
    MyOLED_DrawCircle(108, 20, 15, 2);
    Buzzer_SetVolume(5);
    MainPage_DrawVolumeBar();
    MyOLED_Flip();
}

float ani_temp1 = 0;
float ani_temp2 = 0.1;
float ani_temp3 = 0;
float ani_temp4 = 0.1;
void MainPage_Render(void)
{
    if (Buzzer1.active == 1 && Buzzer1.volume > 0)
    {
        ani_temp3 += ani_temp4;
        MyOLED_Clear_GRAM_Rect(96, 8, 24, 24);
        MyOLED_DrawCircle(108, 20, 15, 2);
        MyOLED_DrawCircle(108, 20, (uint8_t)ani_temp3, 1);
        MyOLED_Update(96, 1, 24, 3);
        if (ani_temp3 >= 8) ani_temp4 = -0.1;
        else if (ani_temp3 <= 0) ani_temp4 = 0.1;
    }
    if (Buzzer2.active == 1 && Buzzer2.volume > 0)
    {
        ani_temp1 += ani_temp2;
        MyOLED_Clear_GRAM_Rect(8, 8, 24, 24);
        MyOLED_DrawCircle(20, 20, 15, 2);
        MyOLED_DrawCircle(20, 20, (uint8_t)ani_temp1, 1);
        MyOLED_Update(8, 1, 24, 3);
        if (ani_temp1 >= 8) ani_temp2 = -0.1;
        else if (ani_temp1 <= 0) ani_temp2 = 0.1;
    }
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
    else if (num == 12)    // 音量 +
    {
        Buzzer_HighVolume();
        MainPage_DrawVolumeBar();
    }
    else if (num == 13)    // 音量 -
    {
        Buzzer_LowerVolume();
        MainPage_DrawVolumeBar();
    }
    else if (num == 14)    // 下一个界面
    {
        NextPage();
    }
    else if (num == 15)    // 上一个界面
    {
        PrevPage();
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


// ============= 关于 =============

/*
关于 简易电子琴
V0.7.1
不会弹琴的电阻
*/

uint8_t gy[] = {0, 1};
uint8_t jydzq[] = {2, 3, 4, 5, 6};
uint8_t bhtqddz[] = {7, 8, 9, 6, 10, 4, 11};

void AboutPage_Init(void)
{
    uint8_t i;

    MyOLED_Fill_GRAM_Rect(0, 0, 128, 64);
    MyOLED_Clear_GRAM_Rect(1, 1, 126, 62);

    for (i = 0; i < 2; i ++)
    {
        MyOLED_Blit_GRAM(ChineseFont[gy[i] * 2], 4 + i * 16, 4, 16);
        MyOLED_Blit_GRAM(ChineseFont[gy[i] * 2 + 1], 4 + i * 16, 4 + 8, 16);
    }
    for (i = 0; i < 5; i ++)
    {
        MyOLED_Blit_GRAM(ChineseFont[jydzq[i] * 2], 44 + i * 16, 4, 16);
        MyOLED_Blit_GRAM(ChineseFont[jydzq[i] * 2 + 1], 44 + i * 16, 4 + 8, 16);
    }
    MyOLED_Blit_String_ASCII_8x16(8, 24, "V0.7.2");
    for (i = 0; i < 7; i ++)
    {
        MyOLED_Blit_GRAM(ChineseFont[bhtqddz[i] * 2], 4 + i * 16, 44, 16);
        MyOLED_Blit_GRAM(ChineseFont[bhtqddz[i] * 2 + 1], 4 + i * 16, 44 + 8, 16);
    }

    MyOLED_Flip();
}

void AboutPage_ButtonDown(uint8_t num)
{
    if (num < 8)
    {
        Buzzer_Play(num);
    }
    else if (num == 12)    // 音量 +
    {
        Buzzer_HighVolume();
    }
    else if (num == 13)    // 音量 -
    {
        Buzzer_LowerVolume();
    }
    else if (num == 14)    // 下一个界面
    {
        NextPage();
    }
    else if (num == 15)    // 上一个界面
    {
        PrevPage();
    }
}

void AboutPage_ButtonUp(uint8_t num)
{
    if (num < 8)
    {
        Buzzer_UnPlay(num);
    }
}
