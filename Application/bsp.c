#include "bsp.h"

/*******************************************************************************
* Function Name  : system_init
* Description    : ϵͳ��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 		
void system_init(void)
{
  /* ����ö�����ͱ��� HSEStartUpStatus */
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();/* ��λϵͳʱ������*/
  RCC_HSEConfig(RCC_HSE_ON);  /* ����HSE*/
  /* �ȴ�HSE�����ȶ�*/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  /* �ж�HSE���Ƿ���ɹ����������if()�ڲ� */
  if(HSEStartUpStatus == SUCCESS)
  { /* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ 
		RCC_SYSCLK_Div1����AHBʱ�� = ϵͳʱ */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ 
	  RCC_HCLK_Div1����APB2ʱ�� = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);
    /* ѡ��PCLK1ʱ��ԴΪ HCLK��AHB�� 2��Ƶ 
		RCC_HCLK_Div2����APB1ʱ�� = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* ����FLASH��ʱ������Ϊ2 */
		// FLASH_SetLatency(FLASH_Latency_2);
    /* ʹ��FLASHԤȡ���� */
		// FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* ѡ�����໷��PLL��ʱ��ԴΪHSE 1��Ƶ��
		��Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE); /* ʹ��PLL */
    /* �ȴ�PLL����ȶ� */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* ѡ��SYSCLKʱ��ԴΪPLL */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* �ȴ�PLL��ΪSYSCLKʱ��Դ */
    while(RCC_GetSYSCLKSource() != 0x08);
  }
}

/*******************************************************************************
* Function Name  : delay_us
* Description    : ΢����ʱ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 	
void delay_us(u32 nus)  
{
	u32 temp;
	SysTick->LOAD = 9*nus;
	SysTick->VAL=0X00; 
	SysTick->CTRL=0X01; 
	do
	{
	  temp=SysTick->CTRL; 
	}while((temp&0x01)&&(!(temp&(1<<16)))); 
	 
	SysTick->CTRL=0x00;  
	SysTick->VAL =0X00;  
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : ������ʱ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 	
void delay_ms(u16 nms)  
{
	u32 temp;
	SysTick->LOAD = 9000*nms;
	SysTick->VAL=0X00; 
	SysTick->CTRL=0X01; 
	do
	{
	  temp=SysTick->CTRL; 
	}while((temp&0x01)&&(!(temp&(1<<16)))); 
	SysTick->CTRL=0x00;  
	SysTick->VAL =0X00;  
}

