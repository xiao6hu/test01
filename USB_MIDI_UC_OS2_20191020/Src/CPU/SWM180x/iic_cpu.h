/*--------------------------------------------------------------------------*/
/**@file     iic_cpu.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef __UART_CPU_H__
#define __UART_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

//IIC时钟线

#define IIC_SCL_GPIO     GPIOC
#define IIC_SCL_PORT     PORTC
#define IIC_SCL_PIN      PIN5
//IIC数据线
#define IIC_SDA_GPIO     GPIOC
#define IIC_SDA_PORT     PORTC
#define IIC_SDA_PIN      PIN4



void set_iic_sda_in(void);
void set_iic_sda_out(void);

//数据口方向设置
#define IIC_SDA_IN()  set_iic_sda_in()
#define IIC_SDA_OUT() set_iic_sda_out()




/*

#define IIC_SDA_IN()\  
        GPIO_Init(IIC_SDA_GPIO ,\
                  IIC_SDA_PIN,\
                  DIR_IN,\ 
                  PULL_UP_DIS,\
                  PULL_DOWM_DIS,\
                  OPEN_DRAIN_DIS)
                  
#define IIC_SDA_OUT() \
        GPIO_Init(IIC_SDA_GPIO ,\ 
                  IIC_SDA_PIN,\ 
                  DIR_OUT,\ 
                  PULL_UP_DIS,\ 
                  PULL_DOWM_DIS,\
                  OPEN_DRAIN_DIS)
*/


//IO操作函数
#define IIC_SCL(Ststus)	GPIO_WriteBit(IIC_SCL_GPIO , IIC_SCL_PIN, (BitAction)Ststus)
#define IIC_SDA(Ststus) GPIO_WriteBit(IIC_SDA_GPIO , IIC_SDA_PIN, (BitAction)Ststus)
#define READ_SDA() 	GPIO_GetBit(IIC_SDA_GPIO , IIC_SDA_PIN)
#endif





