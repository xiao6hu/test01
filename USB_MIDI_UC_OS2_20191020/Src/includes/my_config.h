
#ifndef __MY_CONFIG_H__
#define __MY_CONFIG_H__

#include "math.h"
#include "stdlib.h"
#include "string.h"
//#include <absacc.h>
#include "stdio.h"
#include "stdarg.h"
//#include "my_lib\my_stdlib.h"
#include "typedef.h"
#include "sys.h"


#define  CPU_TYPE STM32		//选择STM32 CPU类型
//#define  CPU_TYPE STC		//选择STC CPU类型
//#define  CPU_TYPE SWM180X		//选择STC CPU类型     
#define IO_KEY_EN			//IO按键使能开关
//#define IR_REMOTE_EN		//红外遥控使能开关


//#define MODEL	L_MAG511	//选择L_MAG511机型
//#define MODEL	L_MAG511B	//选择L_MAG511B机型
//#define MODEL	L_MAG511C	//选择L_MAG511C机型

#define FILTER_TYTE_Q15
//#define FILTER_TYTE_F32

#define FILTER_TYTE 0




//#define INT_FLASH_SAVE
//#define EXT_FLASH_SAVE
#define EEPROM_SAVE


//0:CRC16
//1: 相加、取反、加一

#define CHECK_TYPE     0

#define MAX_DEVICE_NUM 250

#endif
