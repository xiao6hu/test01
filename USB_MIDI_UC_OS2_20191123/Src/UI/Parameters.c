
#include "Parameters.h"

#include "Menu.H"
_CONST_ ParametersStructTypeDef DefaultParameters = DEFAULT_PARAMETERS; 


_CONST_ ParametersStructTypeDef FlashSetParameters __attribute__((at(FlashSetParametersStartAddress)))= DEFAULT_PARAMETERS;
//_CONST_ U8 ParametersSetReservedRoom[ParametersSetReservedLen] __attribute__((at(ParametersSetReservedStartAddress))) = "ParametersSetReservedRoom"; 	
 
 
//ParametersStructTypeDef Backup_RamSetParameters;// = DEFAULT_PARAMETERS;
ParametersStructTypeDef RamSetParameters;// = DEFAULT_PARAMETERS;


SequenListTypeDef L={RamSetParameters.GameRanking,&RamSetParameters.GameRankingNumber};

char* GameVoice[10]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

void LodeParametersData(void)
{
    #ifdef INT_FLASH_SAVE
	FLASH_READ(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);	
    #endif

	#ifdef EEPROM_SAVE
        AT24CXX_Read(EepromSetParametersStartAddress, (uint8_t *)&RamSetParameters, (sizeof(ParametersStructTypeDef) / sizeof (uint8_t)));  
	#endif
	
	language =RamSetParameters.Language;	
}

//保存参数数据到片内Flash
void SaveParametersData(void)
{
    #ifdef INT_FLASH_SAVE
	FLASH_WRITE(FlashSetParameters, RamSetParameters, ParametersStructTypeDef);
    #endif
    
    #ifdef EEPROM_SAVE
	AT24CXX_CmpWrite(EepromSetParametersStartAddress, (uint8_t *)&FlashSetParameters, (uint8_t *)&RamSetParameters, (sizeof(ParametersStructTypeDef) / sizeof(uint8_t)));
    #endif
}

void ResetParametersData(void)
{

    #ifdef INT_FLASH_SAVE
	FLASH_WRITE(FlashSetParameters, DefaultParameters, ParametersStructTypeDef);	
    #endif
    #ifdef EEPROM_SAVE
        AT24CXX_Write(EepromSetParametersStartAddress, (uint8_t *)&DefaultParameters, (sizeof(ParametersStructTypeDef) / sizeof(uint8_t)));
	#endif
	
	LodeParametersData();
	
}

void ParaSetOk(void)
{
	INT8U err;
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) PARA_SET_OK_FLAG,
						(INT8U) OS_FLAG_SET,
						(INT8U  *)&err);		
}

void SlaveParaSet(void)
{
	switch (Packet.data)
	{
		//case 1: ParaSetStart();break;
		case PARA_SET_OK: ParaSetOk();break;
		default: break;	
	}	
}

void ParaUpdate(void)
{
	if (RamSetParameters.LedControl.LedColorCycleTime < 9999)
	{
		TxDeviceCmd(Packet.addr,PARA_UPDATE_CMD,RamSetParameters.LedPara.LedColorCycleMode);	
	}
}

#if 0
u8 Get_ExtMemoryData(void)
{
	FRESULT res;
	char* Path;
	const char *FolderName="VoiceTextFile";
	const char *FileName="VoiceText.txt";
	VoiceTextTypeDef VoiceText;
	u8 TextNumber; 
	u16 Textlength;
	char *PathBuff;
	size_t Length;



	if(f_mount(&fs, USBH_Path, 0) == FR_OK) 
	{
		Path = USBH_Path;
	}
	else if(f_mount(&fs, SD_Path, 0) == FR_OK) 
	{
		Path = SD_Path;
	}
	else
	{
		Return 0;
	}

	Length=strlen(Path);
	PathBuff=(char*)malloc(Length+1);
	strcpy(PathBuff,Path);
	
	Length +=strlen(FolderName);
	PathBuff=(char*)realloc(PathBuff,Length+1);
	strcpy(strchr(PathBuff, '\0'),FolderName);
	
	Length +=strlen(FileName);
	PathBuff=(char*)realloc(PathBuff,Length+1);
	strcpy(strchr(PathBuff, '\0'),FileName);


	//打开文件
	if (f_open(fp,PathBuff,FA_READ) != FR_OK) goto Get_ExtMemoryDataExit;
	if(f_read(fp,&VoiceText.TextNumber[0],sizeof(VoiceText.TextNumber)/sizeof(char),&br)!=FR_OK) goto Get_ExtMemoryDataExit;
	TextNumber = ((VoiceText.TextNumber[1]-0x30)*10) + (VoiceText.TextNumber[0]-0x30);
	TextNumber = TextNumber?TextNumber-1:TextNumber;
	VoiceText.TextData = GameVoice[TextNumber];
	f_lseek(f_tell(fp)+2);
	if(f_read(fp,&VoiceText.Textlength[0],sizeof(VoiceText.Textlength)/sizeof(char),&br)!=FR_OK) goto Get_ExtMemoryDataExit;
	Textlength = ((VoiceText.Textlength[3]-0x30)*1000) + ((VoiceText.Textlength[2]-0x30)*100) + ((Textlength[1]-0x30)*10) + (VoiceText.Textlength[0]-0x30);
	f_lseek(f_tell(fp)+2);
	VoiceText.TextData =(char*)malloc(Textlength+1);
	if(f_read(fp,VoiceText.TextData,Textlength,&br)!=FR_OK) goto Get_ExtMemoryDataExit;

Get_ExtMemoryDataExit:
	free(PathBuff);
	free(VoiceText.TextData);
	f_close(fp);
	return 1
}
#endif


