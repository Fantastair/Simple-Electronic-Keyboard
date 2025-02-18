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
extern Music *current_music;    // 当前播放的音频
extern uint8_t play_mode;    // 播放模式，0 为播放单曲，1 为单曲循环，2 为列表播放，3 为列表循环，4 为随机播放
extern uint8_t play_state;    // 播放状态，0 为停止，1 为播放，2 为暂停
extern uint8_t play_index;    // 播放音符索引
extern uint32_t play_tick;    // 播放时刻
extern uint8_t play_speed;    // 播放速度，0 为 0.5x，1 为 1x，2 为 2x
extern uint16_t record_note_temp[256];    // 音符轨道录音缓冲
extern uint16_t record_tick_temp[256];    // 时刻轨道录音缓冲

void Music_ListAdd(Music *music);
void Music_ListRemove(Music *music);
Music * Music_Pop(char *Name);
void Music_Insert(Music *music, Music *after);
Music * Music_Create(char *Name, uint16_t *Note, uint16_t *Tick);
uint8_t Music_Save(char * Name, uint16_t * Note, uint16_t * Tick, uint16_t Length);
void Music_Rename(Music *music, char *Name);

void SyncFlashTable(void);
void SyncFlashTableBack(void);

void Music_Init(void);
void Music_InitFlashTable(void);

uint32_t Music_GetLength(Music *music);
uint8_t Music_GetNoteLength(Music *music);
uint8_t Music_GetLinkLength(void);
Music *Music_GetNode(uint8_t index);
Music *Music_Search(char *Name);
uint8_t Music_GetPage(Music *music);

void Music_Play(void);
void Music_Stop(void);
void Music_Pause(void);
void Music_Next(void);

void Music_BackupMusic(void);

void Music_InitRecord(void);
void Music_RecordNote(uint8_t note, uint8_t tick);
void Music_RecordSave(void);

#endif
