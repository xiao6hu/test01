#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************设置任务优先级*******************/



/*
#define STARTUP_TASK_PRIO           0

#define UART_RX_TASK_PRIO 			1 
#define GAME_TASK_PRIO              2
#define	MIDI_PLAY_TASK_PRIO		   	3

//#define KEY_TASK_PRIO 				7	  

#define OS_TASK_TMR_PRIO        	4

#define	DISPLAY_TASK_PRIO      		5

#define AUTO_LIGHTS_TASK_PRIO 		6

#define	SCAN_TASK_PRIO      		7

#define GAME_VOICE_PROMPT_TASK_PRIO   8

*/


#define STARTUP_TASK_PRIO           4
//#define UART_RX_TASK_PRIO 			4

#define	MCO_RX_TASK_PRIO      		5

//#define	MCO_TX_TASK_PRIO      		5

#define GAME_TASK_PRIO              6
#define GAME_VOICE_PROMPT_TASK_PRIO 7
#define	MIDI_PLAY_TASK_PRIO		   	8



#define XFS5152CE_Play_TASK_PRIO    9


#define LED_CONTROL_TASK_PRIO  10


#define AUTO_LIGHTS_TASK_PRIO 		11
#define OS_TASK_TMR_PRIO        	12
#define	SCAN_TASK_PRIO      		13
#define	DISPLAY_TASK_PRIO      		14

//#define KEY_TASK_PRIO 				7	






/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE       256   
#define	TASK_MIDI_PLAY_STK_SIZE		2048
#define	TASK_DISPLAY_STK_SIZE   	2024
#define TASK_AUTO_LIGHTS_STK_SIZE   1024

#define TASK_SCAN_STK_SIZE   2048


//#define TASK_KEY_STK_SIZE	256	  //定义栈

#define TASK_UART_RX_STK_SIZE	512	  //定义栈

#define TASK_GAME_STK_SIZE 512
	
#define TASK_GAME_VOICE_PROMPT_STK_SIZE	 256

#define TASK_XFS5152CE_Play_STK_SIZE    256
	
#define TASK_LED_CONTROL_STK_SIZE    256
#define	TASK_MCO_TX_STK_SIZE   256

#define	TASK_MCO_RX_STK_SIZE   256


#endif


