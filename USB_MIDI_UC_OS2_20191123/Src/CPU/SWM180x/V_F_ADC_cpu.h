/*--------------------------------------------------------------------------*/
/**@file     V_F_ADC_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __V_F_ADC_CPU_H__
#define __V_F_ADC_CPU_H__
/* Includes ------------------------------------------------------------------*/
#include "includes.h" 

/*
#define CP_PORT	  	GPIOA       //AD625 --> 2MHZ时钟输出口
#define CP_GPIO		GPIO_Pin_8

#define CP_TIM_NUM		TIM1

#define CF_PORT		GPIOD       //AD625 --> 脉冲入出口
#define CF_GPIO		GPIO_Pin_2

#define CF_TIM_NUM		TIM3 
*/

#define Field_TIM_NUM		TIMR1

#define Field_TIM_IRQn		TIM7_IRQn



#define FieldPositive_PORT		PORTD       //励磁信号正脉冲输出	   ex2
#define FieldPositive_GPIO		GPIOD
#define FieldPositive_PIN		PIN4
	 
#define FieldNegative_PORT		PORTD       //励磁信号负脉冲输出	   ex1
#define FieldNegative_GPIO		GPIOD
#define FieldNegative_PIN		PIN3




#define Mtsnsr_PWM_GROUP    PWM2

#define MtsnsrPositive_PWM_CH       PWM_CH_A
#define MtsnsrPositive_FUNMUX       FUNMUX_PWM2A_OUT

#define MtsnsrNegative_PWM_CH       PWM_CH_B
#define MtsnsrNegative_FUNMUX       FUNMUX_PWM2B_OUT

#define Mtsnsr_Out_Start() PWM_Start(Mtsnsr_PWM_GROUP, ENABLE, ENABLE)
#define Mtsnsr_Out_Stop()  PWM_Stop(Mtsnsr_PWM_GROUP, ENABLE, ENABLE)
	


#define MtsnsrPositive_PORT		PORTD       //空管检测信号正脉冲输出
#define MtsnsrPositive_GPIO		GPIOD
#define MtsnsrPositive_PIN		PIN0

#define MtsnsrNegative_PORT		PORTD       //空管检测信号负脉冲输出
#define MtsnsrNegative_GPIO		GPIOD
#define MtsnsrNegative_PIN		PIN1




#define AL_PORT		PORTA               //空管检测信号正脉冲输出
#define AL_GPIO		GPIOA
#define AL_PIN		PIN13

#define AH_PORT		PORTA               //空管检测信号负脉冲输出
#define AH_GPIO		GPIOA
#define AH_PIN		PIN14

#define IN_PORT		PORTA               //输入控制端
#define IN_GPIO		GPIOA
#define IN_PIN		PIN15


#define FieldPositiveOut(Status)  	GPIO_WriteBit(FieldPositive_GPIO , FieldPositive_PIN, (BitAction)Status)
#define FieldNegativeOut(Status)	GPIO_WriteBit(FieldNegative_GPIO , FieldNegative_PIN, (BitAction)Status)

#define MtsnsrPositiveOut(Status)	GPIO_WriteBit(MtsnsrPositive_GPIO , MtsnsrPositive_PIN, (BitAction)Status)
#define MtsnsrNegativeOut(Status)	GPIO_WriteBit(MtsnsrNegative_GPIO , MtsnsrNegative_PIN, (BitAction)Status)

#define ON 	1
#define OFF 0
#define AL_Out(Status)  	GPIO_WriteBit(AL_GPIO, AL_PIN, (BitAction)Status)
#define AH_Out(Status)		GPIO_WriteBit(AH_GPIO, AH_PIN, (BitAction)Status)


void ALM_GPIO_Config(void);
void Field_Out_GPIO_Config(void);
void Field_Out_TIM_Configuration(void);
void Field_Out_Configuration(void);
void Mtsnsr_Out_Configuration(void);



#endif






