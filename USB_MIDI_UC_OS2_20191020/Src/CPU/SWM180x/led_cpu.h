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
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

#define LED1_GPIO	GPIO_Pin_6
#define LED2_GPIO	GPIO_Pin_7
#define LED3_GPIO	GPIO_Pin_8
#define LED4_GPIO	GPIO_Pin_9

#define LED1_PORT	GPIOB
#define LED2_PORT	GPIOB
#define LED3_PORT	GPIOB
#define LED4_PORT	GPIOB

#define LED1_RCC	RCC_APB2Periph_GPIOB
#define LED2_RCC	RCC_APB2Periph_GPIOB
#define LED3_RCC	RCC_APB2Periph_GPIOB
#define LED4_RCC	RCC_APB2Periph_GPIOB


void LED_GPIO_Configuration(void);
void LED_ON(uint8_t on);
void LED_OFF(uint8_t off);

/*************************************** END OF FILE *************************************/
