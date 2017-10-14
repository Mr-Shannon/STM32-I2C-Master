#include "batell.h"
#include "bsp.h"

uint8_t  data_buffer[BUFFER_LEN];
uint8_t* batell_write_data_point;
uint8_t* batell_read_data_point;
uint8_t  data1_buffer[500];

/*******************************************************************************
* Function Name  : batell_write_data
* Description    : batell发数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void batell_write_data(uint8_t addr, uint8_t *data_buffer, uint16_t length)
{
	//I2C 开始信号
	I2C_GenerateSTART(I2C1, ENABLE); 
	//开始信号的一个应答 EV5事件
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
	//发送一个7位的Batell地址用于写数据
	I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Transmitter); 
	//对发送Batell地址的应答
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));    
	//写入将要写数据的寄存器的地址
	I2C_SendData(I2C1, addr); 
	//对I2C_SendData的一个应答
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//向Batell的寄存器中写入数据
	for(uint16_t i = 0; i < length; i++)
	{
		//写入将要写数据的寄存器的地址
		I2C_SendData(I2C1, data_buffer[i]); 
		//对I2C_SendData的一个应答
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	//I2C 停止信号
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : batell_read_data
* Description    : batell发数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void batell_read_data(uint8_t addr, uint8_t* data_buffer, uint16_t len)
{
  //I2C 开始信号
  I2C_GenerateSTART(I2C1, ENABLE);
	//开始信号的一个应答 EV5事件
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  //发送一个7位的Batell地址用于写数据
  I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Transmitter);
	//对发送Batell地址的应答	
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  //写入将要读的寄存器的地址
  I2C_SendData(I2C1, addr);  
  //对I2C_SendData的一个应答
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	delay_ms(10000);
	//I2C 开始信号
  I2C_GenerateSTART(I2C1, ENABLE);
	//开始信号的一个应答 EV5事件
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  //发送一个7位的Batell地址用于读数据
  I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Receiver); 
  //对发送Batell地址的应答	
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  //开始连续读数据
	while(len)  
  {
		//如果总线上有数据，接受数据
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {  //读数据
      *data_buffer = I2C_ReceiveData(I2C1);
      data_buffer++; 
      len--;        
    } 
		if(len == 1) //最后一个数据无应答
    {	//失能I2C的应答
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      //停止信号
      I2C_GenerateSTOP(I2C1, ENABLE);
    }  
  }
  //使能I2C的应答
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void pd1_exti_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|
                         RCC_APB2Periph_AFIO, ENABLE);
	
	//GPIO
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_1; //TX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化 GPIOD
	
	//NVIC
  NVIC_InitStructure.NVIC_IRQChannel 									 = EXTI1_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 0x01;  
  NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE;   
  NVIC_Init(&NVIC_InitStructure);   
	
	//EXTI
	EXTI_InitStructure.EXTI_Line 		= EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource1);
}
uint8_t exti_flag = 0;
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
	{
		exti_flag = 1;
		printf("EXTI\t");
		batell_read_data(0xAA, data1_buffer, 9);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}     
}

