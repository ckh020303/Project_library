#ifndef __BC26_H
#define __BC26_H	

#include "usart.h"
#include "string.h"
#include "delay.h"
#define BC26_Handler USART3_IRQHandler

#define PLATFORM "aliauth"
#define PRODUCEKEY "hij36YsBG6p"        //修改产品秘钥
#define DEVICENAME "BC26"           //修改设备名称
#define DEVICESECRET "7e7ab3970fb3e2a90a2621eef87ca1d4" //修改设备秘钥
#define MQTT_LINK "iot-06z00gtckmee81i.mqtt.iothub.aliyuncs.com"
#define MQTT_WAY "/sys/hij36YsBG6p/BC26/thing/event/property/post"

typedef struct{
	float temp;
	float humi;
	float STtemp;
	u16 pulse_val;
	u16 ps_val;
	u16 mq2_val;
	u16 mhrd_val;
}BC26_DATA;

typedef struct{
	BC26_DATA data;
	uint8_t (*Init)(void);
	void (*Send_Data)(BC26_DATA data);
}BC26_DEVICE;

void Clear_Buffer(void);//清空缓存	
uint8_t BC26_Init(void);
void BC26_PDPACT(void);
void BC26_RECData(void);
void BC26_RegALIYUNIOT(void);
void BC26_ALYIOTSenddata(BC26_DATA data);
void BC26_SendAT(void);

#endif

