#ifndef __FRAME_H
#define __FRAME_H
#include "Page.h"

typedef struct _tick_func
{
    struct _tick_func *next;    // 下个指针
    void (*tickfunc)(uint16_t);    // 执行函数
    uint16_t current_frame;    // 当前帧数
    uint16_t total_frames;    // 持续帧数
    uint8_t temp_flag;    // 临时标志（为 1 则帧函数结束后会自动销毁）
} TickFunc;    // 帧函数结构体（采用单向链表维护）


void CheckHardware(Page *page);
TickFunc* Search_TickFunc(TickFunc *TF);
TickFunc* Create_TickFunc(void (*tickfunc)(uint16_t), uint16_t total_frames, uint8_t temp_flag);
void Start_TickFunc(TickFunc *TF);
void Apply_TickFunc(void (*tickfunc)(uint16_t), uint16_t total_frames);
void Stop_TickFunc(TickFunc *TF);
void Continue_TickFunc(TickFunc *TF);
void Destroy_TickFunc(TickFunc *TF);
void Frame_Tick(void);



#endif
