/**************************************************************************
MODULE:    MCO
CONTAINS:  MicroCANopen implementation
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
$LastChangedDate: 2004-05-13 01:45:49 -0700 (Thu, 13 May 2004) $
$LastChangedRevision: 5 $
---------------------------------------------------------------------------
HISTORY:   2.01, Pf 13-MAY-04, Implemented Philips LPC2129 support
HISTORY:   2.00, Pf 21-OCT-03, Minor changes to pass conformance test
1.20, Pf 19-AUG-03, Code changed to use process image
Bug fix for incorrect inhibit time handling
1.10, Pf 27-MAY-03, Bug fixes in OD (hi byte was corrupted)
OD changed to indefinite length
Support of define controled MEMORY types
1.01, Pf 17-DEC-02, Made Object Dictionary more readable
1.00, Pf 07-OCT-02, First Published Version
***************************************************************************/ 

#include "mco.h"
#include "mcohw.h"
#include <string.h>
/**************************************************************************
GLOBAL VARIABLES
***************************************************************************/ 
// this structure holds all master node specific configuration
MCO_CONFIG MEM_FAR gMCOConfig;
#if NR_OF_SLAVES > 0
// this structure holds all slave node specific configuration
MCO_SLAVE_CONFIG MEM_FAR gMCOSLVConfig[NR_OF_SLAVES];
#endif
#if NR_OF_SLAVES > 0
// this structure holds all the client sdo configuration data for up to 4 RPDOs
CLIENT_SDO_CONFIG MEM_FAR gCSDOConfig[NR_OF_SLAVES];
#endif
#if NR_OF_TPDOS > 0
// this structure holds all the TPDO configuration data for up to 4 TPDOs
TPDO_CONFIG MEM_FAR gTPDOConfig[NR_OF_TPDOS];
#endif

// this is the next TPDO to be checked in MCO_ProcessStack
BYTE MEM_FAR gTPDONr = NR_OF_TPDOS;

#if NR_OF_RPDOS > 0
// this structure holds all the RPDO configuration data for up to 4 RPDOs
RPDO_CONFIG MEM_FAR gRPDOConfig[NR_OF_RPDOS];
#endif

// this structure holds the current receive message
CAN_MSG MEM_FAR gRxCAN;
// this structure holds the current NMT message
CAN_MSG MEM_FAR gNMT;
// this structure holds the CAN message for SDO responses or aborts
CAN_MSG MEM_FAR gTxSDO;
NorSDO gNorSDO;
extern OD_TABLE OD[MAX_OD_SIZE];

// table with SDO Responses for read requests to OD - defined in user_xxx.c
extern BYTE MEM_CONST SDOResponseTable[];

/**************************************************************************
LOCAL FUNCTIONS
***************************************************************************/

// SDO Abort Messages
#define SDO_ABORT_NO_TOGGLE       0x05030000UL
#define SDO_ABORT_UNSUPPORTED     0x06010000UL
#define SDO_ABORT_NOT_EXISTS      0x06020000UL
#define SDO_ABORT_READONLY        0x06010002UL
#define SDO_ABORT_TYPEMISMATCH    0x06070010UL
#define SDO_ABORT_UNKNOWN_COMMAND 0x05040001UL
#define SDO_ABORT_UNKNOWNSUB      0x06090011UL
/**************************************************************************
DOES:    Generates an SDO Abort Response
RETURNS: nothing
**************************************************************************/
void MCO_Send_SDO_Abort
(
DWORD ErrorCode  // 4 byte SDO abort error code
)
{
	BYTE i;
	
	// construct message data
	gTxSDO.BUF[0] = 0x80;
	for (i=0;i<4;i++)
	{
		gTxSDO.BUF[4+i] = ErrorCode;
		ErrorCode >>= 8;
	}
	
	// transmit message
	if (!MCOHW_PushMessage(&gTxSDO))
	{
		// failed to transmit
		MCOUSER_FatalError(0x8801);
	}
}

#if NR_OF_SLAVES > 0
/**************************************************************************
DOES:    Call-back function to get node state
**************************************************************************/
BYTE MCOUSER_GetNodeState (BYTE Node_ID)
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	        return gMCOSLVConfig[i].Node_State;
	    }
        i++;
    } // for all client sdo 
    return 0xff;
}
/**************************************************************************
DOES:    Call-back function to get node SDO_abort_code
**************************************************************************/
BYTE MCOUSER_GetNodeSDOAbortCode (BYTE Node_ID)
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	        return gCSDOConfig[i].SDO_abort_code;
	    }
        i++;
    } // for all client sdo 
    return 0xff;
}
/**************************************************************************
DOES:    Call-back function for request sdo download
**************************************************************************/
BYTE SDO_RequestDownload (BYTE Node_ID, WORD Index, WORD SubIndex, DWORD Len, BYTE* LpData)//±£´æ
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	    	// is this node SDO in empty state?
	        if (gCSDOConfig[i].SDO_state == 0 && gMCOSLVConfig[i].Node_State != 4 && gMCOSLVConfig[i].Node_State != 0x0f)
	        {
	        	gCSDOConfig[i].SDO_LEN = Len;
	        	gCSDOConfig[i].SDO_LP = LpData;
	         	gCSDOConfig[i].TXSDO.BUF[1] = Index & 0x00ff;
	         	gCSDOConfig[i].TXSDO.BUF[2] = (Index >> 8) & 0x00ff;
	         	gCSDOConfig[i].TXSDO.BUF[3] = SubIndex;
	         	if (Len < 5)
	         	{
	         		gCSDOConfig[i].TXSDO.BUF[0] = 0x20 + (((4 - Len) & 0x03) << 2) + 0x03;
	         		while (Len > 0)
	         		{
	         			Len--;
	         			gCSDOConfig[i].TXSDO.BUF[4+Len] = *(LpData + Len);
					}
				}
				else
				{
					gCSDOConfig[i].TXSDO.BUF[0] = 0x21;
					gCSDOConfig[i].TXSDO.BUF[4] = Len & 0x000000ff;
	         			gCSDOConfig[i].TXSDO.BUF[5] = (Len >> 8)  & 0x000000ff;
	         			gCSDOConfig[i].TXSDO.BUF[6] = (Len >> 16) & 0x000000ff;
	         			gCSDOConfig[i].TXSDO.BUF[7] = (Len >> 24) & 0x000000ff;	
				}
				// set this SDO into request tx state
	         	gCSDOConfig[i].SDO_state = 1;
	         	gCSDOConfig[i].SDO_timeout_times = 0;
	         	gCSDOConfig[i].SDO_abort_code = 0;
	          	// exit the loop
	          	return 1;
	        }
	    }
        i++;
    } // for all client sdo 
    return 0;
}
/**************************************************************************
DOES:    Call-back function to confirm sdo download
**************************************************************************/
BYTE SDO_ConfirmDownload (BYTE Node_ID)
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	    	// is this node SDO in respons received state?
	        if (gCSDOConfig[i].SDO_state == 3)
	        {
	        	//confirm the respons
	        	gCSDOConfig[i].SDO_state = 0;
	          	// exit the loop return ture
	          	return 1;
	        }
	    }
        i++;
    } // for all client sdo 
    return 0;
}
/**************************************************************************
DOES:    Call-back function for request sdo upload
**************************************************************************/
BYTE SDO_RequestUpload (BYTE Node_ID, WORD Index, WORD SubIndex, BYTE* LpData)//¶ÁÈ¡
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	    	// is this node SDO in empty state?
	        if (gCSDOConfig[i].SDO_state == 0 && gMCOSLVConfig[i].Node_State != 4 && gMCOSLVConfig[i].Node_State != 0x0f)
	        {
	        	gCSDOConfig[i].SDO_LP = LpData;
	         	gCSDOConfig[i].TXSDO.BUF[0] = 0x40;
	         	gCSDOConfig[i].TXSDO.BUF[1] = Index & 0x00ff;
	         	gCSDOConfig[i].TXSDO.BUF[2] = (Index >> 8) & 0x00ff;
	         	gCSDOConfig[i].TXSDO.BUF[3] = SubIndex;
	         	// set this SDO into request tx state
	         	gCSDOConfig[i].SDO_state = 1;
	         	gCSDOConfig[i].SDO_timeout_times = 0;
	         	gCSDOConfig[i].SDO_abort_code = 0;
	          	// exit the loop
	          	return 1;
	        }
	    }
        i++;
    } // for all client sdo 
    return 0;
}
/**************************************************************************
DOES:    Call-back function get sdo upload response
**************************************************************************/
BYTE SDO_ConfirmUpload (BYTE Node_ID)
{
	char i;
	i = 0;
    while (i < NR_OF_SLAVES)
    {
    	if (gMCOSLVConfig[i].Node_ID == Node_ID)
    	{
	    	// is this node SDO in respons received state?
	        if (gCSDOConfig[i].SDO_state == 3)
	        {
	        	//confirm the response
	        	gCSDOConfig[i].SDO_state = 0;
	          	return 1;
	        }
	    }
        i++;
    } // for all client sdo 
    return 0;
}
/**************************************************************************
DOES:    Handle an incoimg SDO response.
RETURNS: returns 1 if SDO access success, returns 0 if SDO abort generated
**************************************************************************/
void MCO_Handle_SDO_Response(BYTE *pData,BYTE Nr_CSDO)
{
	char i,len;
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] = pData[0];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[1] = pData[1];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[2] = pData[2];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[3] = pData[3];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[4] = pData[4];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[5] = pData[5];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[6] = pData[6];
	gCSDOConfig[Nr_CSDO].RXSDO.BUF[7] = pData[7];
	// expedited sdo down load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe2) == 0x22) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe0) == 0x60))
	{
	 	gCSDOConfig[Nr_CSDO].SDO_state = 3;	
	 	return;
	}
	// expedited sdo up load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe0) == 0x40) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe2) == 0x42))
	{
	 	len = 4 - ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x0C) >> 2);
	 	for(i = 0; i < len; i++)
	 	{
	 		*(gCSDOConfig[Nr_CSDO].SDO_LP++) = gCSDOConfig[Nr_CSDO].RXSDO.BUF[4 + i];
	 	}
	 	gCSDOConfig[Nr_CSDO].SDO_state = 3;	
	 	return;
	}
	// 1st normal sdo down load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe2) == 0x20) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe0) == 0x60))
	{
	 	gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x00;
	 	for(i = 0; i < 7; i++)
	 	{
	 		gCSDOConfig[Nr_CSDO].TXSDO.BUF[1 + i] = *(gCSDOConfig[Nr_CSDO].SDO_LP++);
	 	}
	 	gCSDOConfig[Nr_CSDO].SDO_LEN -= 7;	
	 	if (!MCOHW_PushMessage(&gCSDOConfig[Nr_CSDO].TXSDO))
  		{
    		MCOUSER_FatalError(0x8801);
  		}
  		if (gCSDOConfig[Nr_CSDO].SDO_timeout != 0)
  		{
  			gCSDOConfig[Nr_CSDO].SDO_timestamp = MCOHW_GetTime() + gCSDOConfig[i].SDO_timeout;
  		}
	 	return;	 	
	}
	// n normal sdo down load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe1) == 0x00)  && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe0) == 0x20))
	{
	 	if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) == (gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x10))
	 	{
	 		if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) == 0x00)
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x10;
	 		else
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x00;
	 	}
	 	else
	 	{
	 		MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	 		gCSDOConfig[Nr_CSDO].SDO_state = 0;	
	  		return;
	  	}
	 	if (gCSDOConfig[Nr_CSDO].SDO_LEN <= 7)
	 	{
	 		for(i = 0; i < gCSDOConfig[Nr_CSDO].SDO_LEN; i++)
	 		{
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[1 + i] = *(gCSDOConfig[Nr_CSDO].SDO_LP++);
	 		}
	 		gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] | 0x01 | ((7 - gCSDOConfig[Nr_CSDO].SDO_LEN) << 1);
	 		gCSDOConfig[Nr_CSDO].SDO_LEN = 0;
	 	}
	 	else
	 	{
	 		for(i = 0; i < 7; i++)
	 		{
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[1 + i] = *(gCSDOConfig[Nr_CSDO].SDO_LP++);
	 		}
	 		gCSDOConfig[Nr_CSDO].SDO_LEN -= 7;
	 	}
	 	if (!MCOHW_PushMessage(&gCSDOConfig[Nr_CSDO].TXSDO))
  		{
    		MCOUSER_FatalError(0x8801);
  		}
  		if (gCSDOConfig[Nr_CSDO].SDO_timeout != 0)
  		{
  			gCSDOConfig[Nr_CSDO].SDO_timestamp = MCOHW_GetTime() + gCSDOConfig[i].SDO_timeout;
  		}
	 	return;		 	
	}
	// last normal sdo down load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe1) == 0x01) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe0) == 0x20))
	{
	 	if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) != (gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x10))
	 	{
	 		MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	 		gCSDOConfig[Nr_CSDO].SDO_state = 0;	
	  		return;
	  	}
	 	gCSDOConfig[Nr_CSDO].SDO_state = 3;	
	 	return;	 	
	}
	// 1st normal sdo up load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe0) == 0x40) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe2) == 0x40))
	{
	 	gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x60;
	 	for(i = 4; i > 0; i--)
	 	{
	 		gCSDOConfig[Nr_CSDO].SDO_LEN = (gCSDOConfig[Nr_CSDO].SDO_LEN << 8) + gCSDOConfig[Nr_CSDO].RXSDO.BUF[3 + i];
	 	}	
	 	if (!MCOHW_PushMessage(&gCSDOConfig[Nr_CSDO].TXSDO))
  		{
    		MCOUSER_FatalError(0x8801);
  		}
  		if (gCSDOConfig[Nr_CSDO].SDO_timeout != 0)
  		{
  			gCSDOConfig[Nr_CSDO].SDO_timestamp = MCOHW_GetTime() + gCSDOConfig[i].SDO_timeout;
  		}
	 	return;	
	}
	// n normal sdo up load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe0) == 0x60) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe1) == 0x00))
	{
	 	if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) == (gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x10))
	 	{
	 		if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) == 0x00)
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x70;
	 		else
	 			gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] = 0x60;
	 	}
	 	else
	 	{
	 		MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	 		gCSDOConfig[Nr_CSDO].SDO_state = 0;	
	  		return;
	  	}
		len = 7 - ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x0e) >> 1);
	 	for(i = 0; i < len; i++)
	 	{
	 		*(gCSDOConfig[Nr_CSDO].SDO_LP++) = gCSDOConfig[Nr_CSDO].RXSDO.BUF[1 + i];
	 	}
	 	gCSDOConfig[Nr_CSDO].SDO_LEN -= len;	
	 	if (!MCOHW_PushMessage(&gCSDOConfig[Nr_CSDO].TXSDO))
  		{
    		MCOUSER_FatalError(0x8801);
  		}
  		if (gCSDOConfig[Nr_CSDO].SDO_timeout != 0)
  		{
  			gCSDOConfig[Nr_CSDO].SDO_timestamp = MCOHW_GetTime() + gCSDOConfig[i].SDO_timeout;
  		}
	 	return;	
	}
	// last normal sdo up load
	if (((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0xe0) == 0x60) && ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0xe1) == 0x01))
	{
	 	if ((gCSDOConfig[Nr_CSDO].TXSDO.BUF[0] & 0x10) != (gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x10))
	 	{
	 		MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	 		gCSDOConfig[Nr_CSDO].SDO_state = 0;	
	  		return;
	  	}
	  	len = 7 - ((gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] & 0x0e) >> 1);
	  	if (gCSDOConfig[Nr_CSDO].SDO_LEN != len)
	  	{
	  		MCO_Send_SDO_Abort(SDO_ABORT_TYPEMISMATCH);
	 		gCSDOConfig[Nr_CSDO].SDO_state = 0;	
	  		return;
	  	}
	  	for(i = 0; i < len; i++)
	 	{
	 		*(gCSDOConfig[Nr_CSDO].SDO_LP++) = gCSDOConfig[Nr_CSDO].RXSDO.BUF[1 + i];
	 	}
	 	gCSDOConfig[Nr_CSDO].SDO_LEN = 0;
	 	gCSDOConfig[Nr_CSDO].SDO_state = 3;	
	 	return;
	}
	// sdo abort code
	if (gCSDOConfig[Nr_CSDO].RXSDO.BUF[0] == 0x80)
	{
	 	gCSDOConfig[Nr_CSDO].SDO_state = 3;	
	 	gCSDOConfig[Nr_CSDO].SDO_abort_code = gCSDOConfig[Nr_CSDO].RXSDO.BUF[7];
	 	gCSDOConfig[Nr_CSDO].SDO_abort_code = (gCSDOConfig[Nr_CSDO].SDO_abort_code << 8) + gCSDOConfig[Nr_CSDO].RXSDO.BUF[6];
	 	gCSDOConfig[Nr_CSDO].SDO_abort_code = (gCSDOConfig[Nr_CSDO].SDO_abort_code << 8) + gCSDOConfig[Nr_CSDO].RXSDO.BUF[5];
	 	gCSDOConfig[Nr_CSDO].SDO_abort_code = (gCSDOConfig[Nr_CSDO].SDO_abort_code << 8) + gCSDOConfig[Nr_CSDO].RXSDO.BUF[4];	
	 	SDO_AbortHandle(gMCOSLVConfig[Nr_CSDO].Node_ID);
	 	
	}
}
/**************************************************************************
DOES:    Call-back function for start all node
**************************************************************************/
void NMT_StartAllNode (void)
{
	gNMT.BUF[0] = 0x01;
	gNMT.BUF[1] = 0x00;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Call-back function for stop all node
**************************************************************************/
void NMT_StopAllNode (void)
{
	gNMT.BUF[0] = 0x02;
	gNMT.BUF[1] = 0x00;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Call-back function for start  node
**************************************************************************/
void NMT_StartNode (BYTE Node_ID)
{
	gNMT.BUF[0] = 0x01;
	gNMT.BUF[1] = Node_ID;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Call-back function for stop  node
**************************************************************************/
void NMT_StopNode (BYTE Node_ID)
{
	gNMT.BUF[0] = 0x02;
	gNMT.BUF[1] = Node_ID;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Call-back function for reset  node
**************************************************************************/
void NMT_ResetNode (BYTE Node_ID)
{
	gNMT.BUF[0] = 0x81;
	gNMT.BUF[1] = Node_ID;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Call-back function for reset communication node
**************************************************************************/
void NMT_ResetCommNode (BYTE Node_ID)
{
	gNMT.BUF[0] = 0x82;
	gNMT.BUF[1] = Node_ID;	
  	if (!MCOHW_PushMessage(&gNMT))
	{
		MCOUSER_FatalError(0x8801);
	}
}
/**************************************************************************
DOES:    Initializes the MicroCANopen stack 
It must be called from within MCOUSER_ResetApplication
RETURNS: nothing
**************************************************************************/
void MCO_SLV_Init 
(
BYTE NR_slave,  // the serial number of slave Node to be init
BYTE Node_ID,   // CANopen node ID (1-126)
WORD Heartbeat,  // Heartbeat time in ms (0 for none)
WORD SDO_timeout,
WORD SDO_timeout_threshold

)
{
	// Init the global variables
	gMCOSLVConfig[NR_slave].Node_ID = Node_ID;
	gMCOSLVConfig[NR_slave].error_code = 0;
	gMCOSLVConfig[NR_slave].heartbeat_time = Heartbeat;
	gMCOSLVConfig[NR_slave].Node_State = 0x0f;
	gMCOSLVConfig[NR_slave].error_register = 0;
	
	// Init SDO Response/Abort message
	gCSDOConfig[NR_slave].TXSDO.ID  = 0x600+Node_ID;
	gCSDOConfig[NR_slave].TXSDO.LEN = 8;
	gCSDOConfig[NR_slave].RXSDO.ID  = 0x580+Node_ID;
	gCSDOConfig[NR_slave].RXSDO.LEN = 8;
	gCSDOConfig[NR_slave].SDO_timeout = SDO_timeout;
	gCSDOConfig[NR_slave].SDO_timeout_threshold = SDO_timeout_threshold;
	// for SDO requests
	if (!MCOHW_SetCANFilter(0x580+Node_ID))
	{
		MCOUSER_FatalError(0x8803);
	}
	// for heartbeat message
	if (!MCOHW_SetCANFilter(0x700+Node_ID))
	{
		MCOUSER_FatalError(0x8803);
	}
}  

#endif
////////////////////////////////
BYTE MCO_Search_ODTABLE
(
WORD index,   // Index of OD entry searched
BYTE subindex // Subindex of OD entry searched 
)
{
	BYTE j = 0;
  	WORD compare;
  	// pointer to od records
  	OD_TABLE *pOD;
  	// initialize pointer
  	pOD = &(OD[0]);
 	// loop until maximum table size
  	while (j < 0xFF)
  	{
          compare = pOD->idx;
          // end of table reached? 
          if (compare == 0xFFFF)
          {
      		return 0xFF;// not found
          }
          // index found?
          if (compare == index)
          {
      		// subindex found?
      		if (pOD->subidx == subindex)
      		{
        		return j;
      		}
          }
   		// increment by SIZEOF(OD_PROCESS_DATA_ENTRY)
          pOD++;
          j++;
	}
  	// not found
  	return 0xFF;
}
/**************************************************************************
DOES:    Common exit routine for SDO_Handler. 
Send SDO response with variable length (1-4 bytes).
Assumes that gTxCAN.ID, LEN and BUF[1-3] are already set
RETURNS: 1 if response transmitted 
**************************************************************************/
BYTE MCO_ReplyWith
(
BYTE *pDat,  // pointer to sdo data
BYTE len     // number of bytes of data in SDO
)
{
	signed char k; // for loop counter
	gTxSDO.BUF[4] = 0;
	gTxSDO.BUF[5] = 0;
	gTxSDO.BUF[6] = 0;
	gTxSDO.BUF[7] = 0;
	// copy "backwards" to swap byte order to lowest first
	for (k = 0; k < len; k++)
	{
		gTxSDO.BUF[4+k] = *pDat;
		pDat++;
	}
	
	// transmit message
	if (!MCOHW_PushMessage(&gTxSDO))
	{
		// failed to transmit
		MCOUSER_FatalError(0x8801);
	}
	
	// transmitted ok
	return 1;
}
/**************************************************************************
DOES:    Common exit routine for SDO_Handler. 
Send NorSDO response with variable length (1-7 bytes).
Assumes that gTxCAN.ID, LEN are already set
RETURNS: 1 if response transmitted 
**************************************************************************/
BYTE MCO_ReplyWithNorSDO
(
BYTE *pDat,  // pointer to sdo data
BYTE len     // number of bytes of data in SDO
)
{
	signed char k; // for loop counter
	gTxSDO.BUF[1] = 0;
	gTxSDO.BUF[2] = 0;
	gTxSDO.BUF[3] = 0;
	gTxSDO.BUF[4] = 0;
	gTxSDO.BUF[5] = 0;
	gTxSDO.BUF[6] = 0;
	gTxSDO.BUF[7] = 0;
	// copy "backwards" to swap byte order to lowest first
	for (k = 0; k < len; k++)
	{
		gTxSDO.BUF[1+k] = *pDat;
		pDat++;
	}
	
	// transmit message
	if (!MCOHW_PushMessage(&gTxSDO))
	{
		// failed to transmit
		MCOUSER_FatalError(0x8801);
	}
	
	// transmitted ok
	return 1;
}
/**************************************************************************
DOES:    COPY  DATAS TO RPDO
RETURNS: nothing
**************************************************************************/
void COPY_DATA_RPDO(BYTE PDONum, BYTE* lpDATA)
{
	BYTE count;
	for (count = 0; count < gRPDOConfig[PDONum].len; count++)
	{
		*gRPDOConfig[PDONum].MapAdd[count] = *lpDATA;
		lpDATA++;
	}
}
/**************************************************************************
DOES:    COPY  DATAS TO TPDO
RETURNS: nothing
**************************************************************************/
void COPY_DATA_TPDO(BYTE PDONum)
{
	BYTE count;
	count = 0;
	for (count = 0; count < gTPDOConfig[PDONum].CANmsg.LEN; count++)
	{
		gTPDOConfig[PDONum].CANmsg.BUF[count] = *gTPDOConfig[PDONum].MapAdd[count];
	}
}
/**************************************************************************
DOES:    COMPARE  DATAS WITH TPDO
RETURNS: 1 EQUAL 0 NOT EQUAL
**************************************************************************/
BYTE CMP_DATA_TPDO(BYTE PDONum)
{
	BYTE count;
	count = 0;
	for (count = 0; count < gTPDOConfig[PDONum].CANmsg.LEN; count++)
	{
		if (gTPDOConfig[PDONum].CANmsg.BUF[count] != *gTPDOConfig[PDONum].MapAdd[count])
			return 1;
	}
	return 0;
}
/**************************************************************************
DOES:    SET THE EMCY REG
**************************************************************************/
void SET_EMCY_REG(BYTE EMCY_REG)
{
	gMCOConfig.error_register = gMCOConfig.error_register | EMCY_REG;	
	gMCOConfig.error_register = gMCOConfig.error_register | 0x01;
}
/**************************************************************************
DOES:    SET THE EMCY REG
**************************************************************************/
void CLR_EMCY_REG(BYTE EMCY_REG)
{
	gMCOConfig.error_register = gMCOConfig.error_register & (~EMCY_REG);
	if ((gMCOConfig.error_register & 0xfe) == 0)
		gMCOConfig.error_register = 0;	
}
/**************************************************************************
DOES:    GET RPDO DATAS MAPPING ADD
RETURNS: nothing
**************************************************************************/

void Get_RPDO_MapAdd(BYTE PDONum)
{
	BYTE count, i, j, SubIdx, ODNum, Len, EnyNum;
	WORD Idx;
	for (i = 0; i<=7; i++)
	{
		gRPDOConfig[PDONum].MapAdd[i] = (BYTE*)0;
	}
	count = 0;
	gRPDOConfig[PDONum].CANID = OD[MCO_Search_ODTABLE((0x1400 + PDONum),0x01)].Val;
	ODNum = MCO_Search_ODTABLE((0x1600 + PDONum),0x00);
	EnyNum = OD[ODNum].Val;
	for (i = 1; i <= EnyNum; i++)
	{  
		ODNum = MCO_Search_ODTABLE((0x1600 + PDONum),i);
		if (ODNum == 0xff)
			MCOUSER_FatalError (0x0010);
		else
		{
			Idx = OD[ODNum].Val >> 16;
			SubIdx = (OD[ODNum].Val >> 8) & 0xff;
			Len = (OD[ODNum].Val >> 3) & 0x0f;
			ODNum = MCO_Search_ODTABLE(Idx,SubIdx);
			if (ODNum == 0xff)
				MCOUSER_FatalError (0x0010);
			else
			{
				gRPDOConfig[PDONum].MapAdd[count] = (BYTE*)(&(OD[ODNum].Val));
				count++;
				for (j = 1; j< Len; j++)
				{
					gRPDOConfig[PDONum].MapAdd[count] = gRPDOConfig[PDONum].MapAdd[count - 1] + 1;
					count++;
				}
			}
		}
	}
	gRPDOConfig[PDONum].len = count;
}
/**************************************************************************
DOES:    GET RPDO DATAS MAPPING ADD
RETURNS: nothing
**************************************************************************/

void Get_TPDO_MapAdd(BYTE PDONum)
{
	BYTE count, i, j, SubIdx, ODNum, Len, EnyNum;
	WORD Idx;
	for (i = 0; i<=7; i++)
	{
		gTPDOConfig[PDONum].MapAdd[i] = (BYTE*)0;
	}
	count = 0;
	gTPDOConfig[PDONum].CANmsg.ID = OD[MCO_Search_ODTABLE((0x1800 + PDONum),0x01)].Val;
	ODNum = MCO_Search_ODTABLE((0x1a00 + PDONum),0x00);
	EnyNum = OD[ODNum].Val;
	for (i = 1; i <= EnyNum; i++)
	{  
		ODNum = MCO_Search_ODTABLE((0x1a00 + PDONum),i);
		if (ODNum == 0xff)
			MCOUSER_FatalError (0x0010);
		else
		{
			Idx = OD[ODNum].Val >> 16;
			SubIdx = (OD[ODNum].Val >> 8) & 0xff;
			Len = (OD[ODNum].Val >> 3) & 0x0f;
			ODNum = MCO_Search_ODTABLE(Idx,SubIdx);
			if (ODNum == 0xff)
				MCOUSER_FatalError (0x0010);
			else
			{
				gTPDOConfig[PDONum].MapAdd[count] = (BYTE*)(&(OD[ODNum].Val));
				count++;
				for (j = 1; j< Len; j++)
				{
					gTPDOConfig[PDONum].MapAdd[count] = gTPDOConfig[PDONum].MapAdd[count - 1] + 1;
					count++;
				}
			}
		}
	}
	gTPDOConfig[PDONum].CANmsg.LEN = count;
}
/**************************************************************************
DOES:    Handle an incoimg  SDO request.
RETURNS: returns 1 if SDO access success, returns 0 if SDO abort generated

*pData: pointer to 8 data bytes with SDO data
**************************************************************************/
BYTE MCO_Handle_SDO_Request (BYTE *pData )
{
	// command byte of SDO request
  	BYTE cmd;
  	// index of SDO request
  	WORD index;
  	// subindex of SDO request
  	BYTE subindex;
  	// search result of Search_OD
  	BYTE found;
  	BYTE len,k;
  	BYTE * pAdd;
	// pointer to an entry in gODProcTable
  	OD_TABLE *pOD; 
  	// init variables
 	// upper 3 bits are the command
  	cmd = *pData & 0xE0;
    //Upload SDO Segment Protocol
    if (((cmd == 0x60 && (gNorSDO.dir & 0x01))  || (cmd == 0x00 && (gNorSDO.dir & 0x02))))
    {
    	found = MCO_Search_ODTABLE(gNorSDO.index,gNorSDO.subindex);
  		if (found != 0xFF)
    	{
    		pOD = &(OD[found]);
    	}
    	else
  		{
  			MCO_Send_SDO_Abort(SDO_ABORT_UNSUPPORTED);
  			return 0; 
        }
	  	if (cmd == 0x60)
	  	{  	
	  		if ((gNorSDO.dir & 0x10) ^ (*pData & 0x10))
	  		{
				if (gNorSDO.point > 7)
				{
					len = 7;
					gTxSDO.BUF[0] = 0x00 | ((7-len) << 1) | (*pData & 0x10);
					MCO_ReplyWithNorSDO(((BYTE*)(pOD->Val) + gNorSDO.size - gNorSDO.point),(7));
					gNorSDO.point = gNorSDO.point - 7;
				}
				else
				{
					len = gNorSDO.point;	
					gTxSDO.BUF[0] = 0x01 | ((7-len) << 1) | (*pData & 0x10);
					MCO_ReplyWithNorSDO(((BYTE*)(pOD->Val) + gNorSDO.size - gNorSDO.point),(len));
					gNorSDO.size = 0;
					gNorSDO.point = 0;
					gNorSDO.dir = 0x00;
				}
				gNorSDO.dir = gNorSDO.dir & (0xef);
				gNorSDO.dir = gNorSDO.dir | (*pData & 0x10);
				return 1; 
				
	  		}
	  		else
	  		{
	  			MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	  			return 0;
	  		}
	  	}
	  	//Download SDO Segment Protocol
	  	if (cmd == 0x00)
	  	{  	
	  		if ((gNorSDO.dir & 0x10) ^ (*pData & 0x10))
	  		{
	  			pAdd = (BYTE*)(pOD->Val) + gNorSDO.size - gNorSDO.point;
	  			len = 7 - ((*pData & 0x0e) >> 1);
				if (gNorSDO.point >= len)
				{
					gNorSDO.point = gNorSDO.point - len;
					gTxSDO.BUF[0] = 0x20 | (*pData & 0x10);
					for (k = 0; k < len; k++)
	      			{
	        			*pAdd = gRxCAN.BUF[1 + k]; 
	        			pAdd++;         					
	      			}
	      			if ( (*pData & 0x01) || (gNorSDO.point == 0))
	      			{
	      				gNorSDO.dir = 0x00;
	      			}	
	      			gNorSDO.dir = gNorSDO.dir & (0xef);
					gNorSDO.dir = gNorSDO.dir | (*pData & 0x10);	
				}
				else
				{
        			MCO_Send_SDO_Abort(SDO_ABORT_TYPEMISMATCH);
        			return 0;
      			}
				if (!MCOHW_PushMessage(&gTxSDO))
	      		{
	        		MCOUSER_FatalError(0x8808);
	      		}
				return 1; 
				
	  		}
	  		else
	  		{
	  			MCO_Send_SDO_Abort(SDO_ABORT_NO_TOGGLE);
	  			return 0;
	  		}
	  	}
	}
  	// is it a read or write command?
  	if ((cmd == 0x40) || (cmd == 0x20)) 
  	{
  		// get high byte of index
  		index = pData[2];
  		// add low byte of index
  		index = (index << 8) + pData[1];
  		// subindex
  		subindex = pData[3];
  		found = MCO_Search_ODTABLE(index,subindex);
  		if (found != 0xFF)
    	{
    		pOD = &(OD[found]);
    	}
    	else
  		{
  			MCO_Send_SDO_Abort(SDO_ABORT_UNSUPPORTED);
        	return 0;
    	}
  		// Copy Multiplexor into response
  		// index low
  		gTxSDO.BUF[1] = pData[1];
  		// index high
  		gTxSDO.BUF[2] = pData[2];
  		// subindex
  		gTxSDO.BUF[3] = pData[3];
  		// read command?
  		if (cmd == 0x40)
  		{
    		// read allowed?
    		if ((pOD->AccTyp & ODRD) != 0) // Check if RD bit is set
    		{
    			len = SizeOfOD(pOD->DataTyp);
    			if (len < 5)
    			{
    				// expedited, len of data
					gTxSDO.BUF[0] = 0x43 | ((4-len) << 2);
      				return MCO_ReplyWith(((BYTE*)&(pOD->Val)),(SizeOfOD(pOD->DataTyp)));
    			}
    			else
    			{
    				gNorSDO.dir = 0x11;
    				gNorSDO.size = SizeOfOD(pOD->DataTyp);
    				gNorSDO.point = gNorSDO.size;
    				gNorSDO.index = index;
    				gNorSDO.subindex = subindex;
    				gTxSDO.BUF[0] = 0x41;
      				return MCO_ReplyWith(((BYTE*)&(gNorSDO.size)),4);
    			}
    		}
  		}
  		// write command?
  		else if (cmd == 0x20)
  		{
    		// is WR bit set? - then write allowed
    		if ((pOD->AccTyp & ODWR) != 0)
    		{
    			if (*pData & 0x02)
    			{        			
      				// for writes: Bits 2 and 3 of *pData are number of bytes without data
      				len = 4 - ((*pData & 0x0C) >> 2); 
      				// is length ok?
      				if (len != SizeOfOD(pOD->DataTyp))
      				{
        				MCO_Send_SDO_Abort(SDO_ABORT_TYPEMISMATCH);
        				return 0;
      				}
      				// retrieve data from SDO write request and copy into process image
      				pOD->Val = 0;
      				while (len > 0)
      				{
        				len--;
        				pOD->Val = pOD->Val << 8;
        				pOD->Val = pOD->Val + gRxCAN.BUF[4+len];          					
      				}
      			}
      			else
      			{
      				gNorSDO.size = 0;
      				len = 4;
      				while (len > 0)
      				{
        				len--;
        				gNorSDO.size = gNorSDO.size << 8;
        				gNorSDO.size = gNorSDO.size + gRxCAN.BUF[4+len];          					
      				}
      				if (gNorSDO.size != SizeOfOD(pOD->DataTyp))
      				{
        				MCO_Send_SDO_Abort(SDO_ABORT_TYPEMISMATCH);
        				return 0;
      				}
      				else
      				{
      					gNorSDO.point = gNorSDO.size;
      					gNorSDO.dir = 0x12;
    					gNorSDO.index = index;
    					gNorSDO.subindex = subindex;
    				}
    			}
      			// write response
      			gTxSDO.BUF[0] = 0x60;	
      			if (!MCOHW_PushMessage(&gTxSDO))
      			{
        			MCOUSER_FatalError(0x8808);
      			}
      			return 1;
    		}
    		// write not allowed
    	}
    	else
    	{
      		MCO_Send_SDO_Abort(SDO_ABORT_UNSUPPORTED);
      		return 0;
    	}
        // Requested OD entry not found
      	if (subindex == 0)
      	{
      		MCO_Send_SDO_Abort(SDO_ABORT_NOT_EXISTS);
    	}
    	else
    	{
      		MCO_Send_SDO_Abort(SDO_ABORT_UNKNOWNSUB);
    	}
		return 0;
    }
  	// ignore abort received - all other produce an error
  	if (cmd != 0x80)
  	{
   		MCO_Send_SDO_Abort(SDO_ABORT_UNKNOWN_COMMAND);
    	return 0;
  	}
  	return 1;
}

#if NR_OF_TPDOS > 0
/**************************************************************************
DOES:    Called when going into the operational mode.
Prepares all TPDOs for operational.
RETURNS: nothing
**************************************************************************/
void MCO_Prepare_TPDOs (void)
{
	BYTE i;
	
	i = 0;
	// prepare all TPDOs for transmission
	while (i < NR_OF_TPDOS)
	{
		// this TPDO is used
		if (gTPDOConfig[i].CANmsg.ID != 0)
		{
			// Copy current process data
			COPY_DATA_TPDO(i);
#ifdef USE_EVENT_TIME
			// Reset event timer for immediate transmission
			gTPDOConfig[i].event_timestamp = MCOHW_GetTime() - 2;
#endif
#ifdef USE_INHIBIT_TIME
			gTPDOConfig[i].inhibit_status = 2; // Mark as ready for transmission
			// Reset inhibit timer for immediate transmission
			gTPDOConfig[i].inhibit_timestamp = MCOHW_GetTime() - 2;
#endif
		}
		i++;
	}
	// ensure that MCO_ProcessStack starts with TPDO1
	gTPDONr = NR_OF_TPDOS;
}

/**************************************************************************
DOES:    Called when a TPDO needs to be transmitted
RETURNS: nothing

PDONr:TPDO number to transmit
**************************************************************************/
void MCO_TransmitPDO (BYTE PDONr)  
{
#ifdef USE_INHIBIT_TIME
	// new inhibit timer started
	gTPDOConfig[PDONr].inhibit_status = 1;
	gTPDOConfig[PDONr].inhibit_timestamp = MCOHW_GetTime() + *gTPDOConfig[PDONr].inhibit_time;
#endif
#ifdef USE_EVENT_TIME
	gTPDOConfig[gTPDONr].event_timestamp = MCOHW_GetTime() + *gTPDOConfig[gTPDONr].event_time; 
#endif
	if (!MCOHW_PushMessage(&gTPDOConfig[PDONr].CANmsg))
	{
		MCOUSER_FatalError(0x8801);
	}
}
#endif // NR_OF_TPDOS > 0

/**************************************************************************
PUBLIC FUNCTIONS
***************************************************************************/ 

/**************************************************************************
DOES:    Initializes the MicroCANopen stack
It must be called from within MCOUSER_ResetApplication
RETURNS: nothing
**************************************************************************/
void MCO_Init 
(
WORD Baudrate,  // CAN baudrate in kbit (1000,800,500,250,125,50,25 or 10)
BYTE Node_ID,   // CANopen node ID (1-126)
WORD Heartbeat  // Heartbeat time in ms (0 for none)
)
{
	BYTE i;
	
	// Init the global variables
	gMCOConfig.Node_ID = Node_ID;
	gMCOConfig.error_code = 0;
	gMCOConfig.emergency_state = 0;  
	gMCOConfig.Baudrate = Baudrate;
	gMCOConfig.heartbeat_time = Heartbeat;
	gMCOConfig.heartbeat_msg.ID = 0x700+Node_ID;
	gMCOConfig.heartbeat_msg.LEN = 1;
	// current NMT state of this node = bootup
	gMCOConfig.heartbeat_msg.BUF[0] = 0;
	gMCOConfig.error_register = 0;
	
	// Init SDO Response/Abort message
	gTxSDO.ID = 0x580+gMCOConfig.Node_ID;
	gTxSDO.LEN = 8;
	// Init EMERGENCY message
	gMCOConfig.emergency_msg.ID = 0x80+Node_ID;
	gMCOConfig.emergency_msg.LEN = 8;
	gMCOConfig.emergency_msg.BUF[0] = 0x00;
	gMCOConfig.emergency_msg.BUF[1] = 0x00;
	gMCOConfig.emergency_msg.BUF[2] = 0x00;
	gMCOConfig.emergency_msg.BUF[3] = 0x00;
	gMCOConfig.emergency_msg.BUF[4] = 0x00;
	gMCOConfig.emergency_msg.BUF[5] = 0x00;
	gMCOConfig.emergency_msg.BUF[6] = 0x00;
	gMCOConfig.emergency_msg.BUF[7] = 0x00;
#if NR_OF_SLAVES > 0 
	// Init NMT master message
	gNMT.ID = 0x00;
	gNMT.LEN = 2;
#endif  
#if NR_OF_TPDOS > 0
	i = 0;
	// init TPDOs
	while (i < NR_OF_TPDOS)
	{
		gTPDOConfig[i].CANmsg.ID = 0;
		i++;
	}
#endif
#if NR_OF_RPDOS > 0
	i = 0;
	// init RPDOs
	while (i < NR_OF_RPDOS)
	{
		gRPDOConfig[i].CANID = 0;
		i++;
	}
#endif
	
	// init the CAN interface
	if (!MCOHW_Init())
	{
		MCOUSER_FatalError(0x8802);
	}
	// for nmt master message
	if (!MCOHW_SetCANFilter(0))
	{
		MCOUSER_FatalError(0x8803);
	}
	// for SDO requests
	if (!MCOHW_SetCANFilter(0x600+Node_ID))
	{
		MCOUSER_FatalError(0x8803);
	}
	
	// signal to MCO_ProcessStack: we just initialized
	gTPDONr = 0xFF;
}  
#if NR_OF_RPDOS > 0
/**************************************************************************
DOES:    This function initializes a receive PDO. Once initialized, the 
MicroCANopen stack automatically updates the data at offset.
NOTE:    For data consistency, the application should not read the data
while function MCO_ProcessStack executes.
RETURNS: nothing   
PDO_NR: RPDO number (1-4)
**************************************************************************/
void MCO_InitRPDO(BYTE PDO_NR )
{
	
#ifdef CHECK_PARAMETERS
	// check PDO range and check node id range 1 - 127
	if (((PDO_NR < 1) || (PDO_NR > NR_OF_RPDOS)))
	{
		MCOUSER_FatalError(0x8804);
	}
#endif
	PDO_NR--;
	Get_RPDO_MapAdd(PDO_NR);
	if (!MCOHW_SetCANFilter(gRPDOConfig[PDO_NR].CANID))
	{
		MCOUSER_FatalError(0x8805);
	}
}
#endif // NR_OF_RPDOS > 0


#if NR_OF_TPDOS > 0
/**************************************************************************
DOES:    This function initializes a transmit PDO. Once initialized, the 
MicroCANopen stack automatically handles transmitting the PDO.
The application can directly change the data at any time.
NOTE:    For data consistency, the application should not write to the data
while function MCO_ProcessStack executes.
RETURNS: nothing  
PDO_NR:  TPDO number (1-4)
**************************************************************************/
void MCO_InitTPDO(BYTE PDO_NR )
{
	
#ifdef CHECK_PARAMETERS
	// check PDO range, node id, len range 1 - 8 and event time or inhibit time set
	if (((PDO_NR < 1)|| (PDO_NR > NR_OF_TPDOS)))
	{
		MCOUSER_FatalError(0x8806);
	}
#endif
	PDO_NR--;
#ifdef USE_EVENT_TIME
	gTPDOConfig[PDO_NR].event_time = (WORD*)(&OD[MCO_Search_ODTABLE((0x1800 + PDO_NR),0x05)].Val);
#endif
#ifdef USE_INHIBIT_TIME
	gTPDOConfig[PDO_NR].inhibit_time = (WORD*)(&OD[MCO_Search_ODTABLE((0x1800 + PDO_NR),0x03)].Val);
#endif
	Get_TPDO_MapAdd(PDO_NR);
}
#endif // NR_OF_TPDOS > 0


/**************************************************************************
DOES:    This function implements the main MicroCANopen protocol stack. 
It must be called frequently to ensure proper operation of the
communication stack. 
Typically it is called from the while(1) loop in main.
RETURNS: 0 if nothing was done, 1 if a CAN message was sent or received
**************************************************************************/
BYTE MCO_ProcessStack(void)
{
	BYTE i;
    BYTE ret_val = 0;
    // check if this is right after boot-up
    // was set by MCO_Init
    if (gTPDONr == 0xFF)
    {
		// init heartbeat time
		gMCOConfig.heartbeat_timestamp = MCOHW_GetTime() + gMCOConfig.heartbeat_time;
		// send boot-up message  
		if (!MCOHW_PushMessage(&gMCOConfig.heartbeat_msg))
		{
			MCOUSER_FatalError(0x8801);
		}
#ifdef AUTOSTART
		// going into operational state
		gMCOConfig.heartbeat_msg.BUF[0] = 0x05;
#if NR_OF_TPDOS > 0
        MCO_Prepare_TPDOs();
#endif
#else
		// going into pre-operational state
		gMCOConfig.heartbeat_msg.BUF[0] = 0x7F;
#endif
		// return value to default
		gTPDONr = NR_OF_TPDOS;
		return 1;
    }
  	// Emergency manager
    if (gMCOConfig.emergency_msg.BUF[2] != gMCOConfig.error_register)
    {
        if (gMCOConfig.emergency_msg.BUF[2] < gMCOConfig.error_register)
        {
            gMCOConfig.emergency_msg.BUF[1] = 0x10;
            gMCOConfig.emergency_msg.BUF[2] = gMCOConfig.error_register;
		}
		else
		{
			gMCOConfig.emergency_msg.BUF[1] = 0x00;
            gMCOConfig.emergency_msg.BUF[2] = gMCOConfig.error_register;
        }
		if (!MCOHW_PushMessage(&gMCOConfig.emergency_msg))
		{
    	    MCOUSER_FatalError(0x8801);
		}
    }        	  
  	// Clinet sdo and heart beat manager
#if NR_OF_SLAVES > 0 
	
	i = 0;
	while (i < NR_OF_SLAVES)
	{
		if ((gMCOSLVConfig[i].heartbeat_time != 0) && MCOHW_IsTimeExpired(gMCOSLVConfig[i].heartbeat_timestamp))
	    {
	        gMCOSLVConfig[i].Node_State = 0x0f;
	        Node_OffLineHandle(gMCOSLVConfig[i].Node_ID);
	    }
		i++;
	} // for all slaves heatbeat 
	i = 0;
	while (i < NR_OF_SLAVES)
	{
		// is this cline sdo request transmit?
		if (gCSDOConfig[i].SDO_state == 1)
		{
          	if (!MCOHW_PushMessage(&gCSDOConfig[i].TXSDO))
			{
    		    MCOUSER_FatalError(0x8801);
			}
			else
			{
				gCSDOConfig[i].SDO_state = 2;
			}
			if (gCSDOConfig[i].SDO_timeout != 0)
			{
				gCSDOConfig[i].SDO_timestamp = MCOHW_GetTime() + gCSDOConfig[i].SDO_timeout;
			}
          	// exit the loop
          	i = NR_OF_SLAVES;
		}
		if (gCSDOConfig[i].SDO_state == 2)
		{
			if (gCSDOConfig[i].SDO_timeout != 0)
			{
	            if (MCOHW_IsTimeExpired(gCSDOConfig[i].SDO_timestamp))
	    	    {
	    	        gCSDOConfig[i].SDO_state = 0x1;
	    	        if (gCSDOConfig[i].SDO_timeout_times >= gCSDOConfig[i].SDO_timeout_threshold)
	    	        {
	    	            gCSDOConfig[i].SDO_state = 0x03;
						gCSDOConfig[i].SDO_timeout_times = 0;
						gMCOSLVConfig[i].Node_State = 0x0f;
						Node_OffLineHandle(gMCOSLVConfig[i].Node_ID);
	    	        }
	    	        else
	    	        {
	    	            gCSDOConfig[i].SDO_timeout_times++;
	    	        }
	    	    }
			}
			// exit the loop
			i = NR_OF_SLAVES;
		}
		i++;
	} // for all slaves heatbeat 
#endif
  	// work on next incoming messages
	
	
#if NR_OF_TPDOS > 0
  	// is the node operational?
  	if (gMCOConfig.heartbeat_msg.BUF[0] == 5)
  	{
    	// check next TPDO for transmission
    	gTPDONr++;
    	if (gTPDONr >= NR_OF_TPDOS)
    	{
      		gTPDONr = 0;
    	}
    	// is the TPDO 'gTPDONr' in use?
    	if (gTPDOConfig[gTPDONr].CANmsg.ID != 0)
    	{
#ifdef USE_DYNAMIC_PDO
      	    Get_TPDO_MapAdd(gTPDONr);
#endif		
      	    if (*gTPDOConfig[gTPDONr].inhibit_time == 0)
      	    {
                // has application data changed?
                if (CMP_DATA_TPDO(gTPDONr))
				{
					// get data from process image and transmit
					COPY_DATA_TPDO(gTPDONr);
					MCO_TransmitPDO(gTPDONr);
					return 1;
				} 
      	    }     
#ifdef USE_EVENT_TIME
      	    // does TPDO use event timer and event timer is expired? if so we need to transmit now
      	    if ((*gTPDOConfig[gTPDONr].event_time != 0) && 
				(MCOHW_IsTimeExpired(gTPDOConfig[gTPDONr].event_timestamp)) && (*gTPDOConfig[gTPDONr].event_time != 0xffff))
      	    {
                COPY_DATA_TPDO(gTPDONr);
				MCO_TransmitPDO(gTPDONr);
				return 1;
      	    }
#endif // USE_EVENT_TIME
#ifdef USE_INHIBIT_TIME
      	    // does the TPDO use an inhibit time? - COS transmission
      	    if (*gTPDOConfig[gTPDONr].inhibit_time != 0)
      	    {
                // is the inihibit timer currently running?
				if (gTPDOConfig[gTPDONr].inhibit_status > 0)
				{
					// has the inhibit time expired?
					if (MCOHW_IsTimeExpired(gTPDOConfig[gTPDONr].inhibit_timestamp))
					{
            	        // is there a new transmit message already waiting?
                        if (gTPDOConfig[gTPDONr].inhibit_status == 2)
						{ 
							// transmit now
							MCO_TransmitPDO(gTPDONr);
							return 1;
						}
						// no new message waiting, but timer expired
						else 
						{
              	            gTPDOConfig[gTPDONr].inhibit_status = 0;
						}
					}
				}
				// is inhibit status 0 or 1?
				if (gTPDOConfig[gTPDONr].inhibit_status < 2)
				{
					// has application data changed?
					if (CMP_DATA_TPDO(gTPDONr))
					{
            	        // Copy application data
						COPY_DATA_TPDO(gTPDONr);
						// has inhibit time expired?
						if (gTPDOConfig[gTPDONr].inhibit_status == 0)
						{
							// transmit now
							MCO_TransmitPDO(gTPDONr);
							return 1;
            	        }
						// inhibit status is 1
						else
						{
							// wait for inhibit time to expire 
							gTPDOConfig[gTPDONr].inhibit_status = 2;
						}
					}
				}
      	    } // Inhibit Time != 0
#endif // USE_INHIBIT_TIME
    	} // PDO active (CAN_ID != 0)  
    } // if node is operational
#endif // NR_OF_TPDOS > 0  
    // do we produce a heartbeat?
    if (gMCOConfig.heartbeat_time != 0)
    {
        // has heartbeat time passed?
    	if (MCOHW_IsTimeExpired(gMCOConfig.heartbeat_timestamp))
    	{
      	    // transmit heartbeat message
      	    if (!MCOHW_PushMessage(&gMCOConfig.heartbeat_msg))
      	    {
				MCOUSER_FatalError(0x8801);
      	    }
      	    // get new heartbeat time for next transmission
      	    gMCOConfig.heartbeat_timestamp = MCOHW_GetTime() + gMCOConfig.heartbeat_time;
      	    ret_val = 1;
    	}
    }
    return ret_val;
}

int CanOpenRxPres(void)
{
	int i;
	// if message received
	if (1)
	{
		// is it an NMT master message?
		if (gRxCAN.ID == 0)
		{
	  		// nmt message is for      this node        or       all nodes
	  		if ((gRxCAN.BUF[1] == gMCOConfig.Node_ID) || (gRxCAN.BUF[1] == 0))
	  		{
				switch (gRxCAN.BUF[0])
				{
	      			// start node
				case 1:
	        		gMCOConfig.heartbeat_msg.BUF[0] = 5;
#if NR_OF_TPDOS > 0          
	        		MCO_Prepare_TPDOs();			//Prepare Transmit PDOs
#endif
	        		break;
					
	      			// stop node
				case 2:
	        		gMCOConfig.heartbeat_msg.BUF[0] = 4;
	        		break;
					
	      			// enter pre-operational
				case 128:
	        		gMCOConfig.heartbeat_msg.BUF[0] = 127;		//7F	
	        		break;
					
	      			// node reset
				case 129:
	        		MCOUSER_ResetApplication();
	        		break;
					
	      			// node reset communication
				case 130:
	        		MCOUSER_ResetCommunication();
	        		break;
					
	      			// unknown command
				default:
	        		break;
				}
				return 1;
	  		} // NMT message addressed to this node
		} // NMT master message received
#if NR_OF_SLAVES > 0 									
		i = 0;
		while (i < NR_OF_SLAVES)
		{
			// is this heartbeat from slave?	
			if (gRxCAN.ID == 0x700 + gMCOSLVConfig[i].Node_ID)
			{ 
	      		// get the state of slave
	      		gMCOSLVConfig[i].Node_State = (gRxCAN.BUF[0] & 0x7f);
	      		if (gMCOSLVConfig[i].Node_State == 0x00)			//Initialization
	      			gMCOSLVConfig[i].Node_State = 0x7f;				//Enter pre-operational
	      		if (gMCOSLVConfig[i].heartbeat_time != 0)
	      		{
	      			gMCOSLVConfig[i].heartbeat_timestamp = MCOHW_GetTime() + gMCOSLVConfig[i].heartbeat_time;
	      		}
	      		// exit the loop
	      		i = NR_OF_SLAVES;
			}
			i++;
		} // for all slaves heatbeat 
#endif
		// if node is not stopped...
		if (gMCOConfig.heartbeat_msg.BUF[0] != 4)
		{
	  		// is the message an SDO request message for us?
	  		if (gRxCAN.ID == gMCOConfig.Node_ID+0x600)
	  		{
	    		// handle SDO request - return value not used in this version
	    		i = MCO_Handle_SDO_Request(&gRxCAN.BUF[0]);
	    		return 1;
	  		}
#if NR_OF_SLAVES > 0 
	  		i = 0;
	  		while (i < NR_OF_SLAVES)
	  		{
				// is this heartbeat from slave?
				if (gRxCAN.ID == gCSDOConfig[i].RXSDO.ID)
				{
	      	  		// get the state of slave
	      	  		MCO_Handle_SDO_Response(&gRxCAN.BUF[0],i);
	      	  		// exit the loop
	      	  		i = NR_OF_SLAVES;
				}
				// is this emergency from slave?
				if (gRxCAN.ID == (0x80 + gMCOSLVConfig[i].Node_ID))
				{
			        gMCOSLVConfig[i].error_register = gRxCAN.BUF[2];
			        gMCOSLVConfig[i].error_code = gRxCAN.BUF[1];
			        gMCOSLVConfig[i].error_code = (gMCOSLVConfig[i].error_code << 8) + gRxCAN.BUF[0];
			        MCO_Handle_Emergency(gMCOSLVConfig[i].Node_ID);
					// exit the loop
					i = NR_OF_SLAVES;
				}
				i++;
	  		} // for all client sdo 
#endif
		}
		
#if NR_OF_RPDOS > 0
		// is the node operational?
		if (gMCOConfig.heartbeat_msg.BUF[0] == 5)
		{
	  		i = 0;
	  		// loop through RPDOs
	  		while (i < NR_OF_RPDOS)
	  		{
				// is this one of our RPDOs?
				if (gRxCAN.ID == gRPDOConfig[i].CANID)
				{
	      			// copy data from RPDO to OD
#ifdef USE_DYNAMIC_PDO
	      			Get_RPDO_MapAdd(i);
#endif 
	      			COPY_DATA_RPDO(i,&gRxCAN.BUF[0]);
	      			// exit the loop
	      			i = NR_OF_RPDOS;
	      			//ret_val = 1;
				}
				i++;
	  		} // for all RPDOs
		} // node is operational
#endif // NR_OF_RPDOS > 0
		return 1;
	} // Message received
	//return 0;
}		

