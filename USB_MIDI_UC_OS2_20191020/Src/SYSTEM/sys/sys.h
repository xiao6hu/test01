#ifndef __SYS_H
#define __SYS_H	

//#include "stm32f10x.h"

#include "stm32f2xx_hal.h"

#include <stdlib.h>
#include <string.h>
#include "typedef.h"
/*
#define _CONST_ const
	

#define true  1
#define false 0
	
#define TRUE  1
#define FALSE 0

typedef   signed          char s8;
typedef   signed          char S8;

typedef   unsigned        char u8;
typedef   unsigned        char U8;


typedef   signed short     int s16;
typedef   signed short     int S16;

typedef   unsigned short   int u16;
typedef   unsigned short   int U16;


typedef   signed           int s32;
typedef   unsigned         int u32;

typedef   signed           int S32;
typedef   unsigned         int U32;

#define    BIT(n)   (1<<n)
*/
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS		1		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入



#define BigLittleSwap16(A)        ((((uint16)(A) & 0xff00) >> 8) | \
                                                       (((uint16)(A) & 0x00ff) << 8))
 
 
#define BigLittleSwap32(A)        ((((uint32)(A) & 0xff000000) >> 24) | \
                                                       (((uint32)(A) & 0x00ff0000) >> 8) | \
                                                       (((uint32)(A) & 0x0000ff00) << 8) | \
                                                       (((uint32)(A) & 0x000000ff) << 24))

void NVIC_Configuration(void);

#define my_strcpy(s1, s2) strcpy((char*)s1, (_CONST_ char*)s2)
	
#define my_strncpy(s1, s2, n) strncpy((char*)s1, (_CONST_ char*)s2, n)

#define my_strcmp(s1, s2) strcmp((_CONST_ char*)s1, (_CONST_ char*)s2)
	
#define my_strchr(s, c) strchr((_CONST_ char*)s, (int)c)

#define my_memcpy(s1, s2, n) *memcpy(s1, s2, n)


#define my_strlen(s) strlen((const char *)s /*s*/)



#endif
