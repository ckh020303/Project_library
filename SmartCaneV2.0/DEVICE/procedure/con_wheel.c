#include "con_wheel.h"

GPIO_State Wheel_State = No_Ready;

//��ʼ��IO��
void Wheel_Init(void){
	GPIO_simple_Init(Wheel_GPIO,Wheel_Pin,GPIO_Mode_Out_PP);
	GPIO_ResetBits(Wheel_GPIO,Wheel_Pin);
	Wheel_State = Ready;
}

//�ر������ֱ���
void Wheel_Off(void){
	if(Wheel_State != Ready){
		Wheel_Init();
	}
	GPIO_ResetBits(Wheel_GPIO,Wheel_Pin);
}

//�������ֱ���
void Wheel_On(void){
	if(Wheel_State != Ready){
		Wheel_Init();
	}
	GPIO_SetBits(Wheel_GPIO,Wheel_Pin);
}

