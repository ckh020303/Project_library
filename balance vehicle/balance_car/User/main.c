#include "stm32f10x.h"                  // Device header
#include "sys.h"

int main(void)
{
	delay_init();
	NVIC_Config();
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
 	while (mpu_dmp_init()!=0);
 	MPU6050_EXTI_Init();
//	Timer_Init_TIM3();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM1(1-1,7200-1);	//10000HZ
	
	while (1)
	{

	}
}
