/**
  **********************************  STM8L  ***********************************
  * @文件名     ： bsp_uart.c
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年04月10日
  * @摘要       ： UART源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-04-10 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp_can.h"
#include "bsp_timer2.h"
#include "bsp.h"
#include "LIGHTS.h" 
#include "Parameters.h"
#include "Study.h"	
	
#include "CANOpenObjDictConfig.h"	
	
UartProtocl Packet;
u8 uartRecieveRes=ANALYZE_OK;
u8 SynsFlag=0;


/************************************************
函数名称 ： UART1_Initializes
功    能 ： UART1初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： hjt
*************************************************/





/*----------------------------------------------------------------------------*/
u8 analyzeUartPacket(void)
{
	//u8 temp;
	u8 res=ANALYZE_OK;
	
	/*
	Packet.sync=ReceiveData[0];
	Packet.addr=ReceiveData[1];
	Packet.command=ReceiveData[2];
	Packet.data=ReceiveData[3];
	Packet.checksum=ReceiveData[4];
	*/
	
	
	//Packet.sync=m->data[0];
	Packet.addr=m->data[0];
	Packet.command=m->data[1];
	Packet.data=m->data[2];

	/*
	temp=Packet.addr+Packet.command+Packet.data;
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);
	*/
	
	RGB_Lights_Cnt=0;
	/*
	if(Packet.sync!=UART_SYNC)
		res=SYNC_ERROR;
	else if(temp!=Packet.checksum)
		res=CHECKSUM_ERROR;
	else 
	*/
	if((RamSetParameters.DeviceAddress!=Packet.addr) && (PublicDeviceAddress!=Packet.addr))
		res=ADDR_ERROR;
	else
	{
		switch(Packet.command)
		{
			case LIGHTS_ON_CMD:	
				if (Touch_Key_Up_Down_Flag==1)
				{
					W_LED_ON;
					rgb_led_on_deal();
				}
				break;

			case LIGHTS_OFF_CMD:
				if (Touch_Key_Up_Down_Flag==1)
				{
					W_LED_OFF; 
					rgb_led_off_deal();
				}
				break; 
					
			case STUDY_CMD:	

			  	Study();
				break;
				
			case SYNC_CMD:
				if (Touch_Key_Up_Down_Flag==1)
				{
				  	//Lights_off_deal();
					
					SynsFlag=1;
					
				}
				break;
				
			default:
				res=COMMAND_UNDEFINE;
				break;	
		}
	}
	return res;
}

void TxDeviceCmd(u8 addr,u8 cmd,u8 dat)
{
	Message TxMessage;
	TxMessage.cob_id=addr;	/**< message's ID */
	TxMessage.rtr=CAN_RTR_Data;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
	TxMessage.len=3;		/**< message's length (0 to 8) */
    //TxMessage.data[0]=UART_SYNC;
    TxMessage.data[0]=addr;
    TxMessage.data[1]=cmd;
    TxMessage.data[2]=dat;
	canSend(CAN,&TxMessage);	
}

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
