#include "shock.h"

GPIO_State Shock_State = No_Ready;

void Shock_Init(void){
	GPIO_simple_Init(SHOCK_GPIO,SHOCK_PIN,GPIO_Mode_Out_PP);
	GPIO_ResetBits(SHOCK_GPIO,SHOCK_PIN);
	Shock_State = Ready;
}
void Shock_Alert(uint16_t times){
	if(Shock_State == No_Ready){
		Shock_Init();
	}
	SHOCK=1;
	if(times != 0){
		delay_ms(times);
		SHOCK=0;
	}
}
void Shock_Remind(void){
	if(Shock_State == No_Ready){
		Shock_Init();
	}
	SHOCK=1;
	delay_ms(100);
	SHOCK=0;
	delay_ms(100);
	SHOCK=1;
	delay_ms(100);
	SHOCK=0;
}

