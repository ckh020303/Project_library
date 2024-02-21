#include "mq2.h"
#include "adc.h"
#include "math.h"

#define CAL_PPM  20
#define RL  5

static float R0 = 6.00;
extern uint8_t times;

//����������ʼ��
void mq2_Init(void){
	Adc_In_Init(MQ2_GPIO,MQ2_Pin);
}

//��������ȡֵ
u16 mq2_Get_Value(void){
	return Get_Adc_Average(MQ2_Ch,10);
}

//��������һ��ȡֵ
u16 mq2_Get_One_Value(void){
	return Get_Adc(MQ2_Ch);
}

//��ȡ����Ũ��,һ�㳬��100mmp����
float mq2_Get_Smoke_concentration(void){
	float Vrl,RS,smoke_ppm;
	Vrl = (float) mq2_Get_Value();
	Vrl = 5.f * Vrl / 4095.f;
	RS = (5.f - Vrl) / Vrl * RL;
	R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
	smoke_ppm = 613.9f * pow(RS/R0, -2.074f);
	return smoke_ppm;
}

