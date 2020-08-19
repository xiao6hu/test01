/******************************************************************************
 * MenuFSM.C - 菜单框架和FSM代码
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
//#include "KeyScan.H"
#include "KS0108.H"
#include "Menu.H"

#include "key.h"



//FIXME!
//改进想法:如何在菜单中可以接管其它有效按键,这里程序仅仅接管了方向按键,若能根据1--9这些数字按键跳转也是不错
//额外的问题是如何在应用程序中得到按键而却不需要在应用程序中做键盘检查循环(抢占CPU时间了,会使得原有可以在背景运行的程序无法执行)
/*
*   A>改进FSM编号的机制,使得维护更简单,能否自动编号?这样会使得添加FSM表容易
*   B>
*
*
*/
//-----------------------------------
//MenuTop.C
void DispMenuTop(void);
void DispMenuTopUp(void);
void DispMenuTopDown(void);
void DispMenuTopLeft(void);
void DispMenuTopRight(void);

void DispMenuTopEscUp(void);
void DispMenuTopEscDown(void);
void DispMenuTopEscOk(void);



void DispIcoMenuInit(void);
void DispIcoMenuUp(void);
void DispIcoMenuDown(void);
void DispIcoMenuLeft(void);
void DispIcoMenuRight(void);

void DispIcoMenuTopEscUp(void);
void DispIcoMenuTopEscDown(void);
void DispIcoMenuTopEscOk(void);






//MenuFunc.C
void DispMenuFuncInit(void);
void DispMenuFuncUp(void);
void DispMenuFuncDown(void);
void DispMenuFuncLeft(void);
void DispMenuFuncRight(void);

void DispMenuFuncEscUp(void);
void DispMenuFuncEscDown(void);
void DispMenuFuncEscOk(void);




//MenuPara.C
void DispMenuParaInit(void);
void DispMenuParaUp(void);
void DispMenuParaDown(void);
void DispMenuParaLeft(void);
void DispMenuParaRight(void);

void DispMenuParaEscUp(void);
void DispMenuParaEscDown(void);
void DispMenuParaEscOk(void);



//MenuLanguage.C
void DispMenuLanguageInit(void);
void DispMenuLanguageUp(void);
void DispMenuLanguageDown(void);
void DispMenuLanguageLeft(void);
void DispMenuLanguageRight(void);

void DispMenuLanguageEscUp(void);
void DispMenuLanguageEscDown(void);
void DispMenuLanguageEscOk(void);



//MenuMeasure.C
void DispMenuMeasureInit(void);
void DispMenuMeasureUp(void);
void DispMenuMeasureDown(void);
void DispMenuMeasureLeft(void);
void DispMenuMeasureRight(void);

#ifdef DynamicBar
//MenuFunc.C
void DispMenuDataInit(void);
void DispMenuDataUp(void);
void DispMenuDataDown(void);
void DispMenuDataLeft(void);
void DispMenuDataRight(void);
#endif
//-----------------------------------


void DispMenuSetDeviceAddressInit(void);
void DispMenuSetDeviceAddressUp(void);
void DispMenuSetDeviceAddressDown(void);
void DispMenuSetDeviceAddressLeft(void);
void DispMenuSetDeviceAddressRight(void);
void DispMenuSetDeviceAddressEscUp(void);
void DispMenuSetDeviceAddressEscDown(void);
void DispMenuSetDeviceAddressEscOk(void);



void DispMenuSetDeviceTotalInit(void);
void DispMenuSetDeviceTotalUp(void);
void DispMenuSetDeviceTotalDown(void);
void DispMenuSetDeviceTotalLeft(void);
void DispMenuSetDeviceTotalRight(void); 
void DispMenuSetDeviceTotalEscUp(void);
void DispMenuSetDeviceTotalEscDown(void);
void DispMenuSetDeviceTotalEscOk(void);



//------------------------------------------------------------------		
//设置音乐延时时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetMusicDelayTimeInit(void);
void DispMenuSetMusicDelayTimeUp(void);
void DispMenuSetMusicDelayTimeDown(void);
void DispMenuSetMusicDelayTimeLeft(void);
void DispMenuSetMusicDelayTimeRight(void); 
void DispMenuSetMusicDelayTimeEscUp(void);
void DispMenuSetMusicDelayTimeEscDown(void);
void DispMenuSetMusicDelayTimeEscOk(void);

//------------------------------------------------------------------		
//设置灯光延时时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetLightsDelayTimeInit(void);
void DispMenuSetLightsDelayTimeUp(void);
void DispMenuSetLightsDelayTimeDown(void);
void DispMenuSetLightsDelayTimeLeft(void);
void DispMenuSetLightsDelayTimeRight(void); 
void DispMenuSetLightsDelayTimeEscUp(void);
void DispMenuSetLightsDelayTimeEscDown(void);
void DispMenuSetLightsDelayTimeEscOk(void);


//------------------------------------------------------------------		
//设置踩踏计数时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetTrampleCountTimeInit(void);
void DispMenuSetTrampleCountTimeUp(void);
void DispMenuSetTrampleCountTimeDown(void);
void DispMenuSetTrampleCountTimeLeft(void);
void DispMenuSetTrampleCountTimeRight(void); 
void DispMenuSetTrampleCountTimeEscUp(void);
void DispMenuSetTrampleCountTimeEscDown(void);
void DispMenuSetTrampleCountTimeEscOk(void);

//------------------------------------------------------------------		
//设置踩踏延时时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetTrampleDelayTimeInit(void);
void DispMenuSetTrampleDelayTimeUp(void);
void DispMenuSetTrampleDelayTimeDown(void);
void DispMenuSetTrampleDelayTimeLeft(void);
void DispMenuSetTrampleDelayTimeRight(void); 
void DispMenuSetTrampleDelayTimeEscUp(void);
void DispMenuSetTrampleDelayTimeEscDown(void);
void DispMenuSetTrampleDelayTimeEscOk(void);

//------------------------------------------------------------------		
//设置同时踩踏人数功能函数声明
//------------------------------------------------------------------
void DispMenuSetTrampleNumberInit(void);
void DispMenuSetTrampleNumberUp(void);
void DispMenuSetTrampleNumberDown(void);
void DispMenuSetTrampleNumberLeft(void);
void DispMenuSetTrampleNumberRight(void);
void DispMenuSetTrampleNumberEscUp(void);
void DispMenuSetTrampleNumberEscDown(void);
void DispMenuSetTrampleNumberEscOk(void);

//------------------------------------------------------------------		
//设置音节延时时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetSyllableDelayTimeInit(void);
void DispMenuSetSyllableDelayTimeUp(void);
void DispMenuSetSyllableDelayTimeDown(void);
void DispMenuSetSyllableDelayTimeLeft(void);
void DispMenuSetSyllableDelayTimeRight(void); 
void DispMenuSetSyllableDelayTimeEscUp(void);
void DispMenuSetSyllableDelayTimeEscDown(void);
void DispMenuSetSyllableDelayTimeEscOk(void);

//------------------------------------------------------------------		
//设置音符延时时间功能函数声明
//------------------------------------------------------------------
void DispMenuSetNoteDelayTimeInit(void);
void DispMenuSetNoteDelayTimeUp(void);
void DispMenuSetNoteDelayTimeDown(void);
void DispMenuSetNoteDelayTimeLeft(void);
void DispMenuSetNoteDelayTimeRight(void); 
void DispMenuSetNoteDelayTimeEscUp(void);
void DispMenuSetNoteDelayTimeEscDown(void);
void DispMenuSetNoteDelayTimeEscOk(void);


void DispMenuMusicStatusSetInit(void);
void DispMenuMusicStatusSetUp(void);
void DispMenuMusicStatusSetDown(void);
void DispMenuMusicStatusSetLeft(void);
void DispMenuMusicStatusSetRight(void); 
void DispMenuMusicStatusSetEscUp(void);
void DispMenuMusicStatusSetEscDown(void);
void DispMenuMusicStatusSetEscOk(void);
 
void DispMenuLightsStatusSetInit(void);
void DispMenuLightsStatusSetUp(void);
void DispMenuLightsStatusSetDown(void);
void DispMenuLightsStatusSetLeft(void);
void DispMenuLightsStatusSetRight(void);
void DispMenuLightsStatusSetEscUp(void);
void DispMenuLightsStatusSetEscDown(void);
void DispMenuLightsStatusSetEscOk(void);


void DispMenuFactorySetInit(void);
void DispMenuFactorySetUp(void);
void DispMenuFactorySetDown(void);
void DispMenuFactorySetLeft(void);
void DispMenuFactorySetRight(void);
void DispMenuFactorySetEscUp(void);
void DispMenuFactorySetEscDown(void);
void DispMenuFactorySetEscOk(void);




//------------------------------------------------------------------		
//灯带模式设置功能函数
//------------------------------------------------------------------
void DispMenuLightsModeSetInit(void);
void DispMenuLightsModeSetUp(void);
void DispMenuLightsModeSetDown(void);
void DispMenuLightsModeSetLeft(void);
void DispMenuLightsModeSetRight(void); 
void DispMenuLightsModeSetEscUp(void);
void DispMenuLightsModeSetEscDown(void);
void DispMenuLightsModeSetEscOk(void);
		

//------------------------------------------------------------------		
//灯带设置功能函数
//------------------------------------------------------------------
void DispMenuLightsSpeedSetInit(void);
void DispMenuLightsSpeedSetUp(void);
void DispMenuLightsSpeedSetDown(void);
void DispMenuLightsSpeedSetLeft(void);
void DispMenuLightsSpeedSetRight(void);
void DispMenuLightsSpeedSetEscUp(void);
void DispMenuLightsSpeedSetEscDown(void);
void DispMenuLightsSpeedSetEscOk(void);
		

//------------------------------------------------------------------		
//灯带间隔设置功能函数
//------------------------------------------------------------------
void DispMenuLightsSpacingSetInit(void);
void DispMenuLightsSpacingSetUp(void);
void DispMenuLightsSpacingSetDown(void);
void DispMenuLightsSpacingSetLeft(void);
void DispMenuLightsSpacingSetRight(void);
void DispMenuLightsSpacingSetEscUp(void);
void DispMenuLightsSpacingSetEscDown(void);
void DispMenuLightsSpacingSetEscOk(void);




//------------------------------------------------------------------		
//游戏模式选择功能函数
//------------------------------------------------------------------
void DispMenuGameModeSelectInit(void);
void DispMenuGameModeSelectUp(void);
void DispMenuGameModeSelectDown(void);
void DispMenuGameModeSelectLeft(void);
void DispMenuGameModeSelectRight(void); 
void DispMenuGameModeSelectEscUp(void);
void DispMenuGameModeSelectEscDown(void);
void DispMenuGameModeSelectEscOk(void);

//------------------------------------------------------------------		
//游戏模式设置功能函数
//------------------------------------------------------------------
void DispMenuGameModeSetInit(void);
void DispMenuGameModeSetUp(void);
void DispMenuGameModeSetDown(void);
void DispMenuGameModeSetLeft(void);
void DispMenuGameModeSetRight(void); 
void DispMenuGameModeSetEscUp(void);
void DispMenuGameModeSetEscDown(void);
void DispMenuGameModeSetEscOk(void);

//------------------------------------------------------------------		
//儿童模式设置功能函数
//------------------------------------------------------------------
void DispMenuBabyModeSetInit(void);
void DispMenuBabyModeSetUp(void);
void DispMenuBabyModeSetDown(void);
void DispMenuBabyModeSetLeft(void);
void DispMenuBabyModeSetRight(void); 
void DispMenuBabyModeSetEscUp(void);
void DispMenuBabyModeSetEscDown(void);
void DispMenuBabyModeSetEscOk(void);

//------------------------------------------------------------------		
//排数设置功能函数
//------------------------------------------------------------------
void DispMenuRowNumberSetInit(void);
void DispMenuRowNumberSetUp(void);
void DispMenuRowNumberSetDown(void);
void DispMenuRowNumberSetLeft(void);
void DispMenuRowNumberSetRight(void); 
void DispMenuRowNumberSetEscUp(void);
void DispMenuRowNumberSetEscDown(void);
void DispMenuRowNumberSetEscOk(void);


//------------------------------------------------------------------		
//各排设备数设置功能函数
//------------------------------------------------------------------
void DispMenuRowsDeviceNumberSetInit(void);
void DispMenuRowsDeviceNumberSetUp(void);
void DispMenuRowsDeviceNumberSetDown(void);
void DispMenuRowsDeviceNumberSetLeft(void);
void DispMenuRowsDeviceNumberSetRight(void); 
void DispMenuRowsDeviceNumberSetEscUp(void);
void DispMenuRowsDeviceNumberSetEscDown(void);
void DispMenuRowsDeviceNumberSetEscOk(void);

//------------------------------------------------------------------		
//时钟设置功能函数
//------------------------------------------------------------------
void DispMenuClockSetInit(void);
void DispMenuClockSetUp(void);
void DispMenuClockSetDown(void);
void DispMenuClockSetLeft(void);
void DispMenuClockSetRight(void); 
void DispMenuClockSetEscUp(void);
void DispMenuClockSetEscDown(void);
void DispMenuClockSetEscOk(void);

//------------------------------------------------------------------		
//时间设置功能函数
//------------------------------------------------------------------
void DispMenuTimeSetInit(void);
void DispMenuTimeSetUp(void);
void DispMenuTimeSetDown(void);
void DispMenuTimeSetLeft(void);
void DispMenuTimeSetRight(void); 
void DispMenuTimeSetEscUp(void);
void DispMenuTimeSetEscDown(void);
void DispMenuTimeSetEscOk(void);

//------------------------------------------------------------------		
//日期设置功能函数
//------------------------------------------------------------------
void DispMenuDateSetInit(void);
void DispMenuDateSetUp(void);
void DispMenuDateSetDown(void);
void DispMenuDateSetLeft(void);
void DispMenuDateSetRight(void); 
void DispMenuDateSetEscUp(void);
void DispMenuDateSetEscDown(void);
void DispMenuDateSetEscOk(void);

		
//------------------------------------------------------------------		
//关于功能函数
//------------------------------------------------------------------
void DispMenuAboutInit(void);
void DispMenuAboutUp(void);
void DispMenuAboutDown(void);
void DispMenuAboutLeft(void);
void DispMenuAboutRight(void); 
void DispMenuAboutEscUp(void);
void DispMenuAboutEscDown(void);
void DispMenuAboutEscOk(void);



//------------------------------------------------------------------		
//CAN1设备数设置功能函数
//------------------------------------------------------------------
void DispMenuCAN1DevicesTotalInit(void);
void DispMenuCAN1DevicesTotalUp(void);
void DispMenuCAN1DevicesTotalDown(void);
void DispMenuCAN1DevicesTotalLeft(void);
void DispMenuCAN1DevicesTotalRight(void); 
void DispMenuCAN1DevicesTotalEscUp(void);
void DispMenuCAN1DevicesTotalEscDown(void);
void DispMenuCAN1DevicesTotalEscOk(void);

//------------------------------------------------------------------		
//CAN2设备数设置功能函数
//------------------------------------------------------------------
void DispMenuCAN2DevicesTotalInit(void);
void DispMenuCAN2DevicesTotalUp(void);
void DispMenuCAN2DevicesTotalDown(void);
void DispMenuCAN2DevicesTotalLeft(void);
void DispMenuCAN2DevicesTotalRight(void); 
void DispMenuCAN2DevicesTotalEscUp(void);
void DispMenuCAN2DevicesTotalEscDown(void);
void DispMenuCAN2DevicesTotalEscOk(void);

//------------------------------------------------------------------		
//游戏选择功能函数
//------------------------------------------------------------------
void DispMenuGameSelectInit(void);
void DispMenuGameSelectUp(void);
void DispMenuGameSelectDown(void);
void DispMenuGameSelectLeft(void);
void DispMenuGameSelectRight(void); 
void DispMenuGameSelectEscUp(void);
void DispMenuGameSelectEscDown(void);
void DispMenuGameSelectEscOk(void);

//------------------------------------------------------------------		
//游清除游戏排名数据功能函数
//------------------------------------------------------------------
void DispMenuCleanRankingDataInit(void);
void DispMenuCleanRankingDataUp(void);
void DispMenuCleanRankingDataDown(void);
void DispMenuCleanRankingDataLeft(void);
void DispMenuCleanRankingDataRight(void); 
void DispMenuCleanRankingDataEscUp(void);
void DispMenuCleanRankingDataEscDown(void);
void DispMenuCleanRankingDataEscOk(void);




//------------------------------------------------------------------		
//设置提示音排号功能函数
//------------------------------------------------------------------
void DispMenuSetBeepRowNumInit(void);
void DispMenuSetBeepRowNumUp(void);
void DispMenuSetBeepRowNumDown(void);
void DispMenuSetBeepRowNumLeft(void);
void DispMenuSetBeepRowNumRight(void);
void DispMenuSetBeepRowNumEscUp(void);
void DispMenuSetBeepRowNumEscDown(void);
void DispMenuSetBeepRowNumEscOk(void);


//------------------------------------------------------------------		
//设置游戏通关时间功能函数
//------------------------------------------------------------------
void DispMenuSetGamePassTimeInit(void);
void DispMenuSetGamePassTimeUp(void);
void DispMenuSetGamePassTimeDown(void);
void DispMenuSetGamePassTimeLeft(void);
void DispMenuSetGamePassTimeRight(void);
void DispMenuSetGamePassTimeEscUp(void);
void DispMenuSetGamePassTimeEscDown(void);
void DispMenuSetGamePassTimeEscOk(void);


//------------------------------------------------------------------		
//设置语音提示时间功能函数
//------------------------------------------------------------------
void DispMenuVoicePromptTimeInit(void);
void DispMenuVoicePromptTimeUp(void);
void DispMenuVoicePromptTimeDown(void);
void DispMenuVoicePromptTimeLeft(void);
void DispMenuVoicePromptTimeRight(void) ;
void DispMenuVoicePromptTimeEscUp(void);
void DispMenuVoicePromptTimeEscDown(void);
void DispMenuVoicePromptTimeEscOk(void);



//------------------------------------------------------------------		
//儿童模式设置功能函数
//------------------------------------------------------------------
void DispMenuBabyModeSetInit(void);
void DispMenuBabyModeSetUp(void);
void DispMenuBabyModeSetDown(void);
void DispMenuBabyModeSetLeft(void);
void DispMenuBabyModeSetRight(void);
void DispMenuBabyModeSetEscUp(void);
void DispMenuBabyModeSetEscDown(void);
void DispMenuBabyModeSetEscOk(void);

//------------------------------------------------------------------		
//发音模式选择功能函数
//------------------------------------------------------------------
void DispMenuSayModeSelectInit(void);
void DispMenuSayModeSelectUp(void);
void DispMenuSayModeSelectDown(void);
void DispMenuSayModeSelectLeft(void);
void DispMenuSayModeSelectRight(void);
void DispMenuSayModeSelectEscUp(void);
void DispMenuSayModeSelectEscDown(void);
void DispMenuSayModeSelectEscOk(void);

//------------------------------------------------------------------		
//发音模式设置功能函数
//------------------------------------------------------------------
void DispMenuSayModeSetInit(void);
void DispMenuSayModeSetUp(void);
void DispMenuSayModeSetDown(void);
void DispMenuSayModeSetLeft(void);
void DispMenuSayModeSetRight(void);
void DispMenuSayModeSetEscUp(void);
void DispMenuSayModeSetEscDown(void);
void DispMenuSayModeSetEscOk(void);

//------------------------------------------------------------------		
//英文字母设置功能函数
//------------------------------------------------------------------
void DispMenuEnglishCharSetInit(void);
void DispMenuEnglishCharSetUp(void);
void DispMenuEnglishCharSetDown(void);
void DispMenuEnglishCharSetLeft(void);
void DispMenuEnglishCharSetRight(void);
void DispMenuEnglishCharSetEscUp(void);
void DispMenuEnglishCharSetEscDown(void);
void DispMenuEnglishCharSetEscOk(void);

//------------------------------------------------------------------		
//汉语拼音设置功能函数
//------------------------------------------------------------------
void DispMenuChinesePinyinSetInit(void);
void DispMenuChinesePinyinSetUp(void);
void DispMenuChinesePinyinSetDown(void);
void DispMenuChinesePinyinSetLeft(void);
void DispMenuChinesePinyinSetRight(void);
void DispMenuChinesePinyinSetEscUp(void);
void DispMenuChinesePinyinSetEscDown(void);
void DispMenuChinesePinyinSetEscOk(void);

//------------------------------------------------------------------		
//设置返回主界面时间功能函数
//------------------------------------------------------------------
void DispMenuSetReturnTopTimeInit(void);
void DispMenuSetReturnTopTimeUp(void);
void DispMenuSetReturnTopTimeDown(void);
void DispMenuSetReturnTopTimeLeft(void);
void DispMenuSetReturnTopTimeRight(void) ;
void DispMenuSetReturnTopTimeEscUp(void);
void DispMenuSetReturnTopTimeEscDown(void);
void DispMenuSetReturnTopTimeEscOk(void);






//------------------------------------------------------------------		
//{"全彩灯设置"}子菜单功能函数
//------------------------------------------------------------------
void DispMenuFullColorLightSetInit(void);
void DispMenuFullColorLightSetUp(void);
void DispMenuFullColorLightSetDown(void);
void DispMenuFullColorLightSetLeft(void);
void DispMenuFullColorLightSetRight(void);
void DispMenuFullColorLightSetEscUp(void);
void DispMenuFullColorLightSetEscDown(void);
void DispMenuFullColorLightSetEscOk(void);




//------------------------------------------------------------------		
//{"配置单个设备"}子菜单功能函数
//------------------------------------------------------------------
void DispMenuConfig_S_DevInit(void);
void DispMenuConfig_S_DevUp(void);
void DispMenuConfig_S_DevDown(void);
void DispMenuConfig_S_DevLeft(void);
void DispMenuConfig_S_DevRight(void);
void DispMenuConfig_S_DevEscUp(void);
void DispMenuConfig_S_DevEscDown(void);
void DispMenuConfig_S_DevEscOk(void);

//------------------------------------------------------------------		
//{"配置多个设备"}子菜单功能函数
//------------------------------------------------------------------
void DispMenuConfig_M_DevInit(void);
void DispMenuConfig_M_DevUp(void);
void DispMenuConfig_M_DevDown(void);
void DispMenuConfig_M_DevLeft(void);
void DispMenuConfig_M_DevRight(void);
void DispMenuConfig_M_DevEscUp(void);
void DispMenuConfig_M_DevEscDown(void);
void DispMenuConfig_M_DevEscOk(void);


//------------------------------------------------------------------		
//{"芯片类型"}菜单功能函数
//------------------------------------------------------------------
void DispMenuChipTypeSetInit(void);
void DispMenuChipTypeSetUp(void);
void DispMenuChipTypeSetDown(void);
void DispMenuChipTypeSetLeft(void);
void DispMenuChipTypeSetRight(void);
void DispMenuChipTypeSetEscUp(void);
void DispMenuChipTypeSetEscDown(void);
void DispMenuChipTypeSetEscOk(void);

//------------------------------------------------------------------		
//{"帧数设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuNumberOfFramesSetInit(void);
void DispMenuNumberOfFramesSetUp(void);
void DispMenuNumberOfFramesSetDown(void);
void DispMenuNumberOfFramesSetLeft(void);
void DispMenuNumberOfFramesSetRight(void);
void DispMenuNumberOfFramesSetEscUp(void);
void DispMenuNumberOfFramesSetEscDown(void);
void DispMenuNumberOfFramesSetEscOk(void);

//------------------------------------------------------------------		
//{"全彩灯花样设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuFullColorLightPatternSetInit(void);
void DispMenuFullColorLightPatternSetUp(void);
void DispMenuFullColorLightPatternSetDown(void);
void DispMenuFullColorLightPatternSetLeft(void);
void DispMenuFullColorLightPatternSetRight(void);
void DispMenuFullColorLightPatternSetEscUp(void);
void DispMenuFullColorLightPatternSetEscDown(void);
void DispMenuFullColorLightPatternSetEscOk(void);


//------------------------------------------------------------------		
//{"最大灯珠数"}菜单功能函数
//------------------------------------------------------------------
void DispMenuMaxLedNumberSetInit(void);
void DispMenuMaxLedNumberSetUp(void);
void DispMenuMaxLedNumberSetDown(void);
void DispMenuMaxLedNumberSetLeft(void);
void DispMenuMaxLedNumberSetRight(void);
void DispMenuMaxLedNumberSetEscUp(void);
void DispMenuMaxLedNumberSetEscDown(void);
void DispMenuMaxLedNumberSetEscOk(void);


//------------------------------------------------------------------		
//{"每组灯珠数"}菜单功能函数
//------------------------------------------------------------------
void DispMenuGroupsLedNumberSetInit(void);
void DispMenuGroupsLedNumberSetUp(void);
void DispMenuGroupsLedNumberSetDown(void);
void DispMenuGroupsLedNumberSetLeft(void);
void DispMenuGroupsLedNumberSetRight(void);
void DispMenuGroupsLedNumberSetEscUp(void);
void DispMenuGroupsLedNumberSetEscDown(void);
void DispMenuGroupsLedNumberSetEscOk(void);


//------------------------------------------------------------------		
//{"灯珠颜色"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLedColourSetInit(void);
void DispMenuLedColourSetUp(void);
void DispMenuLedColourSetDown(void);
void DispMenuLedColourSetLeft(void);
void DispMenuLedColourSetRight(void);
void DispMenuLedColourSetEscUp(void);
void DispMenuLedColourSetEscDown(void);
void DispMenuLedColourSetEscOk(void);

//------------------------------------------------------------------		
//{"控制器地址设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuControlAddrSetInit(void);
void DispMenuControlAddrSetUp(void);
void DispMenuControlAddrSetDown(void);
void DispMenuControlAddrSetLeft(void);
void DispMenuControlAddrSetRight(void);
void DispMenuControlAddrSetEscUp(void);
void DispMenuControlAddrSetEscDown(void);
void DispMenuControlAddrSetEscOk(void);

//------------------------------------------------------------------		
//{"全彩灯颜色顺序"}菜单功能函数
//------------------------------------------------------------------
void DispMenuColorOrderSetInit(void);
void DispMenuColorOrderSetUp(void);
void DispMenuColorOrderSetDown(void);
void DispMenuColorOrderSetLeft(void);
void DispMenuColorOrderSetRight(void);
void DispMenuColorOrderSetEscUp(void);
void DispMenuColorOrderSetEscDown(void);
void DispMenuColorOrderSetEscOk(void);


//------------------------------------------------------------------		
//{"全彩灯颜色循环时间设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLedColorCycleTimeSetInit(void);
void DispMenuLedColorCycleTimeSetUp(void);
void DispMenuLedColorCycleTimeSetDown(void);
void DispMenuLedColorCycleTimeSetLeft(void);
void DispMenuLedColorCycleTimeSetRight(void);
void DispMenuLedColorCycleTimeSetEscUp(void);
void DispMenuLedColorCycleTimeSetEscDown(void);
void DispMenuLedColorCycleTimeSetEscOk(void);

//------------------------------------------------------------------		
//{"灯带控制设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLedControlSetInit(void);
void DispMenuLedControlSetUp(void);
void DispMenuLedControlSetDown(void);
void DispMenuLedControlSetLeft(void);
void DispMenuLedControlSetRight(void);
void DispMenuLedControlSetEscUp(void);
void DispMenuLedControlSetEscDown(void);
void DispMenuLedControlSetEscOk(void);


//------------------------------------------------------------------		
//{"本机灯带开关"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLocal_OnOffInit(void);
void DispMenuLocal_OnOffUp(void);
void DispMenuLocal_OnOffDown(void);
void DispMenuLocal_OnOffLeft(void);
void DispMenuLocal_OnOffRight(void);
void DispMenuLocal_OnOffEscUp(void);
void DispMenuLocal_OnOffEscDown(void);
void DispMenuLocal_OnOffEscOk(void);

//------------------------------------------------------------------		
//{"1000T灯带开关"}菜单功能函数
//------------------------------------------------------------------
void DispMenuT1000_OnOffInit(void);
void DispMenuT1000_OnOffUp(void);
void DispMenuT1000_OnOffDown(void);
void DispMenuT1000_OnOffLeft(void);
void DispMenuT1000_OnOffRight(void);
void DispMenuT1000_OnOffEscUp(void);
void DispMenuT1000_OnOffEscDown(void);
void DispMenuT1000_OnOffEscOk(void);

//------------------------------------------------------------------		
//{"本机灯带定时"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLocal_TimerInit(void);
void DispMenuLocal_TimerUp(void);
void DispMenuLocal_TimerDown(void);
void DispMenuLocal_TimerLeft(void);
void DispMenuLocal_TimerRight(void);
void DispMenuLocal_TimerEscUp(void);
void DispMenuLocal_TimerEscDown(void);
void DispMenuLocal_TimerEscOk(void);


//------------------------------------------------------------------		
//{"T1000灯带定时"}菜单功能函数
//------------------------------------------------------------------
void DispMenuT1000_TimerInit(void);
void DispMenuT1000_TimerUp(void);
void DispMenuT1000_TimerDown(void);
void DispMenuT1000_TimerLeft(void);
void DispMenuT1000_TimerRight(void);
void DispMenuT1000_TimerEscUp(void);
void DispMenuT1000_TimerEscDown(void);
void DispMenuT1000_TimerEscOk(void);
		

//------------------------------------------------------------------		
//{"配置多个设备失败"}菜单功能函数
//------------------------------------------------------------------
void DispMenuConfig_M_DevFailInit(void);
void DispMenuConfig_M_DevFailUp(void);
void DispMenuConfig_M_DevFailDown(void);
void DispMenuConfig_M_DevFailLeft(void);
void DispMenuConfig_M_DevFailRight(void);
void DispMenuConfig_M_DevFailEscUp(void);
void DispMenuConfig_M_DevFailEscDown(void);
void DispMenuConfig_M_DevFailEscOk(void);


//------------------------------------------------------------------		
//{"全彩灯颜色模式设置"}菜单功能函数
//------------------------------------------------------------------
void DispMenuLedColorCycleModeSetInit(void);
void DispMenuLedColorCycleModeSetUp(void);
void DispMenuLedColorCycleModeSetDown(void);
void DispMenuLedColorCycleModeSetLeft(void);
void DispMenuLedColorCycleModeSetRight(void);
void DispMenuLedColorCycleModeSetEscUp(void);
void DispMenuLedColorCycleModeSetEscDown(void);
void DispMenuLedColorCycleModeSetEscOk(void);




extern u16 TopMenuResetTimeCnt;

void (*KeyFuncPtr)(); //按键功能指针


typedef struct{
        U16 KeyStateIndex; //当前状态索引号
        U16 KeyUpState; //按下"向上"键时转向的状态索引号
        U16 KeyDnState; //按下"向下"键时转向的状态索引号
        U16 KeyLState;  //按下"向左"键时转向的状态索引号
        U16 KeyRState;  //按下"向右"键时转向的状态索引号
	
		U16 KeyEscUpState; 	//按下"向下"键时转向的状态索引号
        U16 KeyEscDownState; //按下"向左"键时转向的状态索引号
        U16 KeyEscOkState;  	//按下"向右"键时转向的状态索引号

        void (*CurrentOperate)(); //当前状态应该执行的功能操作
} KbdTabStruct;


//如下是菜单的关键代码
//不清楚具体含义 不能改动
//因为跟编译器相关
//如下的函数的名称也不可以轻易修改
/*
********************************************************************************
   FSM菜单实现机制说明:
A.一些约定:只有4个按键来做菜单周转,这四个按键Up/Down/Left/Down 对应功能是:确认/回退/向上/向下
B.每一层的bar型菜单需要大概5个状态(也就是5个函数)来实现,分别是MenuInit/MenuUp/MenuDown/MenuLeft/MenuRight
对应的功能解释:
MenuInit : 初始化菜单/重新初始化菜单,在进入一个子Bar菜单时,该函数把Bar显示指针根据语言指向子Bar的资源数组,
获得需要的参数:该子菜单多少个Bar项(ItemNum)/每个Bar项的字符长度(Size)/指向Bar显示内容的指针(DispItem).
而后,调用函数BarMenuInit,该函数初始化全部Bar显示--这就是初始化菜单.
若是是从子菜单返回时,那么在返回前,Pop函数被调用,各种菜单参数被还原,在进入本函数后,本函数根据得到参数显示Bar项
初始化菜单/重新初始化菜单的不同点在于:
初始化时      高亮显示的位置是在顶部,对应选中的Item也是在顶部
重新初始化时  由于用户通过按键向上/向下来选择Bar项,而后通过按键确认来进入子菜单,为了在退出子菜单时能得到跟进入
前一致的显示内容,因此需要备份一些相关数据:高亮的位置/选中的Item/当前FSM的Index号  在退出子菜单时Pop还原这些数据
本函数根据这些还原数据恢复显示.

MenuUp   : 根据Bar显示内容执行对应动作函数/或者周转到某层子菜单
MenuDown : 返回上层菜单 通常是简单调用Pop函数
MenuLeft : 移至上一个Bar项 通常是调用BarMenuLeft函数--该函数维护Bar的显示后FSM状态返回MenunInit,但由于没有打开Flash/Reflash项,因此没有执行MenuInit函数
MenuRight: 移至下一个Bar项 通常是调用BarMenuright函数--该函数维护Bar的显示后FSM状态返回MenunInit,但由于没有打开Flash/Reflash项,因此没有执行MenuInit函数


********************************************************************************
*/
_CONST_ KbdTabStruct KeyTab[]={
//       |-----------> Index
//       |  Up
//       |  |  Down
//       |  |  |  Left
//       |  |  |  |  Right   
//       |  |  |  |  |  EscUp
//	     |  |  |  |  |  |  EscDown
//	     |  |  |  |  |	|  |  EscOk
//       |  |  |  |  |  |  |  |      |--->功能函数
       { 0, 1, 2, 3, 4, 5, 6, 7,(*DispMenuTop)},// 待机画面
       { 1, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopUp)},
       { 2, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopDown)},
       { 3, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopLeft)},
       { 4, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopRight)},
	   { 5, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscUp)},
       { 6, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscDown)},
       { 7, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscOk)},


       { 8,  9,10,11,12,13,14,15,(*DispIcoMenuInit)}, //图标层菜单
       { 9,  0, 0, 0, 0, 0, 0, 0,(*DispIcoMenuUp)},   //
       { 10, 0, 0, 0, 0, 0, 0, 0,(*DispIcoMenuDown)}, //
       { 11, 0, 0, 0, 0, 0, 0, 0,(*DispIcoMenuLeft)}, //
       { 12, 0, 0, 0, 0, 0, 0, 0,(*DispIcoMenuRight)},//
	   { 13, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscUp)},
       { 14, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscDown)},
       { 15, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscOk)},
	   

       {16,17,18,19,20,21,22,23,(*DispMenuFuncInit)}, //"功能"
       {17, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFuncUp)},   //
       {18, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFuncDown)}, //
       {19, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFuncLeft)}, //
       {20, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFuncRight)},//
	   {21, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscUp)},
       {22, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscDown)},
       {23, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscOk)},
	   

       {24,25,26,27,28,29,30,31,(*DispMenuParaInit)}, //"参数"
       {25, 0, 0, 0, 0, 0, 0, 0,(*DispMenuParaUp)},   //
       {26, 0, 0, 0, 0, 0, 0, 0,(*DispMenuParaDown)}, //
       {27, 0, 0, 0, 0, 0, 0, 0,(*DispMenuParaLeft)}, //
       {28, 0, 0, 0, 0, 0, 0, 0,(*DispMenuParaRight)},//
	   {29, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscUp)},
       {30, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscDown)},
       {31, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscOk)},
	   
	   
       {32,33,34,35,36,37,38,39,(*DispMenuLanguageInit)}, //"语言"
       {33, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLanguageUp)},   //
       {34, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLanguageDown)}, //
       {35, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLanguageLeft)}, //
       {36, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLanguageRight)},//
	   {37, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscUp)},
       {38, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscDown)},
       {39, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTopEscOk)},
	   
	   
		
       {40,41,42,43,44,45,46,47,(*DispMenuMeasureInit)}, //"测量"  时钟
       {41, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureUp)},   //
       {42, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureDown)}, //
       {43, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureLeft)}, //
       {44, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureRight)},// 
	   {45, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureDown)}, //
       {46, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureLeft)}, //
       {47, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMeasureRight)},// 
		
	   

       {48,49,50,51,52,53,54,55,(*DispMenuSetDeviceAddressInit)}, //
       {49, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressUp)},   //
       {50, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressDown)}, //
       {51, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressLeft)}, //
       {52, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressRight)},//  
	   {53, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressEscUp)},
       {54, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressEscDown)},
       {55, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceAddressEscOk)},
	   
	   
       {56,57,58,59,60,61,62,63,(*DispMenuSetDeviceTotalInit)}, //
       {57, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalUp)},   //
       {58, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalDown)}, //
       {59, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalLeft)}, //
       {60, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalRight)},//  
	   {61, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalEscUp)},
       {62, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalEscDown)},
       {63, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetDeviceTotalEscOk)},
	   
	   
	     
       {64,65,66,67,68,69,70,71,(*DispMenuMusicStatusSetInit)}, //
       {65, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetUp)},   //
       {66, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetDown)}, //
       {67, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetLeft)}, //
       {68, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetRight)},//  
	   {69, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetEscUp)},
       {70, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetEscDown)},
       {71, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMusicStatusSetEscOk)},
	   
	   
       {72,73,74,75,76,77,78,79,(*DispMenuLightsStatusSetInit)}, //
       {73, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetUp)},   //
       {74, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetDown)}, //
       {75, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetLeft)}, //
       {76, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetRight)},//  
	   {77, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetEscUp)},
       {78, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetEscDown)},
       {79, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsStatusSetEscOk)},
	   
	   
	   {80,81,82,83,84,85,86,87,(*DispMenuFactorySetInit)}, //
       {81, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetUp)},   //
       {82, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetDown)}, //
       {83, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetLeft)}, //
       {84, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetRight)},//  
	   {85, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetEscUp)},
       {86, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetEscDown)},
       {87, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFactorySetEscOk)},
	   

       
        //------------------------------------------------------------------		
        //设置音乐延时时间功能函数
        //------------------------------------------------------------------          
        {88,89,90,91,92,93,94,95,(*DispMenuSetMusicDelayTimeInit)},
        {89, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeUp)},
        {90, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeDown)},
        {91, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeLeft)},
        {92, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeRight)}, 
        {93, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeEscUp)},
        {94, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeEscDown)},
        {95, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetMusicDelayTimeEscOk)},

        //------------------------------------------------------------------		
        //设置灯光延时时间功能函数
        //------------------------------------------------------------------
        {96 ,97,98,99,100,101,102,103,(*DispMenuSetLightsDelayTimeInit)},
        {97 , 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeUp)},
        {98 , 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeDown)},
        {99 , 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeLeft)},
        {100, 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeRight)}, 
        {101, 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeEscUp)},
        {102, 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeEscDown)},
        {103, 0, 0, 0,  0,  0,  0,  0,(*DispMenuSetLightsDelayTimeEscOk)},
	
		
		
		//------------------------------------------------------------------		
        //设置踩踏计数时间功能函数
        //------------------------------------------------------------------
        {104,105,106,107,108,109,110,111,(*DispMenuSetTrampleCountTimeInit)},
        {105, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeUp)},
        {106, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeDown)},
        {107, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeLeft)},
        {108, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeRight)}, 
        {109, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeEscUp)},
        {110, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeEscDown)},
        {111, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleCountTimeEscOk)},

		
        //------------------------------------------------------------------		
        //设置踩踏延时时间功能函数
        //------------------------------------------------------------------
        {112,113,114,115,116,117,118,119,(*DispMenuSetTrampleDelayTimeInit)},
        {113, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeUp)},
        {114, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeDown)},
        {115, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeLeft)},
        {116, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeRight)}, 
        {117, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeEscUp)},
        {118, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeEscDown)},
        {119, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleDelayTimeEscOk)},

        //------------------------------------------------------------------		
        //设置同时踩踏人数功能函数
        //------------------------------------------------------------------
        {120,121,122,123,124,125,126,127,(*DispMenuSetTrampleNumberInit)},
        {121, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberUp)},
        {122, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberDown)},
        {123, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberLeft)},
        {124, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberRight)},
        {125, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberEscUp)},
        {126, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberEscDown)},
        {127, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetTrampleNumberEscOk)},

        //------------------------------------------------------------------		
        //设置音节延时时间功能函数
        //------------------------------------------------------------------
        {128,129,130,131,132,133,134,135,(*DispMenuSetSyllableDelayTimeInit)},
        {129, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeUp)},
        {130, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeDown)},
        {131, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeLeft)},
        {132, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeRight)}, 
        {133, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeEscUp)},
        {134, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeEscDown)},
        {135, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetSyllableDelayTimeEscOk)},

	   
		 //------------------------------------------------------------------		
        //设置音符延时时间功能函数
        //------------------------------------------------------------------
        {136,137,138,139,140,141,142,143,(*DispMenuSetNoteDelayTimeInit)},
        {137, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeUp)},
        {138, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeDown)},
        {139, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeLeft)},
        {140, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeRight)}, 
        {141, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeEscUp)},
        {142, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeEscDown)},
        {143, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetNoteDelayTimeEscOk)},
		
		
		
		
		 //------------------------------------------------------------------		
        //灯带模式设置功能函数
        //------------------------------------------------------------------
        {144,145,146,147,148,149,150,151,(*DispMenuLightsModeSetInit)},
        {145, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetUp)},
        {146, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetDown)},
        {147, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetLeft)},
        {148, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetRight)}, 
        {149, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetEscUp)},
        {150, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetEscDown)},
        {151, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsModeSetEscOk)},
		
		
		
		 //------------------------------------------------------------------		
        //灯带速度设置功能函数
        //------------------------------------------------------------------
        {152,153,154,155,156,157,158,159,(*DispMenuLightsSpeedSetInit)},
        {153, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetUp)},
        {154, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetDown)},
        {155, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetLeft)},
        {156, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetRight)}, 
        {157, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetEscUp)},
        {158, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetEscDown)},
        {159, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpeedSetEscOk)},
		
		
		
		//------------------------------------------------------------------		
        //灯带间隔设置功能函数
        //------------------------------------------------------------------
        {160,161,162,163,164,165,166,167,(*DispMenuLightsSpacingSetInit)},
        {161, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetUp)},
        {162, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetDown)},
        {163, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetLeft)},
        {164, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetRight)}, 
        {165, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetEscUp)},
        {166, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetEscDown)},
        {167, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLightsSpacingSetEscOk)},
		

		
		//------------------------------------------------------------------		
        //游戏模式选择功能函数
        //------------------------------------------------------------------
        {168,169,170,171,172,173,174,175,(*DispMenuGameModeSelectInit)},
        {169, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectUp)},
        {170, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectDown)},
        {171, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectLeft)},
        {172, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectRight)}, 
        {173, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectEscUp)},
        {174, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectEscDown)},
        {175, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSelectEscOk)},
		
		
		
		//------------------------------------------------------------------		
		//游戏模式设置功能函数
		//------------------------------------------------------------------
		{176,177,178,179,180,181,182,183,(*DispMenuGameModeSetInit)},
		{177, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetUp)},
		{178, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetDown)},
		{179, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetLeft)},
		{180, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetRight)}, 
		{181, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetEscUp)},
		{182, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetEscDown)},
		{183, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameModeSetEscOk)},


		//------------------------------------------------------------------		
		//儿童模式设置功能函数
		//------------------------------------------------------------------
		{184,185,186,187,188,189,190,191,(*DispMenuBabyModeSetInit)},
		{185, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetUp)},
		{186, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetDown)},
		{187, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetLeft)},
		{188, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetRight)}, 
		{189, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetEscUp)},
		{190, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetEscDown)},
		{191, 0, 0, 0, 0, 0, 0, 0,(*DispMenuBabyModeSetEscOk)},


	
		//------------------------------------------------------------------		
		//排数设置功能函数
		//------------------------------------------------------------------
		{192,193,194,195,196,197,198,199,(*DispMenuRowNumberSetInit)},
		{193, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetUp)},
		{194, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetDown)},
		{195, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetLeft)},
		{196, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetRight)}, 
		{197, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetEscUp)},
		{198, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetEscDown)},
		{199, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowNumberSetEscOk)},

		
		//------------------------------------------------------------------		
		//各排设备数设置功能函数
		//------------------------------------------------------------------
		{200,201,202,203,204,205,206,207,(*DispMenuRowsDeviceNumberSetInit)},
		{201, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetUp)},
		{202, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetDown)},
		{203, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetLeft)},
		{204, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetRight)}, 
		{205, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetEscUp)},
		{206, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetEscDown)},
		{207, 0, 0, 0, 0, 0, 0, 0,(*DispMenuRowsDeviceNumberSetEscOk)},

		//------------------------------------------------------------------		
		//时钟设置设置功能函数
		//------------------------------------------------------------------
		{208,209,210,211,212,213,214,215,(*DispMenuClockSetInit)},
		{209, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetUp)},
		{210, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetDown)},
		{211, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetLeft)},
		{212, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetRight)}, 
		{213, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetEscUp)},
		{214, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetEscDown)},
		{215, 0, 0, 0, 0, 0, 0, 0,(*DispMenuClockSetEscOk)},

		//------------------------------------------------------------------		
		//时间设置功能函数
		//------------------------------------------------------------------
		{216,217,218,219,220,221,222,223,(*DispMenuTimeSetInit)},
		{217, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetUp)},
		{218, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetDown)},
		{219, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetLeft)},
		{220, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetRight)}, 
		{221, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetEscUp)},
		{222, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetEscDown)},
		{223, 0, 0, 0, 0, 0, 0, 0,(*DispMenuTimeSetEscOk)},

		//------------------------------------------------------------------		
		//日期设置功能函数
		//------------------------------------------------------------------
		{224,225,226,227,228,229,230,231,(*DispMenuDateSetInit)},
		{225, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetUp)},
		{226, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetDown)},
		{227, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetLeft)},
		{228, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetRight)}, 
		{229, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetEscUp)},
		{230, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetEscDown)},
		{231, 0, 0, 0, 0, 0, 0, 0,(*DispMenuDateSetEscOk)},


		
		//------------------------------------------------------------------		
		//关于功能函数
		//------------------------------------------------------------------
		{232,233,234,235,236,237,238,239,(*DispMenuAboutInit)},
		{233, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutUp)},
		{234, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutDown)},
		{235, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutLeft)},
		{236, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutRight)}, 
		{237, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutEscUp)},
		{238, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutEscDown)},
		{239, 0, 0, 0, 0, 0, 0, 0,(*DispMenuAboutEscOk)},
		
		
		//------------------------------------------------------------------		
		//CAN1设备数设置功能函数
		//------------------------------------------------------------------
		{240,241,242,243,244,245,246,247,(*DispMenuCAN1DevicesTotalInit)},
		{241, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalUp)},
		{242, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalDown)},
		{243, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalLeft)},
		{244, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalRight)}, 
		{245, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalEscUp)},
		{246, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalEscDown)},
		{247, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN1DevicesTotalEscOk)},
		
		//------------------------------------------------------------------		
		//CAN2设备数设置功能函数
		//------------------------------------------------------------------
		{248,249,250,251,252,253,254,255,(*DispMenuCAN2DevicesTotalInit)},
		{249, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalUp)},
		{250, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalDown)},
		{251, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalLeft)},
		{252, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalRight)}, 
		{253, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalEscUp)},
		{254, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalEscDown)},
		{255, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCAN2DevicesTotalEscOk)},
		
		//------------------------------------------------------------------		
		//游戏选择功能函数
		//------------------------------------------------------------------
		{256,257,258,259,260,261,262,263,(*DispMenuGameSelectInit)},
		{257, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectUp)},
		{258, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectDown)},
		{259, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectLeft)},
		{260, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectRight)}, 
		{261, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectEscUp)},
		{262, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectEscDown)},
		{263, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGameSelectEscOk)},
		
		
		
		//------------------------------------------------------------------		
		//游清除游戏排名数据功能函数
		//------------------------------------------------------------------
		{264,265,266,267,268,269,270,271,(*DispMenuCleanRankingDataInit)},
		{265, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataUp)},
		{266, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataDown)},
		{267, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataLeft)},
		{268, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataRight)}, 
		{269, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataEscUp)},
		{270, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataEscDown)},
		{271, 0, 0, 0, 0, 0, 0, 0,(*DispMenuCleanRankingDataEscOk)},
		
		
		//------------------------------------------------------------------		
		//设置提示音排号功能函数
		//------------------------------------------------------------------
		{272,273,274,275,276,277,278,279,(*DispMenuSetBeepRowNumInit)},
		{273, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumUp)},
		{274, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumDown)},
		{275, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumLeft)},
		{276, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumRight)}, 
		{277, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumEscUp)},
		{278, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumEscDown)},
		{279, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetBeepRowNumEscOk)},
		
		
		//------------------------------------------------------------------		
		//设置游戏通关时间功能函数
		//------------------------------------------------------------------
		{280,281,282,283,284,285,286,287,(*DispMenuSetGamePassTimeInit)},
		{281, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeUp)},
		{282, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeDown)},
		{283, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeLeft)},
		{284, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeRight)}, 
		{285, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeEscUp)},
		{286, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeEscDown)},
		{287, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetGamePassTimeEscOk)},
		
		
		//------------------------------------------------------------------		
		//设置语音提示时间功能函数
		//------------------------------------------------------------------
		{288,289,290,291,292,293,294,295,(*DispMenuVoicePromptTimeInit)},
		{289, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeUp)},
		{290, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeDown)},
		{291, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeLeft)},
		{292, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeRight)}, 
		{293, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeEscUp)},
		{294, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeEscDown)},
		{295, 0, 0, 0, 0, 0, 0, 0,(*DispMenuVoicePromptTimeEscOk)},
		
		
		//------------------------------------------------------------------		
		//发音模式选择功能函数
		//------------------------------------------------------------------
		{296,297,298,299,300,301,302,303,(*DispMenuSayModeSelectInit)},
		{297, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectUp)},
		{298, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectDown)},
		{299, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectLeft)},
		{300, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectRight)}, 
		{301, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectEscUp)},
		{302, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectEscDown)},
		{303, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSelectEscOk)},
		
		//------------------------------------------------------------------		
		//发音模式设置功能函数
		//------------------------------------------------------------------
		{304,305,306,307,308,309,310,311,(*DispMenuSayModeSetInit)},
		{305, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetUp)},
		{306, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetDown)},
		{307, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetLeft)},
		{308, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetRight)}, 
		{309, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetEscUp)},
		{310, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetEscDown)},
		{311, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSayModeSetEscOk)},
		
		
		//------------------------------------------------------------------		
		//英文字母设置功能函数
		//------------------------------------------------------------------
		{312,313,314,315,316,317,318,319,(*DispMenuEnglishCharSetInit)},
		{313, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetUp)},
		{314, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetDown)},
		{315, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetLeft)},
		{316, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetRight)}, 
		{317, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetEscUp)},
		{318, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetEscDown)},
		{319, 0, 0, 0, 0, 0, 0, 0,(*DispMenuEnglishCharSetEscOk)},
		
	
		//------------------------------------------------------------------		
		//汉语拼音设置功能函数
		//------------------------------------------------------------------
		{320,321,322,323,324,325,326,327,(*DispMenuChinesePinyinSetInit)},
		{321, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetUp)},
		{322, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetDown)},
		{323, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetLeft)},
		{324, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetRight)}, 
		{325, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetEscUp)},
		{326, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetEscDown)},
		{327, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChinesePinyinSetEscOk)},
				
		//------------------------------------------------------------------		
		//设置返回主界面时间功能函数
		//------------------------------------------------------------------
		{328,329,330,331,332,333,334,335,(*DispMenuSetReturnTopTimeInit)},
		{329, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeUp)},
		{330, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeDown)},
		{331, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeLeft)},
		{332, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeRight)},
		{333, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeEscUp)},
		{334, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeEscDown)},
		{335, 0, 0, 0, 0, 0, 0, 0,(*DispMenuSetReturnTopTimeEscOk)},
		


		//------------------------------------------------------------------		
		//{"全彩灯设置"}子菜单功能函数
		//------------------------------------------------------------------
		{336,337,338,339,340,341,342,343,(*DispMenuFullColorLightSetInit)},
		{337, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetUp)},
		{338, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetDown)},
		{339, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetLeft)},
		{340, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetRight)},
		{341, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetEscUp)},
		{342, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetEscDown)},
		{343, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightSetEscOk)},
		
		//------------------------------------------------------------------		
		//{"芯片类型"}菜单功能函数
		//------------------------------------------------------------------
		{344,345,346,347,348,349,350,351,(*DispMenuChipTypeSetInit)},
		{345, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetUp)},
		{346, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetDown)},
		{347, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetLeft)},
		{348, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetRight)},
		{349, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetEscUp)},
		{350, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetEscDown)},
		{351, 0, 0, 0, 0, 0, 0, 0,(*DispMenuChipTypeSetEscOk)},
		
		//------------------------------------------------------------------		
		//{"帧数设置"}菜单功能函数
		//------------------------------------------------------------------
		{352,353,354,355,356,357,358,359,(*DispMenuNumberOfFramesSetInit)},
		{353, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetUp)},
		{354, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetDown)},
		{355, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetLeft)},
		{356, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetRight)},
		{357, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetEscUp)},
		{358, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetEscDown)},
		{359, 0, 0, 0, 0, 0, 0, 0,(*DispMenuNumberOfFramesSetEscOk)},
		
		//------------------------------------------------------------------		
		//{"全彩灯花样设置"}菜单功能函数
		//------------------------------------------------------------------
		{360,361,362,363,364,365,366,367,(*DispMenuFullColorLightPatternSetInit)},
		{361, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetUp)},
		{362, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetDown)},
		{363, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetLeft)},
		{364, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetRight)},
		{365, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetEscUp)},
		{366, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetEscDown)},
		{367, 0, 0, 0, 0, 0, 0, 0,(*DispMenuFullColorLightPatternSetEscOk)},
		
		
		//------------------------------------------------------------------		
		//{"最大灯珠数"}菜单功能函数
		//------------------------------------------------------------------
		{368,369,370,371,372,373,374,375,(*DispMenuMaxLedNumberSetInit)},
		{369, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetUp)},
		{370, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetDown)},
		{371, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetLeft)},
		{372, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetRight)},
		{373, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetEscUp)},
		{374, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetEscDown)},
		{375, 0, 0, 0, 0, 0, 0, 0,(*DispMenuMaxLedNumberSetEscOk)},
		
		
		//------------------------------------------------------------------		
		//{"每组灯珠数"}菜单功能函数
		//------------------------------------------------------------------
		{376,377,378,379,380,381,382,383,(*DispMenuGroupsLedNumberSetInit)},
		{377, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetUp)},
		{378, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetDown)},
		{379, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetLeft)},
		{380, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetRight)},
		{381, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetEscUp)},
		{382, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetEscDown)},
		{383, 0, 0, 0, 0, 0, 0, 0,(*DispMenuGroupsLedNumberSetEscOk)},
		
		
		//------------------------------------------------------------------		
		//{"灯珠颜色"}菜单功能函数
		//------------------------------------------------------------------
		{384,385,386,387,388,389,390,391,(*DispMenuLedColourSetInit)},
		{385, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetUp)},
		{386, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetDown)},
		{387, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetLeft)},
		{388, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetRight)},
		{389, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetEscUp)},
		{390, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetEscDown)},
		{391, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColourSetEscOk)},
		
		//------------------------------------------------------------------		
		//{"控制器地址设置"}菜单功能函数
		//------------------------------------------------------------------
		{392,393,394,395,396,397,398,399,(*DispMenuControlAddrSetInit)},
		{393, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetUp)},
		{394, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetDown)},
		{395, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetLeft)},
		{396, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetRight)},
		{397, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetEscUp)},
		{398, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetEscDown)},
		{399, 0, 0, 0, 0, 0, 0, 0,(*DispMenuControlAddrSetEscOk)},

		
		
		//------------------------------------------------------------------		
		//{"全彩灯颜色顺序"}菜单功能函数
		//------------------------------------------------------------------
		{400,401,402,403,404,405,406,407,(*DispMenuColorOrderSetInit)},
		{401, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetUp)},
		{402, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetDown)},
		{403, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetLeft)},
		{404, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetRight)},
		{405, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetEscUp)},
		{406, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetEscDown)},
		{407, 0, 0, 0, 0, 0, 0, 0,(*DispMenuColorOrderSetEscOk)},
		
		
		
		
		//------------------------------------------------------------------		
		//{"全彩灯颜色循环时间设置"}菜单功能函数
		//------------------------------------------------------------------
		{408,409,410,411,412,413,414,415,(*DispMenuLedColorCycleTimeSetInit)},
		{409, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetUp)},
		{410, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetDown)},
		{411, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetLeft)},
		{412, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetRight)},
		{413, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetEscUp)},
		{414, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetEscDown)},
		{415, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleTimeSetEscOk)},
		

		//------------------------------------------------------------------		
		//{"灯带控制设置"}菜单功能函数
		//------------------------------------------------------------------
		{416,417,418,419,420,421,422,423,(*DispMenuLedControlSetInit)},
		{417, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetUp)},
		{418, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetDown)},
		{419, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetLeft)},
		{420, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetRight)},
		{421, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetEscUp)},
		{422, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetEscDown)},
		{423, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedControlSetEscOk)},
		

		//------------------------------------------------------------------		
		//{"本机灯带开关"}菜单功能函数
		//------------------------------------------------------------------
		{424,425,426,427,428,429,430,431,(*DispMenuLocal_OnOffInit)},
		{425, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffUp)},
		{426, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffDown)},
		{427, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffLeft)},
		{428, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffRight)},
		{429, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffEscUp)},
		{430, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffEscDown)},
		{431, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_OnOffEscOk)},

		//------------------------------------------------------------------		
		//{"1000T灯带开关"}菜单功能函数
		//------------------------------------------------------------------
		{432,433,434,435,436,437,438,439,(*DispMenuT1000_OnOffInit)},
		{433, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffUp)},
		{434, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffDown)},
		{435, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffLeft)},
		{436, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffRight)},
		{437, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffEscUp)},
		{438, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffEscDown)},
		{439, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_OnOffEscOk)},
		
		//------------------------------------------------------------------		
		//{"本机灯带定时"}菜单功能函数
		//------------------------------------------------------------------
		{440,441,442,443,444,445,446,447,(*DispMenuLocal_TimerInit)},
		{441, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerUp)},
		{442, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerDown)},
		{443, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerLeft)},
		{444, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerRight)},
		{445, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerEscUp)},
		{446, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerEscDown)},
		{447, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLocal_TimerEscOk)},
		
		
		//------------------------------------------------------------------		
		//{"T1000灯带定时"}菜单功能函数
		//------------------------------------------------------------------
		{448,449,450,451,452,453,454,455,(*DispMenuT1000_TimerInit)},
		{449, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerUp)},
		{450, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerDown)},
		{451, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerLeft)},
		{452, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerRight)},
		{453, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerEscUp)},
		{454, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerEscDown)},
		{455, 0, 0, 0, 0, 0, 0, 0,(*DispMenuT1000_TimerEscOk)},
		


		//------------------------------------------------------------------		
		//{"配置单个设备"}菜单功能函数
		//------------------------------------------------------------------
		{456,457,458,459,460,461,462,463,(*DispMenuConfig_S_DevInit)},
		{457, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevUp)},
		{458, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevDown)},
		{459, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevLeft)},
		{460, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevRight)},
		{461, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevEscUp)},
		{462, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevEscDown)},
		{463, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_S_DevEscOk)},
		
		//------------------------------------------------------------------		
		//{"配置多个设备"}菜单功能函数
		//------------------------------------------------------------------
		{464,465,466,467,468,469,470,471,(*DispMenuConfig_M_DevInit)},
		{465,465,466,467,468,469,470,471,(*DispMenuConfig_M_DevUp)},
		{466, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevDown)},
		{467, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevLeft)},
		{468, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevRight)},
		{469, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevEscUp)},
		{470, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevEscDown)},
		{471, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevEscOk)},
		

		
		//------------------------------------------------------------------		
		//{"配置多个设备失败"}菜单功能函数
		//------------------------------------------------------------------
		{472,473,474,475,476,477,478,479,(*DispMenuConfig_M_DevFailInit)},
		{473, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailUp)},
		{474, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailDown)},
		{475, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailLeft)},
		{476, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailRight)},
		{477, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailEscUp)},
		{478, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailEscDown)},
		{479, 0, 0, 0, 0, 0, 0, 0,(*DispMenuConfig_M_DevFailEscOk)},


		//------------------------------------------------------------------		
		//{"全彩灯颜色模式设置"}菜单功能函数
		//------------------------------------------------------------------
		{480,481,482,483,484,485,486,487,(*DispMenuLedColorCycleModeSetInit)},
		{481, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetUp)},
		{482, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetDown)},
		{483, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetLeft)},
		{484, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetRight)},
		{485, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetEscUp)},
		{486, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetEscDown)},
		{487, 0, 0, 0, 0, 0, 0, 0,(*DispMenuLedColorCycleModeSetEscOk)},
		
		
		
        #ifdef DynamicBar
        {30,31,32,33,34,(*DispMenuDataInit)}, //"Data"
        {31, 0, 0, 0, 0,(*DispMenuDataUp)},   //
        {32, 0, 0, 0, 0,(*DispMenuDataDown)}, //
        {33, 0, 0, 0, 0,(*DispMenuDataLeft)}, //
        {34, 0, 0, 0, 0,(*DispMenuDataRight)},//
        #endif		     
};




/*
*****************************************************************************
* CheckKey - 菜单键盘扫描函数
* DESCRIPTION: -
* 有有效按键则执行对应的功能函数
* 同时,若刷新标志有效,也执行相应的功能函数
* @Para void:
* Return :
*
*****************************************************************************
*/
//U8 CheckKey(void)
U8 CheckKey(u8 KeyVal)	

{
    U8 IsKey;
    U8 Key;
    IsKey = 1;
    Key = key_scan();
	
	
	//GlobalKeyVal =KeyVal;
	
	
	//Key = KeyVal;
	
	if ((Key & (~(KEY_LONG_UP <<6))) != (NO_KEY & (~(KEY_LONG_UP <<6))))
	{
		TopMenuResetTimeCnt=0;
	}
	
	
	switch(Key & (~(KEY_LONG_UP <<6)))
    //switch(Key)
	{
        case Key_Up:{ //向上键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
		
		
		
        case Key_Down:{ //向下键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDnState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
		
		
        case Key_Left:{ //向左键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case Key_Right:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
		
		

		/*
		case KEY_UP:{ //向上键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case KEY_DOWN:{ //向下键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDnState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case KEY_ESC:{ //向左键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case KEY_OK:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
		*/
		
		
		case KEY_ESC_DOWN:{ //向下键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyEscDownState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case KEY_ESC_UP:{ //向左键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyEscUpState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }
        case KEY_ESC_OK:{ //向右键,找出新的菜单状态编号
            KeyFuncIndex=KeyTab[KeyFuncIndex].KeyEscOkState;
            KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
            (*KeyFuncPtr)();//执行当前按键的操作
            break;
        }

//---------------------------------------------------------------------------
#ifdef DigiKeyLink
/*
2006/09/08
数字键做快捷方式输入
1.判别数字键值是否小于ItemNum-1 否则是无效快捷方式
2.有效快捷方式下，把键值给Item，调用Key_Up的处理代码
3.需要添加超时处理，超时了就不能使用？（需要么？不需要么？）
*/
        case Key_1:
        case Key_2:
        case Key_3:
        case Key_4:
        case Key_5:
        case Key_6:
        case Key_7:
        case Key_8:
        case Key_9:
            Key = chang_code(Key);
            if((Key - '1') <= ItemNum-1){
                Item = Key - '1';
                if(ItemNum <= DispMax){//如果需要显示的项比可以显示的项少，那么修正Where 否则默认Where为0
                    Where = Item;
                }else{
                    Where = 0;
                }
                //-------------------------------------------------
                KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpState;
                KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
                (*KeyFuncPtr)();//执行当前按键的操作
                //-------------------------------------------------
            }else {
                IsKey = 0;
            }
            break;
#endif
//---------------------------------------------------------------------------
        default: //按键错误的处理
            IsKey = 0;
            break;
    }

    if(Flash){//两个当中有任意一个不为零，那么执行一次，我们应该不会出现Flash和ReFlash都是1的情况
		
        KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;
        (*KeyFuncPtr)();//执行当前按键的操作
		
		if (Flash==FlashMode_ManualReFlash)
		{
			Flash=FlashMode_AutoInit;
		}
		else
		{
			Flash = 0;
		}
       // ReFlash = 0;
     }

    return IsKey;
}

