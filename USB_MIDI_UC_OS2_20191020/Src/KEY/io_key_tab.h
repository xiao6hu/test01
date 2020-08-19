/*--------------------------------------------------------------------------*/
/**@file   key.h
   @brief  按键消息扫描头文件
   @details
   @author
   @date   2010-12-13
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef	_IO_KEY_TAB_H_
#define _IO_KEY_TAB_H_

//#include "config.h"
#include "msg.h"
#include "sys.h"



/*******************************************************************************************/


/*短按*/
#define IO_KEY_SHORT  \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,

   
/*长按*/    
#define IO_KEY_LONG	 \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,
    
    
/*连按*/
#define IO_KEY_HOLD	 \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,

    
 /*长按抬起*/
#define IO_KEY_LONG_UP \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,
	

const u8 io_key_table[3][7]=				//Music模式下的遥控转换表
{
    {IO_KEY_SHORT},     /*短按*/
    {IO_KEY_LONG},      /*长按*/
    {IO_KEY_HOLD},      /*连按*/
    //{IO_KEY_LONG_UP},   /*长按抬起*/
};
#endif




