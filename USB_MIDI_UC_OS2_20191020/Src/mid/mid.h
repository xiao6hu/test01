

#ifndef _MID_H_
#define _MID_H_



/*
MIDI�ļ���һ�ֶ�ý�������ļ�����ͬ�ڱ�������ļ���MIDI�ļ�ֻ��¼���ס����ŵ�ʱ��������ȡMIDI�ļ������ݣ�Ȼ������׷��͸�������������ģ�ⷢ���������������ԭ��������ļ���MP3��OGG��AMR��WAV��FLAC��AAC�ȣ������ּ�¼���׵��ļ���С�������״�����ȱ������Ϊ���������ģ�ⷢ���ģ����ʲ��ܵõ���֤�����Ҳ�ͬ�������в�ͬ�ķ���Ч�������MIDI�ļ��Ĳ���Ч�����ܵõ���֤������Ϊ�ˣ�΢���ṩ������ϳ�����ʹ��΢����������ʵ��MIDI�ļ�Ч����ͳһ��

��������Ϸ����ħ֮��III�����׳ơ��׵�3�����ı������־�����MIDI���ֲ��ŵ�Ч����΢��DirectX�����DirectMusic�������ڲ���MIDI���֡�

MIDI�ļ������Ϸ��������֣��ļ�ͷ�����졣
�ļ�ͷ��14���ֽ���ɡ���ͷ���ļ���ʶ��MThd����֮����һ��Big-Endian�洢��DWORD��ֵͨ��Ϊ6�����ֵ�������ǡ��ļ�ͷ����Ϣ��ռ�ֽ�������Ҳ�����ļ�ͷ������6���ֽڵ��ֽ���������6�����֮���6���ֽڣ���ʵ��3��Big-Endian�洢��WORD���ֱ��������֣���������ÿ�ķ�������Tick����������������ֵ��0��ʾ����ļ�ֻ��һ�����죬1��ʾ����ļ��ж�����죬���ŵ�ʱ��Ҫͬ�������������죬2��ʾ����ļ���Ȼ�ж�����죬���ǲ��ŵ�ʱ�����һ������һ������Ĳ��š�
��һ���ṹ��������MIDI�ļ�ͷ�Ľṹ��
*/
/*
typedef struct
{
    DWORD   dwFlag;             //MThd��ʶ
    DWORD   dwRestLen;          //ʣ�ಿ�ֳ���
    WORD    wType;              //����
    WORD    wNbTracks;          //������
    WORD    wTicksPerCrotchet;  //ÿ�ķ�������Tick��
}MIDIHeader;
*/
/*
���Ͼ����ļ�ͷ�ˡ�Ȼ������ļ�����Ҫ���֣��ļ������ˡ�
�ļ������ɶ��������ɣ�ÿ������Ŀ�ͷ���������Ľṹ��4�ֽڵ������ʶ"MTrk"��4�ֽ���Big-Endian�洢�������ֽ�����������ʾ��
*/
/*
typedef struct
{
    DWORD   dwFlag;     //Ϊ0x6B72544D����"MTrk"
    DWORD   dwTrackLen; //���쳤�ȣ���ȥ����ͷ��������ֽ�����(Big-Endian)
}MIDITrack;
*/

/*
����dwTrackLen��ֵ��������������ֽ�����ȥ����ͷ����8���ֽڡ�
ÿ�����죬��ȥ����ͷ�����⣬ʣ�µľ�����Ҫ���ļ������ˡ��ڽ����ļ�������ǰ���Ҿ����б�Ҫ����һ��MIDI�ļ����õġ���̬�ֽڡ�����ô���¡�
���������̬�ֽڴ��ڵ�������Ϊ�˼���һ�����ֵĴ洢�ռ䡣1���ֽ���8λ�������ó����еĵ�7λ�洢���֣���ô�����ܴ洢�ķ�Χ��0-127���������Ҫ�洢�������������Χ�ڵĻ������ǾͰ����λ����Ϊ0�����������Ҫ�洢�����ֳ�����0-127�����Χ����ô���ǾͰ����λ����Ϊ1��Ȼ���¼�¸�7λ��ʣ�µ�������һ���ֽڡ���������Ҫ�洢111������֣���Ϊ����ֵ��0-127��Χ�ڣ����ǿ���ֻ��һ���ֽڴ洢��01101111b����������Ҫ�洢333������֣�����ת���ɶ����Ƶ�ʱ����101001101b��������7λ�ܴ洢�ķ�Χ����ô��������ȡ�����ĸ�7λ�͵�7λ��0000010��1001101.Ȼ�������������������ֽڴ洢��10000010b��01001101b.�����Ƕ�ȡ��̬�ֽڵ�ʱ�������ȶ�ȡһ���ֽڣ���¼���ĵ�7λ��Ȼ���ж��������λ���ж��Ƿ���Ҫ������ȡ��һ���ֽڡ�
����VB��������չʾ��ȡ��̬�ֽڵķ��������õ�ʱ�����ļ���1���ļ���Ȼ����Seek #1��λ�ļ�ָ�룬������������ȡ��̬�ֽڡ�
'��ȡ��̬�ֽ�
*/
/*
Function ReadDynamicBytes(ValueOut As Long) As Long '���ض�ȡ���ֽ���
Dim OneByte As Byte
ValueOut = 0
For ReadDynamicBytes = 1 To 4 '����ȡ4���ֽ�
    Get #1, , OneByte '��ȡһ���ֽ�
    ValueOut = (ValueOut * &H80&) Or (OneByte And &H7F) '��¼����ֽڵĵ�7λ��ͬʱ�����Զ�ȡ�������ó�λ�á�
    If (OneByte And &H80) = 0 Then Exit For '����ֽڵ����λ��0��û�к����ֽڣ�ֹͣ��ȡ��
Next
End Function
*/

/*
�����϶�̬�ֽڿ����ڴ洢�����������ر�������MIDI�ļ����ò��š����Ƕ����ȡ4�ֽڣ�28λ������

�����˶�̬�ֽڣ���������Ӧ�ý��ļ������ˡ��ļ������ݶ��ǡ��¼�������Щ���¼�����һ����һ���洢�ġ�һ���¼����й̶��Ľṹ���ӳ٣��¼��ţ��¼�������
���С��ӳ١��Ƕ�̬�ֽڣ����ڱ�ʾ��һ���¼�����һ���¼�֮����ӳ���������ӳ����ĵ�λ��Tick��һ��Tick�ж೤ʱ��ȡ����MIDI�ļ������١�
�¼��ŵ�ֵ��0x80��0xFF֮���ʱ���ʾ���Ǿ����ֵ������ȡ�������ֵ��0��0x7F֮�䣬���ʾ����¼����¼��ź���һ���¼���ͬ������ȡ����ֵ�����Ĳ�����
�й��¼��ŵ������������ϲ���Ҳ��ͬС�졣���Ƕ����ò����������ר��д��һ��C���Գ������ڽ����¼��š���Դ���֪��
*/
//=============================================================================
//���ߣ�0xAA55
//��̳��http://www.0xaa55.com/
//��Ȩ����(C) 2013-2014 ����լ�Ľ��
//�뱣��ԭ������Ϣ��������Ϊ��Ȩ��
//-----------------------------------------------------------------------------
#include<stdio.h>
//#include<malloc.h>
#include<stdlib.h>

#include "sys.h"

#include "ff.h"

//ͳһ���ͳ���
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



//MIDI�ļ�ͷ�Ľṹ��
typedef struct
{
    DWORD   dwFlag;             //MThd��ʶ
    DWORD   dwRestLen;          //ʣ�ಿ�ֳ���
    WORD    wType;              //����
    WORD    wNbTracks;          //������
    WORD    wTicksPerCrotchet;  //ÿ�ķ�������Tick��
}MIDIHeader,*MIDIHeaderP;

//MIDI�ļ��г��ֹ��ı�ʶ
#define MIDI_MThd   0x6468544D
#define MIDI_MTrk   0x6B72544D

//MIDI�ļ�ͷ�ġ����͡�
#define MIDIT_SingleTrack   0   /*������*/
#define MIDIT_MultiSync     1   /*ͬ��������*/
#define MIDIT_MultiAsync    2   /*�첽������*/

//���ֳ��ȵ�Big-Endian��Little-Endian��ת��
#define BSwapW(x)   ((((x) & 0xFF)<<8)|(((x) & 0xFF00)>>8))
#define BSwap24(x)  ((((x) & 0xFF)<<16)|((x) & 0xFF00)|(((x) & 0xFF0000)>>16))
#define BSwapD(x)   ((((x) & 0xFF)<<24)|(((x) & 0xFF00)<<8)|(((x) & 0xFF0000)>>8)|(((x) & 0xFF000000)>>24))

//�������ֽ�ת�����ַ����ĺ���
char*NoteToString(BYTE bNote);

//��ȡ�ַ���Ȼ���ӡ
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
|        MIDI������һ����
--------------------------------------------       
���	   ��������
0	       ��ɫ��ѡ��MSB
1	       ������ȣ��ֵ���
2	       ���������ܣ����������ֵ���
3	       N/A
4	       ̤����������ֵ���
5	       �������ٶȣ��ֵ���
6	       ��λԪ���������루Data Entry MSB��
7	       ���������ֵ���
8	       ƽ����ƣ��ֵ���
9	       N/A
10	       ����������ֵ���
11	       �������������ֵ���
12-15	   N/A
16-19	   һ�������
20-31	   N/A
32	       ���ѡ��
33	       �����ٶȣ�΢����
34	       ���������ܣ���������΢����
35	       N/A
36	       ̤���������΢����
37	       �������ٶȣ�΢����
38	       ��λԪ���������루Data Entry LSB��
39	       ��������΢����
40	       ƽ����ƣ�΢����
41	       N/A
42	       ���������΢����
43	       ������������΢����
44	       Ч��FX����1��΢����
45	       Ч��FX����2��΢����
46-63	   N/A
64	       ������̤��1������̤�壩
65	       ����������ͷǰ�����ϻ��»�����װ������
66	       ������
67	       ����̤��
68	       ������̤�������
69	       ������̤��2
70	       ���
71	       ��ɫ
72	       ����ʱֵ
73	       ����ʱֵ
74	       ����
75-79	   ��������
80-83	   һ���������#5-#8��
84	       ����������
85-90	   N/A
91	       ����Ч�����
92	       ��δ�����Ч����ȣ�
93	       �ϳ�Ч�����
94	       ��δ�����Ч����ȣ�
95	       �Ƶ������
96	       ��������
97	       ���ݵݼ�
98	       δ�Ǽǵĵ�Ԫ����ֵ��NRPN LSB��
99	       δ�Ǽǵĸ�Ԫ����ֵ��NRPN MSB��
100	       �ѵǼǵĵ�Ԫ����ֵ��RPN LSB��
101	       �ѵǼǵĸ�Ԫ����ֵ��RPN MSB��
102-       119	N/A
120	       �ر���������
121	       �ر����п�����
122	       ���ؼ��̿���
123	       �ر���������
124	       Omniģʽ�ر�
125	       Omniģʽ����
126	       ����ģʽ
127	       ����ģʽ
-------------------------------------------------
*/


/*
����?
�ֽ�?
����?
����(ʮ������)?
����(00~7F):�ɿ�������?
8x?
�ɿ�����?
����:00~7F?
����(00~7F):���µ�����?
9x?
��������?
����:00~7F?
����:00~7F?
Ax?
��������?
(Key?After?Touch)?
����:00~7F?����������:00~7F?
Bx?
������?
����������:00~7F?
Cx?�ı�����?��������:00~7F?Dx?����ͨ��?ֵ:00~7F?
Ex?
����?
����(Pitch)��λ:Pitch?mod?128?
���߸�λ:Pitch?div?128?ϵͳ���ֽ���:��̬�ֽ�?
F0?
ϵͳ��?
ϵͳ��:������ͷ��F0,��������β��F7?��ʽ����:00~FF?
����ռ�õ��ֽ���:��̬�ֽ�?FF?
������ʽ?
����:��������һ����ȷ��?
00~7F?
�ϴμ����ʽ�Ĳ���(8x��9x��Ax��Bx��Cx��Dx��Ex)?
�±���ϸ���г���FF����ϸ����������ֽ��������ݾ���������������ԡ�--����ʾ��?����?�ֽ�?����?�ֽ���?
����?
00?
���ù������?02?�����?00?00~?FF?FF?
������ע?
�ı���Ϣ?01?
�����ı�?
--?
�ı���Ϣ?02?
������Ȩ?
--?��Ȩ��Ϣ?
��������?
--?
��������:����ȫ�����죬��һ��ʹ�ñ�ʾ�����⣬�ڶ��α�ʾ������?03?
��������?
--?������?
04?��������?--?�����ı�(ͬ01/2)?05?���?--?���?
06?���?--?���ı����(Marker)?07?��ʼ��?--?���ı���¼��ʼ��(ͬ01/2)?2F?���������־?00?��?
51?�ٶ�?03?3�ֽ�������1���ķ�������΢����?����?
58?
����?
04?��ĸ:00(1),01(2),02(4),03(8)��?



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

