#ifndef __USART1_H
#define	__USART1_H

//#include "stm32f10x.h"

#include "stm32f2xx_hal.h"
#include "sys.h"
#include <stdio.h>

extern USART_HandleTypeDef husart1;	

extern u8 Uart1_TxBuff[3];


void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

void Uart1SendData(unsigned char ch);

#endif /* __USART1_H */
