#include "includes.h"

RTC_HandleTypeDef RtcHandle;
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;


/*
 * 函数名：RTC_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void RTC_Init(void)
{
	
	__HAL_RCC_RTC_ENABLE();
	 /* Configure RTC prescaler and RTC data registers */
 	 /* RTC configured as follow:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
	
	
	RtcHandle.Instance = RTC;
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
	{
	/* Initialization Error */
	Error_Handler(); 
	}
}

char* WeekToChinese(uint8_t Week)
{
	switch (Week)
	{
		case 1: return "一";
		case 2: return "二"; 
		case 3: return "三"; 
		case 4: return "四"; 
		case 5: return "五"; 
		case 6: return "六"; 
		case 7: return "日"; 
		default: return "  "; 
	}
}


uint8_t CaculateWeekDay(int y,uint8_t m, uint8_t d)
{
	uint8_t WeekDay;
	if(m==1||m==2) 
	{
		m+=12;
		y--;
	}
	WeekDay=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
	return WeekDay+1;
} 

