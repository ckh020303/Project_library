#include "stm32f10x.h"                  // Device header
#include "Encoder.h"

void Encoder_TIM2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//GPIO初始化	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);				//定时器初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,			//设置编码器模式
                                TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
								
	TIM_ICStructInit(&TIM_ICInitStructure);							//输入捕获初始化
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update); 							//清除标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);						//中断配置
	
	TIM_SetCounter(TIM2, 0);										//清除计数
	
	TIM_Cmd(TIM2, ENABLE);											//开启定时器
	
}

void Encoder_TIM4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//GPIO初始化	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);				//定时器初始化
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,			//设置编码器模式
                                TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
								
	TIM_ICStructInit(&TIM_ICInitStructure);							//输入捕获初始化
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);							//清除标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);						//中断配置
	
	TIM_SetCounter(TIM4, 0);										//清除计数
	
	TIM_Cmd(TIM4, ENABLE);											//开启定时器
	
}

int Read_Speed(int TIMx)										//获取定时器计数值
{
	int value_1;
	value_1 = 0;
	switch(TIMx)
	{
		case 2:	value_1 = (short) TIM_GetCounter(TIM2); TIM_SetCounter(TIM2, 0); break;
		case 4:	value_1 = (short) TIM_GetCounter(TIM4); TIM_SetCounter(TIM4, 0); break;
		default: value_1 = 0;
	}
	
	return value_1;
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != 0)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != 0)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
