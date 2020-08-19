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

#include "./mcohw.h"
#include "stm32f2xx.h"

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

// Pull the next CAN message from receive buffer
BYTE MCOHW_PullMessage (CAN_MSG MEM_FAR *pReceiveBuf)
{
    BYTE i;
    CanRxMsg RxMessage;
    CAN_Receive(CAN,CAN_FIFO0,&RxMessage);
    pReceiveBuf->ID = RxMessage.StdId;
    pReceiveBuf->LEN = RxMessage.DLC;
    for(i = 0; i < pReceiveBuf->LEN; i++)
        pReceiveBuf->BUF[i] = RxMessage.Data[i]; 
    return 1; // Return False, no msg rcvd 
}

// Push the next transmit message into transmit queue
BYTE MCOHW_PushMessage (CAN_MSG MEM_FAR *pTransmitBuf)
{  
    BYTE i;
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
	return (1);
}


BYTE MCOHW_SetCANFilter (WORD CANID)
{
    BYTE p;
    WORD buf0, buf1;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
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
        CAN_FilterInitStructure.CAN_FilterNumber = p;
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
        CAN_FilterInitStructure.CAN_FilterIdHigh = gFilterList[p] << 5;
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);
        p++;
    }   
    return 1;
}


/*----------------------- END OF FILE ----------------------------------*/

