#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "BUZZER.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Timer.h"
#include "PWM.h"
#include "Servo.h"
#include "Motor.h"
#include "IC.h"
#include "AD.h"
#include "MyDMA.h"
#include "Serial.h"
#include "String.h"
#include "Light.h" 

int main(void)
{
	OLED_Init();
	LED_Init();
	Light_Init();
	 
	while (1)
	{
		
	}
}


