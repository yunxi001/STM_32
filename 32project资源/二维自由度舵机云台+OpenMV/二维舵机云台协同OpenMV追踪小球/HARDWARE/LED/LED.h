/*
 * @Author: _oufen
 * @Date: 2023-07-03 13:31:06
 * @LastEditTime: 2023-07-03 14:31:38
 * @Description:
 */
#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LED_RED PCout(0)
#define LED_GREEN PCout(1)
#define LED_BLUE PCout(2)

#define Buzzer_IN PDout(2)

void Other_Init(void); // LED¼°·äÃùÆ÷³õÊ¼»¯

#endif
