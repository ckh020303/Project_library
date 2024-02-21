#ifndef __DHT_H
#define __DHT_H   
#include "delay.h"
#include "stm32f10x.h"
//DHT11������ʪ�ȴ�������������	 
//IO��������
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}
//IO��������											   
#define	DHT11_DQ_OUT PAout(11) //���ݶ˿�	PA11 
#define	DHT11_DQ_IN  PAin(11)  //���ݶ˿�	PA11 
//IO��
#define DHT11_GPIO GPIOA
#define DHT11_PIN GPIO_Pin_11

typedef struct{
	u8 temp;
	u8 humi;
	u8 tem;
	u8 hum;
}DHT11_DATA;

typedef struct{
	DHT11_DATA data;
	u8 (*Init)(void);
	u8 (*Read_Data)(DHT11_DATA *data);
}DHT11_DEVICE;

u8 Dht11_Init(void);//��ʼ��DHT11
u8 Dht11_Read_Data(DHT11_DATA *data);//��ȡ��ʪ��
u8 Dht11_Read_Byte(void);//����һ���ֽ�
u8 Dht11_Read_Bit(void);//����һ��λ
u8 Dht11_Check(void);//����Ƿ����DHT11
void Dht11_Rst(void);//��λDHT11 
#endif

