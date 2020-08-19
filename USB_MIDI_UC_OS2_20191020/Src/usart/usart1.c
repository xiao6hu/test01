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
#include "usart1.h"
#include <stdarg.h>
#include "mid.h"

#include "lights.h"

#include "Keyboard.h"

#include "mco.h"

#include "CANOpenObjDictConfig.h"
#include "GlobalVar.h"
#include "ds401.h"


extern U8 * _CONST_ DeviceIdToEnglishChar[];

extern U8 * _CONST_ DeviceIdToChinesePinyin[];

extern U8 * _CONST_ DeviceIdToChinesePinyin_Chinese[];       
//UART_HandleTypeDef husart1;					
UART_HandleTypeDef huart1;		


TIM_HandleTypeDef htim6;

u8 Uart1_TxBuff[64];
u8 Uart1_RxBuff[64];


u8 CommandBuffer[BUFFERLENTH];

U8 TimeInterval;

U8 Uart1_TxBitLen=10;
u32 Uart1_baudrate=115200;

u8 bufferIndex;


UartProtocl Packet;
u8 uartRecieveRes=ANALYZE_OK;
u8 IR_Rx_Cnt =0;


extern u32 *pTPRO1_val[2];
extern u32 *pRPRO1_val[2];
extern u32 * pODNodeID;

U8 StudyFlag=0;



#if 1
void TIM6_Configuration(void)
{

	__HAL_RCC_TIM6_CLK_ENABLE();
    htim6.Instance=TIM6;
    htim6.Init.Period= (((SystemCoreClock / Uart1_baudrate/120) * Uart1_TxBitLen)-1);	/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    htim6.Init.Prescaler= (120);				    /* 时钟预分频数 120M/120 */
    htim6.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; 		/* 采样分频 */
    htim6.Init.CounterMode=TIM_COUNTERMODE_UP; /* 向上计数模式 */
    HAL_TIM_Base_Init(&htim6);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
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

void USART1_IRQHandler(void)
{  	
	u8 dat;
    //u8 uartRecieveRes;
	//INT8U err;
	
    HAL_UART_IRQHandler(&huart1);

	#if 1
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
  	{ 
		__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
		
		//if(HAL_OK ==HAL_USART_Receive(&husart1, &dat, 1, 0))	
		
		dat =(uint8_t)(huart1.Instance->DR & (uint8_t)0x00FF);
		{
			
			Post_XFS5152CE_RX_Msg(dat);
			
			XFS5152CE_ReturnStatus(dat);
		
			
			
			
			/*
			switch (dat)
			{	
				
				
				
				case XFS5152CE_INIT_SUCCESS:  	break;	//0x4A //芯片初始化成功
				case XFS5152CE_RX_OK:  			break;	//0x41 //收到正确的命令帧
				case XFS5152CE_RX_ERROR:  		break;	//0x45 //收到错误的命令帧
				case XFS5152CE_BUSY: 			break;	//0x4E //芯片忙碌
				case XFS5152CE_IDLE: 			break;	//0x4F //芯片空闲
				default: break;	
			}
			*/

			/*
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

			*/	
				/*
				
				
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) UART1_RX_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);	
				
				*/
				/*
				uartRecieveRes=analyzeUartPacket(); 
                if (uartRecieveRes == ANALYZE_OK)
                {
                }
				*/
            //}


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
		
/*----------------------------------------------------------------------------*/
u8 analyzeUartPacket()
{
	//u8 temp;
	
	u8 res=ANALYZE_OK;
	INT8U err;
	//OS_FLAGS  Flags;

	//u8 RxData[8];
	
	u8 newOutput[8];
	
	
	/*
	Packet.sync=CommandBuffer[0];
	Packet.addr=CommandBuffer[1];
	Packet.command=CommandBuffer[2];
	Packet.data=CommandBuffer[3];
	Packet.checksum=CommandBuffer[4];
	temp=Packet.addr+Packet.command+Packet.data;
	*/
/*
	RxData[0]= MCO_GET_8BIT_VAL(*pRPRO1_val[0],0);
	RxData[1]= MCO_GET_8BIT_VAL(*pRPRO1_val[0],1);
	RxData[2]= MCO_GET_8BIT_VAL(*pRPRO1_val[0],2);
	RxData[3]= MCO_GET_8BIT_VAL(*pRPRO1_val[0],3);
	RxData[4]= MCO_GET_8BIT_VAL(*pRPRO1_val[1],0);
	RxData[5]= MCO_GET_8BIT_VAL(*pRPRO1_val[1],1);
	RxData[6]= MCO_GET_8BIT_VAL(*pRPRO1_val[1],2);
	RxData[7]= MCO_GET_8BIT_VAL(*pRPRO1_val[1],3);

	*pRPRO1_val[0]=0x00000000;
	*pRPRO1_val[1]=0x00000000;
*/		
	
	
	

	/*
	//Packet.sync=m->data[0];
	Packet.addr=m->data[0];
	Packet.command=m->data[1];
	Packet.data=m->data[2];
*/

	/*
	Packet.addr=RxData[0];
	Packet.command=RxData[1];
	Packet.data=RxData[2];
	*/
/*
	Packet.sync=Uart3_RxBuff[0];
	Packet.addr=Uart3_RxBuff[1];
	Packet.command=Uart3_RxBuff[2];
	Packet.data=Uart3_RxBuff[3];
	Packet.checksum=Uart3_RxBuff[4];
	
	*/
	
	digital_output_handler(CANOpenMasterObject,newOutput, 8);
	
	
	if (CANOpenMasterObject->canHandle== CAN1)
	{
		Packet.addr=newOutput[0];
	}
	else if (CANOpenMasterObject->canHandle== CAN2)
	{
		Packet.addr=newOutput[0] + RamSetParameters.CAN_DeviceTotal.CAN_Device1;
	}
	else
	{
		return CAN_HANDLE_UNDEFINE;
	}
	
	//Packet.addr=newOutput[0];
	Packet.command=newOutput[1];
	Packet.data=newOutput[2];

	Write_Outputs_8_Bit[0] =0x00;
	Write_Outputs_8_Bit[1] =0x00;
	Write_Outputs_8_Bit[2] =0x00;
	Write_Outputs_8_Bit[3] =0x00;
	Write_Outputs_8_Bit[4] =0x00;
	Write_Outputs_8_Bit[5] =0x00;
	Write_Outputs_8_Bit[6] =0x00;
	Write_Outputs_8_Bit[7] =0x00;
	
	
	
	//temp=Packet.addr+Packet.command+Packet.data;
	
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
	
	
	/*
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);
	*/
	/*
	if(Packet.sync!=UART_SYNC)
		res=SYNC_ERROR;
	*/
	
	//else if(Packet.start!=UART_START)
	//	res=START_BYTE_LOSE;
	
	/*
	else if(Packet.addr>=0xfd)
		res=START_BYTE_LOSE;
	
	
	//else if((Packet.lenth<1)||(Packet.lenth>BUFFERLENTH-4))
	//   res=LENTH_ERROR;

	else if(temp!=Packet.checksum)
		res=CHECKSUM_ERROR;
	else
	*/	
	
	
	{
		Post_GAME_RX_Msg(Packet);
		
		
      	//status_flash=1;
		switch(Packet.command)
		{
			case MIDI_PLAY:	
			{
				
				OSTimeDlyHMSM(0, 0,0,10);
				
				ParaUpdate();

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
				
				
				//TxPlayLedControl(&Packet);
				

				
				//Post_GAME_RX_Msg(Packet);

				//成人模式
				if (RamSetParameters.GameModeSelect == ADULT_MODE)
				{
					if (1==Keyboard_Enable_Flag)
					{
						Keyboard_21key();
					}
					else
					{
						/*
						if ((Flags & START_KEY_FLAG) == START_KEY_FLAG)
						{
						
							OSSemPend(GameModePlay_sem,0,&err);
						}
						*/
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
				}
				//儿童模式
				else if (RamSetParameters.GameModeSelect == BABY_MODE)
				{
					/*
					if ((Flags & START_KEY_FLAG) == START_KEY_FLAG)
					{
					
						OSSemPend(GameModePlay_sem,0,&err);
					}
					*/
					if (RamSetParameters.SayModeSelect == ENGLISH_CHAR_MODE)
					{	
						
						my_strcpy(VoiceBuff, DeviceIdToEnglishChar[RamSetParameters.DeviceIdToEnglishChar[Packet.addr]]);
						my_strcpy(my_strchr(VoiceBuff,'\0'), "。");
						XFS5152CE_Play(XFS5152CE_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
						
						//OSSemPost(XFS5152CE_SemSignal);
						
						//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
						
						
						//XFS5152CE_SendData(1+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, (u8*)&DeviceIdToEnglishChar[RamSetParameters.DeviceIdToEnglishChar[Packet.addr]]);
					}
					else if (RamSetParameters.SayModeSelect == CHINESE_PIN_YIN_MODE)
					{
						
						
						my_strcpy(VoiceBuff, DeviceIdToChinesePinyin_Chinese[RamSetParameters.DeviceIdToChinesePinyin[Packet.addr]]);
						//my_strcpy(my_strchr(VoiceBuff,'\0'), "[i1],");
						//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
						
						//OSSemPost(XFS5152CE_SemSignal);
						XFS5152CE_Play(XFS5152CE_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
						
						//XFS5152CE_SendData(1+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, (u8*)&DeviceIdToChinesePinyin[RamSetParameters.DeviceIdToChinesePinyin[Packet.addr]]);
						
					}
					
					//err = OSQFlush(GAME_RX_Q);
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
				
			
			
				LedColorCycleControl();
			
				//Post_GAME_RX_Msg(Packet);
				#if 0
				if(!MIDI_StopPlayFlag)
				{
					MIDI_StopPlayFlag=1;
					MIDI_PlayFlag =0;
					MIDI_StopFlag =1;
					
					AllLightsOff();
				}
				#endif
				
				
				//OSTimeDlyHMSM(0, 0,0,100);
				
				//TxDeviceCmd(DEFAULT_SLAVE_ADDR,SYNC_CMD,0);
				
				
				
				//TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_ON,LightsColour);
				
				
				
				//TxStopLedControl(&Packet);
				
				
				
				
				
				break; 
				
				
			case STUDY_CMD:	
				
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) STUDY_OK_FLAG,
						(INT8U) OS_FLAG_SET,
						(INT8U  *)&err);		
				break;
			
			case PARA_SET_CMD:
				SlaveParaSet();
				break;
			
			default:
				res=COMMAND_UNDEFINE;
				break;	
			}
		}
		return res;
}

#if 0
/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

#endif

#if 1
/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。9600 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* config USART1 clock */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/* USART1 GPIO config */
    /* Configure USART1 Tx (PA.9) as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_9; 
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull= GPIO_PULLUP;
	GPIO_InitStructure.Alternate= GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 mode config */
    huart1.Instance=USART1;   
    huart1.Init.BaudRate = Uart1_baudrate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE ;
    huart1.Init.Mode = UART_MODE_RX | UART_MODE_TX;
	
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	
	/*
    husart1.Init.CLKLastBit= USART_LASTBIT_DISABLE;
    husart1.Init.CLKPhase= USART_PHASE_1EDGE;
    husart1.Init.CLKPolarity =USART_POLARITY_LOW;  
	*/
	
	
    HAL_UART_Init(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	
	USART1_NVIC_Configuration();
	//TIM6_Configuration();
    __HAL_UART_ENABLE(&huart1);
}

#endif




void Uart1SendData(unsigned char ch)
{
	/* 将Printf内容发往串口 */
    HAL_UART_Transmit(&huart1,&ch,1,10);
    while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)); 
	__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_TC);
}

//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void USART1_NVIC_Configuration(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

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
	/* 将Printf内容发往串口 */
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,10);
    while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC));
	return (ch);
}
#endif

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    INT8U  err;
	/*
    if (husart->Instance == USART3)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART3_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);    
    }  
	else
	*/
	
	if (huart->Instance == USART1)
    {
        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART1_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);   
	}	
	else if (huart->Instance == USART2)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART2_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);    
    }  
	else if (huart->Instance == UART4)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART4_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);    
    }  
}

/*
void TxDeviceCmd(u8 addr,u8 cmd,u8 dat)
{
    INT8U  err;
	
	
	u8 temp;

	temp = addr + cmd + dat;
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);

    Uart1_TxBuff[0]=UART_SYNC;
    Uart1_TxBuff[1]=addr;
    Uart1_TxBuff[2]=cmd;
    Uart1_TxBuff[3]=dat;
    Uart1_TxBuff[4]=temp;
    Uart1_TxBuff[5]=UART_END;
		
    HAL_USART_Transmit_IT(&husart1,Uart1_TxBuff,6);

    OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
                (OS_FLAGS) UART1_TX_OK_FLAG ,
                (INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
                (INT32U) 0,
                (INT8U *)&err);				
}
*/




void TxDeviceCmd(u8 addr,u8 cmd,u8 dat)
{
	
	u8 newInput[8];//={0,0,0,0,0,0,0,0};
	UNS32 TPDO1_COBID = 0x0180; 
	UNS32 size = sizeof(UNS32); 
	u8 NodeId;
	
	#if 0
	Message TxMessage;
	if ((addr==0x00) || ((addr >0) &&(addr <= RamSetParameters.CAN_DeviceTotal.CAN_Device1)))
	{
		TxMessage.cob_id=addr;	/**< message's ID */
		TxMessage.rtr=0;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
		TxMessage.len=3;		/**< message's length (0 to 8) */
		//TxMessage.data[0]=UART_SYNC;
		TxMessage.data[0]=addr;
		TxMessage.data[1]=cmd;
		TxMessage.data[2]=dat;
		canSend(CAN1,&TxMessage);	
	}
	else if ((addr==0x00) || ((addr >RamSetParameters.CAN_DeviceTotal.CAN_Device1) && (addr <= (RamSetParameters.CAN_DeviceTotal.CAN_Device2+RamSetParameters.CAN_DeviceTotal.CAN_Device1))))
	{
		TxMessage.cob_id=addr;	/**< message's ID */
		TxMessage.rtr=0;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
		TxMessage.len=3;		/**< message's length (0 to 8) */
		//TxMessage.data[0]=UART_SYNC;
		TxMessage.data[0]=addr;
		TxMessage.data[1]=cmd;
		TxMessage.data[2]=dat;
		canSend(CAN2,&TxMessage);	
	}
	
	#endif
	
	
	if (StudyFlag==1)
	{
		NodeId= dat;
	}
	else
	{
		NodeId=addr;
	}

	//if ((addr==0x00) || ((addr >0) &&(addr <= RamSetParameters.CAN_DeviceTotal.CAN_Device1)))
	if (((NodeId >0) &&(NodeId <= RamSetParameters.CAN_DeviceTotal.CAN_Device1)))	
	{
		/*
		*pODNodeID= 0x180 + (addr & 0x7F);
		*pTPRO1_val[0]=MCO_SET_32BIT_VAL(addr,cmd,dat,0x00);
		*pTPRO1_val[1]=0x00000000;		
		*/

		SetCanHandle(CANOpenMasterObject, CAN1);		
	}
	//else if ((addr==0x00) || ((addr >RamSetParameters.CAN_DeviceTotal.CAN_Device1) && (addr <= (RamSetParameters.CAN_DeviceTotal.CAN_Device2+RamSetParameters.CAN_DeviceTotal.CAN_Device1))))
	else if (((NodeId >RamSetParameters.CAN_DeviceTotal.CAN_Device1) && (NodeId <= (RamSetParameters.CAN_DeviceTotal.CAN_Device2+RamSetParameters.CAN_DeviceTotal.CAN_Device1))))
	{
		/*
		*pODNodeID= 0x180 + (addr & 0x7F);
		*pTPRO1_val[0]=MCO_SET_32BIT_VAL(addr,cmd,dat,0x00);
		*pTPRO1_val[1]=0x00000000;	
		*/
		
		if (StudyFlag==1)
		{
			dat -=RamSetParameters.CAN_DeviceTotal.CAN_Device1;
		}
		else
		{
			addr -=RamSetParameters.CAN_DeviceTotal.CAN_Device1;
		}
		SetCanHandle(CANOpenMasterObject, CAN2);
	}	
	newInput[0]=addr;
	newInput[1]=cmd;
	newInput[2]=dat;
	newInput[3]=0x00;
	newInput[4]=0x00;
	newInput[5]=0x00;
	newInput[6]=0x00;
	newInput[7]=0x00;
	
	
	
	
	TPDO1_COBID |= (addr & 0x7f);
	//setNodeId (CANOpenMasterObject, addr);
	writeLocalDict(CANOpenMasterObject, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&TPDO1_COBID, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	digital_input_handler(CANOpenMasterObject, newInput, 8);	
}


void SimulateGameFailure(void)
{
	
	Packet.addr=0x00;
	Packet.command=MIDI_STOP;
	Packet.data=0x00;
	Post_GAME_RX_Msg(Packet);
}




