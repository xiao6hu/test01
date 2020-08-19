#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART2_IRQ_EN

typedef struct _UartProtocl
{
	u8 sync;
	u8 start;
	
	u8 lenth;
	
	u8 command;
	
	u8 * dat;
	
	u8 checksum;			// for BCD
}UartProtocl;



#define UART_SYNC				0xff
#define UART_START				0xaa
#define UART_END				0xfe



#define MIDI_PLAY				0x01
#define MIDI_STOP				0x02


#define ANALYZE_OK				0x00
#define SYNC_ERROR				0x01
#define START_BYTE_LOSE		0x02
#define LENTH_ERROR			0x03
#define CHECKSUM_ERROR		0x04
#define COMMAND_UNDEFINE		0x05


#define BUFFERLENTH		16							 ///<Ñ­»·Ä£Ê½
extern  u8 CommandBuffer[BUFFERLENTH];
extern  u8 bufferIndex;

extern UartProtocl Packet;
extern u8 uartRecieveRes;


u8 analyzeUartPacket(void);

void USART2_NVIC_Configuration(void);

void USART2_Config(void);
//int fputc(int ch, FILE *f);
void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

void Uart2SendData(unsigned char ch);

#endif /* __USART1_H */
