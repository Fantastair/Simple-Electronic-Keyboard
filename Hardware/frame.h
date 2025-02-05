#ifndef __FRAME_H
#define __FRAME_H
#include "Page.h"

typedef enum {False, True} MyBool;    // 布尔值定义

typedef struct _tick_func
{
    struct _tick_func *next;    // 下一个帧函数
    struct _tick_func *last;    // 上一个帧函数
    void (*func)(uint16_t);    // 执行函数
    uint16_t current_frame;    // 当前帧数
    uint16_t totalframe;    // 持续帧数
    MyBool auto_destroy;    // 临时标志（为 1 则帧函数结束后会自动销毁）
} TickFunc;    // 帧函数结构体，采用有头双向链表的形式管理


void FrameInit(void);
void CheckHardware(Page *page);

void TickFuncRemove(TickFunc * pTF);
MyBool TickFuncInLink(TickFunc * pTF);
void TickFuncAppend(TickFunc * pTF);

TickFunc * CreateTickFunc(void (*func)(uint16_t), uint16_t totalframe, MyBool auto_destroy);
void LaunchTickFunc(TickFunc * pTF);
void StopTickFunc(TickFunc * pTF);
void ApplyTickFunc(void (*func)(uint16_t), uint16_t totalframe);

void FrameTick(void);

#endif
