#ifndef __BATELL_H__
#define __BATELL_H__

#include "stm32f10x.h"
#include "stdio.h"

#define BUFFER_LEN 					500
#define BATELL_DEVICE_ADDR  0x50
#define DATA_MAX_COUNT			500

extern uint8_t data_buffer[BUFFER_LEN];

void batell_write_data(uint8_t addr, uint8_t *data_buffer, uint16_t length);
void batell_read_data(uint8_t addr, uint8_t* data_buffer, uint16_t len);
void pd1_exti_config(void);

#endif
