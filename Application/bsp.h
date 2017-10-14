#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f10x.h"

void system_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);

#endif

