#include "batell.h"
#include "bsp.h"

uint8_t  data_buffer[BUFFER_LEN];
uint8_t* batell_write_data_point;
uint8_t* batell_read_data_point;
uint8_t  data1_buffer[500];

/*******************************************************************************
* Function Name  : batell_write_data
* Description    : batell������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void batell_write_data(uint8_t addr, uint8_t *data_buffer, uint16_t length)
{
	//I2C ��ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE); 
	//��ʼ�źŵ�һ��Ӧ�� EV5�¼�
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
	//����һ��7λ��Batell��ַ����д����
	I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Transmitter); 
	//�Է���Batell��ַ��Ӧ��
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));    
	//д�뽫Ҫд���ݵļĴ����ĵ�ַ
	I2C_SendData(I2C1, addr); 
	//��I2C_SendData��һ��Ӧ��
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//��Batell�ļĴ�����д������
	for(uint16_t i = 0; i < length; i++)
	{
		//д�뽫Ҫд���ݵļĴ����ĵ�ַ
		I2C_SendData(I2C1, data_buffer[i]); 
		//��I2C_SendData��һ��Ӧ��
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	//I2C ֹͣ�ź�
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : batell_read_data
* Description    : batell������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void batell_read_data(uint8_t addr, uint8_t* data_buffer, uint16_t len)
{
  //I2C ��ʼ�ź�
  I2C_GenerateSTART(I2C1, ENABLE);
	//��ʼ�źŵ�һ��Ӧ�� EV5�¼�
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  //����һ��7λ��Batell��ַ����д����
  I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Transmitter);
	//�Է���Batell��ַ��Ӧ��	
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  //д�뽫Ҫ���ļĴ����ĵ�ַ
  I2C_SendData(I2C1, addr);  
  //��I2C_SendData��һ��Ӧ��
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	delay_ms(10000);
	//I2C ��ʼ�ź�
  I2C_GenerateSTART(I2C1, ENABLE);
	//��ʼ�źŵ�һ��Ӧ�� EV5�¼�
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  //����һ��7λ��Batell��ַ���ڶ�����
  I2C_Send7bitAddress(I2C1, BATELL_DEVICE_ADDR, I2C_Direction_Receiver); 
  //�Է���Batell��ַ��Ӧ��	
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  //��ʼ����������
	while(len)  
  {
		//��������������ݣ���������
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {  //������
      *data_buffer = I2C_ReceiveData(I2C1);
      data_buffer++; 
      len--;        
    } 
		if(len == 1) //���һ��������Ӧ��
    {	//ʧ��I2C��Ӧ��
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      //ֹͣ�ź�
      I2C_GenerateSTOP(I2C1, ENABLE);
    }  
  }
  //ʹ��I2C��Ӧ��
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
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ�� GPIOD
	
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

