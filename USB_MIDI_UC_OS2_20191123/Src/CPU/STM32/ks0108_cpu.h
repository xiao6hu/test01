/*--------------------------------------------------------------------------*/
/**@file    ks0108_cpu.h
   @brief   ks0108显示芯片和CPU类型相关文件
   @details
   @author  hjt
   @date    2017-6-12
   @note    
*/
/*----------------------------------------------------------------------------*/
#ifndef __KS0108_CPU_H__
#define __KS0108_CPU_H__

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"

#include "stm32f2xx_hal.h"
#include "sys.h"
#include "ks0108.h"



#define LCD_XSIZE  128
#define LCD_YSIZE  8

//液晶控制线
/*
#define LCD_EN_GPIO     GPIO_Pin_10
#define LCD_EN_PORT     GPIOC
#define LCD_EN_RCC      RCC_APB2Periph_GPIOC
*/
#define LCD_EN_GPIO     GPIO_PIN_2
#define LCD_EN_PORT     GPIOB
#define LCD_EN_RCC      RCC_APB2Periph_GPIOB


/*
#define LCD_RW_GPIO     GPIO_Pin_11
#define LCD_RW_PORT     GPIOC
#define LCD_RW_RCC      RCC_APB2Periph_GPIOC
*/

#define LCD_RW_GPIO     GPIO_PIN_0
#define LCD_RW_PORT     GPIOB
#define LCD_RW_RCC      RCC_APB2Periph_GPIOB




#define LCD_RS_GPIO     GPIO_PIN_5
#define LCD_RS_PORT     GPIOC
#define LCD_RS_RCC      RCC_APB2Periph_GPIOC


/*
#define LCD_CSA_GPIO    GPIO_Pin_2
#define LCD_CSA_PORT    GPIOD
#define LCD_CSA_RCC     RCC_APB2Periph_GPIOD
*/

#define LCD_CSA_GPIO    GPIO_PIN_7
#define LCD_CSA_PORT    GPIOE
#define LCD_CSA_RCC     RCC_APB2Periph_GPIOE


#define LCD_CSB_GPIO    GPIO_PIN_1
#define LCD_CSB_PORT    GPIOB
#define LCD_CSB_RCC     RCC_APB2Periph_GPIOB


#define LCD_BUS_GPIO  	(GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8)  
#define LCD_BUS_PORT    GPIOE
#define LCD_BUS_RCC     RCC_APB2Periph_GPIOE


void set_lcd_port_in(void);
void set_lcd_port_out(void);
void Write_lcd_bus(uint8_t Data);
uint8_t Read_lcd_bus(void);


#define SET_LCD_BUS_IN()     set_lcd_port_in()
#define SET_LCD_BUS_OUT()    set_lcd_port_out()
#define WRITE_LCD_BUS(Data)  Write_lcd_bus(Data)
#define READ_LCD_BUS()       Read_lcd_bus()


#define LCD_EN(Status)	HAL_GPIO_WritePin(LCD_EN_PORT , LCD_EN_GPIO, (GPIO_PinState)Status)
#define LCD_RW(Status) 	HAL_GPIO_WritePin(LCD_RW_PORT , LCD_RW_GPIO, (GPIO_PinState)Status)
#define LCD_RS(Status)	HAL_GPIO_WritePin(LCD_RS_PORT , LCD_RS_GPIO, (GPIO_PinState)Status)


#if (LCD_XSIZE == 128)
//------------------------------------
//2个LCD片选信号
//------------------------------------
//LCD CS A   (CSA = 1 CSB = 0) 
#define LCD_CSA()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_SET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_RESET)


//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
#define LCD_CSB()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_RESET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_SET)

#endif


#if (LCD_XSIZE == 192)
//------------------------------------
//3个LCD片选信号
//LCD CS A   (CSA = 0 CSB = 1) 
#define LCD_CSA()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_RESET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_RESET)

//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
#define LCD_CSB()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_RESET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_SET)



//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
#define LCD_CSC()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_SET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_RESET)

#endif


//LCD CS Non   (CSA = 1 CSB = 1)
#define LCD_CS_None()\
    HAL_GPIO_WritePin(LCD_CSA_PORT , LCD_CSA_GPIO,GPIO_PIN_SET);\
    HAL_GPIO_WritePin(LCD_CSB_PORT , LCD_CSB_GPIO,GPIO_PIN_SET)


#endif /* __KS0108_CPU_H__ */



