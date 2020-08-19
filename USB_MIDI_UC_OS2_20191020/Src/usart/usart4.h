#ifndef __USART4_H
#define	__USART4_H

//#include "stm32f10x.h"

#include "stm32f2xx.h"
#include "sys.h"
#include <stdio.h>

extern USART_HandleTypeDef husart4;	

extern u8 Uart4_TxBuff[3];

void USART4_Config(void);
void USART4_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Uart4SendData(unsigned char ch);

#endif /* __USART1_H */

