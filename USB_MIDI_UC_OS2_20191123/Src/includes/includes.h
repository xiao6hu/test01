#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

//#include "stm32f10x.h"
//#include "stm32f10x_rcc.h"

#include "stm32f2xx.h"
#include "my_config.h"
#include "sys.h"
#include  "ucos_ii.h"  		//uC/OS-II系统函数头文件
#include  	"BSP.h"			//与开发板相关的函数
#include 	"app.h"			//用户任务函数

#include "Parameters.h"
#include 	"led.h"			//LED驱动函数


#include "fatfs.h"
#include "ff.h"

#include "usb_host.h"

#include "mmc_sd.h"
#include "midi.h"


#include "usart1.h"
#include "usart2.h"
#include "uart4.h"


#include "timr.h"
#include "timr_delay.h"

#include "ui.h"
#include "key.h"

#include "msg.h"
#include "mmc_sd.h"	
#include "lights.h"

#include "stmflash.h"


#include "24cxx.h"
#include "RTC.h"
#include "CANOpenObjDictConfig.h"

#include "can.h"

#include "XFS5152CE.h"

#include "game.h"
#include "game1.h"

#include "Menu.H"


#include "LedControl.h"

#endif //__INCLUDES_H__
