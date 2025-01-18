// 前置头文件：MyGPIO.h
#ifndef __MYI2C_H
#define __MYI2C_H

uint8_t MyI2C_R_SDA(void);
void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
void MyIIC_Init(void);
void MyIIC_Start(void);
void MyIIC_Stop(void);
void MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);
void MyIIC_SentAck(uint8_t AckBit);
uint8_t MyIIC_ReceiveAck(void);

#endif
