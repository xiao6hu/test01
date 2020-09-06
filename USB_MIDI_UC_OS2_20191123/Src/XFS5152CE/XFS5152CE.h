#ifndef __XFS5152CE_H
#define	__XFS5152CE_H

#include "stm32f2xx.h"
#include "sys.h"

extern u8 XFS5152CE_Flag;

extern u8 VoiceBuff[4096];


/*
//语音文本资源定义

U8 * _CONST_ XFS5152CE_TEXT[]=
{
	{"请做好准备，游戏开始"},
	{"你失败了！"},	
	{"游戏结束"},
};
*/
#define XFS5152CE_LENGTH(n)(sizeof(n)/sizeof(U8 *)


#define XFS5152CE_CMD_AND_PAR 2

#define XFS5152CE_FRAME_HEADER 0xFD

#define VOICE_MIX_CMD 	0x01 //语音合成命令
#define STOP_MIX_CMD 	0x02 //停止合成命令
#define PAUSE_MIX_CMD 	0x03 //暂停合成命令
#define REGAIN_MIX_CMD 	0x04 //恢复合成命令


#define CHIP_STATSU_INQUIRY  	0x21 //芯片状态查询命令
#define CHIP_SLEEP_CMD 			0x88 //芯片进入省电模式命令
#define CHIP_WORK_CMD 			0xFF //芯片从省电模式返回正常工作状态命令


#define GB2313  	0x0 //
#define GBK 		0x1 //
#define BIG5 		0x2 //
#define UNICODE 	0x3 //
#define BUFF_BLOCK(x) (x<<4)

#define TX_BUFF_TEXT  	0x31 //发送缓存文本命令
#define PLAY_BUFF_TEXT  0x32 //播放缓存文本命令


#define XFS5152CE_INIT_SUCCESS  	0x4A //芯片初始化成功
#define XFS5152CE_RX_OK  			0x41 //收到正确的命令帧
#define XFS5152CE_RX_ERROR  		0x45 //收到错误的命令帧
#define XFS5152CE_BUSY  			0x4E //芯片忙碌
#define XFS5152CE_IDLE  			0x4F //芯片空闲



#define XFS5152CE_NON_WAIT_PLAY   		(0<<0) //不等待上一条语音播放完成立即播放
#define XFS5152CE_WAIT_PLAY 			(1<<0) //等待上一条语音播放完成后再播放

#define XFS5152CE_NON_WAIT_STOP   		(0<<1) //不等待上一条语音播放完成立即播放
#define XFS5152CE_WAIT_STOP 			(1<<1) //等待上一条语音播放完成后再播放





void XFS5152CE_Config(void);
void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data, u8 IsWita);
void XFS5152CE_SendData_Wita(u16 length, u8 command, u8 parameter, u8* data);
void XFS5152CE_ReturnStatus(u8 dat);

void XFS5152CE_Play(u8 PlayStatus);


#endif /* __XFS5152CE_H */

