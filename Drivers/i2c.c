#include "i2c.h"

/*******************************************************************************
* Function Name  : i2c1_config
* Description    : i2c1配置
* Input          : device_addr:器件地址
									 clock_speed:总线时钟速度
* Output         : None
* Return         : None
*******************************************************************************/
void i2c1_config(uint16_t device_addr, uint32_t clock_speed )
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	//GPIO
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	//I2C
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode 								= I2C_Mode_I2C;      
	I2C_InitStructure.I2C_DutyCycle 					= I2C_DutyCycle_2;  
	I2C_InitStructure.I2C_OwnAddress1 				= device_addr; 					 
	I2C_InitStructure.I2C_Ack 								= I2C_Ack_Enable ;  
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed 					= clock_speed;						 
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);   
}
