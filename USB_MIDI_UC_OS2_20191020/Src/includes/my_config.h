
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


#define  CPU_TYPE STM32		//ѡ��STM32 CPU����
//#define  CPU_TYPE STC		//ѡ��STC CPU����
//#define  CPU_TYPE SWM180X		//ѡ��STC CPU����     
#define IO_KEY_EN			//IO����ʹ�ܿ���
//#define IR_REMOTE_EN		//����ң��ʹ�ܿ���


//#define MODEL	L_MAG511	//ѡ��L_MAG511����
//#define MODEL	L_MAG511B	//ѡ��L_MAG511B����
//#define MODEL	L_MAG511C	//ѡ��L_MAG511C����

#define FILTER_TYTE_Q15
//#define FILTER_TYTE_F32

#define FILTER_TYTE 0




//#define INT_FLASH_SAVE
//#define EXT_FLASH_SAVE
#define EEPROM_SAVE


//0:CRC16
//1: ��ӡ�ȡ������һ

#define CHECK_TYPE     0

#define MAX_DEVICE_NUM 250

#endif
