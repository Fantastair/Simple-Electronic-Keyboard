#include <stdlib.h>
#include "stm32f10x.h"
#include "MyFlash.h"
#include "MyTime.h"
#include "Music.h"
#include "frame.h"
#include "Buzzer.h"
#include "MyMisc.h"

#include "MyOLED.h"
#include "MyOLED_Render.h"

void Music_PlayTickFunc(uint16_t tick);

uint8_t *FlashTable;    // 闪存表头指针
Music MusicHead;    // 音频链表头
Music MusicTail;    // 音频链表尾

/**
 * @brief 添加音频到链表尾
 * @param music 音频指针
 */
void Music_ListAdd(Music *music)
{
    MusicTail.last->next = music;
    music->next = &MusicTail;
    music->last = MusicTail.last;
    MusicTail.last = music;
}

/**
 * @brief 删除音频
 * @param music 音频指针
 */
void Music_ListRemove(Music *music)
{
    music->last->next = music->next;
    music->next->last = music->last;
    free(music);
}

/**
 * @brief 创建音频，并添加到链表尾
 * @param Name 音频名称
 * @param Note 音符轨道地址
 * @param Tick 时刻轨道地址
 * @return 音频指针
 */
Music * Music_Create(char *Name, uint16_t *Note, uint16_t *Tick)
{
    Music *music = (Music *)malloc(sizeof(Music));
    music->Name = Name;
    music->Note = Note;
    music->Tick = Tick;
    Music_ListAdd(music);
    return music;
}

TickFunc *MusicTickFunc;    // 播放音乐帧函数

/**
 * @brief 初始化音频模块
 *     音频数据存储在 flash 闪存区域
 *     需要读取表头到内存中，并根据表头信息初始化链表
 */
void Music_Init(void)
{
    uint8_t i;
    uint16_t t1[] = {0x0006, 0x000E, 0x0001, 0x0009, 0x0000, 0x0008};
    uint16_t t2[] = {0, 16, 0, 16, 0, 16};
    uint16_t t3[] = {0x0000, 0x0008, 0x0001, 0x0009, 0x0006, 0x000E};
    uint16_t t4[] = {0, 16, 0, 16, 0, 16};

    FlashTable = (uint8_t *)MyFlash_GetPageAddress(63);

    if (MyFlash_IsEmptyPage(63))    // 如果表头为空，则初始化表头并添加开机音效
    {
        Music_Save("Connect", t3, t4, 6);
        Music_Save("Open ", t1, t2, 6);
        Music_InitFlashTable();
    }

    MusicHead.next = &MusicTail;
    MusicHead.last = NULL;
    MusicTail.last = &MusicHead;
    MusicTail.next = NULL;

    for (i = 48; i < 64; i ++)
    {
        if (MyFlash_ReadData_16((uint32_t)(FlashTable + 16 * i)) == 2)    // 第 i 页存储了音频数据
        {
            Music_Create((char *)(FlashTable + 16 * i + 2), (uint16_t *)MyFlash_GetPageAddress(i), (uint16_t *)(MyFlash_GetPageAddress(i) + 512));
        }
    }

    MusicTickFunc = CreateTickFunc(Music_PlayTickFunc, 0, False);
    Music_Play(Music_GetNode(0));
}

/**
 * @brief 初始化闪存表头数据
 */
void Music_InitFlashTable(void)
{
    uint16_t i;

    for (i = 0; i < 64; i ++)
    {
        if (MyFlash_IsEmptyPage(i))
        {
            MyFlash_WriteData_16(MyFlash_GetPageAddress(63) + 16 * i, 0);
        }
        else
        {
            MyFlash_WriteData_16(MyFlash_GetPageAddress(63) + 16 * i, 1);
        }
    }
}

/**
 * @brief 计算音频时刻长度，并转换成秒
 * @param music 音频指针
 * @return 音频长度（秒）
 */
float Music_GetLength(Music *music)
{
    uint16_t i;
    uint32_t result = 0;

    for (i = 0; music->Note[i] != 0xffff; i ++)
    {
        result += music->Tick[i];
    }
    return MyTime_Tick2Second(result);
}

/**
 * @brief 获取链表长度
 * @return 链表长度
 */
uint8_t Music_GetLinkLength(void)
{
    uint8_t i = 0;
    Music *music = MusicHead.next;

    while (music != &MusicTail)
    {
        i ++;
        music = music->next;
    }
    return i;
}

/**
 * @brief 获取链表节点
 *     如果超出长度返回 NULL
 * @param index 节点索引
 * @return 音频指针
 */
Music *Music_GetNode(uint8_t index)
{
    Music *music = MusicHead.next;

    while (music != &MusicTail)
    {
        if (index == 0)
        {
            return music;
        }
        music = music->next;
        index --;
    }
    return NULL;
}

/**
 * @brief 存储音频数据到闪存
 * @param Name 音频名称，长度必须为偶数（包括末位 \0 停止符）且小于 14
 * @param Note 音符轨道
 * @param Tick 时刻轨道
 * @param Length 数据长度
 * @return 保存成功返回 1，否则返回 0
 */
uint8_t Music_Save(char * Name, uint16_t * Note, uint16_t * Tick, uint16_t Length)
{
    uint8_t page, i, flag = 0;

    for (i = 62; i > 0; i --)
    {
        if (MyFlash_IsEmptyPage(i))
        {
            page = i;
            flag = 1;
            break;
        }
    }
    if (flag == 0) return 0;

    for (i = 0; Name[i] != '\0'; i ++);
    MyFlash_WriteData_16(MyFlash_GetPageAddress(63) + 16 * page, 2);
    MyFlash_WriteData_16x((uint32_t)(FlashTable + 16 * page + 2), (uint16_t *)Name, (i + 1) / 2);
    MyFlash_WriteData_16x(MyFlash_GetPageAddress(page), Note, Length);
    MyFlash_WriteData_16x(MyFlash_GetPageAddress(page) + 512, Tick, Length);
    return 1;
}

Music *current_music = NULL;    // 当前播放的音频指针
uint8_t play_mode = 0;    // 播放模式，0 为播放单曲，1 为单曲循环，2 为列表播放，3 为列表循环，4 为随机播放
uint8_t play_state = 0;    // 播放状态，0 为停止，1 为播放，2 为暂停
uint16_t play_tick;    // 播放时刻
uint16_t play_index;    // 播放索引

/**
 * @brief 播放音乐
 * @param music 音频指针
 */
void Music_Play(Music *music)
{
    play_state = 1;
    if (play_state != 2)    // 播放音乐
    {
        LaunchTickFunc(MusicTickFunc);
        current_music = music;
        play_index = 0;
        play_tick = MyTime_GetTick();
    }
    else    // 恢复播放
    {
        LaunchTickFunc(MusicTickFunc);
        play_tick = MyTime_GetTick();
    }
}

/**
 * @brief 暂停音乐
 */
void Music_Pause(void)
{
    play_state = 2;
    StopTickFunc(MusicTickFunc);
}

/**
 * @brief 停止音乐
 */
void Music_Stop(void)
{
    play_state = 0;
    StopTickFunc(MusicTickFunc);
}

/**
 * @brief 播放下一首
 */
void Music_Next(void)
{
    if (play_mode == 0)    // 播放单曲
    {
        Music_Stop();
    }
    else if (play_mode == 1)    // 单曲循环
    {
        Music_Play(current_music);
    }
    else if (play_mode == 2)    // 列表播放
    {
        if (current_music->next != &MusicTail)
        {
            Music_Play(current_music->next);
        }
        else
        {
            Music_Stop();
        }
    }
    else if (play_mode == 3)    // 列表循环
    {
        if (current_music->next != &MusicTail)
        {
            Music_Play(current_music->next);
        }
        else
        {
            Music_Play(MusicHead.next);
        }
    }
    else if (play_mode == 4)    // 随机播放
    {
        Music_Play(Music_GetNode(MyMisc_RandomInt(0, Music_GetLinkLength() - 1)));
    }
}

void Music_PlayTickFunc(uint16_t tick)
{
    uint32_t t = MyTime_GetTick() - play_tick;

    if (t >= current_music->Tick[play_index])
    {
        if (current_music->Note[play_index] < 8)
        {
            Buzzer_Play(current_music->Note[play_index]);
        }
        else
        {
            Buzzer_UnPlay(current_music->Note[play_index] - 8);
        }
        play_index ++;
        play_tick += t;
        if (current_music->Note[play_index] == 0xFFFF)
        {
            play_state = 0;
            StopTickFunc(MusicTickFunc);
        }
    }
}
