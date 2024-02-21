#ifndef __PULSE_H
#define __PULSE_H 

#include "adc.h"
#include "delay.h"

#define SAMPLE_PERIOD	25  	// ���β�������(ms)
#define BUFF_SIZE		40		// ���ݻ�������С

#define PULSE_GPIO GPIOA
#define PULSE_Pin GPIO_Pin_4
#define PULSE_Ch ADC_Channel_4

typedef struct{
	u16 data;
	void (*Init)(void);
	void (*Get_Value)(u16 *BPM);
}PULSE_DEVICE;

void Pulse_Init(void);//���ʴ�������ʼ��
u16 Pulse_Get_One_Value(void);//���ʴ�����ȡһ��ֵ
u16 Pulse_Get_Value(void);//���ʴ�����ȡֵ
u16 get_Array_Max(u16 *array, u8 size);
u16 get_Array_Min(u16 *array, u8 size);
void pulse_Get_Heart_Rate(u16 *BPM);

extern u16 pulse_readData, pulse_preReadData;		   		// ��ȡ���� AD ֵ
extern u16 pulse_timeCount;       // �������ڼ�������
extern u16 pulse_firstTimeCount;   // ��һ������ʱ��
extern u16 pulse_secondTimeCount;   // �ڶ�������ʱ��
extern u16 pulse_data[BUFF_SIZE];   // �������ݻ���
extern u16 PRE_PULSE;		// ��Ч����
extern u16 PULSE;        // ��ǰ��Ч����
extern u8 pulse_index; 				// ���������±�
extern u16 pulse_max, pulse_min, pulse_mid;			// �����С���м�ֵ
extern u16 pulse_filter;				// �˲�ֵ
extern u16 pulseCount;	// ��������

#endif

