/*--------------------------------------------------------------------------*/
/**@file   key.h
   @brief  ������Ϣɨ��ͷ�ļ�
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


/*�̰�*/
#define IO_KEY_SHORT  \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,

   
/*����*/    
#define IO_KEY_LONG	 \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,
    
    
/*����*/
#define IO_KEY_HOLD	 \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,

    
 /*����̧��*/
#define IO_KEY_LONG_UP \
    /*01*/    NO_MSG,\
	/*02*/    NO_MSG,\
	/*03*/    NO_MSG,\
	/*04*/    NO_MSG,\
	/*05*/    NO_MSG,\
	/*06*/    NO_MSG,\
	/*07*/    NO_MSG,
	

const u8 io_key_table[3][7]=				//Musicģʽ�µ�ң��ת����
{
    {IO_KEY_SHORT},     /*�̰�*/
    {IO_KEY_LONG},      /*����*/
    {IO_KEY_HOLD},      /*����*/
    //{IO_KEY_LONG_UP},   /*����̧��*/
};
#endif




