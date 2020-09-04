#include "includes.h"
#include "can.h"
#include "mco.h"
#include "mcohw.h"
#include "ds401.h"
#include "GlobalVar.h"
extern ApplicationTypeDef Appli_state;
extern u16 TopMenuResetTimeCnt;
/*--------设置队列参量--------*/
//定义消息队列长度	

void		*DeviceMsgGrp[DEVICE_MESSAGES];								//定义消息指针数组					
OS_EVENT	*Device_Q;											//定义事件控制块
u8 DeviceMsg[DEVICE_MESSAGES];

//定义消息队列长度			
void		*KeyMsgGrp[KEY_MESSAGES];								//定义消息指针数组					
OS_EVENT	*Key_Q;											//定义事件控制块
u8 KeyMsg[KEY_MESSAGES];


//定义消息队列长度			
void		*XFS5152CE_RX_MsgGrp[XFS5152CE_RX_MESSAGES];								//定义消息指针数组					
OS_EVENT	*XFS5152CE_RX_Q;											//定义事件控制块
u8 XFS5152CE_RX_Msg[XFS5152CE_RX_MESSAGES];




//定义消息队列长度			
void		*CAN_IRQ_RX_MsgGrp[CAN_IRQ_RX_MESSAGES];								//定义消息指针数组					
OS_EVENT	*CAN_IRQ_RX_Q;											//定义事件控制块
Message CAN_IRQ_RX_Msg[CAN_IRQ_RX_MESSAGES];



//定义消息队列长度			
void		*GAME_RX_MsgGrp[GAME_RX_MESSAGES];								//定义消息指针数组					
OS_EVENT	*GAME_RX_Q;											//定义事件控制块
UartProtocl GAME_RX_Msg[GAME_RX_MESSAGES];


OS_FLAG_GRP *pFlagGrpMidi;
OS_FLAGS  MIDI_Flag;
u8 OnlineDevice=0;

u8 ActivityDevice=NONE_DEVICE;



OS_TMR  *Timr1;

OS_TMR  *Timr2;
/*
OS_TMR  *Timr3;
OS_TMR  *Timr4;
*/
OS_EVENT *XFS5152CE_SemSignal;

OS_EVENT * GameModePlay_sem;

OS_EVENT *  XFS5152CE_PlayEnd_Sem;



OS_STK task_MidiPlay_stk[TASK_MIDI_PLAY_STK_SIZE];		  //定义栈
OS_STK task_Display_stk[TASK_DISPLAY_STK_SIZE];		  //定义栈
OS_STK task_AutoLighits_stk[TASK_AUTO_LIGHTS_STK_SIZE];		  //定义栈

OS_STK task_Scan_stk[TASK_SCAN_STK_SIZE];		  //定义栈

//OS_STK task_Key_stk[TASK_KEY_STK_SIZE];		  //定义栈

OS_STK task_UartRx_stk[TASK_UART_RX_STK_SIZE];		  //定义栈

OS_STK task_game_stk[TASK_GAME_STK_SIZE];		  //定义栈


OS_STK task_GameVoicePrompt_stk[TASK_GAME_VOICE_PROMPT_STK_SIZE];		  //定义栈

OS_STK task_XFS5152CE_Play_stk[TASK_XFS5152CE_Play_STK_SIZE];

OS_STK task_LedControl_stk[TASK_LED_CONTROL_STK_SIZE];


OS_STK task_Mco_Tx_stk[TASK_MCO_TX_STK_SIZE];

OS_STK task_Mco_Rx_stk[TASK_MCO_RX_STK_SIZE];


//u32 NoteDelayTimeCntVal;
//u32 MusicDelayTimeCntVal;


extern Message RxMSG;



void MyTimr1Callback (OS_TMR *ptmr, void *p_arg);

void MyTimr2Callback (OS_TMR *ptmr, void *p_arg);
/*
void MyTimr3Callback (OS_TMR *ptmr, void *p_arg);

void MyTimr4Callback (OS_TMR *ptmr, void *p_arg);
*/

void  Ctx(void);




void App_TaskCreateHook(OS_TCB *ptcb)
{
	(void)ptcb; 
}

void App_TaskDelHook(OS_TCB *ptcb)
{
	(void)ptcb; 
}

void App_TaskIdleHook(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}


void App_TaskStatHook(void)
{

}

void App_TaskSwHook(void)
{
	
}

void App_TCBInitHook(OS_TCB *ptcb)
{
	(void)ptcb; 
}

void App_TimeTickHook(void)
{
	
}




void Task_Start(void *p_arg)
{
	INT8U err;

    (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告

	pFlagGrpMidi = OSFlagCreate (MIDI_Flag,(INT8U *)&err);
	
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
			(OS_FLAGS) 0xffffffff ,
			(INT8U )OS_FLAG_CLR,
			(INT8U *)&err);	
	

	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
			(OS_FLAGS) AUTO_LIGHTS_EN_DIS_FLAG | MIDI_PLAY_EN_DIS_FLAG,
			(INT8U )OS_FLAG_SET,
			(INT8U *)&err);	


	Key_Q=OSQCreate(&KeyMsgGrp[0],KEY_MESSAGES);
	err = OSQFlush(Key_Q);
	
	Device_Q=OSQCreate(&DeviceMsgGrp[0],DEVICE_MESSAGES);
	err = OSQFlush(Device_Q);
	
	XFS5152CE_RX_Q=OSQCreate(&XFS5152CE_RX_MsgGrp[0],XFS5152CE_RX_MESSAGES);
	err = OSQFlush(XFS5152CE_RX_Q);
	
	
	CAN_IRQ_RX_Q=OSQCreate(&CAN_IRQ_RX_MsgGrp[0],CAN_IRQ_RX_MESSAGES);
	err = OSQFlush(CAN_IRQ_RX_Q);
	
	GAME_RX_Q=OSQCreate(&GAME_RX_MsgGrp[0],GAME_RX_MESSAGES);
	err = OSQFlush(GAME_RX_Q);
	
	
	/*
	OSTaskCreate(Task_UartRx,(void *)0,		  	//创建任务2
	   &task_UartRx_stk[TASK_UART_RX_STK_SIZE-1], UART_RX_TASK_PRIO);
	*/
	
	
	
	/*
	OSTaskCreate(Task_Key,(void *)0,		  	//创建任务2
	   &task_Key_stk[TASK_KEY_STK_SIZE-1], KEY_TASK_PRIO);
	*/

	OSTaskCreate(Task_MidiPlay,(void *)0,		  	//创建任务2
	   &task_MidiPlay_stk[TASK_MIDI_PLAY_STK_SIZE-1], MIDI_PLAY_TASK_PRIO);

	

	OSTaskCreate(Task_AutoLighits,(void *)0,		   	//创建任务3
	   &task_AutoLighits_stk[TASK_AUTO_LIGHTS_STK_SIZE-1], AUTO_LIGHTS_TASK_PRIO);



	OSTaskCreate(Task_Display,(void *)0,		   	//创建任务3
	   &task_Display_stk[TASK_DISPLAY_STK_SIZE-1], DISPLAY_TASK_PRIO);
	
	OSTaskCreate(Task_Scan,(void *)0,		   	//创建任务3
	   &task_Scan_stk[TASK_SCAN_STK_SIZE-1], SCAN_TASK_PRIO);
	
	
	OSTaskCreate(Task_Game,(void *)0,		   	//创建任务3
	   &task_game_stk[TASK_GAME_STK_SIZE-1], GAME_TASK_PRIO);

	
	
	
	OSTaskCreate(Task_GameVoicePrompt,(void *)0,		   	//创建任务3
	   &task_GameVoicePrompt_stk[TASK_GAME_VOICE_PROMPT_STK_SIZE-1], GAME_VOICE_PROMPT_TASK_PRIO);
	
	
	
	OSTaskCreate(Task_XFS5152CE_Play,(void *)0,		   	//创建任务3
	   &task_XFS5152CE_Play_stk[TASK_XFS5152CE_Play_STK_SIZE-1], XFS5152CE_Play_TASK_PRIO);
	
	
	
	OSTaskCreate(Task_LedControl,(void *)0,		   	//创建任务3
	   &task_LedControl_stk[TASK_LED_CONTROL_STK_SIZE-1], LED_CONTROL_TASK_PRIO);
	/*
	OSTaskCreate(Task_Mco_Tx,(void *)0,		   	//创建任务3
	   &task_Mco_Tx_stk[TASK_MCO_TX_STK_SIZE-1], MCO_TX_TASK_PRIO);
	*/
	OSTaskCreate(Task_Mco_Rx,(void *)0,		   	//创建任务3
	   &task_Mco_Rx_stk[TASK_MCO_RX_STK_SIZE-1], MCO_RX_TASK_PRIO);
	
	Timr1=OSTmrCreate (	0,
						(500/10),
						OS_TMR_OPT_PERIODIC,
						(OS_TMR_CALLBACK) MyTimr1Callback,
						(void*)0, 
						(INT8U*)"Timr1",
						(INT8U*)&err);
	
	OSTmrStart ((OS_TMR *)Timr1,(INT8U *)&err);
						
	/*				
	Timr2=OSTmrCreate (	0,
					//(5000/10),
					(5000/10),
					OS_TMR_OPT_ONE_SHOT,
					(OS_TMR_CALLBACK) MyTimr2Callback,
					(void*)0, 
					(INT8U*)"Timr2",
					(INT8U*)&err);				
										
	*/	
	/*					
	Timr3=OSTmrCreate (	0,
						(10/10),
						OS_TMR_OPT_ONE_SHOT,
						(OS_TMR_CALLBACK) MyTimr3Callback,
						(void*)0, 
						(INT8U*)"Timr3",
						(INT8U*)&err);
	
	OSTmrStart ((OS_TMR *)Timr3,(INT8U *)&err);					
						
	Timr4=OSTmrCreate (	0,
						(10/10),
						OS_TMR_OPT_PERIODIC,
						(OS_TMR_CALLBACK) MyTimr4Callback,
						(void*)0, 
						(INT8U*)"Timr4",
						(INT8U*)&err);
	
	OSTmrStart ((OS_TMR *)Timr4,(INT8U *)&err);					
	*/					

						
						


	XFS5152CE_SemSignal = OSSemCreate(0);
						
	GameModePlay_sem = OSSemCreate(0);						
			
	XFS5152CE_PlayEnd_Sem = OSSemCreate(0);	



    while (1)
    {

		INT8U err;
		OS_FLAGS  Flags;
		
        INT32U ticks;
		
		
		Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
						(INT8U *)&err);
	
		if ((Flags & TRAMPLE_SPEEDING_FLAG) == TRAMPLE_SPEEDING_FLAG)
		{
			ticks = (RamSetParameters.TrampleDelayTime* OS_TICKS_PER_SEC);
			OSTimeDly(ticks);

			IR_Rx_Cnt =0;
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) TRAMPLE_SPEEDING_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);
		}
		else
		{
			ticks = (RamSetParameters.TrampleCountTime* OS_TICKS_PER_SEC)/ 1000uL;
			OSTimeDly(ticks);
			IR_Rx_Cnt =0;
		}	
    }	
}




void u_disk_detect(void)
{	
	static ApplicationTypeDef pre_state = APPLICATION_IDLE;
	MX_USB_HOST_Process();                    
	if (pre_state != Appli_state) 
	{	
		switch(Appli_state)
		{
			case APPLICATION_DISCONNECT:
				remove_u_disk();
				break;

			case APPLICATION_READY:
				insert_u_disk(); 		
				break;

			default:
				break;
		}
		pre_state = Appli_state;
	}
}


void AutoPlayDelay(void)
{
	
	//#define WeekUpTimeCntVal  (60000/10)
	
	
	//static u16 MIDI_AutoPlayCnt=0;
	INT8U err;
	OS_FLAGS  Flags;
	
	volatile u32 NoteDelayTimeCntVal= RamSetParameters.NoteDelayTime/10;
	volatile u32 MusicDelayTimeCntVal= RamSetParameters.MusicDelayTime*1000/10;
	
	//NoteDelayTimeCntVal= RamSetParameters.NoteDelayTime/10;
	//MusicDelayTimeCntVal= RamSetParameters.MusicDelayTime*1000/10;
	
	
	if (RamSetParameters.MusicDelayTime ==0)
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
					(INT8U) OS_FLAG_SET,
					(INT8U  *)&err);	
	}
	else if (RamSetParameters.MusicDelayTime ==99999)
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);	
	}
	else
	{
		Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
		
		if (((Flags & MIDI_PLAY_FLAG) == MIDI_PLAY_FLAG) ||((Flags & START_KEY_FLAG) == START_KEY_FLAG))
		{
			MIDI_AutoPlayCnt=0;
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);	
			
			
		}
		else
		{
			if ((Flags & MIDI_AUTO_PLAY_FLAG) != MIDI_AUTO_PLAY_FLAG)
			{
				MIDI_AutoPlayCnt++;

				//if ((MIDI_AutoPlayCnt==STOP_PLAY_DELAY_TIME))
				
				if (MIDI_AutoPlayCnt==NoteDelayTimeCntVal)
				{	
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) MIDI_STOP_PLAY_FLAG,
							(INT8U) OS_FLAG_SET,
							(INT8U  *)&err);		
				}

				//else if (MIDI_AutoPlayCnt>AUTO_PLAY_DELAY_TIME)
				else if (MIDI_AutoPlayCnt>MusicDelayTimeCntVal)	
				{
					
					//if ((Flags & MIDI_PLAY_EN_DIS_FLAG) != MIDI_PLAY_EN_DIS_FLAG)
					{
					
						//WEEK_UP();
						
						OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_STOP_PLAY_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);	
						
						
						/*
						OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) RESET_KEY_FLAG ,
						(INT8U )OS_FLAG_WAIT_CLR_ALL,
						(INT32U) 0,
						(INT8U *)&err);
						*/
						
						
						Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
		
						if ((Flags & GAME_VOICE_PLAY_OK_FLAG) != GAME_VOICE_PLAY_OK_FLAG)
						{
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
									(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
									(INT8U) OS_FLAG_SET,
									(INT8U  *)&err);	
						}
					}
				}
				
				/*
				else if (0==(MIDI_AutoPlayCnt%WeekUpTimeCntVal))
				{
					
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) MIDI_STOP_PLAY_FLAG,
							(INT8U) OS_FLAG_SET,
							(INT8U  *)&err);		
				}
				*/
				
			}
		}
	}
}



void MyTimr1Callback (OS_TMR *ptmr, void *p_arg)
{
	(void)p_arg; 
	
	
	if ( RamSetParameters.ReturnTopTime!=0)
	{
		if (++TopMenuResetTimeCnt > RamSetParameters.ReturnTopTime*2)
		{
			 TopMenuResetTimeCnt=0;
			 Jump2Menu(MenuTopIndex,FlashMode_AutoInit);
			 GUI_Clear();
		}
	}
	
	MenuMainLoop();
	
	if (RamSetParameters.LedControl.LedColorCycleTime>0)
	{
		LedColorCycleControl();
	}

	LedTimerControl();

}

void MyTimr2Callback (OS_TMR *ptmr, void *p_arg)
{
	(void)p_arg; 

	SimulateGameFailure();
}


void Task_Game(void *p_arg)
{
	//INT8U err;
	//INT32U ticks;
	
	(void)p_arg; 
	
    while (1)
    {
		/*	
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) START_KEY_FLAG ,
					(INT8U )OS_FLAG_WAIT_SET_ALL,
					(INT32U) 0,
					(INT8U *)&err);
		

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
		
		TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,
				(U8)(GROUP1_W_LED_BIT_OFF|
				GROUP1_R_LED_BIT_OFF|
				GROUP1_G_LED_BIT_OFF|
				GROUP1_B_LED_BIT_OFF));

		OSTimeDlyHMSM(0, 0,0,100);
		
		my_strcpy(VoiceBuff, "请做好准备，游戏开始");

		XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);

*/


		//OSTimeDlyHMSM(0, 0,3,0);
		//OSSemPost(XFS5152CE_SemSignal);
		
		
		//XFS5152CE_SendData_Wita(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);				
		game();						
	}
}


//void MyTimr3Callback (OS_TMR *ptmr, void *p_arg)
	
//void Task_UartRx(void *p_arg)
	
void Task_Mco_Rx (void *p_arg)
{
	//INT8U err;
	INT32U ticks;
	//Message *msg;
	(void)p_arg; 

    while (1)
    {	
		/*
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) UART1_RX_OK_FLAG ,
					(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
					(INT32U) 0,
					(INT8U *)&err);
		*/
		
		//msg = ((Message *)OSQPend(CAN_IRQ_RX_Q,0,&err));
		
		
		
		
		
		analyzeUartPacket();
		
		//analyzeUartPacket(msg);
		//analyzeUartPacket();
		//USART3_RE_RX();
		
		
		/*---------------------------------------------------------------------------------*/
		ticks = (RamSetParameters.SyllableDelayTime * OS_TICKS_PER_SEC)/ 1000uL;
		OSTimeDly(ticks);
		
		OSTimeDlyHMSM(0, 0,0,10);
		
		
		//OSTmrStart ((OS_TMR *)Timr4,(INT8U *)&err);
		
		/*
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) UART1_RX_OK_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);
		*/				
						
						
						
						
		/*---------------------------------------------------------------------------------*/				
	}
}
#if 0
void Task_Mco_Tx (void *p_arg)
{
	(void)p_arg; 
	
	//u8 newInput[8]={0,1,2,3,4,5,6,7};
	
	while (1)
    {
		//Ctx();
		//OSTimeDlyHMSM(0, 0,0,10);
		
		//digital_input_handler(CANOpenMasterObject, newInput, 8);
		//newInput[0]++;
		OSTimeDlyHMSM(0, 0,1,0);
	}
}

#endif

void Task_Key(void *p_arg)
{
	(void)p_arg; 

    while (1)
    {	
		//key_scan();
		OSTimeDlyHMSM(0, 0,0,10);	
	}
}



void Task_Scan(void *p_arg)
{
	(void)p_arg; 

    while (1)
    {	
		u_disk_detect();
		sdmmc_io_detect();	
		get_midi_linghts_key();	
		get_start_reset_key();
		AutoLightsDelay();
		//RGB_Lights_deal();
		AutoPlayDelay();
		OSTimeDlyHMSM(0, 0,0,10);	
	}
}




void FIndDevice(void)
{
	INT8U err;
	OS_FLAGS Device;
	
	static u8 FindDeviceCnt=0;
	

	Device = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
						   (INT8U *)&err);
	
	if ((Device & U_DISK_ONLINE_FLAG) == U_DISK_ONLINE_FLAG)
	{
		if (ActivityDevice != U_DISK_ID)	
		{	
			FindDeviceCnt=0;
			PostDeviceMsg(MSG_USB_DISK_IN);
			return;	
		}		
	}
	
	if ((Device & SD_ONLINE_FLAG) == SD_ONLINE_FLAG)
	{
		if (ActivityDevice != SDMMC_ID)	
		{	
			FindDeviceCnt=0;
			PostDeviceMsg(MSG_SDMMC_IN);
			return;	
		}
	}
	
	if (FindDeviceCnt<10)
	{
		FindDeviceCnt++;
		ActivityDevice= NONE_DEVICE;
		PostDeviceMsg(MSG_FIND_NEW_DEVICE);	
	}
	else
	{
		FindDeviceCnt=0;
	}
}





void Task_MidiPlay(void *p_arg)
{
    
	INT8U err;
	volatile OS_FLAGS Device;
	
	volatile u8 res;
	
	volatile u8 msg;
	
	(void)p_arg; 
	
    while (1)
    {	
		//MX_FATFS_Init();
		
		Device = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
						   (INT8U *)&err);
		
		if (((Device & MIDI_PLAY_EN_DIS_FLAG) != MIDI_PLAY_EN_DIS_FLAG) && ((Device & START_KEY_FLAG) != START_KEY_FLAG))
		{
			msg = *((u8 *)OSQPend(Device_Q,0,&err));
			switch(msg)
			{
				case MSG_SDMMC_IN:
				{	
					res=ReadMidiFile(SD_Path);	
				}
				break;

				case MSG_USB_DISK_IN:
				{	
					res=ReadMidiFile(USBH_Path);
				}
				break;
				
				case MSG_FIND_NEW_DEVICE:
				case MSG_USB_DISK_OUT:
				case MSG_SDMMC_OUT:
				default:
				FIndDevice();
				break;
			}

			switch(res)
			{
				case MIDI_ERR_READ_FOLDER_END: 	
					PostDeviceMsg(MSG_FIND_NEW_DEVICE);
					break;

				default:
					break;	
			}		
		}
		else
		{
			
		}
		OSTimeDlyHMSM(0, 0,0,100);	
    }
}






void Task_AutoLighits(void *p_arg)
{
	(void)p_arg; 
	INT8U err;
	OS_FLAGS Flags;
	
    while (1)
    {	
		Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
		if (((Flags & AUTO_LIGHTS_EN_DIS_FLAG) != AUTO_LIGHTS_EN_DIS_FLAG) && 
			((Flags & AUTO_LIGHT_FLAG) == AUTO_LIGHT_FLAG) && 
			(RamSetParameters.DeviceTotal!=0x00) &&
			//(T1000TimerOnOffFlag == TIMER_ON) &&
			((Flags & START_KEY_FLAG) != START_KEY_FLAG))
		{	
			//T1000_POWER_ON();
			lights_control();		
		}
		else
		{
			//T1000_POWER_OFF();
			OSTimeDlyHMSM(0, 0,0,100);
		}
    }
}


void Task_Display(void *p_arg)
{
	//INT8U err;
	
	u8 KeyVal=NO_KEY;

	(void)p_arg;
    while (1)
    {	
		//XFS5152CE_SendData(10, 0x01, GB2313, "科大讯飞");
		
		//KeyVal = *((u8 *)OSQPend(Key_Q,0,&err));
		
		CheckKey(KeyVal);	
		
		OSTimeDlyHMSM(0, 0,0,10);
    }
}


void Task_GameVoicePrompt(void *p_arg)
{
	INT8U err;
	
	OS_FLAGS Flags;

	(void)p_arg;
	
	u8 MidiAutoPlayFlagbuff;
	
	while (1)
    {	
		
		Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
								(INT8U *)&err);
		
		if (((Flags & START_KEY_FLAG) != START_KEY_FLAG) && ((Flags & RESET_KEY_FLAG) == RESET_KEY_FLAG))
		{
			/*
			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) RESET_KEY_FLAG ,
						(INT8U )OS_FLAG_WAIT_SET_ALL,
						(INT32U) 0,
						(INT8U *)&err);
			*/
			
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) GAME_VOICE_PLAY_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);		

			
			Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
								(INT8U *)&err);
	
			MidiAutoPlayFlagbuff = ((Flags & MIDI_AUTO_PLAY_FLAG) == MIDI_AUTO_PLAY_FLAG) ? 1:0;

			

			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);		
			
			
			
			my_strcpy(VoiceBuff, "欢迎来到跳房子的游戏，\
								  游戏规则：请按开关开始游戏，跟随引导灯光再5秒内踩到发光的方格内，游戏继续，否则游戏失败，请按开关重新开始。\
								  游戏过程为一个往返。系统会记录最快的一个成绩，游戏通关后会播报您的成绩和名次。快抬起双腿来跳吧");
			
			
			XFS5152CE_Play(XFS5152CE_NON_WAIT_PLAY | XFS5152CE_NON_WAIT_STOP);
			
			//OSSemPost(XFS5152CE_SemSignal);
			
			
			//XFS5152CE_SendData_Wita(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);	

			OSTimeDlyHMSM(0, 0,1,0);

			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) XFS5152CE_PlayEnd_FLAG ,
					(INT8U )OS_FLAG_WAIT_SET_ALL,
					(INT32U) 30*1000,
					(INT8U *)&err);

			//OSTimeDlyHMSM(0, 0,30,0);

			if (MidiAutoPlayFlagbuff ==1)
			{
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);		
			}
			else
			{
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);		
			}
			
		
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) GAME_VOICE_PLAY_OK_FLAG,
							(INT8U) OS_FLAG_CLR,
							(INT8U  *)&err);		
			
			
			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) RESET_KEY_FLAG ,
				(INT8U )OS_FLAG_WAIT_CLR_ALL,
				(INT32U) 0,
				(INT8U *)&err);
			
			
			
			OSTimeDlyHMSM(RamSetParameters.VoicePromptTime/3600uL, 
						  RamSetParameters.VoicePromptTime%3600uL/60,
						  RamSetParameters.VoicePromptTime%3600uL%60,0);
						  
		}


		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) RESET_KEY_FLAG ,
						(INT8U )OS_FLAG_CLR,
						(INT8U *)&err);	

		OSTimeDlyHMSM(0, 0,1,0);
	}		

}



void Task_XFS5152CE_Play(void *p_arg)
{
	INT8U err;
	OS_FLAGS Flags;
	OS_SEM_DATA  sem_data;
	(void)p_arg;
	
	while (1)
    {	
		OSSemPend(XFS5152CE_SemSignal,0,&err);
		
		Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,(INT8U *)&err);

		if ((Flags & XFS5152CE_PLAY_FLAG) == XFS5152CE_PLAY_FLAG)
		{
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) XFS5152CE_PLAY_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);
			
			XFS5152CE_SendData(my_strlen(VoiceBuff)+XFS5152CE_CMD_AND_PAR, VOICE_MIX_CMD, GB2313, VoiceBuff);
			
			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) XFS5152CE_PlayEnd_FLAG ,
						(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
						(INT32U) 0,
						(INT8U *)&err);
			
			Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
									(INT8U *)&err);
			
			if ((Flags & XFS5152CE_WAIT_STOP_FLAG) == XFS5152CE_WAIT_STOP_FLAG) 
			{
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) XFS5152CE_WAIT_STOP_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);	
				
				OSSemSet(XFS5152CE_SemSignal,0,&err);
			}
			else
			{
				OSSemQuery(XFS5152CE_SemSignal,&sem_data);
				
				if (sem_data.OSCnt>0x0001)
				{
					OSSemSet(XFS5152CE_SemSignal,1,&err);
					/*
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) XFS5152CE_PLAY_FLAG,
					(INT8U) OS_FLAG_SET,
					(INT8U  *)&err);
					*/
				}
			}		
		}
	}
}



void Task_LedControl(void *p_arg)
{
	INT8U err;
	UartProtocl *p_msg;	
	(void)p_arg;
	
	while (1)
    {	
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q, 0, &err));
		
		switch (p_msg->command)
		{
			case MIDI_PLAY: TxPlayLedControl(p_msg);break;
			case MIDI_STOP: TxStopLedControl(p_msg);break;
			default:break;
		}	
	}
}





void PostDeviceMsg(u8 msg)
{		
	static u8 MsgCnt=0;
	DeviceMsg[MsgCnt] =msg;
	OSQPost(Device_Q,(u8*)&DeviceMsg[MsgCnt]);
	MsgCnt++;								//执行下个缓冲区，避免覆盖原来的按键数据
	if(MsgCnt==DEVICE_MESSAGES)
	{
		MsgCnt=0;
	}
}

void Post_XFS5152CE_RX_Msg(u8 msg)
{		
	static u8 MsgCnt=0;
	XFS5152CE_RX_Msg[MsgCnt] =msg;
	OSQPost(XFS5152CE_RX_Q,(u8*)&XFS5152CE_RX_Msg[MsgCnt]);
	MsgCnt++;								//执行下个缓冲区，避免覆盖原来的按键数据
	if(MsgCnt==XFS5152CE_RX_MESSAGES)
	{
		MsgCnt=0;
	}
}


void Post_CAN_IRQ_RX_Msg(Message msg)
{		
	static u8 MsgCnt=0;
	CAN_IRQ_RX_Msg[MsgCnt] =msg;
	OSQPost( CAN_IRQ_RX_Q,(u8*)& CAN_IRQ_RX_Msg[MsgCnt]);
	MsgCnt++;								//执行下个缓冲区，避免覆盖原来的按键数据
	if(MsgCnt== CAN_IRQ_RX_MESSAGES)
	{
		MsgCnt=0;
	}
}


void Post_GAME_RX_Msg(UartProtocl msg)
{		
	static u8 MsgCnt=0;
	GAME_RX_Msg[MsgCnt] =msg;
	OSQPostFront( GAME_RX_Q,(u8*)& GAME_RX_Msg[MsgCnt]);
	MsgCnt++;								//执行下个缓冲区，避免覆盖原来的按键数据
	if(MsgCnt== GAME_RX_MESSAGES)
	{
		MsgCnt=0;
	}
}

