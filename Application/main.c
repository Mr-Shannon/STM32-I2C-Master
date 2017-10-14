#include "bsp.h"
#include "uart.h"
#include "i2c.h"
#include "batell.h"
#include "tools.h"

extern uint8_t exti_flag ;
extern uint8_t  data1_buffer[500];

int main(void)
{
	system_init();
	uart1_config(115200);
	i2c1_config(0x0F, 400000);
	pd1_exti_config();
	printf("I2CtoUART\n");
	
	data_buffer[0] = 0xAA;
	data_buffer[1] = 0xBB;
	data_buffer[2] = 0xCC;
	data_buffer[3] = 0xDD;
	data_buffer[4] = 0xEE;
	data_buffer[5] = 0xFF;
	while(1)
	{
	batell_read_data(0xAA, data1_buffer, 4);
	for(u8 i = 0; i < 4; i++)
				printf("%3x",data1_buffer[i]);
			printf("\n");
	
	
		delay_ms(10000);
		delay_ms(10000);
		batell_write_data(0x55, data_buffer, 1);
		delay_ms(10000);
		delay_ms(10000);
//		if(exti_flag)
//		{
//			for(u8 i = 0; i < 9; i++)
//				printf("%3x",data1_buffer[i]);
//			printf("\n");
//			exti_flag = 0;
//		}
	}
}
