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
#include "canfestival.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

TIM_HandleTypeDef TIM4_HandleStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  setTimer
  * @param  value:Set time value 0x0000-0xffff
  * @retval NONE
  */
void setTimer(TIMEVAL value)
{
	//TIM_SetAutoreload(TIM4, value);

	__HAL_TIM_SET_AUTORELOAD(&TIM4_HandleStructure,value);
}
/**
  * @brief  getElapsedTime
  * @param  NONE
	* @retval TIMEVAL:Return current timer value
  */
TIMEVAL getElapsedTime(void)
{
	//uint16_t timer = TIM_GetCounter(TIM4);


	uint16_t timer =__HAL_TIM_GET_COUNTER(&TIM4_HandleStructure);

	return timer > last_time_set ? timer - last_time_set : last_time_set - timer; 	
}

/**
  * @brief  TIM4_Configuration
  * @param  NONE
  * @retval NONE
  */
static void TIM4_Configuration(void)
{
	/* Time Base configuration */
	HAL_TIM_Base_DeInit(&TIM4_HandleStructure);
	TIM4_HandleStructure.Instance=TIM4;
	TIM4_HandleStructure.Init.Prescaler = 60- 1;//1M
	TIM4_HandleStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM4_HandleStructure.Init.Period = 1000-1;//1ms
	TIM4_HandleStructure.Init.ClockDivision = TIM_CLOCKPRESCALER_DIV1;
	TIM4_HandleStructure.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&TIM4_HandleStructure);
    /* TIM IT enable */
	__HAL_TIM_CLEAR_FLAG(&TIM4_HandleStructure, TIM_FLAG_CC1);										/* ??3y¨°?3??D??¡À¨º?? */
	__HAL_TIM_ENABLE_IT(&TIM4_HandleStructure, TIM_IT_CC1);
	__HAL_TIM_DISABLE(&TIM4_HandleStructure);

}
/**
  * @brief  NVIC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void NVIC_Configuration(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}
/**
  * @brief  RCC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void RCC_Configuration(void)
{
	/* TIM4 clock enable */
	__HAL_RCC_TIM4_CLK_ENABLE();	
}
/**
  * @brief  TIM4_start
  * @param  NONE
  * @retval NONE
  */
void TIM4_start(void)
{
		RCC_Configuration();

		/* configure TIM4 for remote and encoder */
		NVIC_Configuration();
		TIM4_Configuration();
}
/**
  * @brief  initTimer
  * @param  NONE
  * @retval NONE
  */
void initTimer(void)
{
		TIM4_start();
}
/**
  * @brief  TIM4_IRQHandler
  * @param  NONE
  * @retval NONE
  */
void TIM4_IRQHandler(void)
{
	/*
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
			last_time_set = TIM_GetCounter(TIM4);
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
			TimeDispatch();
	}
	*/

	if (__HAL_TIM_GET_FLAG(&TIM4_HandleStructure,TIM_IT_CC1)!= RESET)
	{
		last_time_set = __HAL_TIM_GET_COUNTER(&TIM4_HandleStructure);
		__HAL_TIM_CLEAR_FLAG(&TIM4_HandleStructure,TIM_IT_CC1);
		TimeDispatch();
	}
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
