#include "psen.h"

//������������ʼ��
void Psen_Init(void){
	Adc_In_Init(PSEN_GPIO,PSEN_Pin);
}

//����������ȡֵ
u16 Psen_Get_Value(void){
	return Get_Adc_Average(PSEN_Ch,10);
}

//����������һ��ȡֵ
u16 Psen_Get_One_Value(void){
	return Get_Adc(PSEN_Ch);
}

//��ѹ�����ǿ��ȡֵ
float Psen_Get_Light_Intensity(void);
