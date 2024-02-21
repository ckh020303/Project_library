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

void Psen_Init(void);//������������ʼ��
u16 Psen_Get_Value(void);//����������ȡֵ
u16 Psen_Get_One_Value(void);//����������һ��ȡֵ
float Psen_Get_Light_Intensity(void);//��ѹ�����ǿ��ȡֵ

#endif


