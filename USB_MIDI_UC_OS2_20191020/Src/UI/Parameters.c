
#include "Parameters.h"
#include "Menu.H"
_CONST_ ParametersStructTypeDef DefaultParameters = DEFAULT_PARAMETERS; 


_CONST_ ParametersStructTypeDef FlashSetParameters __attribute__((at(FlashSetParametersStartAddress)))= DEFAULT_PARAMETERS;
//_CONST_ U8 ParametersSetReservedRoom[ParametersSetReservedLen] __attribute__((at(ParametersSetReservedStartAddress))) = "ParametersSetReservedRoom"; 	
 
 
//ParametersStructTypeDef Backup_RamSetParameters;// = DEFAULT_PARAMETERS;
ParametersStructTypeDef RamSetParameters;// = DEFAULT_PARAMETERS;


SequenListTypeDef L={RamSetParameters.GameRanking,&RamSetParameters.GameRankingNumber};


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
