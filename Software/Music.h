#ifndef __MUSIC_H
#define __MUSIC_H


typedef struct _Music
{
    char *Name;    // 音频名称，仅包含 ASCII 字符
    uint16_t *Note;    // 音符轨道，记录音符的开启或关闭，0 ~ 7 分别对应 8 个音符播放，8 ~ 10 对应关闭，末尾有一个 255 停止符
    uint16_t *Tick;    // 时刻轨道，记录两次音符的操作时刻之差，因此理论上可以记录无限长度的音频
    struct _Music *next;    // 下一个音频
    struct _Music *last;    // 上一个音频
} Music;    // 音频数据结构体，采用有头双向链表管理

/*
为了方便闪存管理，数据按页存储
第 63 页存储表头信息，将 1024 字节划分成 64 份，每份 16 字节，存储每一页的使用情况
0 为未使用，1 为程序代码使用，2 为音频数据使用，则后 15 字节为音频名称（因此音频名称不能超过 14 字节）
存储音频数据的每一页划分成两份，前 512 字节存储音符轨道，后 512 字节存储时刻轨道，未使用的数据为 0xFFFF
*/

extern Music MusicHead, MusicTail;    // 音频链表头尾
extern uint8_t *FlashTable, FlashTableTemp[1024];    // 闪存表头指针

void Music_ListAdd(Music *music);    // 添加音频
void Music_ListRemove(Music *music);    // 移除音频
Music * Music_Create(char *Name, uint16_t *Note, uint16_t *Tick);    // 创建音频
uint8_t Music_Save(char * Name, uint16_t * Note, uint16_t * Tick, uint16_t Length);    // 存储音频数据到闪存

void SyncFlashTable(void);
void SyncFlashTableBack(void);

void Music_Init(void);    // 初始化音频模块
void Music_InitFlashTable(void);    // 初始化闪存表头数据

uint32_t Music_GetLength(Music *music);    // 获取音频长度
uint8_t Music_GetLinkLength(void);    // 获取链表长度
Music *Music_GetNode(uint8_t index);    // 获取链表节点

void Music_Play(Music *music);    // 播放音乐
void Music_Stop(void);    // 停止音乐
void Music_Pause(void);    // 暂停音乐
void Music_Next(void);    // 播放下一首

void Music_BackupMusic(void);    // 备份音频数据

#endif
