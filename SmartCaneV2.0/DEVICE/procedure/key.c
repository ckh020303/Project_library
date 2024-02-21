#include "key.h"

void Key_Init(void){
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_simple_Init(KEY_GPIO,KEY_BIT0,GPIO_Mode_IPU);
	GPIO_simple_Init(KEY_GPIO,KEY_BIT1,GPIO_Mode_IPU);
	GPIO_simple_Init(KEY_GPIO,KEY_BIT2,GPIO_Mode_IPU);
	GPIO_simple_Init(KEY_GPIO,KEY_EN,GPIO_Mode_IPU);
	
	GPIO_EXTILineConfig(KEY_EXIT_GPIO,KEY_EXIT_PIN);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   //模块内分组须保证同一个分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


uint8_t Key_Read(void){
	uint8_t Key_Value,Bit0,Bit1,Bit2;
	Bit0 = GPIO_ReadInputDataBit(KEY_GPIO,KEY_BIT0);
	Bit1 = GPIO_ReadInputDataBit(KEY_GPIO,KEY_BIT1);
	Bit2 = GPIO_ReadInputDataBit(KEY_GPIO,KEY_BIT2);
	Key_Value = (Bit0 | (Bit1 << 1) | (Bit2 << 2));
	return Key_Value;
}

void KEY_Handler(void);

