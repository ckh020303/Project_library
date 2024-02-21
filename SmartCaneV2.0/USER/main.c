#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"

#include "gpio.h"
#include "usart.h"

#include "syn_6288.h"
#include "bc26.h"
#include "beep.h"
#include "dht.h"
#include "psen.h"
#include "pulse.h"
#include "gy906.h"
#include "mpu6050.h"
#include "light.h"

//创建对象
//BC26_DATA data = {1,2,3,4,5,6,7};
BC26_DEVICE bc26_device = {{0,0,0,0,0,0,0},BC26_Init,BC26_ALYIOTSenddata};
BEEP_DEVICE beep_device = {Beep_Init,Beep_Alert,Beep_Remind};
//DHT11_DATA dht11_data = {0,0,0,0};
DHT11_DEVICE dht11_device = {{0,0,0,0},Dht11_Init,Dht11_Read_Data};
PSEN_DEVICE psen_device = {0,Psen_Init,Psen_Get_One_Value};
SYN_DEVICE syn_device = {0,"[v15][t2]初始化完成",Syn_Init,Syn_Voice};
PULSE_DEVICE pulse_device = {0,Pulse_Init,pulse_Get_Heart_Rate};
GY906_DEVICE gy906_device = {0,Gy906_Init,Gy906_ReadTemp};
MPU_DEVICE mpu_device = {{0,0,0,0,0,0,0,0},MPU6050_Init,MPU6050_FallDetection,MPU6050_GetAngle};
LIGHT_DEVICE light_device = {Light_Init,Light_Off,Light_On,Light_alarm};

//函数句柄
TaskHandle_t pxDetectionDataTaskHandle;
TaskHandle_t pxDetectionFallTaskHandle;
TaskHandle_t pxInitTaskHandle;
TaskHandle_t pxAlarmTaskHandle;

void Delay(u32 count){
   u32 i=0;
   for(;i<count;i++);
}

void vDetectionDataTask(void * p){
	for(;;){
		taskENTER_CRITICAL();
		dht11_device.Read_Data(&dht11_device.data);
		taskEXIT_CRITICAL();
		
		taskENTER_CRITICAL();
		psen_device.data = psen_device.Get_Value();
		taskEXIT_CRITICAL();
		
		taskENTER_CRITICAL();
		pulse_device.Get_Value(&pulse_device.data);
		taskEXIT_CRITICAL();
		
		taskENTER_CRITICAL();
		gy906_device.temp = gy906_device.ReadTemp();
		taskEXIT_CRITICAL();
		
		printf("温度:%d,湿度:%d,光照:%d,脉搏:%d,体温:%.2f\r\n",dht11_device.data.temp,dht11_device.data.humi,psen_device.data,pulse_device.data,gy906_device.temp);
		
		taskENTER_CRITICAL();
		bc26_device.data.temp=dht11_device.data.temp;
		bc26_device.data.humi=dht11_device.data.humi;
		bc26_device.data.ps_val=psen_device.data;
		bc26_device.data.pulse_val=pulse_device.data;
		bc26_device.data.STtemp=gy906_device.temp;
		
		bc26_device.Send_Data(bc26_device.data);
		taskEXIT_CRITICAL();
		
		vTaskDelay(2000);
	}
}

void vDetectionFallTask(void * p){
	for(;;){
		if(!mpu_device.FallDetection()){
			xTaskNotifyGive(pxAlarmTaskHandle);
		}
		if(psen_device.Get_Value()>3500){
			light_device.On();
		}else{
			light_device.Off();
		}
	}
}

void vAlarmTask(void * p){
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY)){
			taskENTER_CRITICAL();
			syn_device.data = "[v15][t2]有人跌倒！！";
			while(1){
				printf("跌倒！！\r\n");
				syn_device.Play(syn_device.Music, syn_device.data);
				mpu_device.data = mpu_device.GetAngle();
				if(mpu_device.data.pitch>-65&&mpu_device.data.pitch<65&&mpu_device.data.roll>-65&&mpu_device.data.roll<65){
					taskEXIT_CRITICAL();
					break;
				}
				beep_device.Remind();
				if(psen_device.Get_Value()>3500){
					light_device.Alarm();
				}else{
					light_device.Off();
				}
			}
		}
	}
}

void vInitTask(void * p){
	taskENTER_CRITICAL();
	
	delay_init();
	Usart_Init(USART1,115200,8);
	
	while(BC26_Init());
	printf("ok\r\n");
	
	dht11_device.Init();
	psen_device.Init();
	mpu_device.Init();
	syn_device.Init();
	syn_device.Play(syn_device.Music, syn_device.data);
	beep_device.Init();
	light_device.Init();
	pulse_device.Init();
	gy906_device.Init();
	
	
	xTaskCreate(vDetectionDataTask,"DetectionDataTask",100,NULL,1,&pxDetectionDataTaskHandle);
	xTaskCreate(vDetectionFallTask,"DetectionFallTask",100,NULL,1,&pxDetectionFallTaskHandle);
	xTaskCreate(vAlarmTask,"AlarmTask",100,NULL,3,&pxAlarmTaskHandle);
	
	vTaskDelete(pxInitTaskHandle);
	
	taskEXIT_CRITICAL();
}

int main(void){
#ifdef DEBUG
	debug();
#endif
//	delay_init();
//	Usart_Init(USART1,115200,8);
	
//	Usart_Init(USART1,115200,8);
//	bc26_device.Init();
//	mpu_device.Init();
//	while(1){
//		mpu_device.data = MPU6050_GetData();
//		printf("x=%d,y=%d,z=%d\r\n",mpu_device.data.a_x,mpu_device.data.a_y,mpu_device.data.a_z);
//	}
	xTaskCreate(vInitTask,"InitTask",100,NULL,3,&pxInitTaskHandle);
//	xTaskCreate(	(TaskFunction_t) task1Printf,
//							(const char *)  "task1",
//							(uint16_t) 100,
//							(void *) NULL,
//							(UBaseType_t) 1,
//							(TaskHandle_t) * &xHandleTask1);
//	
	vTaskStartScheduler();
	return 0;
}
