#ifndef __GY906_H
#define __GY906_H
#include "sys.h"
#include "iic1.h"

#define SA				0x00 //从机地址，单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command
#define EEPROM_ACCESS	0x20 //EEPROM access command
#define RAM_TOBJ1		0x07 //To1 address in the eeprom

typedef struct{
	float temp;
	void (*Init)(void);
	float (*ReadTemp)(void);
}GY906_DEVICE;	

u8 PEC_Calculation(u8*);
void Gy906_Init(void);
u16 Gy906_ReadMemory(u8 slaveAddress,u8 command);  //读取gy906的RAM/EEPROM的内容
float Gy906_ReadTemp(void);   //获取温度，即读取gy906的RAM0x07
#endif

