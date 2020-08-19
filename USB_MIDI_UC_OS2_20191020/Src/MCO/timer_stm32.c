/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
//#include "canfestival.h"
#include "stm32f2xx.h"
#include "mcohw.h"
#include "bsp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern WORD volatile gTimCnt;

#if 0
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  setTimer
  * @param  value:Set time value 0x0000-0xffff
  * @retval NONE
  */
void setTimer(TIMEVAL value)
{
	TIM_SetAutoreload(TIM2, value);
}
/**
  * @brief  getElapsedTime
  * @param  NONE
	* @retval TIMEVAL:Return current timer value
  */
TIMEVAL getElapsedTime(void)
{
	uint16_t timer = TIM_GetCounter(TIM2);

	return timer > last_time_set ? timer - last_time_set : last_time_set - timer; 	
}
#endif
/**
  * @brief  TIM2_Configuration
  * @param  NONE
  * @retval NONE
  */
static void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 48- 1;//1M
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;//1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);
}
/**
  * @brief  NVIC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	
    //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	
	NVIC_InitStructure.NVIC_IRQChannelPriority= 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  RCC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void RCC_Configuration(void)
{
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
/**
  * @brief  TIM2_start
  * @param  NONE
  * @retval NONE
  */
void TIM2_start(void)
{
	RCC_Configuration();

	/* configure TIM2 for remote and encoder */
	NVIC_Configuration();
	TIM2_Configuration();
}
/**
  * @brief  initTimer
  * @param  NONE
  * @retval NONE
  */
void initTimer(void)
{
		TIM2_start();
}
/**
  * @brief  TIM2_IRQHandler
  * @param  NONE
  * @retval NONE
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		
		//last_time_set = TIM_GetCounter(TIM2);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		//TimeDispatch();
		
		gTimCnt++;
		
		
		//LED_flash_Dela();
	}
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
