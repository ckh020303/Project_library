#include "beep.h"

GPIO_State Beep_State = No_Ready;

void Beep_Init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	GPIO_simple_Init(BEEP_GPIO,BEEP_PIN,GPIO_Mode_AF_PP);
	 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse=50;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure); 
	
	Beep_State = Ready;
}	

void Beep_Alert(uint16_t times){
	if(Beep_State == No_Ready){
		Beep_Init();
	}
	TIM_Cmd(TIM3,ENABLE);
	if(times != 0){
		delay_ms(times);
		TIM_Cmd(TIM3,DISABLE);
	}
}

void Beep_Remind(){
	if(Beep_State == No_Ready){
		Beep_Init();
	}
	TIM_Cmd(TIM3,ENABLE);
	delay_ms(1000);
	TIM_Cmd(TIM3,DISABLE);
	delay_ms(500);
	TIM_Cmd(TIM3,ENABLE);
	delay_ms(1000);
	TIM_Cmd(TIM3,DISABLE);
	delay_ms(500);
}

