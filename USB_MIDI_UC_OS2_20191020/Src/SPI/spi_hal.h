#ifndef __SPI_HAL_H
#define __SPI_HAL_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SPI 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#if 0									  
void SPI3_Init(void);			 //初始化SPI口
void SPI3_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI3_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
extern SPI_HandleTypeDef SPI3_HandleStructure;	
#endif





#if 1
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
extern SPI_HandleTypeDef SPI1_HandleStructure;	
#endif
	 
#endif

