#ifndef __DHT_H
#define __DHT_H   
#include "delay.h"
#include "stm32f10x.h"
//DHT11数字温湿度传感器驱动代码	 
//IO方向设置
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}
//IO操作函数											   
#define	DHT11_DQ_OUT PAout(11) //数据端口	PA11 
#define	DHT11_DQ_IN  PAin(11)  //数据端口	PA11 
//IO口
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

u8 Dht11_Init(void);//初始化DHT11
u8 Dht11_Read_Data(DHT11_DATA *data);//读取温湿度
u8 Dht11_Read_Byte(void);//读出一个字节
u8 Dht11_Read_Bit(void);//读出一个位
u8 Dht11_Check(void);//检测是否存在DHT11
void Dht11_Rst(void);//复位DHT11 
#endif

