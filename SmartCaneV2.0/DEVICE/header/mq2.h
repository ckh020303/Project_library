#ifndef __MQ2_H
#define __MQ2_H 

#include "stm32f10x.h"
#include "sys.h"

#define MQ2_GPIO GPIOA
#define MQ2_Pin GPIO_Pin_0
#define MQ2_Ch ADC_Channel_0

void mq2_Init(void);//����������ʼ��
u16 mq2_Get_Value(void);//��������ȡֵ
u16 mq2_Get_One_Value(void);//��������һ��ȡֵ
float mq2_Get_Smoke_concentration(void);//��ȡ����Ũ��

#endif

