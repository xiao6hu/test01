/*--------------------------------------------------------------------------*/
/**@file     ui.h
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef __UI_H__
#define __UI_H__

//#include "includes.h"

#include "ks0108.h"
#include "Menu.H"





#define KS0108 			//ѡ��LCD����оƬ�ͺ�	
#define MENU_TYPE  MODEL  //ѡ����Ӧ���Ͳ˵�	

extern  void LCD_GPIO_Configuration(void);	
extern void UI_Init(void);
extern void UI_Loop(void);

#endif  /* __UI_H__ */


