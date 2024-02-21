#include "bsp_i2c_gpio.h"
#include "stm32f10x.h"
#include <string.h>
#include "delay.h"

uint8_t ack;                 /*应答标志位*/
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
        
    IAR工程编译效率高，不能设置为7
	*/
	for (i = 0; i < 50; i++);
}


void delay(int z)
{   
	  int i;
	  int j;
	  for(i = 0;i<z;i++)
		{
			for(j = 0;j<50;j++)
			{
					i2c_Delay();
				}
		}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void Start_I2c(void)
{
  macI2C_SDA_1();         /*发送起始条件的数据信号*/
  macI2C_SCL_1();
  i2c_Delay();    
  macI2C_SDA_0();
  i2c_Delay();       
  macI2C_SCL_0();       /*钳住I2C总线，准备发送或接收数据 */
  i2c_Delay();
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Stop_I2c(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	macI2C_SDA_0();
	macI2C_SCL_1();
	while(macI2C_SCL_READ() == 0){macI2C_SCL_1();}
	i2c_Delay();
	macI2C_SDA_1();
	while(macI2C_SDA_READ() == 0){macI2C_SDA_1();}
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/

void  I2C_SendByte(unsigned char  c)
{
 unsigned char  i;
 
 for(i=0;i<8;i++)  /*要传送的数据长度为8位*/
    {
     if((c<<i)&0x80) macI2C_SDA_1();   /*判断发送位*/
       else  macI2C_SDA_0();                
     i2c_Delay();
     macI2C_SCL_1();               /*置时钟线为高，通知被控器开始接收数据位*/
		 while(macI2C_SCL_READ() == 0){macI2C_SCL_1();}
      i2c_Delay();
			
     macI2C_SCL_0();
    }
    i2c_Delay();
    macI2C_SDA_1() ;                /*8位发送完后释放数据线，准备接收应答位*/
    i2c_Delay();   
    macI2C_SCL_1();
		while(macI2C_SCL_READ() == 0){macI2C_SCL_1();}
    i2c_Delay();
    if(macI2C_SDA_READ())ack=0;     
       else ack=1;        /*判断是否接收到应答信号*/
    macI2C_SCL_0();
    i2c_Delay();

}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/



unsigned char I2C_RcvByte(void)
{
  unsigned char  retc=0,i=0; 
  macI2C_SDA_1();                     /*置数据线为输入方式*/

  for(i=0;i<8;i++)
      {

        i2c_Delay();           
        macI2C_SCL_0();                  /*置时钟线为低，准备接收数据位*/
				while(macI2C_SCL_READ() == 1){macI2C_SCL_0() ;}
        i2c_Delay();
        macI2C_SCL_1();                  /*置时钟线为高使数据线上数据有效*/
				while(macI2C_SCL_READ() == 0){macI2C_SCL_1();}
        i2c_Delay();
        retc=retc<<1;
        if(macI2C_SDA_READ())retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
        i2c_Delay();
      } 
  macI2C_SCL_0();    
   i2c_Delay();
  return(retc);
}



/********************************************************************
                     应答子函数
********************************************************************/
void Ack_I2c(uint8_t a)
{  
  if(a==0) macI2C_SDA_0();              /*在此发出应答或非应答信号 */
  else macI2C_SDA_1();				  /*0为发出应答，1为非应答信号 */
  i2c_Delay();      
  macI2C_SCL_1();
  i2c_Delay(); 
  macI2C_SCL_0();                    /*清时钟线，住I2C总线以便继续接收*/
  i2c_Delay();   
}



uint8_t IIC_Servo(unsigned char servonum,unsigned char angle)
{
  
    Start_I2c();          		//启动总线
	I2C_SendByte(I2C_ADDR);     //发送器件地址
	if(ack==0)return(0);
   	I2C_SendByte(servonum);    //发送数据
	if(ack==0)return(0);
   	I2C_SendByte(angle);    //发送数据
	if(ack==0)return(0);

	Stop_I2c();               //结束总线
	delay_ms(100);
   	return(1);

}




/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(macRCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = macI2C_SCL_PIN | macI2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(macGPIO_PORT_I2C, &GPIO_InitStructure);

}


