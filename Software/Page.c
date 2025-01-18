#include "stm32f10x.h"
#include "Page.h"

void blankfunc1(void) {}    // 空函数，占位用
void blankfunc2(uint8_t a) {}    // 空函数，占位用

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
