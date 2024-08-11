#include "servo.h"
#include "PID.h"
#include "timer.h"

//extern float hy,hp;         //云台角
/************************************************/
void Servo1RunToDegree(float degree)   //舵机1转到degree角度,degree:-90~90
{
	float pwm;
	if(degree>=-120 && degree<=120){
		pwm = 1.0394*degree + 1848.55;
		TIM_SetCompare2(TIM3,pwm);	//修改比较值，修改占空比
	}
}
void Servo2RunToDegree(float degree)   //舵机2转到degree角度,degree:0~180
{
	float pwm;
	if(degree>=-30 && degree<=210){
		pwm = -1.0306*degree + 1939.10; 
		TIM_SetCompare1(TIM4,pwm);	//修改比较值，修改占空比	
	}
}

/************************************************/
void HeadTurnToXY(float x,float y)       //云台转到水平x,俯仰y位置, x:-90~90, y:0~180
{
		Servo1RunToDegree(x);
	  Servo2RunToDegree(y);
}

/***********************************/
float invSqrt(float x)               //快速计算 1/Sqrt(x)函数
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
void CalHeadDegree(void)      //计算机体系下的云台角
{
	const float DEG2RAD = 0.0174533;   //度化弧度因子
	const float RAD2DEG = 57.29578 ;   //弧度化度因子
	float xn=cos(hy*DEG2RAD)*cos(hp*DEG2RAD);   //参考系下的云台指向向量
	float	yn=sin(hy*DEG2RAD)*cos(hp*DEG2RAD);
	float zn=sin(hp*DEG2RAD);           
	float xb,yb,zb,normalizer;  			//机体系下的云台指向向量、模长
	float cby,sby,cbp,sbp,cbr,sbr;       //姿态角的正余弦值
//	printf("xn:%f\tyn:%f\tzn:%f\r\n",xn,yn,zn);

	normalizer = invSqrt(xn*xn + yn*yn +zn*zn);   //求出向量模长的倒数		
	xn *= normalizer;	        //向量单位化
	yn *= normalizer;
	zn *= normalizer;	
	
	bp=pitch,br=roll,by=yaw;				//先算出姿态角的三角值方便计算
	cby=cos(by*DEG2RAD),sby=sin(by*DEG2RAD);       
	cbp=cos(bp*DEG2RAD),sbp=sin(bp*DEG2RAD);
	cbr=cos(br*DEG2RAD),sbr=sin(br*DEG2RAD);
//  printf("yaw:%f\tpitch:%f\troll:%f\r\n",yaw,pitch,roll);
	
	xb = cbp*cby*xn + cbp*sby*yn - sbp*zn;		//套用欧拉角旋转矩阵公式
	yb = (sbr*sbp*cby - cbr*sby)*xn + (sbr*sbp*sby + cbr*cby)*yn + sbr*cbp*zn;
	zb = (cbr*sbp*cby + sbr*sby)*xn + (cbr*sbp*sby - sbr*cby)*yn + cbr*cbp*zn;
//  printf("xb:%f\tyb:%f\tzb:%f\t",xb,yb,zb);

//	xb = (q0*q0+q1*q1-q2*q2-q3*q3)*xn + 2*(q1*q2-q0*q3)*yn + 2*(q1*q3+q0*q2)*zn;//套用四元数旋转矩阵公式	               
//	yb = 2*(q1*q2+q0*q3)*xn +(q0*q0-q1*q1+q2*q2-q3*q3)*yn + 2*(q2*q3-q0*q1)*zn;   
//	zb = 2*(q1*q3-q0*q2)*xn + 2*(q2*q3+q0*q1) *yn + (q0*q0-q1*q1-q2*q2+q3*q3)*zn;	
//	xb=yb, yb=-xb;
	
	normalizer = invSqrt(xb*xb + yb*yb +zb*zb);   //求出向量模长的倒数		
	xb *= normalizer;	        //向量单位化
	yb *= normalizer;
	zb *= normalizer;	
//  printf("normalizer:%f\r\n",normalizer);
	                                                    //利用几何关系计算机体系下的云台角 
	bhp = asin(zb)*RAD2DEG;          										
	bhy = acos( xb*invSqrt(xb*xb+yb*yb) )*RAD2DEG;		
	bhp = -bhp;									                        //加上正负号
	if(yb<0) bhy=-bhy;           
//	printf("bhp:%f\tbhy:%f\r\n",bhp,bhy);
}

/********************************************************/
void Servo_Init(void)                   //舵机时钟初始化为50Hz满幅2000
{
 	TIM3_PWM_Init(2000-1,720-1);	//72M/720=0.1Mhz的计数频率,重装载值2000，所以PWM频率为 0.1M/2000=50hz.    
	TIM4_PWM_Init(2000-1,720-1);	//72M/720=0.1Mhz的计数频率,重装载值2000，所以PWM频率为 0.1M/2000=50hz.  
}

