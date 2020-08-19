#ifndef __RTC_H
#define	__RTC_H

#include "stm32f2xx.h"




typedef struct
{
  uint8_t Hours;            
  uint8_t Minutes;        
  uint8_t Seconds;          
}TimeTypeDef; 


extern RTC_HandleTypeDef RtcHandle;
extern RTC_TimeTypeDef RTC_Time;
extern RTC_DateTypeDef RTC_Date;


/* Defines related to Clock configuration */    
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
  

void RTC_Init(void);
char* WeekToChinese(uint8_t Week);
uint8_t CaculateWeekDay(int y,uint8_t m, uint8_t d);
#endif // __RTC_H
