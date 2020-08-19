/*--------------------------------------------------------------------------*/
/**@file   key.h
   @brief  按键消息扫描头文件
   @details
   @author
   @date   2010-12-13
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef	_MSG_H_
#define _MSG_H_

///msg define
enum
{
    MSG_0 = 0,                          /*0x00*/
    MSG_1,
    MSG_2,
    MSG_3,
    MSG_4,
    MSG_5,
    MSG_6,
    MSG_7,
    MSG_8,
    MSG_9,
    MSG_DP,
    //------------------
	MSG_ESC_KEY,
    MSG_UP_KEY,
    MSG_DOWN_KEY,
    MSG_OK_KEY,
    //-------------------
    MSG_ESC_OK_KEY,
    MSG_ESC_UP_KEY,
    MSG_ESC_DOWN_KEY,
    NO_MSG = 0xFF,
};

#endif




