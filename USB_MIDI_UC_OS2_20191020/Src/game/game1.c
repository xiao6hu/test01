/****************************游戏跳房子**************************************/
#include "includes.h"

extern void MyTimr2Callback (OS_TMR *ptmr, void *p_arg);

void game1(void)
{
	RTC_TimeTypeDef GameStartTime;
	RTC_TimeTypeDef GameStopTime;
	GameRankingTypeDef GameUseTime;
	//GameRankingTypeDef *HighestGameRanking;
	
	
	
	//u8 FirstGameStartFlag=1;
	
	u8 CurrentRanking;
	
	INT8U err;	
	UartProtocl *p_msg;	
	
	
	
	u8 RowNumber=0;
	u8 RowsDeviceNumber;
	
	volatile u8 RowNumberBackup;
	//u8 RowsDeviceNumberBackup;
	
	
	
	volatile s16 DeviceAddr;
	volatile u16 RowsDeviceMark=0;
	volatile u16 RowsDeviceNumberAcc=0;
	volatile u8 GameStartFlag=0;
	
	volatile u32 GameUseTimeAccReg;
	
	
	u8 cnt; 
	
	
	//u8 GameCycleFlag=0;
	
	
	
	Timr2=OSTmrCreate (0,
					(RamSetParameters.GamePassTime*1000/10),
					OS_TMR_OPT_ONE_SHOT,
					(OS_TMR_CALLBACK) MyTimr2Callback,
					(void*)0, 
					(INT8U*)"Timr2",
					(INT8U*)&err);
	
	HAL_RTC_GetTime(&RtcHandle,&GameStartTime,RTC_FORMAT_BIN);

#if 1
	//从1-x	
	RowsDeviceNumberAcc=0;
	for (RowNumber=1; RowNumber <= RamSetParameters.MaxRowNumber;RowNumber++)
	{
		RowsDeviceMark=0;
		//GameStartFlag=0;
		
		/*
		if( RowNumber == RamSetParameters.BeepRowNum)
		{
			my_strcpy(VoiceBuff, "加油，你真棒！");
			OSSemPost(XFS5152CE_SemSignal);
			//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
		}
		*/
		
		OSTmrStart ((OS_TMR *)Timr2,(INT8U *)&err);
		
		for (RowsDeviceNumber=1; RowsDeviceNumber <= RamSetParameters.RowsDeviceNumber[RowNumber];RowsDeviceNumber++)
		{
			TxDeviceCmd(RowsDeviceNumberAcc + RowsDeviceNumber,LIGHTS_ON,
						GROUP1_W_LED_BIT_ON|
						GROUP1_R_LED_BIT_ON|
						GROUP1_G_LED_BIT_ON|
						GROUP1_B_LED_BIT_ON);
			RowsDeviceMark |= (1 << RowsDeviceNumber);	
		}
		
		err = OSQFlush(GAME_RX_Q);
		while(1)
		{
			/*
			if (FirstGameStartFlag==1)
			{
				FirstGameStartFlag=0;
				p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, 10*1000, &err));	
			}
			else
			*/
			{
				p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, (RamSetParameters.GamePassTime+1)*1000, &err));
			}
			if (err== OS_ERR_TIMEOUT)
			{
				GameEnd();	
				return;
			}
			/*
			if (GameStartFlag ==0)
			{
				GameStartFlag=1;
				//OSTmrStart ((OS_TMR *)Timr2,(INT8U *)&err);
			}
			*/
			
			
			#if 0
			if (p_msg->command==MIDI_STOP)
			{	
				RowNumberBackup = FindRowsDeviceId(RowNumber,0)-1;
				for (cnt=1; cnt <=RamSetParameters.RowsDeviceNumber[RowNumber];cnt++,RowNumberBackup--)
				{
					if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
					{
						GameFailure(); 
						return;
					}
				}
			}
			#endif
			
			
			
			#if 1
			
			RowNumberBackup = FindRowsDeviceId(RowNumber,0)-1;
			for (cnt=1; cnt <=RamSetParameters.RowsDeviceNumber[RowNumber];cnt++,RowNumberBackup--)
			{
				#if 0
				if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
				{
					if (p_msg->command==MIDI_STOP)
					{
						GameFailure(); 
						return;
						
					}
					else if (p_msg->command==MIDI_PLAY)
					{
						
						OSSemPost(GameModePlay_sem);
						/*
						if (RamSetParameters.GameModeSelect == ADULT_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);

						}
						else if (RamSetParameters.GameModeSelect == BABY_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) XFS5152CE_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);	
						}	
						*/
						
						
					}
				}
				
				#endif
				
				
				#if 1
				
				if (p_msg->command==MIDI_STOP)
				{
					if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
					{
						GameFailure(); 
						return;
					}		
				}
				else if (p_msg->command==MIDI_PLAY)
				{
					
					//OSSemPost(GameModePlay_sem);

					if (RowNumberBackup == p_msg->addr)
					{
						break;
					}
					else
					{
						if (cnt ==RamSetParameters.RowsDeviceNumber[RowNumber])
						{
							if (RamSetParameters.GameModeSelect == ADULT_MODE)
							{
								OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
											(OS_FLAGS) MIDI_PLAY_FLAG,
											(INT8U) OS_FLAG_CLR,
											(INT8U  *)&err);

							}
							else if (RamSetParameters.GameModeSelect == BABY_MODE)
							{
								OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
											(OS_FLAGS) XFS5152CE_PLAY_FLAG,
											(INT8U) OS_FLAG_CLR,
											(INT8U  *)&err);	
							}	
						}
					}
				}
				#endif
			}
			#endif
			
			
			DeviceAddr=p_msg->addr-RowsDeviceNumberAcc;
			switch (DeviceAddr)
			{
				case 1:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 2:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 3:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 4:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 5:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 6:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 7:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 8:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 9:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				default:break;
			}

			if (RowsDeviceMark==0x0000)
			{
				RowsDeviceNumberAcc+=RamSetParameters.RowsDeviceNumber[RowNumber];
				
				OSTmrStop ((OS_TMR *)Timr2,
							OS_TMR_OPT_NONE,
							(void*)0, 
						   (INT8U *)&err);		
							
				if( RowNumber == RamSetParameters.BeepRowNum)
				{
					
					//OSTimeDlyHMSM(0, 0,0,500);
					my_strcpy(VoiceBuff, "加油，你真棒！");
					
					XFS5152CE_Play(XFS5152CE_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
					
					//OSSemPost(XFS5152CE_SemSignal);
					//OSTimeDlyHMSM(0, 0,2,0);
					
					
					//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
				}			
							
			
				break;
			}
		}
	}
	
	#if 1
	//从x-1
	RowsDeviceNumberAcc=RamSetParameters.DeviceTotal-RamSetParameters.RowsDeviceNumber[RamSetParameters.MaxRowNumber]+1;
	for (RowNumber=RamSetParameters.MaxRowNumber-1; RowNumber >=1;RowNumber--)
	{
		RowsDeviceMark=0;
		//GameStartFlag=0;
		/*
		if( RowNumber == RamSetParameters.BeepRowNum)
		{
			my_strcpy(VoiceBuff, "加油，你真棒！");
			OSSemPost(XFS5152CE_SemSignal);
			//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
		}
		*/
		OSTmrStart ((OS_TMR *)Timr2,(INT8U *)&err);
		
		for (RowsDeviceNumber=1; RowsDeviceNumber <=RamSetParameters.RowsDeviceNumber[RowNumber];RowsDeviceNumber++)
		{
			TxDeviceCmd(RowsDeviceNumberAcc - RowsDeviceNumber,LIGHTS_ON,
						GROUP1_W_LED_BIT_ON|
						GROUP1_R_LED_BIT_ON|
						GROUP1_G_LED_BIT_ON|
						GROUP1_B_LED_BIT_ON);
			RowsDeviceMark |= (1 << RowsDeviceNumber);	
		}
		
		err = OSQFlush(GAME_RX_Q);
		while(1)
		{
			p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, (RamSetParameters.GamePassTime+1)*1000, &err));
			if (err== OS_ERR_TIMEOUT)
			{
				GameEnd();	
				return;
			}
			/*
			if (GameStartFlag ==0)
			{
				GameStartFlag=1;
				OSTmrStart ((OS_TMR *)Timr2,(INT8U *)&err);
			}
			
			*/
			
			#if 0
			if (p_msg->command==MIDI_STOP)
			{
				RowNumberBackup = FindRowsDeviceId(RowNumber,1)+1;
				for (cnt=1; cnt <=RamSetParameters.RowsDeviceNumber[RowNumber]; cnt++,RowNumberBackup++)
				{
					if (RowNumberBackup == p_msg->addr)
					{
						GameFailure(); 
						return;
					}
				}
			}
			#endif
			
			
			#if 1
			RowNumberBackup = FindRowsDeviceId(RowNumber,0)-1;
			for (cnt=1; cnt <=RamSetParameters.RowsDeviceNumber[RowNumber];cnt++,RowNumberBackup--)
			{
				
				
				#if 0
				if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
				{
					if (p_msg->command==MIDI_STOP)
					{
						GameFailure(); 
						return;
						
					}
					else if (p_msg->command==MIDI_PLAY)
					{
						OSSemPost(GameModePlay_sem);
						/*
						if (RamSetParameters.GameModeSelect == ADULT_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);

						}
						else if (RamSetParameters.GameModeSelect == BABY_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) XFS5152CE_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);	
						}	
						*/
					}
				}
				#endif
			
				
	
				#if 0
				if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
				{
					if (p_msg->command==MIDI_STOP)
					{
						GameFailure(); 
						return;
						
					}
					else if (p_msg->command==MIDI_PLAY)
					{
						
						OSSemPost(GameModePlay_sem);
						/*
						if (RamSetParameters.GameModeSelect == ADULT_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);

						}
						else if (RamSetParameters.GameModeSelect == BABY_MODE)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) XFS5152CE_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);	
						}	
						*/
						
						
					}
				}
				
				#endif
				
				
				#if 1
				
				if (p_msg->command==MIDI_STOP)
				{
					if ((RowNumberBackup == p_msg->addr)||(p_msg->addr==0x00))
					{
						GameFailure(); 
						return;
					}		
				}
				else if (p_msg->command==MIDI_PLAY)
				{
					
					//OSSemPost(GameModePlay_sem);

					if (RowNumberBackup == p_msg->addr)
					{
						break;
					}
					else
					{
						if (cnt ==RamSetParameters.RowsDeviceNumber[RowNumber])
						{
							if (RamSetParameters.GameModeSelect == ADULT_MODE)
							{
								OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
											(OS_FLAGS) MIDI_PLAY_FLAG,
											(INT8U) OS_FLAG_CLR,
											(INT8U  *)&err);

							}
							else if (RamSetParameters.GameModeSelect == BABY_MODE)
							{
								OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
											(OS_FLAGS) XFS5152CE_PLAY_FLAG,
											(INT8U) OS_FLAG_CLR,
											(INT8U  *)&err);	
							}	
						}
					}	
				}
				#endif
			}
			#endif
			
			DeviceAddr=RowsDeviceNumberAcc-p_msg->addr;
			
			switch (DeviceAddr)
			{
				case 1:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 2:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 3:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 4:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 5:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 6:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 7:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 8:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				case 9:RowsDeviceMark &= ~(1<<DeviceAddr);break;
				default:break;
			}
			if (RowsDeviceMark==0x0000)
			{
				RowsDeviceNumberAcc-=RamSetParameters.RowsDeviceNumber[RowNumber];
				
				
				OSTmrStop ((OS_TMR *)Timr2,
							OS_TMR_OPT_NONE,
							(void*)0, 
						   (INT8U *)&err);
							
							
				if( RowNumber == RamSetParameters.BeepRowNum)
				{
					//OSTimeDlyHMSM(0, 0,0,500);
					my_strcpy(VoiceBuff, "加油，你真棒！");
					XFS5152CE_Play(XFS5152CE_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
					
					//OSSemPost(XFS5152CE_SemSignal);
					//OSTimeDlyHMSM(0, 0,2,0);
					
					
					//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
				}			
				break;
			}
		}
	}

	#endif
	
	#endif
	
	OSTimeDlyHMSM(0, 0,2,0);
	
	HAL_RTC_GetTime(&RtcHandle,&GameStopTime,RTC_FORMAT_BIN);

	GameUseTimeAccReg=((U32)GameStopTime.Hours * 3600uL + (U32)GameStopTime.Minutes * 60uL + (U32)GameStopTime.Seconds)-
	((U32)GameStartTime.Hours * 3600uL + (U32)GameStartTime.Minutes * 60uL + (U32)GameStartTime.Seconds);
	
	//GameUseTime.Hours = ((U32)GameUseTimeAccReg/3600uL);
	GameUseTime.Minutes = ((U32)GameUseTimeAccReg%3600uL/60uL);
	GameUseTime.Seconds = ((U32)GameUseTimeAccReg%3600uL%60uL);
	
	CurrentRanking=GameRankingCmp(&GameUseTime);
	
	my_strcpy(VoiceBuff, "最高纪录");
	snprintf(my_strchr(VoiceBuff,'\0'), 2+1,"%2d", RamSetParameters.GameRanking[0].Minutes);
	my_strcpy(my_strchr(VoiceBuff,'\0'), "分");
	snprintf(my_strchr(VoiceBuff,'\0'), 2+1,"%2d", RamSetParameters.GameRanking[0].Seconds);
	my_strcpy(my_strchr(VoiceBuff,'\0'), "秒，您本次用时");
	snprintf(my_strchr(VoiceBuff,'\0'), 2+1,"%2d", GameUseTime.Minutes);
	my_strcpy(my_strchr(VoiceBuff,'\0'), "分");
	snprintf(my_strchr(VoiceBuff,'\0'), 2+1,"%2d", GameUseTime.Seconds);
	my_strcpy(my_strchr(VoiceBuff,'\0'), "秒，");
	
	if (CurrentRanking >MAX_GAME_RANKING_NUMBER)
	{
		my_strcpy(my_strchr(VoiceBuff,'\0'), "你的速度太慢，不在排名榜内，请再接再厉！");
	}
	else
	{
		my_strcpy(my_strchr(VoiceBuff,'\0'), "排名第");
		snprintf(my_strchr(VoiceBuff,'\0'), 2+1,"%2d", CurrentRanking);
		my_strcpy(my_strchr(VoiceBuff,'\0'), "名");
	}

	my_strcpy(my_strchr(VoiceBuff,'\0'), "，请按开关继续挑战");

	XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
	//OSTimeDlyHMSM(0, 0,5,0);
	//OSSemPost(XFS5152CE_SemSignal);
	//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);		
	
	GameRankingDataSave();
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) START_KEY_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);


	TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,
				(U8)(GROUP1_W_LED_BIT_OFF|
				GROUP1_R_LED_BIT_OFF|
				GROUP1_G_LED_BIT_OFF|
				GROUP1_B_LED_BIT_OFF));
			
}


