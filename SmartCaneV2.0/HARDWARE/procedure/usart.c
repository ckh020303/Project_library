#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

UART_BUF buf_uart1;     //CH340
UART_BUF BC26_buf;     //NBIOT

#if EN_USART_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void Usart_Init(USART_TypeDef* USARTx, u32 bound, uint8_t Priority){
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_TypeDef* GPIOx;
	uint16_t In_Pin, Out_Pin; 
	 
	if(USARTx == USART1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		GPIOx = GPIOA;
		Out_Pin = GPIO_Pin_9;
		In_Pin = GPIO_Pin_10;
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//��ռ���ȼ�
	}else if(USARTx == USART2){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//ʹ��USART2
		GPIOx = GPIOA;
		Out_Pin = GPIO_Pin_2;
		In_Pin = GPIO_Pin_3;
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//��ռ���ȼ�
	}else if(USARTx == USART3){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3
		GPIOx = GPIOB;
		Out_Pin = GPIO_Pin_10;
		In_Pin = GPIO_Pin_11;
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Priority ;//��ռ���ȼ�
	}
	
  USART_DeInit(USARTx);//��λ����
	
	GPIO_simple_Init(GPIOx,Out_Pin,GPIO_Mode_AF_PP);//USART_TX��ʼ��
  GPIO_simple_Init(GPIOx,In_Pin,GPIO_Mode_IN_FLOATING);//USART_RX��ʼ��

  //Usart NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   //ģ���ڷ����뱣֤ͬһ������
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure);     //��ʼ������
	USART_Cmd(USARTx, ENABLE);                    //ʹ�ܴ���
	
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
}

void Usart_SendStr(USART_TypeDef* USARTx, char*SendBuf){
	while(*SendBuf){
		while((USARTx->SR&0X40)==0);//�ȴ�������� 
		USARTx->DR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
}

void Usart_SendData(USART_TypeDef* USARTx, uint8_t data){
	while((USARTx->SR & 0X40) == 0);
	USARTx->DR = data;
}

//����1�жϷ������
void USART1_IRQHandler(void){
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}

void nbiot_receive_process_event(unsigned char ch )     //����2��nbiot��
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

void USART3_IRQHandler(void)                	//����2�жϷ������
	{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //����ģ�鷵�ص�����
		{    
         nbiot_receive_process_event(USART_ReceiveData(USART3));
    } 
 
} 
#endif	

