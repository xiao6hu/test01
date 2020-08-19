/**@file    ks0108_cpu.h
   @brief   ks0108显示芯片和CPU类型相关文件
   @details
   @author  hjt
   @date    2017-6-12
   @note    
*/

#ifndef __KS0108_CPU_H__
#define __KS0108_CPU_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

#define LCD_XSIZE  128
#define LCD_YSIZE  8



//液晶控制线

#define LCD_EN_GPIO     GPIOA
#define LCD_EN_PORT     PORTA
#define LCD_EN_PIN      PIN11

#define LCD_RW_GPIO     GPIOA
#define LCD_RW_PORT     PORTA
#define LCD_RW_PIN      PIN10


#define LCD_RS_GPIO     GPIOA
#define LCD_RS_PORT     PORTA
#define LCD_RS_PIN      PIN6

#define LCD_CSA_GPIO    GPIOA
#define LCD_CSA_PORT    PORTA
#define LCD_CSA_PIN     PIN8

#define LCD_CSB_GPIO    GPIOA
#define LCD_CSB_PORT    PORTA
#define LCD_CSB_PIN     PIN9


#define LCD_BUS_GPIO_MASK  	0x00FF 
#define LCD_BUS_PORT    PORTB
#define LCD_BUS_GPIO    GPIOB
//---------------------------------
#define LCD_D0_PORT     PORTB
#define LCD_D1_PORT     PORTB
#define LCD_D2_PORT     PORTB
#define LCD_D3_PORT     PORTB
#define LCD_D4_PORT     PORTB
#define LCD_D5_PORT     PORTB
#define LCD_D6_PORT     PORTB
#define LCD_D7_PORT     PORTB
//---------------------------------
#define LCD_D0_GPIO     GPIOB
#define LCD_D1_GPIO     GPIOB
#define LCD_D2_GPIO     GPIOB
#define LCD_D3_GPIO     GPIOB
#define LCD_D4_GPIO     GPIOB
#define LCD_D5_GPIO     GPIOB
#define LCD_D6_GPIO     GPIOB
#define LCD_D7_GPIO     GPIOB
//---------------------------------
#define LCD_D0_PIN      PIN0
#define LCD_D1_PIN      PIN1
#define LCD_D2_PIN      PIN2
#define LCD_D3_PIN      PIN3
#define LCD_D4_PIN      PIN4
#define LCD_D5_PIN      PIN5
#define LCD_D6_PIN      PIN6
#define LCD_D7_PIN      PIN7
//---------------------------------

void set_lcd_port_in(void);
void set_lcd_port_out(void);
void Write_lcd_bus(uint8_t Data);
uint8_t Read_lcd_bus(void);


#define SET_LCD_BUS_IN()     set_lcd_port_in()
#define SET_LCD_BUS_OUT()    set_lcd_port_out()
#define WRITE_LCD_BUS(Data)  Write_lcd_bus(Data)
#define READ_LCD_BUS()       Read_lcd_bus()


#define LCD_EN(Status)	GPIO_WriteBit(LCD_EN_GPIO , LCD_EN_PIN, (BitAction)Status)
#define LCD_RW(Status) 	GPIO_WriteBit(LCD_RW_GPIO , LCD_RW_PIN, (BitAction)Status)
#define LCD_RS(Status)	GPIO_WriteBit(LCD_RS_GPIO , LCD_RS_PIN, (BitAction)Status)






#if (LCD_XSIZE == 128)
//------------------------------------
//2个LCD片选信号
//------------------------------------
//LCD CS A   (CSA = 1 CSB = 0) 
#define LCD_CSA()\
	GPIO_SetBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_ClrBit(LCD_CSB_GPIO , LCD_CSB_PIN)
	
//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
#define LCD_CSB()\
	GPIO_ClrBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_SetBit(LCD_CSB_GPIO , LCD_CSB_PIN)
#endif


#if (LCD_XSIZE == 192)
//------------------------------------
//3个LCD片选信号
//LCD CS A   (CSA = 0 CSB = 0) 
#define LCD_CSA()\
	GPIO_ClrBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_ClrBit(LCD_CSB_GPIO , LCD_CSB_PIN)

//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
#define LCD_CSB()\
	GPIO_ClrBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_SetBit(LCD_CSB_GPIO , LCD_CSB_PIN)
//------------------------------------
//LCD CS B   (CSA = 1 CSB = 0) 
#define LCD_CSC()\
	GPIO_SetBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_ClrBit(LCD_CSB_GPIO , LCD_CSB_PIN)
#endif


//LCD CS Non   (CSA = 1 CSB = 1)
#define LCD_CS_None()\
	GPIO_SetBit(LCD_CSA_GPIO , LCD_CSA_PIN);\
	GPIO_SetBit(LCD_CSB_GPIO , LCD_CSB_PIN)

#endif /* __KS0108_CPU_H__ */



