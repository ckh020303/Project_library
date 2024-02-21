#include "stm32f10x.h"                  // Device header
#include "control.h"

float Med_Angle=1;	//��е��ֵ��---�������޸���Ļ�е��ֵ���ɡ�
float Target_Speed=0;	//�����ٶȡ�---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�

float 
	Vertical_Kp=420,//ֱ����KP��KD
	Vertical_Kd=1.3;
float 
	Velocity_Kp=-0.48,//�ٶȻ�KP��KI
	Velocity_Ki=-0.0024;

int Vertical_out,Velocity_out,Turn_out;//ֱ����&�ٶȻ�&ת�� ���������

float Vertical(float Med,float Angle,float gyro_Y);//��������
int Velocity(int Target,int encoder_left,int encoder_right);
float Turn(int gyro_Z); 

void EXTI9_5_IRQHandler(void)
{
	float PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5) != 0)
	{
		if(PBin(5)==0)
		{
			OLED_Float(0,0,Pitch,2);
			EXTI_ClearITPendingBit(EXTI_Line5);
			
			//1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
			
			//2.������ѹ��ջ������У�����������������
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);		//ֱ����
			Turn_out=Turn(gyroz);											//ת��
			
			PWM_out=Vertical_out;//�������
			
			//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out;							//����
			MOTO2=-PWM_out;							//�ҵ��
			
			OLED_Float(2,0,Velocity_out,2);
			OLED_Float(3,0,Vertical_out,2);
			
			Limit(&MOTO1,&MOTO2);	 						//PWM�޷�			
			Load(MOTO1,MOTO2);		 						//���ص�����ϡ�
		}
	}
}

//void TIM3_IRQHandler(void)		//200hz ���� 0.005s
//{
//	int PWM_out;
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
//	{
//			OLED_Float(0,0,Pitch,2);
//			//1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
//			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=Read_Speed(4);
//			
//			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//�Ƕ�
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
//			//2.������ѹ��ջ������У�����������������
//			
//			/*
//			��һ��
//			�ʣ�
//				������С��ǰ���һ��С���飬���ʹ�û�е��ֵ���㣬=-27�ȣ�Ȼ���ҽ����ӷ��ڻ�е��ֵ�ĽǶ�λ��Ȼ�󿪻���������һ��������ǰ�ܡ�Ϊʲô��
//			��
//				���������
//								��ʼ�ٶ�ƫ��=0����Ϊû�ж�����ѽ������>�⻷���=0�����ڻ������Ƕ�=0��
//								But������ʵ�ʽǶ�= -27�ȡ���>�����ڻ�����͵���(-27-0)*(-1) = +27  //KpΪ������������Ϊ-1
//								����������Ϊ����ʱ �⻷�����=��е��ֵ(�����е��ֵ���Ե�Ч˵��ʵ�ʽǶȣ��������κ�ʱ�򶼿��Ե�Ч˵�ɵġ�Ʃ���ҿ���ʱС�������ڻ�е��ֵλ��ʱ�Ͳ��ܵ�Ч˵�ɡ�)
//								=====>>>>���Կ���ʱ���ӿ϶���ǰ�ܣ�����			
//				���˼�룺
//								��ʼTarget=Real�Ϳ����ˡ�
//				���������
//								���е��ֵƩ�� = -27�ȣ���ʹ���ڻ������Ƕ�=�⻷��� + ��е��ֵ��	
//								
//			������
//			�����⻷���==��е��ֵ���������ٶ�ƫ��=0ʱ����е��ֵ=0���⻷���=0�����ڻ�����=0��
//			�����ڻ�����=0��������ʱ�򿪾��ڻ�����͵���(0-0)*(-1) = 0===>���ӱ�Ȼֱ����ֹ��ԭ�ء�
//			
//			������
//			�����⻷�����=��е��ֵ���������ٶ�ƫ��=0ʱ����е��ֵ!=0�����ҿ��ֲ��ѳ��Ӱ��ڻ�е��ֵλ�á�Ʃ���������е��ֵ=-27�ȣ��ҿ���������ʵ�ʽǶȰ���0��λ�á����ʱ
//			�ٶ�ƫ��=0���⻷���=0�����ڻ�����=0��
//			�����ڻ�����=0===>������С��ֱ����ֹ��BUT�����Ӹ������ڻ�е��ֵλ�ã���̸��ֱ����ֹ�����ӻ����������ǰ���£�ֱ�������̷�������-->
//			-->��С����ǳ��ǳ��ᣬ���п��ܾȻ����������鲻�ᣬ���Ȼ����һֱ������ǰ�ܲ���ͣ�£���С������أ�ϵͳ��������Ӧ�����ӻ�ֱ����ǰ���£�
//			�⣬���ǽ����
//			*/			
//			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
//			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			//ֱ����
//			Turn_out=Turn(gyroz);																						//ת��
//			
//			PWM_out=Vertical_out;//�������
//			//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
//			MOTO1=PWM_out-Turn_out;//����
//			MOTO2=PWM_out+Turn_out;//�ҵ��
//			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
//	}
//}


/*********************
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
*********************/
float Vertical(float Med,float Angle,float gyro_Y)
{
	float PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}


/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,Encoder_Err,EnC_Err_Lowout;
	static int PWM_out;
	float a=0.7;
	
	//1.�����ٶ�ƫ��
	Encoder_Err=((encoder_left+encoder_right)-Target);//��ȥ���--�ҵ���⣺�ܹ����ٶ�Ϊ"0"�ĽǶȣ����ǻ�е��ֵ��
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;
	//4.�����޷�
	Encoder_S=Encoder_S>7200?7200:(Encoder_S<(-7200)?(-7200):Encoder_S);
	
	//5.�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	
	return PWM_out;
}



/*********************
ת�򻷣�ϵ��*Z����ٶ�
*********************/
float Turn(int gyro_Z)
{
	float PWM_out;
	
	PWM_out=(-0.6)*gyro_Z;
	return PWM_out;
}
