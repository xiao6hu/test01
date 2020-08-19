/*--------------------------------------------------------------------------*/
/**@file     uart_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __UART_CPU_H__
#define __UART_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

#define RS485_TX_GPIO   GPIOC
#define RS485_TX_PORT   PORTC
#define RS485_TX_PIN    PIN7

#define RS485_RX_GPIO   GPIOD
#define RS485_RX_PORT   PORTD
#define RS485_RX_PIN    PIN6


#define RS485_DE_GPIO   GPIOC
#define RS485_DE_PORT   PORTC
#define RS485_DE_PIN    PIN6

//模式控制 485模式控制.0,接收;1,发送.
#define RS485_DE(Ststus)    GPIO_WriteBit(RS485_DE_GPIO , RS485_DE_PIN, (BitAction)Ststus)

#define RS485_EN_USART_RX  

#define RS485_UART_NUM  UART3



#define RS485_UART_IRQ  USART3_IRQn

#define RS485_TimeOut_TIM_NUM   TIMR2

#define RS485_TimeOut_TIM_IRQn  TIM6_IRQn



void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_IRS_Send_Data(u8 *buf,u8 len);
void RS485_TimeOut_TIM_Configuration(U32 baudrate);
void UART_IRS_Send(void);


#endif


