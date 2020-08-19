/**************************************************************************
MODULE:    USER
CONTAINS:  MicroCANopen Object Dictionary and Process Image implementation
COPYRIGHT: Embedded Systems Academy, Inc. 2002-2004.
All rights reserved. www.microcanopen.com
This software was written in accordance to the guidelines at
www.esacademy.com/software/softwarestyleguide.pdf
DISCLAIM:  Read and understand our disclaimer免责声明before using this code!
www.esacademy.com/disclaim.htm
LICENSE:   Users that have purchased a license for CANopen Magic
(www.CANopenMagic.com)
may use this code in commercial projects.
Otherwise only educational use is acceptable.
VERSION:   2.01, Pf 13-MAY-04
$LastChangedDate: 2004-03-15 23:05:11 -0800 (Mon, 15 Mar 2004) $
$LastChangedRevision: 2 $
---------------------------------------------------------------------------
HISTORY:   2.01, Pf 13-MAY-04, Release for Philips LPC2129
1.20, Pf 19-AUG-03, First Published Version
Functionality in this module was located in module MCO in
previous releases.
***************************************************************************/

#include <string.h>
#include "stm32f2xx.h"
#include "mco.h"
#include "mcohw.h"
//#include "stm32f0xx_crc.h"
//#include "i2c_24c16.h"

// Global timer/conter variable, incremented every millisecond
extern WORD volatile gTimCnt;
extern WORD volatile gCANFilter;


/**************************************************************************
GLOBAL VARIABLES
***************************************************************************/
//BYTE ODNodeID = 0x06;
BYTE ODNodeID = 0x01;
OD_TABLE OD[MAX_OD_SIZE];

/*

void MCO_Init(void)
{
	initTimer();
	CAN_Configration(Baud_Data);
	MCOUSER_ResetCommunication();
	
	
	pTPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x2000,0x00)].Val;
	pTPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x2000,0x01)].Val;
	
	pRPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x6000,0x00)].Val;
	pRPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x6000,0x01)].Val;
}
*/



BYTE SizeOfOD(BYTE DataTyp)
{
	switch (DataTyp)
	{
	case 0x0005:
		return 1;
	case 0x0006:
		return 2;
	case 0x0007:
		return 4;
	case 0x0040:
		return 30;
	}	
	return 0;
}	
// ////////////////////////////////
// void CpODVal2Ary(DWORD Ary[], BYTE ODNumB, BYTE ODNumE)
// {
//     BYTE i,Len;
//     DWORD j;
//     Len = ODNumE - ODNumB + 1;
//     for(i = 0; i < Len; i++)
//     {
//         Ary[i] = OD[ODNumB + i].Val;
//     }
//     CRC_ResetDR();
//     Ary[i] = CRC_CalcBlockCRC((u32*)Ary, Len);
//     i2c_24c_init(I2C2);
//     i2c_24c_buffer_write((u8*)Ary, 0x00, (Len + 1) * 2, EE24C_Block0_ADDR, I2C2);
//     for(j = 0; j < 600000; j++)
//     {}
//     i2c_24c_init(I2C2);
//     i2c_24c_buffer_write(((u8*)Ary + (Len + 1) * 2), 0x00, (Len + 1) * 2, EE24C_Block1_ADDR, I2C2);
// }
// void CpAry2ODVal(DWORD Ary[], BYTE ODNumB, BYTE ODNumE)
// {
//     BYTE i,Len;
//     DWORD j;
//     Len = ODNumE - ODNumB + 1;
//     i2c_24c_init(I2C2);
//     i2c_24c_buffer_read((u8*)Ary, 0x00, (Len + 1) * 2, EE24C_Block0_ADDR, I2C2);
//     for(j = 0; j < 600000; j++)
//     {}
//     i2c_24c_init(I2C2);
//     i2c_24c_buffer_read(((u8*)Ary + (Len + 1) * 2), 0x00, (Len + 1) * 2, EE24C_Block1_ADDR, I2C2);
//     CRC_ResetDR();
//     if(Ary[Len] == CRC_CalcBlockCRC((u32*)Ary, Len))
//     {
//         for(i = 0; i < Len; i++)
//         {
//             OD[ODNumB + i].Val = Ary[i];
//         }
//     }
// }
// ////////////////////////////////////
void OD_init(BYTE Num,WORD Idx,BYTE SubIdx,
             BYTE ObjTyp,WORD DataTyp,BYTE AccTyp,BYTE PdoMap,
             DWORD DefVal,DWORD Val)
{
	OD[Num].idx = Idx;
	OD[Num].subidx = SubIdx;
	OD[Num].ObjTyp = ObjTyp;
	OD[Num].DataTyp = DataTyp;
	OD[Num].AccTyp = AccTyp;
	OD[Num].PdoMap = PdoMap;
	//OD[Num].DefVal = DefVal;
	OD[Num].Val = Val;
}


#if 0
////////////////////////////////////////////////////////////////
void ODTALE_init(void)
{
	#if 1
	int i;
	i=0;
	OD_init(0 + i,0x1000,0x00,ODVAR,ODUS32,ODRD,ODNMP,OD_DEVICE_TYPE,OD_DEVICE_TYPE);
	OD_init(1 + i,0x1018,0x00,ODVAR,ODUS16,ODRD,ODNMP,4,4);
	OD_init(2 + i,0x1018,0x01,ODVAR,ODUS32,ODRD,ODNMP,OD_VENDOR_ID,OD_VENDOR_ID);
	OD_init(3 + i,0x1018,0x02,ODVAR,ODUS32,ODRD,ODNMP,OD_PRODUCT_CODE,OD_PRODUCT_CODE);
	OD_init(4 + i,0x1018,0x03,ODVAR,ODUS32,ODRD,ODNMP,OD_REVISION,OD_REVISION);
	OD_init(5 + i,0x1018,0x04,ODVAR,ODUS32,ODRD,ODNMP,OD_SERIAL,OD_SERIAL);
	/*RPDO1 Communication Parameter*/
	OD_init(6 + i,0x1400,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);                               //number of entries
	OD_init(7 + i,0x1400,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x200 + ODNodeID,0x200 + ODNodeID);//COB-ID used by PDO = 0x200+ODNodeID
 	OD_init(8 + i,0x1400,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type = asynchronous
	/*RPDO1 Mapping Parameter*/
	OD_init(9 + i,0x1600,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                    //number of mapped objects
	OD_init(10 + i,0x1600,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60000010,0x60000010);//first mapped object     
	OD_init(11 + i,0x1600,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60000110,0x60000110);//second mapped object    
	OD_init(12 + i,0x1600,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60000210,0x60000210);//third mapped object     
	OD_init(13 + i,0x1600,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60000310,0x60000310);//fourth mapped object    
	
	#if 0
	/*RPDO2 Communication Parameter*/
	OD_init(14 + i,0x1401,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(15 + i,0x1401,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x300 + ODNodeID,0x300 +ODNodeID );//COB-ID used by PDO
	OD_init(16 + i,0x1401,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO2 Mapping Parameter*/
	OD_init(17 + i,0x1601,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(18 + i,0x1601,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60010010,0x60010010);//first mapped object     
	OD_init(19 + i,0x1601,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60010110,0x60010110);//second mapped object    
	OD_init(20 + i,0x1601,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60010210,0x60010210);//third mapped object     
	OD_init(21 + i,0x1601,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60010310,0x60010310);//fourth mapped object  
	/*RPDO3 Communication Parameter*/
	OD_init(22 + i,0x1402,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(23 + i,0x1402,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x400 + ODNodeID,0x400 +ODNodeID );//COB-ID used by PDO
	OD_init(24 + i,0x1402,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO3 Mapping Parameter*/
	OD_init(25 + i,0x1602,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(26 + i,0x1602,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60020010,0x60020010);//first mapped object     
	OD_init(27 + i,0x1602,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60020110,0x60020110);//second mapped object    
	OD_init(28 + i,0x1602,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60020210,0x60020210);//third mapped object     
	OD_init(29 + i,0x1602,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60020310,0x60020310);//fourth mapped object  
	/*RPDO4 Communication Parameter*/
	OD_init(30 + i,0x1403,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(31 + i,0x1403,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x500 + ODNodeID,0x500 +ODNodeID );//COB-ID used by PDO
	OD_init(32 + i,0x1403,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO4 Mapping Parameter*/
	OD_init(33 + i,0x1603,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(34 + i,0x1603,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60030010,0x60030010);//first mapped object     
	OD_init(35 + i,0x1603,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60030110,0x60030110);//second mapped object    
	OD_init(36 + i,0x1603,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60030210,0x60030210);//third mapped object     
	OD_init(37 + i,0x1603,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60030310,0x60030310);//fourth mapped object  
	#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/*TPDO1 Communication Parameter*/
	OD_init(38 + i,0x1800,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(39 + i,0x1800,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x180 + ODNodeID,0x180 + ODNodeID);//COB-ID used by PDO
	OD_init(40 + i,0x1800,0x02,ODVAR,ODUS8,ODRW,ODNMP,0xff,0xff);	                      //transmission type  传输类型
	OD_init(41 + i,0x1800,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);	                      //inhibit time       禁止时间
	OD_init(42 + i,0x1800,0x05,ODVAR,ODUS16,ODRW,ODNMP,100,100);	                      //event timer        事件定时周期
	/*TPDO1 Mapping Parameter*/
	OD_init(43 + i,0x1A00,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(44 + i,0x1A00,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20000110,0x20000110);//first mapped object       第一个映射对象
	OD_init(45 + i,0x1A00,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20000210,0x20000210);//second mapped object      第二个映射对象
	OD_init(46 + i,0x1A00,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20000310,0x20000310);//third mapped object       第三个映射对象
	OD_init(47 + i,0x1A00,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20000410,0x20000410);//fourth mapped object      第四个映射对象
	#if 0
	/*TPDO2 Communication Parameter*/
	OD_init(48 + i,0x1801,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(49 + i,0x1801,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x280 + ODNodeID,0x280 + ODNodeID);//COB-ID used by PDO
	OD_init(50 + i,0x1801,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(51 + i,0x1801,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(52 + i,0x1801,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO2 Mapping Parameter*/
	OD_init(53 + i,0x1A01,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(54 + i,0x1A01,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20010110,0x20010110);//first mapped object       第一个映射对象
	OD_init(55 + i,0x1A01,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20010210,0x20010210);//second mapped object      第二个映射对象
	OD_init(56 + i,0x1A01,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20010310,0x20010310);//third mapped object       第三个映射对象
	OD_init(57 + i,0x1A01,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20010410,0x20010410);//fourth mapped object      第四个映射对象
	/*TPDO3 Communication Parameter*/
	OD_init(58 + i,0x1802,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(59 + i,0x1802,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x380 + ODNodeID,0x380 + ODNodeID);//COB-ID used by PDO
	OD_init(60 + i,0x1802,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(61 + i,0x1802,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(62 + i,0x1802,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO3 Mapping Parameter*/
	OD_init(63 + i,0x1A02,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(64 + i,0x1A02,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20020110,0x20020110);//first mapped object       第一个映射对象
	OD_init(65 + i,0x1A02,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20020210,0x20020210);//second mapped object      第二个映射对象
	OD_init(66 + i,0x1A02,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20020310,0x20020310);//third mapped object       第三个映射对象
	OD_init(67 + i,0x1A02,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20020410,0x20020410);//fourth mapped object      第四个映射对象
	/*TPDO4 Communication Parameter*/
	OD_init(68 + i,0x1803,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(69 + i,0x1803,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x480 + ODNodeID,0x480 + ODNodeID);//COB-ID used by PDO
	OD_init(70 + i,0x1803,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(71 + i,0x1802,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(72 + i,0x1803,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO4 Mapping Parameter*/
	OD_init(73 + i,0x1A03,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(74 + i,0x1A03,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20030110,0x20030110);//first mapped object       第一个映射对象
	OD_init(75 + i,0x1A03,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20030210,0x20030210);//second mapped object      第二个映射对象
	OD_init(76 + i,0x1A03,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20030310,0x20030310);//third mapped object       第三个映射对象
	OD_init(77 + i,0x1A03,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20030410,0x20030410);//fourth mapped object      第四个映射对象
	#endif
	
	//SDO
	OD_init(78 + i,0x1280,0x00,ODVAR,ODUS8,ODRW,ODNMP,3,3);	                              //number of entries 入口数量
	OD_init(79 + i,0x1280,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x600 + ODNodeID,0x600 + ODNodeID);//COB-ID Client->Server (tx)  接受SDO 
	OD_init(80 + i,0x1280,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x580 + ODNodeID,0x580 + ODNodeID);//COB-ID Server->Client (rx)  发送SDO 
	OD_init(81 + i,0x1280,0x03,ODVAR,ODUS8,ODRW,ODNMP,0x05,0x05);                         //Node-ID of the SDO server   节点号
	#endif
	
	
	#if 1
	//TPDO参数
	//TPDO1
	OD_init(82 + i,0x2000,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(83 + i,0x2000,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(84 + i,0x2000,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(85 + i,0x2000,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	/*
	//TPDO2
	OD_init(86 + i,0x2001,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(87 + i,0x2001,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(88 + i,0x2001,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(89 + i,0x2001,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//TPDO3
	OD_init(90 + i,0x2002,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(91 + i,0x2002,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(92 + i,0x2002,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(93 + i,0x2002,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//TPDO4
	OD_init(94 + i,0x2003,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(95 + i,0x2003,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(96 + i,0x2003,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(97 + i,0x2003,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	*/
	//RPDO参数
	//RPDO1
	OD_init(98 + i,0x6000,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(99 + i,0x6000,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(100 + i,0x6000,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(101 + i,0x6000,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	/*
	//RPDO2
	OD_init(102 + i,0x6001,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(103 + i,0x6001,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(104 + i,0x6001,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(105 + i,0x6001,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//RPDO3
	OD_init(106 + i,0x6002,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(107 + i,0x6002,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(108 + i,0x6002,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(109 + i,0x6002,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//RPDO4
	OD_init(110 + i,0x6003,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(112 + i,0x6003,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(113 + i,0x6003,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(114 + i,0x6003,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	*/
	//自定义参数
	OD_init(115 + i,0x6004,0x00,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//DeviceAddress
	
	OD_init(116 + i,0x6005,0x00,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//LightsMode
	
	OD_init(117 + i,0x6006,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x0000,0x0000);//LedNumber
	OD_init(118 + i,0x6006,0x01,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//ColorOrder
	OD_init(119 + i,0x6006,0x02,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//ChipType
	OD_init(120 + i,0x6006,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x00,0x00);//PlayFps+StopFps+CycleFps
	OD_init(121 + i,0x6006,0x04,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//PatternNumber
	OD_init(122 + i,0x6006,0x05,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//LedColorCycleMode
	
	
	//结束标志
	OD_init(123 + i,0xffff,0xff,0xff,0xffff,0xff,0xff,0xffffffff,0xffffffff);
	
	#endif
}
#endif


#if 1
////////////////////////////////////////////////////////////////
void ODTALE_init(void)
{
	#if 1
	int i;
	i=0;
	OD_init(0 + i,0x1000,0x00,ODVAR,ODUS32,ODRD,ODNMP,OD_DEVICE_TYPE,OD_DEVICE_TYPE);
	OD_init(1 + i,0x1018,0x00,ODVAR,ODUS16,ODRD,ODNMP,4,4);
	OD_init(2 + i,0x1018,0x01,ODVAR,ODUS32,ODRD,ODNMP,OD_VENDOR_ID,OD_VENDOR_ID);
	OD_init(3 + i,0x1018,0x02,ODVAR,ODUS32,ODRD,ODNMP,OD_PRODUCT_CODE,OD_PRODUCT_CODE);
	OD_init(4 + i,0x1018,0x03,ODVAR,ODUS32,ODRD,ODNMP,OD_REVISION,OD_REVISION);
	OD_init(5 + i,0x1018,0x04,ODVAR,ODUS32,ODRD,ODNMP,OD_SERIAL,OD_SERIAL);
	/*RPDO1 Communication Parameter*/
	OD_init(6 + i,0x1400,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);                               //number of entries
	OD_init(7 + i,0x1400,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x200 + ODNodeID,0x200 + ODNodeID);//COB-ID used by PDO = 0x200+ODNodeID
 	OD_init(8 + i,0x1400,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type = asynchronous
	/*RPDO1 Mapping Parameter*/
	OD_init(9 + i,0x1600,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);                    //number of mapped objects
	OD_init(10 + i,0x1600,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60000020,0x60000020);//first mapped object     
	OD_init(11 + i,0x1600,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60000120,0x60000120);//second mapped object    
	//OD_init(12 + i,0x1600,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60000210,0x60000210);//third mapped object     
	//OD_init(13 + i,0x1600,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60000310,0x60000310);//fourth mapped object    
	
	#if 0
	/*RPDO2 Communication Parameter*/
	OD_init(14 + i,0x1401,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(15 + i,0x1401,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x300 + ODNodeID,0x300 +ODNodeID );//COB-ID used by PDO
	OD_init(16 + i,0x1401,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO2 Mapping Parameter*/
	OD_init(17 + i,0x1601,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(18 + i,0x1601,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60010010,0x60010010);//first mapped object     
	OD_init(19 + i,0x1601,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60010110,0x60010110);//second mapped object    
	OD_init(20 + i,0x1601,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60010210,0x60010210);//third mapped object     
	OD_init(21 + i,0x1601,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60010310,0x60010310);//fourth mapped object  
	/*RPDO3 Communication Parameter*/
	OD_init(22 + i,0x1402,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(23 + i,0x1402,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x400 + ODNodeID,0x400 +ODNodeID );//COB-ID used by PDO
	OD_init(24 + i,0x1402,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO3 Mapping Parameter*/
	OD_init(25 + i,0x1602,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(26 + i,0x1602,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60020010,0x60020010);//first mapped object     
	OD_init(27 + i,0x1602,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60020110,0x60020110);//second mapped object    
	OD_init(28 + i,0x1602,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60020210,0x60020210);//third mapped object     
	OD_init(29 + i,0x1602,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60020310,0x60020310);//fourth mapped object  
	/*RPDO4 Communication Parameter*/
	OD_init(30 + i,0x1403,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);			              //number of entries
	OD_init(31 + i,0x1403,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x500 + ODNodeID,0x500 +ODNodeID );//COB-ID used by PDO
	OD_init(32 + i,0x1403,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);			      //transmission type 
	/*RPDO4 Mapping Parameter*/
	OD_init(33 + i,0x1603,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);
	OD_init(34 + i,0x1603,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x60030010,0x60030010);//first mapped object     
	OD_init(35 + i,0x1603,0x02,ODVAR,ODUS32,ODRD,ODNMP,0x60030110,0x60030110);//second mapped object    
	OD_init(36 + i,0x1603,0x03,ODVAR,ODUS32,ODRD,ODNMP,0x60030210,0x60030210);//third mapped object     
	OD_init(37 + i,0x1603,0x04,ODVAR,ODUS32,ODRD,ODNMP,0x60030310,0x60030310);//fourth mapped object  
	#endif
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/*TPDO1 Communication Parameter*/
	OD_init(12 + i,0x1800,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(13 + i,0x1800,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x180 + ODNodeID,0x180 + ODNodeID);//COB-ID used by PDO
	OD_init(14 + i,0x1800,0x02,ODVAR,ODUS8,ODRW,ODNMP,0xff,0xff);	                      //transmission type  传输类型
	OD_init(15 + i,0x1800,0x03,ODVAR,ODUS16,ODRD,ODNMP,0,0);	                      //inhibit time       禁止时间
	OD_init(16 + i,0x1800,0x05,ODVAR,ODUS16,ODRW,ODNMP,0,0);	                      //event timer        事件定时周期
	/*TPDO1 Mapping Parameter*/
	OD_init(17 + i,0x1A00,0x00,ODVAR,ODUS8,ODRD,ODNMP,2,2);                   //number of mapped objects  映射对象的数量
	OD_init(18 + i,0x1A00,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20000120,0x20000020);//first mapped object       第一个映射对象
	OD_init(19 + i,0x1A00,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20000220,0x20000120);//second mapped object      第二个映射对象
	//OD_init(46 + i,0x1A00,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20000310,0x20000310);//third mapped object       第三个映射对象
	//OD_init(47 + i,0x1A00,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20000410,0x20000410);//fourth mapped object      第四个映射对象
	#if 0
	/*TPDO2 Communication Parameter*/
	OD_init(48 + i,0x1801,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(49 + i,0x1801,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x280 + ODNodeID,0x280 + ODNodeID);//COB-ID used by PDO
	OD_init(50 + i,0x1801,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(51 + i,0x1801,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(52 + i,0x1801,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO2 Mapping Parameter*/
	OD_init(53 + i,0x1A01,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(54 + i,0x1A01,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20010110,0x20010110);//first mapped object       第一个映射对象
	OD_init(55 + i,0x1A01,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20010210,0x20010210);//second mapped object      第二个映射对象
	OD_init(56 + i,0x1A01,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20010310,0x20010310);//third mapped object       第三个映射对象
	OD_init(57 + i,0x1A01,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20010410,0x20010410);//fourth mapped object      第四个映射对象
	/*TPDO3 Communication Parameter*/
	OD_init(58 + i,0x1802,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(59 + i,0x1802,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x380 + ODNodeID,0x380 + ODNodeID);//COB-ID used by PDO
	OD_init(60 + i,0x1802,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(61 + i,0x1802,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(62 + i,0x1802,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO3 Mapping Parameter*/
	OD_init(63 + i,0x1A02,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(64 + i,0x1A02,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20020110,0x20020110);//first mapped object       第一个映射对象
	OD_init(65 + i,0x1A02,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20020210,0x20020210);//second mapped object      第二个映射对象
	OD_init(66 + i,0x1A02,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20020310,0x20020310);//third mapped object       第三个映射对象
	OD_init(67 + i,0x1A02,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20020410,0x20020410);//fourth mapped object      第四个映射对象
	/*TPDO4 Communication Parameter*/
	OD_init(68 + i,0x1803,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                               //number of entries 条目的数量
	OD_init(69 + i,0x1803,0x01,ODVAR,ODUS32,ODRD,ODNMP,0x480 + ODNodeID,0x480 + ODNodeID);//COB-ID used by PDO
	OD_init(70 + i,0x1803,0x02,ODVAR,ODUS8,ODRD,ODNMP,0xff,0xff);                         //transmission type  传输类型
	OD_init(71 + i,0x1802,0x03,ODVAR,ODUS16,ODRD,ODNMP,1,1);                              //inhibit time       禁止时间
	OD_init(72 + i,0x1803,0x05,ODVAR,ODUS16,ODRD,ODNMP,100,100);                          //event timer        事件定时周期
	/*TPDO4 Mapping Parameter*/
	OD_init(73 + i,0x1A03,0x00,ODVAR,ODUS8,ODRD,ODNMP,4,4);                   //number of mapped objects  映射对象的数量
	OD_init(74 + i,0x1A03,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x20030110,0x20030110);//first mapped object       第一个映射对象
	OD_init(75 + i,0x1A03,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x20030210,0x20030210);//second mapped object      第二个映射对象
	OD_init(76 + i,0x1A03,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x20030310,0x20030310);//third mapped object       第三个映射对象
	OD_init(77 + i,0x1A03,0x04,ODVAR,ODUS32,ODRW,ODNMP,0x20030410,0x20030410);//fourth mapped object      第四个映射对象
	#endif
	
	//SDO
	OD_init(20 + i,0x1280,0x00,ODVAR,ODUS8,ODRW,ODNMP,3,3);	                              //number of entries 入口数量
	OD_init(21 + i,0x1280,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x600 + ODNodeID,0x600 + ODNodeID);//COB-ID Client->Server (tx)  接受SDO 
	OD_init(22 + i,0x1280,0x02,ODVAR,ODUS32,ODRW,ODNMP,0x580 + ODNodeID,0x580 + ODNodeID);//COB-ID Server->Client (rx)  发送SDO 
	OD_init(23 + i,0x1280,0x03,ODVAR,ODUS8,ODRW,ODNMP,0x05,0x05);                         //Node-ID of the SDO server   节点号
	#endif
	
	
	#if 1
	//TPDO参数
	//TPDO1
	OD_init(24 + i,0x2000,0x00,ODVAR,ODUS32,ODRW,ODNMP,0x00000000,0x00000000);//position demand value
	OD_init(25 + i,0x2000,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x00000000,0x00000000);//position demand value
	//OD_init(84 + i,0x2000,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	//OD_init(85 + i,0x2000,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	/*
	//TPDO2
	OD_init(86 + i,0x2001,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(87 + i,0x2001,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(88 + i,0x2001,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(89 + i,0x2001,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//TPDO3
	OD_init(90 + i,0x2002,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(91 + i,0x2002,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(92 + i,0x2002,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(93 + i,0x2002,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//TPDO4
	OD_init(94 + i,0x2003,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(95 + i,0x2003,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(96 + i,0x2003,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(97 + i,0x2003,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	*/
	//RPDO参数
	//RPDO1
	OD_init(26 + i,0x6000,0x00,ODVAR,ODUS32,ODRW,ODNMP,0x00000000,0x00000000);//position demand value
	OD_init(27 + i,0x6000,0x01,ODVAR,ODUS32,ODRW,ODNMP,0x00000000,0x00000000);//position demand value
	//OD_init(100 + i,0x6000,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	//OD_init(101 + i,0x6000,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	/*
	//RPDO2
	OD_init(102 + i,0x6001,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(103 + i,0x6001,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(104 + i,0x6001,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(105 + i,0x6001,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//RPDO3
	OD_init(106 + i,0x6002,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(107 + i,0x6002,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(108 + i,0x6002,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(109 + i,0x6002,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	
	//RPDO4
	OD_init(110 + i,0x6003,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(112 + i,0x6003,0x01,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(113 + i,0x6003,0x02,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	OD_init(114 + i,0x6003,0x03,ODVAR,ODUS16,ODRW,ODNMP,0x00,0x00);//position demand value
	*/
	//自定义参数
	OD_init(28 + i,0x6004,0x00,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//DeviceAddress
	OD_init(29 + i,0x6005,0x00,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//LightsMode
	OD_init(30 + i,0x6006,0x00,ODVAR,ODUS16,ODRW,ODNMP,0x0000,0x0000);//LedNumber
	OD_init(31 + i,0x6006,0x01,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//ColorOrder
	OD_init(32 + i,0x6006,0x02,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//ChipType
	OD_init(33 + i,0x6006,0x03,ODVAR,ODUS32,ODRW,ODNMP,0x00,0x00);//PlayFps+StopFps+CycleFps
	OD_init(34 + i,0x6006,0x04,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//PatternNumber
	OD_init(35 + i,0x6006,0x05,ODVAR,ODUS8,ODRW,ODNMP,0x00,0x00);//LedColorCycleMode
	
	//结束标志
	OD_init(36 + i,0xffff,0xff,0xff,0xffff,0xff,0xff,0xffffffff,0xffffffff);
	#endif
}
#endif


/**************************************************************************
GLOBAL FUNCTIONS
***************************************************************************/

/**************************************************************************
DOES:    Call-back function for get data from ProcImg.
**************************************************************************/

/**************************************************************************
DOES:    Call-back function for occurance of node off line event.
**************************************************************************/
void Node_OffLineHandle(BYTE Node_ID)
{
}
/**************************************************************************
DOES:    Call-back function for occurance of SDO abort.
**************************************************************************/
void SDO_AbortHandle(BYTE Node_ID)
{
}
/**************************************************************************
DOES:    Call-back function for occurance of a fatal error.
Stops operation and displays blnking error pattern on LED
**************************************************************************/
void MCOUSER_FatalError (WORD ErrCode)
{
	// To Do: DEBUG, report Error Code
}
/**************************************************************************
DOES:    Call-back function for occurance of a emergency.
Do anything you need when slave node emergency msg received
**************************************************************************/

void MCO_Handle_Emergency(BYTE Node_ID)
{
}
/**************************************************************************
DOES:    Call-back function for reset application.
Starts the watchdog and waits until watchdog causes a reset.
**************************************************************************/
void MCOUSER_ResetApplication (void)
{
	while (1) // No wait until Watchdog hits
	{
	}
}


/**************************************************************************
DOES:    Call-back function for reset communication.
Re-initializes the process image and the entire MicroCANopen
communication.
**************************************************************************/
void MCOUSER_ResetCommunication (void)
{
	WORD delay;
	ODTALE_init();
	// 125kbit, Node 6, 2s heartbeat
  	MCO_Init(125,ODNodeID,0);
#if NR_OF_SLAVES > 0
	//Number 0 slave, node_id 0x06,0ms heartbeat time,10ms sdo timeout, 3times to retry
	MCO_SLV_Init(0,ODNodeID,0,0,0);
#endif	
  	// DEBUG: Delay 10ms for all external HW to catch up
  	delay = gTimCnt + 10;
  	while (!MCOHW_IsTimeExpired(delay))
  	{
  	}
	// RPDO1, default ID(0x200+nodeID), 1 bytes
	MCO_InitRPDO(1);
  	// RPDO2, default ID(0x300+nodeID), 2 bytes
	//MCO_InitRPDO(2);
	// RPDO3, TPDO1 OF NODE 0X07, 2 bytes
  	//MCO_InitRPDO(3);
	MCO_InitTPDO(1);
  	// TPDO1, default ID (0x180+nodeID), 0ms event, 0ms inhibit, 2 bytes
}

/**************************************************************************
END-OF-FILE
***************************************************************************/
