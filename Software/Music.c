#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "MyFlash.h"
#include "MyTime.h"
#include "Music.h"
#include "frame.h"
#include "Buzzer.h"
#include "MyMisc.h"
#include "Serial.h"
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
 * @brief 弹出音频
 * @param Name 音频名称
 * @return 音频指针
 */
Music * Music_Pop(char *Name)
{
    Music *music = Music_Search(Name);

    if (music != NULL)
    {
        music->last->next = music->next;
        music->next->last = music->last;
    }
    return music;
}

/**
 * @brief 插入音频到指定音频后
 * @param music 音频指针
 * @param after 指定音频指针
 */
void Music_Insert(Music *music, Music *after)
{
    music->next = after->next;
    music->last = after;
    after->next->last = music;
    after->next = music;
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

uint8_t FlashTableTemp[1024];    // 闪存表头缓冲数据

/**
 * @brief 同步闪存表头数据到缓冲区
 */
void SyncFlashTable(void)
{
    uint16_t i;

    for (i = 0; i < 1024; i ++)
    {
        FlashTableTemp[i] = MyFlash_ReadData_8((uint32_t)(FlashTable + i));
    }
}

/**
 * @brief 同步缓冲区数据到闪存表头
 */
void SyncFlashTableBack(void)
{
    MyFlash_ErasePage(63);
    MyFlash_WriteData_32x(MyFlash_GetPageAddress(63), (uint32_t *)(FlashTableTemp), 1024 / 4);
}

/**
 * @brief 重命名音频
 * @param music 音频指针
 * @param Name 新名称
 */
void Music_Rename(Music *music, char *Name)
{
    uint8_t i, j;

    for (i = 0; i < 64; i ++)
    {
        if (FlashTableTemp[16 * i] == 2 && strcmp((char *)(FlashTableTemp + 16 * i + 1), music->Name) == 0)
        {
            // strcpy((char *)(FlashTableTemp + 16 * i + 1), Name);
            for (j = 0; Name[j] != '\0'; j ++)
            {
                FlashTableTemp[16 * i + 1 + j] = Name[j];
            }
            FlashTableTemp[16 * i + 1 + j] = '\0';
            j ++;
            for (; j < 15; j ++)
            {
                FlashTableTemp[16 * i + 1 + j] = 0xff;
            }
            break;
        }
    }
    music->Name = Name;
    SyncFlashTableBack();
}

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
    Music *temp1, *temp2, *temp3;

    FlashTable = (uint8_t *)MyFlash_GetPageAddress(63);
    SyncFlashTable();

    if (MyFlash_IsEmptyPage(63))    // 如果表头为空，则初始化表头并添加内置音效
    {
        Music_InitFlashTable();
        Music_Save("_Connect_", t3, t4, 6);
        Music_Save("_Open_", t1, t2, 6);
        SyncFlashTableBack();
    }

    MusicHead.next = &MusicTail;
    MusicHead.last = NULL;
    MusicTail.last = &MusicHead;
    MusicTail.next = NULL;

    for (i = 64; i > 0; i --)
    {
        if (FlashTableTemp[16 * (i - 1)] == 2)
        {
            Music_Create((char *)(FlashTableTemp + 16 * (i - 1) + 1), (uint16_t *)MyFlash_GetPageAddress((i - 1)), (uint16_t *)(MyFlash_GetPageAddress((i - 1)) + 512));
        }
    }

    temp1 = Music_Search("Record - 1");
    if (temp1 != NULL)
    {
        Music_Pop("Record - 1");
        Music_ListAdd(temp1);
        temp2 = Music_Search("Record - 2");
        if (temp2 != NULL)
        {
            Music_Pop("Record - 2");
            Music_ListAdd(temp2);
            temp3 = Music_Search("Record - 3");
            if (temp3 != NULL)
            {
                Music_Pop("Record - 3");
                Music_ListAdd(temp3);
            }
        }
    }
    
    MusicTickFunc = CreateTickFunc(Music_PlayTickFunc, 0, False);
    current_music = Music_GetNode(0);
    play_state = 0;
    Music_Play();
}

/**
 * @brief 初始化闪存表头数据
 */
void Music_InitFlashTable(void)
{
    uint8_t i;

    for (i = 0; i < 64; i ++)
    {
        if (MyFlash_IsEmptyPage(i))
        {
            FlashTableTemp[16 * i] = 0;
        }
        else
        {
            FlashTableTemp[16 * i] = 1;
        }
    }
}

/**
 * @brief 统计音频时刻长度
 * @param music 音频指针
 * @return 音频长度
 */
uint32_t Music_GetLength(Music *music)
{
    uint16_t i;
    uint32_t result = 0;

    for (i = 0; music->Note[i] != 0xffff; i ++)
    {
        result += music->Tick[i];
    }
    return result;
}

/**
 * @brief 获取音符轨道长度
 * @param music 音频指针
 * @return 音符轨道长度
 */
uint8_t Music_GetNoteLength(Music *music)
{
    uint8_t i;

    for (i = 0; music->Note[i] != 0xffff; i ++);
    return i;
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
 * @brief 获取音频
 * @param Name 音频名称
 * @return 音频指针
 */
Music *Music_Search(char *Name)
{
    Music *music = MusicHead.next;

    while (music != &MusicTail)
    {
        if (strcmp(music->Name, Name) == 0)
        {
            return music;
        }
        music = music->next;
    }
    return NULL;
}

/**
 * @brief 获取存储音频数据的页数
 * @param music 音频指针
 * @return 页数，如果未找到返回 0xff
 */
uint8_t Music_GetPage(Music *music)
{
    uint8_t i;

    for (i = 0; i < 64; i ++)
    {
        if (FlashTableTemp[16 * i] == 2 && strcmp((char *)(FlashTableTemp + 16 * i + 1), music->Name) == 0)
        {
            return i;
        }
    }
    return 0xff;
}

/**
 * @brief 存储音频数据到闪存
 * @param Name 音频名称，长度（包括末位 \0 停止符）必须小于等于 15
 * @param Note 音符轨道
 * @param Tick 时刻轨道
 * @param Length 数据长度
 * @return 保存成功返回页数，否则返回 0xff
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
    if (flag == 0) return 0xff;

    FlashTableTemp[16 * page] = 2;
    for (i = 0; Name[i] != '\0'; i ++)
    {
        FlashTableTemp[16 * page + 1 + i ] = Name[i];
    }
    FlashTableTemp[16 * page + 1 + i] = '\0';
    SyncFlashTableBack();
    MyFlash_ErasePage(page);
    MyFlash_WriteData_16x(MyFlash_GetPageAddress(page), Note, Length);
    MyFlash_WriteData_16x(MyFlash_GetPageAddress(page) + 512, Tick, Length);
    return page;
}

Music *current_music = NULL;     // 当前播放的音频指针
uint8_t play_mode = 0;    // 播放模式，0 为播放单曲，1 为单曲循环，2 为列表播放，3 为列表循环，4 为随机播放
uint8_t play_state = 0;    // 播放状态，0 为停止，1 为播放，2 为暂停
uint32_t play_tick;    // 播放时刻
uint8_t play_index;    // 播放索引
uint8_t play_speed = 1;    // 播放速度，0 为 0.5x，1 为 1x，2 为 2x

/**
 * @brief 播放音乐
 */
void Music_Play(void)
{
    Buzzer_SmoothOff();
    if (current_music != NULL)    // 播放音乐
    {
        if (play_state == 0)    // 开始播放
        {
            LaunchTickFunc(MusicTickFunc);
            play_index = 0;
            play_tick = MyTime_GetTick();
            play_state = 1;
        }
        else if (play_state == 2)    // 恢复播放
        {
            LaunchTickFunc(MusicTickFunc);
            play_tick = MyTime_GetTick();
            play_state = 1;
        }
    }
}

/**
 * @brief 暂停音乐
 */
void Music_Pause(void)
{
    uint8_t i;
    
    Buzzer_SmoothOn();
    play_state = 2;
    StopTickFunc(MusicTickFunc);
    for (i = 0; i < 8; i ++)
    {
        Buzzer_UnPlay(i);
    }
}

/**
 * @brief 停止音乐
 */
void Music_Stop(void)
{
    uint8_t i;

    Buzzer_SmoothOn();
    play_state = 0;
    current_music = NULL;
    StopTickFunc(MusicTickFunc);

    for (i = 0; i < 8; i ++)
    {
        Buzzer_UnPlay(i);
    }
}

/**
 * @brief 播放下一首
 */
void Music_Next(void)
{
    Music *temp;

    if (play_mode == 0)    // 播放单曲
    {
        Music_Stop();
    }
    else if (play_mode == 1)    // 单曲循环
    {
        play_state = 0;
        Music_Play();
    }
    else if (play_mode == 2)    // 列表播放
    {
        temp = current_music->next;
        Music_Stop();
        if (temp != &MusicTail)
        {
            current_music = temp;
            Music_Play();
        }
    }
    else if (play_mode == 3)    // 列表循环
    {
        temp = current_music->next;
        Music_Stop();
        if (temp != &MusicTail)
        {
            current_music = temp;
            Music_Play();
        }
        else
        {
            current_music = MusicHead.next;
            Music_Play();
        }
    }
    else if (play_mode == 4)    // 随机播放
    {
        Music_Stop();
        current_music = Music_GetNode(MyMisc_RandomInt(0, Music_GetLinkLength() - 1));
        Music_Play();
    }
}

void Music_PlayTickFunc(uint16_t tick)
{
    uint32_t t_ = MyTime_GetTick() - play_tick, t;

    if (play_speed == 0)
    {
        t = t_ / 2;
    }
    else if (play_speed == 2)
    {
        t = t_ * 2;
    }
    else
    {
        t = t_;
    }

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
        play_tick += t_;
        if (current_music->Note[play_index] == 0xFFFF)
        {
            Music_Next();
        }
    }
}

uint8_t music_ba_state = 0;    // 0：等待发送音频总数；1：等待发送音轨段数；2：等待发送音频名称；3：等待发送音符轨道数据；4：等待发送时刻轨道数据；5：等待发送结束信号
uint8_t current_music_index = 0;    // 当前发送的音频索引
uint8_t music_num;    // 音频总数
uint8_t music_sub_num;    // 音轨段数
uint8_t current_sub;    // 当前发送的音轨段数
/**
 * @brief 备份音频
 */
void Music_BackupMusic(void)
{
    uint16_t i;

    switch (music_ba_state)
    {
    case 0:    // 发送音频总数
        music_num = Music_GetLinkLength();
        current_music_index = 0;
        Serial_SendDataPackage(&music_num, 1);
        music_ba_state = 1;
        break;
    case 1:    // 发送音轨段数
        for (i = 0; Music_GetNode(current_music_index)->Note[i] != 0xffff; i ++);
        music_sub_num = i / 64 + 1;
        current_sub = 0;
        Serial_SendDataPackage(&music_sub_num, 1);
        music_ba_state = 2;
        break;
    case 2:    // 发送音频名称
        for (i = 0; Music_GetNode(current_music_index)->Name[i] != '\0'; i ++);
        Serial_SendDataPackage((uint8_t *)(Music_GetNode(current_music_index)->Name), i + 1);
        music_ba_state = 3;
        break;
    case 3:    // 发送音符轨道数据
        for (i = 0; Music_GetNode(current_music_index)->Note[i + 64 * current_sub] != 0xffff && i < 64; i ++);
        Serial_SendDataPackage((uint8_t *)(Music_GetNode(current_music_index)->Note + 64 * current_sub), i * 2);
        current_sub ++;
        if (current_sub == music_sub_num)
        {
            music_ba_state = 4;
            current_sub = 0;
        }
        break;
    case 4:    // 发送时刻轨道数据
        for (i = 0; Music_GetNode(current_music_index)->Tick[i + 64 * current_sub] != 0xffff && i < 64; i ++);
        Serial_SendDataPackage((uint8_t *)(Music_GetNode(current_music_index)->Tick + 64 * current_sub), i * 2);
        current_sub ++;
        if (current_sub == music_sub_num)
        {
            music_ba_state = 1;
            current_sub = 0;
            current_music_index ++;
            if (current_music_index == music_num)
            {
                music_ba_state = 5;
            }
        }
        break;
    case 5:    // 发送结束信号
        Serial_SendDataPackage((uint8_t *)"\x00", 1);
        music_ba_state = 0;
        break;
    default:
        break;
    }
}

uint16_t record_note_temp[256];    // 音符轨道录音缓冲
uint16_t record_tick_temp[256];    // 时刻轨道录音缓冲
uint8_t record_index;    // 录音索引
/**
 * @brief 初始化录音缓冲
 */
void Music_InitRecord(void)
{
    uint16_t i;

    record_index = 0;
    for (i = 0; i < 256; i ++)
    {
        record_note_temp[i] = 0xffff;
        record_tick_temp[i] = 0xffff;
    }
}

/**
 * @brief 记录音符
 * @param note 操作音符，0 ~ 7 为播放音符，8 ~ 10 为关闭音符
 * @param tick 持续时刻，不超过 256 刻
 */
void Music_RecordNote(uint8_t note, uint8_t tick)
{
    record_note_temp[record_index] = note;
    record_tick_temp[record_index] = tick;
    record_index ++;
}

/**
 * @brief 保存录音，录音名称为 “Record - 1”，最多保存 3 条
 *     如果 Record - 1 已存在，则将原 Record - 1 名称改为 Record - 2，
 *     如果 Record - 2 已存在，则将原 Record - 2 名称改为 Record - 3，
 *     如果 Record - 3 已存在，则删除原 Record - 3
 */
void Music_RecordSave(void)
{
    Music *temp1 = Music_Search("Record - 1");
    Music *temp2 = Music_Search("Record - 2");
    Music *temp3 = Music_Search("Record - 3");
    uint8_t temp, i;

    if (record_index == 0)
    {
        return;
    }

    if (temp1 != NULL)
    {
        if (temp2 != NULL)
        {
            if (temp3 != NULL)
            {
                temp = Music_GetPage(temp3);
                MyFlash_ErasePage(temp);
                Music_ListRemove(temp3);
                FlashTableTemp[16 * temp] = 0;
                for (i = 1; i < 16; i ++)
                {
                    FlashTableTemp[16 * temp + i] = 0xff;
                }
            }
            Music_Rename(temp2, "Record - 3");
        }
        Music_Rename(temp1, "Record - 2");
    }
    i = Music_Save("Record - 1", record_note_temp, record_tick_temp, record_index);
    if (i == 0xff)
    {
        return;
    }
    
    temp1 = Music_Create("Record - 1", (uint16_t *)MyFlash_GetPageAddress(i), (uint16_t *)(MyFlash_GetPageAddress(i) + 512));
    if (temp1 != NULL)
    {
        temp2 = Music_Search("Record - 2");
        if (temp2 != NULL)
        {
            Music_Pop("Record - 2");
            Music_ListAdd(temp2);
            temp3 = Music_Search("Record - 3");
            if (temp3 != NULL)
            {
                Music_Pop("Record - 3");
                Music_ListAdd(temp3);
            }
        }
    }
    SyncFlashTableBack();
}
