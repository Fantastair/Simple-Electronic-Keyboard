#include "stm32f10x.h"
#include "Keyboard.h"
#include "MyMisc.h"
#include "Page.h"
#include "frame.h"
#include "stdlib.h"
#include "Serial.h"

#include "MyTime.h"
#include "MyOLED.h"
#include "MyOLED_Render.h"

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

    Serial_HandleOrder();
}


TickFunc TFHead = {NULL, NULL};
TickFunc TFTail = {NULL, NULL};

/**
 * @brief 初始化模块
 */
void FrameInit(void)
{
    TFHead.next = &TFTail;
    TFTail.last = &TFHead;
}

/**
 * @brief 判断帧函数是否在链表中
 * @param pTF 帧函数指针
 * @retval True or False
 */
MyBool TickFuncInLink(TickFunc * pTF)
{
    TickFunc * pTF2 = TFHead.next;

    while (pTF2 != &TFTail)
    {
        if (pTF2 == pTF) return True;
        pTF2 = pTF2->next;
    }
    return False;
}

/**
 * @brief 添加帧函数到链表最后
 * @param pTF 帧函数指针
 */
void TickFuncAppend(TickFunc * pTF)
{
    pTF->last = TFTail.last;
    TFTail.last->next = pTF;
    pTF->next = &TFTail;
    TFTail.last = pTF;
}

/**
 * @brief 从链表中删除帧函数，必须确保帧函数存在
 * @param pTF 帧函数指针
 */
void TickFuncRemove(TickFunc * pTF)
{
    pTF->last->next = pTF->next;
    pTF->next->last = pTF->last;
    if (pTF->auto_destroy == True) free(pTF);
}

/**
 * @brief 创建帧函数
 * @param func 执行函数，接收一个 uint16_t 参数，返回 void
 * @param totalframe 持续帧数，如果为 0，则永久持续，只能由其他函数手动停止
 * @param auto_destroy 是否自动销毁，为 True 或 False，如果需要重复使用此帧函数，则使用 False
 * @retval 返回新建帧函数的地址
 */
TickFunc * CreateTickFunc(void (*func)(uint16_t), uint16_t totalframe, MyBool auto_destroy)
{
    TickFunc * pTF = (TickFunc *)malloc(sizeof(TickFunc));

    pTF->next = NULL;
    pTF->last = NULL;
    pTF->func = func;
    pTF->totalframe = totalframe;
    pTF->current_frame = 0;
    pTF->auto_destroy = auto_destroy;

    return pTF;
}

/**
 * @brief 启动帧函数，如果帧函数已经启动，则从第 0 帧开始
 * @param pTF 帧函数指针
 */
void LaunchTickFunc(TickFunc * pTF)
{
    pTF->current_frame = 0;
    if (!TickFuncInLink(pTF)) TickFuncAppend(pTF);
}

/**
 * @brief 停止帧函数
 * @param pTF 帧函数指针
 */
void StopTickFunc(TickFunc * pTF)
{
    if (TickFuncInLink(pTF)) TickFuncRemove(pTF);
}

/**
 * @brief 执行帧函数
 */
void FrameTick(void)
{
    TickFunc * pTF = TFHead.next;

    while (pTF != &TFTail)
    {
        pTF->func(pTF->current_frame);
        if (pTF->totalframe != 0)
        {
            if (pTF->current_frame + 1 == pTF->totalframe)
            {
                pTF = pTF->last;
                TickFuncRemove(pTF->next);
            }
            else pTF->current_frame ++;
        }
        pTF = pTF->next;
    }
}

/**
 * @brief 快速创建并启动一个临时帧函数
 * @param func 执行函数，接收一个 uint16_t 参数，返回 void
 * @param totalframe 持续帧数，如果为 0，则永久持续，只能由其他函数手动停止
 */
void ApplyTickFunc(void (*func)(uint16_t), uint16_t totalframe)
{
    LaunchTickFunc(CreateTickFunc(func, totalframe, True));
}
