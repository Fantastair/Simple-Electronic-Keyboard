#include "stm32f10x.h"
#include "MyGPIO.h"

#define I2C_SCL_GPIO    GPIOB
#define I2C_SDA_GPIO    GPIOB
#define I2C_SCL_PIN     GPIO_Pin_8
#define I2C_SDA_PIN     GPIO_Pin_9

#define MyI2C_SCL_GPIO_Init()    MyGPIO_Init(&MyGPIOB, I2C_SCL_PIN, GPIO_Mode_Out_OD, GPIO_Speed_50MHz)
#define MyI2C_SDA_GPIO_Init()    MyGPIO_Init(&MyGPIOB, I2C_SDA_PIN, GPIO_Mode_Out_OD, GPIO_Speed_50MHz)
#define MyI2C_W_SCL(x)      GPIO_WriteBit(I2C_SCL_GPIO, I2C_SCL_PIN, (BitAction)(x))
#define MyI2C_W_SDA(x)      GPIO_WriteBit(I2C_SDA_GPIO, I2C_SDA_PIN, (BitAction)(x))
#define MyI2C_R_SDA()       GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_PIN)

/**
  * @brief 软件IIC初始化
  * @param 无
  * @retval 无
  */
void MyIIC_Init(void)
{
    MyI2C_SCL_GPIO_Init();
    MyI2C_SDA_GPIO_Init();

    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/**
  * @brief 软件IIC起始信号
  * @param 无
  * @retval 无
  * @explain IIC开启需要在SCL为高(1)的时候将SDA置零(0)
  */
void MyIIC_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

/**
  * @brief 软件IIC停止信号
  * @param 无
  * @retval 无
  * @explain IIC结束需要在SCL为高(1)的时候将SDA置一(1)
  */
void MyIIC_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/**
  * @brief 软件IIC发送应答信号
  * @param AckBit 有无应答
  * @retval 无
  */
void MyIIC_SentAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);    //当发送完一个数据以后,SCL本身就是低的,所以前面不需要再给SCL低了
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}
/**
  * @brief 软件IIC接收应答信号
  * @param 无
  * @retval 无
  */
uint8_t MyIIC_ReceiveAck(void)
{
    uint8_t AckBit;

    MyI2C_W_SDA(1);    //主机主动空出SDA,从机会立刻占据,发送应答或者非应答信号
    MyI2C_W_SCL(1);    //SCL拉高以后,主机便可以去读取从机给的信号
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);    //放手SDA

    return AckBit;
}

/**
  * @brief 软件IIC发送一个字节
  *   主机在SCL(0)低的时候只会发送一位,从机在SCL(1)为高的时候一次也只接收一位
  * @param Byte 要发送的字节
  * @retval 无
  */
void MyIIC_SendByte(uint8_t Byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
    MyI2C_W_SCL(1);    //额外的一个时钟，不处理应答信号
    MyI2C_W_SCL(0);
}

/**
  * @brief 软件IIC接收一个字节
  * @param 无
  * @retval 无
  */
uint8_t MyIIC_ReceiveByte(void)
{
    uint8_t i, Byte=0x00;

    MyI2C_W_SDA(1);
    for(i=0; i<8; i++)
    { 
        MyI2C_W_SCL(1);
        if(MyI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);    //高位先行，所以右移
        }
        MyI2C_W_SCL(0);
    }

    return Byte;
}
