
#ifndef __RTC_H__
#define __RTC_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h" 

#define YEAR_HIGH_BYTE 0x20

typedef struct
{
	U8 SEC;
	U8 MIN;
	U8 HOUR; 
	U8 WEEK;
	U8 DAY;
	U8 MONTH;
	U8 YEAR;
}TimeStructTypeDef;

extern TimeStructTypeDef TimeStruct;


void SetTime(void);
void GetTime(void);

#endif