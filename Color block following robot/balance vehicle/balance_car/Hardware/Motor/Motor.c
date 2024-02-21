#include "stm32f10x.h"                  // Device header
#include "Motor.h"

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

int abs(int p)								//绝对值函数
{
	int q;
	q=p>0?p:(-p);
	return q;
}

void Limit(int *motoA,int *motoB)			//限幅
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

void Load(int moto1, int moto2)				//控制正反转
{
	//1.研究正负号，对应正反转
	if(moto2>0)
    	Bin1=1,Bin2=0;
	else 				
    	Bin1=0,Bin2=1;	
  	//2.研究PWM值
	TIM_SetCompare4(TIM1,abs(moto2));
	
	//1.研究正负号，对应正反转
	if(moto1>0)	
    	Ain1=1,Ain2=0;//正转
	else 				
    	Ain1=0,Ain2=1;//反转
	//2.研究PWM值
	TIM_SetCompare1(TIM1,abs(moto1));
}

