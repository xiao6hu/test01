#ifndef	__GAME_H_
#define	__GAME_H_

#include "includes.h"




/*
typedef struct{ 
	//RTC_TimeTypeDef GameRankingTime;
	uint8_t Hours;            
	uint8_t Minutes;          
	uint8_t Seconds; 
}GameRankingTypeDef;

#define DEFAULT_GAME_RANKING_PARAMETERS \
{\
	0 ,\
	0 ,\
	0 ,\
}
*/

extern U8 * _CONST_ DeviceIdToEnglishChar[];
extern U8 * _CONST_ DeviceIdToChinesePinyin[];

void game(void);
void game2(void);
/*
int SequenListInsert(SequenListTypeDef *L,GameRankingTypeDef *data,int i);
int SequenListDelete(SequenListTypeDef *L,int i);
int SequenListCompare(SequenListTypeDef *L,GameRankingTypeDef *data);
*/


void GameRankingDataSave(void);
u8 FindRowsDeviceId(u8 RowNumber,u8 Flag);
void GameRankingDataClean(void);
//int SequenListCompare(SequenListTypeDef *L,GameRankingTypeDef *data);
#endif	//__GAME_H_


