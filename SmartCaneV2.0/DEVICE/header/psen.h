#ifndef __PSEN_H
#define __PSEN_H 
#include "adc.h"

#define PSEN_GPIO GPIOA
#define PSEN_Pin GPIO_Pin_0
#define PSEN_Ch ADC_Channel_0
#define e 2.7182

typedef struct{
	u16 data;
	void (*Init)(void);
	u16 (*Get_Value)(void);
}PSEN_DEVICE;

void Psen_Init(void);//光敏传感器初始化
u16 Psen_Get_Value(void);//光敏传感器取值
u16 Psen_Get_One_Value(void);//光敏传感器一次取值
float Psen_Get_Light_Intensity(void);//电压求光照强度取值

#endif


