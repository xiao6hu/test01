/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp_usart.h
  * @作者       ： hjt
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年07月06日
  * @摘要       ： USART头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H

/* 包含的头文件 --------------------------------------------------------------*/

#include "includes.h"
/* 宏定义 --------------------------------------------------------------------*/

typedef struct _UartProtocl
{
	u8 sync;
	u8 addr;
	u8 command;
	u8 data;
	//u8 checksum;			// for BCD
}UartProtocl;

#define UART_SYNC		0xff
#define UART_END		0xfe
#define MIDI_PLAY_CMD	0x03
#define MIDI_STOP_CMD	0x04
#define STUDY_CMD    	0xfd
#define SYNC_CMD    	0xff




#define DEFAULT_SLAVE_ADDR    0x00
#define PublicDeviceAddress   0x00


#define ANALYZE_OK			0x00
#define SYNC_ERROR			0x01
#define START_BYTE_LOSE		0x02
#define LENTH_ERROR			0x03
#define CHECKSUM_ERROR		0x04
#define COMMAND_UNDEFINE	0x05
#define ADDR_ERROR			0x06


extern UartProtocl Packet;
extern u8 SynsFlag;


/* 函数申明 ------------------------------------------------------------------*/
u8 analyzeUartPacket(void);
void TxDeviceCmd(u8 addr,u8 cmd,u8 dat);

#endif /* _BSP_CAN_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
