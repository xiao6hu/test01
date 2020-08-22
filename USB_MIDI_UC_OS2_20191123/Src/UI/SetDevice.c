
#include "includes.h"

#include "sys.h"
#include "menu.h"
#include "ks0108.h"

#include "usart1.h"
#include "stmflash.h"
#include "mco.h"

#include "GlobalVar.h"
#include "ds401.h"


//#include "RTC.h"
/*
#define my_strcpy(s1, s2) strcpy((char*)s1, (_CONST_ char*)s2)
	
#define my_strncpy(s1, s2, n) strncpy((char*)s1, (_CONST_ char*)s2, n)

#define my_strcmp(s1, s2) strcmp((_CONST_ char*)s1, (_CONST_ char*)s2)
	
#define my_strchr(s, c) strchr((_CONST_ char*)s, (int)c)

#define my_memcpy(s1, s2, n) *memcpy(s1, s2, n)

*/

U16 DeviceNumberCount=0;
U8 Config_M_Dev_Flag=1;
U16 StatrDeviceNumber=0;
U16 EndDeviceNumber=0;

extern u8 StudyFlag;

extern u32 *pTPRO1_val[2];
extern u32 *pRPRO1_val[2];
extern u32 * pODNodeID;

#define MAX_TEXT_LEN (32+1)




typedef struct{
	U8 Text[MAX_TEXT_LEN];    	//�����ַ�������
	U8 CursorPos;				//���λ��	
	//U8 *TextPointer;             //�����ַ�ָ��
	U8 MinText[MAX_TEXT_LEN];   //��������ַ�������
	U8 MaxText[MAX_TEXT_LEN];   //������С�ַ�������
}ParaSetType;


ParaSetType ParaSet;
U16 DeviceTotal;
/*
struct
{
  U16 MusicStatus     : 1;                          //0
  U16 LightsStatus    : 1;                          //1
  U16 Reserved        : 14;                          //2     
 
}DeviceStatus;
*/

#if 0
U8 * _CONST_ DeviceIdToEnglishChar[]=
{
	{"A"},{"B"},{"C"},{"D"},{"E"},{"F"},{"G"},{"H"},{"I"},{"J"},//Ӣ����ĸ
	{"K"},{"L"},{"M"},{"N"},{"O"},{"P"},{"Q"},{"R"},{"S"},{"T"},
	{"U"},{"V"},{"W"},{"X"},{"Y"},{"Z"},
};

U8 * _CONST_ DeviceIdToChinesePinyin[]=
{	
	//��ĸ 23
	{"b"},/*[��]*/
	{"p"},/*[��]*/
	{"m"},/*[��]*/
	{"f"},/*[��]*/
	{"d"},/*[��]*/ 
	{"t"},/*[��]*/ 
	{"n"},/*[ګ]*/ 
	{"l"},/*[��]*/ 
	{"g"},/*[��]*/ 
	{"k"},/*[��]*/ 
	{"h"},/*[��]*/
	{"j"},/*[��]*/ 
	{"q"},/*[��]*/ 
	{"x"},/*[ϣ]*/        
	{"z"},/*[��]*/
	{"c"},/*[��]*/
	{"s"},/*[˼]*/ 
	{"r"},/*[��]*/ 
	{"zh"},/*[֪]*/ 
	{"ch"},/*[��]*/ 
	{"sh"},/*[ʫ]*/
	{"y"},/*[ҽ]*/ 
	{"w"},/*[��]*/

	//�� �� ƴ �� �� ĸ �� 33
	
	{"a"},/*[��]*/ 
	{"an"},/*[��]*/ 
	{"ao"},/*[��]*/ 
	{"ai"},/*[��]*/ 
	{"ang"},/*[��]*/
	{"o"},/*[�]*/ 
	{"ong"},/*[��]*/ 
	{"ou"},/*[ŷ]*/
	{"e"},/*[��]*/ 
	{"en"},/*[��]*/ 
	{"er"},/*[��]*/ 
	{"ei"},/*[��]*/ 
	{"eng"},/*[��]*/
	{"i"},/*[��]*/ 
	{"ia"},/*[ѽ]*/ 
	{"iu"},/*[��]*/ 
	{"ie"},/*[Ү]*/ 
	{"in"},/*[��]*/ 
	{"ing"},/*[Ӣ]*/
	{"u"},/*[��]*/ 
	{"un"},/*[��]*/ 
	{"ua"},/*[��]*/ 
	{"uo"},/*[��]*/ 
	{"ue"},/*[��]*/ 
	{"ui"},/*[��]*/
	{"��"},/*[��]*/
	{"iao"},/*[��]*/ 
	{"ian"},/*[��]*/ 
	{"iang"},/*[��]*/ 
	{"iong"},/*[��]*/
	{"uai"},/*[��]*/ 
	{"uan"},/*[��]*/ 
	{"uang"},/*[��]*/

	//�����϶����� 16
	{"zhi"}, 
	{"chi"}, 
	{"shi"}, 
	{"ri"}, 
	{"zi"}, 
	{"ci"}, 
	{"si"},
	{"yi"}, 
	{"wu"}, 
	{"yu"},
	{"ye"}, 
	{"yue"},
	{"yin"}, 
	{"yun"}, 
	{"yuan"}, 
	{"ying"},

};
#endif





void DisplayInputOverrange(void)
{	
	PUSH();
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_InputOverrange[language].Text+0),0,3,126,GUI_TA_HCENTER);		
	OSTimeDlyHMSM(0, 0,1,0);	
	POP();
}





void DisplaySuccessFailure(u8 status)
{	
	PUSH();
	GUI_Clear();
	if (status)
	{
		GUI_DispStringAtBar(*(Menu_SuccessFailureIndex[language].Text+0),0,3,126,GUI_TA_HCENTER);		
	}
	else
	{
		GUI_DispStringAtBar(*(Menu_SuccessFailureIndex[language].Text+1),0,3,126,GUI_TA_HCENTER);
	}
	OSTimeDlyHMSM(0, 0,1,0);	
	POP();
}





void DisplayPromptInfo(U8 *String)
{	
	//PUSH();
	GUI_Clear();
	GUI_DispStringAtBar(String,0,3,126,GUI_TA_HCENTER);		
	OSTimeDlyHMSM(0, 0,1,0);	
	//POP();
}

/*
void LodeParametersData(void)
{
	STMFLASH_Read((u32)&DeviceTotal,(u16*)&DeviceTotal,1);
	STMFLASH_Read((u32)&DeviceStatus,(u16*)&DeviceStatus,1);
	
	min_lights_addr = MIN_LIGHTS_ADDR;	
	max_lights_addr = DeviceTotal;	
	
}

void ResetParametersData(void)
{
	DeviceTotal=0;
	DeviceStatus.MusicStatus=1;
	DeviceStatus.LightsStatus=1;
	STMFLASH_Write((u32)&DeviceTotal,(u16*)&DeviceTotal,1);
	STMFLASH_Write((u32)&DeviceStatus,(u16*)&DeviceStatus,1);
}
*/

//------------------------------------------------------------------	
//��ʾ��һ���˵�
//------------------------------------------------------------------
void MenuItemAdd(void)
{
	if(Item==(ItemNum-1)){
        Item = 0;
    }else{
        Item++;
    }
	
	Flash =FlashMode_ReFlash;	
}

//------------------------------------------------------------------	
//��ʾ��һ���˵�
//------------------------------------------------------------------
void MenuItemSub(void)
{
	if(Item==0){
        Item = ItemNum-1;
    }else{
        Item--;
    }
	Flash =FlashMode_ReFlash;
}
//------------------------------------------------------------------	
//�ַ�������ʾ
//------------------------------------------------------------------
void ParaSetDisplsy(U8 x0, U8 y0, U8 x1, U8 Mode)
{	
	//GUI_DispStringAtBar(ParaSet.Text, x0, y0, x1, Mode);//�˵�����
	
	GUI_SetEnFont(En_8x16);
	GUI_DispStringAt(ParaSet.Text, x0, y0);//�˵�����
	
	/*
	GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
	GUI_DispCharAt(ParaSet.Text[ParaSet.CursorPos], (x0 + ParaSet.CursorPos * 16), y0);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	*/
	
	
	GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
	GUI_DispCharAt(ParaSet.Text[ParaSet.CursorPos], (x0 + ParaSet.CursorPos * 8), y0);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
}


//------------------------------------------------------------------	
//�ַ������С
//9 --> 0 ��+ --> -
//------------------------------------------------------------------
void ParaSetSub(void)
{
	if((ParaSet.Text[ParaSet.CursorPos]) == ParaSet.MinText[ParaSet.CursorPos])
	{
        (ParaSet.Text[ParaSet.CursorPos]) = ParaSet.MaxText[ParaSet.CursorPos];
    }
	else
	{
		if (ParaSet.Text[ParaSet.CursorPos] == '+')
		{
			ParaSet.Text[ParaSet.CursorPos] = '-';
		}
		else
		{
			(ParaSet.Text[ParaSet.CursorPos])--;
		}
    }
	Flash =FlashMode_ReFlash;
}	

//------------------------------------------------------------------	
//�ַ���������
//0 --> 9 ��- --> +
//------------------------------------------------------------------
void ParaSetAdd(void)
{
	
	
	
	
	if(ParaSet.Text[ParaSet.CursorPos] == ParaSet.MaxText[ParaSet.CursorPos])
	{
        ParaSet.Text[ParaSet.CursorPos] = ParaSet.MinText[ParaSet.CursorPos];
    }
	else
	{
		/*
		if (ParaSet.Text[ParaSet.CursorPos] == '-')
		{
			ParaSet.Text[ParaSet.CursorPos] = '+';
		}
		else
		*/	
		
		
		
		{
			ParaSet.Text[ParaSet.CursorPos]++;
		}
    }
	
	Flash =FlashMode_ReFlash;
}

//------------------------------------------------------------------	
//�ַ�����������
//------------------------------------------------------------------
void ParaSetCursorLeftMoves(void)
{
	if (ParaSet.CursorPos == 0)
	{
		ParaSet.CursorPos = (strlen(( char *)ParaSet.Text)-1);
	}
	else
	{
		ParaSet.CursorPos--;
		
		if ((ParaSet.Text[ParaSet.CursorPos] == '.')||
			(ParaSet.Text[ParaSet.CursorPos] == ':')||
			(ParaSet.Text[ParaSet.CursorPos] == '-')||
			(ParaSet.Text[ParaSet.CursorPos] == '/'))
		{
			ParaSet.CursorPos--;
		}

		if ((ParaSet.Text[ParaSet.CursorPos-0] == ' ')&&
			(ParaSet.Text[ParaSet.CursorPos-1] == '>')&&
			(ParaSet.Text[ParaSet.CursorPos-2] == '-')&&
			(ParaSet.Text[ParaSet.CursorPos-3] == '-')&&
			(ParaSet.Text[ParaSet.CursorPos-4] == ' '))
		{
			ParaSet.CursorPos-=5;
		}
			 
	}
	Flash =FlashMode_ReFlash;
}


//------------------------------------------------------------------	
//�ַ�����������
//------------------------------------------------------------------
void ParaSetCursorRightMoves(void)
{
	ParaSet.CursorPos++;
	if (ParaSet.Text[ParaSet.CursorPos] == '\0')
	{
		ParaSet.CursorPos=0;
	}
	
	if ((ParaSet.Text[ParaSet.CursorPos] == '.')||
		(ParaSet.Text[ParaSet.CursorPos] == ':')||
		(ParaSet.Text[ParaSet.CursorPos] == '-')||
		(ParaSet.Text[ParaSet.CursorPos] == '/'))
	{
		ParaSet.CursorPos++;
	}
	if ((ParaSet.Text[ParaSet.CursorPos+0] == ' ')&&
		(ParaSet.Text[ParaSet.CursorPos+1] == '-')&&
		(ParaSet.Text[ParaSet.CursorPos+2] == '-')&&
		(ParaSet.Text[ParaSet.CursorPos+3] == '>')&&
		(ParaSet.Text[ParaSet.CursorPos+4] == ' '))
	{
		ParaSet.CursorPos+=5;
	}
	Flash =FlashMode_ReFlash;
}


//------------------------------------------------------------------		
//�����豸��ַ��ʼ��
//------------------------------------------------------------------
void DispMenuSetDeviceAddressInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		my_strcpy(ParaSet.Text, "000");
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(SetDeviceAddress_PosX, SetDeviceAddress_PosY, 126, GUI_TA_HCENTER);	  


   
}
//------------------------------------------------------------------		
//�����豸��ַ���
//------------------------------------------------------------------
void DispMenuSetDeviceAddressUp(void)
{
	INT8U  err;
	
	U16 DeviceAddress;
	
	//static U16 DeviceAddressBucak;
	
	
	//UartProtocl *p_msg;
	
	
	DeviceAddress=atoi((char *)ParaSet.Text);
	
	
	if (DeviceAddress >MAX_DEVICE_NUM)
	{
		
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
	
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) STUDY_OK_FLAG,
							(INT8U) OS_FLAG_CLR,
							(INT8U  *)&err);
		
		
		StudyFlag=1;
		TxDeviceCmd(DEFAULT_SLAVE_ADDR,STUDY_CMD,DeviceAddress);
		StudyFlag=0;
		
	
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) STUDY_OK_FLAG ,
				(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
				(INT32U) 1000,
				(INT8U *)&err);
		if (err == OS_ERR_TIMEOUT)
		{
			DisplaySuccessFailure(0);
		}
		else
		{
			DisplaySuccessFailure(1);
		}
		
		
		/*
		
		err = OSQFlush(GAME_RX_Q);
		
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q,1000,&err));
			
		if ((err == OS_ERR_TIMEOUT) || ((DeviceAddress != (p_msg->addr)) && (STUDY_CMD != p_msg->command)))
		{
			DisplaySuccessFailure(0);
		}
		else
		{
			DisplaySuccessFailure(1);
		}
		*/
		/*
		*pTPRO1_val[0]=0x00000000;
		*pTPRO1_val[1]=0x00000000;	
		*/
		Read_Inputs_8_Bit[0]=0x00;
		Read_Inputs_8_Bit[1]=0x00;
		Read_Inputs_8_Bit[2]=0x00;
		Read_Inputs_8_Bit[3]=0x00;
		Read_Inputs_8_Bit[4]=0x00;
		Read_Inputs_8_Bit[5]=0x00;
		Read_Inputs_8_Bit[6]=0x00;
		Read_Inputs_8_Bit[7]=0x00;
		
		/*
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) UART1_RX_OK_FLAG ,
					(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
					(INT32U) 1000,
					(INT8U *)&err);
					*/
		/*
		
		
		
		if (err == OS_ERR_TIMEOUT)
		{
			DisplaySuccessFailure(0);
		}
		else
		{
			DisplaySuccessFailure(1);
		}
		*/
		//DisplaySuccessFailure((err != OS_ERR_TIMEOUT) ? 1:0);
		
		
		
		
		
	}
	Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);

}	


//------------------------------------------------------------------	
//�����豸��ַ������
//------------------------------------------------------------------
void DispMenuSetDeviceAddressDown(void)
{
	
	//if ((GlobalKeyVal & (KEY_LONG_UP <<6 ))== (KEY_SHORT_UP <<6 ))
	{
		POP();
	}
}
//------------------------------------------------------------------	
//�����豸��ַ���ּ�
//------------------------------------------------------------------
void DispMenuSetDeviceAddressLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);

}

//------------------------------------------------------------------	
//�����豸��ַ�������
//------------------------------------------------------------------
void DispMenuSetDeviceAddressRight(void) 
{
	//ParaSetSub();
	ParaSetCursorRightMoves();
	Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);
	
}


void DispMenuSetDeviceAddressEscUp(void)
{
	//ParaSetCursorLeftMoves();
	//Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);
}

void DispMenuSetDeviceAddressEscDown(void)
{
	//ParaSetCursorRightMoves();
	//Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);
	
}
void DispMenuSetDeviceAddressEscOk(void)
{
	
}



//------------------------------------------------------------------		
//�����豸������ʼ��
//------------------------------------------------------------------
void DispMenuSetDeviceTotalInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.DeviceTotal);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(SetDevicesTotal_PosX , SetDevicesTotal_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
}

//------------------------------------------------------------------		
//�����豸�������
//------------------------------------------------------------------
void DispMenuSetDeviceTotalUp(void)
{
	U16 DeviceTotalBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	DeviceTotalBackup=atoi((char *)ParaSet.Text);

	if (DeviceTotalBackup >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.DeviceTotal=DeviceTotalBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceTotal-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceTotal,//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceTotal) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceTotal-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceTotal,//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceTotal) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((DeviceTotalBackup == RamSetParameters.DeviceTotal) ? 1:0);
	}
	Jump2Menu(SetDeviceTotalIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�����豸��������
//------------------------------------------------------------------
void DispMenuSetDeviceTotalDown(void)
{
	//if ((GlobalKeyVal & (KEY_LONG_UP <<6 ))== (KEY_SHORT_UP <<6 ))
	{
		POP();
	}
}
//------------------------------------------------------------------		
//�����豸�������ּ�
//------------------------------------------------------------------
void DispMenuSetDeviceTotalLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(SetDeviceTotalIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//�����豸�����������
//------------------------------------------------------------------
void DispMenuSetDeviceTotalRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(SetDeviceTotalIndex,FlashMode_ReFlash);
}



void DispMenuSetDeviceTotalEscUp(void)
{
	//ParaSetCursorLeftMoves();
	//Jump2Menu(SetDeviceTotalIndex,FlashMode_ReFlash);
}
void DispMenuSetDeviceTotalEscDown(void)
{
	//ParaSetCursorRightMoves();
	//Jump2Menu(SetDeviceTotalIndex,FlashMode_ReFlash);
	
}
void DispMenuSetDeviceTotalEscOk(void)
{
	
}	




//------------------------------------------------------------------		
//����������ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00000");
		my_strcpy(ParaSet.MaxText, "99999");
		
		snprintf((char*)&ParaSet.Text[0], 5+1,"%05d", RamSetParameters.MusicDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(MusicDelayTime_PosX , MusicDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}

//------------------------------------------------------------------		
//����������ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeUp(void)
{
	U32 MusicDelayTimeBackup;
	
	RamSetParameters.MusicDelayTime=atol((char *)ParaSet.Text);

	MusicDelayTimeBackup = RamSetParameters.MusicDelayTime;

	
	
	#ifdef INT_FLASH_SAVE
	//FLASH_WRITE(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
	//FLASH_READ(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
	FLASH_WRITE(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 	
	FLASH_READ(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 
    #endif

    #ifdef EEPROM_SAVE
    AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.MusicDelayTime-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.MusicDelayTime,//��д�����ݻ�����
				(sizeof(RamSetParameters.MusicDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
	
	AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.MusicDelayTime-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.MusicDelayTime,//��д�����ݻ�����
				(sizeof(RamSetParameters.MusicDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
	
    #endif

	DisplaySuccessFailure((MusicDelayTimeBackup == RamSetParameters.MusicDelayTime) ? 1:0);

	Jump2Menu(SetMusicDelayTimeIndex,FlashMode_ReFlash);
}



//------------------------------------------------------------------		
//����������ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//����������ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetMusicDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//����������ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetMusicDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetMusicDelayTimeEscUp(void)
{
	
}
void DispMenuSetMusicDelayTimeEscDown(void)
{

}
void DispMenuSetMusicDelayTimeEscOk(void)
{
	
}	

//------------------------------------------------------------------		
//���õƹ���ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00000");
		my_strcpy(ParaSet.MaxText, "99999");
		
		snprintf((char*)&ParaSet.Text[0], 5+1,"%05d", RamSetParameters.LightsDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(LightsDelayTime_PosX , LightsDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//���õƹ���ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeUp(void)
{
	U32 LightsDelayTimeBackup;
	
	RamSetParameters.LightsDelayTime=atol((char *)ParaSet.Text);

	LightsDelayTimeBackup = RamSetParameters.LightsDelayTime;

    #ifdef INT_FLASH_SAVE
        //FLASH_WRITE(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
        //FLASH_READ(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
        FLASH_WRITE(FlashSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime);     
        FLASH_READ(FlashSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime); 
    #endif
    
    #ifdef EEPROM_SAVE
        AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsDelayTime-(U32)&RamSetParameters,//��д���ַ
                    (uint8_t *)&RamSetParameters.LightsDelayTime,//��д�����ݻ�����
                    (sizeof(RamSetParameters.LightsDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
        
        AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsDelayTime-(U32)&RamSetParameters,//��д���ַ
                    (uint8_t *)&RamSetParameters.LightsDelayTime,//��д�����ݻ�����
                    (sizeof(RamSetParameters.LightsDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
        
    #endif

	DisplaySuccessFailure((LightsDelayTimeBackup == RamSetParameters.LightsDelayTime) ? 1:0);

	Jump2Menu(SetLightsDelayTimeIndex,FlashMode_ReFlash);
}



//------------------------------------------------------------------		
//���õƹ���ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//���õƹ���ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetLightsDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//���õƹ���ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetLightsDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetLightsDelayTimeEscUp(void)
{
	
}
void DispMenuSetLightsDelayTimeEscDown(void)
{

}
void DispMenuSetLightsDelayTimeEscOk(void)
{
	
}	

//------------------------------------------------------------------		
//���ò�̤����ʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.TrampleCountTime);
		
		//my_strcpy(ParaSet.Text, "000");
		
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(TrampleCountTime_PosX , TrampleCountTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_MS_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//���ò�̤����ʱ�����
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeUp(void)
{
	U16 TrampleCountTimeBackup;
	
	RamSetParameters.TrampleCountTime=atoi((char *)ParaSet.Text);

	TrampleCountTimeBackup = RamSetParameters.TrampleCountTime;

	
    #ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.TrampleCountTime,RamSetParameters.TrampleCountTime,RamSetParameters.TrampleCountTime);     
       FLASH_READ(RamSetParameters.TrampleCountTime,RamSetParameters.TrampleCountTime,RamSetParameters.TrampleCountTime); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleCountTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.TrampleCountTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.TrampleCountTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleCountTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.TrampleCountTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.TrampleCountTime) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif

    
	DisplaySuccessFailure((TrampleCountTimeBackup == RamSetParameters.TrampleCountTime) ? 1:0);

	Jump2Menu(SetTrampleCountTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//���ò�̤����ʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//���ò�̤����ʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetTrampleCountTimeIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//���ò�̤����ʱ��������
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetTrampleCountTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetTrampleCountTimeEscUp(void)
{
	
}
void DispMenuSetTrampleCountTimeEscDown(void)
{

}
void DispMenuSetTrampleCountTimeEscOk(void)
{
	
}	


//------------------------------------------------------------------		
//���ò�̤��ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.TrampleDelayTime);
		
		//my_strcpy(ParaSet.Text, "000");
		
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(TrampleDelayTime_PosX , TrampleDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//���ò�̤��ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeUp(void)
{
	U16 TrampleDelayTimeBackup;
	
	RamSetParameters.TrampleDelayTime=atoi((char *)ParaSet.Text);

	TrampleDelayTimeBackup = RamSetParameters.TrampleDelayTime;

	

    #ifdef INT_FLASH_SAVE
           FLASH_WRITE(RamSetParameters.TrampleDelayTime,RamSetParameters.TrampleDelayTime,RamSetParameters.TrampleDelayTime);     
           FLASH_READ(RamSetParameters.TrampleDelayTime,RamSetParameters.TrampleDelayTime,RamSetParameters.TrampleDelayTime); 
    #endif
           
    #ifdef EEPROM_SAVE
           AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleDelayTime-(U32)&RamSetParameters,//��д���ַ
                       (uint8_t *)&RamSetParameters.TrampleDelayTime,//��д�����ݻ�����
                       (sizeof(RamSetParameters.TrampleDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
           
           AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleDelayTime-(U32)&RamSetParameters,//��д���ַ
                       (uint8_t *)&RamSetParameters.TrampleDelayTime,//��д�����ݻ�����
                       (sizeof(RamSetParameters.TrampleDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
               
    #endif

	DisplaySuccessFailure((TrampleDelayTimeBackup == RamSetParameters.TrampleDelayTime) ? 1:0);

	Jump2Menu(SetTrampleDelayTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//���ò�̤��ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//���ò�̤��ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetTrampleDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//���ò�̤��ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetTrampleDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetTrampleDelayTimeEscUp(void)
{
	
}
void DispMenuSetTrampleDelayTimeEscDown(void)
{

}
void DispMenuSetTrampleDelayTimeEscOk(void)
{
	
}	


//------------------------------------------------------------------		
//����ͬʱ��̤������ʼ��
//------------------------------------------------------------------
void DispMenuSetTrampleNumberInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00");
		my_strcpy(ParaSet.MaxText, "99");
		
		snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", RamSetParameters.TrampleNumber);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(TrampleNumber_PosX , TrampleNumber_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//����ͬʱ��̤�������
//------------------------------------------------------------------
void DispMenuSetTrampleNumberUp(void)
{
	U16 TrampleNumberBackup;
	
	RamSetParameters.TrampleNumber=atoi((char *)ParaSet.Text);

	TrampleNumberBackup = RamSetParameters.TrampleNumber;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber);     
       FLASH_READ(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleNumber-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.TrampleNumber,//��д�����ݻ�����
                   (sizeof(RamSetParameters.TrampleNumber) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.TrampleNumber-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.TrampleNumber,//��д�����ݻ�����
                   (sizeof(RamSetParameters.TrampleNumber) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((TrampleNumberBackup == RamSetParameters.TrampleNumber) ? 1:0);

	Jump2Menu(SetTrampleNumberIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//����ͬʱ��̤��������
//------------------------------------------------------------------
void DispMenuSetTrampleNumberDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//����ͬʱ��̤�������ּ�
//------------------------------------------------------------------
void DispMenuSetTrampleNumberLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetTrampleNumberIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//����ͬʱ��̤�����������
//------------------------------------------------------------------
void DispMenuSetTrampleNumberRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetTrampleNumberIndex,FlashMode_ReFlash);
}


void DispMenuSetTrampleNumberEscUp(void)
{
	
}
void DispMenuSetTrampleNumberEscDown(void)
{

}
void DispMenuSetTrampleNumberEscOk(void)
{
	
}	


//------------------------------------------------------------------		
//����������ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.SyllableDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(SyllableDelayTime_PosX , SyllableDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_MS_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//����������ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeUp(void)
{
	U16 SyllableDelayTimeBackup;
	
	RamSetParameters.SyllableDelayTime=atoi((char *)ParaSet.Text);

	SyllableDelayTimeBackup = RamSetParameters.SyllableDelayTime;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.SyllableDelayTime,RamSetParameters.SyllableDelayTime,RamSetParameters.SyllableDelayTime);     
       FLASH_READ(RamSetParameters.SyllableDelayTime,RamSetParameters.SyllableDelayTime,RamSetParameters.SyllableDelayTime); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.SyllableDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.SyllableDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.SyllableDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.SyllableDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.SyllableDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.SyllableDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((SyllableDelayTimeBackup == RamSetParameters.SyllableDelayTime) ? 1:0);

	Jump2Menu(SetSyllableDelayTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//����������ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//����������ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetSyllableDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//����������ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetSyllableDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetSyllableDelayTimeEscUp(void)
{
	
}
void DispMenuSetSyllableDelayTimeEscDown(void)
{

}
void DispMenuSetSyllableDelayTimeEscOk(void)
{
	
}	


//------------------------------------------------------------------		
//����������ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.NoteDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(NoteDelayTime_PosX , NoteDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_MS_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//����������ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeUp(void)
{
	U16 NoteDelayTimeBackup;
	
	RamSetParameters.NoteDelayTime=atoi((char *)ParaSet.Text);

	NoteDelayTimeBackup = RamSetParameters.NoteDelayTime;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime);     
       FLASH_READ(RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.NoteDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.NoteDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.NoteDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.NoteDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.NoteDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.NoteDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif

 
	DisplaySuccessFailure((NoteDelayTimeBackup == RamSetParameters.NoteDelayTime) ? 1:0);

	Jump2Menu(SetNoteDelayTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//����������ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//����������ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetNoteDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//����������ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetNoteDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetNoteDelayTimeEscUp(void)
{
	
}
void DispMenuSetNoteDelayTimeEscDown(void)
{

}
void DispMenuSetNoteDelayTimeEscOk(void)
{
	
}






//------------------------------------------------------------------		
//����״̬���ó�ʼ��
//------------------------------------------------------------------
void DispMenuMusicStatusSetInit(void)
{ 
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.DeviceStatus.MusicStatus;
        Where = 0;
		ItemNum = (*Menu_OnOff).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_ResIndex[language].Text+2),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_OnOff[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
    //OnOffStatusSetInit(DeviceStatus.MusicStatus);
}

//------------------------------------------------------------------		
//����״̬�������
//------------------------------------------------------------------
void DispMenuMusicStatusSetUp(void)
{
	RamSetParameters.DeviceStatus.MusicStatus = Item;
    
	#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus);     
       FLASH_READ(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceStatus-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.DeviceStatus,//��д�����ݻ�����
                   (sizeof(RamSetParameters.DeviceStatus) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceStatus-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.DeviceStatus,//��д�����ݻ�����
                   (sizeof(RamSetParameters.DeviceStatus) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif
    
	DisplaySuccessFailure((RamSetParameters.DeviceStatus.MusicStatus == Item) ? 1:0);
	
	Jump2Menu(MusicStatusSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//����״̬���÷���
//------------------------------------------------------------------
void DispMenuMusicStatusSetDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//����״̬����ѡ���
//------------------------------------------------------------------
void DispMenuMusicStatusSetLeft(void)
{
	MenuItemAdd();
	Jump2Menu(MusicStatusSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//����״̬����ѡ���
//------------------------------------------------------------------
void DispMenuMusicStatusSetRight(void)  
{
	MenuItemSub();
	Jump2Menu(MusicStatusSetIndex,FlashMode_ReFlash);
}



void DispMenuMusicStatusSetEscUp(void)
{
	
}
void DispMenuMusicStatusSetEscDown(void)
{
	
}
void DispMenuMusicStatusSetEscOk(void)
{
	
}
 
 
//------------------------------------------------------------------		
//�ƹ�״̬���ó�ʼ��
//------------------------------------------------------------------
void DispMenuLightsStatusSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.DeviceStatus.LightsStatus;
        Where = 0;
		ItemNum = (*Menu_OnOff).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_ResIndex[language].Text+3),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_OnOff[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
	//OnOffStatusSetInit(DeviceStatus.LightsStatus);	
}

//------------------------------------------------------------------		
//�ƹ�״̬�������
//------------------------------------------------------------------
void DispMenuLightsStatusSetUp(void)
{
	RamSetParameters.DeviceStatus.LightsStatus = Item;


    #ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus);     
       FLASH_READ(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceStatus-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.DeviceStatus,//��д�����ݻ�����
                   (sizeof(RamSetParameters.DeviceStatus) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceStatus-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.DeviceStatus,//��д�����ݻ�����
                   (sizeof(RamSetParameters.DeviceStatus) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif

	DisplaySuccessFailure((RamSetParameters.DeviceStatus.LightsStatus == Item) ? 1:0);

	Jump2Menu(LightsStatusSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�ƹ�״̬���÷���
//------------------------------------------------------------------
void DispMenuLightsStatusSetDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//�ƹ�״̬����ѡ���
//------------------------------------------------------------------

void DispMenuLightsStatusSetLeft(void)
{
	MenuItemAdd();
	Jump2Menu(LightsStatusSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�ƹ�״̬����ѡ���
//------------------------------------------------------------------
void DispMenuLightsStatusSetRight(void)
{
	MenuItemSub();
	Jump2Menu(LightsStatusSetIndex,FlashMode_ReFlash);
}
void DispMenuLightsStatusSetEscUp(void)
{
	
}
void DispMenuLightsStatusSetEscDown(void)
{
	
}
void DispMenuLightsStatusSetEscOk(void)
{
	
}






//------------------------------------------------------------------		
//�������ó�ʼ��
//------------------------------------------------------------------
void DispMenuFactorySetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item =0;
        Where = 0;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Tip[language].Text+4),0,3,126,GUI_TA_HCENTER);	
	
}

//------------------------------------------------------------------		
//�����������
//------------------------------------------------------------------
void DispMenuFactorySetUp(void)
{
	ResetParametersData();
	DisplaySuccessFailure(1);
	POP();
	Jump2Menu(MenuFuncIndex,FlashMode_ManualInit);
}


//------------------------------------------------------------------		
//�������÷���
//------------------------------------------------------------------
void DispMenuFactorySetDown(void)
{
	POP();
}


void DispMenuFactorySetLeft(void)
{
	
}
void DispMenuFactorySetRight(void)
{
	
}
void DispMenuFactorySetEscUp(void)
{
	
}
void DispMenuFactorySetEscDown(void)
{
	
}
void DispMenuFactorySetEscOk(void)
{
	
}





//------------------------------------------------------------------		
//�ƴ�ģʽ���ó�ʼ��
//------------------------------------------------------------------
void DispMenuLightsModeSetInit(void)
{ 
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.LightsMode;
        Where = 0;
		ItemNum = (*Menu_LightsMode).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+9),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_LightsMode[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
    //OnOffStatusSetInit(DeviceStatus.MusicStatus);
}

//------------------------------------------------------------------		
////�ƴ�ģʽ�������
//------------------------------------------------------------------
void DispMenuLightsModeSetUp(void)
{
	RamSetParameters.LightsMode = Item;
    
	#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.LightsMode,RamSetParameters.LightsMode,RamSetParameters.LightsMode);     
       FLASH_READ(RamSetParameters.LightsMode,RamSetParameters.LightsMode,RamSetParameters.LightsMode); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsMode-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsMode,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsMode) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsMode-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsMode,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsMode) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif
    
	DisplaySuccessFailure((RamSetParameters.LightsMode == Item) ? 1:0);
	
	Jump2Menu(LightsModeSetIndex,FlashMode_ReFlash);
}



//------------------------------------------------------------------		
////�ƴ�ģʽ���÷���
//------------------------------------------------------------------
void DispMenuLightsModeSetDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//�ƴ�ģʽ����ѡ���
//------------------------------------------------------------------
void DispMenuLightsModeSetLeft(void)
{
	MenuItemAdd();
	Jump2Menu(LightsModeSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�ƴ�ģʽ����ѡ���
//------------------------------------------------------------------
void DispMenuLightsModeSetRight(void)  
{
	MenuItemSub();
	Jump2Menu(LightsModeSetIndex,FlashMode_ReFlash);
}



void DispMenuLightsModeSetEscUp(void)
{
	
}
void DispMenuLightsModeSetEscDown(void)
{
	
}
void DispMenuLightsModeSetEscOk(void)
{
	
}







//------------------------------------------------------------------		
//�ƴ��ٶ����ó�ʼ��
//------------------------------------------------------------------
void DispMenuLightsSpeedSetInit(void)
{ 
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.LightsSpeed;
        Where = 0;
		ItemNum = (*Menu_LightsSpeed).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+10),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_LightsSpeed[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
    //OnOffStatusSetInit(DeviceStatus.MusicStatus);
}

//------------------------------------------------------------------		
//�ƴ��ٶ��������
//------------------------------------------------------------------
void DispMenuLightsSpeedSetUp(void)
{
	RamSetParameters.LightsSpeed = Item;
    
	#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.LightsSpeed,RamSetParameters.LightsSpeed,RamSetParameters.LightsSpeed);     
       FLASH_READ(RamSetParameters.LightsSpeed,RamSetParameters.LightsSpeed,RamSetParameters.LightsSpeed); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsSpeed-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsSpeed,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsSpeed) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsSpeed-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsSpeed,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsSpeed) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif
    
	DisplaySuccessFailure((RamSetParameters.LightsSpeed == Item) ? 1:0);
	
	Jump2Menu(LightsSpeedSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�ƴ��ٶ����÷���
//------------------------------------------------------------------
void DispMenuLightsSpeedSetDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//�ƴ��ٶ�����ѡ���
//------------------------------------------------------------------
void DispMenuLightsSpeedSetLeft(void)
{
	MenuItemAdd();
	Jump2Menu(LightsSpeedSetIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�ƴ��ٶ�����ѡ���
//------------------------------------------------------------------
void DispMenuLightsSpeedSetRight(void)  
{
	MenuItemSub();
	Jump2Menu(LightsSpeedSetIndex,FlashMode_ReFlash);
}



void DispMenuLightsSpeedSetEscUp(void)
{
	
}
void DispMenuLightsSpeedSetEscDown(void)
{
	
}
void DispMenuLightsSpeedSetEscOk(void)
{
	
}



//------------------------------------------------------------------		
//�ƴ�������ó�ʼ��
//------------------------------------------------------------------
void DispMenuLightsSpacingSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0");
		my_strcpy(ParaSet.MaxText, "9");
		
		snprintf((char*)&ParaSet.Text[0], 1+1,"%01d", RamSetParameters.LightsSpacing);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(LightsSpacing_PosX , LightsSpacing_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//�ƴ�����������
//------------------------------------------------------------------
void DispMenuLightsSpacingSetUp(void)
{
	U16 LightsSpacingBackup;
	
	RamSetParameters.LightsSpacing=atoi((char *)ParaSet.Text);

	LightsSpacingBackup = RamSetParameters.LightsSpacing;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime);     
       FLASH_READ(RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime,RamSetParameters.NoteDelayTime); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsSpacing-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsSpacing,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsSpacing) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LightsSpacing-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LightsSpacing,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LightsSpacing) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif

 
	DisplaySuccessFailure((LightsSpacingBackup == RamSetParameters.LightsSpacing) ? 1:0);

	Jump2Menu(LightsSpacingSetIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//�ƴ�������÷���
//------------------------------------------------------------------
void DispMenuLightsSpacingSetDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//�ƴ�����������ּ�
//------------------------------------------------------------------
void DispMenuLightsSpacingSetLeft(void)
{
	ParaSetAdd();
	Jump2Menu(LightsSpacingSetIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//�ƴ�������ù������
//------------------------------------------------------------------
void DispMenuLightsSpacingSetRight(void) 
{
	ParaSetSub();
	
	//ParaSetCursorRightMoves();
	Jump2Menu(LightsSpacingSetIndex,FlashMode_ReFlash);
}



void DispMenuLightsSpacingSetEscUp(void)
{
	
}
void DispMenuLightsSpacingSetEscDown(void)
{
	
}
void DispMenuLightsSpacingSetEscOk(void)
{
	
}



//------------------------------------------------------------------		
//��Ϸģʽѡ���ܺ���
//------------------------------------------------------------------
void DispMenuGameModeSelectInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//-----------------
    BarMenu = (struct Menu *)&Menu_GameModeSelect[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ���˵�����

    //��ʾ������ʾ          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.GameModeSelect;
        Where = Item;
    }
    //������ʼ������
    BarMenuInit();
}


void DispMenuGameModeSelectUp(void)
{
	
	
	u8 GameModeSelectBackup;
//-----------
    PUSH();//
//-------------------
    GUI_Clear();
	
	
	GameModeSelectBackup=Item;
    RamSetParameters.GameModeSelect=GameModeSelectBackup;
	
	
#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameModeSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameModeSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameModeSelect) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameModeSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameModeSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameModeSelect) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
    //GUI_Clear();
	DisplaySuccessFailure((RamSetParameters.GameModeSelect == GameModeSelectBackup) ? 1:0);
//-------------------
    POP();
//-------------------
}


void DispMenuGameModeSelectDown(void)
{
	POP();
}
void DispMenuGameModeSelectLeft(void)
{
	BarMenuLeft();
}
void DispMenuGameModeSelectRight(void)
{
	BarMenuRight();
}
void DispMenuGameModeSelectEscUp(void)
{
	
}
void DispMenuGameModeSelectEscDown(void)
{
	
}
void DispMenuGameModeSelectEscOk(void)
{
	
}



//------------------------------------------------------------------		
//��Ϸģʽ���ù��ܺ���
//------------------------------------------------------------------
void DispMenuGameModeSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_GameModeSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

	
    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
	
	 if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item = 0;
        Where = Item;
    }
	
    //������ʼ������
    BarMenuInit();

}


void DispMenuGameModeSetUp(void)
{

	switch(Item){
	  case 0:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(RowNumberSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  case 1:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(RowsDeviceNumberSetIndex,FlashMode_AutoInit);
		  break;
	  }

	  case 2:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(SetBeepRowNumIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  
	  case 3:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(GamePassTimeIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  
	  case 4:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(VoicePromptTimeIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  
	  
	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}


void DispMenuGameModeSetDown(void)
{
	POP();
}
void DispMenuGameModeSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuGameModeSetRight(void)
{
	BarMenuRight();
}
void DispMenuGameModeSetEscUp(void)
{
	
}
void DispMenuGameModeSetEscDown(void)
{
	
}
void DispMenuGameModeSetEscOk(void)
{
	
}


//------------------------------------------------------------------		
//�������ù��ܺ���
//------------------------------------------------------------------
void DispMenuRowNumberSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.MaxRowNumber);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(RowNumberSet_PosX , RowNumberSet_PosY, 126, GUI_TA_HCENTER);
}
void DispMenuRowNumberSetUp(void)
{
	U16 MaxRowNumberBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	MaxRowNumberBackup=atoi((char *)ParaSet.Text);

	if (MaxRowNumberBackup >MAX_ROW_NUMBER)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.MaxRowNumber=MaxRowNumberBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.MaxRowNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.MaxRowNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.MaxRowNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.MaxRowNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.MaxRowNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.MaxRowNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((MaxRowNumberBackup == RamSetParameters.MaxRowNumber) ? 1:0);
	}
	Jump2Menu(RowNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuRowNumberSetDown(void)
{
	POP();
}
void DispMenuRowNumberSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(RowNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuRowNumberSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(RowNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuRowNumberSetEscUp(void)
{
	
}
void DispMenuRowNumberSetEscDown(void)
{
	
}
void DispMenuRowNumberSetEscOk(void)
{
	
}

//------------------------------------------------------------------		
//�����豸�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuRowsDeviceNumberSetInit(void)
{
	char StringBuff[5];
	u16 RowNumber;
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000 --> 0");
		my_strcpy(ParaSet.MaxText, "999 --> 9");
		
		//snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.DeviceIdToChar[0].RowNumber);
		ParaSet.Text[0]='0';
		ParaSet.Text[1]='0';
		ParaSet.Text[2]='1';
		ParaSet.Text[3]=' ';
		ParaSet.Text[4]='-';
		ParaSet.Text[5]='-';
		ParaSet.Text[6]='>';
		ParaSet.Text[7]=' ';		
		snprintf((char*)&ParaSet.Text[8], 1+1,"%01d", RamSetParameters.RowsDeviceNumber[0]);
		ParaSet.Text[9]='\0';
		ParaSet.CursorPos=0;
	}
	
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos != 8))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		RowNumber=atoi((char *)ParaSet.Text);
		if (RowNumber >MAX_DEVICE_NUM)
		{
			RowNumber =MAX_DEVICE_NUM;
			snprintf((char*)&ParaSet.Text, 1+1,"%03d", RowNumber);
			ParaSet.Text[3]=' ';
		}
		snprintf((char*)&ParaSet.Text[8], 1+1,"%01d", RamSetParameters.RowsDeviceNumber[RowNumber]);
		ParaSet.Text[9]='\0';
		
	}
	
	
	
	GUI_Clear();
	//GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	
	GUI_DispStringAtBar((U8 *)"�ź�  �豸��",0,1,126,GUI_TA_HCENTER);
	
	ParaSetDisplsy(RowsDeviceNumberSet_PosX , RowsDeviceNumberSet_PosY, 126, GUI_TA_HCENTER);
}
void DispMenuRowsDeviceNumberSetUp(void)
{
	u8 RowNumber;
	u8 RowsDeviceNumber;
	
	char StringBuff[5];
	
	
	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	StringBuff[3]='\0';
	RowNumber=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[8],1);
	StringBuff[1]='\0';
	RowsDeviceNumber=atoi(StringBuff);
	
	

	if (RowNumber >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		RamSetParameters.RowsDeviceNumber[RowNumber]=RowsDeviceNumber;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.RowsDeviceNumber[RowNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.RowsDeviceNumber[RowNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.RowsDeviceNumber[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.RowsDeviceNumber[RowNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.RowsDeviceNumber[RowNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.RowsDeviceNumber[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure(((RowsDeviceNumber == RamSetParameters.RowsDeviceNumber[RowNumber])) ? 1:0);
	}
	Jump2Menu(RowsDeviceNumberSetIndex,FlashMode_ReFlash);
}



void DispMenuRowsDeviceNumberSetDown(void)
{
	POP();
}
void DispMenuRowsDeviceNumberSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(RowsDeviceNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuRowsDeviceNumberSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(RowsDeviceNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuRowsDeviceNumberSetEscUp(void)
{
	
}
void DispMenuRowsDeviceNumberSetEscDown(void)
{
	
}
void DispMenuRowsDeviceNumberSetEscOk(void)
{
	
}





//------------------------------------------------------------------		
//��ͯģʽ���ù��ܺ���
//------------------------------------------------------------------
void DispMenuBabyModeSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_BabyModeSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
	
	if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item = 0;
        Where = Item;
    }
	
    //������ʼ������
    BarMenuInit();
	
	

}


void DispMenuBabyModeSetUp(void)
{

	switch(Item){
	  case 0:{//Jump2Menu
		  //GUI_Clear();  
		  //PUSH();
		  Jump2Menu(SayModeSelectIndex,FlashMode_ManualInit);
		  break;
	  }
	  case 1:{//PUSH  -->POP
		  //PUSH();
		  Jump2Menu(SayModeSetIndex,FlashMode_ManualInit);
		  break;
	  }

	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}



void DispMenuBabyModeSetDown(void)
{
	POP();
}
void DispMenuBabyModeSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuBabyModeSetRight(void)
{
	BarMenuRight();
}
void DispMenuBabyModeSetEscUp(void)
{
	
}
void DispMenuBabyModeSetEscDown(void)
{
	
}
void DispMenuBabyModeSetEscOk(void)
{
	
}



//------------------------------------------------------------------		
//����ģʽѡ���ܺ���
//------------------------------------------------------------------
void DispMenuSayModeSelectInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//-----------------
    BarMenu = (struct Menu *)&Menu_SayModeSelect[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ���˵�����

    //��ʾ������ʾ          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.SayModeSelect;
        Where = Item;
    }
    //������ʼ������
    BarMenuInit();
}


void DispMenuSayModeSelectUp(void)
{
	
	
	u8 SayModeSelectBackup;
//-----------
    PUSH();//
//-------------------
    GUI_Clear();
	
	
	SayModeSelectBackup=Item;
    RamSetParameters.SayModeSelect=SayModeSelectBackup;
	
	
#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.SayModeSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.SayModeSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.SayModeSelect) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.SayModeSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.SayModeSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.SayModeSelect) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
    //GUI_Clear();
	DisplaySuccessFailure((RamSetParameters.SayModeSelect == SayModeSelectBackup) ? 1:0);
//-------------------
    POP();
//-------------------
}


void DispMenuSayModeSelectDown(void)
{
	POP();
}
void DispMenuSayModeSelectLeft(void)
{
	BarMenuLeft();
}
void DispMenuSayModeSelectRight(void)
{
	BarMenuRight();
}
void DispMenuSayModeSelectEscUp(void)
{
	
}
void DispMenuSayModeSelectEscDown(void)
{
	
}
void DispMenuSayModeSelectEscOk(void)
{
	
}



//------------------------------------------------------------------		
//����ģʽ���ù��ܺ���
//------------------------------------------------------------------
void DispMenuSayModeSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_SayModeSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
	
	if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.SayModeSelect;
        Where = Item;
    }
	
    //������ʼ������
    BarMenuInit();

}


void DispMenuSayModeSetUp(void)
{

	switch(Item){
	  case 0:{//Jump2Menu
		  //GUI_Clear();  
		  PUSH();
		  Jump2Menu(EnglishCharSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  case 1:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(ChinesePinyinSetIndex,FlashMode_AutoInit);
		  break;
	  }

	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}




void DispMenuSayModeSetDown(void)
{
	POP();
}
void DispMenuSayModeSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuSayModeSetRight(void)
{
	BarMenuRight();
}
void DispMenuSayModeSetEscUp(void)
{
	
}
void DispMenuSayModeSetEscDown(void)
{
	
}
void DispMenuSayModeSetEscOk(void)
{
	
}

#if 1
U8 * _CONST_ DeviceIdToEnglishChar[]=
{
	{"A"},{"B"},{"C"},{"D"},{"E"},{"F"},{"G"},{"H"},{"I"},{"J"},//Ӣ����ĸ
	{"K"},{"L"},{"M"},{"N"},{"O"},{"P"},{"Q"},{"R"},{"S"},{"T"},
	{"U"},{"V"},{"W"},{"X"},{"Y"},{"Z"},
};
#endif
//------------------------------------------------------------------		
//Ӣ����ĸ���ù��ܺ���
//------------------------------------------------------------------
void DispMenuEnglishCharSetInit(void)
{
	char StringBuff[5];
	U16 DeviceNumber;
	static U16 DeviceNumberBackup;
	
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000 --> 0");
		my_strcpy(ParaSet.MaxText, "999 --> x");
		
		ParaSet.MinText[8]=0;
		ParaSet.MaxText[8]=(sizeof(DeviceIdToEnglishChar)/sizeof(U8 *))-1;
		
		DeviceNumber=1;
		DeviceNumberBackup=DeviceNumber;
		
		//snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.DeviceIdToChar[0].DeviceId);
		ParaSet.Text[0]='0';
		ParaSet.Text[1]='0';
		ParaSet.Text[2]='1';
		ParaSet.Text[3]=' ';
		ParaSet.Text[4]='-';
		ParaSet.Text[5]='-';
		ParaSet.Text[6]='>';
		ParaSet.Text[7]=' ';		
		ParaSet.Text[8]= RamSetParameters.DeviceIdToEnglishChar[DeviceNumber];
		ParaSet.Text[9]='\0';
		ParaSet.CursorPos=0;
		
	}
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos != 8))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		DeviceNumber=atoi((char *)ParaSet.Text);
		
		
		if (DeviceNumber >MAX_DEVICE_NUM)
		{
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			DeviceNumber=DeviceNumberBackup;
			
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", DeviceNumber);
			ParaSet.Text[3]=' ';
			
		}
		
		if (DeviceNumberBackup != DeviceNumber)
		{	
			DeviceNumberBackup=DeviceNumber;
			ParaSet.Text[8]=RamSetParameters.DeviceIdToEnglishChar[DeviceNumber];
		}
		//my_strcpy(StringBuff,DeviceIdToEnglishChar[ParaSet.Text[8]]);
	}
	GUI_Clear();
	//GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar((U8 *)"�豸��  �ַ�",0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(EnglishCharSet_PosX , EnglishCharSet_PosY, 126, GUI_TA_HCENTER);
	
	if (ParaSet.CursorPos==8)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
	}
	GUI_DispStringAt(DeviceIdToEnglishChar[ParaSet.Text[8]], EnglishCharSet_PosX+64, EnglishCharSet_PosY);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
}



void DispMenuEnglishCharSetUp(void)
{
	U16 DeviceNumber;
	
	char StringBuff[5];
	
	
	my_strncpy(StringBuff,ParaSet.Text,3);
	StringBuff[3]='\0';
	DeviceNumber=atoi((char *)ParaSet.Text);

	if (DeviceNumber >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		RamSetParameters.DeviceIdToEnglishChar[DeviceNumber]=ParaSet.Text[8];
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceIdToEnglishChar[DeviceNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceIdToEnglishChar[DeviceNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceIdToEnglishChar[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceIdToEnglishChar[DeviceNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceIdToEnglishChar[DeviceNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceIdToEnglishChar[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure(((ParaSet.Text[8] == RamSetParameters.DeviceIdToEnglishChar[DeviceNumber])) ? 1:0);
	}
	Jump2Menu(EnglishCharSetIndex,FlashMode_ReFlash);
}



void DispMenuEnglishCharSetDown(void)
{
	POP();
}
void DispMenuEnglishCharSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(EnglishCharSetIndex,FlashMode_ReFlash);
}
void DispMenuEnglishCharSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(EnglishCharSetIndex,FlashMode_ReFlash);
}
void DispMenuEnglishCharSetEscUp(void)
{
	
}
void DispMenuEnglishCharSetEscDown(void)
{
	
}
void DispMenuEnglishCharSetEscOk(void)
{
	
}

#if 1
U8 * _CONST_ DeviceIdToChinesePinyin[]=
{	
	//��ĸ 23
	{"b"},/*[��]*/
	{"p"},/*[��]*/
	{"m"},/*[��]*/
	{"f"},/*[��]*/
	{"d"},/*[��]*/ 
	{"t"},/*[��]*/ 
	{"n"},/*[ګ]*/ 
	{"l"},/*[��]*/ 
	{"g"},/*[��]*/ 
	{"k"},/*[��]*/ 
	{"h"},/*[��]*/
	{"j"},/*[��]*/ 
	{"q"},/*[��]*/ 
	{"x"},/*[ϣ]*/        
	{"z"},/*[��]*/
	{"c"},/*[��]*/
	{"s"},/*[˼]*/ 
	{"r"},/*[��]*/ 
	{"zh"},/*[֪]*/ 
	{"ch"},/*[��]*/ 
	{"sh"},/*[ʫ]*/
	{"y"},/*[ҽ]*/ 
	{"w"},/*[��]*/

	//�� �� ƴ �� �� ĸ �� 33
	
	{"a"},/*[��]*/ 
	{"an"},/*[��]*/ 
	{"ao"},/*[��]*/ 
	{"ai"},/*[��]*/ 
	{"ang"},/*[��]*/
	{"o"},/*[�]*/ 
	{"ong"},/*[��]*/ 
	{"ou"},/*[ŷ]*/
	{"e"},/*[��]*/ 
	{"en"},/*[��]*/ 
	{"er"},/*[��]*/ 
	{"ei"},/*[��]*/ 
	{"eng"},/*[��]*/
	{"i"},/*[��]*/ 
	{"ia"},/*[ѽ]*/ 
	{"iu"},/*[��]*/ 
	{"ie"},/*[Ү]*/ 
	{"in"},/*[��]*/ 
	{"ing"},/*[Ӣ]*/
	{"u"},/*[��]*/ 
	{"un"},/*[��]*/ 
	{"ua"},/*[��]*/ 
	{"uo"},/*[��]*/ 
	{"ue"},/*[��]*/ 
	{"ui"},/*[��]*/
	{"��"},/*[��]*/
	{"iao"},/*[��]*/ 
	{"ian"},/*[��]*/ 
	{"iang"},/*[��]*/ 
	{"iong"},/*[��]*/
	{"uai"},/*[��]*/ 
	{"uan"},/*[��]*/ 
	{"uang"},/*[��]*/

	//�����϶����� 16
	{"zhi"}, /*֯*/
	{"chi"}, /*��*/
	{"shi"}, /*ʨ*/
	{"ri"},  /*��*/
	{"zi"},  /*��*/
	{"ci"},  /*��*/
	{"si"},  /*˿*/
	{"yi"},  /*��*/
	{"wu"},  /*��*/
	{"yu"},  /*��*/
	{"ye"},  /*Ҭ*/
	{"yue"}, /*��*/
	{"yin"}, /*Բ*/
	{"yun"}, /*ӡ*/
	{"yuan"},/*��*/ 
	{"ying"},/*ӥ*/

};
#endif

#if 1
U8 * _CONST_ DeviceIdToChinesePinyin_Chinese[]=
{	
	//��ĸ 23
	{"��"},/*[b]*/
	{"��"},/*[p]*/
	{"��"},/*[m]*/
	{"��"},/*[f]*/
	{"��"},/*[d]*/ 
	{"��"},/*[t]*/ 
	{"ګ"},/*[n]*/ 
	{"��"},/*[l]*/ 
	{"��"},/*[g]*/ 
	{"��"},/*[k]*/ 
	{"��"},/*[h]*/
	{"��"},/*[j]*/ 
	{"��"},/*[q]*/ 
	{"ϣ"},/*[x]*/        
	{"��"},/*[z]*/
	{"��"},/*[c]*/
	{"˼"},/*[s]*/ 
	{"��"},/*[r]*/ 
	{"֪"},/*[zh]*/ 
	{"��"},/*[ch]*/ 
	{"ʫ"},/*[sh]*/
	{"ҽ"},/*[y]*/ 
	{"��"},/*[w]*/

	//�� �� ƴ �� �� ĸ �� 33
	
	{"��"},/*[a]*/ 
	{"��"},/*[an]*/ 
	{"��"},/*[ao]*/ 
	{"��"},/*[ai]*/ 
	{"��"},/*[ang]*/
	{"�"},/*[o]*/ 
	{"��"},/*[ong]*/ 
	{"ŷ"},/*[ou]*/
	{"��"},/*[e]*/ 
	{"��"},/*[en]*/ 
	{"��"},/*[er]*/ 
	{"��"},/*[ei]*/ 
	{"��"},/*[eng]*/
	{"��"},/*[i]*/ 
	{"ѽ"},/*[ia]*/ 
	{"��"},/*[iu]*/ 
	{"Ү"},/*[ie]*/ 
	{"��"},/*[in]*/ 
	{"Ӣ"},/*[ing]*/
	{"��"},/*[u]*/ 
	{"��"},/*[un]*/ 
	{"��"},/*[ua]*/ 
	{"��"},/*[uo]*/ 
	{"��"},/*[ue]*/ 
	{"��"},/*[ui]*/
	{"��"},/*[��]*/
	{"��"},/*[iao]*/ 
	{"��"},/*[ian]*/ 
	{"��"},/*[iang]*/ 
	{"��"},/*[iong]*/
	{"��"},/*[uai]*/ 
	{"��"},/*[uan]*/ 
	{"��"},/*[uang]*/

	//�����϶����� 16
	{"֯"},  /*[zhi]*/
	{"��"},  /*[chi]*/
	{"ʨ"},  /*[shi]*/
	{"��"},  /*[ri]*/
	{"��"},  /*[zi]*/
	{"��"},  /*[ci]*/
	{"˿"},  /*[si]*/
	{"��"},  /*[yi]*/
	{"��"},  /*[wu]*/
	{"��"},  /*[yu]*/
	{"Ҭ"},  /*[ye]*/
	{"��"},  /*[yue]*/
	{"Բ"},  /*[yuan]*/
	{"ӡ"},  /*[yin]*/
	{"��"},  /*[yun]*/
	{"ӥ"},  /*[ying]*/

};
#endif
//------------------------------------------------------------------		
//����ƴ�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuChinesePinyinSetInit(void)
{
	char StringBuff[5];
	
	U16 DeviceNumber;
	static U16 DeviceNumberBackup;
	
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000 --> 0");
		my_strcpy(ParaSet.MaxText, "299 --> x");
		
		ParaSet.MinText[8]=0;
		ParaSet.MaxText[8]=(sizeof(DeviceIdToChinesePinyin)/sizeof(U8 *))-1;
		
		DeviceNumber=1;
		DeviceNumberBackup=DeviceNumber;
		
		
		//snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.DeviceIdToChar[0].DeviceId);
		ParaSet.Text[0]='0';
		ParaSet.Text[1]='0';
		ParaSet.Text[2]='1';
		ParaSet.Text[3]=' ';
		ParaSet.Text[4]='-';
		ParaSet.Text[5]='-';
		ParaSet.Text[6]='>';
		ParaSet.Text[7]=' ';		
		ParaSet.Text[8]=RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber];
		ParaSet.Text[9]='\0';
		ParaSet.CursorPos=0;
	}
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos != 8))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		DeviceNumber=atoi((char *)ParaSet.Text);
		if (DeviceNumber >MAX_DEVICE_NUM)
		{
			
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			DeviceNumber=DeviceNumberBackup;
			
			//DeviceNumber =MAX_DEVICE_NUM;
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", DeviceNumber);
			ParaSet.Text[3]=' ';
			
		}
		
		if (DeviceNumberBackup != DeviceNumber)
		{	
			DeviceNumberBackup=DeviceNumber;
			ParaSet.Text[8]=RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber];
		}
		
		
		
		//ParaSet.Text[8]=RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber];
	}
	
	GUI_Clear();
	//GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	
	
	
	GUI_DispStringAtBar((U8 *)"�豸��  ƴ��",0,1,126,GUI_TA_HCENTER);
	
	#if 0
	if (/*((u8)(ParaSet.Text[8])>=0) &&*/ ((u8)(ParaSet.Text[8])<23))
		GUI_DispStringAtBar((U8 *)"�豸��  ��ĸ",0,1,126,GUI_TA_HCENTER);
	else if (((u8)(ParaSet.Text[8])>=23) && ((u8)(ParaSet.Text[8])<56))
		GUI_DispStringAtBar((U8 *)"�豸��  ��ĸ",0,1,126,GUI_TA_HCENTER);
	else if (((u8)(ParaSet.Text[8])>=56) && ((u8)(ParaSet.Text[8])<72))
		GUI_DispStringAtBar((U8 *)"�豸��  ����",0,1,126,GUI_TA_HCENTER);
	#endif
	
	ParaSetDisplsy(ChinesePinyinSet_PosX , ChinesePinyinSet_PosY, 126, GUI_TA_HCENTER);
	
	
	if (ParaSet.CursorPos==8)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
	}
	
	
	GUI_DispStringAt(DeviceIdToChinesePinyin[ParaSet.Text[8]], ChinesePinyinSet_PosX+64, ChinesePinyinSet_PosY);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	
}
void DispMenuChinesePinyinSetUp(void)
{
	U16 DeviceNumber;
	
	char StringBuff[5];
	
	
	my_strncpy(StringBuff,ParaSet.Text,3);
	StringBuff[3]='\0';
	DeviceNumber=atoi((char *)ParaSet.Text);

	if (DeviceNumber >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber]=ParaSet.Text[8];
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceIdToChinesePinyin[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.DeviceIdToChinesePinyin[0]) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure(((ParaSet.Text[8] == RamSetParameters.DeviceIdToChinesePinyin[DeviceNumber])) ? 1:0);
	}
	Jump2Menu(ChinesePinyinSetIndex,FlashMode_ReFlash);
}



void DispMenuChinesePinyinSetDown(void)
{
	POP();
}
void DispMenuChinesePinyinSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(ChinesePinyinSetIndex,FlashMode_ReFlash);
}
void DispMenuChinesePinyinSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(ChinesePinyinSetIndex,FlashMode_ReFlash);
}
void DispMenuChinesePinyinSetEscUp(void)
{
	
}
void DispMenuChinesePinyinSetEscDown(void)
{
	
}
void DispMenuChinesePinyinSetEscOk(void)
{
	
}






//------------------------------------------------------------------		
//ʱ�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuClockSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_ClockSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
    //������ʼ������
    BarMenuInit();

}


void DispMenuClockSetUp(void)
{

	switch(Item){
	  case 0:{//Jump2Menu
		  //GUI_Clear();
		  
		  
		  PUSH();
		  
		  Jump2Menu(TimeSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  case 1:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(DateSetIndex,FlashMode_AutoInit);
		  
		  break;
	  }

	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}



void DispMenuClockSetDown(void)
{
	POP();
}
void DispMenuClockSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuClockSetRight(void)
{
	BarMenuRight();
}
void DispMenuClockSetEscUp(void)
{
	
}
void DispMenuClockSetEscDown(void)
{
	
}
void DispMenuClockSetEscOk(void)
{
	
}



//------------------------------------------------------------------		
//ʱ�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuTimeSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00:00:00");
		my_strcpy(ParaSet.MaxText, "29:59:59");



		HAL_RTC_GetTime(&RtcHandle,&RTC_Time,RTC_FORMAT_BIN);
		snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", RTC_Time.Hours);
		ParaSet.Text[2]=':';
		snprintf((char*)&ParaSet.Text[3], 2+1,"%02d", RTC_Time.Minutes);
		ParaSet.Text[5]=':';
		snprintf((char*)&ParaSet.Text[6], 2+1,"%02d", RTC_Time.Seconds);

		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_ClockSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(TimeSet_PosX , TimeSet_PosY, 126, GUI_TA_HCENTER);
    //GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
	
}
void DispMenuTimeSetUp(void)
{
	char StringBuff[2+1]="**";

	
	my_strncpy(StringBuff,&ParaSet.Text[0],2);
	RTC_Time.Hours=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[3],2);
	RTC_Time.Minutes=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[6],2);
	RTC_Time.Seconds=atoi(StringBuff);

	HAL_RTC_SetTime(&RtcHandle,&RTC_Time,RTC_FORMAT_BIN);
	DisplaySuccessFailure(1);
	Jump2Menu(TimeSetIndex,FlashMode_ReFlash);
}
void DispMenuTimeSetDown(void)
{
	POP();
}
void DispMenuTimeSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(TimeSetIndex,FlashMode_ReFlash);
}
void DispMenuTimeSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(TimeSetIndex,FlashMode_ReFlash);
}
void DispMenuTimeSetEscUp(void)
{
	
}
void DispMenuTimeSetEscDown(void)
{
	
}
void DispMenuTimeSetEscOk(void)
{
	
}




//------------------------------------------------------------------		
//�������ù��ܺ���
//------------------------------------------------------------------
void DispMenuDateSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "2000/00/00");
		my_strcpy(ParaSet.MaxText, "2099/19/39");

		ParaSet.Text[0]='2';
		ParaSet.Text[1]='0';
		HAL_RTC_GetDate(&RtcHandle,&RTC_Date,RTC_FORMAT_BIN);
		snprintf((char*)&ParaSet.Text[2], 2+1,"%02d", RTC_Date.Year);
		ParaSet.Text[4]='/';
		snprintf((char*)&ParaSet.Text[5], 2+1,"%02d", RTC_Date.Month);
		ParaSet.Text[7]='/';
		snprintf((char*)&ParaSet.Text[8], 2+1,"%02d", RTC_Date.Date);
		
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_ClockSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(DateSet_PosX , DateSet_PosY, 126, GUI_TA_HCENTER);
}

void DispMenuDateSetUp(void)
{
	char StringBuff[2+1]="**";
	
	my_strncpy(StringBuff,&ParaSet.Text[0+2],2);
	RTC_Date.Year=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[3+2],2);
	RTC_Date.Month=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[6+2],2);
	RTC_Date.Date=atoi(StringBuff);

	
	RTC_Date.WeekDay=CaculateWeekDay(RTC_Date.Year+2000,RTC_Date.Month,RTC_Date.Date);
	
	HAL_RTC_SetDate(&RtcHandle,&RTC_Date,RTC_FORMAT_BIN);
	DisplaySuccessFailure(1);
	
	Jump2Menu(DateSetIndex,FlashMode_ReFlash);
}
void DispMenuDateSetDown(void)
{
	POP();
}
void DispMenuDateSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(DateSetIndex,FlashMode_ReFlash);
}
void DispMenuDateSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(DateSetIndex,FlashMode_ReFlash);
}
void DispMenuDateSetEscUp(void)
{
	
}
void DispMenuDateSetEscDown(void)
{
	
}
void DispMenuDateSetEscOk(void)
{
	
}



//------------------------------------------------------------------		
//���ڹ��ܺ���
//------------------------------------------------------------------
void DispMenuAboutInit(void)
{
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_Version[language].Text), 0, 3, 126, GUI_TA_HCENTER);
}
void DispMenuAboutUp(void)
{
	POP();
}
void DispMenuAboutDown(void)
{
	POP();
}
void DispMenuAboutLeft(void)
{

}
void DispMenuAboutRight(void)
{

}
void DispMenuAboutEscUp(void)
{
	
}
void DispMenuAboutEscDown(void)
{
	
}
void DispMenuAboutEscOk(void)
{
	
}






//------------------------------------------------------------------		
//CAN1�豸�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuCAN1DevicesTotalInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.CAN_DeviceTotal.CAN_Device1);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(CAN1DevicesTotalSet_PosX , CAN1DevicesTotalSet_PosY, 126, GUI_TA_HCENTER);
}
void DispMenuCAN1DevicesTotalUp(void)
{
	U16 CAN_DeviceTotalBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	CAN_DeviceTotalBackup=atoi((char *)ParaSet.Text);

	if (CAN_DeviceTotalBackup >CAN_MAX_DEVICE_NUMBER)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.CAN_DeviceTotal.CAN_Device1=CAN_DeviceTotalBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.CAN_DeviceTotal.CAN_Device1-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.CAN_DeviceTotal.CAN_Device1,//��д�����ݻ�����
					(sizeof(RamSetParameters.CAN_DeviceTotal.CAN_Device1) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.CAN_DeviceTotal.CAN_Device1-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.CAN_DeviceTotal.CAN_Device1,//��д�����ݻ�����
					(sizeof(RamSetParameters.CAN_DeviceTotal.CAN_Device1) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((CAN_DeviceTotalBackup == RamSetParameters.CAN_DeviceTotal.CAN_Device1) ? 1:0);
	}
	Jump2Menu(CAN1DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN1DevicesTotalDown(void)
{
	POP();
}
void DispMenuCAN1DevicesTotalLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(CAN1DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN1DevicesTotalRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(CAN1DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN1DevicesTotalEscUp(void)
{
	
}

void DispMenuCAN1DevicesTotalEscDown(void)
{
	
}
void DispMenuCAN1DevicesTotalEscOk(void)
{
	
}


//------------------------------------------------------------------		
//CAN2�豸�����ù��ܺ���
//------------------------------------------------------------------
void DispMenuCAN2DevicesTotalInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.CAN_DeviceTotal.CAN_Device2);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(CAN2DevicesTotalSet_PosX , CAN2DevicesTotalSet_PosY, 126, GUI_TA_HCENTER);
}
void DispMenuCAN2DevicesTotalUp(void)
{
	U16 CAN_DeviceTotalBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	CAN_DeviceTotalBackup=atoi((char *)ParaSet.Text);

	if (CAN_DeviceTotalBackup >CAN_MAX_DEVICE_NUMBER)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.CAN_DeviceTotal.CAN_Device2=CAN_DeviceTotalBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.CAN_DeviceTotal.CAN_Device2-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.CAN_DeviceTotal.CAN_Device2,//��д�����ݻ�����
					(sizeof(RamSetParameters.CAN_DeviceTotal.CAN_Device2) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.CAN_DeviceTotal.CAN_Device2-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.CAN_DeviceTotal.CAN_Device2,//��д�����ݻ�����
					(sizeof(RamSetParameters.CAN_DeviceTotal.CAN_Device2) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((CAN_DeviceTotalBackup == RamSetParameters.CAN_DeviceTotal.CAN_Device2) ? 1:0);
	}
	Jump2Menu(CAN2DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN2DevicesTotalDown(void)
{
	POP();
}
void DispMenuCAN2DevicesTotalLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(CAN2DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN2DevicesTotalRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(CAN2DevicesTotalIndex,FlashMode_ReFlash);
}
void DispMenuCAN2DevicesTotalEscUp(void)
{
	
}

void DispMenuCAN2DevicesTotalEscDown(void)
{
	
}
void DispMenuCAN2DevicesTotalEscOk(void)
{
	
}

//------------------------------------------------------------------		
//��Ϸѡ���ܺ���
//------------------------------------------------------------------
void DispMenuGameSelectInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//-----------------
    BarMenu = (struct Menu *)&Menu_GameSelect[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ���˵�����

    //��ʾ������ʾ          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.GameSelect;
        Where = Item;
    }
    //������ʼ������
    BarMenuInit();
}


void DispMenuGameSelectUp(void)
{
	
	
	u8 GameSelectBackup;
//-----------
    PUSH();//
//-------------------
    GUI_Clear();
	
	
	GameSelectBackup=Item;
    RamSetParameters.GameSelect=GameSelectBackup;
	
	
#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameSelect) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameSelect-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameSelect,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameSelect) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
    //GUI_Clear();
	DisplaySuccessFailure((RamSetParameters.GameSelect == GameSelectBackup) ? 1:0);
//-------------------
    POP();
//-------------------
}


void DispMenuGameSelectDown(void)
{
	POP();
}
void DispMenuGameSelectLeft(void)
{
	BarMenuLeft();
}
void DispMenuGameSelectRight(void)
{
	BarMenuRight();
}
void DispMenuGameSelectEscUp(void)
{
	
}
void DispMenuGameSelectEscDown(void)
{
	
}
void DispMenuGameSelectEscOk(void)
{
	
}







//------------------------------------------------------------------		
//�����Ϸ�������ݲ˵���ʼ��
//------------------------------------------------------------------
void DispMenuCleanRankingDataInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item =0;
        Where = 0;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Tip[language].Text+4),0,3,126,GUI_TA_HCENTER);	
	
}

//------------------------------------------------------------------		
//�����Ϸ�������ݲ˵����
//------------------------------------------------------------------
void DispMenuCleanRankingDataUp(void)
{
	GameRankingDataClean();
	DisplaySuccessFailure(1);
	POP();
	Jump2Menu(MenuFuncIndex,FlashMode_ManualInit);
}


//------------------------------------------------------------------		
//�����Ϸ�������ݲ˵�����
//------------------------------------------------------------------
void DispMenuCleanRankingDataDown(void)
{
	POP();
}


void DispMenuCleanRankingDataLeft(void)
{
	
}
void DispMenuCleanRankingDataRight(void)
{
	
}
void DispMenuCleanRankingDataEscUp(void)
{
	
}
void DispMenuCleanRankingDataEscDown(void)
{
	
}
void DispMenuCleanRankingDataEscOk(void)
{
	
}






//------------------------------------------------------------------		
//������ʾ���źų�ʼ��
//------------------------------------------------------------------
void DispMenuSetBeepRowNumInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.BeepRowNum);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(SetBeepRowNum_PosX , SetBeepRowNum_PosY, 126, GUI_TA_HCENTER);
}


//------------------------------------------------------------------		
//������ʾ���źų�ʼ��
//------------------------------------------------------------------
void DispMenuSetBeepRowNumUp(void)
{
	U16 BeepRowNumBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	BeepRowNumBackup=atoi((char *)ParaSet.Text);

	if (BeepRowNumBackup >CAN_MAX_DEVICE_NUMBER)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.BeepRowNum=BeepRowNumBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.BeepRowNum-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.BeepRowNum,//��д�����ݻ�����
					(sizeof(RamSetParameters.BeepRowNum) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.BeepRowNum-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.BeepRowNum,//��д�����ݻ�����
					(sizeof(RamSetParameters.BeepRowNum) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((BeepRowNumBackup == RamSetParameters.BeepRowNum) ? 1:0);
	}
	Jump2Menu(SetBeepRowNumIndex,FlashMode_ReFlash);
}



//------------------------------------------------------------------		
//������ʾ���ź����
//------------------------------------------------------------------
void DispMenuSetBeepRowNumDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//������ʾ���ź����ּ�
//------------------------------------------------------------------
void DispMenuSetBeepRowNumLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(SetBeepRowNumIndex,FlashMode_ReFlash);
}
//------------------------------------------------------------------		
//������ʾ���źŹ������
//------------------------------------------------------------------
void DispMenuSetBeepRowNumRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(SetBeepRowNumIndex,FlashMode_ReFlash);
}



void DispMenuSetBeepRowNumEscUp(void)
{
	
}

void DispMenuSetBeepRowNumEscDown(void)
{
	
}
void DispMenuSetBeepRowNumEscOk(void)
{
	
}




//------------------------------------------------------------------		
//������Ϸͨ��ʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetGamePassTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00");
		my_strcpy(ParaSet.MaxText, "99");
		
		snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", RamSetParameters.GamePassTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GamePassTime_PosX , GamePassTime_PosY, 126, GUI_TA_HCENTER);
   GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//������Ϸͨ��ʱ�����
//------------------------------------------------------------------
void DispMenuSetGamePassTimeUp(void)
{
	U16 GamePassTimeBackup;
	
	RamSetParameters.GamePassTime=atoi((char *)ParaSet.Text);

	GamePassTimeBackup = RamSetParameters.GamePassTime;

#ifdef INT_FLASH_SAVE
	

	
       FLASH_WRITE(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber);     
       FLASH_READ(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GamePassTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GamePassTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GamePassTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GamePassTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GamePassTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GamePassTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((GamePassTimeBackup == RamSetParameters.GamePassTime) ? 1:0);

	Jump2Menu(GamePassTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//������Ϸͨ��ʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetGamePassTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//������Ϸͨ��ʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetGamePassTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(GamePassTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//������Ϸͨ��ʱ��������
//------------------------------------------------------------------
void DispMenuSetGamePassTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(GamePassTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetGamePassTimeEscUp(void)
{
	
}
void DispMenuSetGamePassTimeEscDown(void)
{

}
void DispMenuSetGamePassTimeEscOk(void)
{
	
}	



//------------------------------------------------------------------		
//����������ʾʱ���ʼ��
//------------------------------------------------------------------
void DispMenuVoicePromptTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00000");
		my_strcpy(ParaSet.MaxText, "99999");
		
		snprintf((char*)&ParaSet.Text[0], 5+1,"%05d", RamSetParameters.VoicePromptTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(VoicePromptTime_PosX , VoicePromptTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//����������ʾʱ�����
//------------------------------------------------------------------
void DispMenuVoicePromptTimeUp(void)
{
	U32 VoicePromptTimeBackup;
	
	RamSetParameters.VoicePromptTime=atol((char *)ParaSet.Text);

	VoicePromptTimeBackup = RamSetParameters.VoicePromptTime;

    #ifdef INT_FLASH_SAVE
        //FLASH_WRITE(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
        //FLASH_READ(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
        FLASH_WRITE(FlashSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime);     
        FLASH_READ(FlashSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime,RamSetParameters.LightsDelayTime); 
    #endif
    
    #ifdef EEPROM_SAVE
        AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.VoicePromptTime-(U32)&RamSetParameters,//��д���ַ
                    (uint8_t *)&RamSetParameters.VoicePromptTime,//��д�����ݻ�����
                    (sizeof(RamSetParameters.VoicePromptTime) / sizeof(uint8_t))); //��д�����ݳ���
        
        AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.VoicePromptTime-(U32)&RamSetParameters,//��д���ַ
                    (uint8_t *)&RamSetParameters.VoicePromptTime,//��д�����ݻ�����
                    (sizeof(RamSetParameters.VoicePromptTime) / sizeof(uint8_t))); //��д�����ݳ���
        
    #endif

	DisplaySuccessFailure((VoicePromptTimeBackup == RamSetParameters.VoicePromptTime) ? 1:0);

	Jump2Menu(VoicePromptTimeIndex,FlashMode_ReFlash);
}



//------------------------------------------------------------------		
//����������ʾʱ�䷵��
//------------------------------------------------------------------
void DispMenuVoicePromptTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//����������ʾʱ�����ּ�
//------------------------------------------------------------------
void DispMenuVoicePromptTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(VoicePromptTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//����������ʾʱ��������
//------------------------------------------------------------------
void DispMenuVoicePromptTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(VoicePromptTimeIndex,FlashMode_ReFlash);
}


void DispMenuVoicePromptTimeEscUp(void)
{
	
}
void DispMenuVoicePromptTimeEscDown(void)
{

}
void DispMenuVoicePromptTimeEscOk(void)
{
	
}	



//------------------------------------------------------------------		
//���÷���������ʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.ReturnTopTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(ReturnTopTime_PosX , ReturnTopTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//���÷���������ʱ�����
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeUp(void)
{
	U16 ReturnTopTimeBackup;
	
	RamSetParameters.ReturnTopTime=atoi((char *)ParaSet.Text);

	ReturnTopTimeBackup = RamSetParameters.ReturnTopTime;

#ifdef INT_FLASH_SAVE
	

	
       FLASH_WRITE(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber);     
       FLASH_READ(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.ReturnTopTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.ReturnTopTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.ReturnTopTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.ReturnTopTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.ReturnTopTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.ReturnTopTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((ReturnTopTimeBackup == RamSetParameters.ReturnTopTime) ? 1:0);

	Jump2Menu(ReturnTopTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//���÷���������ʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//���÷���������ʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(ReturnTopTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//���÷���������ʱ��������
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(ReturnTopTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetReturnTopTimeEscUp(void)
{
	
}
void DispMenuSetReturnTopTimeEscDown(void)
{

}
void DispMenuSetReturnTopTimeEscOk(void)
{
	
}	




//------------------------------------------------------------------		
//{"ȫ�ʵ�����"}�Ӳ˵����ܺ���
//------------------------------------------------------------------
void DispMenuFullColorLightSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_FullColorLightSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

	
    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
	
	if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item = 0;
        Where = Item;
    }
	
    //������ʼ������
    BarMenuInit();
}




void DispMenuFullColorLightSetUp(void)
{
	
	switch(Item){
	  /*
	  case 0:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(ControlAddrSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  */
	  case 0:{//PUSH  -->POP
		  //PUSH();
		  Jump2Menu(ColorOrderSetIndex,FlashMode_ManualInit);
		  break;
	  }

	  case 1:{//PUSH  -->POP
		  //PUSH();
		  Jump2Menu(ChipTypeSetIndex,FlashMode_ManualInit);
		  break;
	  }
	  
	  case 2:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(NumberOfFramesSetIndex,FlashMode_AutoInit);
		  break;
	  }

	  case 3:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(FullColorLightPatternSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  
	  case 4:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(MaxLedNumberSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  case 5:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(GroupsLedNumberSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  /*
	  case 7:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(LedColourSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  */ 
	  case 6:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(LedColorCycleTimeSetIndex,FlashMode_AutoInit);
		  break;
	  }

	   case 7:{//PUSH  -->POP
		  //PUSH();
		  Jump2Menu(LedControlSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  case 8:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(LedColorCycleModeSetIndex,FlashMode_AutoInit);
		  break;
	  }
	  
	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}


void DispMenuFullColorLightSetDown(void)
{
	POP();
}

void DispMenuFullColorLightSetLeft(void)
{
	BarMenuLeft();
}

void DispMenuFullColorLightSetRight(void)
{
	BarMenuRight();
}

void DispMenuFullColorLightSetEscUp(void)
{
	
}

void DispMenuFullColorLightSetEscDown(void)
{
	
}

void DispMenuFullColorLightSetEscOk(void)
{
	
}




//------------------------------------------------------------------		
//���õ����豸��ʼ��
//------------------------------------------------------------------
void DispMenuConfig_S_DevInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item =0;
        Where = 0;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Tip[language].Text+4),0,3,126,GUI_TA_HCENTER);	
	
}

//------------------------------------------------------------------		
//���õ����豸���
//------------------------------------------------------------------
void DispMenuConfig_S_DevUp(void)
{
	ResetParametersData();
	DisplaySuccessFailure(1);
	POP();
	Jump2Menu(MenuParaIndex,FlashMode_ManualInit);
}


//------------------------------------------------------------------		
//���õ����豸����
//------------------------------------------------------------------
void DispMenuConfig_S_DevDown(void)
{
	POP();
}


void DispMenuConfig_S_DevLeft(void)
{
	
}
void DispMenuConfig_S_DevRight(void)
{
	
}
void DispMenuConfig_S_DevEscUp(void)
{
	
}
void DispMenuConfig_S_DevEscDown(void)
{
	
}
void DispMenuConfig_S_DevEscOk(void)
{
	
}


u8 ConfigDevTimeOut(u8 NodeId)
{	
	u16 TimeOut;
	UNS32  abortCode;
	//UNS8 NodeId=DeviceNumber & 0x7f;
	
	for (TimeOut =100; TimeOut>0; TimeOut--)
	{
		 OSTimeDlyHMSM(0, 0,0,10);
		 if (getWriteResultNetworkDict (CANOpenMasterObject, NodeId, &abortCode) ==SDO_FINISHED)
		 {
			 return 1;
		 }
		 
	}
	return 0;
}

u8 ConfigDev(u8 DeviceNumber)
{
	char StringBuff[16+1];
	
	u32 ValBuff;
	u16 TimeOut;
	
	UNS8 NodeId;
	UNS32  abortCode;
	UNS8 Result;
	INT8U err;
	
	GUI_Clear();
	my_strcpy(StringBuff,"��������");
	GUI_DispStringAtBar(StringBuff,0,1,128,GUI_TA_HCENTER);
	snprintf(StringBuff, 3+1,"%03d", DeviceNumber);
	my_strcpy(&StringBuff[3],"���豸");
	GUI_DispStringAtBar(StringBuff,0,4,128,GUI_TA_HCENTER);
	
	OSTimeDlyHMSM(0, 0,0,100);
	
	if (((DeviceNumber >0) &&(DeviceNumber <= RamSetParameters.CAN_DeviceTotal.CAN_Device1)))	
	{
		NodeId =DeviceNumber;
		SetCanHandle(CANOpenMasterObject, CAN1);	
	}
	else if (((DeviceNumber >RamSetParameters.CAN_DeviceTotal.CAN_Device1) && (DeviceNumber <= (RamSetParameters.CAN_DeviceTotal.CAN_Device2+RamSetParameters.CAN_DeviceTotal.CAN_Device1))))
	{
		NodeId =DeviceNumber - RamSetParameters.CAN_DeviceTotal.CAN_Device1;
		SetCanHandle(CANOpenMasterObject, CAN2);
	}
	
	//NodeId =DeviceNumber;

	
	UNS32 size;
	UNS32 COB_ID_Client_to_Server_Transmit_SDO = 0x600+NodeId;
	UNS32 COB_ID_Server_to_Client_Receive_SDO = 0x580+NodeId;
	UNS8 Node_ID_of_the_SDO_Server = NodeId;

    TxDeviceCmd(DeviceNumber, PARA_SET_CMD, PARA_SET_START);

	size = sizeof (UNS32);
	writeLocalDict( CANOpenMasterObject, /*CO_Data* d*/
			0x1280, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COB_ID_Client_to_Server_Transmit_SDO, /*void * pSourceData,*/ 
			(UNS32*)&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	writeLocalDict( CANOpenMasterObject, /*CO_Data* d*/
			0x1280, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&COB_ID_Server_to_Client_Receive_SDO, /*void * pSourceData,*/ 
			(UNS32*)&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	
	size = sizeof (UNS8);
	writeLocalDict( CANOpenMasterObject, /*CO_Data* d*/
			0x1280, /*UNS16 index*/
			0x03, /*UNS8 subind*/ 
			&Node_ID_of_the_SDO_Server, /*void * pSourceData,*/ 
			(UNS32*)&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	ValBuff= RamSetParameters.LedPara.LedParaGroups[DeviceNumber].GroupsLedNumber;
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x00, sizeof(U16), uint16, (BYTE*)&ValBuff, 0)) return 0;
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}		   		 
	ValBuff= RamSetParameters.LedPara.ColorOrder;
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x01, sizeof(U8), uint8, (BYTE*)&ValBuff, 0)) return 0;
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}
	ValBuff= RamSetParameters.LedPara.ChipType;
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x02, sizeof(U8), uint8, (BYTE*)&ValBuff, 0)) return 0;
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}
	ValBuff= MCO_SET_32BIT_VAL (RamSetParameters.LedPara.LedParaGroups[DeviceNumber].PlayFps, 
								RamSetParameters.LedPara.LedParaGroups[DeviceNumber].StopFps, 
								RamSetParameters.LedPara.LedParaGroups[DeviceNumber].CycleFps, 0x00);
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x03, sizeof(U32), uint32, (BYTE*)&ValBuff, 0)) return 0;
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}
	ValBuff= RamSetParameters.LedPara.LedParaGroups[DeviceNumber].PatternNumber;
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x04, sizeof(U8), uint8, (BYTE*)&ValBuff, 0)) return 0;	
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}
	ValBuff= RamSetParameters.LedPara.LedParaGroups[DeviceNumber].LedColorCycleMode;
	if (writeNetworkDict (CANOpenMasterObject, NodeId, 0x6006, 0x05, sizeof(U8), uint8, (BYTE*)&ValBuff, 0)) return 0;
	else 
	{
		if (0==ConfigDevTimeOut(NodeId))	return 0;
	}
	TxDeviceCmd(DeviceNumber, PARA_SET_CMD, PARA_SET_END);
	
	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) PARA_SET_OK_FLAG ,
				(INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
				(INT32U) 1000,
				(INT8U *)&err);
	if (err == OS_ERR_TIMEOUT)
	{
		return 0;
	}
	else
	{	
		return 1;	
	}
}


void ConfigDevStatus(u8 DeviceNumber)
{
	char StringBuff[16+1];
	PUSH();
	GUI_Clear();
	snprintf(StringBuff, 3+1,"%03d", DeviceNumber);
	my_strcpy(&StringBuff[3],"���豸");
	GUI_DispStringAtBar(StringBuff,0,1,128,GUI_TA_HCENTER);
	my_strcpy(StringBuff,"���óɹ�!");
	GUI_DispStringAtBar(StringBuff,0,4,128,GUI_TA_HCENTER);
	OSTimeDlyHMSM(0, 0,1,0);	
	POP();
}

//------------------------------------------------------------------		
//���ö���豸��ʼ��
//------------------------------------------------------------------
void DispMenuConfig_M_DevInit(void)
{
	char StringBuff[16+1];
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-000");
		my_strcpy(ParaSet.MaxText, "999-999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", StatrDeviceNumber);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 3+1,"%03d", EndDeviceNumber);
		ParaSet.CursorPos=0;
		
		Config_M_Dev_Flag=1;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(MenuPara[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(Config_M_Dev_PosX , Config_M_Dev_PosY, 126, GUI_TA_HCENTER);	
}
	


void DispMenuConfig_M_DevUp(void)
{
	char StringBuff[16+1];

	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	
	StatrDeviceNumber=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[4],3);
	EndDeviceNumber=atoi(StringBuff);


	if ((StatrDeviceNumber >255) || (EndDeviceNumber >255))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{	
		if (1==Config_M_Dev_Flag)
		{	
			DeviceNumberCount = StatrDeviceNumber;
		}
		for (; DeviceNumberCount <= EndDeviceNumber; DeviceNumberCount++)
		{
			Config_M_Dev_Flag = ConfigDev(DeviceNumberCount);
			
			if (0==Config_M_Dev_Flag)
			{
				resetSDOline(CANOpenMasterObject, 0);
				Reset_Read_Inputs_8_Bit();
				//PUSH();
				Jump2Menu(Config_M_DevFailIndex,FlashMode_ManualReFlash);	
				return;
			}
			else
			{
				ConfigDevStatus(DeviceNumberCount);
			}
		}	
	}
	Jump2Menu(Config_M_DevIndex,FlashMode_ReFlash);
}


void DispMenuConfig_M_DevDown(void)
{
	POP();
	
	
	
}
void DispMenuConfig_M_DevLeft(void)
{
	ParaSetAdd();
	Jump2Menu(Config_M_DevIndex,FlashMode_ReFlash);	
}
void DispMenuConfig_M_DevRight(void)
{
	ParaSetCursorRightMoves();
	Jump2Menu(Config_M_DevIndex,FlashMode_ReFlash);	
}
void DispMenuConfig_M_DevEscUp(void)
{
	
}
void DispMenuConfig_M_DevEscDown(void)
{
	
}
void DispMenuConfig_M_DevEscOk(void)
{
	
}



//------------------------------------------------------------------		
//���ö���豸ʧ�ܳ�ʼ��
//------------------------------------------------------------------
void DispMenuConfig_M_DevFailInit(void)
{
	char StringBuff[16+1];
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
		//PUSH();
        Item = 0;
        Where = 0;
		ItemNum = (*Menu_RetrySkip).TextNum;
		//-------------------------
	}
	GUI_Clear();
	snprintf(StringBuff, 3+1,"%03d", DeviceNumberCount);
	my_strcpy(&StringBuff[3],"���豸");
	GUI_DispStringAtBar(StringBuff,0,1,128,GUI_TA_HCENTER);
	my_strcpy(StringBuff,"����ʧ��!");
	GUI_DispStringAtBar(StringBuff,0,3,128,GUI_TA_HCENTER);
	
	if (0==Item)
	{
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispStringAt(*(Menu_RetrySkip[language].Text+0),16,6);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
		GUI_DispStringAt(*(Menu_RetrySkip[language].Text+1),80,6);
	}
	else
	{
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
		GUI_DispStringAt(*(Menu_RetrySkip[language].Text+0),16,6);
		GUI_SetTextMode(GUI_TEXTMODE_REVERSE);
		GUI_DispStringAt(*(Menu_RetrySkip[language].Text+1),80,6);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
}

//------------------------------------------------------------------		
//���ö���豸ʧ�����
//------------------------------------------------------------------
void DispMenuConfig_M_DevFailUp(void)
{
	if (Item)
	{
		if (DeviceNumberCount < EndDeviceNumber)
		{
			DeviceNumberCount++;
		}
		else
		{
			DeviceNumberCount =EndDeviceNumber;
		}
		
	}
	Jump2Menu(Config_M_Dev_UpIndex,FlashMode_AutoInit);
	//POP();
}



//------------------------------------------------------------------		
//���ö���豸ʧ�ܷ���
//------------------------------------------------------------------
void DispMenuConfig_M_DevFailDown(void)
{
	//POP();
	Jump2Menu(Config_M_DevIndex,FlashMode_AutoInit);
}
//------------------------------------------------------------------		
//���ö���豸ʧ��ѡ���
//------------------------------------------------------------------

void DispMenuConfig_M_DevFailLeft(void)
{
	MenuItemAdd();
	Jump2Menu(Config_M_DevFailIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//���ö���豸ʧ��ѡ���
//------------------------------------------------------------------
void DispMenuConfig_M_DevFailRight(void)
{
	MenuItemSub();
	Jump2Menu(Config_M_DevFailIndex,FlashMode_ReFlash);
}
void DispMenuConfig_M_DevFailEscUp(void)
{
	
}
void DispMenuConfig_M_DevFailEscDown(void)
{
	
}
void DispMenuConfig_M_DevFailEscOk(void)
{
	
}
















#if 0
//------------------------------------------------------------------		
//���ö���豸��ʼ��
//------------------------------------------------------------------
void DispMenuConfig_M_DevInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item =0;
        Where = 0;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Tip[language].Text+4),0,3,126,GUI_TA_HCENTER);	
	
}

//------------------------------------------------------------------		
//���ö���豸���
//------------------------------------------------------------------
void DispMenuConfig_M_DevUp(void)
{
	ResetParametersData();
	DisplaySuccessFailure(1);
	POP();
	Jump2Menu(MenuParaIndex,FlashMode_ManualInit);
}


//------------------------------------------------------------------		
//���ö���豸����
//------------------------------------------------------------------
void DispMenuConfig_M_DevDown(void)
{
	POP();
}


void DispMenuConfig_M_DevLeft(void)
{
	
}
void DispMenuConfig_M_DevRight(void)
{
	
}
void DispMenuConfig_M_DevEscUp(void)
{
	
}
void DispMenuConfig_M_DevEscDown(void)
{
	
}
void DispMenuConfig_M_DevEscOk(void)
{
	
}
#endif








//------------------------------------------------------------------		
//{"��������ַ����"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuControlAddrSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		my_strcpy(ParaSet.Text, "000");
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(ControlAddrSet_PosX, ControlAddrSet_PosY, 126, GUI_TA_HCENTER);	   
}

//------------------------------------------------------------------		
//��������ַ�������
//------------------------------------------------------------------
void DispMenuControlAddrSetUp(void)
{
	INT8U  err;
	U16 ControlAddr;
	UartProtocl *p_msg;
	ControlAddr=atoi((char *)ParaSet.Text);
	if (ControlAddr >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) STUDY_OK_FLAG,
							(INT8U) OS_FLAG_CLR,
							(INT8U  *)&err);
		
		//TxLedControlCmd(0x0001,DEFAULT_SLAVE_ADDR, STUDY_CMD, &ControlAddr);
		
		TxStudyCodeCommands(ControlAddr);
		
		err = OSQFlush(GAME_RX_Q);
		
		p_msg = ((UartProtocl *)OSQPend(GAME_RX_Q,1000,&err));
			
		if ((err == OS_ERR_TIMEOUT) || ((ControlAddr != p_msg->addr) && (STUDY_CMD != p_msg->command)))
		{
			DisplaySuccessFailure(0);
		}
		else
		{
			DisplaySuccessFailure(1);
		}
	}
	Jump2Menu(ControlAddrSetIndex,FlashMode_ReFlash);
}	


//------------------------------------------------------------------	
//��������ַ���÷���
//------------------------------------------------------------------
void DispMenuControlAddrSetDown(void)
{
	
	//if ((GlobalKeyVal & (KEY_LONG_UP <<6 ))== (KEY_SHORT_UP <<6 ))
	{
		POP();
	}
}
//------------------------------------------------------------------	
//��������ַ�������ּ�
//------------------------------------------------------------------
void DispMenuControlAddrSetLeft(void)
{
	ParaSetAdd();
	Jump2Menu(ControlAddrSetIndex,FlashMode_ReFlash);

}

//------------------------------------------------------------------	
//��������ַ���ù������
//------------------------------------------------------------------
void DispMenuControlAddrSetRight(void) 

{
	//ParaSetSub();
	ParaSetCursorRightMoves();
	Jump2Menu(ControlAddrSetIndex,FlashMode_ReFlash);
	
}


void DispMenuControlAddrSetEscUp(void)
{
	//ParaSetCursorLeftMoves();
	//Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);
}

void DispMenuControlAddrSetEscDown(void)
{
	//ParaSetCursorRightMoves();
	//Jump2Menu(SetDeviceAddressIndex,FlashMode_ReFlash);
	
}
void DispMenuControlAddrSetEscOk(void)
{
	
}


//------------------------------------------------------------------		
//{"ȫ�ʵ���ɫ˳��"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuColorOrderSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//-----------------
    BarMenu = (struct Menu *)&Menu_ColorOrder[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ���˵�����

    //��ʾ������ʾ          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.LedPara.ColorOrder;
        Where = Item;
    }
    //������ʼ������
    BarMenuInit();
}

void DispMenuColorOrderSetUp(void)
{
		
	
	u8 ColorOrderBackup;
//-----------
    PUSH();//
//-------------------
    GUI_Clear();
	
	
	ColorOrderBackup=Item;
    RamSetParameters.LedPara.ColorOrder=ColorOrderBackup;
	
	
#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
	
	
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.ColorOrder-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.ColorOrder,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.ColorOrder) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.ColorOrder-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.ColorOrder,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.ColorOrder) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
    //GUI_Clear();
	DisplaySuccessFailure((RamSetParameters.LedPara.ColorOrder == ColorOrderBackup) ? 1:0);
//-------------------
    POP();
//-------------------
}



void DispMenuColorOrderSetDown(void)
{
	POP();
}
void DispMenuColorOrderSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuColorOrderSetRight(void)
{
	BarMenuRight();
}

void DispMenuColorOrderSetEscUp(void)
{
	
}
void DispMenuColorOrderSetEscDown(void)
{
	
}
void DispMenuColorOrderSetEscOk(void)
{
	
}


//------------------------------------------------------------------		
//{"оƬ����"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuChipTypeSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//-----------------
    BarMenu = (struct Menu *)&Menu_ChipType[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //�û��ĳ�ʼ���������������
    //��ʾ���˵�����

    //��ʾ������ʾ          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item =  RamSetParameters.LedPara.ChipType;
        Where = Item;
    }
    //������ʼ������
    BarMenuInit();
}

void DispMenuChipTypeSetUp(void)
{
		
	
	u8 ChipTypeBackup;
//-----------
    PUSH();//
//-------------------
    GUI_Clear();
	
	
	ChipTypeBackup=Item;
    RamSetParameters.LedPara.ChipType=ChipTypeBackup;
	
	
#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
	
	
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.ChipType-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.ChipType,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.ChipType) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.ChipType-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.ChipType,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.ChipType) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
    //GUI_Clear();
	DisplaySuccessFailure((RamSetParameters.LedPara.ChipType == ChipTypeBackup) ? 1:0);
//-------------------
    POP();
//-------------------
}



void DispMenuChipTypeSetDown(void)
{
	POP();
}
void DispMenuChipTypeSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuChipTypeSetRight(void)
{
	BarMenuRight();
}

void DispMenuChipTypeSetEscUp(void)
{
	
}
void DispMenuChipTypeSetEscDown(void)
{
	
}
void DispMenuChipTypeSetEscOk(void)
{
	
}


u8 LedParaGroupsDeviceNumber=1;
u16 LedParaGroupsDeviceNumberBackup;
//------------------------------------------------------------------		
//{"֡������"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuNumberOfFramesSetInit(void)
{
	
	char StringBuff[16+1];
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-00-00-00");
		my_strcpy(ParaSet.MaxText, "299-39-39-39");
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", LedParaGroupsDeviceNumber);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps);
		ParaSet.Text[6]='-';
		snprintf((char*)&ParaSet.Text[7], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps);
		ParaSet.Text[9]='-';
		snprintf((char*)&ParaSet.Text[10], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps);
		ParaSet.CursorPos=0;
	}
	
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos < 3))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		LedParaGroupsDeviceNumberBackup=atoi(StringBuff);

		if (LedParaGroupsDeviceNumberBackup > MAX_DEVICE_NUM)
		{
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", LedParaGroupsDeviceNumber);
			ParaSet.Text[3]='-';
			LedParaGroupsDeviceNumberBackup = LedParaGroupsDeviceNumber;
		}
		if (LedParaGroupsDeviceNumberBackup != LedParaGroupsDeviceNumber)
		{	
			LedParaGroupsDeviceNumber=LedParaGroupsDeviceNumberBackup;
			snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", LedParaGroupsDeviceNumber);
			ParaSet.Text[3]='-';
			snprintf((char*)&ParaSet.Text[4], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps);
			ParaSet.Text[6]='-';
			snprintf((char*)&ParaSet.Text[7], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps);
			ParaSet.Text[9]='-';
			snprintf((char*)&ParaSet.Text[10], 2+1,"%02d", RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps);	
		}
	}
	
	GUI_Clear();
	my_strcpy(StringBuff,"�豸��:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],3);
	StringBuff[10]='\0';
	//my_strcpy(my_strchr(StringBuff,'\0'),"֡������");
	GUI_DispStringAtBar(StringBuff,0,0,126,GUI_TA_HCENTER);
	
	//GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <3)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((0+3-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	my_strcpy(StringBuff,*(Menu_UpDown[language].Text+0));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[4],2);
	StringBuff[7]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"fps");
	GUI_DispStringAtBar(StringBuff,0,2,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >3)&&(ParaSet.CursorPos <6))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3+2-(ParaSet.CursorPos-4))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	my_strcpy(StringBuff,*(Menu_UpDown[language].Text+1));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[7],2);
	StringBuff[7]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"fps");
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >6)&&(ParaSet.CursorPos <9))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3+2-(ParaSet.CursorPos-7))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	my_strcpy(StringBuff,*(Menu_UpDown[language].Text+2));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[10],2);
	StringBuff[7]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"fps");
	GUI_DispStringAtBar(StringBuff,0,6,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >9)&&(ParaSet.CursorPos <12))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3+2-(ParaSet.CursorPos-10))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
}





void DispMenuNumberOfFramesSetUp(void)
{
	U16 NumberOfFramesSetBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	char StringBuff[3+1]="***";
	
	//U16 LedParaGroupsDeviceNumberBackup;
	U8 PlayFpsBackup;
	U8 StopFpsBackup;
	U8 CycleFpsBackup;
	
	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	StringBuff[3]='\0';
	LedParaGroupsDeviceNumberBackup=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[4],2);
	StringBuff[2]='\0';
	PlayFpsBackup=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[7],2);
	StringBuff[2]='\0';
	StopFpsBackup=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[10],2);
	StringBuff[2]='\0';
	CycleFpsBackup=atoi(StringBuff);
	
	if ((LedParaGroupsDeviceNumberBackup > MAX_DEVICE_NUM) || (PlayFpsBackup >30) || (StopFpsBackup >30) || (CycleFpsBackup >30))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{	
		LedParaGroupsDeviceNumber = LedParaGroupsDeviceNumberBackup;
		RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps = PlayFpsBackup;
		RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps = StopFpsBackup;
		RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps = CycleFpsBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps) / sizeof(uint8_t))); //��д�����ݳ���
		
		
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps) / sizeof(uint8_t))); //��д�����ݳ���
		
		
		
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps) / sizeof(uint8_t))); //��д�����ݳ���
		
		
		
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps) / sizeof(uint8_t))); //��д�����ݳ���
		
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps,//��д�����ݻ�����
				(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps) / sizeof(uint8_t))); //��д�����ݳ���
		

		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps-(U32)&RamSetParameters,//��д���ַ
				(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps,//��д�����ݻ�����
				(sizeof(RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps) / sizeof(uint8_t))); //��д�����ݳ���



		#endif
	   
		DisplaySuccessFailure(((PlayFpsBackup == RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].PlayFps)    && 
							   (StopFpsBackup == RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].StopFps)    && 		
							   (CycleFpsBackup == RamSetParameters.LedPara.LedParaGroups[LedParaGroupsDeviceNumber].CycleFps)) ? 1:0);
		

	}
	Jump2Menu(NumberOfFramesSetIndex,FlashMode_ReFlash);
}
void DispMenuNumberOfFramesSetDown(void)
{
	POP();
}
void DispMenuNumberOfFramesSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(NumberOfFramesSetIndex,FlashMode_ReFlash);
}
void DispMenuNumberOfFramesSetRight(void)
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(NumberOfFramesSetIndex,FlashMode_ReFlash);
}
void DispMenuNumberOfFramesSetEscUp(void)
{
	
}
void DispMenuNumberOfFramesSetEscDown(void)
{
	
}
void DispMenuNumberOfFramesSetEscOk(void)
{
	
}


u8 FullColorLightPatternSetDeviceNumber=1;
u16 FullColorLightPatternSetDeviceNumberBackup;
//------------------------------------------------------------------		
//{"ȫ�ʵƻ�������"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuFullColorLightPatternSetInit(void)
{
	char StringBuff[16+1];
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-000");
		my_strcpy(ParaSet.MaxText, "299-999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", FullColorLightPatternSetDeviceNumber);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 3+1,"%03d", RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber);
		ParaSet.CursorPos=0;
	}
	
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos < 3))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		FullColorLightPatternSetDeviceNumberBackup=atoi(StringBuff);

		if (FullColorLightPatternSetDeviceNumberBackup > MAX_DEVICE_NUM)
		{
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", FullColorLightPatternSetDeviceNumber);
			ParaSet.Text[3]='-';
			FullColorLightPatternSetDeviceNumberBackup = FullColorLightPatternSetDeviceNumber;
		}
		if (FullColorLightPatternSetDeviceNumberBackup != FullColorLightPatternSetDeviceNumber)
		{	
			FullColorLightPatternSetDeviceNumber = FullColorLightPatternSetDeviceNumberBackup;
			snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", FullColorLightPatternSetDeviceNumber);
			ParaSet.Text[3]='-';
			snprintf((char*)&ParaSet.Text[4], 3+1,"%03d", RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber);
		}
	}

	GUI_Clear();
	my_strcpy(StringBuff,"�豸��:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],3);
	StringBuff[10]='\0';
	//my_strcpy(my_strchr(StringBuff,'\0'),"��������");
	GUI_DispStringAtBar(StringBuff,0,1,126,GUI_TA_HCENTER);
	
	//GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <3)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((0+3-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	my_strcpy(StringBuff,"����:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[4],3);
	StringBuff[8]='\0';
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >3)&&(ParaSet.CursorPos <7))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((0+3-(ParaSet.CursorPos-4))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	/*
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	GUI_SetEnFont(En_8x16);
	GUI_DispStringAt(*(Menu_FullColorLightPatternSet[language].Text+0),FullColorLightPatternSet_PosX,FullColorLightPatternSet_PosY);
	ParaSetDisplsy(PosX , PosY, 126, GUI_TA_HCENTER);
	//GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);	
	*/
}



void DispMenuFullColorLightPatternSetUp(void)
{
	char StringBuff[3+1];
	//u16 FullColorLightPatternSetDeviceNumberBackup;
	u16 PatternNumberBackup;

	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	FullColorLightPatternSetDeviceNumberBackup=atoi(StringBuff);
	my_strncpy(StringBuff,&ParaSet.Text[4],3);
	PatternNumberBackup=atoi(StringBuff);

	if ((FullColorLightPatternSetDeviceNumberBackup > MAX_DEVICE_NUM) ||(PatternNumberBackup >999))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		FullColorLightPatternSetDeviceNumber = FullColorLightPatternSetDeviceNumberBackup;
		RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber=PatternNumberBackup;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
		//GUI_Clear();
		DisplaySuccessFailure((RamSetParameters.LedPara.LedParaGroups[FullColorLightPatternSetDeviceNumber].PatternNumber == PatternNumberBackup) ? 1:0);
	}
	Jump2Menu(FullColorLightPatternSetIndex,FlashMode_ReFlash);
	
}

void DispMenuFullColorLightPatternSetDown(void)
{
	POP();
}
void DispMenuFullColorLightPatternSetLeft(void)
{
	ParaSetAdd();
	Jump2Menu(FullColorLightPatternSetIndex,FlashMode_ReFlash);
}
void DispMenuFullColorLightPatternSetRight(void)
{
	ParaSetCursorRightMoves();
	Jump2Menu(FullColorLightPatternSetIndex,FlashMode_ReFlash);
}

void DispMenuFullColorLightPatternSetEscUp(void)
{
	
}
void DispMenuFullColorLightPatternSetEscDown(void)
{
	
}
void DispMenuFullColorLightPatternSetEscOk(void)
{
	
}


//------------------------------------------------------------------		
//{"��������"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuMaxLedNumberSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.LedPara.MaxLedNumber);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(MaxLedNumberSet_PosX , MaxLedNumberSet_PosY, 126, GUI_TA_HCENTER);
	GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
	
}


void DispMenuMaxLedNumberSetUp(void)
{
	U16 MaxLedNumberBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;
	
	
	MaxLedNumberBackup=atoi((char *)ParaSet.Text);

	if (MaxLedNumberBackup >2048)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.LedPara.MaxLedNumber=MaxLedNumberBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.MaxLedNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.MaxLedNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.MaxLedNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.MaxLedNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.MaxLedNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.MaxLedNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((MaxLedNumberBackup == RamSetParameters.LedPara.MaxLedNumber) ? 1:0);
	}
	Jump2Menu(MaxLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuMaxLedNumberSetDown(void)
{
	POP();
}
void DispMenuMaxLedNumberSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(MaxLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuMaxLedNumberSetRight(void)
{
	ParaSetCursorRightMoves();

	Jump2Menu(MaxLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuMaxLedNumberSetEscUp(void)
{
	
}
void DispMenuMaxLedNumberSetEscDown(void)
{
	
}
void DispMenuMaxLedNumberSetEscOk(void)
{
	
}


u8 GroupsLedNumberSetDeviceNumber=1;
u16 GroupsLedNumberSetDeviceNumberBackup;
//------------------------------------------------------------------		
//{"ÿ�������"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuGroupsLedNumberSetInit(void)
{
	char StringBuff[16+1];
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-0000");
		my_strcpy(ParaSet.MaxText, "299-9999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", GroupsLedNumberSetDeviceNumber);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 4+1,"%04d", RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber);
		ParaSet.CursorPos=0;
	}
	
	
	
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos < 3))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		GroupsLedNumberSetDeviceNumberBackup=atoi(StringBuff);

		if (GroupsLedNumberSetDeviceNumberBackup > MAX_DEVICE_NUM)
		{
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", GroupsLedNumberSetDeviceNumber);
			ParaSet.Text[3]='-';
			GroupsLedNumberSetDeviceNumberBackup = GroupsLedNumberSetDeviceNumber;
		}
		if (GroupsLedNumberSetDeviceNumberBackup != GroupsLedNumberSetDeviceNumber)
		{	
			GroupsLedNumberSetDeviceNumber = GroupsLedNumberSetDeviceNumberBackup;
			snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", GroupsLedNumberSetDeviceNumber);
			ParaSet.Text[3]='-';
			snprintf((char*)&ParaSet.Text[4], 4+1,"%04d", RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber);
		}
	}

	GUI_Clear();
	
	my_strcpy(StringBuff,"�豸��:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],3);
	StringBuff[10]='\0';
	//my_strcpy(my_strchr(StringBuff,'\0'),"������");
	GUI_DispStringAtBar(StringBuff,0,1,126,GUI_TA_HCENTER);
	
	//GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <3)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((0+3-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	my_strcpy(StringBuff,"������:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[4],4);
	StringBuff[11]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"��");
	
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >3)&&(ParaSet.CursorPos <8))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((2+4-(ParaSet.CursorPos-4))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	/*
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GroupsLedNumberSet_PosX , GroupsLedNumberSet_PosY, 126, GUI_TA_HCENTER);
	GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
	*/
}


void DispMenuGroupsLedNumberSetUp(void)
{
	char StringBuff[4+1];
	//u16 GroupsLedNumberSetDeviceNumberBackup;
	U16 GroupsLedNumberBackup;
	
	//RamSetParameters.DeviceTotal=atoi((char *)ParaSet.Text);
	//DeviceTotalBackup = RamSetParameters.DeviceTotal;

	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	GroupsLedNumberSetDeviceNumberBackup=atoi(StringBuff);
	my_strncpy(StringBuff,&ParaSet.Text[4],4);
	GroupsLedNumberBackup=atoi(StringBuff);

	if ((GroupsLedNumberSetDeviceNumberBackup >MAX_DEVICE_NUM) || (GroupsLedNumberBackup >2048))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		GroupsLedNumberSetDeviceNumber =GroupsLedNumberSetDeviceNumberBackup;
		RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber=GroupsLedNumberBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((GroupsLedNumberBackup == RamSetParameters.LedPara.LedParaGroups[GroupsLedNumberSetDeviceNumber].GroupsLedNumber) ? 1:0);
	}
	Jump2Menu(GroupsLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuGroupsLedNumberSetDown(void)
{
	POP();
}
void DispMenuGroupsLedNumberSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(GroupsLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuGroupsLedNumberSetRight(void)
{
	ParaSetCursorRightMoves();

	Jump2Menu(GroupsLedNumberSetIndex,FlashMode_ReFlash);
}
void DispMenuGroupsLedNumberSetEscUp(void)
{
	
}
void DispMenuGroupsLedNumberSetEscDown(void)
{
	
}
void DispMenuGroupsLedNumberSetEscOk(void)
{
	
}


U8 * _CONST_ LedColorCycleMode_Color[]=
{		
	"��",
	"��",
	"��",
	"��",
};
u8  LedColorCycleModeSetDeviceNumber=1;
u16 LedColorCycleModeSetDeviceNumberBackup;
//------------------------------------------------------------------		
//{"ȫ�ʵ���ɫģʽ����"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuLedColorCycleModeSetInit(void)
{
	char StringBuff[16+1];
	u8  LedColorCycleModeBackup=0;
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-0");
		my_strcpy(ParaSet.MaxText, "299-3");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", LedColorCycleModeSetDeviceNumber);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 1+1,"%01d", RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode);
		ParaSet.CursorPos=0;
	}
	
	if((Flash == FlashMode_ReFlash) && (ParaSet.CursorPos < 3))//��0����,��ʼ��Item��ֵ
	{
		my_strncpy(StringBuff,ParaSet.Text,3);
		StringBuff[3]='\0';
		LedColorCycleModeSetDeviceNumberBackup=atoi(StringBuff);

		if (LedColorCycleModeSetDeviceNumberBackup > MAX_DEVICE_NUM)
		{
			DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
			snprintf((char*)&ParaSet.Text, 3+1,"%03d", LedColorCycleModeSetDeviceNumber);
			ParaSet.Text[3]='-';
			LedColorCycleModeSetDeviceNumberBackup = LedColorCycleModeSetDeviceNumber;
		}
		if (LedColorCycleModeSetDeviceNumberBackup != LedColorCycleModeSetDeviceNumber)
		{	
			LedColorCycleModeSetDeviceNumber = LedColorCycleModeSetDeviceNumberBackup;
			snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", LedColorCycleModeSetDeviceNumber);
			ParaSet.Text[3]='-';
			snprintf((char*)&ParaSet.Text[4], 1+1,"%01d", RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode);
		}
	}
	GUI_Clear();
	my_strcpy(StringBuff,"�豸��:");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],3);
	StringBuff[10]='\0';
	//my_strcpy(my_strchr(StringBuff,'\0'),"��������");
	GUI_DispStringAtBar(StringBuff,0,1,126,GUI_TA_HCENTER);
	
	//GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <3)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((0+3-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	my_strncpy(StringBuff,&ParaSet.Text[4],1);
	StringBuff[1]='\0';
	LedColorCycleModeBackup = atoi(StringBuff);
	
	
	my_strcpy(StringBuff,"��ɫģʽ:");
	my_strcpy(my_strchr(StringBuff,'\0'),LedColorCycleMode_Color[LedColorCycleModeBackup]);
	//StringBuff[8]='\0';
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >3)&&(ParaSet.CursorPos <5))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		//my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		//StringBuff[1]='\0';
		my_strcpy(StringBuff,LedColorCycleMode_Color[LedColorCycleModeBackup]);
		GUI_DispStringAt(StringBuff, PosX-((0+2-(ParaSet.CursorPos-4))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	/*
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	GUI_SetEnFont(En_8x16);
	GUI_DispStringAt(*(Menu_LedColorCycleModeSet[language].Text+0),LedColorCycleModeSet_PosX,LedColorCycleModeSet_PosY);
	ParaSetDisplsy(PosX , PosY, 126, GUI_TA_HCENTER);
	//GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);	
	*/
}



void DispMenuLedColorCycleModeSetUp(void)
{
	char StringBuff[3+1];
	//u16 LedColorCycleModeSetDeviceNumberBackup;
	u16 LedColorCycleModeBackup;

	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	StringBuff[3]='\0';
	LedColorCycleModeSetDeviceNumberBackup=atoi(StringBuff);
	my_strncpy(StringBuff,&ParaSet.Text[4],1);
	StringBuff[1]='\0';
	LedColorCycleModeBackup=atoi(StringBuff);

	if ((LedColorCycleModeSetDeviceNumberBackup > MAX_DEVICE_NUM) ||(LedColorCycleModeBackup >4-1))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		LedColorCycleModeSetDeviceNumber = LedColorCycleModeSetDeviceNumberBackup;
		RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode=LedColorCycleModeBackup;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect);     
       FLASH_READ(RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect,RamSetParameters.GameModeSelect); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
  
		//GUI_Clear();
		DisplaySuccessFailure((RamSetParameters.LedPara.LedParaGroups[LedColorCycleModeSetDeviceNumber].LedColorCycleMode == LedColorCycleModeBackup) ? 1:0);
	}
	Jump2Menu(LedColorCycleModeSetIndex,FlashMode_ReFlash);
	
}

void DispMenuLedColorCycleModeSetDown(void)
{
	POP();
}
void DispMenuLedColorCycleModeSetLeft(void)
{
	ParaSetAdd();
	Jump2Menu(LedColorCycleModeSetIndex,FlashMode_ReFlash);
}
void DispMenuLedColorCycleModeSetRight(void)
{
	ParaSetCursorRightMoves();
	Jump2Menu(LedColorCycleModeSetIndex,FlashMode_ReFlash);
}

void DispMenuLedColorCycleModeSetEscUp(void)
{
	
}
void DispMenuLedColorCycleModeSetEscDown(void)
{
	
}
void DispMenuLedColorCycleModeSetEscOk(void)
{
	
}







//------------------------------------------------------------------		
//{"������ɫ"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuLedColourSetInit(void)
{
	char StringBuff[16+1];
	
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000-000-000");
		my_strcpy(ParaSet.MaxText, "999-999-999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.LedPara.LedColour.red);
		ParaSet.Text[3]='-';
		snprintf((char*)&ParaSet.Text[4], 3+1,"%03d", RamSetParameters.LedPara.LedColour.green);
		ParaSet.Text[7]='-';
		snprintf((char*)&ParaSet.Text[8], 3+1,"%03d", RamSetParameters.LedPara.LedColour.blue);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);
	my_strcpy(StringBuff,*(Menu_RGB_Color[language].Text+0));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],3);
	StringBuff[6]='\0';
	GUI_DispStringAtBar(StringBuff,0,2,126,GUI_TA_HCENTER);
	
	
	if(ParaSet.CursorPos <3)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	my_strcpy(StringBuff,*(Menu_RGB_Color[language].Text+1));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[4],3);
	StringBuff[6]='\0';
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >3)&&(ParaSet.CursorPos <7))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-4))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	
	
	my_strcpy(StringBuff,*(Menu_RGB_Color[language].Text+2));
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[8],3);
	StringBuff[6]='\0';
	GUI_DispStringAtBar(StringBuff,0,6,126,GUI_TA_HCENTER);

	
	if((ParaSet.CursorPos >7))
	{
		
		
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-8))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

}
	



void DispMenuLedColourSetUp(void)
{
	char StringBuff[3+1]="***";
	
	U16 LedRedColourBackup;
	U16 LedGreenColourBackup;
	U16 LedBlueColourBackup;
	

	my_strncpy(StringBuff,&ParaSet.Text[0],3);
	LedRedColourBackup=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[4],3);
	LedGreenColourBackup=atoi(StringBuff);

	my_strncpy(StringBuff,&ParaSet.Text[8],3);
	LedBlueColourBackup=atoi(StringBuff);

	

	if ((LedRedColourBackup >255) || (LedGreenColourBackup >255) || (LedBlueColourBackup >255))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{	
		RamSetParameters.LedPara.LedColour.red = LedRedColourBackup;
		RamSetParameters.LedPara.LedColour.green = LedGreenColourBackup;
		RamSetParameters.LedPara.LedColour.blue = LedBlueColourBackup;
		
		
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		
		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.red-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.red,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.red) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.green-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.green,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.green) / sizeof(uint8_t))); //��д�����ݳ���
		
		
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.blue-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.blue,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.blue) / sizeof(uint8_t))); //��д�����ݳ���
					
					
		
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.red-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.red,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.red) / sizeof(uint8_t))); //��д�����ݳ���
					
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.green-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.green,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.green) / sizeof(uint8_t))); //��д�����ݳ���			
					
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedPara.LedColour.blue-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedPara.LedColour.blue,//��д�����ݻ�����
					(sizeof(RamSetParameters.LedPara.LedColour.blue) / sizeof(uint8_t))); //��д�����ݳ���			
					
		
		#endif
	   
		DisplaySuccessFailure(((LedRedColourBackup == RamSetParameters.LedPara.LedColour.red)    && 
							  (LedGreenColourBackup ==RamSetParameters.LedPara.LedColour.green) && 
							  (LedBlueColourBackup == RamSetParameters.LedPara.LedColour.blue)) ? 1:0);
	}
	Jump2Menu(LedColourSetIndex,FlashMode_ReFlash);
}

void DispMenuLedColourSetDown(void)
{
	POP();
}
void DispMenuLedColourSetLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(LedColourSetIndex,FlashMode_ReFlash);
}
void DispMenuLedColourSetRight(void)
{
	ParaSetCursorRightMoves();

	Jump2Menu(LedColourSetIndex,FlashMode_ReFlash);
}
void DispMenuLedColourSetEscUp(void)
{
	
}
void DispMenuLedColourSetEscDown(void)
{
	
}
void DispMenuLedColourSetEscOk(void)
{
	
}


//------------------------------------------------------------------		
//ȫ�ʵ���ɫѭ��ʱ�����ó�ʼ��
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "0000");
		my_strcpy(ParaSet.MaxText, "9999");
		
		snprintf((char*)&ParaSet.Text[0], 4+1,"%04d", RamSetParameters.LedControl.LedColorCycleTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_FullColorLightSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(LedColorCycleTimeSet_PosX , LedColorCycleTimeSet_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//ȫ�ʵ���ɫѭ��ʱ���������
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetUp(void)
{
	U16 LedColorCycleTimeBackup;
	
	LedColorCycleTimeBackup=atoi((char *)ParaSet.Text);

	RamSetParameters.LedControl.LedColorCycleTime=LedColorCycleTimeBackup;

#ifdef INT_FLASH_SAVE
	

	
       FLASH_WRITE(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber);     
       FLASH_READ(RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber,RamSetParameters.TrampleNumber); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.LedColorCycleTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.LedColorCycleTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.LedColorCycleTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.LedColorCycleTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.LedColorCycleTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.LedColorCycleTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((LedColorCycleTimeBackup == RamSetParameters.LedControl.LedColorCycleTime) ? 1:0);

	Jump2Menu(LedColorCycleTimeSetIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//ȫ�ʵ���ɫѭ��ʱ�����÷���
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//��ȫ�ʵ���ɫѭ��ʱ���������ּ�
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetLeft(void)
{
	ParaSetAdd();
	Jump2Menu(LedColorCycleTimeSetIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//ȫ�ʵ���ɫѭ��ʱ�����ù������
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(LedColorCycleTimeSetIndex,FlashMode_ReFlash);
}

void DispMenuLedColorCycleTimeSetEscUp(void)
{
	
}
void DispMenuLedColorCycleTimeSetEscDown(void)
{

}
void DispMenuLedColorCycleTimeSetEscOk(void)
{
	
}	


//------------------------------------------------------------------		
//�ƴ��������ù��ܺ���
//------------------------------------------------------------------
void DispMenuLedControlSetInit(void)
{
//----------------------
    GUI_Clear();
//--------------------
    //�л�Bar��ʾ����
//---------------------
    BarMenu = (struct Menu *)&Menu_LedControlSet[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

	
    //�û��ĳ�ʼ���������������
    //��ʾ������Ӧ��������ʾ,1-->"����"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
	
	 if(Flash == FlashMode_ManualInit){//�״ν��뱾��˵�������Item where
        PUSH();
        Item = 0;
        Where = Item;
    }
	
    //������ʼ������
    BarMenuInit();

}


void DispMenuLedControlSetUp(void)
{

	switch(Item){
	  /*
	  case 0:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(Local_OnOffIndex,FlashMode_AutoInit);
		  break;
	  }
	  */
	  case 0:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(T1000_OnOffIndex,FlashMode_AutoInit);
		  break;
	  }
	  /*
	  case 2:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(Local_TimerIndex,FlashMode_AutoInit);
		  break;
	  }
	  */
	  
	  case 1:{//PUSH  -->POP
		  PUSH();
		  Jump2Menu(T1000_TimerIndex,FlashMode_AutoInit);
		  break;
	  }

	  default:{
		  Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
		  return;
	  }
  }
}


void DispMenuLedControlSetDown(void)
{
	POP();
}
void DispMenuLedControlSetLeft(void)
{
	BarMenuLeft();
}
void DispMenuLedControlSetRight(void)
{
	BarMenuRight();
}
void DispMenuLedControlSetEscUp(void)
{
	
}
void DispMenuLedControlSetEscDown(void)
{
	
}
void DispMenuLedControlSetEscOk(void)
{
	
}

//------------------------------------------------------------------		
//�����ƴ��������ó�ʼ��
//------------------------------------------------------------------
void DispMenuLocal_OnOffInit(void)
{ 
	U8 ItemBackup = Item;
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.LedControl.Local_OnOff;
        Where = 0;
		ItemNum = (*Menu_OnOff).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_LedControlSet[language].Text+ItemBackup),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_OnOff[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
    //OnOffStatusSetInit(DeviceStatus.MusicStatus);
}

//------------------------------------------------------------------		
//�����ƴ������������
//------------------------------------------------------------------
void DispMenuLocal_OnOffUp(void)
{
	RamSetParameters.LedControl.Local_OnOff = Item;
    
	#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus);     
       FLASH_READ(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.Local_OnOff-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.Local_OnOff,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.Local_OnOff) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.Local_OnOff-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.Local_OnOff,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.Local_OnOff) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif
    

	DisplaySuccessFailure((RamSetParameters.LedControl.Local_OnOff == Item) ? 1:0);
	
	
	if (RamSetParameters.LedControl.Local_OnOff == TIMER_ON)
	{
		LOCAL_TIMING_OFF();
	}
	else
	{
		LOCAL_TIMING_ON();
	}
	
	
	Jump2Menu(Local_OnOffIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�����ƴ��������÷���
//------------------------------------------------------------------
void DispMenuLocal_OnOffDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//�����ƴ���������ѡ���
//------------------------------------------------------------------
void DispMenuLocal_OnOffLeft(void)
{
	MenuItemAdd();
	Jump2Menu(Local_OnOffIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//�����ƴ���������ѡ���
//------------------------------------------------------------------
void DispMenuLocal_OnOffRight(void)  
{
	MenuItemSub();
	Jump2Menu(Local_OnOffIndex,FlashMode_ReFlash);
}



void DispMenuLocal_OnOffEscUp(void)
{
	
}
void DispMenuLocal_OnOffEscDown(void)
{
	
}
void DispMenuLocal_OnOffEscOk(void)
{
	
}


//------------------------------------------------------------------		
//1000T�ƴ��������ó�ʼ��
//------------------------------------------------------------------
void DispMenuT1000_OnOffInit(void)
{ 
	U8 ItemBackup = Item;
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        Item = RamSetParameters.LedControl.T1000_OnOff;
        Where = 0;
		ItemNum = (*Menu_OnOff).TextNum;
		//-------------------------
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_LedControlSet[language].Text+ItemBackup),0,1,126,GUI_TA_HCENTER);
	GUI_DispStringAtBar(*(Menu_OnOff[language].Text+Item),0,4,126,GUI_TA_HCENTER);
	
	
    //OnOffStatusSetInit(DeviceStatus.MusicStatus);
}

//------------------------------------------------------------------		
//1000T�ƴ������������
//------------------------------------------------------------------
void DispMenuT1000_OnOffUp(void)
{
	RamSetParameters.LedControl.T1000_OnOff = Item;
    
	#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus);     
       FLASH_READ(RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus,RamSetParameters.DeviceStatus); 
    #endif
       
    #ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.T1000_OnOff-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.T1000_OnOff,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.T1000_OnOff) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.T1000_OnOff-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.LedControl.T1000_OnOff,//��д�����ݻ�����
                   (sizeof(RamSetParameters.LedControl.T1000_OnOff) / sizeof(uint8_t))); //��д�����ݳ���
           
    #endif
    
	DisplaySuccessFailure((RamSetParameters.LedControl.T1000_OnOff == Item) ? 1:0);
	
	if (RamSetParameters.LedControl.T1000_OnOff == TIMER_ON)
	{
		T1000_TIMING_OFF();
	}
	else
	{
		T1000_TIMING_ON();
	}

	Jump2Menu(T1000_OnOffIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//1000T�ƴ��������÷���
//------------------------------------------------------------------
void DispMenuT1000_OnOffDown(void)
{
	POP();
}
//------------------------------------------------------------------		
//1000T�ƴ���������ѡ���
//------------------------------------------------------------------
void DispMenuT1000_OnOffLeft(void)
{
	MenuItemAdd();
	Jump2Menu(T1000_OnOffIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//1000T�ƴ���������ѡ���
//------------------------------------------------------------------
void DispMenuT1000_OnOffRight(void)  
{
	MenuItemSub();
	Jump2Menu(T1000_OnOffIndex,FlashMode_ReFlash);
}



void DispMenuT1000_OnOffEscUp(void)
{
	
}
void DispMenuT1000_OnOffEscDown(void)
{
	
}
void DispMenuT1000_OnOffEscOk(void)
{
	
}


u8 LocalTimerNumber =0;
u8 LocalTimerNumberBackup;
u8 LocalTimerStatus;
u8 LocalTimerWeekNumber =1;

void Update_Local_Timer_Text(void)
{
	snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", LocalTimerNumber);
	ParaSet.Text[2]='-';
	snprintf((char*)&ParaSet.Text[3], 1+1,"%01d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].Status);
	ParaSet.Text[4]='-';
	snprintf((char*)&ParaSet.Text[5], 1+1,"%01d", LocalTimerWeekNumber);
	ParaSet.Text[6]='-';
	snprintf((char*)&ParaSet.Text[7], 1+1,"%01d", (RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].Week & (1 << LocalTimerWeekNumber)) ? 1 : 0);
	ParaSet.Text[8]='-';
	snprintf((char*)&ParaSet.Text[9], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOnTime.Hours);
	ParaSet.Text[11]=':';
	snprintf((char*)&ParaSet.Text[12], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOnTime.Minutes);
	ParaSet.Text[14]=':';
	snprintf((char*)&ParaSet.Text[15], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOnTime.Seconds);
	ParaSet.Text[17]='-';
	snprintf((char*)&ParaSet.Text[18], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOffTime.Hours);
	ParaSet.Text[20]=':';
	snprintf((char*)&ParaSet.Text[21], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOffTime.Minutes);
	ParaSet.Text[23]=':';
	snprintf((char*)&ParaSet.Text[24], 2+1,"%02d", RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].TimerOffTime.Seconds);
	ParaSet.Text[26]='\0';
}


//��ʱ��[01][��]
//����[һ] [��]
//[00:00:00] ��
//[00:00:00] �ر�
//------------------------------------------------------------------		
//{"���صƴ���ʱ"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuLocal_TimerInit(void)
{
	char StringBuff[16+1];
	//u8 LocalTimerNumber=0;
	u8 TimerStatusBuff=0;
	u8 WeekStatusBuff;
	
	//u8 TimeOnOffBuff=0;
	//u8 WeekBuff=0;

	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00-0-1-0-00:00:00-00:00:00");
		my_strcpy(ParaSet.MaxText, "99-1-7-1-29:59:59-29:59:59");
		
		Update_Local_Timer_Text();
		ParaSet.CursorPos=0;
	}
	
	if(Flash == FlashMode_ReFlash) 
	{
		if (ParaSet.CursorPos < 2)//��0����,��ʼ��Item��ֵ
		{ 
			my_strncpy(StringBuff, &ParaSet.Text[0], 2);
			StringBuff[2]='\0';
			LocalTimerNumberBackup=atoi(StringBuff);

			if (LocalTimerNumberBackup > TIMER_NUMBER-1)
			{
				DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
				snprintf((char*)&ParaSet.Text, 2+1,"%02d", LocalTimerNumber);
				ParaSet.Text[2]='-';
				LocalTimerNumberBackup = LocalTimerNumber;
			}
			if (LocalTimerNumberBackup != LocalTimerNumber)
			{	
				LocalTimerNumber = LocalTimerNumberBackup;
				Update_Local_Timer_Text();
			}
		}
		else if ((ParaSet.CursorPos > 4) && (ParaSet.CursorPos < 6))//��0����,��ʼ��Item��ֵ
		{
			my_strncpy(StringBuff,&ParaSet.Text[5],1);
			StringBuff[1]='\0';
			LocalTimerWeekNumber=atoi(StringBuff);
			snprintf((char*)&ParaSet.Text[7], 1+1,"%01d", (RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].Week & (1 << LocalTimerWeekNumber)) ? 1:0);
			ParaSet.Text[8]='-';
		}
	}
	
	GUI_Clear();
	//GUI_DispStringAtBar(*(Menu_LedControlSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);

	my_strncpy(StringBuff,&ParaSet.Text[3],1);
	StringBuff[1]='\0';
	TimerStatusBuff=atoi(StringBuff);

	my_strcpy(StringBuff,"��ʱ��[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],2);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"][");
	my_strcpy(my_strchr(StringBuff,'\0'),*(Menu_OnOff[language].Text+TimerStatusBuff));
	my_strcpy(my_strchr(StringBuff,'\0'),"]");
	
	GUI_DispStringAtBar(StringBuff,0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <2)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >2)&&(ParaSet.CursorPos <4))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strcpy(StringBuff,*(Menu_OnOff[language].Text+TimerStatusBuff));
		//StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-3))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	
	my_strncpy(StringBuff,&ParaSet.Text[5],1);
	StringBuff[1]='\0';
	LocalTimerWeekNumber=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[7],1);
	StringBuff[1]='\0';
	WeekStatusBuff=atoi(StringBuff);
	
	my_strcpy(StringBuff,"����[");
	my_strcpy(my_strchr(StringBuff,'\0'),WeekToChinese(LocalTimerWeekNumber));
	my_strcpy(my_strchr(StringBuff,'\0'),"][");
	my_strcpy(my_strchr(StringBuff,'\0'),*(Menu_OnOff[language].Text+WeekStatusBuff));
	my_strcpy(my_strchr(StringBuff,'\0'),"]");
	GUI_DispStringAtBar(StringBuff,0,2,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >4)&&(ParaSet.CursorPos <6))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		/*
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		TimeStatusBuff=atoi(StringBuff);
		*/
		my_strcpy(StringBuff,WeekToChinese(LocalTimerWeekNumber));
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-5))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	if((ParaSet.CursorPos >6)&&(ParaSet.CursorPos <8))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		/*
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		TimeStatusBuff=atoi(StringBuff);
		*/
		my_strcpy(StringBuff,*(Menu_OnOff[language].Text+WeekStatusBuff));
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-7))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	//my_strcpy(StringBuff,*(Menu_TimerSet[language].Text+1));
	//my_strcpy(my_strchr(StringBuff,'\0'),"[");
	
	my_strcpy(StringBuff,"[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[9],8);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"]��");
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);

	
	if((ParaSet.CursorPos >8)&&(ParaSet.CursorPos <11))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((13-(ParaSet.CursorPos-9))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >11)&&(ParaSet.CursorPos <14))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((10-(ParaSet.CursorPos-12))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >14)&&(ParaSet.CursorPos <17))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-15))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}



	//my_strcpy(StringBuff,*(Menu_TimerSet[language].Text+1));
	//my_strcpy(my_strchr(StringBuff,'\0'),"[");
	
	my_strcpy(StringBuff,"[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[18],8);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"]�ر�");
	GUI_DispStringAtBar(StringBuff,0,6,126,GUI_TA_HCENTER);

	if((ParaSet.CursorPos >17)&&(ParaSet.CursorPos <20))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((13-(ParaSet.CursorPos-18))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >20)&&(ParaSet.CursorPos <23))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((10-(ParaSet.CursorPos-21))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >23)&&(ParaSet.CursorPos <26))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-24))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
}
	


void DispMenuLocal_TimerUp(void)
{
	char StringBuff[16+1];
	//u8 LocalTimerNumber=0;
	TimerStructTypeDef TimerTimeBackup;
	
	//u8 LocalTimerStatus;
	//u8 LocalTimerWeekNumber;

	my_strncpy(StringBuff,&ParaSet.Text[0],2);
	StringBuff[2]='\0';
	LocalTimerNumberBackup=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[3],1);
	StringBuff[1]='\0';
	TimerTimeBackup.Status=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[5],1);
	StringBuff[1]='\0';
	LocalTimerWeekNumber=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[7],1);
	StringBuff[1]='\0';
	LocalTimerStatus=atoi(StringBuff);
	
	TimerTimeBackup.Week =RamSetParameters.LedControl.Local_Timer[LocalTimerNumber].Week;
	TimerTimeBackup.Week &=~(1 << LocalTimerWeekNumber);
	TimerTimeBackup.Week |= (LocalTimerStatus << LocalTimerWeekNumber);
	
	my_strncpy(StringBuff,&ParaSet.Text[9],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Hours=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[12],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Minutes=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[15],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Seconds=atoi(StringBuff);
	

	my_strncpy(StringBuff,&ParaSet.Text[18],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Hours=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[21],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Minutes=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[24],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Seconds=atoi(StringBuff);


	if ((LocalTimerNumberBackup > TIMER_NUMBER) 	||
		(TimerTimeBackup.TimerOnTime.Hours > 23)   	|| 
		(TimerTimeBackup.TimerOnTime.Minutes > 59) 	||
		(TimerTimeBackup.TimerOnTime.Seconds > 59)  ||
		(TimerTimeBackup.TimerOffTime.Hours > 23)   || 
		(TimerTimeBackup.TimerOffTime.Minutes > 59) ||
		(TimerTimeBackup.TimerOffTime.Seconds > 59))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{	
		
		LocalTimerNumber=LocalTimerNumberBackup;
		RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]=TimerTimeBackup;

		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		
		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedControl.Local_Timer[LocalTimerNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]) / sizeof(uint8_t))); //��д�����ݳ���
					
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedControl.Local_Timer[LocalTimerNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure(!memcmp((U8*)&TimerTimeBackup,(U8*)&RamSetParameters.LedControl.Local_Timer[LocalTimerNumber],
									 (sizeof(RamSetParameters.LedControl.Local_Timer[LocalTimerNumber]) / sizeof(uint8_t))) ? 1:0);
	}
	Jump2Menu(Local_TimerIndex,FlashMode_ReFlash);
}


void DispMenuLocal_TimerDown(void)
{
	POP();
}
void DispMenuLocal_TimerLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(Local_TimerIndex,FlashMode_ReFlash);
}
void DispMenuLocal_TimerRight(void)
{
	ParaSetCursorRightMoves();

	Jump2Menu(Local_TimerIndex,FlashMode_ReFlash);
}
void DispMenuLocal_TimerEscUp(void)
{
	
}
void DispMenuLocal_TimerEscDown(void)
{
	
}
void DispMenuLocal_TimerEscOk(void)
{
	
}
	


u8 T1000TimerNumber =0;
u8 T1000TimerNumberBackup;
u8 T1000TimerStatus;
u8 T1000TimerWeekNumber =1;

void Update_T1000_Timer_Text(void)
{
	snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", T1000TimerNumber);
	ParaSet.Text[2]='-';
	snprintf((char*)&ParaSet.Text[3], 1+1,"%01d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].Status);
	ParaSet.Text[4]='-';
	snprintf((char*)&ParaSet.Text[5], 1+1,"%01d", T1000TimerWeekNumber);
	ParaSet.Text[6]='-';
	snprintf((char*)&ParaSet.Text[7], 1+1,"%01d", (RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].Week & (1 << T1000TimerWeekNumber)) ? 1:0);
	ParaSet.Text[8]='-';
	snprintf((char*)&ParaSet.Text[9], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOnTime.Hours);
	ParaSet.Text[11]=':';
	snprintf((char*)&ParaSet.Text[12], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOnTime.Minutes);
	ParaSet.Text[14]=':';
	snprintf((char*)&ParaSet.Text[15], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOnTime.Seconds);
	ParaSet.Text[17]='-';
	snprintf((char*)&ParaSet.Text[18], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOffTime.Hours);
	ParaSet.Text[20]=':';
	snprintf((char*)&ParaSet.Text[21], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOffTime.Minutes);
	ParaSet.Text[23]=':';
	snprintf((char*)&ParaSet.Text[24], 2+1,"%02d", RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].TimerOffTime.Seconds);
	ParaSet.Text[26]='\0';
}


//��ʱ��[01][��]
//����[һ] [��]
//[00:00:00] ��
//[00:00:00] �ر�

//------------------------------------------------------------------		
//{"T1000�ƴ���ʱ"}�˵����ܺ���
//------------------------------------------------------------------
void DispMenuT1000_TimerInit(void)
{
	char StringBuff[16+1];
	//u8 T1000TimerNumber=0;
	u8 TimerStatusBuff=0;
	u8 WeekStatusBuff;
	
	//u8 TimeOnOffBuff=0;
	//u8 WeekBuff=0;

	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00-0-1-0-00:00:00-00:00:00");
		my_strcpy(ParaSet.MaxText, "99-1-7-1-29:59:59-29:59:59");
		
		Update_T1000_Timer_Text();
		ParaSet.CursorPos=0;
	}
	
	if(Flash == FlashMode_ReFlash) 
	{
		if (ParaSet.CursorPos < 2)//��0����,��ʼ��Item��ֵ
		{
			my_strncpy(StringBuff, &ParaSet.Text[0], 2);
			StringBuff[2]='\0';
			T1000TimerNumberBackup=atoi(StringBuff);

			if (T1000TimerNumberBackup > TIMER_NUMBER-1)
			{
				DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
				snprintf((char*)&ParaSet.Text, 2+1,"%02d", T1000TimerNumber);
				ParaSet.Text[2]='-';
				T1000TimerNumberBackup = T1000TimerNumber;
			}
			if (T1000TimerNumberBackup != T1000TimerNumber)
			{	
				T1000TimerNumber = T1000TimerNumberBackup;	
				Update_T1000_Timer_Text();
			}
		}else if ((ParaSet.CursorPos > 4) && (ParaSet.CursorPos < 6))//��0����,��ʼ��Item��ֵ
		{
			my_strncpy(StringBuff,&ParaSet.Text[5],1);
			StringBuff[1]='\0';
			T1000TimerWeekNumber=atoi(StringBuff);
			snprintf((char*)&ParaSet.Text[7], 1+1,"%01d", (RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].Week & (1 << T1000TimerWeekNumber)) ? 1:0);
			ParaSet.Text[8]='-';
		}
	}
	
	GUI_Clear();
	//GUI_DispStringAtBar(*(Menu_LedControlSet[language].Text+Item),0,0,126,GUI_TA_HCENTER);

	my_strncpy(StringBuff,&ParaSet.Text[3],1);
	StringBuff[1]='\0';
	TimerStatusBuff=atoi(StringBuff);

	my_strcpy(StringBuff,"��ʱ��[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[0],2);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"][");
	my_strcpy(my_strchr(StringBuff,'\0'),*(Menu_OnOff[language].Text+TimerStatusBuff));
	my_strcpy(my_strchr(StringBuff,'\0'),"]");
	
	GUI_DispStringAtBar(StringBuff,0,0,126,GUI_TA_HCENTER);
	
	if(ParaSet.CursorPos <2)
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-0))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >2)&&(ParaSet.CursorPos <4))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strcpy(StringBuff,*(Menu_OnOff[language].Text+TimerStatusBuff));
		//StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-3))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	
	my_strncpy(StringBuff,&ParaSet.Text[5],1);
	StringBuff[1]='\0';
	T1000TimerWeekNumber=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[7],1);
	StringBuff[1]='\0';
	WeekStatusBuff=atoi(StringBuff);
	
	my_strcpy(StringBuff,"����[");
	my_strcpy(my_strchr(StringBuff,'\0'),WeekToChinese(T1000TimerWeekNumber));
	my_strcpy(my_strchr(StringBuff,'\0'),"][");
	my_strcpy(my_strchr(StringBuff,'\0'),*(Menu_OnOff[language].Text+WeekStatusBuff));
	my_strcpy(my_strchr(StringBuff,'\0'),"]");
	GUI_DispStringAtBar(StringBuff,0,2,126,GUI_TA_HCENTER);
	
	if((ParaSet.CursorPos >4)&&(ParaSet.CursorPos <6))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		/*
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		TimeStatusBuff=atoi(StringBuff);
		*/
		my_strcpy(StringBuff,WeekToChinese(T1000TimerWeekNumber));
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-5))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
	
	if((ParaSet.CursorPos >6)&&(ParaSet.CursorPos <8))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		/*
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		TimeStatusBuff=atoi(StringBuff);
		*/
		my_strcpy(StringBuff,*(Menu_OnOff[language].Text+WeekStatusBuff));
		GUI_DispStringAt(StringBuff, PosX-((3-(ParaSet.CursorPos-7))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	//my_strcpy(StringBuff,*(Menu_TimerSet[language].Text+1));
	//my_strcpy(my_strchr(StringBuff,'\0'),"[");
	
	my_strcpy(StringBuff,"[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[9],8);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"]��");
	GUI_DispStringAtBar(StringBuff,0,4,126,GUI_TA_HCENTER);

	
	if((ParaSet.CursorPos >8)&&(ParaSet.CursorPos <11))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((13-(ParaSet.CursorPos-9))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >11)&&(ParaSet.CursorPos <14))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((10-(ParaSet.CursorPos-12))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >14)&&(ParaSet.CursorPos <17))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-15))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}



	//my_strcpy(StringBuff,*(Menu_TimerSet[language].Text+1));
	//my_strcpy(my_strchr(StringBuff,'\0'),"[");
	
	my_strcpy(StringBuff,"[");
	my_strncpy(my_strchr(StringBuff,'\0'),&ParaSet.Text[18],8);
	StringBuff[9]='\0';
	my_strcpy(my_strchr(StringBuff,'\0'),"]�ر�");
	GUI_DispStringAtBar(StringBuff,0,6,126,GUI_TA_HCENTER);

	if((ParaSet.CursorPos >17)&&(ParaSet.CursorPos <20))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((13-(ParaSet.CursorPos-18))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >20)&&(ParaSet.CursorPos <23))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((10-(ParaSet.CursorPos-21))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}

	if((ParaSet.CursorPos >23)&&(ParaSet.CursorPos <26))
	{
		GUI_SetTextMode(GUI_TEXTMODE_UNDERLINE);
		GUI_SetEnFont(En_8x16);
		my_strncpy(StringBuff,&ParaSet.Text[ParaSet.CursorPos],1);
		StringBuff[1]='\0';
		GUI_DispStringAt(StringBuff, PosX-((7-(ParaSet.CursorPos-24))*8), PosY);
		GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	}
}
	


void DispMenuT1000_TimerUp(void)
{
	char StringBuff[16+1];
	//u8 T1000TimerNumber=0;
	TimerStructTypeDef TimerTimeBackup;
	
	//u8 T1000TimerStatus;
	//u8 T1000TimerWeekNumber;

	my_strncpy(StringBuff,&ParaSet.Text[0],2);
	StringBuff[2]='\0';
	T1000TimerNumberBackup=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[3],1);
	StringBuff[1]='\0';
	TimerTimeBackup.Status=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[5],1);
	StringBuff[1]='\0';
	T1000TimerWeekNumber=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[7],1);
	StringBuff[1]='\0';
	T1000TimerStatus=atoi(StringBuff);
	
	TimerTimeBackup.Week =RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber].Week;
	TimerTimeBackup.Week &=~(1 << T1000TimerWeekNumber);
	TimerTimeBackup.Week |= (T1000TimerStatus << T1000TimerWeekNumber);
	
	my_strncpy(StringBuff,&ParaSet.Text[9],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Hours=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[12],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Minutes=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[15],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOnTime.Seconds=atoi(StringBuff);
	

	my_strncpy(StringBuff,&ParaSet.Text[18],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Hours=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[21],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Minutes=atoi(StringBuff);
	
	my_strncpy(StringBuff,&ParaSet.Text[24],2);
	StringBuff[2]='\0';
	TimerTimeBackup.TimerOffTime.Seconds=atoi(StringBuff);


	if ((T1000TimerNumberBackup > TIMER_NUMBER) 	||
		(TimerTimeBackup.TimerOnTime.Hours > 23)   	|| 
		(TimerTimeBackup.TimerOnTime.Minutes > 59) 	||
		(TimerTimeBackup.TimerOnTime.Seconds > 59)  ||
		(TimerTimeBackup.TimerOffTime.Hours > 23)   || 
		(TimerTimeBackup.TimerOffTime.Minutes > 59) ||
		(TimerTimeBackup.TimerOffTime.Seconds > 59))
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{	
		
		T1000TimerNumber=T1000TimerNumberBackup;
		RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]=TimerTimeBackup;

		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 	
		FLASH_READ(RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal,RamSetParameters.DeviceTotal); 
		#endif

		
		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]) / sizeof(uint8_t))); //��д�����ݳ���
					
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber],//��д�����ݻ�����
					(sizeof(RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure(!memcmp((U8*)&TimerTimeBackup,(U8*)&RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber],
									 (sizeof(RamSetParameters.LedControl.T1000_Timer[T1000TimerNumber]) / sizeof(uint8_t))) ? 1:0);
	}
	Jump2Menu(T1000_TimerIndex,FlashMode_ReFlash);
}


void DispMenuT1000_TimerDown(void)
{
	POP();
}
void DispMenuT1000_TimerLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(T1000_TimerIndex,FlashMode_ReFlash);
}
void DispMenuT1000_TimerRight(void)
{
	ParaSetCursorRightMoves();

	Jump2Menu(T1000_TimerIndex,FlashMode_ReFlash);
}
void DispMenuT1000_TimerEscUp(void)
{
	
}
void DispMenuT1000_TimerEscDown(void)
{
	
}
void DispMenuT1000_TimerEscOk(void)
{
	
}

//------------------------------------------------------------------		
//������Ϸʧ����ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetGameFailDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00");
		my_strcpy(ParaSet.MaxText, "99");
		
		snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", RamSetParameters.GameFailDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GameFailDelayTime_PosX , GameFailDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//������Ϸʧ����ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetGameFailDelayTimeUp(void)
{
	U16 GameFailDelayTimeBackup;
	
	RamSetParameters.GameFailDelayTime=atoi((char *)ParaSet.Text);

	GameFailDelayTimeBackup = RamSetParameters.GameFailDelayTime;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameFailDelayTime,RamSetParameters.GameFailDelayTime,RamSetParameters.GameFailDelayTime);     
       FLASH_READ(RamSetParameters.GameFailDelayTime,RamSetParameters.GameFailDelayTime,RamSetParameters.GameFailDelayTime); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameFailDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameFailDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameFailDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameFailDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameFailDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameFailDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((GameFailDelayTimeBackup == RamSetParameters.GameFailDelayTime) ? 1:0);

	Jump2Menu(SetGameFailDelayTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//������Ϸʧ����ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetGameFailDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//������Ϸʧ����ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetGameFailDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetGameFailDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//������Ϸʧ����ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetGameFailDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetGameFailDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetGameFailDelayTimeEscUp(void)
{
	
}
void DispMenuSetGameFailDelayTimeEscDown(void)
{

}
void DispMenuSetGameFailDelayTimeEscOk(void)
{
	
}	





//------------------------------------------------------------------		
//������Ϸ�ȴ���ʱʱ���ʼ��
//------------------------------------------------------------------
void DispMenuSetGameWaitDelayTimeInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "00");
		my_strcpy(ParaSet.MaxText, "99");
		
		snprintf((char*)&ParaSet.Text[0], 2+1,"%02d", RamSetParameters.GameWaitDelayTime);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GameWaitDelayTime_PosX , GameWaitDelayTime_PosY, 126, GUI_TA_HCENTER);
    GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_S_Index), PosX, PosY);
}


//------------------------------------------------------------------		
//������Ϸ�ȴ���ʱʱ�����
//------------------------------------------------------------------
void DispMenuSetGameWaitDelayTimeUp(void)
{
	U16 GameWaitDelayTimeBackup;
	
	RamSetParameters.GameWaitDelayTime=atoi((char *)ParaSet.Text);

	GameWaitDelayTimeBackup = RamSetParameters.GameWaitDelayTime;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.GameWaitDelayTime,RamSetParameters.GameWaitDelayTime,RamSetParameters.GameWaitDelayTime);     
       FLASH_READ(RamSetParameters.GameWaitDelayTime,RamSetParameters.GameWaitDelayTime,RamSetParameters.GameWaitDelayTime); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameWaitDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameWaitDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameWaitDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameWaitDelayTime-(U32)&RamSetParameters,//��д���ַ
                   (uint8_t *)&RamSetParameters.GameWaitDelayTime,//��д�����ݻ�����
                   (sizeof(RamSetParameters.GameWaitDelayTime) / sizeof(uint8_t))); //��д�����ݳ���
           
#endif
    
	DisplaySuccessFailure((GameWaitDelayTimeBackup == RamSetParameters.GameWaitDelayTime) ? 1:0);

	Jump2Menu(SetGameWaitDelayTimeIndex,FlashMode_ReFlash);
}


//------------------------------------------------------------------		
//������Ϸ�ȴ���ʱʱ�䷵��
//------------------------------------------------------------------
void DispMenuSetGameWaitDelayTimeDown(void)
{
	POP();
}

//------------------------------------------------------------------		
//������Ϸ�ȴ���ʱʱ�����ּ�
//------------------------------------------------------------------
void DispMenuSetGameWaitDelayTimeLeft(void)
{
	ParaSetAdd();
	Jump2Menu(SetGameWaitDelayTimeIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//������Ϸ�ȴ���ʱʱ��������
//------------------------------------------------------------------
void DispMenuSetGameWaitDelayTimeRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();
	Jump2Menu(SetGameWaitDelayTimeIndex,FlashMode_ReFlash);
}


void DispMenuSetGameWaitDelayTimeEscUp(void)
{
	
}
void DispMenuSetGameWaitDelayTimeEscDown(void)
{

}
void DispMenuSetGameWaitDelayTimeEscOk(void)
{
	
}	






//------------------------------------------------------------------		
//������Ϸ��ʼ�豸��ַ��ʼ��
//------------------------------------------------------------------
void DispMenuSetGameStartDeviceAddrInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.GameStartDeviceAddr);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GameStartDeviceAddr_PosX , GameStartDeviceAddr_PosY, 126, GUI_TA_HCENTER);
    //GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
}

//------------------------------------------------------------------		
//������Ϸ��ʼ�豸��ַ���
//------------------------------------------------------------------
void DispMenuSetGameStartDeviceAddrUp(void)
{
	U16 GameStartDeviceAddrBackup;
	
	//RamSetParameters.GameStartDeviceAddr=atoi((char *)ParaSet.Text);
	//GameStartDeviceAddrBackup = RamSetParameters.GameStartDeviceAddr;
	
	
	GameStartDeviceAddrBackup=atoi((char *)ParaSet.Text);

	if (GameStartDeviceAddrBackup >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.GameStartDeviceAddr=GameStartDeviceAddrBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.GameStartDeviceAddr,RamSetParameters.GameStartDeviceAddr,RamSetParameters.GameStartDeviceAddr); 	
		FLASH_READ(RamSetParameters.GameStartDeviceAddr,RamSetParameters.GameStartDeviceAddr,RamSetParameters.GameStartDeviceAddr); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameStartDeviceAddr-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.GameStartDeviceAddr,//��д�����ݻ�����
					(sizeof(RamSetParameters.GameStartDeviceAddr) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameStartDeviceAddr-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.GameStartDeviceAddr,//��д�����ݻ�����
					(sizeof(RamSetParameters.GameStartDeviceAddr) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((GameStartDeviceAddrBackup == RamSetParameters.GameStartDeviceAddr) ? 1:0);
	}
	Jump2Menu(SetGameStartDeviceAddrIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//������Ϸ��ʼ�豸��ַ����
//------------------------------------------------------------------
void DispMenuSetGameStartDeviceAddrDown(void)
{
	//if ((GlobalKeyVal & (KEY_LONG_UP <<6 ))== (KEY_SHORT_UP <<6 ))
	{
		POP();
	}
}
//------------------------------------------------------------------		
//������Ϸ��ʼ�豸��ַ���ּ�
//------------------------------------------------------------------
void DispMenuSetGameStartDeviceAddrLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(SetGameStartDeviceAddrIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//������Ϸ��ʼ�豸��ַ�������
//------------------------------------------------------------------
void DispMenuSetGameStartDeviceAddrRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(SetGameStartDeviceAddrIndex,FlashMode_ReFlash);
}



void DispMenuSetGameStartDeviceAddrEscUp(void)
{
	//ParaSetCursorLeftMoves();
	//Jump2Menu(SetGameStartDeviceAddrIndex,FlashMode_ReFlash);
}
void DispMenuSetGameStartDeviceAddrEscDown(void)
{
	//ParaSetCursorRightMoves();
	//Jump2Menu(SetGameStartDeviceAddrIndex,FlashMode_ReFlash);
	
}
void DispMenuSetGameStartDeviceAddrEscOk(void)
{
	
}	




//------------------------------------------------------------------		
//������Ϸ�����豸��ַ��ʼ��
//------------------------------------------------------------------
void DispMenuSetGameEndDeviceAddrInit(void)
{
	if(Flash == FlashMode_AutoInit)//��0����,��ʼ��Item��ֵ
	{
        //Item = 0;
        Where = 0;
		//-------------------------
		my_strcpy(ParaSet.MinText, "000");
		my_strcpy(ParaSet.MaxText, "999");
		
		snprintf((char*)&ParaSet.Text[0], 3+1,"%03d", RamSetParameters.GameEndDeviceAddr);
		ParaSet.CursorPos=0;
	}
	GUI_Clear();
	GUI_DispStringAtBar(*(Menu_GameModeSet[language].Text+Item),0,1,126,GUI_TA_HCENTER);
	ParaSetDisplsy(GameEndDeviceAddr_PosX , GameEndDeviceAddr_PosY, 126, GUI_TA_HCENTER);
    //GUI_DispStringAt(*(Menu_Unit[language].Text+Unit_PCS_Index), PosX, PosY);
}

//------------------------------------------------------------------		
//������Ϸ�����豸��ַ���
//------------------------------------------------------------------
void DispMenuSetGameEndDeviceAddrUp(void)
{
	U16 GameEndDeviceAddrBackup;
	
	//RamSetParameters.GameEndDeviceAddr=atoi((char *)ParaSet.Text);
	//GameEndDeviceAddrBackup = RamSetParameters.GameEndDeviceAddr;
	
	
	GameEndDeviceAddrBackup=atoi((char *)ParaSet.Text);

	if (GameEndDeviceAddrBackup >MAX_DEVICE_NUM)
	{
		DisplayPromptInfo(*(Menu_PromptInfo[language].Text+0));
	}
	else
	{
		
		RamSetParameters.GameEndDeviceAddr=GameEndDeviceAddrBackup;
		
		#ifdef INT_FLASH_SAVE
		FLASH_WRITE(RamSetParameters.GameEndDeviceAddr,RamSetParameters.GameEndDeviceAddr,RamSetParameters.GameEndDeviceAddr); 	
		FLASH_READ(RamSetParameters.GameEndDeviceAddr,RamSetParameters.GameEndDeviceAddr,RamSetParameters.GameEndDeviceAddr); 
		#endif

		#ifdef EEPROM_SAVE
		AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameEndDeviceAddr-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.GameEndDeviceAddr,//��д�����ݻ�����
					(sizeof(RamSetParameters.GameEndDeviceAddr) / sizeof(uint8_t))); //��д�����ݳ���
		
		AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.GameEndDeviceAddr-(U32)&RamSetParameters,//��д���ַ
					(uint8_t *)&RamSetParameters.GameEndDeviceAddr,//��д�����ݻ�����
					(sizeof(RamSetParameters.GameEndDeviceAddr) / sizeof(uint8_t))); //��д�����ݳ���
		
		#endif
	   
		DisplaySuccessFailure((GameEndDeviceAddrBackup == RamSetParameters.GameEndDeviceAddr) ? 1:0);
	}
	Jump2Menu(SetGameEndDeviceAddrIndex,FlashMode_ReFlash);
}

//------------------------------------------------------------------		
//������Ϸ�����豸��ַ����
//------------------------------------------------------------------
void DispMenuSetGameEndDeviceAddrDown(void)
{
	//if ((GlobalKeyVal & (KEY_LONG_UP <<6 ))== (KEY_SHORT_UP <<6 ))
	{
		POP();
	}
}
//------------------------------------------------------------------		
//������Ϸ�����豸��ַ���ּ�
//------------------------------------------------------------------
void DispMenuSetGameEndDeviceAddrLeft(void)
{
	ParaSetAdd();
	
	Jump2Menu(SetGameEndDeviceAddrIndex,FlashMode_ReFlash);
	
}

//------------------------------------------------------------------		
//������Ϸ�����豸��ַ�������
//------------------------------------------------------------------
void DispMenuSetGameEndDeviceAddrRight(void) 
{
	//ParaSetSub();
	
	ParaSetCursorRightMoves();

	Jump2Menu(SetGameEndDeviceAddrIndex,FlashMode_ReFlash);
}



void DispMenuSetGameEndDeviceAddrEscUp(void)
{
	//ParaSetCursorLeftMoves();
	//Jump2Menu(SetGameEndDeviceAddrIndex,FlashMode_ReFlash);
}
void DispMenuSetGameEndDeviceAddrEscDown(void)
{
	//ParaSetCursorRightMoves();
	//Jump2Menu(SetGameEndDeviceAddrIndex,FlashMode_ReFlash);
	
}
void DispMenuSetGameEndDeviceAddrEscOk(void)
{
	
}	
