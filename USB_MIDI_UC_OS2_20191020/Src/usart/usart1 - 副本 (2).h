#ifndef __USART3_H
#define	__USART3_H

//#include "stm32f10x.h"
#include "stm32f2xx_hal.h"
#include "sys.h"


#include <stdio.h>

#define USART3_IRQ_EN

typedef struct _UartProtocl
{
	u8 sync;
	//u8 start;
	u8 addr;
	//u8 lenth;
	
	u8 data;
	
	u8 command;
	
	//u8 * dat;
	
	u8 checksum;			// for BCD
}UartProtocl;
      
#define USART3_RE_PIN	GPIO_PIN_12
#define USART3_RE_GPIO	GPIOB
#define USART3_RE_RCC	RCC_APB2Periph_GPIOB

#define USART3_RE_TX()	HAL_GPIO_WritePin(USART3_RE_GPIO, USART3_RE_PIN,GPIO_PIN_SET);
#define USART3_RE_RX()	HAL_GPIO_WritePin(USART3_RE_GPIO, USART3_RE_PIN,GPIO_PIN_RESET);


#define UART_SYNC				0xff
#define UART_START				0xaa
#define UART_END				0xfe



#define MIDI_PLAY				0x01
#define MIDI_STOP				0x02


#define STUDY_CMD    0xfd

#define DEFAULT_SLAVE_ADDR    0x00

#define ANALYZE_OK				0x00
#define SYNC_ERROR				0x01
#define START_BYTE_LOSE		0x02
#define LENTH_ERROR			0x03
#define CHECKSUM_ERROR		0x04
#define COMMAND_UNDEFINE		0x05


#define BUFFERLENTH		6							 ///<Ñ­»·Ä£Ê½
extern  u8 CommandBuffer[BUFFERLENTH];
extern  u8 bufferIndex;

extern UartProtocl Packet;
extern u8 uartRecieveRes;

extern u8 IR_Rx_Cnt;


extern USART_HandleTypeDef husart3;	

extern u8 Uart3_TxBuff[6];
extern u8 Uart3_RxBuff[6];



u8 analyzeUartPacket(void);

void USART3_NVIC_Configuration(void);

void USART3_Config(void);
//int fputc(int ch, FILE *f);
void USART3_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

void Uart3SendData(unsigned char ch);



void TxDeviceCmd(u8 addr,u8 cmd,u8 dat);



#endif /* __USART1_H */
