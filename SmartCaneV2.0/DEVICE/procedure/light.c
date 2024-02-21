#include "light.h"

GPIO_State Light_State = No_Ready;

//��ʼ���ƹ����IO��
void Light_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);    //���PB4�ڵ�Ĭ�ϸ���
	GPIO_simple_Init(Light_GPIO,Light_Pin,GPIO_Mode_Out_PP);
	GPIO_ResetBits(Light_GPIO,Light_Pin);
	Light_State = Ready;
}

//�رյƹ�
void Light_Off(void){
	if(Light_State != Ready){
		Light_Init();
	}
	GPIO_ResetBits(Light_GPIO,Light_Pin);
}

//�򿪵ƹ�
void Light_On(void){
	if(Light_State != Ready){
		Light_Init();
	}
	GPIO_SetBits(Light_GPIO,Light_Pin);
}

void Light_alarm(void){
	Light_On();
	delay_ms(1000);
	Light_Off();
	delay_ms(1000);
	Light_On();
}

