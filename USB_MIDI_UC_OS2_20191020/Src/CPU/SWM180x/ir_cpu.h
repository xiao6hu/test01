/*--------------------------------------------------------------------------*/
/**@file     ir_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __IR_CPU_H__
#define __IR_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

#define IR_GPIO	GPIOA
#define IR_PORT	PORTA
#define IR_PIN	PIN4
#define IR_TIMR TIMR0

#define IR_TIMR_Start() TIMR_Start(IR_TIMR)
#define IR_TIMR_Stop() TIMR_Stop(IR_TIMR)



//#define Get_PluseWidth_Val() TIMRG-> PCVAL
#define Get_PluseWidth_Val() 0xFFFFFFFF-TIMR_GetCurValue(IR_TIMR)

#define PluseWidth_1ms_Val  (SYSTEM_CORE_CLOCK / 1000)

void IR_Configuration(void);

#endif  /*__IR_CPU_H__*/
