/*--------------------------------------------------------------------------*/
/**@file     ui.c
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/
/* Includes */
#include "ui.h"



/*
#include "ZhiKu\Chinese.c"
#include "ZhiKu\ASCII.c"
#include "ZhiKu\PIC.c"

#include "KS0108\ks0108.c"
/

#if (MENU_TYPE == L_MAG511) 
#include "Menu\L_mag511_menu.c"
#endif
*/


void UI_Init(void)
{
	LCD_GPIO_Configuration();
	GUI_Init();
    GUI_Clear();
	Flash = FlashMode_AutoInit;
}



void UI_Loop(void)
{
	if(KeyFuncIndex ==0) //KeyFuncIndex为0时是待机界面
	{
		//待机界面代码
		if (Flash)
		{
			DispMenuTop();//待机界面();
			Flash = 0;
		}	
	}
}	


	







