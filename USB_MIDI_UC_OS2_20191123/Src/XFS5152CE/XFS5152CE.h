#ifndef __XFS5152CE_H
#define	__XFS5152CE_H

#include "stm32f2xx.h"
#include "sys.h"

extern u8 XFS5152CE_Flag;

extern u8 VoiceBuff[4096];


/*
//�����ı���Դ����

U8 * _CONST_ XFS5152CE_TEXT[]=
{
	{"������׼������Ϸ��ʼ"},
	{"��ʧ���ˣ�"},	
	{"��Ϸ����"},
};
*/
#define XFS5152CE_LENGTH(n)(sizeof(n)/sizeof(U8 *)


#define XFS5152CE_CMD_AND_PAR 2

#define XFS5152CE_FRAME_HEADER 0xFD

#define VOICE_MIX_CMD 	0x01 //�����ϳ�����
#define STOP_MIX_CMD 	0x02 //ֹͣ�ϳ�����
#define PAUSE_MIX_CMD 	0x03 //��ͣ�ϳ�����
#define REGAIN_MIX_CMD 	0x04 //�ָ��ϳ�����


#define CHIP_STATSU_INQUIRY  	0x21 //оƬ״̬��ѯ����
#define CHIP_SLEEP_CMD 			0x88 //оƬ����ʡ��ģʽ����
#define CHIP_WORK_CMD 			0xFF //оƬ��ʡ��ģʽ������������״̬����


#define GB2313  	0x0 //
#define GBK 		0x1 //
#define BIG5 		0x2 //
#define UNICODE 	0x3 //
#define BUFF_BLOCK(x) (x<<4)

#define TX_BUFF_TEXT  	0x31 //���ͻ����ı�����
#define PLAY_BUFF_TEXT  0x32 //���Ż����ı�����


#define XFS5152CE_INIT_SUCCESS  	0x4A //оƬ��ʼ���ɹ�
#define XFS5152CE_RX_OK  			0x41 //�յ���ȷ������֡
#define XFS5152CE_RX_ERROR  		0x45 //�յ����������֡
#define XFS5152CE_BUSY  			0x4E //оƬæµ
#define XFS5152CE_IDLE  			0x4F //оƬ����



#define XFS5152CE_NON_WAIT_PLAY   		(0<<0) //���ȴ���һ���������������������
#define XFS5152CE_WAIT_PLAY 			(1<<0) //�ȴ���һ������������ɺ��ٲ���

#define XFS5152CE_NON_WAIT_STOP   		(0<<1) //���ȴ���һ���������������������
#define XFS5152CE_WAIT_STOP 			(1<<1) //�ȴ���һ������������ɺ��ٲ���





void XFS5152CE_Config(void);
void XFS5152CE_SendData(u16 length, u8 command, u8 parameter, u8* data, u8 IsWita);
void XFS5152CE_SendData_Wita(u16 length, u8 command, u8 parameter, u8* data);
void XFS5152CE_ReturnStatus(u8 dat);

void XFS5152CE_Play(u8 PlayStatus);


#endif /* __XFS5152CE_H */

