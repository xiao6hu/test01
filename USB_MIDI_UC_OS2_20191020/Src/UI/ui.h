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





#define KS0108 			//选择LCD驱动芯片型号	
#define MENU_TYPE  MODEL  //选择相应机型菜单	

extern  void LCD_GPIO_Configuration(void);	
extern void UI_Init(void);
extern void UI_Loop(void);

#endif  /* __UI_H__ */


