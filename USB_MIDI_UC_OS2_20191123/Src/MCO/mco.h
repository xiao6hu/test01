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
VERSION:   2.01, Pf/Aa/Ck 13-MAY-04
           $LastChangedDate: 2004-05-14 06:19:32 -0700 (Fri, 14 May 2004) $
           $LastChangedRevision: 7 $
---------------------------------------------------------------------------
HISTORY:   2.01, Pf 13-MAY-04, updated for 32-bit usage
           2.00, Pf 21-OCT-03, Minor changes to pass conformance test
           1.20, Pf 19-AUG-03, Code changed to use process image
                 Bug fix for incorrect inhibit time handling
           1.10, Pf 27-MAY-03, Bug fixes in OD (hi byte was corrupted)
                 OD changed to indefinite length
                 Support of define controled MEMORY types
           1.01, Pf 17-DEC-02, Made Object Dictionary more readable
           1.00, Pf 07-OCT-02, First Published Version
***************************************************************************/

#ifndef _MCO_H
#define _MCO_H

/**************************************************************************
DEFINES: MEMORY TYPES USED
**************************************************************************/

#ifdef __C51__

// CONST Object Dictionary Data
#define MEM_CONST const code

// Process data and frequently used variables
#define MEM_NEAR data

// Seldomly used variables
#define MEM_FAR xdata

#else // __C51__

// CONST Object Dictionary Data
#define MEM_CONST const

// Process data and frequently used variables
#define MEM_NEAR

// Seldomly used variables
#define MEM_FAR

#endif // __C51__

/**************************************************************************
DEFINES: CONST ENTRIES IN OBJECT DICTIONARY
Modify these for your application
**************************************************************************/

#define OD_DEVICE_TYPE   0x0004019eL
#define OD_VENDOR_ID     0x00000000L
#define OD_PRODUCT_CODE  0x00010000L
#define OD_REVISION      0x00010000L

// The following are optional and can also be left "undefined"
#define OD_SERIAL        0xFFFFFFFFL

/**************************************************************************
DEFINES: Definition of the process image
Modify these for your application
**************************************************************************/

// Define the size of the process image
#define PROCIMG_SIZE 32
#define MAX_OD_SIZE  50
// Define process variables: offsets into the process image
// Digital Input 1
// Digital Input 2




// CAN COMMAND FROM COMPUTER
#define CAN_CMD       0x00
// MOTO PRESET SPEED
#define MSPD05        0X01
// MOTO STATE
#define MST05         0X03
// MOTO COMMAND
#define MCD05         0x05
// MOTO HEATBEAT TIME
#define HBT05         0x07

/**************************************************************************
DEFINES: ENABLING/DISABLING CODE FUNCTIONALITY
**************************************************************************/
// Maximum number of slave nodes
#define NR_OF_SLAVES 127
// Maximum number of transmit PDOs (0 to 4)
#define NR_OF_TPDOS 1

// Maximum number of receive PDOs (0 to 4)
#define NR_OF_RPDOS 1

// If defined, 1 or more TPDOs use the event timer
#define USE_EVENT_TIME

// If defined, 1 or more TPDOs are change-of-state and use the inhibit timer
#define USE_INHIBIT_TIME

// If defined, the PDO mapping parameters are remaped befor process
#define USE_DYNAMIC_PDO

// If defined, the PDO parameters are added to the Object Dictionary
// Entries must be added to the SDOResponseTable in user_xxxx.c
#define PDO_IN_OD

// If defined, the Process Data is accesible via SDO requests
// Entries must be added to the ODProcTable in user_xxxx.c
#define PROCIMG_IN_OD

// If defined, OD entry [1017,00] is supported with SDO read/write accesses
// This is also an example on how to implement dynamic/variable OD entries
#define DYNAMIC_HEARTBEAT

// If defined, node starts up automatically (does not wait for NMT master)
#define AUTOSTART

// If defined, all parameters passed to functions are checked for consistency.
// On failures, the user function MCOUSER_FatalError is called.
#define CHECK_PARAMETERS

/**************************************************************************
END OF CUSTOMIZATION AREA - DON'T CHANGE ANYTHING BELOW
**************************************************************************/

#ifdef PROCIMG_IN_OD
// Defines for access type in OD entries
#define ODRD 0x10
#define ODWR 0x20
#define ODRW 0x30
#define ODVAR 0x07
#define ODUS32 0x0007
#define ODUS16 0x0006
#define ODSTR30 0x0040
#define ODUS8 0x0005
#define ODNMP 0x00
#define ODMP 0x01
#endif // PROCIMG_IN_OD
#define CURRENT 0x02
#define VOLTAGE 0x04
#define TEMPERATURE 0x08
#define COMMUNICATION 0x10
#define PROFILE 0x20
#define MANUFACTURERE 0x80
#ifndef USE_EVENT_TIME
  #ifndef USE_INHIBIT_TIME
#error At least one, USE_EVENT_TIME or USE_INHIBIT_TIME must be defined!
  #endif
#endif

#if (NR_OF_RPDOS == 0)
  #if (NR_OF_TPDOS == 0)
#error At least one PDO must be defined!
  #endif
#endif

#if ((NR_OF_TPDOS > 4) || (NR_OF_RPDOS > 4))
#error This configuration was never tested by ESAcademy!
#endif

/**************************************************************************
GLOBAL TYPE DEFINITIONS
**************************************************************************/
/*
#define BYTE  unsigned char
#define WORD  unsigned short int
#define DWORD unsigned int
#define BOOLEAN unsigned char
*/
#include "integer.h"

//#define FALSE 0
//#define TRUE 1

//#define u32 unsigned long

#define MCO_SET_8BIT_VAL(VAL,VAL1,NUM) 			(((u32)VAL &=~(0x000000FF << (NUM*8))); (u32)VAL |=((u32)VAL1 << (NUM*8)))
#define MCO_SET_16BIT_VAL(VAL,VAL1,NUM) 		(((u32)VAL &=~(0x00FFFF << (NUM*16))); (u32)VAL |=(((u32)VAL1 << (NUM*16))))
#define MCO_SET_32BIT_VAL(VAL1,VAL2,VAL3,VAL4) 	(((u32)VAL1) | ((u32)VAL2 <<8) | ((u32)VAL3 <<16) | ((u32)VAL4 <<24))

#define MCO_GET_8BIT_VAL(VAL,NUM) 				(u8)(((u32)VAL>>(NUM*8))&0xFF)
#define MCO_GET_16BIT_VAL(VAL1,VAL2) 			(u16)(((u32)VAL>>(NUM*16))&0xFFFF)
#define MCO_GET_32BIT_VAL(VAL) 					((u32)VAL1)

// Data structure for a single CAN message
typedef struct
{
  WORD ID;                  // Message Identifier
  BYTE LEN;                 // Data length (0-8)
  BYTE dummy32bit;          // Fill it to 32-bit width
  BYTE BUF[8];              // Data buffer
} CAN_MSG;
//normal sdo recode
typedef struct
{
  WORD index;               // Index
  BYTE subindex;            // Subindex
  BYTE dir;                 // 1:upload 2:download 0:no use
  DWORD size;               // SDO Data size
  DWORD point;              // Current SDO Data point
} NorSDO;
// This structure holds all master node specific configuration
typedef struct
{
  CAN_MSG heartbeat_msg;    // Heartbeat message contents
  CAN_MSG emergency_msg;    // Heartbeat message contents
  WORD Baudrate;            // Current Baud rate in kbit
  WORD heartbeat_time;      // Heartbeat time in ms
  WORD heartbeat_timestamp; // Timestamp of last heartbeat
  WORD error_code;          // Bits: 0=RxQueue 1=TxQueue 3=CAN
  BYTE Node_ID;             // Current Node ID (1-126)
  BYTE error_register;      // Error regiter for OD entry [1001,00]
  BYTE emergency_state;
  //BYTE dummy32bit[3];          // Fill it to 32-bit width
} MCO_CONFIG;

// This structure holds all slave node specific configuration
typedef struct
{
  WORD heartbeat_time;      // Heartbeat time in ms
  WORD heartbeat_timestamp; // Timestamp of last heartbeat
  BYTE Node_ID;             // Current Node ID (1-126)
  BYTE Node_State;          // Current Node state
  WORD error_code;          // Bits: 0=RxQueue 1=TxQueue 3=CAN
  BYTE error_register;      // Error regiter for OD entry [1001,00]
} MCO_SLAVE_CONFIG;

// This structure holds all slave node specific configuration
typedef struct
{
  CAN_MSG TXSDO;            // Current/last CAN message to be transmitted
  CAN_MSG RXSDO;            // Current/last CAN message has be received
  DWORD SDO_abort_code;      // Abort_code of SDO request
  WORD SDO_timestamp;       // Timestamp of SDO request
  WORD SDO_timeout;         // Timeout valve of SDO request
  WORD SDO_timeout_threshold;  // Timeout valve of SDO request
  WORD SDO_timeout_times;   // Timeout times of SDO request
  DWORD SDO_state;           // current SDO state 0:empty 1:request tx 2:waiting for respons 3:respons received
  DWORD SDO_LEN;			//Data lenth
  BYTE* SDO_LP;				//Data point
  //BYTE dummy32bit[3];          // Fill it to 32-bit width
} CLIENT_SDO_CONFIG;
// This structure holds all the TPDO configuration data for one TPDO
typedef struct
{
  CAN_MSG CANmsg;              // Current/last CAN message to be transmitted
#ifdef USE_EVENT_TIME
  WORD* event_time;          // Event timer in ms (0 for COS only operation)
  WORD  event_timestamp;     // If event timer is used, this is the
                            // timestamp for the next transmission
#endif
#ifdef USE_INHIBIT_TIME
  WORD* inhibit_time;        // Inhibit timer in ms (0 if COS not used)
  WORD  inhibit_timestamp;   // If inhibit timer is used, this is the
                            // timestamp for the next transmission
  BYTE  inhibit_status;      // 0: Inhibit timer not started or expired
                            // 1: Inhibit timer started
                            // 2: Transmit msg waiting for expiration of inhibit
#endif
  BYTE* MapAdd[8];              // Offest to application data in process image
} TPDO_CONFIG;

// This structure holds all the RPDO configuration data for one RPDO
typedef struct
{
  WORD CANID;               // Message Identifier
  BYTE len;
  BYTE* MapAdd[8];              // Pointer to destination of data
} RPDO_CONFIG;

#ifdef PROCIMG_IN_OD
// This structure holds all data for one process data entry in the OD
typedef struct
{
  WORD idx;                 // Index of OD entry
  BYTE subidx;              // Subindex of OD entry
  BYTE len;                 // Data length in bytes (1-4), plus bits ODRD, ODWR, RMAP/WMAP
  BYTE offset;              // Offset to process data in process image
} OD_PROCESS_DATA_ENTRY;
#endif // PROCIMG_IN_OD
typedef struct
{
	WORD idx;
	BYTE subidx;
	BYTE ObjTyp;
	WORD DataTyp;
	BYTE AccTyp;
	BYTE PdoMap;
	//DWORD DefVal;
	DWORD Val;
} OD_TABLE;
/**************************************************************************
GLOBAL FUNCTIONS
**************************************************************************/
void Get_RPDO_MapAdd(BYTE PDONum);
BYTE SizeOfOD(BYTE DataTyp);
void CLR_EMCY_REG(BYTE EMCY_REG);
void SET_EMCY_REG(BYTE EMCY_REG);
BYTE MCO_Search_ODTABLE
  (
  WORD index,   // Index of OD entry searched
  BYTE subindex // Subindex of OD entry searched
  );
/**************************************************************************
DOES:    Call-back function for occurance of node off line event.
**************************************************************************/
void Node_OffLineHandle(BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for occurance of SDO abort.
**************************************************************************/
void SDO_AbortHandle(BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for occurance of a emergency.
         Do anything you need when slave node emergency msg received
**************************************************************************/
void MCO_Handle_Emergency(BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function to get node state
**************************************************************************/
BYTE MCOUSER_GetNodeState (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function to get node SDO_abort_code
**************************************************************************/
BYTE MCOUSER_GetNodeSDOAbortCode (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for request sdo download
**************************************************************************/
BYTE SDO_RequesDownload
(
	BYTE Node_ID,
	WORD Index,
	WORD SubIndex,
	DWORD Len,
	BYTE* LpData
);
/**************************************************************************
DOES:    Call-back function to confirm sdo download
**************************************************************************/
BYTE SDO_ConfirmDownload (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for request sdo upload
**************************************************************************/
BYTE SDO_RequesUpload
(
	BYTE Node_ID,
	WORD Index,
	WORD SubIndex,
	BYTE* LpData
);
/**************************************************************************
DOES:    Call-back function get sdo upload response
**************************************************************************/
BYTE SDO_ConfirmUpload (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for start all node
**************************************************************************/
void NMT_StartAllNode (void);
/**************************************************************************
DOES:    Call-back function for stop all node
**************************************************************************/
void NMT_StopAllNode (void);
/**************************************************************************
DOES:    Call-back function for start  node
**************************************************************************/
void NMT_StartNode (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for stop  node
**************************************************************************/
void NMT_StopNode (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for reset  node
**************************************************************************/
void NMT_ResetNode (BYTE Node_ID);
/**************************************************************************
DOES:    Call-back function for reset communication node
**************************************************************************/
void NMT_ResetCommNode (BYTE Node_ID);

/**************************************************************************
DOES:    Initializes the MicroCANopen stack
         It must be called from within MCOUSER_ResetApplication
RETURNS: nothing
**************************************************************************/
void MCO_Init
  (
  WORD Baudrate,  // CAN baudrate in kbit(1000,800,500,250,125,50,25 or 10)
  BYTE Node_ID,   // CANopen node ID (1-126)
  WORD Heartbeat  // Heartbeat time in ms (0 for none)
  );
/**************************************************************************
DOES:    Initializes the MicroCANopen stack
         It must be called from within MCOUSER_ResetApplication
RETURNS: nothing
**************************************************************************/
void MCO_SLV_Init
  (
  BYTE NR_slave,
  BYTE Node_ID,   // CANopen node ID (1-126)
  WORD Heartbeat,  // Heartbeat time in ms (0 for none)
  WORD SDO_timeout, //SDO TIME OUT TIME
  WORD SDO_timeout_threshold
  );
/**************************************************************************
DOES:    This function initializes a transmit PDO. Once initialized, the
         MicroCANopen stack automatically handles transmitting the PDO.
         The application can directly change the data at any time.
NOTE:    For data consistency, the application should not write to the data
         while function MCO_ProcessStack executes.
RETURNS: nothing
**************************************************************************/
void MCO_InitTPDO
  (
  BYTE PDO_NR       // TPDO number (1-4)
);

/**************************************************************************
DOES:    This function initializes a receive PDO. Once initialized, the
         MicroCANopen stack automatically updates the data at offset.
NOTE:    For data consistency, the application should not read the data
         while function MCO_ProcessStack executes.
RETURNS: nothing
**************************************************************************/
void MCO_InitRPDO
  (
  BYTE PDO_NR      // RPDO number (1-4)
);

/**************************************************************************
DOES:    This function implements the main MicroCANopen protocol stack.
         It must be called frequently to ensure proper operation of the
         communication stack.
         Typically it is called from the while(1) loop in main.
RETURNS: 0 if nothing was done, 1 if a CAN message was sent or received
**************************************************************************/
BYTE MCO_ProcessStack (void);

int CanOpenRxPres(void);
/**************************************************************************
USER CALL-BACK FUNCTIONS
These must be implemented by the application.
**************************************************************************/

/**************************************************************************
DOES:    Call-back function for reset application.
         Starts the watchdog and waits until watchdog causes a reset.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetApplication
  (
  void
  );

/**************************************************************************
DOES:    This function both resets and initializes both the CAN interface
         and the CANopen protocol stack. It is called from within the
         CANopen protocol stack, if a NMT master message was received that
         demanded "Reset Communication".
         This function should call MCO_Init and MCO_InitTPDO/MCO_InitRPDO.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetCommunication
  (
  void
  );

/**************************************************************************
DOES:    This function is called if a fatal error occurred.
         Error codes of mcohwxxx.c are in the range of 0x8000 to 0x87FF.
         Error codes of mco.c are in the range of 0x8800 to 0x8FFF.
         All other error codes may be used by the application.
RETURNS: nothing
**************************************************************************/
void MCOUSER_FatalError
  (
  WORD ErrCode // To debug, search source code for the ErrCode encountered
  );
///////////////////////////////////////////
//void CpAry2ODVal(DWORD* Ary, BYTE ODNumB, BYTE ODNumE);
///////////////////////////////////////////
//void CpODVal2Ary(DWORD* Ary, BYTE ODNumB, BYTE ODNumE);
#endif
