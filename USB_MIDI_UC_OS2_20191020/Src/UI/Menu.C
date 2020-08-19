//#include "CPU.H"

#include "sys.h"
#include "Menu.H"

#include "PIC.C"//图标

/*
----------------------------------------------------------------------------
菜单相关变量
----------------------------------------------------------------------------
*/

U8 Flash = 0;               //跳转进入子菜单 刷新标志  >1刷新
U8 language = CN;           //语言标志
U16 KeyFuncIndex = 0;        //索引
//3个备份数组

U16 FatherIndex[MenuSize+1]; //父菜单备份数组

U8 Layer = 0;               //菜单层数

U8 ItemBackup[MenuSize];      //当前菜单项备份
U8 ItemBackup_i = 0;          //

U8 WhereBackup[MenuSize];     //反显位置备份
U8 WhereBackup_i = 0;         //


U8 KeyPressCount = 0;               //用于版本显示

#ifdef DynamicBar
U8 BarDataMode = FixedData;
#endif


U8 Item;            //当前Bar项  范围:[0,ItemNum-1]  包括0 包括ItemNum-1  共包括ItemNum个取值
U8 ItemNum;         //Bar项数
U8 Where;           //当前反显位置,[0,DispMax-1]  反映反显的Bar在屏幕上的位置
//U8 DispMax = 3;     //单屏可以显示的最大项 通常这是个固定值--因为你的液晶的大小决定了你一次可以显示的多少,你也可以在程序中修改,但记得维护它--因为所有的Bar型菜单都是要用到它的
//#define DispMax  10

U8 DispMin;         //ItemNum和DispMax中的小者
//当前Bar显示内容指针
//U8 _CONST_ **DispItem; //项显示内容指针
U8 **DispItem; //项显示内容指针

#ifdef DynamicBar
U8 ** DynamicBarDispItem; //动态项显示内容指针
#endif

//菜单资源指针
struct Menu * BarMenu;
//Tip指针
struct Menu * TipBar;


/*
----------------------------------------------------------------------------
      |---->指针对象存放地址,也就是{"Function"},{"Parameter"},{"Measure"},等等这些文本存放在哪里
      |         |---->指针存放的位置,是这些字符的首地址应当存放在哪里  因为这是个指针数组存放的内容就是每个字符串的开始地址
      |         |
U8 _CONST_ * _CONST_ TopItemEN[]=
{
{"Function"},
{"Parameter"},
{"Measure"},
{"Language"},
{"Test"},
};
我们均把这些存放在代码区,以减少对RAM的占用

----------------------------------------------------------------------------
*/


//---------------------------------------------------
//各个菜单的资源
//命名方式是:
//---------------------------------------------------
//顶层菜单


//U8 _CONST_ * _CONST_ TopItemCN[]=
U8 * _CONST_ TopItemCN[]=
{
{"功能"},
{"参数"},
//{"测量"},
{"时钟"},
{"语言"},
{"测试"},
};


//U8 _CONST_ * _CONST_ TopItemEN[]=
U8 * _CONST_ TopItemEN[]=
{
{"Function"},
{"Parameter"},
//{"Measure"},
{"Clock"},
{"Language"},
{"Test"},
};


//"功能"菜单项的内容和长度

//U8 _CONST_ * _CONST_ FuncItemCN[]=

U8 * _CONST_ FuncItemCN[]=
{
	/*
{"读出数据"},
{"背光控制"},
{"出厂设置"},
{"修改密码"},
*/
	
{"音乐状态设置"},
{"灯光状态设置"},
{"游戏模式选择"},
{"游戏选择"},
{"清除排名数据"},
{"出厂设置"},
{"关于"},
};

//U8 _CONST_ * _CONST_ FuncItemEN[]=
U8 * _CONST_ FuncItemEN[]=
{
/*	
{"Read Data"},
{"BackLight"},
{"Factory Set"},
{"Change CODE"}
*/	
{"MusicStatusSet"},
{"LightsStatusSet"},
{"GameModeSelect"},
{"GameSelect"},
{"CleanRankingData"},
{"Factory Set"},
{"About"},
};


//"参数"菜单项的内容和长度

//U8 _CONST_ * _CONST_ ParaItemCN[]=
U8 * _CONST_ ParaItemCN[]=
{
/*
{"阈值"},
{"死时间T"},
{"灵敏度K"},
{"测量时间"}
*/
	
{"设置设备地址"},
{"设置设备总数"},
{"音乐延时时间"},
{"灯光延时时间"},
{"踩踏计数时间"},
{"踩踏延时时间"},
{"同时踩踏人数"},
{"音节延时时间"},
{"音符延时时间"},

//{"灯光模式设置"},
{"灯光速度设置"},
{"灯光间隔设置"},

{"游戏模式设置"},
{"儿童模式设置"},


{"CAN1设备数设置"},
{"CAN2设备数设置"},

{"返回主界面时间"},


{"全彩灯设置"},

/*{"配置单个设备"},*/
{"配置多个设备"},




};

//U8 _CONST_ * _CONST_ ParaItemEN[]=
U8 * _CONST_ ParaItemEN[]=
{
	/*
{"Menu0"},
{"Menu1"},
{"Menu2"},
{"Menu3"},
{"Menu4"},
{"Menu5"},
{"Menu6"},
{"Menu7"},
*/

{"SetDeviceAddress"},
{"SetDevicesTotal "},
{"Music DT"},
{"Lights DT"},
{"Trample CT"},
{"Trample DT"},
{"Trample NT"},
{"Syllable DT"},
{"Note DT"},

//{"LightsMode"},
{"LightsSpeed"},

{"LightsSpacing"},


{"GameModeSet"},
{"BabyModeSet"},

{"CAN1DevicesTotal"},
{"CAN2DevicesTotal"},

{"ReturnTopTime"},

{"FullColorLightSet"},

/*{"Config_S_Dev"},*/
{"Config_M_Dev"},





};

//{"全彩灯设置"}子菜单
U8 * _CONST_ FullColorLightSetCN[]=
{
	/*{"控制器地址设置"},*/
	{"全彩灯颜色顺序"},
	{"芯片类型"},
	{"帧数设置"},
	{"全彩灯花样设置"},
	{"最大灯珠数"},
	{"每组灯珠数"},
	/*{"灯珠颜色"},*/
	{"颜色转换时间"},
	{"灯带控制设置"},
	{"全彩灯颜色模式"},
};

//{"全彩灯设置"}子菜单
U8 * _CONST_ FullColorLightSetEN[]=
{
	
	/*{"ControlAddrSet"},*/
	{"ColorOrder"},
	{"ChipType"},
	{"Number of frames"},
	{"PatternSet"},
	{"MaxLedNumber"},
	{"GroupsLedNumber"},
	/*{"LedColour"},*/
	{"ColourChangeTime"},
	{"LedControlSet"},
	{"LedColorCycleMode"},

};

//{"全彩灯颜色顺序"}子菜单
U8 * _CONST_ ColorOrderCN[]=
{
	{"红-绿-蓝"},
	{"红-蓝-绿"},
	{"绿-红-蓝"},
	{"绿-蓝-红"},
	{"蓝-红-绿"},
	{"蓝-绿-红"},
};


//{"全彩灯颜色顺序"}子菜单
U8 * _CONST_ ColorOrderEN[]=
{
	{"R-G-B"},
	{"R-B-G"},
	{"G-R-B"},
	{"G-B-R"},
	{"B-R-G"},
	{"B-G-R"},
};


//{"芯片类型"}子菜单
U8 * _CONST_ ChipTypeCN[]=
{
	
	{"WS2811高速"},
	{"WS2811低速"},
};


//{"芯片类型"}子菜单
U8 * _CONST_ ChipTypeEN[]=
{
	
	{"WS2811_HS"},
	{"WS2811_LS"},
};




//{"全彩灯花样设置"}子菜单
U8 * _CONST_ FullColorLightPatternSetCN[]=
{
	{"花样:"},
	/*
	{"花样01"},
	{"花样02"},
	{"花样03"},
	{"花样04"},
	{"花样05"},
	{"花样06"},
	{"花样07"},
	{"花样08"},
	{"花样09"},
	{"花样10"},	
	*/
};
//{"全彩灯花样设置"}子菜单
U8 * _CONST_ FullColorLightPatternSetEN[]=
{
	{"Pattern:"},
	/*
	{"Pattern01"},
	{"Pattern02"},
	{"Pattern03"},
	{"Pattern04"},
	{"Pattern05"},
	{"Pattern06"},
	{"Pattern07"},
	{"Pattern08"},
	{"Pattern09"},
	{"Pattern10"},	
	*/
};






//{"RGB颜色显示"}
U8 * _CONST_ RGB_ColorCN[]=
{
	{"红:"},
	{"绿:"},	
	{"蓝:"},
};

U8 * _CONST_ RGB_ColorEN[]=
{
	{"R:"},
	{"G:"},	
	{"B:"},
};


//{"踩下/抬起显示"}
U8 * _CONST_ Menu_UpDownCN[]=
{
	{"踩下:"},
	{"抬起:"},
	{"循环:"},
};

U8 * _CONST_ Menu_UpDownEN[]=
{
	{"DOWN:"},
	{"UP:"},	
	{"CYCLE:"},
};




//{"灯带控制设置"}
U8 * _CONST_ Menu_LedControlSetCN[]=
{
	//{"本机灯带开关"},
	{"1000T灯带开关"},
	//{"本机灯带定时"},
	{"1000T灯带定时"}
};

U8 * _CONST_ Menu_LedControlSetEN[]=
{
	//{"Local_OnOff:"},
	{"T1000_OnOff"},	
	//{"Local_Timer"},
	{"1000T_Timer"},
};


//{"定时器设置"}子菜单
U8 * _CONST_ Menu_TimerSetCN[]=
{
	{"定时器"},
	{"时间"},
	{"定时"},
};

U8 * _CONST_ Menu_TimerSetEN[]=
{
	{"Timer:"},
	{"Time"},	
	{"Timing"},
};





//{"儿童模式设置"}子菜单
U8 * _CONST_ BabyModeSetCN[]=
{
	{"发音模式选择"},
	{"发音模式设置"},
};

U8 * _CONST_ BabyModeSetEN[]=
{
	{"SayModeSelect"},
	{"SayModeSet"},
};


//{"发音模式选择"}子菜单
U8 * _CONST_ SayModeSelectCN[]=
{
	{"英文字母"},
	{"汉语拼音"},
};

U8 * _CONST_ SayModeSelectEN[]=
{
	{"EnglishChar"},
	{"ChinesePinyin"},
};


//{"发音模式设置"}子菜单
U8 * _CONST_ SayModeSetCN[]=
{
	{"英文字母设置"},
	{"汉语拼音设置"},
};

U8 * _CONST_ SayModeSetEN[]=
{
	{"EnglishCharSet"},
	{"ChinesePinyinSet"},
};


//{"游戏选择"},子菜单
U8 * _CONST_ GameSelectCN[]=
{
	{"跳房子"},
	{"游戏2"},
	{"游戏3"},
	{"游戏4"},
	{"游戏5"},
};

U8 * _CONST_ GameSelectEN[]=
{
	{"Jumping house"},
	{"Game2"},
	{"Game3"},
	{"Game4"},
	{"Game5"},
};


//{"游戏模式设置"}子菜单
U8 * _CONST_ GameModeSetCN[]=
{
	{"设置总排数"},
	{"设置各排设备数"},
	{"设置提示音排号"},
	{"游戏通关时间"},
	{"语音提示时间"},
};

U8 * _CONST_ GameModeSetEN[]=
{
	{"SetRowNum"},
	{"SetRowsDeviceNum"},
	{"SetBeepRowNum"},
	{"GamePassTime"},
	{"VoicePromptTime"},
};


//"语言"菜单项的内容和长度

//U8 _CONST_ * _CONST_ LanguageItemCN[]=
U8 * _CONST_ LanguageItemCN[]=
{
{"中文"},
{"English"},
};

//U8 _CONST_ * _CONST_ LanguageItemEN[]=
U8 * _CONST_ LanguageItemEN[]=
{
{"中文"},
{"English"},
};

//测量

//U8 _CONST_ * _CONST_ MeasureItemCN[]=
//U8 * _CONST_ MeasureItemCN[]=
U8 * _CONST_ MeasureItemCN[]=
{
	
{"检测上拉电阻"},
{"清空出错记录"},
{"在线命令"},
{"输入口测试"},
{"模拟主轴测试"},
{"轴信号测试"},
{"继电器测试"},

};

//U8 _CONST_ * _CONST_ MeasureItemEN[]=
U8 * _CONST_ MeasureItemEN[]=
{
{"PushUp Res"},
{"ClearErrCount"},
{"OnlineCMD "},
{"SysInputTest"},
{"SVC TEST"},
{"AXis TEST"},
{"Relay TEST"},
};
//提示键的文本

//U8 _CONST_ * _CONST_ TipCN[]=
U8 * _CONST_ TipCN[]=
{
{"菜单"},
{"选择"},
{"确认"},
{"取消"},
{"?确认?"},
};


//U8 _CONST_ * _CONST_ TipEN[]=
U8 * _CONST_ TipEN[]=
{
{"Menu"},
{"Select"},
{"Ok"},
{"Cancel"},
{"? OK ?"},
};


U8 * _CONST_ WelcomeCN[]=
{
	{"欢迎使用!"},
};

U8 * _CONST_ WelcomeEN[]=
{
	{"Welcome!"},
};






U8 * _CONST_ VersionCN[]=
{
	{"版本:0.0.1"},
};

U8 * _CONST_ VersionEN[]=
{
	{"Version:0.0.1"},
};


U8 * _CONST_ OnOffCN[]=
{	
	{"关"},
	{"开"},	
};

U8 * _CONST_ OnOffEN[]=
{
	{"Off"},
	{"On"},
};

U8 * _CONST_ RetrySkipCN[]=
{	
	{"重试"},
	{"跳过"},	
};

U8 * _CONST_ RetrySkipEN[]=
{
	{"Retry"},
	{"Skip"},
};



U8 * _CONST_ InputOverrangeCN[]=
{
	{"输入超范围!"},
};


U8 * _CONST_ InputOverrangeEN[]=
{
	{"InputOverrange!"},
};


U8 * _CONST_ SuccessFailureCN[]=
{
	{"成功!"},
	{"失败!"},
};


U8 * _CONST_ SuccessFailureEN[]=
{
	{"Success!"},
	{"Failure!"},
};





U8 * _CONST_ ResIndexCN[]=
{
	{"设备地址"},
	{"设备总数"},
	{"音乐状态"},
	{"灯光状态"},	
	{"灯光状态"},
	
	

    
};

U8 * _CONST_ ResIndexEN[]=
{
	{"Device Address"},
	{"Devices Total"},
	{"MusicStatus"},
	{"LightsStatus"},
	
	
};


U8 * _CONST_ UnitCN[]=
{
    {"毫秒"},
    {"秒"},
    {"个"},
    
};

U8 * _CONST_ UnitEN[]=
{
    {"MS"},
    {"S"},
    {"PCS"},
};



U8 * _CONST_ PromptInfoCN[]=
{
    {"超出设置范围!"},
};

U8 * _CONST_ PromptInfoEN[]=
{
    {"Out of range!"},
};





U8 * _CONST_ LightsModeCN[]=
{
	
/*	
{"灯带少"},
{"灯带多"},
*/
{"灯模式1"},
{"灯模式2"},
{"灯模式3"},
};

U8 * _CONST_ LightsModeEN[]=
{
/*
{"LightsLess"},
{"LightsMany"},
*/
{"LightsMode1"},
{"LightsMode2"},
{"LightsMode3"},
};


U8 * _CONST_ LightsSpeedCN[]=
{
{"速度1"},
{"速度2"},
//{"速度3"},
};

U8 * _CONST_ LightsSpeedEN[]=
{
{"Speed1"},
{"Speed2"},
//{"Speed3"},
};


U8 * _CONST_ GameModeSelectCN[]=
{
{"成人模式"},
{"儿童模式"},
};

U8 * _CONST_ GameModeSelectEN[]=
{
{"AdultMode"},
{"BabyMode"},
};

//时钟设置
U8 * _CONST_ ClockSetCN[]=
{
{"时间设置"},
{"日期设置"},
};

U8 * _CONST_ ClockSetEN[]=
{
{"TimeSetting"},
{"DateSetting"},
};



//----------------------------------------
//宏定义:

#define MenuWhat(n) {(U8 **)n,(sizeof(n)/sizeof(U8 *)),0}







//{"定时器设置"}
_CONST_ struct Menu Menu_TimerSet[]=
{
	MenuWhat(Menu_TimerSetCN),
	MenuWhat(Menu_TimerSetEN),
};


//{"灯带控制设置"}
_CONST_ struct Menu Menu_LedControlSet[]=
{
	MenuWhat(Menu_LedControlSetCN),
	MenuWhat(Menu_LedControlSetEN),
};

//{"踩下/抬起显示"}
_CONST_ struct Menu Menu_UpDown[]=
{
	MenuWhat(Menu_UpDownCN),
	MenuWhat(Menu_UpDownEN),
};



//{"RGB颜色显示"}
_CONST_ struct Menu Menu_RGB_Color[]=
{
	MenuWhat(RGB_ColorCN),
	MenuWhat(RGB_ColorEN),
};


//{"全彩灯设置"}子菜单
_CONST_ struct Menu Menu_FullColorLightSet[]=
{
	MenuWhat(FullColorLightSetCN),
	MenuWhat(FullColorLightSetEN),
};



//{"全彩灯颜色顺序"}子菜单
_CONST_ struct Menu Menu_ColorOrder[]=
{
	MenuWhat(ColorOrderCN),
	MenuWhat(ColorOrderEN),
};



//{"芯片类型"}子菜单
_CONST_ struct Menu Menu_ChipType[]=
{
	MenuWhat(ChipTypeCN),
	MenuWhat(ChipTypeEN),
};


//{"全彩灯花样设置"}子菜单
_CONST_ struct Menu Menu_FullColorLightPatternSet[]=
{
	MenuWhat(FullColorLightPatternSetCN),
	MenuWhat(FullColorLightPatternSetEN),
};


//儿童模式设置菜单
_CONST_ struct Menu Menu_BabyModeSet[]=
{
	MenuWhat(BabyModeSetCN),
	MenuWhat(BabyModeSetEN),
};

//儿童模式设置菜单
_CONST_ struct Menu Menu_SayModeSelect[]=
{
	MenuWhat(SayModeSelectCN),
	MenuWhat(SayModeSelectEN),
};


//发音模式设置菜单
_CONST_ struct Menu Menu_SayModeSet[]=
{
	MenuWhat(SayModeSetCN),
	MenuWhat(SayModeSetEN),
};


//游戏选择菜单
_CONST_ struct Menu Menu_GameSelect[]=
{
	MenuWhat(GameSelectCN),
	MenuWhat(GameSelectEN),
};


//时钟设置菜单
_CONST_ struct Menu Menu_ClockSet[]=
{
	MenuWhat(ClockSetCN),
	MenuWhat(ClockSetEN),
};


//{"游戏模式设置"}子菜单
_CONST_ struct Menu Menu_GameModeSet[]=
{
	MenuWhat(GameModeSetCN),
	MenuWhat(GameModeSetEN),
};

//-------------------------------
//游戏模式选择，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_GameModeSelect[]=
{
	MenuWhat(GameModeSelectCN),
	MenuWhat(GameModeSelectEN),
};

//-------------------------------
//灯带模式，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_LightsMode[] =
{
	MenuWhat(LightsModeCN),
	MenuWhat(LightsModeEN),
};

//-------------------------------
//灯带速度，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_LightsSpeed[] =
{
	MenuWhat(LightsSpeedCN),
	MenuWhat(LightsSpeedEN),
};



//-------------------------------
//提示信息，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_PromptInfo[] =
{
	MenuWhat(PromptInfoCN),
	MenuWhat(PromptInfoEN),
};

//-------------------------------
//单位，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_Unit[] =
{
	MenuWhat(UnitCN),
	MenuWhat(UnitEN),
};




//-------------------------------
//输入超范围，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_InputOverrange[] =
{
	MenuWhat(InputOverrangeCN),
	MenuWhat(InputOverrangeEN),
};

//-------------------------------
//成功/失败，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_SuccessFailureIndex[] =
{
	MenuWhat(SuccessFailureCN),
	MenuWhat(SuccessFailureEN),
};


//-------------------------------
//资源条项，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_ResIndex[] =
{
	MenuWhat(ResIndexCN),
	MenuWhat(ResIndexEN),
};

//-------------------------------
//欢迎使用！，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_Welcome[] =
{
	MenuWhat(WelcomeCN),
	MenuWhat(WelcomeEN),
};


//-------------------------------
//开/关，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_OnOff[] =
{
	MenuWhat(OnOffCN),
	MenuWhat(OnOffEN),
};

//-------------------------------
//重试/跳过，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_RetrySkip[] =
{
	MenuWhat(RetrySkipCN),
	MenuWhat(RetrySkipEN),
};


//-------------------------------
//版本号，中英文显示
//-------------------------------
_CONST_ struct Menu Menu_Version[] =
{
	MenuWhat(VersionCN),
	MenuWhat(VersionEN),
};


//图标层
_CONST_ struct Menu IcoMenu[] =
{
MenuWhat(TopItemCN),
MenuWhat(TopItemEN),
};

//功能菜单
_CONST_ struct Menu MenuFunc[] =
{
MenuWhat(FuncItemCN),
MenuWhat(FuncItemEN),
};
//参数菜单
_CONST_ struct Menu MenuPara[] =
{
MenuWhat(ParaItemCN),
MenuWhat(ParaItemEN),
};
//测量菜单
_CONST_ struct Menu MenuMeasure[] =
{
MenuWhat(MeasureItemCN),
MenuWhat(MeasureItemEN),
};

//语言菜单
_CONST_ struct Menu MenuLanguage[] =
{
MenuWhat(LanguageItemCN),
MenuWhat(LanguageItemEN),
};

//Tip
_CONST_ struct Menu Tip[] =
{
MenuWhat(TipCN),
MenuWhat(TipEN),
};


//更改密码时询问的图标索引号
#define WarningICO  5
#define PassICO     0
//图标索引
U8 _CONST_ * _CONST_ icos[]=
{
 Function,   //功能
 Para,       //参数
 Time,       //时钟
 //Action,     //测量
	
 Menu,       //语言
 Question,   //询问
 Warning,
 Sound,
};


//跳转到指定菜单
/*
void Jump2Menu(U8 Index)
{
    KeyFuncIndex = Index;             //jump to Menu index
}
*/
void Jump2Menu(U16 Index,U8 FlashMode)
{
    KeyFuncIndex = Index;             //jump to Menu index
    if(FlashMode){
        Flash = FlashMode;
    }
}

