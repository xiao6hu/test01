
#ifndef __MENU_H__
#define __MENU_H__

/******************************************************************************
 * Menu.H - 菜单相关宏开关 函数声明等
 *
 *
 * DESCRIPTION: -
 *
 * modification history
 * --------------------
 * 01a, 04/sep/2007, 梁炎昌 written
 * --------------------
 ******************************************************************************/

/*
LessCPUUseage:
高亮显示当前项的做法是
1.反显整个Bar项文字 提示当前项
2.通过一个'>'来提示当前位置
第1种方法在低速CPU和LCD上比较费时
因此这个情况下用第2种方法,毕竟反显一个'>'比反显整行要省事的多
但没有解决根本问题,如果依然会出现要全部刷新的情况
如果CPU慢#define LessCPUUseage
否则不定义该值
*/
//
//#define LessCPUUseage

//动态生成条项菜单  需要回滚方式的支持
//#define DynamicBar
//是否回滚方式
#define RockBack

//数字键快捷方式
//需要预定义数字键的键值Key_1---Key_9
//并提供一个函数能把键值转化成实际对应数字 Key = chang_code(Key);
//#define DigiKeyLink

//检查如果需要动态条项菜单,就需要回滚方式,这里做检查
#ifdef DynamicBar
    #ifndef RockBack
        #error  "DynamicBar need RockBack!"
        #error  "#define RockBack pls"
    #endif
#endif

//--------------
//显示位置的宏定义
//父Bar项的显示位置
#define FatherXOffset    10     //Father的X偏置
#define FatherYOffset    2      //Father的Y偏置
//Bar项的显示XY偏置
#define BarXOffset    0     //Bar的X偏置
#define BarYOffset    0     //Bar的Y偏置
//Bar项的XY尺寸
#define BarXSize      (7*12)    //Bar的长度
#define BarYSize      2         //Bar的间隔

//语言代码
#define CN    0
#define EN    1

//不初始化Flash
#define FlashMode_NoAction     0
//系统系统负责初始化
#define FlashMode_AutoInit     1
//手动初始化
#define FlashMode_ManualInit   2
//重绘画面
#define FlashMode_ReFlash      3

#define FlashMode_ManualReFlash      4



//也就是连续使用Push的个数
#define MenuSize 10          //菜单深度最大值

#ifdef DynamicBar
#define FixedData       0
#define DynamicData     1
//条项内容的来源是固定的还是动态生成的，通常是固定的，
#endif

//U8 DispMax = 3;     //单屏可以显示的最大项 通常这是个固定值--因为你的液晶的大小决定了你一次可以显示的多少,你也可以在程序中修改,但记得维护它--因为所有的Bar型菜单都是要用到它的
#define DispMax  3

//-----------------------------------------------------
//Bar型菜单的数据结构
struct Menu
{
    //U8 _CONST_ **Text;     //指向显示内容数组的指针
    U8 **Text;     //指向显示内容数组的指针
    U8 TextNum;         //项个数
    void* (*Action)();  //对应的执行函数
};

//----------------------------------------------------
//对外声明定义于Menu.C的内容
extern U8 ReFlash ;             //子项菜单返回 刷新标志
extern U8 Flash ;               //跳转进入子菜单 刷新标志
extern U8 language;           //语言标志

extern U16 KeyFuncIndex ;        //索引

extern U16 FatherIndex[MenuSize+1]; //父菜单备份数组
extern U8 Layer;               //菜单层数

extern U8 ItemBackup[MenuSize];      //当前菜单项备份
extern U8 ItemBackup_i;          //

extern U8 WhereBackup[MenuSize];     //反显位置备份
extern U8 WhereBackup_i;         //


extern U8 KeyPressCount;         //统计连续按键时间,长时间按键则 版本显示

#ifdef DynamicBar
extern U8 BarDataMode;
#endif
extern U8 Item;            //当前Bar项  范围:[0,ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值
extern U8 ItemNum;         //Bar项数
extern U8 Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置

extern U8 DispMin;         //ItemNum和DispMax中的小者

//当前Bar显示内容指针
//extern U8 _CONST_ **DispItem; //项显示内容指针
extern U8 **DispItem; //项显示内容指针

#ifdef DynamicBar
extern U8 ** DynamicBarDispItem; //动态项显示内容指针
#endif
//---------------------------------------------------
//返回当前菜单
#define SelfMenuIndex FatherIndex[Layer-1]

//----------------------------------------------------
//extern void Jump2Menu(U8 Index);//,U8 FlashMode);
extern void Jump2Menu(U16 Index,U8 FlashMode);


//-------------------------------
//资源条项，中英文显示
//-------------------------------
extern  _CONST_ struct Menu Menu_ResIndex[];
	
	
	
//-------------------------------
//输入超范围，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_InputOverrange[];	
	
	
//-------------------------------
//成功/失败，中英文显示
//-------------------------------
extern  _CONST_ struct Menu Menu_SuccessFailureIndex[];

//-------------------------------
//单位，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_Unit[];

enum{
    Unit_MS_Index,
    Unit_S_Index,
    Unit_PCS_Index,
};



//{"定时器设置"}
extern _CONST_ struct Menu Menu_TimerSet[];

//{"灯带控制设置"}
extern _CONST_ struct Menu Menu_LedControlSet[];


//{"踩下/抬起显示"}
extern _CONST_ struct Menu Menu_UpDown[];

//{"RGB颜色显示"}
extern _CONST_ struct Menu Menu_RGB_Color[];

//{"全彩灯设置"}子菜单
extern _CONST_ struct Menu Menu_FullColorLightSet[];


//{"全彩灯颜色顺序"}子菜单
extern _CONST_ struct Menu Menu_ColorOrder[];

//{"芯片类型"}子菜单
extern _CONST_ struct Menu Menu_ChipType[];

//{"全彩灯花样设置"}子菜单
extern _CONST_ struct Menu Menu_FullColorLightPatternSet[];



//儿童模式设置菜单
extern _CONST_ struct Menu Menu_BabyModeSet[];
//儿童模式设置菜单
extern _CONST_ struct Menu Menu_SayModeSelect[];

//发音模式设置菜单
extern _CONST_ struct Menu Menu_SayModeSet[];

//游戏选择菜单
extern _CONST_ struct Menu Menu_GameSelect[];

//时钟设置菜单
extern _CONST_ struct Menu Menu_ClockSet[];

//{"游戏模式设置"}子菜单
extern _CONST_ struct Menu Menu_GameModeSet[];

//-------------------------------
//游戏模式选择，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_GameModeSelect[];

//-------------------------------
//灯带模式，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_LightsMode[]; 
//-------------------------------
//灯带速度，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_LightsSpeed[];


//-------------------------------
//提示信息，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_PromptInfo[];

//-------------------------------
//欢迎使用！，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_Welcome[];

//-------------------------------
//版本号，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_Version[];

//-------------------------------
//开/关，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_OnOff[];
//-------------------------------
//重试/跳过，中英文显示
//-------------------------------
extern _CONST_ struct Menu Menu_RetrySkip[];

//图标层
extern _CONST_ struct Menu IcoMenu[];
//功能菜单
extern _CONST_ struct Menu MenuFunc[];
//参数菜单
extern _CONST_ struct Menu MenuPara[];
//测量菜单
extern _CONST_ struct Menu MenuMeasure[];

//语言菜单
extern _CONST_ struct Menu MenuLanguage[];

//Tip
extern _CONST_ struct Menu Tip[];
//图标
extern U8 _CONST_ * _CONST_ icos[];

//菜单资源指针
extern struct Menu * BarMenu;
//Tip指针
extern struct Menu * TipBar;

//--------------------------------------------------------------------
#define Enter_PosX   50      //"进入"显示的位置
#define Enter_PosY    6
#define ICO_PosX     48      //图标显示的位置
#define ICO_PosY     2
#define ICO_XSize    32      //图标的尺寸
#define ICO_YSize    (-2)

#define MenuBarPosX     125
#define MenuBarNumPosX  120



#define SetDeviceAddress_PosX  51
#define SetDeviceAddress_PosY  4

#define SetDevicesTotal_PosX   51-8
#define SetDevicesTotal_PosY   4

#define MusicDelayTime_PosX    51-8-8
#define MusicDelayTime_PosY    4


#define LightsDelayTime_PosX   51-8-8
#define LightsDelayTime_PosY   4

#define TrampleCountTime_PosX      51-20
#define TrampleCountTime_PosY      4

#define TrampleDelayTime_PosX      51-12
#define TrampleDelayTime_PosY      4

#define TrampleNumber_PosX     51-4
#define TrampleNumber_PosY     4

#define SyllableDelayTime_PosX     51-20
#define SyllableDelayTime_PosY     4

#define NoteDelayTime_PosX     51-20
#define NoteDelayTime_PosY     4


#define LightsSpacing_PosX     63-12

#define LightsSpacing_PosY    4



#define BabyModeSet_PosX 27
#define BabyModeSet_PosY 4

#define RowsDeviceNumberSet_PosX 19
#define RowsDeviceNumberSet_PosY 4

#define RowNumberSet_PosX 51
#define RowNumberSet_PosY 4

#define TimeSet_PosX 31
#define TimeSet_PosY 4
#define DateSet_PosX 23
#define DateSet_PosY 4

#define CAN1DevicesTotalSet_PosX 51
#define CAN1DevicesTotalSet_PosY 4


#define CAN2DevicesTotalSet_PosX 51
#define CAN2DevicesTotalSet_PosY 4

#define GameSelect_PosX 51
#define GameSelect_PosY 4





#define SetBeepRowNum_PosX 51
#define SetBeepRowNum_PosY 4

#define GamePassTime_PosX  51-4
#define GamePassTime_PosY  4


#define VoicePromptTime_PosX   51-8-8
#define VoicePromptTime_PosY   4


#define EnglishCharSet_PosX 27
#define EnglishCharSet_PosY 4


#define ChinesePinyinSet_PosX 27
#define ChinesePinyinSet_PosY 4

  
#define ReturnTopTime_PosX    39
#define ReturnTopTime_PosY    4



#define FullColorLightSet_PosX 			39
#define FullColorLightSetI_PosY 		4


#define ControlAddrSet_PosX				51
#define ControlAddrSet_PosY		  		4
		  
		  
#define ColorOrderSet_PosX				39
#define ColorOrderSet_PosY				4
		  
		  
#define ChipTypeSet_PosX 				39
#define ChipTypeSet_PosY 				4

#define NumberOfFramesSet_PosX			43
#define NumberOfFramesSet_PosY			4

#define FullColorLightPatternSet_PosX 	27
#define FullColorLightPatternSet_PosY 	4

#define MaxLedNumberSet_PosX			39
#define MaxLedNumberSet_PosY			4

#define GroupsLedNumberSet_PosX			43
#define GroupsLedNumberSet_PosY			4

#define LedColourSet_PosX				39
#define LedColourSet_PosY				4


#define LedColorCycleTimeSet_PosX       39
#define LedColorCycleTimeSet_PosY		4




#define LedControlSet_PosX            	39 
#define LedControlSet_PosY            	4

#define Local_OnOff_PosX 				39
#define Local_OnOff_PosY 				4

#define T1000_OnOff_PosX 				39
#define T1000_OnOff_PosY 				4

#define Local_Timer_PosX				39
#define Local_Timer_PosY				4

#define T1000_Timer_PosX 				39
#define T1000_Timer_PosY 				4

#define Config_S_Dev_PosX 				39
#define Config_S_Dev_PosY 				4

#define Config_M_Dev_PosX 				35
#define Config_M_Dev_PosY 				4
#define Config_M_DevFail_PosX			39
#define Config_M_DevFail_PosY			4

#define LedColorCycleModeSet_PosX       39
#define LedColorCycleModeSet_PosY       4


#define GameFailDelayTime_PosX  51-4
#define GameFailDelayTime_PosY  4

#define GameWaitDelayTime_PosX  51-4
#define GameWaitDelayTime_PosY  4


#define GameStartDeviceAddr_PosX  51
#define GamStartDeviceAddr_PosY  4

#define GameEndDeviceAddr_PosX  51
#define GameEndDeviceAddr_PosY  4








//-----------------------
//如下定义从KeyTab中得到
//调整KeyTab时必须更新如下的定义
//否则菜单链接出错!
#define MenuTopIndex       0
#define MenuIndex          8

#define MenuFuncIndex     16

#define MenuParaIndex     24

#define MenuLanguageIndex 32

#define MenuMeasureIndex  40


#define SetDeviceAddressIndex	48

#define SetDeviceTotalIndex  	56

#define SetMusicDelayTimeIndex  	88
#define SetLightsDelayTimeIndex  	96

#define SetTrampleCountTimeIndex  104
#define SetTrampleDelayTimeIndex  	112
#define SetTrampleNumberIndex  		120
#define SetSyllableDelayTimeIndex  	128

#define SetNoteDelayTimeIndex  	 136


#define MusicStatusSetIndex	  	64

#define LightsStatusSetIndex  	72


#define FactorySetIndex      80



#define MenuDataIndex     30



#define LightsModeSetIndex 144

#define LightsSpeedSetIndex 152


#define LightsSpacingSetIndex  160



#define GameModeSelectIndex   168
#define GameModeSetIndex   176
#define BabyModeSetIndex  184

#define RowNumberSetIndex 192
#define RowsDeviceNumberSetIndex 200


#define ClockSetIndex 208
#define TimeSetIndex 216
#define DateSetIndex 224


#define AboutIndex   232

#define CAN1DevicesTotalIndex   240
#define CAN2DevicesTotalIndex   248


#define GameSelectIndex   256

#define CleanRankingDataIndex  264


#define SetBeepRowNumIndex   	272
#define GamePassTimeIndex 		280
#define VoicePromptTimeIndex 	288


#define SayModeSelectIndex 		296
#define SayModeSetIndex			304	
#define EnglishCharSetIndex		312
#define ChinesePinyinSetIndex	320

#define ReturnTopTimeIndex      328





#define FullColorLightSetIndex 	336



#define Config_S_DevIndex 		456
#define Config_M_DevIndex		464
#define Config_M_DevFailIndex	472
#define Config_M_Dev_UpIndex    465


#define ControlAddrSetIndex    			392
#define ColorOrderSetIndex				400

#define ChipTypeSetIndex 				344
#define NumberOfFramesSetIndex			352
#define FullColorLightPatternSetIndex 	360
#define MaxLedNumberSetIndex			368
#define GroupsLedNumberSetIndex			376
#define LedColourSetIndex				384

#define LedColorCycleTimeSetIndex       408

#define LedControlSetIndex            	416 
#define Local_OnOffIndex 				424
#define T1000_OnOffIndex 				432
#define Local_TimerIndex 				440
#define T1000_TimerIndex 				448


#define LedColorCycleModeSetIndex       480

#define SetGameFailDelayTimeIndex       488   
#define SetGameWaitDelayTimeIndex       496
#define SetGameStartDeviceAddrIndex     504
#define SetGameEndtDeviceAddrIndex      512


















void BarMenuInit(void);
void BarMenuLeft(void);
void BarMenuRight(void);
void PUSH(void);
void POP(void);

void DispMenuTop(void);


//U8 CheckKey(void);
U8 CheckKey(u8 KeyVal);	

void MenuMainLoop(void);

void LodeParametersData(void);

#endif
