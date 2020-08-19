/*--------------------------------------------------------------------------*/
/**@file     DigitalOutput_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __DigitalOutput_CPU_H__
#define __DigitalOutput_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h" 

#define P_F_PORT	PORTA
#define P_F_GPIO	GPIOA
#define P_F_PIN 	PIN12

#define P_F_PWM_GROUP    PWM1
#define P_F_PWM_CH       PWM_CH_A
#define P_F_FUNMUX       FUNMUX_PWM1A_OUT

#if (P_F_PWM_CH == PWM_CH_A)
    #define FrequePulseOutput_Start() PWM_Start(P_F_PWM_GROUP, ENABLE, DISABLE)
	#define FrequePulseOutput_Stop()  PWM_Stop(P_F_PWM_GROUP, ENABLE, DISABLE)
	
#elif (P_F_PWM_CH == PWM_CH_B)
    #define FrequePulseOutputStart()  PWM_Start(P_F_PWM_GROUP, DISABLE, ENABLE)
	#define FrequePulseOutput_Stop()  PWM_Stop(P_F_PWM_GROUP, DISABLE, ENABLE)
#endif

#define P_F_SetPluseDuty(PluseVal)  PWM_SetHDuty(P_F_PWM_GROUP, P_F_PWM_CH, PluseVal)
#define P_F_SetCycle(CycleVal)      PWM_SetCycle(P_F_PWM_GROUP, P_F_PWM_CH, CycleVal)

#define LodeFrequeOutputVal(FrequeVal) P_F_SetCycle(FrequeVal); P_F_SetPluseDuty(FrequeVal/2) 

/*----------------------------------------------------------------------------*/
void DigitalOutput_GPIO_Config(void); 
void FrequeOutput_Configuration(void);
void PulseOutput_Configuration(void);

#endif







