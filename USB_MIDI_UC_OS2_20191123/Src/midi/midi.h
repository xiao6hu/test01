#ifndef _MIDI_H_
#define _MIDI_H_
#include "includes.h"
#include "stm32f2xx_hal.h"
#include "sys.h"
#include "usart1.h"	
#include "ff.h"
#include "midi_cfg.h"


//ͳһ���ͳ���
typedef signed int      MIDIInt,*MIDIIntP;
typedef signed char     MIDIInt8,*MIDIInt8P;
typedef signed short    MIDIInt16,*MIDIInt16P;
typedef signed long     MIDIInt32,*MIDIInt32P;
typedef unsigned int    MIDIUInt,*MIDIUIntP;
typedef unsigned char   MIDIUInt8,*MIDIUInt8P;
typedef unsigned short  MIDIUInt16,*MIDIUInt16P;
typedef unsigned long   MIDIUInt32,*MIDIUInt32P;

//����Ľṹ����midi�ļ�ͷ����Ϣ
#define TRACK_MODE_SINGLE		0	//����
#define TRACK_MODE_MUL_SYNC		1	//���ͬ��
#define TRACK_MODE_MUL_ASYNC	2	//����첽
typedef struct
{
	MIDIUInt32  HeaderFlag;
	MIDIUInt32  RestLen;
	MIDIUInt16  tracks_mode;
	MIDIUInt16  tracks_cnt;
	MIDIUInt16  tempo;
}MIDI_HEADER;

//MIDI_HEADER midi_header;


//����Ľṹ����midi ͨ������Ϣ
typedef struct
{
	MIDIUInt32	ptr;//��ǰ�����λ��
	MIDIUInt32 	len;//�ڳ�
	MIDIUInt8 	last_command;//�ϴ�ִ�е�����
	MIDIUInt8 	track_end;//��־��ͨ���Ƿ����
	MIDIUInt32 	flow_cnt;//һ�������ļ������������Ͷ�ʱ�����бȽ�
	MIDIUInt8 	needTLV;//�Ƿ���Ҫ���ڶ�ȡtlv
	MIDIUInt8 	search_whole_words;//�Ƿ���Ҫ���������
}TRACK_CONTROL;

#define MAX_TRACKS_CNT	16

//MIDI�ļ��г��ֹ��ı�ʶ
#define MIDI_MThd   0x4D546864 //0x6468544D
#define MIDI_MTrk   0x4D54726B //0x6B72544D
/*
	�ú������ڷ���midi�ļ���ʽ����һЩ��Ҫ�ĳ�ʼ��
	��ȷ����0
	����������ܷ��صĴ�����Ϊ
	MIDI_HEADER_FORMAT_ERROR
	MIDI_NOTSUPPORT_ASYNC
	MIDI_TRACK_PARSER_ERROR
	
*/
#define MIDI_ERROR					0	
#define MIDI_HEADER_FORMAT_ERROR	-1
#define MIDI_NOTSUPPORT_ASYNC		-2
#define MIDI_TRACK_PARSER_ERROR		-3
#define MIDI_READ_ERROR				0


//���ֳ��ȵ�Big-Endian��Little-Endian��ת��
#define BSwapW(x)   ((((x) & 0xFF)<<8)|(((x) & 0xFF00)>>8))
#define BSwap24(x)  ((((x) & 0xFF)<<16)|((x) & 0xFF00)|(((x) & 0xFF0000)>>16))
#define BSwapD(x)   ((((x) & 0xFF)<<24)|(((x) & 0xFF00)<<8)|(((x) & 0xFF0000)>>8)|(((x) & 0xFF000000)>>24))


enum{
	
	MIDI_ERR_NONE=0x00,
	MIDI_ERR_ERR,
	MIDI_ERR_READ_FILE_END,
	MIDI_ERR_READ_FOLDER_END,
	
	
	MIDI_ERR_DEVICE_CHANGE,
	MIDI_ERR_MIDI_FUNCTION_DIS,	
	MIDI_ERR_PLAY_EN_DIS,
};







extern MIDIUInt8 MidiStartstopFlag;

extern int res;
extern FIL fsrc,fdst;
extern FATFS fs;
extern UINT br, bw;            // File R/W count
extern BYTE buffer[512];       // file copy buffer
extern DIR dir;
extern FILINFO filinfo;
extern FATFS fs;


extern BYTE MIDI_PlayFlag;
extern BYTE MIDI_StopFlag;
extern BYTE MIDI_AutoPlayFlag;
extern BYTE MIDI_StopPlayFlag;
extern u32 MIDI_AutoPlayCnt;
extern u32  Midi_AutoLightsCnt;



#define READERR {return MIDI_READ_ERROR;}
#define FMTERR 	{return MIDI_READ_ERROR;}
#define READ(x) 	if(f_read(fp,&x,sizeof(x),&br)!=FR_OK)READERR			 
#define F_LSEEK(x) 	if(f_lseek(fp,x)!=FR_OK)READERR

void Sand_3Byte_MidiCmd(BYTE Byte1,BYTE Byte2,BYTE Byte3);
void PauseMidiPlay(void);
u8 ReadMidiFile(char* Path);
int ParseMIDI(char*pszFileName);
void midi_heart_beat(void);
MIDIInt16 parser_midifile_header(void);
static MIDIUInt16 get_wait_time(MIDIUInt32 wait);
static MIDIInt16 do_event(TRACK_CONTROL *ptrack);
static MIDIUInt32 GetVLE (MIDIUInt32 ptr_origin,MIDIUInt8 *jmp);
static MIDIUInt32 parser_track_header(MIDIUInt32 *ptr,TRACK_CONTROL *ptrack);
static void ChangeSpeed (MIDIUInt32 time);

void midi_timer_start(void);
void midi_timer_stop(void);
void midi_write_uart(MIDIUInt8 *dat,MIDIUInt16 len);

int MidiPlayControl(void) ;

void midi_timer1_ISR(void);


/*
static unsigned long read4byte(unsigned char *ptr);
static unsigned short read2byte(unsigned char *ptr);
static unsigned char read1byte(unsigned char *ptr);
*/

#endif

