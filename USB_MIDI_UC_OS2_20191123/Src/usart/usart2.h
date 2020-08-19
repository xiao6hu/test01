#ifndef __USART2_H
#define	__USART2_H

#include "stm32f2xx.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

#define UART2_BUFFERLENTH  32

#define UART2_BUFFERLENTH  32


void TIM7_Configuration(void);
void Uart2SendData(unsigned char ch);
void MX_USART2_UART_Init(void);
u8 analyzeUart2Packet(Message *m);

#endif /* __USART1_H */
