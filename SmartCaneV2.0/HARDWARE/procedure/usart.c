#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

UART_BUF buf_uart1;     //CH340
UART_BUF BC26_buf;     //NBIOT

#if EN_USART_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void Usart_Init(USART_TypeDef* USARTx, u32 bound, uint8_t Priority){
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_TypeDef* GPIOx;
	uint16_t In_Pin, Out_Pin; 
	 
	if(USARTx == USART1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		GPIOx = GPIOA;
		Out_Pin = GPIO_Pin_9;
		In_Pin = GPIO_Pin_10;
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//抢占优先级
	}else if(USARTx == USART2){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART2
		GPIOx = GPIOA;
		Out_Pin = GPIO_Pin_2;
		In_Pin = GPIO_Pin_3;
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//抢占优先级
	}else if(USARTx == USART3){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3
		GPIOx = GPIOB;
		Out_Pin = GPIO_Pin_10;
		In_Pin = GPIO_Pin_11;
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//抢占优先级
	}
	
  USART_DeInit(USARTx);//复位串口
	
	GPIO_simple_Init(GPIOx,Out_Pin,GPIO_Mode_AF_PP);//USART_TX初始化
  GPIO_simple_Init(GPIOx,In_Pin,GPIO_Mode_IN_FLOATING);//USART_RX初始化

  //Usart NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   //模块内分组须保证同一个分组
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USARTx, &USART_InitStructure);     //初始化串口
	USART_Cmd(USARTx, ENABLE);                    //使能串口
	
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
}

void Usart_SendStr(USART_TypeDef* USARTx, char*SendBuf){
	while(*SendBuf){
		while((USARTx->SR&0X40)==0);//等待发送完成 
		USARTx->DR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
}

void Usart_SendData(USART_TypeDef* USARTx, uint8_t data){
	while((USARTx->SR & 0X40) == 0);
	USARTx->DR = data;
}

//串口1中断服务程序
void USART1_IRQHandler(void){
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}

void nbiot_receive_process_event(unsigned char ch )     //串口2给nbiot用
{
    if(BC26_buf.index >= BUFLEN)
    {
        BC26_buf.index = 0 ;
    }
    else
    {
        BC26_buf.buf[BC26_buf.index++] = ch;
    }
}

void USART3_IRQHandler(void)                	//串口2中断服务程序
	{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收模块返回的数据
		{    
         nbiot_receive_process_event(USART_ReceiveData(USART3));
    } 
 
} 
#endif	

