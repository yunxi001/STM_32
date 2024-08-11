#include "servo.h"
#include "PID.h"
#include "timer.h"

//extern float hy,hp;         //��̨��
/************************************************/
void Servo1RunToDegree(float degree)   //���1ת��degree�Ƕ�,degree:-90~90
{
	float pwm;
	if(degree>=-120 && degree<=120){
		pwm = 1.0394*degree + 1848.55;
		TIM_SetCompare2(TIM3,pwm);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}
void Servo2RunToDegree(float degree)   //���2ת��degree�Ƕ�,degree:0~180
{
	float pwm;
	if(degree>=-30 && degree<=210){
		pwm = -1.0306*degree + 1939.10; 
		TIM_SetCompare1(TIM4,pwm);	//�޸ıȽ�ֵ���޸�ռ�ձ�	
	}
}

/************************************************/
void HeadTurnToXY(float x,float y)       //��̨ת��ˮƽx,����yλ��, x:-90~90, y:0~180
{
		Servo1RunToDegree(x);
	  Servo2RunToDegree(y);
}

/***********************************/
float invSqrt(float x)               //���ټ��� 1/Sqrt(x)����
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/********************************************/
void CalHeadDegree(void)      //�������ϵ�µ���̨��
{
	const float DEG2RAD = 0.0174533;   //�Ȼ���������
	const float RAD2DEG = 57.29578 ;   //���Ȼ�������
	float xn=cos(hy*DEG2RAD)*cos(hp*DEG2RAD);   //�ο�ϵ�µ���ָ̨������
	float	yn=sin(hy*DEG2RAD)*cos(hp*DEG2RAD);
	float zn=sin(hp*DEG2RAD);           
	float xb,yb,zb,normalizer;  			//����ϵ�µ���ָ̨��������ģ��
	float cby,sby,cbp,sbp,cbr,sbr;       //��̬�ǵ�������ֵ
//	printf("xn:%f\tyn:%f\tzn:%f\r\n",xn,yn,zn);

	normalizer = invSqrt(xn*xn + yn*yn +zn*zn);   //�������ģ���ĵ���		
	xn *= normalizer;	        //������λ��
	yn *= normalizer;
	zn *= normalizer;	
	
	bp=pitch,br=roll,by=yaw;				//�������̬�ǵ�����ֵ�������
	cby=cos(by*DEG2RAD),sby=sin(by*DEG2RAD);       
	cbp=cos(bp*DEG2RAD),sbp=sin(bp*DEG2RAD);
	cbr=cos(br*DEG2RAD),sbr=sin(br*DEG2RAD);
//  printf("yaw:%f\tpitch:%f\troll:%f\r\n",yaw,pitch,roll);
	
	xb = cbp*cby*xn + cbp*sby*yn - sbp*zn;		//����ŷ������ת����ʽ
	yb = (sbr*sbp*cby - cbr*sby)*xn + (sbr*sbp*sby + cbr*cby)*yn + sbr*cbp*zn;
	zb = (cbr*sbp*cby + sbr*sby)*xn + (cbr*sbp*sby - sbr*cby)*yn + cbr*cbp*zn;
//  printf("xb:%f\tyb:%f\tzb:%f\t",xb,yb,zb);

//	xb = (q0*q0+q1*q1-q2*q2-q3*q3)*xn + 2*(q1*q2-q0*q3)*yn + 2*(q1*q3+q0*q2)*zn;//������Ԫ����ת����ʽ	               
//	yb = 2*(q1*q2+q0*q3)*xn +(q0*q0-q1*q1+q2*q2-q3*q3)*yn + 2*(q2*q3-q0*q1)*zn;   
//	zb = 2*(q1*q3-q0*q2)*xn + 2*(q2*q3+q0*q1) *yn + (q0*q0-q1*q1-q2*q2+q3*q3)*zn;	
//	xb=yb, yb=-xb;
	
	normalizer = invSqrt(xb*xb + yb*yb +zb*zb);   //�������ģ���ĵ���		
	xb *= normalizer;	        //������λ��
	yb *= normalizer;
	zb *= normalizer;	
//  printf("normalizer:%f\r\n",normalizer);
	                                                    //���ü��ι�ϵ�������ϵ�µ���̨�� 
	bhp = asin(zb)*RAD2DEG;          										
	bhy = acos( xb*invSqrt(xb*xb+yb*yb) )*RAD2DEG;		
	bhp = -bhp;									                        //����������
	if(yb<0) bhy=-bhy;           
//	printf("bhp:%f\tbhy:%f\r\n",bhp,bhy);
}

/********************************************************/
void Servo_Init(void)                   //���ʱ�ӳ�ʼ��Ϊ50Hz����2000
{
 	TIM3_PWM_Init(2000-1,720-1);	//72M/720=0.1Mhz�ļ���Ƶ��,��װ��ֵ2000������PWMƵ��Ϊ 0.1M/2000=50hz.    
	TIM4_PWM_Init(2000-1,720-1);	//72M/720=0.1Mhz�ļ���Ƶ��,��װ��ֵ2000������PWMƵ��Ϊ 0.1M/2000=50hz.  
}

