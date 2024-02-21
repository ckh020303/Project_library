#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"

#define ACK	   0
#define NACK   1  //不应答或否定的应答

#define IIC_PORT	    GPIOB
#define IIC_SCL		GPIO_Pin_8 //时钟
#define IIC_SDA		GPIO_Pin_9 //数据

#define IIC_W_SCL(Bit)		GPIO_WriteBit(IIC_PORT, IIC_SCL, (BitAction)(Bit))
#define IIC_W_SDA(Bit)		GPIO_WriteBit(IIC_PORT, IIC_SDA, (BitAction)(Bit))
#define IIC_R_SDA()	        GPIO_ReadInputDataBit(IIC_PORT, IIC_SDA)
#define IIC_DELAY()         delay_us(10)

void IIC_StartBit(void);
void IIC_StopBit(void);
void IIC_SendBit(uint8_t bit);
uint8_t IIC_SendByte(uint8_t byte);
uint8_t IIC_ReceiveBit(void);
uint8_t IIC_ReceiveByte(uint8_t ack_bit);
uint8_t IIC_ReadReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data);
uint8_t IIC_WriteReg_Len(uint8_t addr,uint8_t RegAddress,uint8_t len,uint8_t *Data);
void IIC_Init(void);
#endif

