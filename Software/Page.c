#include "stm32f10x.h"
#include <stddef.h>
#include <string.h>
#include "MyOLED.h"
#include "MyOLED_Render.h"
#include "Page.h"
#include "MyTime.h"
#include "Keyboard.h"
#include "MyMisc.h"
#include "Buzzer.h"
#include "led.h"
#include "Music.h"
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
void MusicPlayPage_Init(void);
void MusicPlayPage_Render(void);
void MusicPlayPage_ButtonDown(uint8_t num);
void MusicPlayPage_ButtonUp(uint8_t num);

Page WelcomePage = {WelcomePage_Init, WelcomePage_Render, blankfunc2, blankfunc2};
Page MainPage = {MainPage_Init, MainPage_Render, MainPage_ButtonDown, MainPage_ButtonUp};
Page MusicPlayPage = {MusicPlayPage_Init, MusicPlayPage_Render, MusicPlayPage_ButtonDown, MusicPlayPage_ButtonUp};
Page AboutPage = {AboutPage_Init, blankfunc1, AboutPage_ButtonDown, AboutPage_ButtonUp};

Page *PageList[] = {
    &WelcomePage,
    &MainPage,
    &MusicPlayPage,
    &AboutPage
};    // 菜单切换界面列表
uint8_t PageListIndex = 0;    // 菜单切换界面列表索引
Page *current_page;    // 当前界面函数指针

uint8_t led_flag = 1;    // LED 启用标志，0 关闭，1 开启
uint8_t led_boom_flag = 0;    // LED 节拍标志，0 无节拍，1 有节拍
uint32_t led_boom_tick;    // LED 节拍时刻
uint8_t led_boom_state = 0;    // LED 节拍状态
#define LEDBOOMTICK 64

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
    if (PageListIndex == sizeof(PageList) / sizeof(PageList[0]))
    {
        PageListIndex = 1;
    }
    SetPage(PageList[PageListIndex]);
}

/**
 * @brief 切换到上一个界面
 */
void PrevPage(void)
{
    PageListIndex --;
    if (PageListIndex == 0)
    {
        PageListIndex = sizeof(PageList) / sizeof(PageList[0]) - 1;
    }
    SetPage(PageList[PageListIndex]);
}

void Page_Init(void)
{
    SetPage(PageList[PageListIndex]);
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
    PageListIndex ++;
}

void WelcomePage_Render(void)
{
    if ((MyTime_GetTick() - WelcomePage_temp) > 256)
    {
        SetPage(PageList[PageListIndex]);
    }
}


// ============= 主界面 =============

uint8_t record_flag = 0;    // 录音标志，0 为未录音，1 为录音中
uint32_t last_record_tick;    // 开始录音时刻
/**
 * @brief 绘制麦克风，30X30
 * @param Left 左上角横坐标
 * @param Top 左上角纵坐标
 */
void MainPage_DrawMIC(uint8_t Left, uint8_t Top)
{
    MyOLED_Clear_GRAM_Rect(Left - 1, Top - 1, 32, 32);
    MyOLED_DrawCircle(Left + 15, Top + 16, 10, 1);
    MyOLED_Clear_GRAM_Rect(Left + 5, Top + 6, 20, 10);
    MyOLED_DrawCircle(Left + 15, Top + 8, 6, 1);
    MyOLED_DrawCircle(Left + 15, Top + 16, 6, 1);
    MyOLED_Clear_GRAM_Rect(Left + 5, Top + 8, 20, 8);
    MyOLED_Fill_GRAM_Rect(Left + 9, Top + 8, 1, 8);
    MyOLED_Fill_GRAM_Rect(Left + 21, Top + 8, 1, 8);
    MyOLED_Fill_GRAM_Rect(Left + 9, Top + 12, 12, 1);
    MyOLED_Fill_GRAM_Rect(Left + 15, Top + 26, 1, 4);
    MyOLED_Fill_GRAM_Rect(Left + 11, Top + 30, 8, 1);
    MyOLED_Update(Left - 1, (Top - 1) / 8, 32, 4 + ((Top - 1) % 8 != 0));
}

/**
 * @brief 开始录音
 */
void MainPage_StartRecord(void)
{
    record_flag = 1;
    MainPage_DrawMIC(5, 5);
    MainPage_DrawMIC(93, 5);
    last_record_tick = 0;
    Music_InitRecord();
}

/**
 * @brief 停止录音
 */
void MainPage_StopRecord(void)
{
    record_flag = 0;
    MyOLED_Clear_GRAM_Rect(5, 5, 30, 30);
    MyOLED_Clear_GRAM_Rect(93, 5, 30, 30);
    MyOLED_DrawCircle(20, 20, 15, 2);
    MyOLED_DrawCircle(108, 20, 15, 2);
    MyOLED_Update(4, 0, 32, 5);
    MyOLED_Update(92, 0, 32, 5);
    Music_RecordSave();
}

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
    MainPage_DrawVolumeBar();
    MyOLED_Flip();
}

float ani_temp1 = 0;
float ani_temp2 = 0.1;
float ani_temp3 = 0;
float ani_temp4 = 0.1;
void MainPage_Render(void)
{
    uint32_t t;
    uint8_t i;
    if (record_flag == 0)
    {
        if (Buzzer1.active == 1 && Buzzer1.volume > 0)
        {
            ani_temp3 += ani_temp4;
            MyOLED_Clear_GRAM_Rect(96, 8, 24, 24);
            MyOLED_DrawCircle(108, 20, 15, 2);
            MyOLED_DrawCircle(108, 20, (uint8_t)ani_temp3, 1);
            MyOLED_Update(93, 0, 30, 5);
            if (ani_temp3 >= 8) ani_temp4 = -0.1;
            else if (ani_temp3 <= 0) ani_temp4 = 0.1;
        }
        if (Buzzer2.active == 1 && Buzzer2.volume > 0)
        {
            ani_temp1 += ani_temp2;
            MyOLED_Clear_GRAM_Rect(8, 8, 24, 24);
            MyOLED_DrawCircle(20, 20, 15, 2);
            MyOLED_DrawCircle(20, 20, (uint8_t)ani_temp1, 1);
            MyOLED_Update(5, 0, 30, 5);
            if (ani_temp1 >= 8) ani_temp2 = -0.1;
            else if (ani_temp1 <= 0) ani_temp2 = 0.1;
        }
    }
    if (led_boom_flag == 1)
    {
        t = MyTime_GetTick();
        if ((t - led_boom_tick) > LEDBOOMTICK)
        {
            led_boom_state = !led_boom_state;
            for (i = 0; i < 8; i ++)
            {
                if (led_boom_state && led_flag == 1 && LED_IsOn(i) == 0)
                {
                    LED_On(i);
                }
                else if (LED_IsOn(i) == 1)
                {
                    LED_Off(i);
                }
            }
            led_boom_tick = t;
        }
    }
}

void MainPage_ButtonDown(uint8_t num)
{
    uint32_t temp;
    
    if (num < 8)
    {
        MyOLED_Fill_GRAM_Rect(17 + num * 12, 41, 10, 22);
        MyMisc_DrawNoteSmall(20 + num * 12, 50, num, 0);
        MyOLED_Update(17 + num * 12, 5, 10, 3);
        Buzzer_Play(num);
        if (led_flag == 1 && led_boom_flag == 0)
        {
            LED_On(num);
        }
        if (record_flag == 1)
        {
            if (last_record_tick == 0)
            {
                last_record_tick = MyTime_GetTick();
                Music_RecordNote(num, 0);
            }
            else
            {
                temp = MyTime_GetTick();
                if (temp - last_record_tick > 255)
                {
                    Music_RecordNote(num, 255);
                }
                else
                {
                    Music_RecordNote(num, temp - last_record_tick);
                }
                last_record_tick = temp;
            }
        }
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
    else if (num == 11)    // LED 开关
    {
        led_flag = !led_flag;
        if (led_flag == 0)
        {
            LED_IndicateOff();
        }
        else
        {
            LED_IndicateOn();
        }
    }
    else if (num == 8)    // 录音
    {
        if (record_flag == 0)
        {
            MainPage_StartRecord();
        }
        else
        {
            MainPage_StopRecord();
        }
    }
    else if (num == 9)    // 播放最新的录音（如果有）
    {
        current_music = Music_Search("Record - 1");
        if (current_music != NULL)
        {
            Music_Play();
        }
    }
    else if (num == 10)    // 开启/关闭 LED 节拍
    {
        led_boom_flag = !led_boom_flag;
        if (led_boom_flag == 1)
        {
            led_boom_tick = MyTime_GetTick();
        }
        else
        {
            for (num = 0; num < 8; num ++)
            {
                if (LED_IsOn(num) == 1)
                {
                    LED_Off(num);
                }
            }
        }
    }
}

void MainPage_ButtonUp(uint8_t num)
{
    uint32_t temp;

    if (num < 8)
    {
        MyOLED_Clear_GRAM_Rect(17 + num * 12, 41, 10, 22);
        MyMisc_DrawNoteSmall(20 + num * 12, 53, num, 1);
        MyOLED_Update(17 + num * 12, 5, 10, 3);
        Buzzer_UnPlay(num);
        if (led_boom_flag == 0)
        {
            LED_Off(num);
        }
        if (record_flag == 1)
        {
            temp = MyTime_GetTick();
            if (temp - last_record_tick > 255)
            {
                Music_RecordNote(num + 8, 255);
            }
            else
            {
                Music_RecordNote(num + 8, temp - last_record_tick);
            }
            last_record_tick = temp;
        }
    }
}


// ============= 关于 =============

uint8_t gy[] = {0, 1};
uint8_t jydzq[] = {2, 3, 4, 5, 6};
uint8_t bhtqddz[] = {7, 8, 9, 6, 10, 4, 11};

void AboutPage_Init(void)
{
    uint8_t i;

    MyOLED_Fill_GRAM_Rect(0, 0, 128, 64);
    MyOLED_Clear_GRAM_Rect(1, 1, 126, 62);

    // for (i = 0; i < 2; i ++)
    // {
    //     MyOLED_Blit_GRAM(ChineseFont[gy[i] * 2], 4 + i * 16, 4, 16);
    //     MyOLED_Blit_GRAM(ChineseFont[gy[i] * 2 + 1], 4 + i * 16, 4 + 8, 16);
    // }
    for (i = 0; i < 5; i ++)
    {
        MyOLED_Blit_GRAM(ChineseFont[jydzq[i] * 2], 24 + i * 16, 4, 16);
        MyOLED_Blit_GRAM(ChineseFont[jydzq[i] * 2 + 1], 24 + i * 16, 4 + 8, 16);
    }
    MyOLED_Blit_String_ASCII_8x16(40, 24, "V1.0.6");
    for (i = 0; i < 7; i ++)
    {
        MyOLED_Blit_GRAM(ChineseFont[bhtqddz[i] * 2], 8 + i * 16, 44, 16);
        MyOLED_Blit_GRAM(ChineseFont[bhtqddz[i] * 2 + 1], 8 + i * 16, 44 + 8, 16);
    }

    MyOLED_Flip();
}

void AboutPage_ButtonDown(uint8_t num)
{
if (num == 12)    // 音量 +
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

}


// ============= 播放音乐界面 =============

#define MUSICPLAYICONLEFT 56
#define MUSICPLAYICONTOP 40
uint8_t show_play_state;    // 显示的播放状态，0 为播放，1 为暂停，2 为未知
char * show_music_name = NULL;    // 显示的音乐名称
/**
 * @brief 绘制播放按钮，大小为 16×16
 * @param State 播放状态，0 为播放，1 为暂停
 */
void MusicPlayPage_DrawPlayButton(uint8_t State)
{
    uint8_t x, height;
    float temp;

    if (show_play_state != State)
    {
        MyOLED_Clear_GRAM_Rect(MUSICPLAYICONLEFT, MUSICPLAYICONTOP, 16, 16);
        if (State == 1)    // 绘制暂停图案
        {
            MyOLED_Fill_GRAM_Rect(MUSICPLAYICONLEFT + 1, MUSICPLAYICONTOP, 4, 16);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYICONLEFT + 10, MUSICPLAYICONTOP, 4, 16);
        }
        else    // 绘制播放图案
        {
            for (x = 1; x < 14; x ++)
            {
                temp = (60.0 - (x + 0.5) * 4.0) / 7.0;
                if (temp - (uint8_t)temp < 0.5)
                {
                    height = (uint8_t)temp * 2;
                }
                else
                {
                    height = (uint8_t)temp * 2 + 2;
                }
                MyOLED_Fill_GRAM_Rect(MUSICPLAYICONLEFT + x, MUSICPLAYICONTOP + 8 - height / 2, 1, height);
            }
        }
        MyOLED_Update(MUSICPLAYICONLEFT, MUSICPLAYICONTOP / 8, 16, 2 + (MUSICPLAYICONTOP % 8 != 0));
        show_play_state = State;
    }
}

uint16_t total_index, last_index = 0;    // 音乐总音符数，上次音符数
/**
 * @brief 绘制音乐名称
 */
void MusicPlayPage_DrawMusicName(void)
{
    if (show_music_name != current_music->Name)
    {
        MyOLED_Clear_GRAM_Rect(1, 1, 126, 15);
        if (current_music == NULL)
        {
            MyOLED_Blit_String_ASCII_8x16(32, 0, "* ---- *");
            show_music_name = "* ---- *";
        }
        else
        {
            for (total_index = 0; current_music->Note[total_index] != 0xffff; total_index ++);
            last_index = total_index;
            MyOLED_Blit_String_ASCII_8x16(64 - strlen(current_music->Name) * 4, 0, current_music->Name);
            show_music_name = current_music->Name;
        }
        MyOLED_Update(0, 0, 128, 2);
    }
}

/**
 * @brief 绘制播放进度
 */
void MusicPlayPage_DrawProcess(void)
{
    uint8_t width;

    if (last_index != play_index || play_state == 1)
    {
        width = 126 * play_index / total_index;
        MyOLED_Fill_GRAM_Rect(1, 60, width, 3);
        MyOLED_Clear_GRAM_Rect(1 + width, 60, 126 - width, 3);
        MyOLED_Update(1, 7, 126, 1);
        last_index = play_index;
    }
}

Music * chosen_music = NULL;    // 选择的音乐
uint32_t chosen_tick = 0;    // 选择的时刻
char * show_chosen_music_name = NULL;    // 显示的选择音乐名称
/**
 * @brief 绘制选择的音乐名称
 */
void MusicPlayPage_DrawChosenMusicName(void)
{
    if ((chosen_music != NULL && chosen_music->Name != show_chosen_music_name) || (chosen_music == NULL && show_chosen_music_name != NULL))
    {
        MyOLED_Clear_GRAM_Rect(1, 16, 126, 16);
        if (chosen_music == NULL)
        {
            show_chosen_music_name = NULL;
            MyOLED_Update(0, 2, 128, 2);
        }
        else
        {
            MyOLED_Blit_String_ASCII_8x16(64 - strlen(chosen_music->Name) * 4, 16, chosen_music->Name);
            show_chosen_music_name = chosen_music->Name;
            MyOLED_Update_Reverse(1, 2, 126, 2);
        }
    }
}

#define MUSICPLAYMODELEFT 88
#define MUSICPLAYMODETOP 40
uint8_t show_play_mode;    // 显示的播放模式，5 为未知
/**
 * @brief 绘制播放模式
 */
void MusicPlayPage_DrawPlayMode(void)
{
    if (show_play_mode != play_mode)
    {
        MyOLED_Clear_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP, 16, 16);
        switch (play_mode)
        {
        case 0:
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 1, 12);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 2, 1, 12);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 2, 14, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 13, 14, 1);

            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 5, 1, 6);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 7, MUSICPLAYMODETOP + 5, 1);
            
            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 2, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 13, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 13, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 12, 1);
            break;
        case 1:
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 1, 8);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 6, 1, 8);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 2, 15, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 13, 15, 1);

            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 5, 1, 6);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 7, MUSICPLAYMODETOP + 5, 1);
            
            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 13, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 12, 1);

            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 4, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 11, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 12, 1);
            break;
        case 2:
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 10, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 7, 10, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 12, 10, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP, 1, 16);
            
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 13, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 14, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 13, 1);
            break;
        case 3:
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 1, 8);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 6, 1, 8);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 2, 15, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 13, 15, 1);

            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 15, MUSICPLAYMODETOP + 13, 0);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 12, 1);

            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 4, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 11, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 12, 1);            
            break;
        case 4:
            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 2, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT, MUSICPLAYMODETOP + 13, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 2, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 1, MUSICPLAYMODETOP + 13, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 2, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 2, MUSICPLAYMODETOP + 13, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 3, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 3, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 4, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 4, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 5, MUSICPLAYMODETOP + 4, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 5, MUSICPLAYMODETOP + 11, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 6, MUSICPLAYMODETOP + 5, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 6, MUSICPLAYMODETOP + 10, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 6, MUSICPLAYMODETOP + 6, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 6, MUSICPLAYMODETOP + 9, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 7, MUSICPLAYMODETOP + 7, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 7, MUSICPLAYMODETOP + 8, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 6, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 9, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 5, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 8, MUSICPLAYMODETOP + 10, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 9, MUSICPLAYMODETOP + 4, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 9, MUSICPLAYMODETOP + 11, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 10, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 10, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 11, MUSICPLAYMODETOP + 2, 5, 1);
            MyOLED_Fill_GRAM_Rect(MUSICPLAYMODELEFT + 11, MUSICPLAYMODETOP + 13, 5, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 3, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 14, MUSICPLAYMODETOP + 12, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 4, 1);
            MyOLED_Blit_Point(MUSICPLAYMODELEFT + 13, MUSICPLAYMODETOP + 11, 1);
            break;
        default:
            break;
        }
        MyOLED_Update(MUSICPLAYMODELEFT, MUSICPLAYMODETOP / 8, 16, 2 + (MUSICPLAYMODETOP % 8 != 0));
        show_play_mode = play_mode;
    }
}

#define MUSICPLAYSPEEDLEFT 12
#define MUSICPLAYSPEEDTOP 40
uint8_t show_speed;    // 显示的速度，3 为未知
/**
 * @brief 绘制速度
 */
void MusicPlayPage_DrawSpeed(void)
{
    if (show_speed != play_speed)
    {
        MyOLED_Clear_GRAM_Rect(MUSICPLAYSPEEDLEFT, MUSICPLAYSPEEDTOP, 32, 16);
        if (play_speed == 0)
        {
            MyOLED_Blit_String_ASCII_8x16(MUSICPLAYSPEEDLEFT, MUSICPLAYSPEEDTOP, "0.5x");
        }
        else if (play_speed == 1)
        {
            MyOLED_Blit_String_ASCII_8x16(MUSICPLAYSPEEDLEFT, MUSICPLAYSPEEDTOP, "1.0x");
        }
        else if (play_speed == 2)
        {
            MyOLED_Blit_String_ASCII_8x16(MUSICPLAYSPEEDLEFT, MUSICPLAYSPEEDTOP, "2.0x");
        }
        MyOLED_Update(MUSICPLAYSPEEDLEFT, MUSICPLAYSPEEDTOP / 8, 32, 2 + (MUSICPLAYSPEEDTOP % 8 != 0));
        show_speed = play_speed;
    }
}

uint32_t mpp_tick_temp;

void MusicPlayPage_Init(void)
{
    MyOLED_Fill_GRAM_Rect(0, 0, 128, 64);
    MyOLED_Clear_GRAM_Rect(1, 1, 126, 62);
    show_play_state = 2;
    show_play_mode = 5;
    show_speed = 3;
    show_music_name = NULL;
    show_chosen_music_name = NULL;
    MyOLED_Flip();
}

#define LEDSWITCHTICK 16

void MusicPlayPage_Render(void)
{
    uint8_t i;
    uint32_t tick_temp;
    static int8_t temp = 0;
    static int8_t flag = 1;

    MusicPlayPage_DrawMusicName();
    MusicPlayPage_DrawChosenMusicName();
    MusicPlayPage_DrawSpeed();
    MusicPlayPage_DrawPlayButton(play_state);
    MusicPlayPage_DrawPlayMode();
    MusicPlayPage_DrawProcess();
    if (chosen_music != NULL && MyTime_GetTick() - chosen_tick > 256)
    {
        chosen_music = NULL;
    }
    if (led_flag == 1 && play_state == 1)
    {
        switch (play_mode)
        {
        case 0:    // 播放单曲，所有灯常亮
            for (i = 0; i < 8; i ++)
            {
                if (!LED_IsOn(i))
                {
                    LED_On(i);
                }
            }
            break;
        case 1:    // 单曲循环，所有灯闪烁
            tick_temp = MyTime_GetTick();
            if (tick_temp - mpp_tick_temp > LEDSWITCHTICK * 4)
            {
                for (i = 0; i < 8; i ++)
                {
                    if (flag == 1)
                    {
                        LED_Off(i);
                    }
                    else
                    {
                        LED_On(i);
                    }
                }
                flag = -flag;
                mpp_tick_temp = tick_temp;
            }
            break;
        case 2:    // 顺序播放，一个一个亮
            tick_temp = MyTime_GetTick();
            if (tick_temp - mpp_tick_temp > LEDSWITCHTICK)
            {
                for (i = 0; i < 8; i ++)
                {
                    if (LED_IsOn(i) && i != temp)
                    {
                        LED_Off(i);
                    }
                    else if (!LED_IsOn(i) && i == temp)
                    {
                        LED_On(i);
                    }
                }
                temp = (temp + 1) % 8;
                mpp_tick_temp = tick_temp;
            }
            break;
        case 3:    // 循环播放，来回亮一个灯
            tick_temp = MyTime_GetTick();
            if (tick_temp - mpp_tick_temp > LEDSWITCHTICK)
            {
                for (i = 0; i < 8; i ++)
                {
                    if (LED_IsOn(i) && i != temp)
                    {
                        LED_Off(i);
                    }
                    else if (!LED_IsOn(i) && i == temp)
                    {
                        LED_On(i);
                    }
                }
                temp += flag;
                if (temp >= 7)
                {
                    flag = -1;
                }
                else if (temp <= 0)
                {
                    flag = 1;
                }
                mpp_tick_temp = tick_temp;
            }
            break;
        case 4:    // 随机播放，随机亮灯
            tick_temp = MyTime_GetTick();
            if (tick_temp - mpp_tick_temp > LEDSWITCHTICK)
            {
                temp = MyMisc_RandomInt(0, 7);
                for (i = 0; i < 8; i ++)
                {
                    if (i == temp)
                    {
                        LED_On(i);
                    }
                    else
                    {
                        LED_Off(i);
                    }
                }
                mpp_tick_temp = tick_temp;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        for (i = 0; i < 8; i ++)
        {
            if (LED_IsOn(i))
            {
                LED_Off(i);
            }
        }
    }
}

void MusicPlayPage_ButtonDown(uint8_t num)
{
    Music *temp;
    
    switch (num)
    {
    case 0:    // 关闭/开启灯效
        led_flag = !led_flag;
        if (led_flag == 0)
        {
            LED_IndicateOff();
        }
        else
        {
            LED_IndicateOn();
        }
        break;
    case 6:    // 停止
        Music_Stop();
        break;
    case 7:    // 重播
        temp = current_music;
        Music_Stop();
        current_music = temp;
        Music_Play();
        break;
    case 8:    // 选择下一首
        if (chosen_music == NULL)
        {
            chosen_music = Music_GetNode(0);
        }
        else
        {
            chosen_music = chosen_music->next;
            if (chosen_music == &MusicTail)
            {
                chosen_music = MusicHead.next;
            }
        }
        chosen_tick = MyTime_GetTick();
        break;
    case 9:    // 设置播放模式
        play_mode = (play_mode + 1) % 5;
        break;
    case 10:    // 播放 / 暂停
        if (chosen_music != NULL)
        {
            if (play_state == 1)
            {
                Music_Stop();
            }
            else if (play_state == 2 && chosen_music != current_music)
            {
                play_index = 0;
            }

            current_music = chosen_music;
            chosen_music = NULL;
            Music_Play();
        }
        else if (play_state != 1)
        {
            if (current_music == NULL)
            {
                current_music = Music_GetNode(2);
            }
            Music_Play();
            for (total_index = 0; current_music->Note[total_index] != 0xffff; total_index ++);
            last_index = total_index;
        }
        else
        {
            Music_Pause();
        }
        break;
    case 11:    // 设置速度
        play_speed = (play_speed + 1) % 3;
        break;
    case 12:    // 音量 +
        Buzzer_HighVolume();
        break;
    case 13:    // 音量 -
        Buzzer_LowerVolume();
        break;
    case 14:    // 下一个界面
        NextPage();
        break;
    case 15:    // 上一个界面
        PrevPage();
        break;
    default:
        // 处理未匹配的 case
        break;
    }
}

void MusicPlayPage_ButtonUp(uint8_t num)
{

}
