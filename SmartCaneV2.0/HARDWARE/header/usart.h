#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "stm32f10x.h" 
#include "gpio.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 256
#define EN_USART_RX 			1		//使能（1）/禁止（0）串口1接收
#define BUFLEN 256      //数组缓存大小

typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern UART_BUF buf_uart1;     //CH340
extern UART_BUF BC26_buf;     //NBIOT
//如果想串口中断接收，请不要注释以下宏定义
void Usart_Init(USART_TypeDef* USARTx, u32 bound, uint8_t PreemptionPriority);
void Usart_SendStr(USART_TypeDef* USARTx, char*SendBuf);
void Usart_SendData(USART_TypeDef* USARTx, uint8_t data);
	
#endif


