/****************************游戏追光跑道**************************************/
#include "includes.h"

extern void MyTimr2Callback (OS_TMR *ptmr, void *p_arg);






void game2(void)
{
	RTC_TimeTypeDef GameStartTime;
	RTC_TimeTypeDef GameStopTime;
	GameRankingTypeDef GameUseTime;

	volatile u32 GameUseTimeAccReg;
	
	u8 CurrentRanking;
	INT8U err;	
	volatile UartProtocl *p_msg;	

	//OS_FLAGS  Flags;

	u32 GameFailDelayTime;
	#define GAME_FAIL_PER_TICKS 10


	err = OSQFlush(GAME_RX_Q);
	do
	{ 
		//p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, 0, &err));
		OSTimeDlyHMSM(0, 0,0,10);
		p_msg = (UartProtocl *)OSQAccept(GAME_RX_Q, &err);
		
	}while ((p_msg->command != MIDI_PLAY) || (p_msg->addr != RamSetParameters.GameStartDeviceAddr));


	/*
	MIDI_StopPlayFlag=0;
	MIDI_PlayFlag =0;
	MIDI_StopFlag =0;
	MIDI_AutoPlayFlag =0;	
	MIDI_AutoPlayCnt =0;
	Midi_AutoLightsCnt=0;
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
		(OS_FLAGS) MIDI_STOP_FLAG| MIDI_AUTO_PLAY_FLAG | MIDI_STOP_PLAY_FLAG | AUTO_LIGHT_FLAG,
		(INT8U) OS_FLAG_CLR,
		(INT8U  *)&err);	

	Cycles=0;
	Status=0;
	flag=0;
*/
	
	my_strcpy(VoiceBuff, "追光跑道游戏现在开始！");
	//XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
	XFS5152CE_Play(XFS5152CE_WAIT_PLAY);
	do
	{ 
		OSTimeDlyHMSM(0, 0,0,10);
		p_msg = (UartProtocl *)OSQAccept(GAME_RX_Q, &err);
		
	}while ((p_msg->command != MIDI_STOP) || (p_msg->addr != RamSetParameters.GameStartDeviceAddr));

	T1000_POWER_ON();
	OSTimeDlyHMSM(0, 0,4,0);
	T1000_POWER_OFF();

	HAL_RTC_GetTime(&RtcHandle,&GameStartTime,RTC_FORMAT_BIN);
	OSTimeDlyHMSM(0, 0,(RamSetParameters.GameWaitDelayTime),0);
	
/*
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
			(OS_FLAGS) GAME_FAILURE_FLAG,
			(INT8U) OS_FLAG_CLR,
			(INT8U  *)&err);

	Timr2=OSTmrCreate (0,
					((RamSetParameters.GameFailDelayTime - RamSetParameters.GameWaitDelayTime)*1000/10),
					OS_TMR_OPT_ONE_SHOT,
					(OS_TMR_CALLBACK) MyTimr2Callback,
					(void*)0, 
					(INT8U*)"Timr2",
					(INT8U*)&err);
	OSTmrStart ((OS_TMR *)Timr2,(INT8U *)&err);
*/
	GameFailDelayTime = (RamSetParameters.GameFailDelayTime - RamSetParameters.GameWaitDelayTime)*1000;
	err = OSQFlush(GAME_RX_Q);	
	while(1)
	{
		/*
		Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
				(INT8U *)&err);

		if ((Flags & GAME_FAILURE_FLAG) == GAME_FAILURE_FLAG)
		{
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) GAME_FAILURE_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);	
			Game2Failure();	
			return;
		}
*/
		OSTimeDlyHMSM(0, 0,0,GAME_FAIL_PER_TICKS);
		//p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, GAME_FAIL_PER_TICKS, &err));
		p_msg = (UartProtocl *)OSQAccept(GAME_RX_Q, &err);
		if ((p_msg->command==MIDI_PLAY) && (p_msg->addr == RamSetParameters.GameEndDeviceAddr))
		{
			break;		
		}
		else
		{
			GameFailDelayTime = (GameFailDelayTime > GAME_FAIL_PER_TICKS) ? (GameFailDelayTime - GAME_FAIL_PER_TICKS) :0;
			if (GameFailDelayTime == 0)
			{
				Game2Failure();	
				//OSTimeDlyHMSM(0, 0,5,0);
				return;
			}
		}
	}
	
	/*
	do
	{ 
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, 0, &err));
		
	}while ((p_msg->command != MIDI_PLAY) || (p_msg->addr != RamSetParameters.GameEndDeviceAddr));
	*/
	
	//OSTimeDlyHMSM(0, 0,2,0);
	
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

	if ((GameUseTimeAccReg >= RamSetParameters.GameGoodUseTimeStart) && (GameUseTimeAccReg <= RamSetParameters.GameGoodUseTimeEnd))
	{
		my_strcpy(my_strchr(VoiceBuff,'\0'), "，像豹子一样的速度，完美！");
	}
	else 
	//if ((GameUseTimeAccReg >= RamSetParameters.GameBadUseTimeStart) && (GameUseTimeAccReg <= RamSetParameters.GameBadUseTimeEnd))
	{
		my_strcpy(my_strchr(VoiceBuff,'\0'), "，龟速前行，需要锻炼身体哟！");
	}
	
	my_strcpy(my_strchr(VoiceBuff,'\0'), "，请踩下开始键继续挑战");
	//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff,XFS5152CE_WAIT_PLAY);	

	XFS5152CE_Play(XFS5152CE_WAIT_PLAY);

	GameRankingDataSave();	

	//OSTimeDlyHMSM(0, 0,15,0);

}





void Game2Failure(void)
{
	INT8U err;
	
	my_strcpy(VoiceBuff, "游戏失败，请踩下开始键重新开始！");
	//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff,XFS5152CE_WAIT_PLAY);		

	XFS5152CE_Play(XFS5152CE_WAIT_PLAY);
	
	//OSSemPost(XFS5152CE_SemSignal);
	//OSTimeDlyHMSM(0,0,1,0);
	//GameEnd();
	//OSTimeDlyHMSM(0, 0,2,0);
	/*
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) START_KEY_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);
	
	TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,
				(U8)(GROUP1_W_LED_BIT_OFF|
				GROUP1_R_LED_BIT_OFF|
				GROUP1_G_LED_BIT_OFF|
				GROUP1_B_LED_BIT_OFF));

				*/
		
}

void Game2End(void)
{
	INT8U err;	
	

	my_strcpy(VoiceBuff, "游戏结束，请踩下开始键重新开始！");
	XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_WAIT_STOP);
	//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);	
	//OSSemPost(XFS5152CE_SemSignal);
	//OSTimeDlyHMSM(0, 0,2,0);

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

