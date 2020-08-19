/******************** (C) COPYRIGHT 2012 **************************
 * 文件名  ：usart1.c
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

#include "usart3.h"
#include <stdarg.h>
#include "mid.h"

#include "lights.h"

#include "Keyboard.h"



USART_HandleTypeDef husart3;					

TIM_HandleTypeDef htim6;

u8 Uart3_TxBuff[6];
u8 Uart3_RxBuff[6];


u8 CommandBuffer[BUFFERLENTH];

U8 TimeInterval;

U8 UartTxBitLen=10;
u32 baudrate=9600;

u8 bufferIndex;

UartProtocl Packet;

u8 uartRecieveRes=ANALYZE_OK;

u8 IR_Rx_Cnt =0;


#if 1
void TIM6_Configuration(void)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

    htim6.Instance=TIM6;
    htim6.Init.Period= (((SystemCoreClock / baudrate/72) * UartTxBitLen)-1);	/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    htim6.Init.Prescaler= (72);				    /* 时钟预分频数 72M/72 */
    htim6.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; 		/* 采样分频 */

    htim6.Init.CounterMode=TIM_COUNTERMODE_UP; /* 向上计数模式 */

    HAL_TIM_Base_Init(&htim6);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	
	
	//HAL_NVIC_SetPriority(TIM6_IRQn, 0, 2);
    //HAL_NVIC_EnableIRQ(TIM6_IRQn);

	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);							    		/* 清除溢出中断标志 */
    __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);

    __HAL_TIM_DISABLE(&htim6);

}


/****************************************************************
 * 函数名：void TIM6_IRQHandler(void)
 * 描述  ：RS485接收超时中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
void TIM6_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE))
	{	
        __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);

		TimeInterval++;

		if((TimeInterval>4) && bufferIndex) 
		{			
		    __HAL_TIM_DISABLE(&htim6);
			bufferIndex=0;
			//TimeInterval=0;
			
		}    
	}		
}
#endif






/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
#ifdef USART3_IRQ_EN
void USART3_IRQHandler(void)
{  	
	u8 dat;
    //u8 uartRecieveRes;
	INT8U err;
	
    HAL_USART_IRQHandler(&husart3);

	
	#if 1
	if(__HAL_USART_GET_FLAG(&husart3, USART_FLAG_RXNE))
  	{ 
  		__HAL_UART_CLEAR_FLAG(&husart3, USART_FLAG_RXNE);
        //if(HAL_OK ==HAL_USART_Receive(&husart3, &dat, 1, 0))	
		
		dat =(uint8_t)(husart3.Instance->DR & (uint8_t)0x00FF);
		{
            if((TimeInterval>2)&&(TimeInterval<=4))
            {
                bufferIndex=0;
            }
            
            TimeInterval=0;
            __HAL_TIM_ENABLE(&htim6);
                
            CommandBuffer[bufferIndex++]=dat;
            if(bufferIndex>BUFFERLENTH-1)
            {
                bufferIndex=0;
                
                
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) UART3_RX_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);	
				
				
				/*
				uartRecieveRes=analyzeUartPacket(); 
                if (uartRecieveRes == ANALYZE_OK)
                {
                }
				*/
            }


 /*        
    		if(dat==0xff)
    		{
    			bufferIndex=0;
    		}
    		else if(dat==0xfe)
    		{
    			CommandBuffer[bufferIndex++]=dat;
    			if(bufferIndex>6-1)
    				bufferIndex=0;
    			else
    			{
    				uartRecieveRes=analyzeUartPacket();
    				
    				if(uartRecieveRes!=ANALYZE_OK)
    				{	
    					//put_msg_fifo(MSG_COMMAND_ERROR);					
    				}
    			}
    			return;
    		}

    		CommandBuffer[bufferIndex++]=dat;
    		if(bufferIndex>BUFFERLENTH-1)
    		{
    			bufferIndex=0;	
    		}
*/
             
        }
	}
	
	#endif
}
		
#endif






/*----------------------------------------------------------------------------*/
u8 analyzeUartPacket(void)
{
	u8 temp;
	u8 res=ANALYZE_OK;
	
	INT8U err;

	Packet.sync=CommandBuffer[0];
	Packet.addr=CommandBuffer[1];
	Packet.command=CommandBuffer[2];
	Packet.data=CommandBuffer[3];
	Packet.checksum=CommandBuffer[4];
	temp=Packet.addr+Packet.command+Packet.data;
	

/*
	Packet.sync=Uart3_RxBuff[0];
	Packet.addr=Uart3_RxBuff[1];
	Packet.command=Uart3_RxBuff[2];
	Packet.data=Uart3_RxBuff[3];
	Packet.checksum=Uart3_RxBuff[4];
	
	*/
	
	temp=Packet.addr+Packet.command+Packet.data;
	
	//temp=Packet.addr^Packet.command^Packet.data;
	
	
	
	
	/*
	if(Packet.lenth>1)
	{
		u8 i;
		u16 sum=0;
		sum=temp;
		for(i=0;i<Packet.lenth-1;i++)
			sum+=CommandBuffer[4+i];
		temp=sum%100;
	}
	*/
	
	
	
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);
	
	
	if(Packet.sync!=UART_SYNC)
		res=SYNC_ERROR;
	
	//else if(Packet.start!=UART_START)
	//	res=START_BYTE_LOSE;
	
	
	else if(Packet.addr>=0xfd)
		res=START_BYTE_LOSE;
	
	
	//else if((Packet.lenth<1)||(Packet.lenth>BUFFERLENTH-4))
	//   res=LENTH_ERROR;

	else if(temp!=Packet.checksum)
		res=CHECKSUM_ERROR;
	else
	{
      	//status_flash=1;
		switch(Packet.command)
		{
			case MIDI_PLAY:	
			{
				
				if (RamSetParameters.MusicDelayTime ==0)
				{
					break;
				}
				
				MIDI_StopPlayFlag=0;
				MIDI_PlayFlag =1;
				
				MIDI_StopFlag =0;
				MIDI_AutoPlayFlag =0;	
				MIDI_AutoPlayCnt =0;
				Midi_AutoLightsCnt=0;
				
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_STOP_FLAG| MIDI_AUTO_PLAY_FLAG | MIDI_STOP_PLAY_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);		
				
				
				
				if (1==Keyboard_Enable_Flag)
				{
					Keyboard_21key();
				}
				else
				{
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_PLAY_FLAG,
						(INT8U) OS_FLAG_SET,
						(INT8U  *)&err);	
				
					#if 1
					if (++IR_Rx_Cnt >RamSetParameters.TrampleNumber)
					{	
						MIDI_AutoPlayFlag =1;

						OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
									(OS_FLAGS) TRAMPLE_SPEEDING_FLAG,
									(INT8U) OS_FLAG_SET,
									(INT8U  *)&err);	
						
					}
					#endif
				}
				
				//if (AutoLightsFlag)
				
				
				if ((RamSetParameters.LightsDelayTime >0) && (RamSetParameters.LightsDelayTime <99999))
				{
					OSFlagAccept ((OS_FLAG_GRP *)pFlagGrpMidi,
								 (OS_FLAGS) AUTO_LIGHT_FLAG ,
								 (INT8U )OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME,
								 (INT8U *)&err);
								 
					if (OS_ERR_FLAG_NOT_RDY == err)			 
					{
						
					}
					else
					{					
						
						Cycles=0;
						Status=0;
						flag=0;
						TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,0);	
					}	
				}	
			}
			break;
			
			
			case MIDI_STOP:
				
			
				#if 0
				if(!MIDI_StopPlayFlag)
				{
					MIDI_StopPlayFlag=1;
					MIDI_PlayFlag =0;
					MIDI_StopFlag =1;
					
					AllLightsOff();
				}
				#endif
				
				OSTimeDlyHMSM(0, 0,0,100);
				
				//TxDeviceCmd(DEFAULT_SLAVE_ADDR,SYNC_CMD,0);
				TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_ON,LightsColour);
				break; 
				
				
			case STUDY_CMD:	
				
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) STUDY_OK_FLAG,
						(INT8U) OS_FLAG_SET,
						(INT8U  *)&err);		
				break;
			
			
			default:
				res=COMMAND_UNDEFINE;
				break;	
			}
		}
		return res;
}



/*
 * 函数名：USART3_Config
 * 描述  ：USART3 GPIO 配置,工作模式配置。9600 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;

	/* config USART3 clock */
    //RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* USART3 GPIO config */
    /* Configure USART3 Tx (PB.10) as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);    
    /* Configure USART3 Rx (PB.11) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure USART3 Rx (PB.12) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART3_RE_RX();
	
	  
	/* USART3 mode config */
	//USART_InitStructure.USART_BaudRate = 115200;

    husart3.Instance=USART3;   
    husart3.Init.BaudRate = baudrate;
    husart3.Init.WordLength = USART_WORDLENGTH_8B;
    husart3.Init.StopBits = USART_STOPBITS_1;
    husart3.Init.Parity = USART_PARITY_NONE ;
    husart3.Init.Mode = USART_MODE_RX | USART_MODE_TX;
    husart3.Init.CLKLastBit= USART_LASTBIT_DISABLE;
    husart3.Init.CLKPhase= USART_PHASE_1EDGE;
    husart3.Init.CLKPolarity =USART_POLARITY_LOW;  
        
    HAL_USART_Init(&husart3);
    __HAL_USART_ENABLE_IT(&husart3, USART_IT_RXNE);
    
	USART3_NVIC_Configuration();
    
	TIM6_Configuration();
	
	
	//HAL_USART_Receive_IT(&husart3,Uart3_RxBuff,6);
	
	
    __HAL_UART_ENABLE(&husart3);
}

void Uart3SendData(unsigned char ch)
{
	USART3_RE_TX();
	/* 将Printf内容发往串口 */
	//USART_SendData(USART3, (unsigned char) ch);


    HAL_USART_Transmit(&husart3,&ch,1,10);

    
	//while (!(USART3->SR & USART_FLAG_TXE));
	//while (!USART_GetFlagStatus(USART3, USART_FLAG_TC));

    while (!__HAL_USART_GET_FLAG(&husart3, USART_FLAG_TC));

    
	USART3_RE_RX();
 
}

#ifdef USART3_IRQ_EN 
//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void USART3_NVIC_Configuration(void)
{
/*
  NVIC_InitTypeDef NVIC_InitStructure;	//定义数据结构体
  
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//将中断矢量放到Flash的0地址

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级配置的模式，详情请阅读原材料中的文章

  //使能串口中断，并设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);	//将结构体丢到配置函数，即写入到对应寄存器中
  */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}
#endif


#if 1
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	
	USART3_RE_TX();
	/* 将Printf内容发往串口 */
	//USART_SendData(USART1, (unsigned char) ch);
	//while (!(USART1->SR & USART_FLAG_TXE));
	
	HAL_USART_Transmit(&husart3,(uint8_t*)&ch,1,10);
    while (!__HAL_USART_GET_FLAG(&husart3, USART_FLAG_TC));

	
	USART3_RE_RX();
	return (ch);
}
#endif


void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
    INT8U  err;

    if (husart->Instance == USART3)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART3_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);    

        USART3_RE_RX();
		//HAL_USART_Receive_IT(&husart3,Uart3_RxBuff,6);
    }  
	else if (husart->Instance == USART1)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART1_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);   
	}			
}

/*
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
	
	INT8U  err;
	
    if (husart->Instance == USART3)
    {
       
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) UART3_RX_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);	
		
		//USART3_RE_RX();
		
		//HAL_USART_Receive_IT(&husart3,Uart3_RxBuff,6);
		
		
    }
}
*/



void TxDeviceCmd(u8 addr,u8 cmd,u8 dat)
{
    INT8U  err;
	u8 temp;

	temp = addr + cmd + dat;
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);

    Uart3_TxBuff[0]=UART_SYNC;
    Uart3_TxBuff[1]=addr;
    Uart3_TxBuff[2]=cmd;
    Uart3_TxBuff[3]=dat;
    Uart3_TxBuff[4]=temp;
    Uart3_TxBuff[5]=UART_END;
    USART3_RE_TX();
    HAL_USART_Transmit_IT(&husart3,Uart3_TxBuff,6);

    OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
                (OS_FLAGS) UART3_TX_OK_FLAG ,
                (INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
                (INT32U) 0,
                (INT8U *)&err);

	USART3_RE_RX();


    
	
	
	//HAL_USART_TransmitReceive_IT(&husart3,Uart3_TxBuff,Uart3_RxBuff,6);
	//OSTimeDlyHMSM(0, 0,0,10);	
	
	//USART3_RE_RX();	
	//HAL_USART_Receive_IT(&husart3,Uart3_RxBuff,6);

 /*   
	Uart3SendData(UART_SYNC);
	Uart3SendData(addr);
	Uart3SendData(cmd);
	Uart3SendData(dat);
	Uart3SendData(temp);
	Uart3SendData(UART_END);
	*/
}
#if 0
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


#endif


