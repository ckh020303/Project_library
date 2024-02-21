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
#include "MPU6050.h"

uint8_t id;
int16_t AX, AY, AZ, GX, GY, GZ;
double X;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1,1,"ID:");
	id = MPU6050_GetID();
	OLED_ShowHexNum(1,4,id,2);
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2,1,AX,5);
		OLED_ShowSignedNum(3,1,AY,5);
		OLED_ShowSignedNum(4,1,AZ,5);
		OLED_ShowSignedNum(2,8,GX,5);
		OLED_ShowSignedNum(3,8,GY,5);
		OLED_ShowSignedNum(4,8,GZ,5);
	}
}


