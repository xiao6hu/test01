/****************************��Ϸ������**************************************/

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
		case 1: game2();break;
		case 2: break;
		case 3:	break;
		case 4: break;
		default:break;	
	}
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
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.GameRanking,//��д�����ݻ�����
				(sizeof(RamSetParameters.GameRanking) / sizeof(uint8_t))); //��д�����ݳ���
	
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRankingNumber-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.GameRankingNumber,//��д�����ݻ�����
				(sizeof(RamSetParameters.GameRankingNumber) / sizeof(uint8_t))); //��д�����ݳ���
	
	
	
	
	AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.GameRanking,//��д�����ݻ�����
				(sizeof(RamSetParameters.GameRanking) / sizeof(uint8_t))); //��д�����ݳ���
	
	
	
	
	
	#endif
}


void GameRankingDataClean(void)
{
	#ifdef INT_FLASH_SAVE
	FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
	FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
	#endif

	
	#ifdef EEPROM_SAVE
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&DefaultParameters.GameRanking,//��д�����ݻ�����
				(sizeof(DefaultParameters.GameRanking) / sizeof(uint8_t))); //��д�����ݳ���
	
	AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRankingNumber-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&DefaultParameters.GameRankingNumber,//��д�����ݻ�����
				(sizeof(DefaultParameters.GameRankingNumber) / sizeof(uint8_t))); //��д�����ݳ���
	
	AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameRanking-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&DefaultParameters.GameRanking,//��д�����ݻ�����
				(sizeof(DefaultParameters.GameRanking) / sizeof(uint8_t))); //��д�����ݳ���
	
	#endif
	
	
	LodeParametersData();
	

}

//��ڲ���1���ź�
//��ڲ���2���ŷ����豸��ַ��־ 0����С���� 1���Ӵ�С 
//���ڲ�����ָ���źŵ��豸��ַ
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



