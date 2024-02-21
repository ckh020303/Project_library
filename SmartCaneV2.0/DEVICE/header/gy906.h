#ifndef __GY906_H
#define __GY906_H
#include "sys.h"
#include "iic1.h"

#define SA				0x00 //�ӻ���ַ������MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
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
u16 Gy906_ReadMemory(u8 slaveAddress,u8 command);  //��ȡgy906��RAM/EEPROM������
float Gy906_ReadTemp(void);   //��ȡ�¶ȣ�����ȡgy906��RAM0x07
#endif

