#include "stm32f10x.h"
#include "MyI2C.h"

uint8_t OLED_GRAM[8][128];    //显存缓存

/**
  * @brief 向 OLED 写入命令
  * @param IIC_Byte 写入的命令代码
  * @retval 无
  */
void MyOLED_Write_Command(uint8_t Command)
{
    MyIIC_Start();
    MyIIC_SendByte(0x78);    //从机地址
    MyIIC_SendByte(0x00);    //写命令
    MyIIC_SendByte(Command); //写入命令
    MyIIC_Stop();
}

/**
  * @brief 设置 OLED 起始光标位置
  * @param Y 以左上角为原点，向下方向的像素坐标，步长为8，范围：0~7
  * @param X 以左上角为原点，向右方向的像素坐标，范围：0~127
  * @retval 无
  */
void MyOLED_SetCursor(uint8_t Y, uint8_t X)
{
    MyOLED_Write_Command(0xB0 | Y);                   // 设置Y位置
    MyOLED_Write_Command(0x10 | ((X & 0xF0) >> 4));   // 设置X位置高4位
    MyOLED_Write_Command(0x00 | (X & 0x0F));          // 设置X位置低4位
}
 
/**
  * @brief 向 OLED 写入数据
  * @param Data 写入的数据
  * @retval 无
  */
void MyOLED_Write_Data(uint8_t Data)
{
    MyIIC_Start();
    MyIIC_SendByte(0x78);    //从机地址
    MyIIC_SendByte(0x40);    //写数据
    MyIIC_SendByte(Data);    //写命令
    MyIIC_Stop();            //发送停止信号
}

/**
  * @brief  清除OLED显存缓存
  * @param  无
  * @retval 无
  */
void MyOLED_Clear_GRAM(void)
{
    uint8_t i, j;

    for (j = 0; j < 8; j++)
    {
        for(i = 0; i < 128; i++)
        {
            OLED_GRAM[j][i] = 0x00;
        }
    }
}

/**
  * @brief  将OLED显存缓存写入显存，刷新屏幕内容
  *   显存缓存的内容不会立即显示在屏幕上，需要调用此函数才行
  *   这么设计是为了能够精确到像素级的内容布局，以及后续的帧率优化
  * @param  无
  * @retval 无
  */
void MyOLED_Flip(void)
{
    uint8_t i, j;

    for (j = 0; j < 8; j++)
    {
        MyOLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            MyOLED_Write_Data(OLED_GRAM[j][i]);
        }
    }
}

/**
  * @brief  将部分OLED显存缓存写入显存，刷新一个矩形区域内的屏幕内容
  *   如果只需要更新部分屏幕内容，此函数会比MyOLED_Flip更快
  *   此函数的矩形参数不同于其他函数，垂直方向将8个像素划分为一页，共8页
  * @param Left 矩形左端像素坐标
  * @param Top_Page 矩形上端页索引，0 ~ 7
  * @param Width 矩形像素宽度
  * @param Page_Height 矩形页高度
  * @retval 无
  */
void MyOLED_Update(uint8_t Left, uint8_t Top_Page, uint8_t Width, uint8_t Page_Height)
{
    uint8_t i, j;

    for (j = Top_Page; j < Top_Page + Page_Height; j++)
    {
        MyOLED_SetCursor(j, Left);
        for (i = Left; i < Left + Width; i++)
        {
            MyOLED_Write_Data(OLED_GRAM[j][i]);
        }
    }
}

/**
  * @brief  将OLED显存缓存反相（0变1，1变0）后写入显存，刷新屏幕内容
  * @param  无
  * @retval 无
  */
void MyOLED_Flip_Reverse(void)
{
    uint8_t i, j;

    for (j = 0; j < 8; j++)
    {
        MyOLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            MyOLED_Write_Data(~OLED_GRAM[j][i]);
        }
    }
}

/**
  * @brief  将部分OLED显存缓存反相（0变1，1变0）后写入显存，刷新一个矩形区域内的屏幕内容
  * @param Left 矩形左端像素坐标
  * @param Top_Page 矩形上端页索引，0 ~ 7
  * @param Width 矩形像素宽度
  * @param Page_Height 矩形页高度
  * @retval 无
  */
void MyOLED_Update_Reverse(uint8_t Left, uint8_t Top_Page, uint8_t Width, uint8_t Page_Height)
{
    uint8_t i, j;

    for (j = Top_Page; j < Top_Page + Page_Height; j++)
    {
        MyOLED_SetCursor(j, Left);
        for (i = Left; i < Left + Width; i++)
        {
            MyOLED_Write_Data(~OLED_GRAM[j][i]);
        }
    }
}

/**
  * @brief  初始化OLED屏幕
  * @param  clear 确定是否在初始化之后清除屏幕内容，将该值设为 1 则清除屏幕
  * @retval 无
  */
void MyOLED_Init(void)
{
	uint16_t i, j;
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}

    MyIIC_Init();                //软件IIC协议初始化

    MyOLED_Write_Command(0xAE);    //关闭显示
    MyOLED_Write_Command(0xD5);    //设置显示时钟分频比/振荡器频率
    MyOLED_Write_Command(0x80);
    MyOLED_Write_Command(0xA8);    //设置多路复用率
    MyOLED_Write_Command(0x3F);
    MyOLED_Write_Command(0xD3);    //设置显示偏移
    MyOLED_Write_Command(0x00);
    MyOLED_Write_Command(0x40);    //设置显示开始行
    MyOLED_Write_Command(0xA1);    //设置左右方向，0xA1正常 0xA0左右反置
    MyOLED_Write_Command(0xC8);    //设置上下方向，0xC8正常 0xC0上下反置
    MyOLED_Write_Command(0xDA);    //设置COM引脚硬件配置
    MyOLED_Write_Command(0x12);
    MyOLED_Write_Command(0x81);    //设置对比度控制
    MyOLED_Write_Command(0xCF);
    MyOLED_Write_Command(0xD9);    //设置预充电周期
    MyOLED_Write_Command(0xF1);
    MyOLED_Write_Command(0xDB);    //设置VCOMH取消选择级别
    MyOLED_Write_Command(0x30);
    MyOLED_Write_Command(0xA4);    //设置整个显示打开/关闭
    MyOLED_Write_Command(0xA6);    //设置正常/倒转显示
    MyOLED_Write_Command(0x8D);    //设置充电泵
    MyOLED_Write_Command(0x14);
    MyOLED_Write_Command(0xAF);    //开启显示
}

/**
  * @brief  清除一个矩形内的OLED显存缓存
  * @param  Left 矩形左端像素坐标
  * @param  Top 矩形上端像素坐标
  * @param  Width 矩形像素宽度
  * @param  Height 矩形像素高度
  * @retval 无
  */
void MyOLED_Clear_GRAM_Rect(uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height)
{
    uint8_t page_num = (Top + Height - 1)/8 - Top/8 + 1, page = Top/8,
    up_offset = Top % 8,    //低位1的个数
    down_offset = 7 - (Top + Height - 1) % 8,    //高位1的个数
    up_data = (1<<up_offset)-1, down_data = ((1<<down_offset)-1) << (8-down_offset),
    i, j;

    switch (page_num)
    {
    case 1:
        j = up_data | down_data;
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] &= j;
        }
        break;
    case 2:
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] &= up_data;
            OLED_GRAM[page + 1][i] &= down_data;
        }
        break;
    default:
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] &= up_data;
            OLED_GRAM[page + page_num - 1][i] &= down_data;
            for (j = page + 1; j < page + page_num - 1; j++)
            {
                OLED_GRAM[j][i] = 0x00;
            }
        }
        break;
    }
}

/**
  * @brief  用 1 填充一个矩形内的OLED显存缓存
  * @param  Left 矩形左端像素坐标
  * @param  Top 矩形上端像素坐标
  * @param  Width 矩形像素宽度
  * @param  Height 矩形像素高度
  * @retval 无
  */
void MyOLED_Fill_GRAM_Rect(uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height)
{
    uint8_t page_num = (Top + Height - 1)/8 - Top/8 + 1, page = Top/8,
    up_offset = 8 - Top % 8,    //高位1的个数
    down_offset = (Top + Height - 1) % 8 + 1,    //低位1的个数
    up_data = ((1<<up_offset)-1) << (8-up_offset), down_data = (1<<down_offset)-1,
    i, j;

    switch (page_num)
    {
    case 1:
        j = up_data & down_data;
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] |= j;
        }
        break;
    case 2:
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] |= up_data;
            OLED_GRAM[page + 1][i] |= down_data;
        }
        break;
    default:
        for (i = Left; i < Left + Width; i++)
        {
            OLED_GRAM[page][i] |= up_data;
            OLED_GRAM[page + page_num - 1][i] |= down_data;
            for (j = page + 1; j < page + page_num - 1; j++)
            {
                OLED_GRAM[j][i] = 0xff;
            }
        }
        break;
    }
}

/** 
  * @brief 覆盖显存缓存上的一块数据，大小 Length * 8
  * @param data 数据内容，每元素占一字节的任意长度数组
  * @param Left 开始覆盖的左端像素坐标，0 ~ 127
  * @param Top 开始覆盖的上端像素坐标，0 ~ 63
  * @param Length 数据长度（字节数）
  * @retval 无
  */
void MyOLED_Blit_GRAM(const uint8_t data[], uint8_t Left, uint8_t Top, uint8_t Length)
{
    uint8_t page = Top / 8, offset = Top % 8,
    i;

    if (offset == 0)
    {
        for (i = 0; i < Length; i++)
        {
            OLED_GRAM[page][Left + i] |= data[i];
        }
    } else
    {
        uint8_t down_mask = (1 << offset) - 1,
        up_mask = 0xff ^ down_mask;

        for (i = 0; i < Length; i++)
        {
            OLED_GRAM[page][Left + i] |= (OLED_GRAM[page][Left + i]&down_mask) | (data[i]<<offset);
            OLED_GRAM[page+1][Left + i] |= (OLED_GRAM[page+1][Left + i]&up_mask) | (data[i]>>(8-offset));
        }
    }
}

/**
  * @brief 修改显存缓存中一个点的数据
  * @param X 横坐标，0 ~ 127
  * @param Y 纵坐标，0 ~ 63
  * @param Bit 数据，0 或 1
  * @retval 无
  */
void MyOLED_Blit_Point(uint8_t X, uint8_t Y, uint8_t Bit)
{
    uint8_t Data = 1 << Y % 8;

    if (Bit == 1)
        OLED_GRAM[Y / 8][X] |= Data;
    else
        OLED_GRAM[Y / 8][X] &= ~Data;
}
