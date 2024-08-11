#ifndef __VOFA_H
#define __VOFA_H
#include "sys.h"

void RawData_Test(void);
void FireWater_Test(void);
void Float_to_Byte(float f, unsigned char byte[]);
void Byte_to_Float(float *f, unsigned char byte[]);
void JustFloat_Test(void);

void Float_to_Byte(float f, unsigned char byte[]);
void vofa_sendData(float a, float b, float c);

/**注意以下为调试程序*/
// RawData_Test();
// FireWater_Test();
// JustFloat_Test();
// vofa_sendData(1, 2, 3);
/*测试时使用即可*/
#endif
