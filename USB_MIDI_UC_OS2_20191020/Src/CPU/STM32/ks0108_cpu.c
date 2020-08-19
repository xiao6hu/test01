/*--------------------------------------------------------------------------*/
/**@file     ks0108_cpu.c
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#include "ks0108_cpu.h"

void LCD_GPIO_Configuration(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    
	//RCC_APB2PeriphClockCmd( LCD_EN_RCC | LCD_RW_RCC | LCD_RS_RCC | LCD_CSA_RCC | LCD_CSB_RCC | LCD_BUS_RCC, ENABLE);

	GPIO_InitStructure.Pin =  LCD_EN_GPIO;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 
	HAL_GPIO_Init( LCD_EN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =  LCD_RW_GPIO; 
	HAL_GPIO_Init(LCD_RW_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =  LCD_RS_GPIO; 
	HAL_GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =  LCD_CSA_GPIO; 
	HAL_GPIO_Init(LCD_CSA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =  LCD_CSB_GPIO; 
	HAL_GPIO_Init(LCD_CSB_PORT, &GPIO_InitStructure);	

	set_lcd_port_out();

}


void set_lcd_port_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 
	GPIO_InitStructure.Pin =  LCD_BUS_GPIO; 
    GPIO_InitStructure.Pull =  GPIO_PULLUP; 
	HAL_GPIO_Init(LCD_BUS_PORT, &GPIO_InitStructure);
}


void set_lcd_port_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStructure.Pin =  LCD_BUS_GPIO; 
	HAL_GPIO_Init(LCD_BUS_PORT, &GPIO_InitStructure);
}

/*
void SET_LCD_E(bool Ststus)
{
	GPIO_WriteBit((LCD_E_PORT , LCD_E_GPIO, Ststus);
}

void SET_LCD_RW(bool Ststus)
{
	GPIO_WriteBit((LCD_RW_PORT , LCD_RW_GPIO, Ststus);
}

void SET_LCD_RS(bool Ststus)
{
	GPIO_WriteBit((LCD_RS_PORT , LCD_RS_GPIO, Ststus);
}
*/
void Write_lcd_bus(uint8_t Data)
{
	
	uint16_t PortVal;
	
	PortVal = MEM_ADDR(GPIOE_IDR_Addr);
	
	PortVal &= (~LCD_BUS_GPIO);
	
	PortVal |= ((uint16_t)Data<<8);
	
	//GPIO_Write(LCD_BUS_PORT,PortVal);
	
	MEM_ADDR(GPIOE_ODR_Addr)=PortVal;
	
	//GPIO_Write(LCD_BUS_PORT,Data);
	
	
}
uint8_t Read_lcd_bus(void)
{
	uint16_t PortVal;
	//PortVal = GPIO_ReadInputData(LCD_BUS_PORT);

    PortVal = MEM_ADDR(GPIOE_IDR_Addr) >>8;
	return (uint8_t)PortVal;
}


/*
#if (LCD_XSIZE == 128)
//------------------------------------
//2个LCD片选信号
//------------------------------------
//LCD CS A   (CSA = 0 CSB = 1) 
void LCD_CSA(void)
{
	GPIO_ResetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_SetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}
//------------------------------------
//LCD CS B   (CSA = 1 CSB = 0) 
void LCD_CSB(void)
{
	GPIO_SetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_ResetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}
#endif


#if (LCD_XSIZE == 192)
//------------------------------------
//3个LCD片选信号
//LCD CS A   (CSA = 0 CSB = 1) 
void LCD_CSA(void)
{
	GPIO_ResetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_ResetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}

//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
void LCD_CSB(void)
{
	GPIO_ResetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_SetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}
//------------------------------------
//LCD CS B   (CSA = 0 CSB = 1) 
void LCD_CSC(void)
{
	GPIO_SetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_ResetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}
#endif


//LCD CS Non   (CSA = 1 CSB = 1)
void LCD_CS_None(void)
{
	GPIO_SetBits(LCD_CSA_PORT , LCD_CSA_GPIO);
	GPIO_ResetBits(LCD_CSB_PORT , LCD_CSB_GPIO);
}
*/
