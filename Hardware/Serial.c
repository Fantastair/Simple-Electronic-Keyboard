#include "stm32f10x.h"
#include "Serial.h"
#include "MyNVIC.h"
#include "MyGPIO.h"
#include "Buzzer.h"
#include "Page.h"

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
 * @param Length 数据长度
 */
void Serial_SendDataPackage(uint8_t * data, uint8_t Length)
{
    uint8_t i;
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

uint8_t shake_hands[] = {0x66};

/**
 * @brief 指令处理函数，如果指令传输完毕，则会解释并执行指令
 */
void Serial_HandleOrder(void)
{
    if (data_ready_flag != 1) return;

    if (SerialRecvData[1] == 0x02)  // 读指令
    {
        switch (SerialRecvData[2])
        {
        case 0x00:    // 握手
            Serial_SendDataPackage(shake_hands, 1);
            break;
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
        case 0x01:
            Buzzer_SetVolume(SerialRecvData[3]);
            MainPage_DrawVolumeBar();
            break;
        default:
            break;
        }
        Serial_SendByte(0x88);
    }
    clear_read_flag();
}
