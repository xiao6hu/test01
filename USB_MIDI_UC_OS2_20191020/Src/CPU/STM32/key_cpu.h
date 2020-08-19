/*--------------------------------------------------------------------------*/
/**@file     key.h
   @brief
   @details
   @author
   @date   2011-4-26
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef __KEY_CPU_H__
#define __KEY_CPU_H__

/* Includes ------------------------------------------------------------------*/
//#include "includes.h"

#include "stm32f2xx_hal.h"
#include "sys.h"


#define KEY1_GPIO	GPIO_PIN_7	//ESC¼ü  KEY1      
#define KEY1_PORT	GPIOC
#define KEY1_RCC	RCC_APB2Periph_GPIOC

#define KEY2_GPIO	GPIO_PIN_15  //UP¼ü   KEY2
#define KEY2_PORT	GPIOD
#define KEY2_RCC	RCC_APB2Periph_GPIOD

#define KEY3_GPIO	GPIO_PIN_14  //DOWN¼ü KEY3
#define KEY3_PORT	GPIOD
#define KEY3_RCC	RCC_APB2Periph_GPIOD

#define KEY4_GPIO	GPIO_PIN_13  //OK¼ü   KEY4
#define KEY4_PORT	GPIOD
#define KEY4_RCC	RCC_APB2Periph_GPIOD



/*************************************** END OF FILE *************************************/

#endif

