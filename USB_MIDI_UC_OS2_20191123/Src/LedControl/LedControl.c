
#include "includes.h"

#include "CRC16.h"


u8 LedControl_TxBuff[5+32+2];

LedParaStructTypeDef LedParaData;

FpsSetStructTypeDef FpsSetData;
u8 LedData[32];


u8 LedControl[32];

u8 LocalTimerOnOffFlag=TIMER_OFF;
u8 T1000TimerOnOffFlag=TIMER_OFF;

//
void TxLedControlCmd(u16 length,u8 addr, u8 command, u8 *data)
{
	INT8U  err;
	U16 CRC16_Val;
	
	LedControl[0] = LED_FRAME_HEADER;
	LedControl[1] = addr;
	LedControl[2] = command;
	LedControl[3] = (u8)(length>>8);
	LedControl[4] = (u8)(length&0x00ff);
	my_memcpy((U8*)&LedControl[5],data,length);
	CRC16_Val= CRC16(LedControl,length+5);
	
	LedControl[length+5] =(u8)(CRC16_Val >>8);
	LedControl[length+5+1] =(u8)(CRC16_Val &0x00FF);
	
	HAL_UART_Transmit_IT(&huart2,LedControl,length+5+2);
	
	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) UART2_TX_OK_FLAG,
				(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
				(INT32U) 0,
				(INT8U *)&err);
}


void TxPlayLedControl(UartProtocl *pPacket)
{
	#if 0
	RamSetParameters.LedPara.LedGroupsNumber=pPacket->addr;
	LedParaData= RamSetParameters.LedPara;
	
	TxLedControlCmd((sizeof(LedParaData) / sizeof (uint8_t)),
					((pPacket->addr * RamSetParameters.LedPara.GroupsLedNumber) / RamSetParameters.LedPara.MaxLedNumber)+1,
					 LED_PLAY_CMD,
					(u8 *) &LedParaData);
	#endif
}
	
void TxStopLedControl(UartProtocl *pPacket)
{
	#if 0
	FpsSetData.fps= RamSetParameters.LedPara.StopFps;
	TxLedControlCmd((sizeof(FpsSetData) / sizeof (uint8_t)),
					((pPacket->addr * RamSetParameters.LedPara.GroupsLedNumber) / RamSetParameters.LedPara.MaxLedNumber)+1,
					LED_SET_FPS_CMD,
					(u8 *) &FpsSetData);
	#endif
}



void TxStudyCodeCommands(u8 ControlAddr)
{
	TxLedControlCmd(0x0001,DEFAULT_SLAVE_ADDR, STUDY_CMD, &ControlAddr);
}

void LedColorCycleControl(void)
{
	static u16 LedColorCycleTime=0;
	
	
	if (RamSetParameters.LedControl.LedColorCycleTime==0)
	{
		LedColorCycleTime=0x0000;
	}
	else if (RamSetParameters.LedControl.LedColorCycleTime==9999)
	{
		LedColorCycleTime=0xffff;
		RamSetParameters.LedPara.LedColorCycleMode=0;
	}
	

	if (++LedColorCycleTime>RamSetParameters.LedControl.LedColorCycleTime*2)
	{
		LedColorCycleTime=0;
		if (RamSetParameters.LedPara.LedColorCycleMode<3)
		{
			RamSetParameters.LedPara.LedColorCycleMode++;
		}	
		else
		{
			RamSetParameters.LedPara.LedColorCycleMode=0;
		}
	}
}


u8 WeekChange[8]={0,1,2,3,4,5,6,7};


void LedTimerControl(void)
{
	u8 TimerNumber;
	RTC_TimeTypeDef TimerTime;
	RTC_DateTypeDef RTC_Date;
	
	u32 TimerTime_SecNum;
	u32 TimerOnTime_SecNum;
	u32 TimerOffTime_SecNum;
	
	HAL_RTC_GetDate(&RtcHandle,&RTC_Date,RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&RtcHandle,&TimerTime,RTC_FORMAT_BIN);

	TimerTime_SecNum = ((U32)TimerTime.Hours * 3600uL + (U32)TimerTime.Minutes * 60uL + (U32)TimerTime.Seconds);

	for (TimerNumber=0; TimerNumber<TIMER_NUMBER; TimerNumber++)
	{
		if (RamSetParameters.LedControl.Local_OnOff==TIMER_ON)
		{
			if (RamSetParameters.LedControl.Local_Timer[TimerNumber].Status==TIMER_ON)
			{
				if ((RamSetParameters.LedControl.Local_Timer[TimerNumber].Week & (1 << WeekChange[RTC_Date.WeekDay & 0x07])) == (1 << WeekChange[RTC_Date.WeekDay & 0x07]))
				{
					TimerOnTime_SecNum = ((U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOnTime.Hours * 3600uL 
										+ (U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOnTime.Minutes * 60uL 
										+ (U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOnTime.Seconds);
					
					TimerOffTime_SecNum = ((U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOffTime.Hours * 3600uL 
										+ (U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOffTime.Minutes * 60uL 
										+ (U32)RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerOffTime.Seconds);

					if ((TimerTime_SecNum >= TimerOnTime_SecNum) && (TimerTime_SecNum < TimerOffTime_SecNum)) 
					{
						LOCAL_TIMING_ON();
						break;
					}
				}
			}
		}
		else
		{
			LOCAL_TIMING_ON();
			break;
		}
		if (TimerNumber == TIMER_NUMBER)
		{
			LOCAL_TIMING_OFF();
		}	
	}
		
	for (TimerNumber=0; TimerNumber<TIMER_NUMBER;TimerNumber++)
	{
		if (RamSetParameters.LedControl.T1000_OnOff==TIMER_ON)
		{
			if (RamSetParameters.LedControl.T1000_Timer[TimerNumber].Status==TIMER_ON)
			{
				if ((RamSetParameters.LedControl.T1000_Timer[TimerNumber].Week & (1 << WeekChange[RTC_Date.WeekDay & 0x07])) == (1 << WeekChange[RTC_Date.WeekDay & 0x07]))
				{
					TimerOnTime_SecNum = ((U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOnTime.Hours * 3600uL 
										+ (U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOnTime.Minutes * 60uL 
										+ (U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOnTime.Seconds);
					
					TimerOffTime_SecNum = ((U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOffTime.Hours * 3600uL 
										+ (U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOffTime.Minutes * 60uL 
										+ (U32)RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerOffTime.Seconds);

					if ((TimerTime_SecNum >= TimerOnTime_SecNum) && (TimerTime_SecNum < TimerOffTime_SecNum)) 
					{
						T1000_TIMING_ON();
						break;
					}		
				}
			}
		}
		else
		{
			T1000_TIMING_ON();
			break;
		}
	}
	if (TimerNumber == TIMER_NUMBER)
	{
		T1000_TIMING_OFF();
	}			
}


/*
void LedTimerControl(void)
{
	u8 TimerNumber;
	RTC_TimeTypeDef TimerTime;

	HAL_RTC_GetTime(&RtcHandle,&TimerTime,RTC_FORMAT_BIN);

	for (TimerNumber=0; TimerNumber<TIMER_NUMBER;TimerNumber++)
	{
		if (RamSetParameters.LedControl.Local_OnOff==TIMER_ON)
		{
			if (RamSetParameters.LedControl.Local_Timer[TimerNumber].Status==TIMER_ON)
			{
				if ((RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerTime.Hours == TimerTime.Hours) 
					&& (RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerTime.Minutes == TimerTime.Minutes) 
					&& (RamSetParameters.LedControl.Local_Timer[TimerNumber].TimerTime.Seconds >= TimerTime.Seconds)) 
				{
					if (RamSetParameters.LedControl.Local_Timer[TimerNumber].OnOff==TIMER_ON)
					{
						LOCAL_TIMING_ON();
					}
					else
					{
						LOCAL_TIMING_OFF();
					}	
				}
			}
		}
		else
		{
			LOCAL_TIMING_ON();
			
		}
		
		
		if (RamSetParameters.LedControl.T1000_OnOff==TIMER_ON)
		{
			if (RamSetParameters.LedControl.T1000_Timer[TimerNumber].Status==TIMER_ON)
			{
				if ((RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerTime.Hours == TimerTime.Hours) 
					&& (RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerTime.Minutes == TimerTime.Minutes) 
					&& (RamSetParameters.LedControl.T1000_Timer[TimerNumber].TimerTime.Seconds >= TimerTime.Seconds)) 
				{
					if (RamSetParameters.LedControl.T1000_Timer[TimerNumber].OnOff==TIMER_ON)
					{
						T1000_TIMING_ON();
					}
					else
					{
						T1000_TIMING_OFF();
					}	
				}
			}
		}
		else
		{
			T1000_TIMING_ON();
		}
	}		
}
*/

