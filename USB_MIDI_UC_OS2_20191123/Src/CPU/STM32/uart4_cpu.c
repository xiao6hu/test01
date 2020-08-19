
#include "uart_cpu.h"
#include "uart.h"

u8 RS485_RX_CNT=0;  //接收到的数据长度 	

u8 *IRS_TX_Buf_Pointer;
u8 IRS_TX_Buf_Len;


#ifdef RS485_EN_USART_RX   	//如果使能了接收

/****************************************************************
 * 函数名：void TIM6_NVIC_Configuration(void)  
 * 描述  ：模拟输出I/O口配置  4_20mA输出口配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void TIM6_NVIC_Configuration(void)
{

}

/****************************************************************
 * 函数名：void RS485_TimeOut_Configuration(U32 baudrate) 
 * 描述  ：RS485接收超时配置
 * 输入  ：U32 baudrate->当前的串口波特率
 * 输出  ：无 
 ***************************************************************/ 
/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void RS485_TimeOut_TIM_Configuration(U32 baudrate)
{
	TIMR_Init(RS485_TimeOut_TIM_NUM, TIMR_MODE_TIMER , (((SystemCoreClock / baudrate) * UartTxBitLen)-1) ,1);
	//IRQ_Connect(IRQ16_31_TIMR, IRQ16_IRQ | IRQ_INT1 , 2);
	IRQ_Connect(IRQ0_15_TIMR, IRQ5_IRQ, 2);
    TIMR_Stop(RS485_TimeOut_TIM_NUM);
	
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval : None
  */


#if 0
/****************************************************************
 * 函数名：void TIM6_IRQHandler(void)
 * 描述  ：RS485接收超时中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
void TIM6_IRQHandler(void)
{
	//UART_INTTimeoutEn(RS485_TimeOut_TIM_NUM, ENABLE);
	if ( TIM_GetITStatus(RS485_TimeOut_TIM_NUM , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(RS485_TimeOut_TIM_NUM , TIM_FLAG_Update);  
		   
  		TimeInterval++;

	  	if((TimeInterval>4) && countnumber) 
	  	{

			TIM_Cmd(RS485_TimeOut_TIM_NUM, DISABLE);
			
			/*
			RS485_DataChange();
	     	Deal();                             //将接收到的数据进行处理
			*/
	    	//countnumber=0;

			Tx_Flag = true;

	  	}                      
	} 
}
#endif



#if 1
/*----------------------------------------------------------------------------*/
u8 analyzeUartPacket(void)
{
	u16 tmp;
	#if (CHECK_TYPE ==0)
	u16 Crc16Res;
	#elif (CHECK_TYPE ==1)
	u8 CheckSumRes;
	#endif
	
	
	u8 res=ANALYZE_OK;

	my_memcpy((u8*)&RxProtocl, CommandBuffer, BUFFERLENTH);
		
	if(RxProtocl.sync!=UART_SYNC)
	{
		res=SYNC_ERROR;
	}
	else
	{
		#if (CHECK_TYPE ==0)
		Crc16Res = CRC16(((U8*)&RxProtocl)+1, 7);
		
		
		#if 1
		tmp =Crc16Res>>8;
		tmp |=Crc16Res<<8;
		Crc16Res =tmp;
		#endif
		
		
		
		if (Crc16Res !=RxProtocl.crc16.u2)
			
		
		
		#elif (CHECK_TYPE ==1)
		CheckSumRes = GetCheckSum(((U8*)&RxProtocl)+1, 7);
		if (RxProtocl.CheckSum != CheckSumRes)
		#endif
		{	
			res=CHECKSUM_ERROR;
		}
		else
		{
			switch(RxProtocl.command)
			{
				case IonsAdj_CMD:	
				{
					RS485_Send_Data("ok",3);
				}
				break;
		;
				default:
					res=COMMAND_UNDEFINE;
					break;	
			}
		}
	}
	return res;
}

	
/****************************************************************
 * 函数名：void USART3_IRQHandler(void)
 * 描述  ：RS485串口中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
void IRQ6_Handler(void)	
{
	U32 res;
	u8 uartRecieveRes;
	if (UART_INTRXThresholdStat(RS485_UART_NUM))
	{
		if (UART_ReadByte(RS485_UART_NUM, &res))	//读取接收到的数据
		{	
			if((TimeInterval>2)&&(TimeInterval<=4))
			{
				bufferIndex=0;
			}
			
			TimeInterval=0;
			TIMR_Start(RS485_TimeOut_TIM_NUM);
			
				
			CommandBuffer[bufferIndex++]=res;
			if(bufferIndex>BUFFERLENTH-1)
			{
				bufferIndex=0;
				uartRecieveRes=analyzeUartPacket();	
				
				if (uartRecieveRes == ANALYZE_OK)
				{
				}
			}
		}		
	}
}
#endif



#if 0
/****************************************************************
 * 函数名：void USART3_IRQHandler(void)
 * 描述  ：RS485串口中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
//void USART3_IRQHandler(void)
void IRQ6_Handler(void)	
{
	static U32 res;

	//res = IRQ_Which(IRQ17_IRQ);

	//if (((res & IRQ_INT0) == IRQ_INT0) || ((res & IRQ_BOTH)== IRQ_BOTH))
	{
		
		if (UART_INTRXThresholdStat(RS485_UART_NUM))
		{
			if (UART_ReadByte(RS485_UART_NUM, &res))	//读取接收到的数据
			{	 
				
				if((TimeInterval>2)&&(TimeInterval<=4))
				{
					countnumber=0;
				}
				
				
				if(countnumber>=8) 
				{
					countnumber=0;
				} 

				ReceiveData[countnumber++] =(U8)res;   //将接收到的数据到缓冲区内
				
				TimeInterval=0;
				
				//UART_INTTimeoutEn(RS485_UART_NUM);
				
				//RS485_TimeOut_Timr_Start();
				
				//TIMR_SetPeriod(RS485_TimeOut_TIM_NUM, (((SystemCoreClock / 9600) * UartTxBitLen)-1));
				TIMR_Start(RS485_TimeOut_TIM_NUM);
				
				
				//TIM_SetCounter(RS485_TimeOut_TIM_NUM, 0x0000);
				//TIM_Cmd(RS485_TimeOut_TIM_NUM, ENABLE);					 
			} 
		}	
	}
	
	
	//if (((res & IRQ_INT1) == IRQ_INT1) || ((res & IRQ_BOTH)== IRQ_BOTH))
	{
		if (UART_INTTXThresholdStat(RS485_UART_NUM))
		{
			UART_IRS_Send();
		}
	}
} 

#endif



#if 0
/****************************************************************
 * 函数名：void USART3_IRQHandler(void)
 * 描述  ：RS485串口中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
//void USART3_IRQHandler(void)
void IRQ6_Handler(void)	
{
	static U32 res;

	//res = IRQ_Which(IRQ17_IRQ);

	//if (((res & IRQ_INT0) == IRQ_INT0) || ((res & IRQ_BOTH)== IRQ_BOTH))
	{
		
		if (UART_INTRXThresholdStat(RS485_UART_NUM))
		{
			//if (UART_ReadByte(RS485_UART_NUM, &res))	//读取接收到的数据
			{	 
				
				if((TimeInterval>2)&&(TimeInterval<=4))
				{
					countnumber=0;
				}
				
				
				if(countnumber>=8) 
				{
					countnumber=0;
				} 

				do
				{
					if (UART_ReadByte(RS485_UART_NUM, &res))	//读取接收到的数据
					{
						ReceiveData[countnumber++] = (U8)res;	
					}
					else
					{
						countnumber=0;
						break;
					}
				}while (!UART_IsRXFIFOEmpty(RS485_UART_NUM));					
				
				
				
				//ReceiveData[countnumber++]=res;   //将接收到的数据到缓冲区内
				
				TimeInterval=0;
				
				//UART_INTTimeoutEn(RS485_UART_NUM);
				
				//RS485_TimeOut_Timr_Start();
				
				//TIMR_SetPeriod(RS485_TimeOut_TIM_NUM, (((SystemCoreClock / 9600) * UartTxBitLen)-1));
				TIMR_Start(RS485_TimeOut_TIM_NUM);
				
				
				//TIM_SetCounter(RS485_TimeOut_TIM_NUM, 0x0000);
				//TIM_Cmd(RS485_TimeOut_TIM_NUM, ENABLE);					 
			} 
		}	
	}
	
	
	//if (((res & IRQ_INT1) == IRQ_INT1) || ((res & IRQ_BOTH)== IRQ_BOTH))
	{
		if (UART_INTTXThresholdStat(RS485_UART_NUM))
		{
			UART_IRS_Send();
		}
	}
} 

#endif


#endif
				
/****************************************************************
 * 函数名：void RS485_Init(u32 bound) 
 * 描述  ：RS485串口初始化
 * 输入  ：u32 bound->当前的串口波特率
 * 输出  ：无 
 ***************************************************************/ 																	 
//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	 
void RS485_Init(u32 bound)
{  
	UART_InitStructure initStruct;
	
    PORT_Init(RS485_TX_PORT, RS485_TX_PIN, FUNMUX_UART3_TXD, DIGIT_IN_DIS);
    //引脚初始化，包含引脚方向=输出、上拉电阻=禁止、下拉电阻=禁止、开漏输出=禁止
    //GPIO_Init(RS485_TX_PORT, RS485_TX_GPIO, 1, 0, 0, 0);   
    PORT_Init(RS485_RX_PORT, RS485_RX_PIN, FUNMUX_UART3_RXD, DIGIT_IN_EN);
    //引脚初始化，包含引脚方向=输入、上拉电阻=使能、下拉电阻=禁止、开漏输出=禁止
    //GPIO_Init(RS485_RX_PORT, RS485_RX_GPIO, 0, 1, 0, 0);
    
    GPIO_Init(RS485_DE_GPIO, RS485_DE_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);


#ifdef RS485_EN_USART_RX		 //如果使能了接收
	
	initStruct.Baudrate =bound;
	
	initStruct.RXThreshold =0;			//取值0--7
	initStruct.RXThresholdIEn =1;		//当RX FIFO中数据个数 >= RXThreshold时触发中断
	
	initStruct.TXThreshold=2;			//取值0--7
	initStruct.TXThresholdIEn=0;		//当TX FIFO中数据个数 <= TXThreshold时触发中断
	
	initStruct.TimeoutTime =0;			//超时时长 = TimeoutTime/(Baudrate/10) 秒
	initStruct.TimeoutIEn =0;			//超时中断，超过 TimeoutTime/(Baudrate/10) 秒没有在RX线上接收到数据时触发中断
	
	UART_Init(RS485_UART_NUM, &initStruct);
 
	IRQ_Connect(IRQ0_15_UART3, IRQ6_IRQ, 1);
	
	//UART_INTRXThresholdEn(RS485_UART_NUM);
	

    switch(RamSetParameters.CheckMode)
    {	
        case Odd_Parity_Item:
            //USART_InitStructure.USART_Parity = USART_Parity_Odd;///奇校验位
			//UartTxBitLen=11;
            //break;
            
        case Even_Parity_Item:
            //USART_InitStructure.USART_Parity = USART_Parity_Even ;///偶校验位
			//UartTxBitLen=11;
            //break;

        case No_Parity_Item:  
        default: 
			//USART_InitStructure.USART_Parity = USART_Parity_No;///无校验位
			UartTxBitLen=10;
			break;
		
    }
 

	UART_Open(RS485_UART_NUM);
	
#endif
    RS485_DE(0);	
    //默认为接收模式
	
	
	
}
void UART_IRS_Send(void)
{
	while (!UART_IsTXFIFOFull(RS485_UART_NUM))
	{
		if (IRS_TX_Buf_Len ==0)
		{
			
			if (!UART_IsTXBusy(RS485_UART_NUM))
			{	
				UART_INTTXThresholdDis(RS485_UART_NUM);
				RS485_DE(0);	//默认为接收模式
			}
			break;
		}
		UART_WriteByte(RS485_UART_NUM,*(IRS_TX_Buf_Pointer++));	
		IRS_TX_Buf_Len--;	
	}
}

/****************************************************************
 * 函数名：void RS485_Send_Data(u8 *buf,u8 len)
 * 描述  ：RS485串口发送指定长度的数据
 * 输入  ：u8 *buf->待发送数据的指针 u8 len->待发送数据的字节数
 * 输出  ：无 
 ***************************************************************/ 						
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_IRS_Send_Data(u8 *buf,u8 len)
{
	IRS_TX_Buf_Pointer = buf;
	IRS_TX_Buf_Len = len;
	RS485_DE(1);			//设置为发送模式
	
	
	UART_IRS_Send();
	UART_INTTXThresholdEn(RS485_UART_NUM);
	RS485_RX_CNT=0;	  
}


/****************************************************************
 * 函数名：void RS485_Send_Data(u8 *buf,u8 len)
 * 描述  ：RS485串口发送指定长度的数据
 * 输入  ：u8 *buf->待发送数据的指针 u8 len->待发送数据的字节数
 * 输出  ：无 
 ***************************************************************/ 						
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void Uart1_Send_Data(u8 *buf,u8 len)
{
	static u8 t;
	
	RS485_DE(1);			//设置为发送模式
	
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(UART_IsTXBusy(UART1));	
		UART_WriteByte(RS485_UART_NUM,buf[t]);	
	}
	while(UART_IsTXBusy(RS485_UART_NUM));
	RS485_RX_CNT=0;	  
	RS485_DE(0);				//设置为接收模式	
}


/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
    RS485_DE(1);
	/* 将Printf内容发往串口 */
	while(UART_IsTXBusy(RS485_UART_NUM));	
	UART_WriteByte(RS485_UART_NUM,ch);
	while(UART_IsTXBusy(RS485_UART_NUM));
	RS485_DE(0);
	return (ch);
}













