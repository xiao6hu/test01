/******************************************************************************
 * MenuLanguage.C - 语言切换模版
 *
 *
 * DESCRIPTION: -
 *
 * modification history
 * --------------------
 * 01a, 04/sep/2007, 梁炎昌 written
 * --------------------
 ******************************************************************************/
//#include "CPU.H"
#include "Ks0108.H"
#include "Menu.H"
#include "Parameters.h"


/*
这里是语言切换界面，这里也演示了如何通过提前修改Item Where
void DispMenuLanguageInit()函数中：
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
    if(Flash == FlashMode_ManualInit){//首次进入本层菜单，调整Item where
        PUSH();
        Item = language;
        Where = Item;
    }
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
*/


void DispMenuLanguageEscUp(void)
{
}
void DispMenuLanguageEscDown(void)
{
}
	
void DispMenuLanguageEscOk(void)
{
}

//语言
//Bar型菜单
void DispMenuLanguageInit()
{
//----------------------
    GUI_Clear();
//--------------------
    //切换Bar显示内容
//-----------------
    BarMenu = (struct Menu *)&MenuLanguage[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //用户的初始化代码请放在这里
    //显示父菜单名字

    //显示按键提示          2-->OK
//    GUI_DispStringAt(Tip[language].Text+2*(Tip[language].TextLen),Enter_PosX,Enter_PosY);
    GUI_DispStringAtBar(*(Tip[language].Text+2),0,Enter_PosY,126,GUI_TA_HCENTER);
    if(Flash == FlashMode_ManualInit){//首次进入本层菜单，调整Item where
        PUSH();
        Item = language;
        Where = Item;
    }
    //公共初始化部分
    BarMenuInit();
}
/*
************************************************************
*   FIXME!
*   这里是做语言切换的
*   但是这会有一个有意思的问题:
*   我们的菜单保存机制是备份过去的父菜单的Text等信息的,因此即便是更换了语言,
*   这个保存的信息依然没有被更换,就是说,在这里切换语言后,显示的父索引还是没有变
*   改正方法:
*   通常语言切换的地方就一个路径而已,因此更换语言后正确修改保存的信息...
************************************************************
*/
void DispMenuLanguageUp()
{
    //U16 LanguageBackup;



//-----------
    PUSH();//
//-------------------
    GUI_Clear();
    if(Item==0){
        language = CN;
        GUI_Delay(10);
    }else if(Item==1){
        language = EN;
        GUI_Delay(10);
    }
    RamSetParameters.Language=language;
    //LanguageBackup =RamSetParameters.Language;

#ifdef INT_FLASH_SAVE
       FLASH_WRITE(RamSetParameters.Language,RamSetParameters.Language,RamSetParameters.Language);     
       FLASH_READ(RamSetParameters.Language,RamSetParameters.Language,RamSetParameters.Language); 
#endif
       
#ifdef EEPROM_SAVE
       AT24CXX_Write(EepromSetParametersStartAddress+(U32)&RamSetParameters.Language-(U32)&RamSetParameters,//待写入地址
                   (uint8_t *)&RamSetParameters.Language,//待写入数据缓冲区
                   (sizeof(RamSetParameters.Language) / sizeof(uint8_t))); //待写入数据长度
       
       AT24CXX_Read(EepromSetParametersStartAddress+(U32)&RamSetParameters.Language-(U32)&RamSetParameters,//待写入地址
                   (uint8_t *)&RamSetParameters.Language,//待写入数据缓冲区
                   (sizeof(RamSetParameters.Language) / sizeof(uint8_t))); //待写入数据长度
           
#endif
  
    GUI_Clear();
//-------------------
    POP();
//-------------------

}
void DispMenuLanguageDown()
{
    POP();
}
void DispMenuLanguageLeft()
{
    BarMenuLeft();
}
void DispMenuLanguageRight()
{
    BarMenuRight();
}






