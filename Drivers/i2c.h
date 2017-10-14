#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"

void i2c1_config(uint16_t device_addr, uint32_t clock_speed );

#endif
