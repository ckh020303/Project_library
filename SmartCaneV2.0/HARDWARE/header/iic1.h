#ifndef __IIC1_H
#define __IIC1_H
#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"

#define ACK	   0
#define NACK   1  //不应答或否定的应答

#define IIC1_PORT	    GPIOB
#define IIC1_SCL		GPIO_Pin_0 //时钟
#define IIC1_SDA		GPIO_Pin_1 //数据

#define IIC1_W_SCL(Bit)		GPIO_WriteBit(IIC1_PORT, IIC1_SCL, (BitAction)(Bit))
#define IIC1_W_SDA(Bit)		GPIO_WriteBit(IIC1_PORT, IIC1_SDA, (BitAction)(Bit))
#define IIC1_R_SDA()	        GPIO_ReadInputDataBit(IIC1_PORT, IIC1_SDA)
#define IIC1_DELAY()         delay_us(10)

void IIC1_StartBit(void);
void IIC1_StopBit(void);
void IIC1_SendBit(uint8_t bit);
uint8_t IIC1_SendByte(uint8_t byte);
uint8_t IIC1_ReceiveBit(void);
uint8_t IIC1_ReceiveByte(uint8_t ack_bit);
uint8_t IIC1_ReadReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data);
uint8_t IIC1_WriteReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data);
void IIC1_Init(void);
#endif

