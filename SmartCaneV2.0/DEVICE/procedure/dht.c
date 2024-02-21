#include "dht.h"
#include "usart.h"

//初始化DHT11
//返回0:不存在
//返回1:存在 
u8 Dht11_Init(void){
	GPIO_simple_Init(DHT11_GPIO,DHT11_PIN,GPIO_Mode_Out_PP);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	
	Dht11_Rst();
	return Dht11_Check();
}

//读取温湿度
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,读取失败;1,正常
u8 Dht11_Read_Data(DHT11_DATA *data){
 	u8 buf[5],i;
	__set_PRIMASK(1);
	Dht11_Rst();
	if(Dht11_Check()==0){
		//读取40位数据
		for(i=0;i<5;i++){
			buf[i]=Dht11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){
			data->humi=buf[0];
			data->hum=buf[1];
			data->temp=buf[2];
			data->tem=buf[3];
			__set_PRIMASK(0);
			return 1;
		}
	}
	__set_PRIMASK(0);
	return 0;	    
}

//读出一个字节
//返回值：读到的数据
u8 Dht11_Read_Byte(void){        
	u8 i,dat=0;
	for (i=0;i<8;i++) {
		dat<<=1; 
		dat|=Dht11_Read_Bit();
	}						    
	return dat;
}

//读出一个位
//返回值：1/0
u8 Dht11_Read_Bit(void){
	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN) return 1;
	return 0;
}

//检测是否存在DHT11
u8 Dht11_Check(void){   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//复位DHT11
void Dht11_Rst(void){                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT=0; 	//拉低DQ
	delay_ms(20);    	//拉低至少18ms
	DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}

