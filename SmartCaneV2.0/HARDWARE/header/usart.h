#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "stm32f10x.h" 
#include "gpio.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 256
#define EN_USART_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define BUFLEN 256      //���黺���С

typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_BUF buf_uart1;     //CH340
extern UART_BUF BC26_buf;     //NBIOT
//����봮���жϽ��գ��벻Ҫע�����º궨��
void Usart_Init(USART_TypeDef* USARTx, u32 bound, uint8_t PreemptionPriority);
void Usart_SendStr(USART_TypeDef* USARTx, char*SendBuf);
void Usart_SendData(USART_TypeDef* USARTx, uint8_t data);
	
#endif


