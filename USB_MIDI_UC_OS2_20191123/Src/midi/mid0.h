

#ifndef _MID_H_
#define _MID_H_



/*
MIDI文件是一种多媒体音乐文件。不同于别的音乐文件，MIDI文件只记录曲谱。播放的时候，软件会读取MIDI文件的内容，然后把曲谱发送给声卡，由声卡模拟发音。相对于留声机原理的音乐文件（MP3、OGG、AMR、WAV、FLAC、AAC等），这种记录曲谱的文件更小，更容易创作。缺点是因为声音是软件模拟发出的，音质不能得到保证，而且不同的声卡有不同的发音效果，因此MIDI文件的播放效果不能得到保证。不过为此，微软提供了软波表合成器，使用微软的软波表可以实现MIDI文件效果的统一。

著名的游戏《恶魔之星III》（俗称“雷电3”）的背景音乐就是用MIDI音乐播放的效果。微软DirectX的组件DirectMusic就能用于播放MIDI音乐。

MIDI文件大体上分两个部分：文件头和音轨。
文件头由14个字节组成。开头是文件标识“MThd”，之后是一个Big-Endian存储的DWORD，值通常为6。这个值的意义是“文件头的信息所占字节数”，也就是文件头接下来6个字节的字节数（就是6了嘛）。之后的6个字节，其实是3个Big-Endian存储的WORD，分别是类型字，音轨数，每四分音符的Tick数。类型字有三个值：0表示这个文件只有一个音轨，1表示这个文件有多个音轨，播放的时候要同步播放所有音轨，2表示这个文件虽然有多个音轨，但是播放的时候必须一个音轨一个音轨的播放。
用一个结构体来描述MIDI文件头的结构：
*/
/*
typedef struct
{
    DWORD   dwFlag;             //MThd标识
    DWORD   dwRestLen;          //剩余部分长度
    WORD    wType;              //类型
    WORD    wNbTracks;          //音轨数
    WORD    wTicksPerCrotchet;  //每四分音符的Tick数
}MIDIHeader;
*/
/*
以上就是文件头了。然后就是文件的重要部分：文件内容了。
文件内容由多个音轨组成，每个音轨的开头都是这样的结构：4字节的音轨标识"MTrk"，4字节以Big-Endian存储的音轨字节数。如下所示：
*/
/*
typedef struct
{
    DWORD   dwFlag;     //为0x6B72544D，即"MTrk"
    DWORD   dwTrackLen; //音轨长度（除去音轨头部以外的字节数）(Big-Endian)
}MIDITrack;
*/

/*
其中dwTrackLen的值，是整个音轨的字节数减去音轨头部的8个字节。
每个音轨，除去音轨头部以外，剩下的就是主要的文件内容了。在介绍文件内容以前，我觉得有必要科普一下MIDI文件所用的“动态字节”是怎么回事。
首先这个动态字节存在的意义是为了减少一个数字的存储空间。1个字节是8位，我们拿出其中的低7位存储数字，那么我们能存储的范围是0-127，如果我们要存储的数字在这个范围内的话，我们就把最高位设置为0。而如果我们要存储的数字超出了0-127这个范围，那么我们就把最高位设置为1，然后记录下高7位，剩下的留给下一个字节。假设我们要存储111这个数字，因为它的值在0-127范围内，我们可以只用一个字节存储：01101111b。而假设我要存储333这个数字，把它转换成二进制的时候是101001101b，超出了7位能存储的范围，那么我们先提取出它的高7位和低7位：0000010和1001101.然后我们用这样的两个字节存储：10000010b和01001101b.当我们读取动态字节的时候，我们先读取一个字节，记录它的低7位，然后判断它的最高位来判断是否需要继续读取下一个字节。
以下VB代码用于展示读取动态字节的方法。调用的时候，以文件号1打开文件，然后用Seek #1定位文件指针，最后调用它来读取动态字节。
'读取动态字节
*/
/*
Function ReadDynamicBytes(ValueOut As Long) As Long '返回读取的字节数
Dim OneByte As Byte
ValueOut = 0
For ReadDynamicBytes = 1 To 4 '最多读取4个字节
    Get #1, , OneByte '读取一个字节
    ValueOut = (ValueOut * &H80&) Or (OneByte And &H7F) '记录这个字节的低7位，同时左移以读取的数据让出位置。
    If (OneByte And &H80) = 0 Then Exit For '这个字节的最高位是0，没有后续字节，停止读取。
Next
End Function
*/

/*
理论上动态字节可用于存储大数，不过特别大的数在MIDI文件里用不着。我们顶多读取4字节（28位整数）

讲完了动态字节，接下来就应该讲文件内容了。文件的内容都是“事件”，这些“事件”是一个接一个存储的。一个事件都有固定的结构：延迟，事件号，事件参数。
其中“延迟”是动态字节，用于表示上一个事件到这一个事件之间的延迟量。这个延迟量的单位是Tick。一个Tick有多长时间取决于MIDI文件的曲速。
事件号的值在0x80到0xFF之间的时候表示的是具体的值，若读取到的这个值在0到0x7F之间，则表示这个事件的事件号和上一个事件相同，而读取到的值是它的参数。
有关事件号的资料在网络上查找也大同小异。但是都讲得不够清楚。我专门写了一个C语言程序用于解释事件号。看源码便知。
*/
//=============================================================================
//作者：0xAA55
//论坛：http://www.0xaa55.com/
//版权所有(C) 2013-2014 技术宅的结界
//请保留原作者信息，否则视为侵权。
//-----------------------------------------------------------------------------
#include<stdio.h>
//#include<malloc.h>
#include<stdlib.h>

#include "sys.h"

#include "ff.h"

//统一类型长度
typedef signed int      MIDIInt,*MIDIIntP;
typedef signed char     MIDIInt8,*MIDIInt8P;
typedef signed short    MIDIInt16,*MIDIInt16P;
typedef signed long     MIDIInt32,*MIDIInt32P;
typedef unsigned int    MIDIUInt,*MIDIUIntP;
typedef unsigned char   MIDIUInt8,*MIDIUInt8P;
typedef unsigned short  MIDIUInt16,*MIDIUInt16P;
typedef unsigned long   MIDIUInt32,*MIDIUInt32P;
/*
typedef MIDIUInt8       BYTE;
typedef MIDIUInt16      WORD;
typedef MIDIUInt32      DWORD;
*/



//MIDI文件头的结构体
typedef struct
{
    DWORD   dwFlag;             //MThd标识
    DWORD   dwRestLen;          //剩余部分长度
    WORD    wType;              //类型
    WORD    wNbTracks;          //音轨数
    WORD    wTicksPerCrotchet;  //每四分音符的Tick数
}MIDIHeader,*MIDIHeaderP;

//MIDI文件中出现过的标识
#define MIDI_MThd   0x6468544D
#define MIDI_MTrk   0x6B72544D

//MIDI文件头的“类型”
#define MIDIT_SingleTrack   0   /*单音轨*/
#define MIDIT_MultiSync     1   /*同步多音轨*/
#define MIDIT_MultiAsync    2   /*异步多音轨*/

//各种长度的Big-Endian到Little-Endian的转换
#define BSwapW(x)   ((((x) & 0xFF)<<8)|(((x) & 0xFF00)>>8))
#define BSwap24(x)  ((((x) & 0xFF)<<16)|((x) & 0xFF00)|(((x) & 0xFF0000)>>16))
#define BSwapD(x)   ((((x) & 0xFF)<<24)|(((x) & 0xFF00)<<8)|(((x) & 0xFF0000)>>8)|(((x) & 0xFF000000)>>24))

//将音符字节转换成字符串的函数
char*NoteToString(BYTE bNote);

//读取字符串然后打印
size_t ReadStringAndPrint(FIL*,size_t);
int ParseMIDI(char*pszFileName);

extern BYTE MIDI_PlayFlag;
extern BYTE MIDI_StopFlag;
extern BYTE MIDI_AutoPlayFlag;
extern BYTE MIDI_StopPlayFlag;

extern u32 MIDI_AutoPlayCnt;
extern u32  Midi_AutoLightsCnt;

 
 
 
extern  int res;  
extern  FIL fsrc,fdst;
extern  FATFS fs;
extern  UINT br, bw;            // File R/W count
extern  BYTE buffer[512];       // file copy buffer


u8 ReadMidiFile(char* Path);


typedef struct
{
	size_t CurrentEventPos;
	DWORD  EventDelay;
	BYTE   TracksDelayFlag;
	BYTE   TracksFirstReadFlag;
	
}MIDIINFO;


void Sand_3Byte_MidiCmd(BYTE Byte1,BYTE Byte2,BYTE Byte3);

void Sand_2Byte_MidiCmd(BYTE Byte1,BYTE Byte2);
void PauseMidiPlay(void);
#define TRACK_END   0xff

#define  PLAY_STOP_DELAY_TIME   	3*100//3S
#define  AUTO_PLAY_DELAY_TIME   	10*100	//10S
#define  AUTO_LIGHTS_DELAY_TIME   	10*100//30S
#define  STOP_PLAY_DELAY_TIME        3*100 //3S


enum{
	
	MIDI_ERR_NONE=0x00,
	MIDI_ERR_ERR,
	MIDI_ERR_READ_FILE_END,
	MIDI_ERR_READ_FOLDER_END,
	
	
	MIDI_ERR_DEVICE_CHANGE,
	MIDI_ERR_MIDI_FUNCTION_DIS,	
};


#define MIDI_CONTROLLER_NUMBER   




/*
--------------------------------------------
|        MIDI控制器一览表
--------------------------------------------       
编号	   参数意义
0	       音色库选择MSB
1	       颤音深度（粗调）
2	       呼吸（吹管）控制器（粗调）
3	       N/A
4	       踏板控制器（粗调）
5	       连滑音速度（粗调）
6	       高位元组数据输入（Data Entry MSB）
7	       主音量（粗调）
8	       平衡控制（粗调）
9	       N/A
10	       声像调整（粗调）
11	       情绪控制器（粗调）
12-15	   N/A
16-19	   一般控制器
20-31	   N/A
32	       插口选择
33	       颤音速度（微调）
34	       呼吸（吹管）控制器（微调）
35	       N/A
36	       踏板控制器（微调）
37	       连滑音速度（微调）
38	       低位元组数据输入（Data Entry LSB）
39	       主音量（微调）
40	       平衡控制（微调）
41	       N/A
42	       声像调整（微调）
43	       情绪控制器（微调）
44	       效果FX控制1（微调）
45	       效果FX控制2（微调）
46-63	   N/A
64	       保持音踏板1（延音踏板）
65	       滑音（在音头前加入上或下滑音做装饰音）
66	       持续音
67	       弱音踏板
68	       连滑音踏板控制器
69	       保持音踏板2
70	       变调
71	       音色
72	       放音时值
73	       起音时值
74	       亮音
75-79	   声音控制
80-83	   一般控制器（#5-#8）
84	       连滑音控制
85-90	   N/A
91	       混响效果深度
92	       （未定义的效果深度）
93	       合唱效果深度
94	       （未定义的效果深度）
95	       移调器深度
96	       数据累增
97	       数据递减
98	       未登记的低元组数值（NRPN LSB）
99	       未登记的高元组数值（NRPN MSB）
100	       已登记的低元组数值（RPN LSB）
101	       已登记的高元组数值（RPN MSB）
102-       119	N/A
120	       关闭所有声音
121	       关闭所有控制器
122	       本地键盘开关
123	       关闭所有音符
124	       Omni模式关闭
125	       Omni模式开启
126	       单音模式
127	       复音模式
-------------------------------------------------
*/


/*
种类?
字节?
含义?
参数(十六进制)?
音符(00~7F):松开的音符?
8x?
松开音符?
力度:00~7F?
音符(00~7F):按下的音符?
9x?
按下音符?
力度:00~7F?
音符:00~7F?
Ax?
触后音符?
(Key?After?Touch)?
力度:00~7F?控制器号码:00~7F?
Bx?
控制器?
控制器参数:00~7F?
Cx?改变乐器?乐器号码:00~7F?Dx?触后通道?值:00~7F?
Ex?
滑音?
音高(Pitch)低位:Pitch?mod?128?
音高高位:Pitch?div?128?系统码字节数:动态字节?
F0?
系统码?
系统码:不含开头的F0,但包括结尾的F7?程式种类:00~FF?
数据占用的字节数:动态字节?FF?
其他格式?
数据:个数由上一参数确定?
00~7F?
上次激活格式的参数(8x、9x、Ax、Bx、Cx、Dx、Ex)?
下表详细地列出了FF的详细情况，对于字节数由数据决定的情况，表中以“--”表示。?种类?字节?含义?字节数?
数据?
00?
设置轨道音序?02?音序号?00?00~?FF?FF?
歌曲备注?
文本信息?01?
音轨文本?
--?
文本信息?02?
歌曲版权?
--?版权信息?
歌曲标题?
--?
歌曲标题:用于全局音轨，第一次使用表示主标题，第二次表示副标题?03?
音轨名称?
--?音轨名?
04?乐器名称?--?音轨文本(同01/2)?05?歌词?--?歌词?
06?标记?--?用文本标记(Marker)?07?开始点?--?用文本记录开始点(同01/2)?2F?音轨结束标志?00?无?
51?速度?03?3字节整数，1个四分音符的微秒数?分子?
58?
节拍?
04?分母:00(1),01(2),02(4),03(8)等?



*/




#define MIDI_CONTROLLER_NUMBER1     1 
#define MIDI_CONTROLLER_NUMBER64    64 
#define MIDI_CONTROLLER_NUMBER121   121




#ifdef NDEBUG


#define my_fputc(...) 
#define my_fputs(...) 
#define my_fprintf(...)
#define my_printf(...)
#else
#define my_fputc fputc
#define my_fputs fputs
#define my_fprintf fprintf
#define my_printf printf
#endif


#endif

