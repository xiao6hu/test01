
#ifndef __BL8025T_H__
#define __BL8025T_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h" 

#define YEAR_HIGH_BYTE 0x20

#define BL8025T_SlaveAddress 0x64

#define WriteMode 0x00
#define ReadMode  0x01

//¶¨Òå¼Ä´æÆ÷µØÖ·
#define	BL8025T_REG_ADDR_SEC    			0x00
#define	BL8025T_REG_ADDR_MIN    			0x01
#define	BL8025T_REG_ADDR_HOUR   			0x02
#define	BL8025T_REG_ADDR_WEEK   			0x03
#define	BL8025T_REG_ADDR_DAY    			0x04
#define	BL8025T_REG_ADDR_MONTH  			0x05
#define	BL8025T_REG_ADDR_YEAR   			0x06
#define	BL8025T_REG_ADDR_RAM    			0x07
#define	BL8025T_REG_ADDR_MIN_Alarm  		0x08
#define	BL8025T_REG_ADDR_HOUR_Alarm 		0x09

#define	BL8025T_REG_ADDR_WEEK_Alarm 		0x0A
#define	BL8025T_REG_ADDR_DAY_Alarm  		0x0A

#define	BL8025T_REG_ADDR_Timer_Counter0 	0x0B
#define	BL8025T_REG_ADDR_Timer_Counter1 	0x0C
#define	BL8025T_REG_ADDR_Extension_Register 0x0D
#define	BL8025T_REG_ADDR_Flag_Register  	0x0E
#define	BL8025T_REG_ADDR_Control_Register   0x0F	

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

