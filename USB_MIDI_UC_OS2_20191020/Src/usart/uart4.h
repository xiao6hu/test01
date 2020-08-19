#ifndef __UART4_H
#define	__UART4_H

//#include "stm32f10x.h"

#include "stm32f2xx.h"
#include "sys.h"
#include <stdio.h>

extern UART_HandleTypeDef huart4;	

extern u8 Uart4_TxBuff[3];

void UART4_Config(void);
//void UART4_printf(UART_TypeDef* UARTx, uint8_t *Data,...);
void Uart4SendData(unsigned char ch);

#endif /* __USART1_H */

