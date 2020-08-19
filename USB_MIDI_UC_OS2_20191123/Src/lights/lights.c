

#include "includes.h"
#include "lights.h"

u16 LightsCntVal = LIGHTS_CNT_DEFAULT_MAX_VAL ;

u8 LightsCnt=0;
u8 AutoLightsFlag=0;


u8 min_lights_addr = MIN_LIGHTS_ADDR;
u8 max_lights_addr = MAX_LIGHTS_ADDR;

u8 CurrentLightsAddr;
u8 LastLightsAddr;


//LightsColourTypeDef lights_colour_status=WHITE;



u8 lights_mode;

u8 LightsColour=0; 
u8 lights_colour_status=0; //0:W 1:R 2:G 3:B

u8 Mid_Max_CurrentLightsAddr;
u8 Mid_Max_LastLightsAddr;

u8 Mid_Min_CurrentLightsAddr;
u8 Mid_Min_LastLightsAddr;


u8 Cycles=0;
u8 Status=0;
u8 flag=0;

u8 RGB_LightsStatus;
u16 RGB_Lights_Cnt = 0;


//u8 Cycles=0;

/*
void AllLightsOff(void)
{	
	AutoLightsFlag =0;	
	Cycles=0;
	LightsCntVal =LIGHTS_CNT_DEFAULT_MAX_VAL; 
	lights_control_cmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF);
	//lights_control_cmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF);
}

void lights_control_cmd(u8 lights_addr,u8 lights_status)
{
	u8 temp;
	u8 lights_data=0;
	temp = lights_addr + lights_status+lights_data;
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);
	#if 0
	Uart3SendData(UART_SYNC);
	Uart3SendData(lights_addr);
	Uart3SendData(lights_status);
	
	Uart3SendData(lights_data);
	
	Uart3SendData(temp);
	Uart3SendData(UART_END);
	#endif	
}

*/


void AutoLightsDelay(void)
{
	//static u16 Midi_AutoLightsCnt=0;
	
	
	INT8U err;
	OS_FLAGS  Flags;
	
	volatile u32 LightsDelayTimeCntVal= RamSetParameters.LightsDelayTime*1000/10;
	
	
	if (RamSetParameters.LightsDelayTime ==0)
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHT_FLAG,
					(INT8U) OS_FLAG_SET,
					(INT8U  *)&err);	
	}
	else if (RamSetParameters.LightsDelayTime ==99999)
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHT_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);	
	}
	else
	{
		Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
		if (((Flags & MIDI_PLAY_FLAG) == MIDI_PLAY_FLAG) ||((Flags & START_KEY_FLAG) == START_KEY_FLAG))
		
		
		//if (MIDI_PlayFlag)
		{
			Midi_AutoLightsCnt=0;
			AutoLightsFlag =0;
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) AUTO_LIGHT_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);
		}
		else
		{
			//if (++Midi_AutoLightsCnt>AUTO_LIGHTS_DELAY_TIME)
			
			if (Midi_AutoLightsCnt < LightsDelayTimeCntVal)
			{
				Midi_AutoLightsCnt++;
			}
			else if (Midi_AutoLightsCnt==LightsDelayTimeCntVal)
			{
				Midi_AutoLightsCnt++;
				AutoLightsFlag =1;

				lights_colour_status =WHITE;
				lights_colour_select();
				
				
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) AUTO_LIGHT_FLAG,
						(INT8U) OS_FLAG_SET,
						(INT8U  *)&err);
				
			}
		}
	}
}


u8 Lights_min_to_mid_to_max(void)
{
	//static u8 flag=0;
	
	if(!flag)
	{	
		flag=1;
		//CurrentLightsAddr = min_lights_addr;
		//LastLightsAddr= max_lights_addr;
		
		
		Mid_Max_CurrentLightsAddr=(RamSetParameters.DeviceTotal/2)+1;
		Mid_Max_LastLightsAddr= RamSetParameters.DeviceTotal;

		Mid_Min_CurrentLightsAddr= RamSetParameters.DeviceTotal/2;
		Mid_Min_LastLightsAddr= MIN_LIGHTS_ADDR;


		//MidLightsAddCnt =0;
		
		//MidLightsAdd = RamSetParameters.DeviceTotal/2;;
		
		
		//CurrentLightsAddr = MIN_LIGHTS_ADDR;
		//LastLightsAddr= RamSetParameters.DeviceTotal;
	}
	TxDeviceCmd(Mid_Max_LastLightsAddr,LIGHTS_OFF,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Min_LastLightsAddr,LIGHTS_OFF,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Max_CurrentLightsAddr,LIGHTS_ON,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Min_CurrentLightsAddr,LIGHTS_ON,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	
	Mid_Max_LastLightsAddr = Mid_Max_CurrentLightsAddr;
	Mid_Min_LastLightsAddr = Mid_Min_CurrentLightsAddr;
	
	
	
	if (Mid_Max_CurrentLightsAddr < RamSetParameters.DeviceTotal)
	{
		Mid_Max_CurrentLightsAddr++;
		Mid_Min_CurrentLightsAddr--;
		return 0;
	}
	else
	{	
		Status=1;
		flag=0;
		return 1;
	}
}

u8 Lights_min_form_mid_form_max(void)
{
	//static u8 flag=0;
	
	if(!flag)
	{	
		flag=1;
		//CurrentLightsAddr = min_lights_addr;
		//LastLightsAddr= max_lights_addr;
		
		
		Mid_Max_CurrentLightsAddr=RamSetParameters.DeviceTotal;
		Mid_Max_LastLightsAddr= (RamSetParameters.DeviceTotal/2)+1;

		Mid_Min_CurrentLightsAddr= MIN_LIGHTS_ADDR;
		Mid_Min_LastLightsAddr=RamSetParameters.DeviceTotal/2; 


		//MidLightsAddCnt =0;
		
		//MidLightsAdd = RamSetParameters.DeviceTotal/2;;
		
		
		//CurrentLightsAddr = MIN_LIGHTS_ADDR;
		//LastLightsAddr= RamSetParameters.DeviceTotal;
	}
	
	
	TxDeviceCmd(Mid_Max_LastLightsAddr,LIGHTS_OFF,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Min_LastLightsAddr,LIGHTS_OFF,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Max_CurrentLightsAddr,LIGHTS_ON,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	TxDeviceCmd(Mid_Min_CurrentLightsAddr,LIGHTS_ON,0);
	//OSTimeDlyHMSM(0, 0,0,5);
	
	Mid_Max_LastLightsAddr = Mid_Max_CurrentLightsAddr;
	Mid_Min_LastLightsAddr = Mid_Min_CurrentLightsAddr;
	
	if (Mid_Min_CurrentLightsAddr < RamSetParameters.DeviceTotal/2)
	{
		Mid_Max_CurrentLightsAddr--;
		Mid_Min_CurrentLightsAddr++;
		
		return 0;
	}
	else
	{	
		Status=0;
		flag=0;
		return 1;
	}	
}

void LightsMode2(void)
{
	if (Status==0)
	{
		Lights_min_to_mid_to_max();
	}
	else
	{
		Lights_min_form_mid_form_max();
	}
	
	switch (RamSetParameters.LightsSpeed)	
	{
		case 0:
		{
			LightsCntVal =SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL;
		}
		break;
	
		case 1:
		{
			LightsCntVal=SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL;	
		}
		break;
		
		
		default: break;
	}

	OSTimeDlyHMSM(0, 0,0,LightsCntVal);
}


u8 Lights_1_x(void)
{
	//static u8 flag=0;
	
	if(!flag)
	{	
		flag=1;
		//CurrentLightsAddr = min_lights_addr;
		//LastLightsAddr= max_lights_addr;
		
		CurrentLightsAddr = MIN_LIGHTS_ADDR;
		LastLightsAddr= RamSetParameters.DeviceTotal;
	}
	TxDeviceCmd(LastLightsAddr,LIGHTS_OFF,0);
	
	//OSTimeDlyHMSM(0, 0,0,5);
	
	TxDeviceCmd(CurrentLightsAddr,LIGHTS_ON,0);
	
	//OSTimeDlyHMSM(0, 0,0,5);
	
	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr < RamSetParameters.DeviceTotal)
	{
		CurrentLightsAddr++;	
		return 0;
	}
	else
	{	
		flag=0;
		return 1;
	}
}
	
u8 Lights_x_1(void)
{
	//static u8 flag=0;
	
	if(!flag)
	{	
		flag=1;
		//CurrentLightsAddr = max_lights_addr;
		//LastLightsAddr= min_lights_addr;

		CurrentLightsAddr = RamSetParameters.DeviceTotal;
		LastLightsAddr= MIN_LIGHTS_ADDR;
	}
	
	TxDeviceCmd(LastLightsAddr,LIGHTS_OFF,0);
	
	//OSTimeDlyHMSM(0, 0,0,5);
	
	TxDeviceCmd(CurrentLightsAddr,LIGHTS_ON,0);
	
	//OSTimeDlyHMSM(0, 0,0,5);
	
	
	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr > MIN_LIGHTS_ADDR)
	{
		CurrentLightsAddr--;		
		return 0;
	}
	else
	{
		flag=0;
		return 1;
	}	
}	


void LightsMode1(void)
{
		
	#define MAX_CYCLES 3

	//static u8 Cycles=0;
	//static u8 Status=0;

	
	if (Cycles < MAX_CYCLES)
	{
		if (Lights_1_x())
		{
			Cycles++;
		}
	}	
	else if (Cycles < (MAX_CYCLES*2))	
	{	
		if (Lights_x_1())
		{
			Cycles++;
		}
	}
	else if (Cycles < (MAX_CYCLES*3))	
	{	
		if (Status==0)
		{
			if (Lights_1_x()) 	
			{
				Status++;
			}
		}
		else if (Status==1)
		{
			if (Lights_x_1())
			{
				Status=0;
				Cycles++;
			}
		}		
	}
	else
	{
		Cycles=0;
		
		
		/*
		if (LightsCntVal >LIGHTS_CNT_DEFAULT_MIN_VAL)
		{
			LightsCntVal -= LIGHTS_CNT_SETP_VAL; 
		}
		else
		{
			LightsCntVal =LIGHTS_CNT_DEFAULT_MAX_VAL; 
		}
		*/
		

		switch (RamSetParameters.LightsSpeed)	
		{
			case 0:
			{
				if (LightsCntVal >SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL)
				{
					LightsCntVal -= LIGHTS_CNT_SETP_VAL; 
				}
				else
				{
					LightsCntVal =SPEED1_LIGHTS_CNT_DEFAULT_MAX_VAL; 
				}
			}
			break;
		
			case 1:
			{
				if (LightsCntVal >SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL)
				{
					LightsCntVal -= LIGHTS_CNT_SETP_VAL; 
				}
				else
				{
					LightsCntVal =SPEED2_LIGHTS_CNT_DEFAULT_MAX_VAL; 
				}
			}
			break;
			
			
			default: break;
		}
	}

	OSTimeDlyHMSM(0, 0,0,LightsCntVal);
}	



u8 Mode3_Lights_1_x(void)
{
	//static u8 flag=0;
	u16 cnt;

	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = MIN_LIGHTS_ADDR;
		LastLightsAddr= RamSetParameters.DeviceTotal;
	}

	for (cnt=0; cnt < (CurrentLightsAddr / (RamSetParameters.LightsSpacing+1))+1;cnt++)
	{
		if (LastLightsAddr != (cnt * RamSetParameters.LightsSpacing))
		{
			TxDeviceCmd(LastLightsAddr - (cnt * RamSetParameters.LightsSpacing),LIGHTS_OFF,0);
		}
		TxDeviceCmd(CurrentLightsAddr - (cnt * RamSetParameters.LightsSpacing),LIGHTS_ON,0);
	}
	
	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr < RamSetParameters.DeviceTotal)
	{
		CurrentLightsAddr++;	
		return 0;
	}
	else
	{	
		flag=0;
		return 1;
	}
}
	



u8 Mode3_Lights_x_1(void)
{
	//static u8 flag=0;
	
	u16 cnt;
	
	
	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = RamSetParameters.DeviceTotal;
		LastLightsAddr= MIN_LIGHTS_ADDR;
	}
	
	for (cnt=0; cnt < ((RamSetParameters.DeviceTotal-CurrentLightsAddr)/(RamSetParameters.LightsSpacing))+1;cnt++)
	{
		if (LastLightsAddr != (cnt * (RamSetParameters.LightsSpacing+1)))
		{
			TxDeviceCmd(LastLightsAddr + (cnt * RamSetParameters.LightsSpacing),LIGHTS_OFF,0);
		}
		TxDeviceCmd(CurrentLightsAddr + (cnt * RamSetParameters.LightsSpacing),LIGHTS_ON,0);
	}

	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr > MIN_LIGHTS_ADDR)
	{
		CurrentLightsAddr--;		
		return 0;
	}
	else
	{
		flag=0;
		return 1;
	}	
}	


void LightsMode3(void)
{
	#define MODE3_MAX_CYCLES 3
	
	//static u8 Cycles=0;
	//static u8 Status=0;

	
	if (Cycles < MODE3_MAX_CYCLES)
	{
		//if (Mode3_Lights_1_x())
		if (Mode3_Lights_x_1())	
		
		{
			Cycles++;
		}
	}	
	else if (Cycles < (MODE3_MAX_CYCLES*2))	
	{	
		if (Mode3_Lights_x_1())
		{
			Cycles++;
		}
	}
/*
	else if (Cycles < ((MODE3_MAX_CYCLES*2)+1))	
	{	
		if (Status==0)
		{
			if (Lights_1_x()) 	
			{
				Status++;
			}
		}
		else if (Status==1)
		{
			if (Lights_x_1())
			{
				Status=0;
				Cycles++;
			}
		}
	}
	*/
	else if (Cycles ==((MODE3_MAX_CYCLES*2)+1))	
	{	
		if (Lights_1_x()) 	
		{
			Cycles++;
		}
	}
	else if (Cycles ==((MODE3_MAX_CYCLES*2)+2))	
	{		
		if (Lights_x_1())
		{
			Cycles++;
		}
	}
	else
	{
		Cycles=0;
	
	}
	
	switch (RamSetParameters.LightsSpeed)	
	{
		case 0:
		{
			LightsCntVal =SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL;
		}
		break;
	
		case 1:
		{
			LightsCntVal=SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL;	
		}
		break;
		
		
		default: break;
	}
	
	OSTimeDlyHMSM(0, 0,0,LightsCntVal);	
}




u8 Mode4_Lights_1_x(void)
{
	static U8 LightsSpacingCount=0;
	
	//static U8 LightsFirstCyclesStatus;
	
	static U8 cnt;
	
	U8 Last_LastLightsAddr;
	U8 Last_CurrentLightsAddr;
	
	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = MIN_LIGHTS_ADDR-1;
		LastLightsAddr= RamSetParameters.DeviceTotal-1;

		//LightsSpacingCount =0;
		
		//LightsFirstCyclesStatus =0;
		
	}

	TxDeviceCmd(LastLightsAddr+1,LIGHTS_OFF,LightsColour);
	
	OSTimeDlyHMSM(0, 0,0,10);
	
	TxDeviceCmd(CurrentLightsAddr+1,LIGHTS_ON,LightsColour);
	
	OSTimeDlyHMSM(0, 0,0,10);
	
	if (LightsSpacingCount>0)
	{
		Last_LastLightsAddr=(LastLightsAddr+(LightsSpacingCount+1)) % RamSetParameters.DeviceTotal;
		TxDeviceCmd(Last_LastLightsAddr+1,LIGHTS_OFF,LightsColour);
		
		OSTimeDlyHMSM(0, 0,0,10);
	
		Last_CurrentLightsAddr=(CurrentLightsAddr+(LightsSpacingCount+1)) % RamSetParameters.DeviceTotal;
		
		TxDeviceCmd(Last_CurrentLightsAddr+1,LIGHTS_ON,LightsColour);
		
		OSTimeDlyHMSM(0, 0,0,10);
	}

	LastLightsAddr = CurrentLightsAddr;

	CurrentLightsAddr++;
	
	if ((CurrentLightsAddr %= RamSetParameters.DeviceTotal)==0)
	{	
		cnt++;
		if ((cnt %=2)==0)
		{
			LightsSpacingCount++;
			if ((LightsSpacingCount %= RamSetParameters.LightsSpacing)==0)
			{
				flag=0;
				return 1;	
			}
		}
	}
	return 0;
}




u8 Mode4_Lights_x_1(void)
{
	static U8 LightsSpacingCount=0;
	
	//static U8 LightsFirstCyclesStatus;
	
	static U8 cnt;
	
	U8 Last_LastLightsAddr;
	U8 Last_CurrentLightsAddr;
	
	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = RamSetParameters.DeviceTotal;
		LastLightsAddr= MIN_LIGHTS_ADDR;
		
		//LightsSpacingCount =0;
		
		//LightsFirstCyclesStatus =0;
		
	}

	TxDeviceCmd(LastLightsAddr,LIGHTS_OFF,LightsColour);
	
	OSTimeDlyHMSM(0, 0,0,10);
	
	TxDeviceCmd(CurrentLightsAddr,LIGHTS_ON,LightsColour);
	
	OSTimeDlyHMSM(0, 0,0,10);
	
	if (LightsSpacingCount>0)
	{
		
		if (LastLightsAddr>(LightsSpacingCount+1))
		{
			Last_LastLightsAddr=(LastLightsAddr-(LightsSpacingCount+1));
		}
		else
		{
			//Last_LastLightsAddr=((LightsSpacingCount+1)-LastLightsAddr);
			
			Last_LastLightsAddr=(RamSetParameters.DeviceTotal-((LightsSpacingCount+1)-LastLightsAddr));
		}
	
		TxDeviceCmd(Last_LastLightsAddr,LIGHTS_OFF,LightsColour);
		
		OSTimeDlyHMSM(0, 0,0,10);
	
		//Last_CurrentLightsAddr=(CurrentLightsAddr-(LightsSpacingCount+2)) % RamSetParameters.DeviceTotal;
		
		
		if (CurrentLightsAddr>(LightsSpacingCount+1))
		{
			Last_CurrentLightsAddr=(CurrentLightsAddr-(LightsSpacingCount+1));
		}
		else
		{
			//Last_CurrentLightsAddr=((LightsSpacingCount+1)-CurrentLightsAddr);
			Last_CurrentLightsAddr=(RamSetParameters.DeviceTotal-((LightsSpacingCount+1)-CurrentLightsAddr));
		}

		TxDeviceCmd(Last_CurrentLightsAddr,LIGHTS_ON,LightsColour);
		
		OSTimeDlyHMSM(0, 0,0,10);
	}

	LastLightsAddr = CurrentLightsAddr;
	
	
	
	if (CurrentLightsAddr > MIN_LIGHTS_ADDR)
	{	
		CurrentLightsAddr--;
	}
	else
	{	
		flag=0;
		cnt++;
		if ((cnt %=2)==0)
		{
			LightsSpacingCount++;
			if ((LightsSpacingCount %= RamSetParameters.LightsSpacing)==0)
			{
				return 1;	
			}
		}
	}
	return 0;
}



void LightsMode4(void)
{
	#define MODE4_MAX_CYCLES 1
	
	if (Cycles < MODE4_MAX_CYCLES)
	{
		if (Mode4_Lights_1_x())
		//if (Mode4_Lights_x_1())	
		{
			Cycles++;
		}
	}	
	else if (Cycles < (MODE4_MAX_CYCLES*2))	
	{	
		if (Mode4_Lights_x_1())
		{
			Cycles++;
		}
	}
	else
	{
		Cycles=0;

		lights_colour_status++;
		lights_colour_status %=4;
		lights_colour_select();

	}

	
	
	switch (RamSetParameters.LightsSpeed)	
	{
		case 0:
		{
			LightsCntVal =SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL;
		}
		break;
	
		case 1:
		{
			LightsCntVal=SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL;	
		}
		break;
		
		
		default: break;
	}
	
	OSTimeDlyHMSM(0, 0,0,LightsCntVal);	
	//OSTimeDlyHMSM(0, 0,1,0);	
}











void lights_colour_select(void)		
{
	LightsColour=0;
	switch (lights_colour_status)
	{
		case WHITE:	
			LightsColour = GROUP1_R_LED_BIT_ON |
							GROUP1_G_LED_BIT_ON |
							GROUP1_B_LED_BIT_ON |
							GROUP2_R_LED_BIT_ON |
							GROUP2_G_LED_BIT_ON |
							GROUP2_B_LED_BIT_ON;
			break;
		case RED:
			LightsColour= GROUP1_R_LED_BIT_ON |GROUP2_R_LED_BIT_ON;
			break;
		case GREEN:
			LightsColour= GROUP1_G_LED_BIT_ON |GROUP2_G_LED_BIT_ON;
			break;
		case BLUE:
			LightsColour= GROUP1_B_LED_BIT_ON |GROUP2_B_LED_BIT_ON;
			break;
		default:
			LightsColour=0;
			break;
	}
}

void lights_control(void)
{
	/*
	if (RamSetParameters.LightsMode==0)
	{
		 LightsMode1();	
	}	
	else if (RamSetParameters.LightsMode== 1)
	{
		 LightsMode2();	
	}
	*/
	
	switch (RamSetParameters.LightsMode)	
	{	
		case 0: LightsMode1(); break;
		case 1: LightsMode2(); break;
		case 2: LightsMode3(); break;
		
		case 3: LightsMode4(); break;
		
		case 4: LightsMode5(); break;
		
		default: break;	
	}		
}	

void RGB_Lights_deal(void)
{
	OS_FLAGS Flags;
	INT8U err;
	
	Flags = OSFlagQuery((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
	
	
	if ((Flags & START_KEY_FLAG) != START_KEY_FLAG)
	{
		if ((Flags & AUTO_LIGHT_FLAG) == AUTO_LIGHT_FLAG) 
		{
			RGB_Lights_Cnt =LIGHTS_UP_DELAY_TIME_CNT_VAL;
			RGB_LightsStatus=0; 
		}
		else
		{
			if(++RGB_Lights_Cnt >LIGHTS_UP_DELAY_TIME_CNT_VAL)
			{
				RGB_Lights_Cnt=0;
				RGB_LightsStatusSelect();
				TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_ON,LightsColour);
			} 
		}
	}	
}



#if 0
void RGB_LightsStatusSelect(void)
{
	LightsColour=0x00;
	switch (RGB_LightsStatus)
	{
	   case 0:
		   
			LightsColour = 	GROUP1_R_LED_BIT_ON |
							GROUP1_G_LED_BIT_ON |
							GROUP1_B_LED_BIT_ON |
							GROUP2_R_LED_BIT_ON |
							GROUP2_G_LED_BIT_ON |
							GROUP2_B_LED_BIT_ON;
	   
			RGB_LightsStatus++;
			break;
	  
		case 1:
			LightsColour = 	GROUP1_R_LED_BIT_ON |
							GROUP1_G_LED_BIT_OFF |
							GROUP1_B_LED_BIT_OFF |
							GROUP2_R_LED_BIT_OFF |
							GROUP2_G_LED_BIT_ON |
							GROUP2_B_LED_BIT_OFF;
		
			RGB_LightsStatus++;
			break;

		case 2:
			LightsColour = 	GROUP1_R_LED_BIT_OFF |
							GROUP1_G_LED_BIT_ON |
							GROUP1_B_LED_BIT_OFF |
							GROUP2_R_LED_BIT_OFF |
							GROUP2_G_LED_BIT_OFF |
							GROUP2_B_LED_BIT_ON;
			RGB_LightsStatus++;
			break;
		case 3:
			LightsColour = 	GROUP1_R_LED_BIT_OFF |
							GROUP1_G_LED_BIT_OFF |
							GROUP1_B_LED_BIT_ON |
							GROUP2_R_LED_BIT_ON |
							GROUP2_G_LED_BIT_ON |
							GROUP2_B_LED_BIT_OFF;
			RGB_LightsStatus++;
			//RGB_LightsStatus=1;
			break;
	
		
		case 4:
			LightsColour = 	GROUP1_R_LED_BIT_ON |
							GROUP1_G_LED_BIT_ON |
							GROUP1_B_LED_BIT_OFF |
							GROUP2_R_LED_BIT_ON |
							GROUP2_G_LED_BIT_OFF |
							GROUP2_B_LED_BIT_ON;

			RGB_LightsStatus++;
			break;
		
		case 5:
		
			LightsColour = 	GROUP1_R_LED_BIT_ON |
							GROUP1_G_LED_BIT_OFF |
							GROUP1_B_LED_BIT_ON |
							GROUP2_R_LED_BIT_OFF |
							GROUP2_G_LED_BIT_ON |
							GROUP2_B_LED_BIT_ON;

			RGB_LightsStatus++;
			break;	
			
		case 6:
		
			LightsColour = 	GROUP1_R_LED_BIT_OFF |
							GROUP1_G_LED_BIT_ON |
							GROUP1_B_LED_BIT_ON |
							GROUP2_R_LED_BIT_ON |
							GROUP2_G_LED_BIT_OFF |
							GROUP2_B_LED_BIT_OFF;
		
		
		
			//RGB_LightsStatus++;
			RGB_LightsStatus=0;
			break;	
	
		default:
		
			LightsColour = 	GROUP1_R_LED_BIT_OFF |
							GROUP1_G_LED_BIT_OFF |
							GROUP1_B_LED_BIT_OFF |
							GROUP2_R_LED_BIT_OFF |
							GROUP2_G_LED_BIT_OFF |
							GROUP2_B_LED_BIT_OFF;
		
		
		
			break;	
	}  
}
#endif


#if 1
void RGB_LightsStatusSelect(void)
{
	LightsColour=0x00;
	switch (RGB_LightsStatus)
	{
	   case 0:
		   
			LightsColour |= GROUP1_R_LED_BIT_ON;
			LightsColour |=	GROUP1_G_LED_BIT_ON;
			LightsColour |=	GROUP1_B_LED_BIT_ON;
			LightsColour |=	GROUP2_R_LED_BIT_ON;
			LightsColour |=	GROUP2_G_LED_BIT_ON;
			LightsColour |=	GROUP2_B_LED_BIT_ON;
	   
			RGB_LightsStatus++;
			break;
	  
		case 1:
			LightsColour |= GROUP1_R_LED_BIT_ON ;
			LightsColour &=	GROUP1_G_LED_BIT_OFF;
			LightsColour &=	GROUP1_B_LED_BIT_OFF;
			LightsColour &=	GROUP2_R_LED_BIT_OFF;
			LightsColour |=	GROUP2_G_LED_BIT_ON ;
			LightsColour &=	GROUP2_B_LED_BIT_OFF;
		
			RGB_LightsStatus++;
			break;

		case 2:
			LightsColour &= GROUP1_R_LED_BIT_OFF;
			LightsColour |=	GROUP1_G_LED_BIT_ON ;
			LightsColour &=	GROUP1_B_LED_BIT_OFF;
			LightsColour &=	GROUP2_R_LED_BIT_OFF;
			LightsColour &=	GROUP2_G_LED_BIT_OFF;
			LightsColour |=	GROUP2_B_LED_BIT_ON ;
			RGB_LightsStatus++;
			break;
		case 3:
			LightsColour &= GROUP1_R_LED_BIT_OFF;
			LightsColour &=	GROUP1_G_LED_BIT_OFF;
			LightsColour |=	GROUP1_B_LED_BIT_ON ;
			LightsColour |=	GROUP2_R_LED_BIT_ON ;
			LightsColour |=	GROUP2_G_LED_BIT_ON ;
			LightsColour &=	GROUP2_B_LED_BIT_OFF;
			RGB_LightsStatus++;
			//RGB_LightsStatus=1;
			break;
	
		
		case 4:
			LightsColour |= GROUP1_R_LED_BIT_ON ;
			LightsColour |=	GROUP1_G_LED_BIT_ON ;
			LightsColour &=	GROUP1_B_LED_BIT_OFF;
			LightsColour |=	GROUP2_R_LED_BIT_ON ;
			LightsColour &=	GROUP2_G_LED_BIT_OFF;
			LightsColour |=	GROUP2_B_LED_BIT_ON ;
			RGB_LightsStatus++;
			break;
		
		case 5:
		
			LightsColour |= GROUP1_R_LED_BIT_ON ;
			LightsColour &=	GROUP1_G_LED_BIT_OFF;
			LightsColour |=	GROUP1_B_LED_BIT_ON ;
			LightsColour &=	GROUP2_R_LED_BIT_OFF;
			LightsColour |=	GROUP2_G_LED_BIT_ON ;
			LightsColour |=	GROUP2_B_LED_BIT_ON ;
			RGB_LightsStatus++;
			break;	
			
		case 6:
		
			LightsColour &= GROUP1_R_LED_BIT_OFF;
			LightsColour |=	GROUP1_G_LED_BIT_ON ;
			LightsColour |=	GROUP1_B_LED_BIT_ON ;
			LightsColour |=	GROUP2_R_LED_BIT_ON ;
			LightsColour &=	GROUP2_G_LED_BIT_OFF;
			LightsColour &=	GROUP2_B_LED_BIT_OFF;
		
		
		
			//RGB_LightsStatus++;
			RGB_LightsStatus=0;
			break;	
	
		default:
		
			LightsColour &= GROUP1_R_LED_BIT_OFF;
			LightsColour &=	GROUP1_G_LED_BIT_OFF;
			LightsColour &=	GROUP1_B_LED_BIT_OFF;
			LightsColour &=	GROUP2_R_LED_BIT_OFF;
			LightsColour &=	GROUP2_G_LED_BIT_OFF;
			LightsColour &=	GROUP2_B_LED_BIT_OFF;
		
			RGB_LightsStatus=0;
		
			break;	
	}  
}
#endif








u8 Mode5_Lights_1_x(void)
{
	//U8 Last_LastLightsAddr;
    //U8 Last_CurrentLightsAddr;
	
	//u8 RowNumber=0;
	u8 RowsDeviceNumber;
	u8 RowsDeviceId;
	
	
	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = MIN_LIGHTS_ADDR;
		LastLightsAddr= RamSetParameters.MaxRowNumber;	
	}
	
	RowsDeviceId=FindRowsDeviceId(LastLightsAddr,0);
	for (RowsDeviceNumber=1; RowsDeviceNumber <= RamSetParameters.RowsDeviceNumber[LastLightsAddr];RowsDeviceNumber++)
	{
		TxDeviceCmd(RowsDeviceId-RowsDeviceNumber,LIGHTS_OFF,LightsColour);		
	}
		
	RowsDeviceId=FindRowsDeviceId(CurrentLightsAddr,0);
	for (RowsDeviceNumber=1; RowsDeviceNumber <= RamSetParameters.RowsDeviceNumber[CurrentLightsAddr];RowsDeviceNumber++)
	{
		TxDeviceCmd(RowsDeviceId-RowsDeviceNumber,LIGHTS_ON,LightsColour);	
	}

	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr < RamSetParameters.MaxRowNumber)
	{
		CurrentLightsAddr++;
		return 0;
	}
	else
	{	
		flag=0;
		return 1;
	}
}



u8 Mode5_Lights_x_1(void)
{
	//U8 Last_LastLightsAddr;
	//U8 Last_CurrentLightsAddr;
	
	//u8 RowNumber=0;
	u8 RowsDeviceNumber;
	u8 RowsDeviceId;
	
	
	if(!flag)
	{	
		flag=1;
		CurrentLightsAddr = RamSetParameters.MaxRowNumber;
		LastLightsAddr= MIN_LIGHTS_ADDR;
	}
	
	RowsDeviceId=FindRowsDeviceId(LastLightsAddr,1);
	for (RowsDeviceNumber=1; RowsDeviceNumber <= RamSetParameters.RowsDeviceNumber[LastLightsAddr];RowsDeviceNumber++)
	{
		TxDeviceCmd(RowsDeviceId+RowsDeviceNumber,LIGHTS_OFF,LightsColour);		
	}
		
	RowsDeviceId=FindRowsDeviceId(CurrentLightsAddr,1);
	for (RowsDeviceNumber=1; RowsDeviceNumber <= RamSetParameters.RowsDeviceNumber[CurrentLightsAddr];RowsDeviceNumber++)
	{
		TxDeviceCmd(RowsDeviceId+RowsDeviceNumber,LIGHTS_ON,LightsColour);	
	}

	LastLightsAddr = CurrentLightsAddr;
	
	if (CurrentLightsAddr > MIN_LIGHTS_ADDR)
	{
		CurrentLightsAddr--;		
		return 0;
	}
	else
	{
		flag=0;
		return 1;
	}	
}




void LightsMode5(void)
{
	#define MODE5_MAX_CYCLES 1
	
	if (Cycles < MODE5_MAX_CYCLES)
	{
		if (Mode5_Lights_1_x())
		//if (Mode4_Lights_x_1())	
		{
			Cycles++;
		}
	}	
	else if (Cycles < (MODE5_MAX_CYCLES*2))	
	{	
		if (Mode5_Lights_x_1())
		{
			Cycles++;
		}
	}
	else
	{
		Cycles=0;

		lights_colour_status++;
		lights_colour_status %=4;
		lights_colour_select();

	}
	
	switch (RamSetParameters.LightsSpeed)	
	{
		case 0:
		{
			LightsCntVal =SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL;
		}
		break;
	
		case 1:
		{
			LightsCntVal=SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL;	
		}
		break;
		
		
		default: break;
	}
	
	//OSTimeDlyHMSM(0, 0,0,LightsCntVal);	
	//OSTimeDlyHMSM(0, 0,1,0);	
	
	
	
	OSTimeDly(LightsCntVal);
}










