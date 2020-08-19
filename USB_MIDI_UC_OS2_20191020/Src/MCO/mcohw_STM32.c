/**************************************************************************
MODULE:    MCOHW_LPCAN2
CONTAINS:  Preliminary, limited hardware driver implementation for 
Philips LPCAN2000 derivatives with CAN interface.
Compiled and Tested with Keil Tools www.keil.com
COPYRIGHT: Embedded Systems Academy, Inc. 2002-2004.
All rights reserved. www.microcanopen.com
This software was written in accordance to the guidelines at
www.esacademy.com/software/softwarestyleguide.pdf
DISCLAIM:  Read and understand our disclaimer before using this code!
www.esacademy.com/disclaim.htm
LICENSE:   Users that have purchased a license for CANopen Magic
(www.CANopenMagic.com)
may use this code in commercial projects.
Otherwise only educational use is acceptable.
VERSION:   2.01, Pf 13-MAY-04
---------------------------------------------------------------------------
HISTORY:   2.01, Pf 13-MAY-04, FullCAN SW mode implemented
1.21, Pf 09-FEB-04, Timer and second CAN interface added
1.20, Pf 12-OCT-03, First Implementation for Philips LPCAN222
1.10, Pf 27-MAY-03, Cleaned file to eliminate compiler warnings
Support of MEMORY types
1.00, Ck 07-OCT-02, First Published Version
***************************************************************************/ 

//#include <LPCAN21XX.H> // LPCAN21XX Peripheral Register
#include "stm32f2xx.h"
#include "typedef.h"
#include "mcohw.h"

#include "config.h"
// Maximum number of FullCAN Filters
#define MAX_FILTERS 6

// Global timer/conter variable, incremented every millisecond¡ø
WORD volatile gTimCnt = 0;

// Counts number of filters (CAN message objects) used
WORD volatile gCANFilter = 0;

// FullCAN Message List
CAN_MSG volatile gFullCANList[MAX_FILTERS];

// Lookup table for receive filters
WORD gFilterList[MAX_FILTERS];                           

extern CAN_HandleTypeDef CAN1_HandleStructure;
extern CAN_HandleTypeDef CAN2_HandleStructure;

/*
u8   Baud_Data=0x01;
//u16 setData=0x45;

extern OD_TABLE OD[MAX_OD_SIZE];
extern WORD volatile gTimCnt;
extern BYTE ODNodeID;

typedef struct
{
  u8 BaudRate_Data;
  u8 ODNodeID_Data;
}e2prom;
e2prom Image;
#define SIZE sizeof(Image)

u32 *pTPRO1_val[2];
u32 *pRPRO1_val[2];

void initTimer(void);
unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);
*/

// Pull the next CAN message from receive buffer
BYTE MCOHW_PullMessage (CAN_MSG MEM_FAR *pReceiveBuf)
{
    BYTE i;
	CAN_HandleTypeDef*  CANx_Handle=&CAN1_HandleStructure;
	/*
    CanRxMsg RxMessage;
    CAN_Receive(CAN,CAN_FIFO0,&RxMessage);
    pReceiveBuf->ID = RxMessage.StdId;
    pReceiveBuf->LEN = RxMessage.DLC;
    for(i = 0; i < pReceiveBuf->LEN; i++)
        pReceiveBuf->BUF[i] = RxMessage.Data[i]; 
    return 1; // Return False, no msg rcvd 
	*/

	HAL_CAN_Receive(CANx_Handle, CAN_FIFO0, 10);
	pReceiveBuf->ID = (uint16_t)(CANx_Handle->pRxMsg->StdId);
    pReceiveBuf->LEN = CANx_Handle->pRxMsg->DLC;
    for(i = 0; i < pReceiveBuf->LEN; i++)
        pReceiveBuf->BUF[i] = CANx_Handle->pRxMsg->Data[i];
    return 1; // Return False, no msg rcvd 	
}



// Push the next transmit message into transmit queue
BYTE MCOHW_PushMessage (CAN_MSG MEM_FAR *pTransmitBuf)
{  
    BYTE i;
	
	CAN_HandleTypeDef*  CANx_Handle=&CAN1_HandleStructure;
	
	/*
    CanTxMsg TxMessage;
    TxMessage.StdId = pTransmitBuf->ID;
    TxMessage.ExtId = 0x00;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = pTransmitBuf->LEN;
    for(i = 0; i < pTransmitBuf->LEN; i++)
		TxMessage.Data[i] = pTransmitBuf->BUF[i];    
    if (CAN_Transmit(CAN,&TxMessage) != CAN_NO_MB)
    {
        return 1;
    }
    else
    {
        return 0;
    }
	*/
	
    CANx_Handle->pTxMsg->StdId = pTransmitBuf->ID;
    CANx_Handle->pTxMsg->ExtId = 0x00;
    CANx_Handle->pTxMsg->IDE = CAN_ID_STD;
    CANx_Handle->pTxMsg->RTR = CAN_RTR_DATA;
    CANx_Handle->pTxMsg->DLC = pTransmitBuf->LEN;
    for(i = 0; i < pTransmitBuf->LEN; i++)
		CANx_Handle->pTxMsg->Data[i] = pTransmitBuf->BUF[i];    
	if (HAL_CAN_Transmit(CANx_Handle, 10)==HAL_OK)	
    //if (CAN_Transmit(CAN,&TxMessage) != CAN_NO_MB)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/**************************************************************************
DOES: Reads and returns the value of the current 1 millisecond system
timer tick.
**************************************************************************/
WORD MCOHW_GetTime (void)
{
    return gTimCnt;
}
//WORD TimCount;
void CanTimTik(void)
{
    gTimCnt++;
}
// Checks if a TimeStamp expired
BYTE MCOHW_IsTimeExpired(WORD timestamp)
{
	WORD time_now;
  	time_now = gTimCnt;
  	if (time_now > timestamp)
  	{
    	if ((time_now - timestamp) < 0x8000)
      		return 1;
    	else
      		return 0;
  	}
  	else
  	{
    	if ((timestamp - time_now) > 0x8000)
      		return 1;
    	else
      		return 0;
  	}
}
// Init CAN Interface and Timer
BYTE MCOHW_Init (void)
{
	
	
	/*
	Image.ODNodeID_Data=1;
	Image.BaudRate_Data=9;

	ODNodeID=Image.ODNodeID_Data;
	Baud_Data=Image.BaudRate_Data;
	//SYS_Tick_Configuration();
	//NVIC_Configuration();

	initTimer();
	canInit(CAN1,CAN_BAUD_125K);    
	MCOUSER_ResetCommunication();
	
	
	pTPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x2000,0x00)].Val;
	pTPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x2000,0x01)].Val;
	
	pRPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x6000,0x00)].Val;
	pRPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x6000,0x01)].Val;
	
	*/
	
	
	
	
	
	return (1);
}


BYTE MCOHW_SetCANFilter (WORD CANID)
{
    BYTE p;
    WORD buf0, buf1;
	
	CAN_FilterConfTypeDef CAN_FilterConfStructure;	
	CAN_HandleTypeDef*  CANx_Handle=&CAN1_HandleStructure;
	
    if (gCANFilter == 0)
    { // First call, init entry zero
        gFilterList[0] = 0x17FF; // Disabled and unused
    }
    if (gCANFilter >= MAX_FILTERS)
    {
		return 0;
    }
	
    // Filters must be sorted by priority
	
    // new filter is sorted into array
    p = 0;
    while (p < gCANFilter) // loop through all existing filters 
    {
        if (gFilterList[p] > CANID)
        {
            break;
        }
        p++;
    }
    // insert new filter here
    buf0 = gFilterList[p]; // save current entry
    gFilterList[p] = CANID; // insert the new entry
    // move all remaining entries one row up
    gCANFilter++;
    while (p < gCANFilter)
    {
        p++;
        buf1 = gFilterList[p];
        gFilterList[p] = buf0;
        buf0 = buf1;
    }
    p = 0;
    while (p < gCANFilter) // loop through all existing filters 
    {
        CAN_FilterConfStructure.FilterNumber = p;
        CAN_FilterConfStructure.FilterMode = CAN_FILTERMODE_IDLIST;
        CAN_FilterConfStructure.FilterScale = CAN_FILTERSCALE_32BIT;
        CAN_FilterConfStructure.FilterIdHigh = gFilterList[p] << 5;
        CAN_FilterConfStructure.FilterIdLow = 0x0000;
        CAN_FilterConfStructure.FilterMaskIdHigh = 0x0000;
        CAN_FilterConfStructure.FilterMaskIdLow = 0x0000;
        CAN_FilterConfStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        CAN_FilterConfStructure.FilterActivation = ENABLE;
        HAL_CAN_ConfigFilter(CANx_Handle,&CAN_FilterConfStructure);
        p++;
    }   
    return 1;
}


/*----------------------- END OF FILE ----------------------------------*/

