#ifndef __USART1_H
#define	__USART1_H

#include "stm32f2xx.h"
//#include "sys.h"
#include <stdio.h> 
#include "CANOpenObjDictConfig.h"	

#define u8 unsigned char	



typedef struct _UartProtocl
{
	//u8 sync;
	u8 addr;
	u8 command;
	u8 data;
	//u8 checksum;			// for BCD
}UartProtocl;
      

#define USART1_RE_PIN	GPIO_PIN_12
#define USART1_RE_GPIO	GPIOB
#define USART1_RE_RCC	RCC_APB2Periph_GPIOB


#define USART1_RE_TX()	//HAL_GPIO_WritePin(USART3_RE_GPIO, USART3_RE_PIN,GPIO_PIN_SET);
#define USART1_RE_RX()	//HAL_GPIO_WritePin(USART3_RE_GPIO, USART3_RE_PIN,GPIO_PIN_RESET);


#define UART_SYNC				0xff
#define UART_START				0xaa
#define UART_END				0xfe

#define MIDI_PLAY				0x03
#define MIDI_STOP				0x04


#define PARA_SET_CMD    0xfb 
#define PARA_UPDATE_CMD  0xfa

#define PARA_SET_START    0x01
#define PARA_SET_END      0x02
#define PARA_SET_OK       0x03
#define PARA_SET_ERROR    0x04

#define STUDY_CMD    0xfd
#define DEFAULT_SLAVE_ADDR    0x7F


#define ANALYZE_OK				0x00
#define SYNC_ERROR				0x01
#define START_BYTE_LOSE		0x02
#define LENTH_ERROR			0x03
#define CHECKSUM_ERROR		0x04
#define COMMAND_UNDEFINE	0x05
#define CAN_HANDLE_UNDEFINE	0x06

#define BUFFERLENTH		6							 ///<Ñ­»·Ä£Ê½
extern  u8 CommandBuffer[BUFFERLENTH];
extern  u8 bufferIndex;

extern UartProtocl Packet;
extern u8 uartRecieveRes;


extern u8 IR_Rx_Cnt;
extern UART_HandleTypeDef huart1;	

extern u8 Uart1_TxBuff[64];
extern u8 Uart1_RxBuff[64];

u8 analyzeUartPacket(void);

void USART1_NVIC_Configuration(void);
void USART1_Config(void);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Uart1SendData(unsigned char ch);
void TxDeviceCmd(u8 addr,u8 cmd,u8 dat);

void SimulateGameFailure(void);
#endif /* __USART1_H */
