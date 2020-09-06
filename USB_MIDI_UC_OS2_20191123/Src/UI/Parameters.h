
#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include "includes.h"

#include "LedControl.h"

#define FlashSetParametersStartAddress 	(0x8000000+0x20000)-0x800
#define EepromSetParametersStartAddress  0x02


#define FLASH_READ(ADDR, BUFF, LEN) STMFLASH_Read((u32)&ADDR, (u16 *)&BUFF, (sizeof(LEN) / sizeof(u16)))
//#define FLASH_READ(ADDR, BUFF, LEN) SWMFLASH_Read(ADDR, (u32 *)&BUFF, (sizeof(LEN) / sizeof(u32)))

#define FLASH_WRITE(ADDR, BUFF, LEN) STMFLASH_Write((u32)&ADDR, (u16 *)&BUFF, (sizeof(LEN) / sizeof(u16)))
//#define FLASH_WRITE(ADDR, BUFF, LEN) SWMFLASH_Write(ADDR, (u32 *)&BUFF, (sizeof(LEN) / sizeof(u32)))


#define EEPROM_WRITE(ADDR, BUFF, LEN) 	 
#define EEPROM_READ (ADDR, BUFF, LEN)

/*
AT24CXX_Write(EepromSetParametersStartAddress+(U32)&BUFF-(U32)&ADDR,//待写入地址
              (uint8_t *)&BUFF,//待写入数据缓冲区
              (sizeof(BUFF) / sizeof(uint8_t))); //待写入数据长度

              
AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.MusicDelayTime-(U32)&RamSetParameters,//待写入地址
              (uint8_t *)&RamSetParameters.MusicDelayTime,//待写入数据缓冲区
              (sizeof(RamSetParameters.MusicDelayTime) / sizeof(uint8_t))); //待写入数据长度

*/

#define ADULT_MODE 0
#define BABY_MODE  1

#define ENGLISH_CHAR_MODE 0
#define CHINESE_PIN_YIN_MODE  1



typedef struct{ 
    char TextNumber[2];       //语音文本编号
	char Textlength[4];		 //语音文本长度
	char *TextData;		 //语音文本数据指针
}VoiceTextTypeDef;

extern char* GameVoice[10];


typedef struct
{
  U16 MusicStatus     : 1;                          //0
  U16 LightsStatus    : 1;                          //1
  U16 Reserved        : 14;                          //2     
 
}DeviceStatusTypeDef;



#define MAX_ROWS_DEVICE_NUMBER   10
#define MAX_ROW_NUMBER   		 255
/*
typedef struct
{
	U8 RowNumber; 
	U8 RowsDeviceNumber;   
}RowTypeDef;
*/

#define DEFAULT_ROWS_DEVICE_NUMBER_PARAMETERS \
{\
/*0*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*1*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*2*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*3*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*4*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*5*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*6*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*7*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*8*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*9*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*A*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*B*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*C*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*D*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*E*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
/*F*/	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\
}
/*
typedef struct
{
	U8 DeviceId;                           
	char Char;                        
}DeviceIdToCharTypeDef;
*/

#define DEFAULT_DEVICE_ID_TO_ENGLISH_CHAR_PARAMETERS \
{\
/*0*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*1*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*2*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*3*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*4*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*5*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*6*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*7*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*8*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*9*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*A*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*B*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*C*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*D*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*E*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*F*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
}

#define DEFAULT_DEVICE_ID_TO_CHINESE_PIN_YIN_PARAMETERS \
{\
/*0*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*1*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*2*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*3*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*4*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*5*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*6*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*7*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*8*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*9*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*A*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*B*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*C*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*D*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*E*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
/*F*/	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
}


typedef struct{ 
	uint8_t Minutes;          
	uint8_t Seconds; 
}GameRankingTypeDef;


#define MAX_GAME_RANKING_NUMBER  50
#define DEFAULT_GAME_RANKING_PARAMETERS \
{\
/*0*/	{59,59},\
/*1*/	{59,59},\
/*2*/	{59,59},\
/*3*/	{59,59},\
/*4*/	{59,59},\
/*5*/	{59,59},\
/*6*/	{59,59},\
/*7*/	{59,59},\
/*8*/	{59,59},\
/*9*/	{59,59},\
/*10*/	{59,59},\
/*11*/	{59,59},\
/*12*/	{59,59},\
/*13*/	{59,59},\
/*14*/	{59,59},\
/*15*/	{59,59},\
/*16*/	{59,59},\
/*17*/	{59,59},\
/*18*/	{59,59},\
/*19*/	{59,59},\
/*20*/	{59,59},\
/*21*/	{59,59},\
/*22*/	{59,59},\
/*23*/	{59,59},\
/*24*/	{59,59},\
/*25*/	{59,59},\
/*26*/	{59,59},\
/*27*/	{59,59},\
/*28*/	{59,59},\
/*29*/	{59,59},\
/*30*/	{59,59},\
/*31*/	{59,59},\
/*32*/	{59,59},\
/*33*/	{59,59},\
/*34*/	{59,59},\
/*35*/	{59,59},\
/*36*/	{59,59},\
/*37*/	{59,59},\
/*38*/	{59,59},\
/*39*/	{59,59},\
/*40*/	{59,59},\
/*41*/	{59,59},\
/*42*/	{59,59},\
/*43*/	{59,59},\
/*44*/	{59,59},\
/*45*/	{59,59},\
/*46*/	{59,59},\
/*47*/	{59,59},\
/*48*/	{59,59},\
/*49*/	{59,59},\
/*50*/	{59,59},\
}

typedef struct{ 
	GameRankingTypeDef *pdata;          
	int *last;
}SequenListTypeDef;



typedef struct{ 
	uint8_t CAN_Device1;          
	uint8_t CAN_Device2; 
}CAN_DeviceTotalTypeDef;

#define CAN_MAX_DEVICE_NUMBER 125

#define DEFAULT_CAN_DEVICE_TOTA_PARAMETERS \
{\
/*CAN1*/	CAN_MAX_DEVICE_NUMBER,\
/*CAN2*/	CAN_MAX_DEVICE_NUMBER,\
}


typedef struct{ 
    DeviceStatusTypeDef DeviceStatus;       //0  16bit
	U16 Language;			                //1  0:简体中文 1：英文
	U16 DeviceTotal;   	                    //2  设备总数：0-255
    U32 MusicDelayTime;                     //3  音乐延时时间：0-99999
    U32 LightsDelayTime;                    //4  灯光延时时间：0-99999
    U32 TrampleCountTime;                   //5  踩踏计数时间：0-9999
    U16 TrampleDelayTime;                   //6  踩踏延时时间：0-9999
    U16 TrampleNumber;                      //7  同时踩踏人数：0-255
    U16 SyllableDelayTime;	                //8  音节延时时间：0-9999
    U16 NoteDelayTime;                      //9  音符延时时间：0-9999   
	U16 LightsMode;                      	//10  灯带模式设置  0：少 1：多
	U16 LightsSpeed;                     	//11  灯带速度设置  1、2、3  
	U8 LightsSpacing;                 		//12  灯带间隔设置  0-9
	U8 GameModeSelect;                      //13  游戏模式选择  0：成人模式 1：儿童模式
	U8 SayModeSelect;						//14  发音模式选择  0：英文字母 1：汉语拼音
	U8 MaxRowNumber;
	U8 RowsDeviceNumber[MAX_ROW_NUMBER+1];
	U8 DeviceIdToEnglishChar[MAX_ROW_NUMBER+1];					
	U8 DeviceIdToChinesePinyin[MAX_ROW_NUMBER+1];	
	U8 BeepRowNum;//
	U8 GamePassTime;//单位:S
	U32 VoicePromptTime;//单位:S
	int GameRankingNumber;
	GameRankingTypeDef GameRanking[MAX_GAME_RANKING_NUMBER+1];
	U8 GameSelect;
	CAN_DeviceTotalTypeDef CAN_DeviceTotal;
	
	U16 ReturnTopTime;

	LedParaStructTypeDef LedPara;
	
	LedControlStructTypeDef LedControl;

	u8 GameStartDeviceAddr;
	u8 GameEndDeviceAddr;
	u8 GameFailDelayTime;
	u8 GameWaitDelayTime;	

	u8 GameGoodUseTimeStart;
	u8 GameGoodUseTimeEnd;
	u8 GameBadUseTimeStart;
	u8 GameBadUseTimeEnd;

}ParametersStructTypeDef;   


#define	DEFAULT_PARAMETERS	{                                                           			 \
/*DeviceStatusTypeDef DeviceStatus;         //0  16bit                      */  		{0x0000},\
/*U16  Language;			                //1  0:简体中文 1：英文    		    */  	 0,		\
/*U16  DeviceTotal;   	                    //2  设备总数：0-255           	    */  	 0,		\
/*U32 MusicDelayTime;                       //3  音乐延时时间：0-99999秒    	        */  10,	    \
/*U32 LightsDelayTime;                      //4  灯光延时时间：0-99999秒      	    */  10,		\
/*U16 TrampleCountTime;                     //5  踩踏计数时间：0-9999毫秒           */ 	2000,         \
/*U16 TrampleDelayTime;                     //6  踩踏延时时间：0-9999秒           */  	0,	\
/*U16 TrampleNumber;                        //7  同时踩踏人数：0-255个          	*/  	5,	    \
/*U16 SyllableDelayTime;	                //8  音节延时时间：0-9999毫秒            */  0,   \
/*U16 NoteDelayTime;                        //9  音符延时时间：0-9999毫秒            */  0,   \
/*U16 LightsMode;	                		//10  灯带模式设置            */  			4,   \
/*U16 LightsSpeed;                        	//11  灯带速度设置            */  			0,   \
/*U8 LightsSpacing;                   		//12  灯带间隔设置  0-9    */  				5, \
/*U8 GameModeSelect;                      	//13  游戏模式选择  0：游戏模式 1：儿童模式*/ 0,\
/*U8 SayModeSelect;							//14  发音模式选择  0：英文字母 1：汉语拼音*/	0,\
/*U8 MaxRowNumber;*/																	MAX_ROW_NUMBER,\
/*U8 RowsDeviceNumber[MAX_ROWS_DEVICE_NUMBER+1];*/ 										DEFAULT_ROWS_DEVICE_NUMBER_PARAMETERS,\
/*U8 DeviceIdToEnglishChar[MAX_ROWS_DEVICE_NUMBER+1];*/									DEFAULT_DEVICE_ID_TO_ENGLISH_CHAR_PARAMETERS,\
/*U8 DeviceIdToChinesePinyin[MAX_ROWS_DEVICE_NUMBER+1];*/								DEFAULT_DEVICE_ID_TO_CHINESE_PIN_YIN_PARAMETERS,\
/*U8 SetBeepRowNum;*/																	255,\
/*U8 GamePassTime;*/																	5,\
/*U32 VoicePromptTime;*/																300,\
/*int GameRankingNumber;*/																1,\
/*GameRankingTypeDef GameRanking[MAX_GAME_RANKING_NUMBER+1];*/							DEFAULT_GAME_RANKING_PARAMETERS,\
/*U8 GameSelect;*/																		1,\
/*CAN_DeviceTotalTypeDef CAN_DeviceTotal;*/												DEFAULT_CAN_DEVICE_TOTA_PARAMETERS,\
/*U16 ReturnTopTime;*/																	60, \
/*LedParaStructTypeDef LedPara;*/														{20, 4, 0, 1024, {255, 0, 0},0,LedParaGroupsInit},\
/*LedControlStructTypeDef LedControl;*/                                                 LedControlStructInit,\
/*u8 GameStartDeviceAddr;*/																1,\
/*u8 GameEndDeviceAddr;*/																10,\
/*u8 GameFailDelayTime;*/																30,\
/*u8 GameWaitDelayTime;*/																15,\
/*u8 GameGoodUseTimeStart;*/															15,\
/*u8 GameGoodUseTimeEnd;*/																20,\
/*u8 GameBadUseTimeStart;*/																20,\
/*u8 GameBadUseTimeEnd;*/																99,\
}											  




extern _CONST_ ParametersStructTypeDef DefaultParameters;
extern _CONST_ ParametersStructTypeDef FlashSetParameters;
extern ParametersStructTypeDef RamSetParameters;


extern SequenListTypeDef L;

void LodeParametersData(void);
void SaveParametersData(void);
void ResetParametersData(void);	



int SequenListInsert(SequenListTypeDef *L,GameRankingTypeDef *data,int i);
int SequenListDelete(SequenListTypeDef *L,int i);
int SequenListCompare(SequenListTypeDef *L,GameRankingTypeDef *data);
u8 GameRankingCmp(GameRankingTypeDef *GameRanking);

void SlaveParaSet(void);

void ParaUpdate(void);

u8 Get_ExtMemoryData(void);








#endif

