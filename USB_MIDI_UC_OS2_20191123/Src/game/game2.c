/****************************游戏追光跑道**************************************/
#include "includes.h"

//extern void MyTimr2Callback (OS_TMR *ptmr, void *p_arg);


void game2(void)
{
	RTC_TimeTypeDef GameStartTime;
	RTC_TimeTypeDef GameStopTime;
	GameRankingTypeDef GameUseTime;

	INT8U err;	
	UartProtocl *p_msg;	

	err = OSQFlush(GAME_RX_Q);
	do
	{ 
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, 0, &err));
		
	}while ((p_msg->command != MIDI_PLAY) || (p_msg->addr != RamSetParameters.GameStartDeviceAddr));

	T1000_POWER_OFF();

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

	my_strcpy(VoiceBuff, "追光跑道现在开始");
	XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);

	HAL_RTC_GetTime(&RtcHandle,&GameStartTime,RTC_FORMAT_BIN);

	p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, (RamSetParameters.GameWaitDelayTime+1)*1000, &err));
		
	if ((p_msg->command==MIDI_STOP) || (p_msg->addr != RamSetParameters.GameStartDeviceAddr))
	{
		T1000_POWER_ON();
	}

	//err = OSQFlush(GAME_RX_Q);
	while(1)
	{
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, ((RamSetParameters.GameFailDelayTime - RamSetParameters.GameWaitDelayTime)+1)*1000, &err));
		
		if (err== OS_ERR_TIMEOUT)
		{
			GameEnd();	
			return;
		}
		else if ((p_msg->command==MIDI_PLAY) || (p_msg->addr != RamSetParameters.GameEndDeviceAddr))
		{
			break;		
		}
	}
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
	GameRankingDataSave();			
}


