//------------------------------------------------
#ifndef __CRC16_H__
	#define __CRC16_H__ 
	
	//#include "config.h"

	typedef unsigned short USHORT;
	typedef unsigned char UCHAR;
	
	#define _CONST_ const
		
	//CRC16Ð§Ñé³ÌÐò
	extern USHORT CRC16( UCHAR * pucFrame, USHORT usLen );
	extern UCHAR _CONST_ aucCRCHi[];
	extern UCHAR _CONST_ aucCRCLo[];
#endif

