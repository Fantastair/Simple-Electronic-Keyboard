#ifndef __PAGE_H
#define __PAGE_H
#include "stm32f10x.h"


typedef struct
{
    void (*PageInit)(void);    // 界面初始化函数指针
    void (*PageRender)(void);    // 界面渲染函数指针
    void (*PageButtonDown)(uint8_t);    // 界面按键按下处理函数指针
    void (*PageButtonUp)(uint8_t);    // 界面按键抬起处理函数指针
} Page;    // 界面结构体

extern Page WelcomePage;    // 开场界面
extern Page *current_page;    // 当前界面函数指针

void SetPage(Page *page);
void Page_Init(void);


#endif
