#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"

void Servo_Init(void);

void ServoRunToDegree(u8 servo,float degree);
void HeadTurnToXY(float x,float y);
void HeadPointToVector(float x,float y);
void CalHeadDegree(void);
float invSqrt(float x);  
#endif
