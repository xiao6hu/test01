/******************** (C) COPYRIGHT 2012 **************************
 * 文件名  ：usart4.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：The One STM32开发板
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.5.0
 *
 * 淘宝    ：http://sztheone.taobao.com/
**********************************************************************************/
#include "includes.h"
#include "XFS5152CE.h"
#include <stdarg.h>


//u8 text[]={"科大讯飞"};


u8 XFS5152CE_Flag=0;

u8 VoiceBuff[4096];





void XFS5152CE_Config(void)
{
	#if 0
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/* USART1 GPIO config */
    /* Configure USART1 Tx (PA.9) as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_9; 
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	//GPIO_InitStructure.Alternate= GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif
	
	USART1_Config();
}
/*
void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data)
{
	INT8U  err;
	u8 dat;
	u16 len;
	u8 ErrorCnt;
	
	ErrorCnt=3;
	do
	{
		Uart1SendData(XFS5152CE_FRAME_HEADER);
		Uart1SendData((u8)(length>>8));
		Uart1SendData((u8)(length&0x00ff));
		Uart1SendData(command);
		if (--length <= 0) return; 	
		Uart1SendData(parameter);
		if (--length <= 0) return; 
		HAL_USART_Transmit_IT(&husart1,data,length);
		
		
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) UART1_TX_OK_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
					(INT32U) 0,
					(INT8U *)&err);
		
		HAL_USART_Receive(&husart1,&dat,1,10);
	}
	while ((dat != XFS5152CE_RX_OK) && (--ErrorCnt>0));
	
	ErrorCnt=3;
	do
	{
		HAL_USART_Receive(&husart1,&dat,1,10);
	}
	while ((dat != XFS5152CE_IDLE) && (--ErrorCnt>0));	
}
*/


#if 0

void XFS5152CE_SendData_Wita(u16 length, u8 command, u8 parameter, u8* data)
{
	INT8U  err;
	//u8 dat;
	u16 len;
	u8 ErrorCnt;
	u8 msg;
	
	ErrorCnt=3;
	do
	{
		len=length;
		Uart1SendData(XFS5152CE_FRAME_HEADER);
		Uart1SendData((u8)(len>>8));
		Uart1SendData((u8)(len&0x00ff));
		Uart1SendData(command);
		if (--len <= 0) return; 	
		Uart1SendData(parameter);
		if (--len <= 0) return; 
		HAL_UART_Transmit_IT(&huart1,data,len);
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) UART1_TX_OK_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
					(INT32U) 0,
					(INT8U *)&err);
		
		msg = *((u8 *)OSQPend(XFS5152CE_RX_Q,1*1000,&err));
	}
	while ((msg != XFS5152CE_RX_OK) && (--ErrorCnt>0));
	ErrorCnt=3;
	do
	{
		msg = *((u8 *)OSQPend(XFS5152CE_RX_Q,10*1000,&err));
	}
	while ((msg != XFS5152CE_IDLE)  && (--ErrorCnt>0));	
	
}
	
#endif


#if 1

void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data, u8 IsWita)
{
	//INT8U  err;
	u16 len;
	u8 ErrorCnt;
	//u8 msg;
	u8 ch;
	ErrorCnt=3;
	do
	{
		len=length;
		Uart1SendData(XFS5152CE_FRAME_HEADER);
		Uart1SendData((u8)(len>>8));
		Uart1SendData((u8)(len&0x00ff));
		Uart1SendData(command);
		if (--len > 0) 
		{
			Uart1SendData(parameter);
			if (--len > 0) 
			{
				HAL_UART_Transmit(&huart1,data,len,10);
				while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)); 
				__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_TC);
				
			}
		}

		while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
		{
			OSTimeDlyHMSM(0, 0,0,100);
		}

		//ch =(uint8_t)(huart1.Instance->DR & (uint8_t)0x00FF);
		
		HAL_UART_Receive(&huart1,&ch,1,10);
		//__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
	}
	while ((ch != XFS5152CE_RX_OK) && (--ErrorCnt>0));
	
	
	if ((ErrorCnt ==0) || ((IsWita & XFS5152CE_WAIT_PLAY) != XFS5152CE_WAIT_PLAY))
	{
		return;
	}
	ErrorCnt=3;
	do
	{
		while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
		{
			OSTimeDlyHMSM(0, 0,0,100);
		}
		HAL_UART_Receive(&huart1,&ch,1,10);
		//ch =(uint8_t)(huart1.Instance->DR & (uint8_t)0x00FF);
		//__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
	}
	while ((ch != XFS5152CE_IDLE) && (--ErrorCnt>0));
}
	
#endif



#if 0

void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data)
{
	INT8U  err;
	
	Uart1SendData(XFS5152CE_FRAME_HEADER);
	Uart1SendData((u8)(length>>8));
	Uart1SendData((u8)(length&0x00ff));
	Uart1SendData(command);
	if (--length <= 0) return; 	
	Uart1SendData(parameter);
	if (--length <= 0) return; 
	HAL_UART_Transmit_IT(&huart1,data,length);
	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) UART1_TX_OK_FLAG,
				(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
				(INT32U) 0,
				(INT8U *)&err);
		
	XFS5152CE_Flag=1;
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PlayEnd_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);
		
}
#endif

#if 0

void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data)
{
	INT8U  err;
	

	do
	{
		Uart1SendData(XFS5152CE_FRAME_HEADER);
		Uart1SendData((u8)(length>>8));
		Uart1SendData((u8)(length&0x00ff));
		Uart1SendData(command);
		if (--length > 0) 
		{
			Uart1SendData(parameter);
			if (--length > 0) 
			{
				HAL_UART_Transmit(&huart1,data,length);
			}
		}

		while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
		{
			OSTimeDlyHMSM(0, 0,0,100);
		}
		HAL_USART_Receive(&huart1,&ch,1,10);
		__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
	}
	while (ch== )


	while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
	{
		OSTimeDlyHMSM(0, 0,0,100);
	}
 	HAL_USART_Receive(&huart1,&ch,1,10);
	__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);







}






	/*
	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) UART1_TX_OK_FLAG,
				(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
				(INT32U) 0,
				(INT8U *)&err);
		
	XFS5152CE_Flag=1;
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PlayEnd_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);
	*/	
}
#endif


void XFS5152CE_ReturnStatus(u8 dat)
{
	static u8 XFS5152CE_FailCount=0;
	
	INT8U  err;
	
	if (XFS5152CE_Flag==0)
	{
		return;
	}
	else if (XFS5152CE_Flag==1)
	{
		if (dat != XFS5152CE_RX_OK)
		{
			if (++XFS5152CE_FailCount<3)
			{
				OSSemPost(XFS5152CE_SemSignal);
				return;
			}
		}
		else
		{
			XFS5152CE_Flag=2;
			return;
		}
	}
	else if (XFS5152CE_Flag==2)
	{ 
		if (dat != XFS5152CE_IDLE)
		{
			if (++XFS5152CE_FailCount<3)
			{
				return;
			}
		}	
	}
	XFS5152CE_FailCount=0;
	XFS5152CE_Flag=0;
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PlayEnd_FLAG,
				(INT8U) OS_FLAG_SET,
				(INT8U  *)&err);
}


#if 0
/*
PlayStatus=0 不等待上一条语音播放完成立即播放
PlayStatus=1 等待上一条语音播放完成后再播放
*/
void XFS5152CE_Play(u8 PlayStatus)
{
	INT8U  err;
	OS_FLAGS  Flags;

	
	

	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
		(OS_FLAGS) XFS5152CE_PLAY_FLAG,
		(INT8U) OS_FLAG_CLR,
		(INT8U  *)&err);	

	
	if ((PlayStatus & XFS5152CE_WAIT_PLAY)!=XFS5152CE_WAIT_PLAY)
	{
		XFS5152CE_Flag=0;
		
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PlayEnd_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);	
		OSSemPost(XFS5152CE_SemSignal);	
	}
	else
	{
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) XFS5152CE_PlayEnd_FLAG ,
						(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
						(INT32U) 0,
						(INT8U *)&err);
		
		OSSemPost(XFS5152CE_SemSignal);
		
		
		
		
	}
	
	
	
	
	if ((PlayStatus & XFS5152CE_WAIT_STOP)==XFS5152CE_WAIT_STOP)
	{
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) XFS5152CE_PlayEnd_FLAG ,
					(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
					(INT32U) 0,
					(INT8U *)&err);
	}
}
#endif



#if 1
/*
PlayStatus=0 不等待上一条语音播放完成立即播放
PlayStatus=1 等待上一条语音播放完成后再播放
*/
void XFS5152CE_Play(u8 PlayStatus)
{
	
	XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff, PlayStatus);	
	#if 0
	
	INT8U  err;
	
	

	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PLAY_FLAG,
				(INT8U) OS_FLAG_SET,
				(INT8U  *)&err);

	if ((PlayStatus & XFS5152CE_WAIT_PLAY)!=XFS5152CE_WAIT_PLAY)
	{
		/*
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_WAIT_PLAY_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);	
		*/
		XFS5152CE_Flag=0;
		
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_PlayEnd_FLAG,
				(INT8U) OS_FLAG_SET,
				(INT8U  *)&err);	
		
	}
	
	else
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_WAIT_PLAY_FLAG,
				(INT8U) OS_FLAG_SET,
				(INT8U  *)&err);	
	}
	
	if ((PlayStatus & XFS5152CE_WAIT_STOP)!=XFS5152CE_WAIT_STOP)
	{
		/*
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_WAIT_STOP_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);	
		*/
	}
	else
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) XFS5152CE_WAIT_STOP_FLAG,
				(INT8U) OS_FLAG_SET,
				(INT8U  *)&err);	
	}
			
	OSSemPost(XFS5152CE_SemSignal);	
	#endif
}
#endif



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

