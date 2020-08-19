/****************************游戏跳房子**************************************/

#include "stm32f2xx.h"
#include "my_config.h"
#include "sys.h"
#include "Parameters.h"
#include "game.h"


unsigned char GameType=0;
void game(void)
{
	switch (GameType)
	{
		case 0: game1();break;
		case 1: break;
		case 2: break;
		case 3:	break;
		case 4: break;
		default:break;	
	}
}

void GameFailure(void)
{
	INT8U err;
	
	my_strcpy(VoiceBuff, "游戏失败，清按开关重新开始！");
	//XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);		

	XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_WAIT_STOP);
	
	//OSSemPost(XFS5152CE_SemSignal);
	//OSTimeDlyHMSM(0,0,1,0);
	//GameEnd();
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

void GameEnd(void)
{
	INT8U err;	
	

	my_strcpy(VoiceBuff, "游戏结束，清按开关重新开始！");
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













int SequenListInsert(SequenListTypeDef *L,GameRankingTypeDef *data,int i)
{
	int j;

	if (*(L->last)>= MAX_GAME_RANKING_NUMBER)
	{
		return 0;
	}
	else if(i<1 || i>*(L->last)+2)
	{
		return 0;
	}
	else
	{
		for( j=*(L->last);j>=i-1;j--)
		{
			L->pdata[j+1]=L->pdata[j];
		}	
		L->pdata[i-1]=*data;
		*(L->last)=*(L->last)+1;
	}
	return 1;
}



int SequenListDelete(SequenListTypeDef *L,int i)
{
	int j;
	if(i<1|| i>*(L->last)+1)
	{
		return 0;
	}
	else
	{
		for( j=i;j<=*(L->last);j++)
		{
			L->pdata[j-1]=L->pdata[j];
		}	
		*(L->last)=*(L->last)-1;
	}
	return 1;
}




int SequenListCompare(SequenListTypeDef *L,GameRankingTypeDef *data)
{
	int j;
	for( j=0;j<=*(L->last);j++)
	{
		
		if (((U32)data->Minutes * 60uL + (U32)data->Seconds) ==((U32) L->pdata[j].Minutes * 60uL + (U32) L->pdata[j].Seconds))
		{
			break;
		}
		if (((U32)data->Minutes * 60uL + (U32)data->Seconds) <((U32) L->pdata[j].Minutes * 60uL + (U32) L->pdata[j].Seconds))
		{


	 		if (SequenListInsert(L,data,j+1)==0)
			{
				SequenListDelete(L,*(L->last)+1);
				SequenListInsert(L,data,j+1);	
			}
			break;

		}	
	}	
	return j;
}




u8 GameRankingCmp(GameRankingTypeDef *GameUseTime)
{
	u8 i;
	i=SequenListCompare(&L,GameUseTime);
	return i+1;
}


void GameRankingDataSave(void)
{

	#ifdef INT_FLASH_SAVE
	FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
	FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
	#endif

	#ifdef EEPROM_SAVE
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&RamSetParameters.GameRanking,//待写入数据缓冲区
				(sizeof(RamSetParameters.GameRanking) / sizeof(uint8_t))); //待写入数据长度
	
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRankingNumber-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&RamSetParameters.GameRankingNumber,//待写入数据缓冲区
				(sizeof(RamSetParameters.GameRankingNumber) / sizeof(uint8_t))); //待写入数据长度
	
	
	
	
	AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&RamSetParameters.GameRanking,//待写入数据缓冲区
				(sizeof(RamSetParameters.GameRanking) / sizeof(uint8_t))); //待写入数据长度
	
	
	
	
	
	#endif
}


void GameRankingDataClean(void)
{
	#ifdef INT_FLASH_SAVE
	FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
	FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
	#endif

	
	#ifdef EEPROM_SAVE
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&DefaultParameters.GameRanking,//待写入数据缓冲区
				(sizeof(DefaultParameters.GameRanking) / sizeof(uint8_t))); //待写入数据长度
	
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRankingNumber-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&DefaultParameters.GameRankingNumber,//待写入数据缓冲区
				(sizeof(DefaultParameters.GameRankingNumber) / sizeof(uint8_t))); //待写入数据长度
	
	AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//待写入地址
				(uint8_t *)&DefaultParameters.GameRanking,//待写入数据缓冲区
				(sizeof(DefaultParameters.GameRanking) / sizeof(uint8_t))); //待写入数据长度
	
	#endif
	
	
	LodeParametersData();
	

}

//入口参数1：排号
//入口参数2：排返回设备地址标志 0：从小到大 1：从大到小 
//出口参数：指定排号低设备地址
u8 FindRowsDeviceId(u8 RowNumber,u8 Flag)
{		
	u16 cnt;
	u8 RowsDeviceId=0;
	
	if (Flag==0)
	{
		RowsDeviceId=1;
		
		for (cnt=1;cnt<=RowNumber;cnt++)
		{
			RowsDeviceId += RamSetParameters.RowsDeviceNumber[cnt];
		}
	}
	else
	{
		RowsDeviceId=RamSetParameters.DeviceTotal;
		
		for (cnt=RamSetParameters.MaxRowNumber;cnt>=RowNumber;cnt--)
		{
			RowsDeviceId -= RamSetParameters.RowsDeviceNumber[cnt];
		}
	}
	
	return RowsDeviceId;
}



