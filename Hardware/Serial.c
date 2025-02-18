#include <stddef.h>
#include <string.h>
#include "stm32f10x.h"
#include "Serial.h"
#include "MyNVIC.h"
#include "MyGPIO.h"
#include "Buzzer.h"
#include "Keyboard.h"
#include "Page.h"
#include "frame.h"
#include "Music.h"
#include "MyOLED_Render.h"
#include "MyFlash.h"

#define BAUDRATE 115200
#define TX_GPIO MyGPIOA
#define TX_Pin GPIO_Pin_9
#define RX_GPIO MyGPIOA
#define RX_Pin GPIO_Pin_10


/**
 * @brief 初始化串口通信，全双工
 *     需要提前初始化对应的 GPIO 端口，TX 为 复用推挽模式，RX 为 浮空输入 或 上拉输入
 * @param BaudRate 波特率
 * @retval 无
 */
void __Serial_Init(uint32_t BaudRate)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief 配置串口的接收数据中断
 * @param PreemptionPriority 抢占优先级，可以为 0 ~ 15
 * @param SubPriority 响应优先级，可以为 0 ~ 15
 * @retval 无
 */
void Serial_NVIC_Config(uint8_t PreemptionPriority, uint8_t SubPriority)
{
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    MyNVIC_Init(USART1_IRQn, PreemptionPriority, SubPriority);
}


/**
 * @brief 发送 1 字节数据
 * @param Byte 数据字节
 * @retval 无
 */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
 * @brief 发送任意长度的字节数组
 * @param Array 数组首地址
 * @param Length 数组长度
 * @retval 无
 */
void Serial_SendArray(uint8_t * Array, uint32_t Length)
{
    uint32_t i;
    for (i = 0; i <  Length; i ++)
    {
        Serial_SendByte(Array[i]);
    }
}

/**
 * @brief 发送一个字符串
 * @param String 字符串首地址
 * @retval 无
 */
void Serial_SendString(char * String)
{
    uint32_t i;
    for (i = 0; String[i] != '\0'; i ++)
    {
        Serial_SendByte(String[i]);
    }
}

/**
 * @brief 以文本的形式发送一个十进制数字
 * @param Number 发送的数字
 * @param Length 数组长度
 * @retval 无
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint32_t temp = 10;
    uint8_t i;

    for (i = 0; i < Length - 2; i ++)
    {
        temp *= 10;
    }

    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(Number / temp % 10 + '0');
        temp /= 10;
    }
}


/**
 * @brief 判断是否已收到数据
 * @param USARTx 结构体指针，指定要接收的 USART 外设，x 可以为 1 ~ 3
 * @retval 1 表示已收到，0 表示未收到
 */
uint8_t Serial_DataReceived(void)
{
    return USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET;
}

/**
 * @brief 获取接收到的数据
 * @param USARTx 结构体指针，指定要接收的 USART 外设，x 可以为 1 ~ 3
 * @retval 接收到的数据
 */
uint8_t Serial_GetData(void)
{
    return USART_ReceiveData(USART1);
}

/**
 * @brief 等待接收到数据
 * @param USARTx 结构体指针，指定要接收的 USART 外设，x 可以为 1 ~ 3
 * @retval 无
 */
void Serial_WaitData(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
}


void Serial_Init(void)
{
    MyGPIO_Init(&TX_GPIO, TX_Pin, GPIO_Mode_AF_PP,  GPIO_Speed_50MHz);
    MyGPIO_Init(&RX_GPIO, RX_Pin, GPIO_Mode_IPU,  GPIO_Speed_50MHz);
    __Serial_Init(BAUDRATE);
    Serial_NVIC_Config(1, 1);
}


uint8_t SerialRecvData[256];  // 接收数据包，第一项表示总长度
uint8_t data_ready_flag = 0;  // 1 表示有新的数据可以读取
uint8_t data_recv_status = 0;  //0、1、2、3、4、5 共 6 个状态
uint8_t data_length = 0;  // 表示当前接收的数据长度

// 清空数据包接收状态
void clear_recv_status(void)
{
    SerialRecvData[0] = 0;
    data_ready_flag = 0;
    data_recv_status = 0;
    data_length = 0;
}

// 清除数据可读标志位
void clear_read_flag(void)
{
    data_ready_flag = 0;
}

/**
 * @brief 中断函数
 */
void USART1_IRQHandler(void)
{
    uint8_t temp;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        temp = Serial_GetData();
        // MySerial_SendByte(&MyUSART1, temp);

        switch (data_recv_status)
        {
            case 0:  // 等待包头 1
                if (temp == 0xff)
                {
                    data_recv_status = 1;
                }
                else
                {
                    clear_recv_status();
                }
                break;

            case 1:  // 等待包头 2
                if (temp == 0xaa)
                {
                    data_recv_status = 2;
                }
                else
                {
                    clear_recv_status();
                }
                break;

            case 2:  // 等待数据长度
                SerialRecvData[0] = temp;
                data_recv_status = 3;
                break;

            case 3:  // 等待数据接收
                SerialRecvData[data_length + 1] = temp;
                data_length ++;
                if (data_length == SerialRecvData[0])
                {
                    data_recv_status = 4;
                }
                break;

            case 4:  // 等待包尾 1
                if (temp == 0x55)
                {
                    data_recv_status = 5;
                }
                else
                {
                    clear_recv_status();
                }
                break;

            case 5:  // 等待包尾 2
                if (temp == 0x00)
                {
                    data_ready_flag = 1;
                    data_recv_status = 0;
                    data_length = 0;
                }
                else
                {
                    clear_recv_status();
                }
                break;

            default:
                break;
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/**
 * @brief 发送响应数据包
 * @param data 数据字节数组
 * @param Length 数据长度，最多 255 字节
 */
void Serial_SendDataPackage(uint8_t * data, uint16_t Length)
{
    uint16_t i;
    Serial_SendByte(0xff);
    Serial_SendByte(0xaa);
    Serial_SendByte(Length);
    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(data[i]);
    }    
    Serial_SendByte(0x55);
    Serial_SendByte(0x00);
}

uint8_t shake_hands = 0x66;
uint8_t temp_data[32];
uint16_t temp16;
uint32_t temp32;
Music *tempm;
/**
 * @brief 指令处理函数，如果指令传输完毕，则会解释并执行指令
 */
void Serial_HandleOrder(void)
{
    uint8_t i;

    if (data_ready_flag != 1) return;

    if (SerialRecvData[1] == 0x02)  // 读指令
    {
        switch (SerialRecvData[2])
        {
        case 0x00:    // 握手
            Serial_SendDataPackage(&shake_hands, 1);
            Buzzer_SetVolume(1);
            current_music = Music_GetNode(1);
            play_state = 0;
            play_mode = 0;
            play_speed = 1;
            Music_Play();
            break;
        case 0x01:    // 读取音量
            temp_data[0] = Buzzer_GetVolume();
            Serial_SendDataPackage(temp_data, 1);
            break;
        case 0x02:    // 读取按键状态
            temp16 = Keyboard_GetInput();
            temp_data[0] = (temp16 & 0xff00) >> 8;
            temp_data[1] = temp16 & 0xff;
            Serial_SendDataPackage(temp_data, 2);
            break;
        case 0x03:    // 读取 Flash 表头数据
            Serial_SendDataPackage(FlashTableTemp + 16 * SerialRecvData[3], 16);
            break;
        case 0x04:    // 备份音频数据
            Music_BackupMusic();
            break;
        case 0x05:    // 读取当前界面编号
            Serial_SendDataPackage(&PageListIndex, 1);
            break;
        case 0x06:    // 读取播放信息
            temp_data[0] = play_state;
            temp_data[1] = play_speed;
            temp_data[2] = play_mode;
            temp_data[3] = play_index;
            Serial_SendDataPackage(temp_data, 4);
            break;
        case 0x07:    // 读取歌曲信息
            if (current_music == NULL)
            {
                temp_data[0] = 0;
                Serial_SendDataPackage(temp_data, 1);
            }
            else
            {
                for (i = 0; current_music->Name[i] != '\0'; i ++)
                {
                    temp_data[i] = current_music->Name[i];
                }
                temp_data[i] = '\0';
                temp32 = Music_GetLength(current_music);
                temp_data[i + 1] = (uint8_t)((temp32 & 0xff000000) >> 24);
                temp_data[i + 2] = (uint8_t)((temp32 & 0x00ff0000) >> 16);
                temp_data[i + 3] = (uint8_t)((temp32 & 0x0000ff00) >> 8);
                temp_data[i + 4] = (uint8_t)(temp32 & 0x000000ff);
                temp_data[i + 5] = Music_GetNoteLength(current_music);
                Serial_SendDataPackage(temp_data, i + 6);
            }
        default:
            break;
        }
    }
    else if (SerialRecvData[1] == 0x12)  // 写指令
    {
        switch (SerialRecvData[2])
        {
        case 0x00:    // 模拟按键输入
            if (SerialRecvData[3] == 0x00) current_page->PageButtonDown(SerialRecvData[4]);
            else current_page->PageButtonUp(SerialRecvData[4]);
            break;
        case 0x01:    // 设置音量
            Buzzer_SetVolume(SerialRecvData[3]);
            if (current_page == &MainPage) MainPage_DrawVolumeBar();
            break;
        case 0x02:    // 写入表头数据
            for (i = 0; i < 16; i ++)
            {
                FlashTableTemp[16 * SerialRecvData[3] + i] = SerialRecvData[i + 4];
            }
            if (SerialRecvData[3] == 63) SyncFlashTableBack();
            break;
        case 0x03:    // 擦除页
            MyFlash_ErasePage(SerialRecvData[3]);
            break;
        case 0x04:    // 擦除所有页
            MyFlash_EraseAllPages();
            break;
        case 0x05:    // 写入数据
            for (i = 0; i < SerialRecvData[6]; i ++)
            {
                MyFlash_WriteData_16(MyFlash_GetPageAddress(SerialRecvData[3]) + SerialRecvData[4] * SerialRecvData[5] + i * 2, SerialRecvData[7 + i]);
            }
            break;
        case 0x06:    // 播放
            if (play_state != 1)
            {
                if (current_music == NULL)
                {
                    current_music = Music_GetNode(2);
                }
                Music_Play();
            }
            break;
        case 0x07:    // 暂停
            Music_Pause();
            break;
        case 0x08:    // 停止
            Music_Stop();
            break;
        case 0x09:    // 设置播放速度
            play_speed = SerialRecvData[3];
            break;
        case 0x0a:    // 设置播放模式
            play_mode = SerialRecvData[3];
            break;
        case 0x0b:    // 设置播放进度
            play_index = SerialRecvData[3];
            break;
        case 0x0c:    // 切换界面
            PageListIndex = SerialRecvData[3];
            SetPage(PageList[PageListIndex]);
            break;
        case 0x0d:    // 下一首
            Music_Next();
            break;
        case 0x0e:    //消音
            for (i = 0; i < 8; i ++)
            {
                Buzzer_UnPlay(i);
            }
            break;
        case 0x0f:    // 删除对应指定页的音频数据结构体（需要在清除表头数据之前完成这一步）
            if (FlashTableTemp[16 * SerialRecvData[3]] == 2)
            {
                tempm = MusicHead.next;

                while (tempm != &MusicTail)
                {
                    if (strcmp(tempm->Name, (char *)(FlashTableTemp + 16 * SerialRecvData[3] + 1)) == 0)
                    {
                        Music_ListRemove(tempm);
                        break;
                    }
                    tempm = tempm->next;
                }
            }
            break;
        case 0x10:    // 添加对应指定页的音频数据结构体（需要在添加表头数据之后完成这一步）
            if (FlashTableTemp[16 * SerialRecvData[3]] == 2)
            {
                Music_Create((char *)(FlashTableTemp + 16 * SerialRecvData[3] + 1), (uint16_t *)MyFlash_GetPageAddress(SerialRecvData[3]), (uint16_t *)(MyFlash_GetPageAddress(SerialRecvData[3]) + 512));
            }
            break;
        default:
            break;
        }
        Serial_SendByte(0x88);
    }
    clear_read_flag();
}
