#ifndef	_APP_H_
#define	_APP_H_
#include "includes.h"
#include "can.h"
#include "usart1.h"
/**************** 用户任务声明 *******************/

void Task_Start(void *p_arg);
//void Task_LED2(void *p_arg);
//void Task_LED3(void *p_arg);

void Task_MidiPlay(void *p_arg);
void Task_Game(void *p_arg);

void Task_Display(void *p_arg);
void Task_AutoLighits(void *p_arg);

void Task_Scan(void *p_arg);

void Task_UartRx(void *p_arg);
void Task_Key(void *p_arg);


void Task_GameVoicePrompt(void *p_arg);

void Task_XFS5152CE_Play(void *p_arg);


void Task_LedControl(void *p_arg);

void Task_Mco_Tx (void *p_arg);
void Task_Mco_Rx (void *p_arg);


/*--------设置队列参量--------*/

#define		DEVICE_MESSAGES		10u	
//定义消息队列长度			
extern void		*DeviceMsgGrp[DEVICE_MESSAGES];								//定义消息指针数组					
extern OS_EVENT	*Device_Q;	
extern u8 DeviceMsg[DEVICE_MESSAGES];


#define		KEY_MESSAGES		10u	
//定义消息队列长度			
extern void		*KeyMsgGrp[KEY_MESSAGES];								//定义消息指针数组					
extern OS_EVENT	*Key_Q;											//定义事件控制块
extern u8 KeyMsg[KEY_MESSAGES];


#define		XFS5152CE_RX_MESSAGES		10u	
//定义消息队列长度			
extern void		*XFS5152CE_RX_MsgGrp[XFS5152CE_RX_MESSAGES];								//定义消息指针数组					
extern OS_EVENT	*XFS5152CE_RX_Q;											//定义事件控制块
extern u8 XFS5152CE_RX_Msg[XFS5152CE_RX_MESSAGES];
void Post_XFS5152CE_RX_Msg(u8 msg);



#define CAN_IRQ_RX_MESSAGES  10u
//定义消息队列长度			
extern void		*CAN_IRQ_RX_MsgGrp[CAN_IRQ_RX_MESSAGES];								//定义消息指针数组					
extern OS_EVENT	*CAN_IRQ_RX_Q;											//定义事件控制块

extern Message CAN_IRQ_RX_Msg[CAN_IRQ_RX_MESSAGES];
void Post_CAN_IRQ_RX_Msg(Message msg);



#define GAME_RX_MESSAGES  10u
//定义消息队列长度			
extern void		*GAME_RX_MsgGrp[GAME_RX_MESSAGES];								//定义消息指针数组					
extern OS_EVENT	*GAME_RX_Q;											//定义事件控制块
extern UartProtocl GAME_RX_Msg[GAME_RX_MESSAGES];
void Post_GAME_RX_Msg(UartProtocl msg);


extern u8 OnlineDevice;
extern u8 ActivityDevice;

extern OS_FLAGS  MIDI_Flag;
extern OS_FLAG_GRP *pFlagGrpMidi;


extern OS_TMR  *Timr1;
extern OS_TMR  *Timr2;

extern OS_EVENT *XFS5152CE_SemSignal;
extern OS_EVENT * GameModePlay_sem;
extern OS_EVENT *  XFS5152CE_PlayEnd_Sem;

#define MIDI_PLAY_FLAG  BIT(0)
#define MIDI_STOP_FLAG  BIT(1)
#define MIDI_AUTO_PLAY_FLAG  BIT(2)
#define MIDI_DELAY_FLAG BIT(3)

#define AUTO_LIGHTS_EN_DIS_FLAG BIT(4)
#define MIDI_PLAY_EN_DIS_FLAG BIT(5)


#define  U_DISK_ONLINE_FLAG BIT(6)


#define  SD_ONLINE_FLAG BIT(7)

#define AUTO_LIGHT_FLAG  BIT(8) 

#define UART1_RX_OK_FLAG  BIT(9) 
#define UART1_TX_OK_FLAG  BIT(10) 

#define DEVICE_CHANGE_FLAG  BIT(11)

#define MIDI_STOP_PLAY_FLAG BIT(12)

#define  UART4_TX_OK_FLAG BIT(13)

#define STUDY_OK_FLAG BIT(14)

#define  TRAMPLE_SPEEDING_FLAG BIT(15)

#define START_KEY_FLAG 	BIT(16)
#define RESET_KEY_FLAG 	BIT(17)


#define XFS5152CE_PlayEnd_FLAG 	BIT(18)

#define XFS5152CE_PLAY_FLAG BIT(19)


#define XFS5152CE_WAIT_PLAY_FLAG BIT(20)
#define XFS5152CE_WAIT_STOP_FLAG BIT(21)


#define GAME_VOICE_PLAY_OK_FLAG BIT(22)


#define UART2_RX_OK_FLAG  BIT(23) 
#define UART2_TX_OK_FLAG  BIT(24) 

#define PARA_SET_OK_FLAG  BIT(25) 

#define GAME_FAILURE_FLAG BIT(26) 



void PostDeviceMsg(u8 msg);

#endif	//_APP_H_
