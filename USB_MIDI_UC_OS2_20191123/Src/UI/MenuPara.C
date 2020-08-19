/******************************************************************************
 * MenuPara.C - "参数"菜单  菜单函数使用模版
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



void DispMenuParaEscUp(void)
{
}
void DispMenuParaEscDown(void)
{
}
void DispMenuParaEscOk(void)
{
}



//参数
void DispMenuParaInit()
{
//----------------------
    GUI_Clear();
//--------------------
    //切换Bar显示内容
//---------------------
    BarMenu = (struct Menu *)&MenuPara[language];
    ItemNum = (*BarMenu).TextNum;
    DispItem = (*BarMenu).Text;

    //用户的初始化代码请放在这里
    //显示按键对应的文字提示,1-->"进入"
    GUI_DispStringAtBar(*(Tip[language].Text+1),0,Enter_PosY,126,GUI_TA_HCENTER);
    //公共初始化部分
    BarMenuInit();
}

/*
*****************************************************************************
菜单使用例程
0.Jump2Menu + FlashMode_AutoInit/FlashMode_ManualInit
  跳转到任意菜单,FlashMode_AutoInit-->在BarMenuInit函数中执行PUSH;
  FlashMode_ManualInit-->子菜单的Init函数中PUSH,看例子:DispMenuLanguageInit
1.PUSH + POP
  执行一些功能,并使用LCD后,回到原有界面,回到原有界面--包括反显位置都恢复
2.Jump2Menu(SelfMenuIndex,FlashMode_ReFlash);
  执行一些功能,并使用LCD后,执行如上调用,回到原有界面--包括反显位置都恢复
3.POP
  执行一些功能,并使用LCD后,回到上层菜单,恢复上层界面
4.Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
  执行一定功能后,但不占用LCD的话,直接界面不动

*****************************************************************************
*/
/*
"设置设备地址"},
{"设置设备总数"},
{"音乐延时时间"},
{"灯光延时时间"},
{"踩踏计数时间"},
{"踩踏延时时间"},
{"同时踩踏人数"},
{"音节延时时间"},
{"音符延时时间"},

*/
void DispMenuParaUp()
{
    switch(Item){
        case 0:{//Jump2Menu
            //GUI_Clear();
			
			
			PUSH();
			
			Jump2Menu(SetDeviceAddressIndex,FlashMode_AutoInit);
            break;
        }
        case 1:{//PUSH  -->POP
			PUSH();
			Jump2Menu(SetDeviceTotalIndex,FlashMode_AutoInit);
			
            break;
        }
	
        case 2:
		{ 
			PUSH();
            Jump2Menu(SetMusicDelayTimeIndex,FlashMode_AutoInit);
            break;
        }
		
        case 3:
		{  
			PUSH();
            Jump2Menu(SetLightsDelayTimeIndex,FlashMode_AutoInit);
            break;
        }
		
		
		
		case 4:
		{
			PUSH();
            Jump2Menu(SetTrampleCountTimeIndex,FlashMode_AutoInit);
            break;
        }
		
		
        case 5:
		{
			PUSH();
            Jump2Menu(SetTrampleDelayTimeIndex,FlashMode_AutoInit);
            break;
        }

		case 6:
		{
			PUSH();
            Jump2Menu(SetTrampleNumberIndex,FlashMode_AutoInit);
            break;
        }
		
		
		 case 7:
		{
			PUSH();
            Jump2Menu(SetSyllableDelayTimeIndex,FlashMode_AutoInit);
            break;
        }
		
		case 8:
		{
			PUSH();
            Jump2Menu(SetNoteDelayTimeIndex,FlashMode_AutoInit);
            break;
        }
		
		/*
		case 9:
		{
			PUSH();
            Jump2Menu(LightsModeSetIndex,FlashMode_AutoInit);
            break;
        }
		*/
		case 9:
		{
			PUSH();
            Jump2Menu(LightsSpeedSetIndex,FlashMode_AutoInit);
            break;
        }
		
		case 10:
		{
			PUSH();
            Jump2Menu(LightsSpacingSetIndex,FlashMode_AutoInit);
            break;
        }
		
		
		case 11:
		{
			//PUSH();
            Jump2Menu(GameModeSetIndex,FlashMode_ManualInit);
            break;
        }
		
		case 12:
		{
			//PUSH();
			Jump2Menu(BabyModeSetIndex,FlashMode_AutoInit);
            break;
        }
		
		
		case 13:
		{
			PUSH();
			Jump2Menu(CAN1DevicesTotalIndex,FlashMode_AutoInit);
            break;
        }
		
		case 14:
		{
			PUSH();
			Jump2Menu(CAN2DevicesTotalIndex,FlashMode_AutoInit);
            break;
        }
		
		case 15:
		{
			PUSH();
			Jump2Menu(ReturnTopTimeIndex,FlashMode_AutoInit);
            break;
        }
		
		
		case 16:
		{
			//PUSH();
			Jump2Menu(FullColorLightSetIndex,FlashMode_AutoInit);
            break;
        }

		/*
		case 17:
		{
			//PUSH();
			Jump2Menu(Config_S_DevIndex,FlashMode_AutoInit);
			break;
		}
		*/

		case 17:
		{
			PUSH();
			Jump2Menu(Config_M_DevIndex,FlashMode_AutoInit);
			break;
		}


        default:{
            Jump2Menu(SelfMenuIndex,FlashMode_NoAction);
            return;
        }
    }
}
void DispMenuParaDown()
{
    POP();
}

void DispMenuParaLeft()
{
    BarMenuLeft();
}
void DispMenuParaRight()
{
    BarMenuRight();
}

