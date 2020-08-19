/*********************************************************************************************************
*
* File                : main.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.32
* Version             : V1.0
* By                  : mao
* Help Web			  : http://fantin.taobao.com/
*
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"

void LED_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( LED1_RCC 
						| LED2_RCC 
						| LED3_RCC 
						| LED4_RCC , ENABLE); 						 
	/**
	*  LED1 -> PB15 , LED2 -> PB14 , LED3 -> PB13 , LED4 -> PB12
	*/					 
	GPIO_InitStructure.GPIO_Pin =  LED1_GPIO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  LED2_GPIO; 
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  LED3_GPIO; 
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  LED4_GPIO; 
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);
}

void LED_ON(uint8_t on)
{ 
	if(on==0)
		GPIO_SetBits(LED1_PORT , LED1_GPIO);
	else if(on==1)
		GPIO_SetBits(LED2_PORT , LED2_GPIO);
	else if(on==2)
		GPIO_SetBits(LED3_PORT , LED3_GPIO);
	else if(on==3)
		GPIO_SetBits(LED4_PORT , LED4_GPIO);
}

void LED_OFF(uint8_t off)
{ 
	if(off==0)
		GPIO_ResetBits(LED1_PORT , LED1_GPIO);
	else if(off==1)
		GPIO_ResetBits(LED2_PORT , LED2_GPIO);
	else if(off==2)
		GPIO_ResetBits(LED3_PORT , LED3_GPIO);
	else if(off==3)
		GPIO_ResetBits(LED4_PORT , LED4_GPIO);
}


/*************************************** END OF FILE *************************************/
