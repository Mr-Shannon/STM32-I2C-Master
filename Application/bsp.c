#include "bsp.h"

/*******************************************************************************
* Function Name  : system_init
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 		
void system_init(void)
{
  /* 定义枚举类型变量 HSEStartUpStatus */
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();/* 复位系统时钟设置*/
  RCC_HSEConfig(RCC_HSE_ON);  /* 开启HSE*/
  /* 等待HSE起振并稳定*/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  /* 判断HSE起是否振成功，是则进入if()内部 */
  if(HSEStartUpStatus == SUCCESS)
  { /* 选择HCLK（AHB）时钟源为SYSCLK 1分频 
		RCC_SYSCLK_Div1——AHB时钟 = 系统时 */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* 选择PCLK2时钟源为 HCLK（AHB） 1分频 
	  RCC_HCLK_Div1——APB2时钟 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);
    /* 选择PCLK1时钟源为 HCLK（AHB） 2分频 
		RCC_HCLK_Div2——APB1时钟 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* 设置FLASH延时周期数为2 */
		// FLASH_SetLatency(FLASH_Latency_2);
    /* 使能FLASH预取缓存 */
		// FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* 选择锁相环（PLL）时钟源为HSE 1分频，
		倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE); /* 使能PLL */
    /* 等待PLL输出稳定 */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* 选择SYSCLK时钟源为PLL */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* 等待PLL成为SYSCLK时钟源 */
    while(RCC_GetSYSCLKSource() != 0x08);
  }
}

/*******************************************************************************
* Function Name  : delay_us
* Description    : 微秒延时
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
* Description    : 毫秒延时
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

