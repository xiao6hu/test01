
#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

#include "stm32f2xx.h"
#include "sys.h"
#include "usart1.h"
#include "RTC.h"

#define LED_FRAME_HEADER 	0xfd
#define LED_PLAY_CMD 		0x01
#define LED_ALL_OFF_CMD 	0x02
#define LED_SET_FPS_CMD 	0x03




typedef struct  
{
	u8 red 	;	
	u8 green;	
	u8 blue ;
}LedColourStructTypeDef;


#pragma pack (1)




typedef struct
{
	u16 PatternNumber;	//花样号
	u16 GroupsLedNumber;
	u8 	PlayFps;
	u8 	StopFps;
	u8  CycleFps;
	u8  LedColorCycleMode;
}LedParaGroupsStructTypeDef;	
	

typedef struct
{
	u16 LedGroupsNumber;
	u8  ColorOrder;
	u8 	ChipType;
	//u8 	PlayFps;
	//u8 	StopFps;
	//u8  CycleFps;
	u16 MaxLedNumber;
	//u16 PatternNumber;	//花样号
	//u16 GroupsLedNumber;
	LedColourStructTypeDef LedColour;
	u8 LedColorCycleMode;
	LedParaGroupsStructTypeDef LedParaGroups[256];	
}LedParaStructTypeDef;

typedef struct
{
	u8 	fps;
}FpsSetStructTypeDef;


typedef struct
{
	u8 Status;
	u8 Week;
	TimeTypeDef TimerOnTime;
	TimeTypeDef TimerOffTime;
}TimerStructTypeDef;

#define TIMER_NUMBER  10

typedef struct
{
	u8 LedColorMaxCycleMode;
	u16 LedColorCycleTime;
	u8 Local_OnOff;
	u8 T1000_OnOff;
	TimerStructTypeDef Local_Timer[TIMER_NUMBER];
	TimerStructTypeDef T1000_Timer[TIMER_NUMBER];
}LedControlStructTypeDef;

#pragma pack ()


#define TIMER_OFF 0
#define TIMER_ON 1

extern u8 LocalTimerOnOffFlag;

#define LOCAL_TIMING_ON() LocalTimerOnOffFlag=TIMER_ON

#define LOCAL_TIMING_OFF() LocalTimerOnOffFlag=TIMER_OFF

extern u8 T1000TimerOnOffFlag;

#define T1000_TIMING_ON() T1000TimerOnOffFlag=TIMER_ON	//T1000_POWER_ON()

#define T1000_TIMING_OFF() T1000TimerOnOffFlag=TIMER_OFF //T1000_POWER_OFF()

#define LedParaGroupsStructInit \
{\
/*	u16 PatternNumber;			*/		1,	\
/*	u16 GroupsLedNumber;		*/      20,	\
/*	u8 	PlayFps;				*/		30,	\
/*	u8 	StopFps;				*/		10,	\
/*	u8  CycleFps;				*/      2,	\
/*	u8  LedColorCycleMode;      */		0,	\
}



#define LedParaGroupsInit \
{\
/*00*/	LedParaGroupsStructInit,\
/*01*/	LedParaGroupsStructInit,\
/*02*/	LedParaGroupsStructInit,\
/*03*/	LedParaGroupsStructInit,\
/*04*/	LedParaGroupsStructInit,\
/*05*/	LedParaGroupsStructInit,\
/*06*/	LedParaGroupsStructInit,\
/*07*/	LedParaGroupsStructInit,\
/*08*/	LedParaGroupsStructInit,\
/*09*/	LedParaGroupsStructInit,\
/*0A*/	LedParaGroupsStructInit,\
/*0B*/	LedParaGroupsStructInit,\
/*0C*/	LedParaGroupsStructInit,\
/*0D*/	LedParaGroupsStructInit,\
/*0E*/	LedParaGroupsStructInit,\
/*0F*/	LedParaGroupsStructInit,\
/*10*/	LedParaGroupsStructInit,\
/*11*/	LedParaGroupsStructInit,\
/*12*/	LedParaGroupsStructInit,\
/*13*/	LedParaGroupsStructInit,\
/*14*/	LedParaGroupsStructInit,\
/*15*/	LedParaGroupsStructInit,\
/*16*/	LedParaGroupsStructInit,\
/*17*/	LedParaGroupsStructInit,\
/*18*/	LedParaGroupsStructInit,\
/*19*/	LedParaGroupsStructInit,\
/*1A*/	LedParaGroupsStructInit,\
/*1B*/	LedParaGroupsStructInit,\
/*1C*/	LedParaGroupsStructInit,\
/*1D*/	LedParaGroupsStructInit,\
/*1E*/	LedParaGroupsStructInit,\
/*1F*/	LedParaGroupsStructInit,\
/*20*/	LedParaGroupsStructInit,\
/*21*/	LedParaGroupsStructInit,\
/*22*/	LedParaGroupsStructInit,\
/*23*/	LedParaGroupsStructInit,\
/*24*/	LedParaGroupsStructInit,\
/*25*/	LedParaGroupsStructInit,\
/*26*/	LedParaGroupsStructInit,\
/*27*/	LedParaGroupsStructInit,\
/*28*/	LedParaGroupsStructInit,\
/*29*/	LedParaGroupsStructInit,\
/*2A*/	LedParaGroupsStructInit,\
/*2B*/	LedParaGroupsStructInit,\
/*2C*/	LedParaGroupsStructInit,\
/*2D*/	LedParaGroupsStructInit,\
/*2E*/	LedParaGroupsStructInit,\
/*2F*/	LedParaGroupsStructInit,\
/*30*/	LedParaGroupsStructInit,\
/*31*/	LedParaGroupsStructInit,\
/*32*/	LedParaGroupsStructInit,\
/*33*/	LedParaGroupsStructInit,\
/*34*/	LedParaGroupsStructInit,\
/*35*/	LedParaGroupsStructInit,\
/*36*/	LedParaGroupsStructInit,\
/*37*/	LedParaGroupsStructInit,\
/*38*/	LedParaGroupsStructInit,\
/*39*/	LedParaGroupsStructInit,\
/*3A*/	LedParaGroupsStructInit,\
/*3B*/	LedParaGroupsStructInit,\
/*3C*/	LedParaGroupsStructInit,\
/*3D*/	LedParaGroupsStructInit,\
/*3E*/	LedParaGroupsStructInit,\
/*3F*/	LedParaGroupsStructInit,\
/*40*/	LedParaGroupsStructInit,\
/*41*/	LedParaGroupsStructInit,\
/*42*/	LedParaGroupsStructInit,\
/*43*/	LedParaGroupsStructInit,\
/*44*/	LedParaGroupsStructInit,\
/*45*/	LedParaGroupsStructInit,\
/*46*/	LedParaGroupsStructInit,\
/*47*/	LedParaGroupsStructInit,\
/*48*/	LedParaGroupsStructInit,\
/*49*/	LedParaGroupsStructInit,\
/*4A*/	LedParaGroupsStructInit,\
/*4B*/	LedParaGroupsStructInit,\
/*4C*/	LedParaGroupsStructInit,\
/*4D*/	LedParaGroupsStructInit,\
/*4E*/	LedParaGroupsStructInit,\
/*4F*/	LedParaGroupsStructInit,\
/*50*/	LedParaGroupsStructInit,\
/*51*/	LedParaGroupsStructInit,\
/*52*/	LedParaGroupsStructInit,\
/*53*/	LedParaGroupsStructInit,\
/*54*/	LedParaGroupsStructInit,\
/*55*/	LedParaGroupsStructInit,\
/*56*/	LedParaGroupsStructInit,\
/*57*/	LedParaGroupsStructInit,\
/*58*/	LedParaGroupsStructInit,\
/*59*/	LedParaGroupsStructInit,\
/*5A*/	LedParaGroupsStructInit,\
/*5B*/	LedParaGroupsStructInit,\
/*5C*/	LedParaGroupsStructInit,\
/*5D*/	LedParaGroupsStructInit,\
/*5E*/	LedParaGroupsStructInit,\
/*5F*/	LedParaGroupsStructInit,\
/*60*/	LedParaGroupsStructInit,\
/*61*/	LedParaGroupsStructInit,\
/*62*/	LedParaGroupsStructInit,\
/*63*/	LedParaGroupsStructInit,\
/*64*/	LedParaGroupsStructInit,\
/*65*/	LedParaGroupsStructInit,\
/*66*/	LedParaGroupsStructInit,\
/*67*/	LedParaGroupsStructInit,\
/*68*/	LedParaGroupsStructInit,\
/*69*/	LedParaGroupsStructInit,\
/*6A*/	LedParaGroupsStructInit,\
/*6B*/	LedParaGroupsStructInit,\
/*6C*/	LedParaGroupsStructInit,\
/*6D*/	LedParaGroupsStructInit,\
/*6E*/	LedParaGroupsStructInit,\
/*6F*/	LedParaGroupsStructInit,\
/*70*/	LedParaGroupsStructInit,\
/*71*/	LedParaGroupsStructInit,\
/*72*/	LedParaGroupsStructInit,\
/*73*/	LedParaGroupsStructInit,\
/*74*/	LedParaGroupsStructInit,\
/*75*/	LedParaGroupsStructInit,\
/*76*/	LedParaGroupsStructInit,\
/*77*/	LedParaGroupsStructInit,\
/*78*/	LedParaGroupsStructInit,\
/*79*/	LedParaGroupsStructInit,\
/*7A*/	LedParaGroupsStructInit,\
/*7B*/	LedParaGroupsStructInit,\
/*7C*/	LedParaGroupsStructInit,\
/*7D*/	LedParaGroupsStructInit,\
/*7E*/	LedParaGroupsStructInit,\
/*7F*/	LedParaGroupsStructInit,\
/*80*/	LedParaGroupsStructInit,\
/*81*/	LedParaGroupsStructInit,\
/*82*/	LedParaGroupsStructInit,\
/*83*/	LedParaGroupsStructInit,\
/*84*/	LedParaGroupsStructInit,\
/*85*/	LedParaGroupsStructInit,\
/*86*/	LedParaGroupsStructInit,\
/*87*/	LedParaGroupsStructInit,\
/*88*/	LedParaGroupsStructInit,\
/*89*/	LedParaGroupsStructInit,\
/*8A*/	LedParaGroupsStructInit,\
/*8B*/	LedParaGroupsStructInit,\
/*8C*/	LedParaGroupsStructInit,\
/*8D*/	LedParaGroupsStructInit,\
/*8E*/	LedParaGroupsStructInit,\
/*8F*/	LedParaGroupsStructInit,\
/*90*/	LedParaGroupsStructInit,\
/*91*/	LedParaGroupsStructInit,\
/*92*/	LedParaGroupsStructInit,\
/*93*/	LedParaGroupsStructInit,\
/*94*/	LedParaGroupsStructInit,\
/*95*/	LedParaGroupsStructInit,\
/*96*/	LedParaGroupsStructInit,\
/*97*/	LedParaGroupsStructInit,\
/*98*/	LedParaGroupsStructInit,\
/*99*/	LedParaGroupsStructInit,\
/*9A*/	LedParaGroupsStructInit,\
/*9B*/	LedParaGroupsStructInit,\
/*9C*/	LedParaGroupsStructInit,\
/*9D*/	LedParaGroupsStructInit,\
/*9E*/	LedParaGroupsStructInit,\
/*9F*/	LedParaGroupsStructInit,\
/*A0*/	LedParaGroupsStructInit,\
/*A1*/	LedParaGroupsStructInit,\
/*A2*/	LedParaGroupsStructInit,\
/*A3*/	LedParaGroupsStructInit,\
/*A4*/	LedParaGroupsStructInit,\
/*A5*/	LedParaGroupsStructInit,\
/*A6*/	LedParaGroupsStructInit,\
/*A7*/	LedParaGroupsStructInit,\
/*A8*/	LedParaGroupsStructInit,\
/*A9*/	LedParaGroupsStructInit,\
/*AA*/	LedParaGroupsStructInit,\
/*AB*/	LedParaGroupsStructInit,\
/*AC*/	LedParaGroupsStructInit,\
/*AD*/	LedParaGroupsStructInit,\
/*AE*/	LedParaGroupsStructInit,\
/*AF*/	LedParaGroupsStructInit,\
/*B0*/	LedParaGroupsStructInit,\
/*B1*/	LedParaGroupsStructInit,\
/*B2*/	LedParaGroupsStructInit,\
/*B3*/	LedParaGroupsStructInit,\
/*B4*/	LedParaGroupsStructInit,\
/*B5*/	LedParaGroupsStructInit,\
/*B6*/	LedParaGroupsStructInit,\
/*B7*/	LedParaGroupsStructInit,\
/*B8*/	LedParaGroupsStructInit,\
/*B9*/	LedParaGroupsStructInit,\
/*BA*/	LedParaGroupsStructInit,\
/*BB*/	LedParaGroupsStructInit,\
/*BC*/	LedParaGroupsStructInit,\
/*BD*/	LedParaGroupsStructInit,\
/*BE*/	LedParaGroupsStructInit,\
/*BF*/	LedParaGroupsStructInit,\
/*C0*/	LedParaGroupsStructInit,\
/*C1*/	LedParaGroupsStructInit,\
/*C2*/	LedParaGroupsStructInit,\
/*C3*/	LedParaGroupsStructInit,\
/*C4*/	LedParaGroupsStructInit,\
/*C5*/	LedParaGroupsStructInit,\
/*C6*/	LedParaGroupsStructInit,\
/*C7*/	LedParaGroupsStructInit,\
/*C8*/	LedParaGroupsStructInit,\
/*C9*/	LedParaGroupsStructInit,\
/*CA*/	LedParaGroupsStructInit,\
/*CB*/	LedParaGroupsStructInit,\
/*CC*/	LedParaGroupsStructInit,\
/*CD*/	LedParaGroupsStructInit,\
/*CE*/	LedParaGroupsStructInit,\
/*CF*/	LedParaGroupsStructInit,\
/*D0*/	LedParaGroupsStructInit,\
/*D1*/	LedParaGroupsStructInit,\
/*D2*/	LedParaGroupsStructInit,\
/*D3*/	LedParaGroupsStructInit,\
/*D4*/	LedParaGroupsStructInit,\
/*D5*/	LedParaGroupsStructInit,\
/*D6*/	LedParaGroupsStructInit,\
/*D7*/	LedParaGroupsStructInit,\
/*D8*/	LedParaGroupsStructInit,\
/*D9*/	LedParaGroupsStructInit,\
/*DA*/	LedParaGroupsStructInit,\
/*DB*/	LedParaGroupsStructInit,\
/*DC*/	LedParaGroupsStructInit,\
/*DD*/	LedParaGroupsStructInit,\
/*DE*/	LedParaGroupsStructInit,\
/*DF*/	LedParaGroupsStructInit,\
/*E0*/	LedParaGroupsStructInit,\
/*E1*/	LedParaGroupsStructInit,\
/*E2*/	LedParaGroupsStructInit,\
/*E3*/	LedParaGroupsStructInit,\
/*E4*/	LedParaGroupsStructInit,\
/*E5*/	LedParaGroupsStructInit,\
/*E6*/	LedParaGroupsStructInit,\
/*E7*/	LedParaGroupsStructInit,\
/*E8*/	LedParaGroupsStructInit,\
/*E9*/	LedParaGroupsStructInit,\
/*EA*/	LedParaGroupsStructInit,\
/*EB*/	LedParaGroupsStructInit,\
/*EC*/	LedParaGroupsStructInit,\
/*ED*/	LedParaGroupsStructInit,\
/*EE*/	LedParaGroupsStructInit,\
/*EF*/	LedParaGroupsStructInit,\
/*F0*/	LedParaGroupsStructInit,\
/*F1*/	LedParaGroupsStructInit,\
/*F2*/	LedParaGroupsStructInit,\
/*F3*/	LedParaGroupsStructInit,\
/*F4*/	LedParaGroupsStructInit,\
/*F5*/	LedParaGroupsStructInit,\
/*F6*/	LedParaGroupsStructInit,\
/*F7*/	LedParaGroupsStructInit,\
/*F8*/	LedParaGroupsStructInit,\
/*F9*/	LedParaGroupsStructInit,\
/*FA*/	LedParaGroupsStructInit,\
/*FB*/	LedParaGroupsStructInit,\
/*FC*/	LedParaGroupsStructInit,\
/*FD*/	LedParaGroupsStructInit,\
/*FE*/	LedParaGroupsStructInit,\
/*FF*/	LedParaGroupsStructInit,\
}

#define LedControlStructInit \
{\
	3,\
	60,\
	0,\
	0,\
	{\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
	},\
	{\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
		{0,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},\
	},\
}


extern LedParaStructTypeDef LedParaData;
extern FpsSetStructTypeDef FpsSetData;



#if 0
extern u8 LedData[32];
#define LED_FILE_INIT \
{\
/*u8 	DeviceAddr;						*/	0x01,\
/*u16 	DataLen;						*/	0x0000,\
/*u8 	Command;                        */	0x00,\
/*u8 	*data;                          */	(void *),\
}
#endif

void TxLedControlCmd(u16 length,u8 addr, u8 command, u8 *data);
void TxPlayLedControl(UartProtocl *pPacket);
void TxStopLedControl(UartProtocl *pPacket);
void TxStudyCodeCommands(u8 ControlAddr);

void LedColorCycleControl(void);

void LedTimerControl(void);

#endif

