#ifndef __OPENMV_H__
#define __OPENMV_H__

#include "sys.h"

extern int16_t OpenMv_data1; // data1
extern int16_t OpenMv_data2; // data2
extern int16_t OpenMv_data3; // data3

void OpenMv_init(u32 bound);   // 初始化
void OpenMv_sendData(u8 *str); // 发送数据包
void u1_sendData(u8 *str);

#endif
