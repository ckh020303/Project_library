#include "bc26.h"

uint8_t err;
char *strx,*strx1;
char regstr[BUFLEN];
char atstr[BUFLEN];

char CGPADDR[]="AT+CGPADDR=1\r\n";
char QMTCFG[]="AT+QMTCFG=ALIAUTH,0,\"hij36YsBG6p\",\"BC26\",\"7e7ab3970fb3e2a90a2621eef87ca1d4\"\r\n";
char QMTOPEN[]="AT+QMTOPEN=0,\"iot-06z00cm626d015p.mqtt.iothub.aliyuncs.com\",1883\r\n";
char QMTCONN[]="AT+QMTCONN=0,\"BC26\"\r\n";

//清空BC26接收的缓存
void Clear_Buffer(void)	{
    printf(BC26_buf.buf);  //清空前打印信息
	delay_ms(20);
    BC26_buf.index=0;
    memset(BC26_buf.buf,0,BUFLEN);
}

//BC26初始化
uint8_t  BC26_Init(void){
    int errcount = 0;
    err = 0;    //判断模块卡是否就绪最重要
	Usart_Init(USART3,115200,8);
	
    Usart_SendStr(USART3,"ATi\r\n");
	delay_ms(300);
    strx=strstr((const char*)BC26_buf.buf,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        printf("\r\n单片机正在连接到模块...\r\n");
        Clear_Buffer();	
        Usart_SendStr(USART3,"ATi\r\n");
		delay_ms(300);
        strx=strstr((const char*)BC26_buf.buf,(const char*)"OK");
    }
	
	Usart_SendStr(USART3,"AT+CIMI\r\n");
	delay_ms(300);
    Clear_Buffer();
	
	Usart_SendStr(USART3,CGPADDR);
	delay_ms(300);
    Clear_Buffer();
	
	Usart_SendStr(USART3,"AT+CGATT?\r\n");
	delay_ms(300);
    strx=strstr((const char*)BC26_buf.buf,(const char*)"+CGATT: 1");
    Clear_Buffer();	
    while(strx==NULL)
    {	
        Usart_SendStr(USART3,"AT+CGATT?\r\n");
		delay_ms(300);
        strx=strstr((const char*)BC26_buf.buf,(const char*)"+CGATT: 1");//返回 +CGATT: 1
		Clear_Buffer();
    }
	
	Usart_SendStr(USART3,QMTCFG);
	delay_ms(300);
    Clear_Buffer();
	
	BC26_SendAT();
	
	return err;
}

void BC26_SendAT(void)
{
	int errcount=0;
    while(strx1==NULL)
    {
        errcount++;
        Usart_SendStr(USART3,QMTOPEN);
		delay_ms(1000);
        strx=strstr((const char*)BC26_buf.buf,(const char*)"+QMTOPEN: 0,0");//返回 +QMTOPEN: 0,0
		Clear_Buffer();
		if(strx!=NULL)
		{
			Usart_SendStr(USART3,QMTCONN);
			delay_ms(300);
			strx1=strstr((const char*)BC26_buf.buf,(const char*)"+QMTOPEN: 0,0");//返回 +QMTOPEN: 0,0
			Clear_Buffer();
			break;
		}
        if(errcount>20)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }
}

void BC26_ALYIOTSenddata(BC26_DATA data){
	sprintf(atstr,"AT+QMTPUB=0,1,1,0,\
	\"/sys/hij36YsBG6p/BC26/thing/event/property/post\",\"{\"params\":\
	{\"Temp\":%.1f,\"Humi\":%.1f,\"ps_val\":%d,\"pulse_val\":%d,\"STtemp\":%.1f}}\"\r\n",
	data.temp,data.humi,data.ps_val,data.pulse_val,data.STtemp);
	Usart_SendStr(USART3,atstr);
	delay_ms(300);
    strx = strstr((const char*)BC26_buf.buf,(const char*)"+QMTPUB: 0,1,0");
	Clear_Buffer();
	while(strx==NULL)
	{
		Usart_SendStr(USART3,atstr);
		delay_ms(300);
		strx = strstr((const char*)BC26_buf.buf,(const char*)"+QMTPUB: 0,1,0");
		Clear_Buffer();
	}
}
