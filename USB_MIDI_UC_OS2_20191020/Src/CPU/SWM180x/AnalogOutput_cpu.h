/*--------------------------------------------------------------------------*/
/**@file     AnalogOutput_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __AnalogOutput_CPU_H__
#define __AnalogOutput_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/*----------------------------------------------------------------------------*/


#define OUT_4_20mA_PORT	  	    PORTD
#define OUT_4_20mA_GPIO		    GPIOD
#define OUT_4_20mA_PIN	  	    PIN2


#define OUT_4_20mA_PWM_GROUP    PWM0
#define OUT_4_20mA_PWM_CH       PWM_CH_A
#define OUT_4_20mA_FUNMUX       FUNMUX_PWM0A_OUT


#if (OUT_4_20mA_PWM_CH == PWM_CH_A)
    #define OUT_4_20mA_Start() PWM_Start(OUT_4_20mA_PWM_GROUP, ENABLE, DISABLE)
	#define OUT_4_20mA_Stop()  PWM_Stop(OUT_4_20mA_PWM_GROUP, ENABLE, DISABLE)
	
#elif (OUT_4_20mA_PWM_CH == PWM_CH_B)
    #define OUT_4_20mA_State() PWM_Start(OUT_4_20mA_PWM_GROUP, DISABLE, ENABLE)
	#define OUT_4_20mA_Stop()  PWM_Stop(OUT_4_20mA_PWM_GROUP, DISABLE, ENABLE)
#endif

#define OUT_4_20mA_SetPluseDuty(PluseVal) PWM_SetHDuty(OUT_4_20mA_PWM_GROUP, OUT_4_20mA_PWM_CH, PluseVal)

/*----------------------------------------------------------------------------*/
void AnalogOutput_GPIO_Config(void);
void AnalogOutput_Configuration(void);

#endif




