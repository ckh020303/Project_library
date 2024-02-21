#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"  
#include "malloc.h"  

//#define ver_ID 1
//#define hor_ID 9
//		
//float target_x = 103.0,					//λ��Ŀ��ֵ
//	  target_y = 21.0;
//		
//int temp_h=110,							//ԭ�����Ƕ�
//	temp_v=91;

//float sp_x, sp_y;					//xy���������ֵ	
//		
//float kp = -0.5,  ki= 0,  kd= 0.04;	//PID����

//int servo_x, servo_y;				//��ת�Ƕ�

//int motor_180to270(int angle);
//void Servo_reset(void);

//int main(void)
//{
//	float sx,sy;
//	
//	//��ʼ������
//	i2c_CfgGpio();
//	delay_init();  	    //��ʼ����ʱ����
//	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
//	USART2_init();
//	
//	while(!IIC_Servo(ver_ID,motor_180to270(90)));		//����,����
//	
//	delay_ms(100);
//	Servo_reset();
//	delay_ms(1000);
//	
//	Cx = 100;
//	Cy = 100;
//	
//	while(1)
//	{
//		int i=0;
//		for(i=0;i<=20;i++){
//			servo_x = PWM_GetAngle(500+i*100);
//			IIC_Servo(ver_ID,motor_180to270(servo_x));
//			delay_ms(1000);
//		}
//		
//		printf("Cx:%d,Cy:%d\n",Cx,Cy);
//		printf("1111\r\n");

//		sp_x = PID_x(target_x, Cx, kp, ki, kd);
//		sp_y = PID_y(target_y, Cy, kp, ki, kd);
//		
//		printf("spx:%.2f,spy:%.2f\n",sp_x,sp_y);
//		
//		sx = sp_x + PWM_GetCRR(temp_v);
//		sy = sp_y + PWM_GetCRR(temp_h);
//		
//		printf("spx:%.2f,spy:%.2f\n",sx,sy);
//		
//		servo_x = PWM_GetAngle(sx);
//		servo_y = PWM_GetAngle(sy);
//		
//		printf("servo_x:%d,servo_y:%d\n",servo_x,servo_y);

//		IIC_Servo(ver_ID,motor_180to270(servo_x));
//		IIC_Servo(hor_ID,motor_180to270(servo_y));
//	}
//}

//void Servo_reset(void)
//{
//	while(!IIC_Servo(hor_ID,motor_180to270(temp_h)));
//	while(!IIC_Servo(ver_ID,motor_180to270(temp_v)));
//}

//int motor_180to270(int angle){
//	return angle*180/270;
//}
