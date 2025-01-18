#include "stm32f10x.h"
#include "Keyboard.h"
#include "MyMisc.h"
#include "Page.h"
#include "frame.h"
#include "stdlib.h"

uint16_t last_keyboard_num = 0;

/**
 * @brief 检测硬件输入，这是硬件层和逻辑层的对接函数
 * @param page 当前界面指针
 */
void CheckHardware(Page *page)
{
    uint8_t i;
    uint16_t keyboard_num;

    keyboard_num = Keyboard_GetInput();
    for (i = 0; i < 16; i ++)
    {
        if (MyMisc_GetValue16(last_keyboard_num, i) == 0 && MyMisc_GetValue16(keyboard_num, i) == 1)
        {
            page->PageButtonDown(i);
        }
        else if (MyMisc_GetValue16(last_keyboard_num, i) == 1 && MyMisc_GetValue16(keyboard_num, i) == 0)
        {
            page->PageButtonUp(i);
        }
    }
    last_keyboard_num = keyboard_num;
}


TickFunc *TickFuncHead = NULL;

/**
 * @brief 搜索帧函数结构体
 * @param TF 结构体指针
 * @retval 帧函数结构体地址，不存在则返回 NULL
 */
TickFunc* Search_TickFunc(TickFunc *TF)
{
    TickFunc *pTF = TickFuncHead;

    while (pTF != NULL)
    {
        if (pTF == TF)
        {
            return pTF;
        }
        pTF = pTF->next;
    }
    return NULL;
}

/**
 * @brief 创建一个帧函数对象
 *     初始当前帧为 0，下一个帧函数指针为 NULL
 * @param tickfunc 执行函数
 * @param total_frames 持续帧数
 * @param temp_flag 临时标志，为 1 则会在帧函数执行完毕后自动销毁其内存
 * @retval 帧函数对象的结构体地址
 */
TickFunc* Create_TickFunc(void (*tickfunc)(uint16_t), uint16_t total_frames, uint8_t temp_flag)
{
    TickFunc *pTF = (TickFunc*)malloc(sizeof(TickFunc));

    pTF->current_frame = 0;
    pTF->next = NULL;
    pTF->tickfunc = tickfunc;
    pTF->total_frames = total_frames;
    pTF->temp_flag = temp_flag;

    return pTF;
}

/**
 * @brief 启动帧函数
 *     如果该帧函数已经启动，则重新将帧数置 0
 * @param TF 帧函数指针
 */
void Start_TickFunc(TickFunc *TF)
{
    TickFunc *pTF = TickFuncHead;
    TF->current_frame = 0;

    if (pTF == NULL)
    {
        TickFuncHead = TF;
        return;
    }
    
    if (Search_TickFunc(TF) == NULL)
    {
        while (pTF->next != NULL) {pTF = pTF->next;}
        pTF->next = TF;
    }
}

/**
 * @brief 申请帧函数，这会直接创建并启动临时标志为 1 的帧函数
 * @param tickfunc 执行函数
 * @param total_frames 持续帧数
 */
void Apply_TickFunc(void (*tickfunc)(uint16_t), uint16_t total_frames)
{
    Start_TickFunc(Create_TickFunc(tickfunc, total_frames, 1));
}

/**
 * @brief 手动停止帧函数
 *     停止不存在的帧函数是安全的
 * @param TF 帧函数指针
 */
void Stop_TickFunc(TickFunc *TF)
{
    TickFunc *pTF = TickFuncHead;

    if (pTF == NULL) return;

    while (pTF->next != NULL && pTF->next != TF) pTF = pTF->next;

    if (pTF->next == NULL) return;
    else
    {
        pTF->next = TF->next;
    }
}

/**
 * @brief 继续被暂停的帧函数
 *     继续未被暂停的帧函数是安全的
 * @param TF 帧函数指针
 */
void Continue_TickFunc(TickFunc *TF)
{
    TickFunc *pTF = TickFuncHead;

    if (pTF == NULL)
    {
        TickFuncHead = TF;
        return;
    }
    
    if (Search_TickFunc(TF) == NULL)
    {
        while (pTF->next != NULL) {pTF = pTF->next;}
        pTF->next = TF;
    }
}

/**
 * @brief 销毁帧函数，释放其内存
 *     可以直接销毁正在启动的帧函数
 * @param TF 帧函数指针
 */
void Destroy_TickFunc(TickFunc *TF)
{
    if (Search_TickFunc(TF) != NULL) Stop_TickFunc(TF);
    free(TF);
}

/**
 * @brief 执行所有帧函数的执行函数，并清除已经结束的帧函数
 *     帧函数的执行顺序取决于启动的顺序
 */
void Frame_Tick(void)
{
    TickFunc *pTF = TickFuncHead;

    while (pTF != NULL)
    {
        pTF->tickfunc(pTF->current_frame);
        pTF->current_frame ++;
        if (pTF->current_frame == pTF->total_frames)
        {
            if (pTF->temp_flag)
                Destroy_TickFunc(pTF);
            else
                Stop_TickFunc(pTF);
        }
    }
}
