#include "stm32f10x.h"                  // Device header

/*************************************
*函  数  名	：PWM_Init()
*函数功能	：PWM初始化
*输	入	：无
*输	出	：无
*************************************/
void PWM_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//配置GPIO，PWM波形需要借用GPIO口来输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置时基单元
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;										//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;										//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	//初始化输出比较单元
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;																	//CCR
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}

/*************************************
*函  数  名	：void PWM_SetCompare_x(uint16_t Compare_x)
*函数功能	：设置x轴PWM的CCR的值
*输	入	：PID算法后的驱动值x
*输	出	：无
*************************************/
void PWM_SetCompare_x(uint16_t Compare_x)														//在运行中更改CCR的值，CCR初始化为0
{
	TIM_SetCompare2(TIM3, Compare_x);																	//这是设置CCR的值，并不是直接设置占空比
}



/*************************************
*函  数  名	：void PWM_SetCompare_y(uint16_t Compare_y)
*函数功能	：设置y轴PWM的CCR的值
*输	入	：PID算法后的驱动值y
*输	出	：无
*************************************/
void PWM_SetCompare_y(uint16_t Compare_y)
{
	TIM_SetCompare1(TIM3, Compare_y);																	//这是设置CCR的值，并不是直接设置占空比
}

void Servo_SetAngle_x(float Angle)
{
	PWM_SetCompare_x(Angle / 270 * 2000 + 500);
}

void Servo_SetAngle_y(float Angle)
{
	PWM_SetCompare_y(Angle / 270 * 2000 + 500);
}

void PWM_Limit_x(uint16_t *Compare_x){
	if(*Compare_x>1410) *Compare_x=1410;
	if(*Compare_x<1000) *Compare_x=1000;
}

void PWM_Limit_y(uint16_t *Compare_y){	
	if(*Compare_y>1600) *Compare_y=1600;
	if(*Compare_y<1300) *Compare_y=1250;
}
