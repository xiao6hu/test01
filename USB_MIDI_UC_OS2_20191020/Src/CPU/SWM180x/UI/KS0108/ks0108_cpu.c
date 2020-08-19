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


/****************************************************************
 * ��������void set_lcd_port_in(void)
 * ����  ������LCD���ݿ�Ϊ����״̬ 
 * ����  ���� 
 * ���  ���� 
 ***************************************************************/ 
void set_lcd_port_in(void)
{
    GPIO_Init(LCD_D0_GPIO, LCD_D0_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D1_GPIO, LCD_D1_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D2_GPIO, LCD_D2_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D3_GPIO, LCD_D3_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D4_GPIO, LCD_D4_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D5_GPIO, LCD_D5_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D6_GPIO, LCD_D6_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D7_GPIO, LCD_D7_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);  
}

/****************************************************************
 * ��������void set_lcd_port_out(void)
 * ����  ������LCD���ݿ�Ϊ���״̬ 
 * ����  ���� 
 * ���  ���� 
 ***************************************************************/ 
void set_lcd_port_out(void)
{
    GPIO_Init(LCD_D0_GPIO, LCD_D0_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D1_GPIO, LCD_D1_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D2_GPIO, LCD_D2_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D3_GPIO, LCD_D3_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D4_GPIO, LCD_D4_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D5_GPIO, LCD_D5_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D6_GPIO, LCD_D6_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_D7_GPIO, LCD_D7_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS); 
}

/****************************************************************
 * ��������void LCD_GPIO_Configuration(void)	
 * ����  ��LCD I/O������ 
 * ����  ���� 
 * ���  ���� 
 ***************************************************************/ 
void LCD_GPIO_Configuration(void)	
{
    GPIO_Init(LCD_EN_GPIO, LCD_EN_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_RW_GPIO, LCD_RW_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_RS_GPIO, LCD_RS_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_CSA_GPIO, LCD_CSA_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(LCD_CSB_GPIO, LCD_CSB_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
	set_lcd_port_out();
}

/****************************************************************
 * ��������void Write_lcd_bus(U8 Data)
 * ����  ��д���ݵ�LCD���ݿ� 
 * ����  ��U8 Data ��д������� 
 * ���  ���� 
 ***************************************************************/ 
void Write_lcd_bus(U8 Data)
{
	U16 PortVal;
	PortVal = GPIO_ReadData(LCD_BUS_GPIO);
	PortVal &= (~LCD_BUS_GPIO_MASK);
	PortVal |= Data;
	GPIO_WriteData(LCD_BUS_GPIO,PortVal);
}

/****************************************************************
 * ��������u8 get_io_key_val(void)
 * ����  ����ȡLCD���ݿ�����  
 * ����  ���� 
 * ���  ��U8 ���������� 
 ***************************************************************/ 
U8 Read_lcd_bus(void)
{
/*
	U16 PortVal;
	PortVal = GPIO_ReadData(LCD_BUS_PORT);
	return (U8)PortVal;
*/
    return (U8)GPIO_ReadData(LCD_BUS_GPIO);
}


/****************************************************************
 * ��������void LCD_DELAY(void)
 * ����  ��LCD �ӳٳ���
 * ����  ���� 
 * ���  ���� 
 ***************************************************************/ 
void LCD_DELAY(void)
{
	
	U8 del=20;
	do
	{
		del--;	
	}
	while (del);
	
}
	



