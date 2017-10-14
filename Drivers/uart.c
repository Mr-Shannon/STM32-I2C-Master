#include "uart.h"

/*******************************************************************************
* Function Name  : fputc
* Description    : printf�Ӷ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)   //Printf
{
		USART_SendData(USART1,(uint8_t)ch);
    while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE)); 
    return ch;
}

/*******************************************************************************
* Function Name  : uart1_config
* Description    : USART1�ĳ�ʼ������
* Input          : baud_rate:���ڲ�����
* Output         : None
* Return         : None
*******************************************************************************/
void uart1_config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
                         RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//GPIO
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_9; //TX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ�� GPIOA
  
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; //RX
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING ; //��������  
	GPIO_Init(GPIOA,&GPIO_InitStructure);   //��ʼ�� GPIOB 
	
	//UART
  USART_InitStructure.USART_BaudRate						=	baud_rate;
  USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode								= USART_Mode_Rx|USART_Mode_Tx;
  USART_InitStructure.USART_Parity							= USART_Parity_No;  //��У��
  USART_InitStructure.USART_StopBits						= USART_StopBits_1;
  USART_InitStructure.USART_WordLength					= USART_WordLength_8b;
  USART_Init(USART1,&USART_InitStructure);  
	USART_Cmd(USART1,ENABLE);

}
 
