
//#include "stm32f10x.h"
#include "includes.h"
#include "stm32f2xx_hal.h"
#include "sys.h"


//#include "sdio_sdcard.h"
#include "usart1.h"	
#include "ff.h"

#include "mid.h"

#include "timr_delay.h"
#include "sys.h"
#include "timr.h"

#include "key.h"

int res;  
FIL fsrc,fdst;
FATFS fs;
UINT br, bw;            // File R/W count
BYTE buffer[512];       // file copy buffer


DIR dir;
FILINFO filinfo;
FATFS fs;




DWORD UsDelay;

DWORD UsTime;


BYTE MIDI_PlayFlag =0;

BYTE MIDI_StopFlag =0;

BYTE MIDI_AutoPlayFlag =0;

BYTE MIDI_StopPlayFlag=0;


u32 MIDI_AutoPlayCnt=0;
u32  Midi_AutoLightsCnt=0;



WORD ReadAllTracksEnd =0;
	
MIDIINFO MidiInfo[16];

//=============================================================================
//ReadDynamicBytes:
//��ȡ��̬�ֽڣ�����ȡ4�ֽڡ����ض�ȡ���ֽ���
//-----------------------------------------------------------------------------
//�¼�     ����      ����
//bEvent   bNote     bVel
void Sand_3Byte_MidiCmd(BYTE Byte1,BYTE Byte2,BYTE Byte3)
{
    INT8U  err;
	#if 1
	
    /*
    Uart1SendData(Byte1);
    Uart1SendData(Byte2);
    Uart1SendData(Byte3);
    */
        
    Uart4_TxBuff[0]= Byte1;
    Uart4_TxBuff[1]= Byte2;
    Uart4_TxBuff[2]= Byte3;
    HAL_UART_Transmit_IT(&huart4,Uart4_TxBuff,3); 
	
    OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
                (OS_FLAGS) UART4_TX_OK_FLAG ,
                (INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
                (INT32U) 0,
                (INT8U *)&err);
	#endif
}


/*
void Sand_2Byte_MidiCmd(BYTE Byte1,BYTE Byte2)
{
	
	INT8U  err;
	#if 1
    
	//Uart1SendData(Byte1);
	//Uart1SendData(Byte2);
    
    Uart1_TxBuff[0]= Byte1;
    Uart1_TxBuff[1]= Byte2;

    HAL_USART_Transmit_IT(&husart1,Uart1_TxBuff,2); 
	
	
    OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
                (OS_FLAGS) UART1_TX_OK_FLAG ,
                (INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
                (INT32U) 0,
                (INT8U *)&err);

	
	#endif
}
*/								 
//=============================================================================
//ReadDynamicBytes:
//��ȡ��̬�ֽڣ�����ȡ4�ֽڡ����ض�ȡ���ֽ���
//-----------------------------------------------------------------------------
size_t ReadDynamicBytes(FIL*fp,DWORD*pOut)
{
    size_t bBytesRead;//�Ѷ�ȡ���ֽ���
    *pOut=0;
    for(bBytesRead=1;bBytesRead<=4;bBytesRead++)//����ȡ4�ֽ�
    {
        //int iValue=fgetc(fp);
		
		//int iValue=fgetc(fp);
		
		char iValue;
		
		//f_gets(&iValue,1,fp);
		
		f_read(fp,&iValue,1,&br);
		
		if (f_eof(fp) ==1)
		
        //if(iValue==EOF)
            return 0;
        *pOut=(*pOut<<7)|(iValue & 0x7F);//�¶�����ǵ�λ
        if(!(iValue & 0x80))//���û�к����ֽ�
            break;//��ֹͣ��ȡ��
    }
    return bBytesRead;//���ض�ȡ���ֽ���
}


#if 0
//=============================================================================
//ParseMIDI:
//����MIDI�ļ���ʧ�ܷ����㣬�ɹ����ط���
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"��ȡ�ļ�%s��������\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s����һ��������MIDI�ļ�\r\n",pszFileName);goto BadReturn;}
	#define READ(x) if(f_read(fp,&x,sizeof(x),&br)!=FR_OK)READERR

	
	
	
	//int res;
    //FIL *fsrc,fdst;
	
	FIL *fp = &fdst;
	
    MIDIHeader midiHeader;
    size_t i;

    //���ļ�
    //fp=fopen(pszFileName,"rb");
	
	res = f_open(fp,pszFileName,FA_READ);
	//res = f_open(fp,"0:midfile\baga01.mid",FA_READ);
	
    if (res != FR_OK)
        READERR;

    //MIDI�ļ�ͷ����һ��MIDIHeader�ṹ�塣
    //����Ҫע�����е���ֵ����Big-Endian�洢��
    //��Ҫ����ת��

    //��ȡMIDI�ļ�ͷ
    READ(midiHeader);
    
    //����ļ���ʽ
    if(midiHeader.dwFlag!=MIDI_MThd)//��ʶ������"MThd"
        FMTERR;

    //ת��ΪLittle-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //�����ļ�ͷ
    switch(midiHeader.wType)
    {
    case MIDIT_SingleTrack:
        my_fputs("����:������\r\n",stdout);
        break;
    case MIDIT_MultiSync:
        my_fputs("����:ͬ��������\r\n",stdout);
        break;
    case MIDIT_MultiAsync:
        my_fputs("����:�첽������\r\n",stdout);
        break;
    default:
        my_fprintf(stdout,"����:δ֪(0x%04X)\n",midiHeader.wType);
        break;
    }

    //��ӡ����������Ϣ
    my_fprintf(stdout,
        "������:%u\r\n"
        "ÿ�ķ�����ʱ����:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //��ȷ��ת��MIDI�����λ�ã�����midiHeader.dwRestLen�����á���
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	
	f_lseek (&fdst, 8+midiHeader.dwRestLen);
	

    //׼����ȡ��������
    for(i=0;i<midiHeader.wNbTracks;i++)
    {
        DWORD dwTrackFlag;
        DWORD dwTrackLen;

        //ÿ������Ŀ�ͷ����һ��dwTrackFlag��һ��dwTrackLen
        //dwTrackFlag��ֵ������MIDI_MTrk
        //dwTrackLen�������һ�������λ��

        size_t TrackStartPos;

        my_fputs("��ȡ����\r\n",stdout);

        READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//����ļ���ʽ
        READ(dwTrackLen);   TrackStartPos=f_tell(fp);//��¼��ǰλ��

        //ת��Big-Endian
        dwTrackLen=BSwapD(dwTrackLen);

        //��ʾÿ��������ֽ���
        my_fprintf(stdout,"�����С:%u �ֽ�\r\n",dwTrackLen);

        //�������Ҫ�������¼�����
        for(;;)//ѭ����ȡ�¼�
        {
            DWORD dwDelay;
            BYTE bEvent;
			DWORD Delay;
            //ÿ���¼��Ĺ��ɶ��ǣ�
            //��ʱ���¼��ţ�����
            //���е���ʱ�Ƕ�̬�ֽڣ�������С���¼��Ŷ���

            //��һ���¼���
            BYTE bLastEvent;

			
            //��ȡ��ʱ
            if(!ReadDynamicBytes(fp,&dwDelay))
                READERR;
			
			
			

			Delay = UsDelay/midiHeader.wTicksPerCrotchet*dwDelay/2;
			
			//Delay_us(Delay+1);
			
			HAL_Delay(Delay+1);
			
            //��ȡ�¼���
            READ(bEvent);

            //ÿ��ȡһ���¼�����ӡ����¼���ʱ��
            my_fprintf(stdout,"ʱ��:%u,",dwDelay);

            //�����¼�
ProcEvent:  if(bEvent <= 0x7F)
            {//0��0x7F:����һ���¼����¼�����ͬ������ȡ������ֽھ�����һ���¼��ŵĲ����ĵ�һ���ֽ�
                //fseek(fp,-1,SEEK_CUR);//����һ���ֽ�
				
				f_lseek(fp,f_tell(fp)-1);//����һ���ֽ�
				
				
                bEvent=bLastEvent;
                goto ProcEvent;
            }
            else if(bEvent <= 0x8F)
            {//0x80��0x8F:�ɿ�����
                BYTE bNote,bVel;

                READ(bNote);//����
                READ(bVel); //����

				
				while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
				MIDI_StopFlag =0;
				
				Sand_3Byte_MidiCmd(bEvent,//�¼�
							 bNote, //����
							 bVel); //����

                my_fprintf(stdout,"(ͨ��%u)�ɿ�����:%s, ����:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    NoteToString(bNote),//����
                    bVel);//����
				
				
            }
            else if(bEvent <= 0x9F)
            {//0x90��0x9F:��������
                BYTE bNote,bVel;

                READ(bNote);//����
                READ(bVel); //����

				
				//while ((!MIDI_PlayFlag) && (bVel !=0x00));
				
				
				if (MIDI_AutoPlayFlag)
				{
					MIDI_PlayFlag =0;
					MIDI_StopFlag =0;
					
				}
				else
				{	
					while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));

					if (bVel !=0x00)
					{
						MIDI_PlayFlag =0;
						
						if (!Midi_En_dis)
						{
							goto BadReturn;
						}	
					}
					else
					{
						MIDI_StopFlag=0;		
					}	
				}
				
				Sand_3Byte_MidiCmd(bEvent,//�¼�
							 bNote, //����
						     bVel); //����
				

                my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    NoteToString(bNote),//����
                    bVel);//����
				
            }
            else if(bEvent <= 0xAF)
            {//0xA0��0xAF:��������
                BYTE bNote,bVel;

                READ(bNote);//����
                READ(bVel); //����

				
				Sand_3Byte_MidiCmd(bEvent,//�¼�
							 bNote, //����
							 bVel); //����
				
				
				
                my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    NoteToString(bNote),//����
                    bVel);//����
				
            }
            else if(bEvent <= 0xBF)
            {//0xB0��0xBF:������
                BYTE bReg,bVal;

                READ(bReg);//�Ĵ�����
                READ(bVal);//�Ĵ���ֵ

				
				Sand_3Byte_MidiCmd(bEvent,//�¼�
							 bReg,  //�Ĵ�����
							 bVal); //�Ĵ���ֵ
				
				
				
                my_fprintf(stdout,"(ͨ��%u)������:%u,ֵ=%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    bReg,//�Ĵ���
                    bVal);//ֵ
				
            }
            else if(bEvent <= 0xCF)
            {//0xC0��0xCF:�ı�����
                BYTE bInstrument;

                READ(bInstrument);//������
                
				
				
				Sand_2Byte_MidiCmd(bEvent,	//�¼�
						     bInstrument);  //������
							
				
                my_fprintf(stdout,"(ͨ��%u)�ı�����:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    bInstrument);//������
				
            }
            else if(bEvent <= 0xDF)
            {//0xD0��0xDF:����ͨ��
                BYTE bChannel;

                READ(bChannel);//ͨ����
                
				Sand_2Byte_MidiCmd(bEvent,	   //�¼�
						           bChannel);  //ͨ����
				
				
                my_fprintf(stdout,"(ͨ��%u)����ͨ��:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    bChannel);//ͨ����
				
            }
            else if(bEvent <= 0xEF)
            {//0xE0��0xEF:����
                WORD wPitch;//����������Little-Endian��WORD

                READ(wPitch);
                
				
				Sand_3Byte_MidiCmd(bEvent,	    //�¼�
						           wPitch,		//��������L
								   wPitch>>8); //��������H

				
                my_fprintf(stdout,"(ͨ��%u)����:%u\r\n",
                    bEvent & 0xF,//��4λ��ʾͨ����
                    wPitch);//����
				
				
				
				
            }
            else if(bEvent == 0xF0)
            {//0xF0:ϵͳ��
                BYTE bSysCode=0;

				
				
                my_fputs("ϵͳ��:",stdout);
				
				
                for(;;)
                {
                    READ(bSysCode);
					
                    if(bSysCode!=0xF7)
					{	
					
						Sand_2Byte_MidiCmd(bEvent,	    //�¼�
								           bSysCode);   //ϵͳ��
					
						
					
                        my_fprintf(stdout,"0x%02X,",bSysCode);
						
						
					}
                    else
                    {
						
						Sand_2Byte_MidiCmd(bEvent,         //�¼�
								           bSysCode);      //ϵͳ��
						
						
                        my_fprintf(stdout,"0x%02X\r\n",bSysCode);
						
                        break;//��ȡ��0xF7����
                    }
                }
            }
            else if(bEvent == 0xFF)
            {//Ԫ�¼�
                BYTE bType,bBytes;
                size_t CurrentPos;
                
                READ(bType);//Ԫ��������
                READ(bBytes);//Ԫ�����ֽ���

				
				Sand_3Byte_MidiCmd(bEvent,      //�¼�
								   bType,		//Ԫ��������
								   bBytes);		//Ԫ�����ֽ���

                CurrentPos=f_tell(fp);//��¼Ԫ���ݶ�ȡ��λ��

				
                my_fputs("Ԫ���� - ",stdout);
				
                switch(bType)
                {
                case 0x00://���ù������
                    {
                        WORD wTrackSeq;
                        READ(wTrackSeq);
                        wTrackSeq=BSwapW(wTrackSeq);
						
			
                        my_fprintf(stdout,"���ù������:0x%04X\r\n",wTrackSeq);
						
						
                    }
                    break;
                case 0x01://������ע
                    my_fputs("��ע:",stdout);
				
					
				
                    ReadStringAndPrint(fp,bBytes);
				
				
                    fputc('\n',stdout);
				
				
				
                    break;
                case 0x02://��Ȩ
                    my_fputs("��Ȩ:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
					
                    break;
                case 0x03://��������
                    my_fputs("����:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x04://��������
                    my_fputs("������:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x05://���
                    my_fputs("���:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x06://���
                    my_fputs("���:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x07://��ʼ��
                    my_fputs("��ʼ��:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x21://���쿪ʼ��ʶ
                    my_fputs("���쿪ʼ��ʶ.\n",stdout);
                    break;
                case 0x2F://���������ʶ
                    my_fputs("���������ʶ.\n",stdout);
                    goto EndOfTrack;
                case 0x51://�ٶ�
                    {
                        BYTE bVelocity1,bVelocity2,bVelocity3;

                        //�ٶ���24λ������һ���ķ�������΢������
                        READ(bVelocity1);
                        READ(bVelocity2);
                        READ(bVelocity3);

                        //������Big-Endian
                         my_fprintf(stdout,"�ٶ�: ÿ�ķ�����%u΢��.\r\n",
                            bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
						
						UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
						
						
                    }
                    break;
                case 0x58://����
                    {
                        BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

                        READ(bBeatNumerator);//����
                        READ(bBeatDenominator);//��ĸ
                        READ(bMetronomeTimer);//������ʱ��
                        READ(bNb32ndNotesInCrotchet);//ÿ�ķ������ж�����ʮ��������

                        my_fprintf(stdout,"����:%u/%u. ������ʱ��:%u. ÿ�ķ�������%u����ʮ��������.\r\n",
                            bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
                    }
                    break;
                case 0x59://����
                    {
                        MIDIInt8 bTunePitch;
                        BYTE bTuneType;

                        READ(bTunePitch);//������
                        READ(bTuneType);//�����С��

                        my_fprintf(stdout,"������:%d, ",bTunePitch);
                        switch(bTuneType)
                        {
                        case 0://���
                            my_fputs("���\r\n",stdout);
                            break;
                        case 1://С��
                            my_fputs("С��\r\n",stdout);
                            break;
                        default://��֪��
                            my_fprintf(stdout,"δ֪��(0x%02X).\r\n",bTuneType);
                            break;
                        }
                    }
                    break;
                case 0x7F://�����ض���Ϣ
                    my_fputs("�����ض���Ϣ:",stdout);
                    ReadStringAndPrint(fp,bBytes);
				
					
                    fputc('\r',stdout);
					
				
                    break;
                default:
					
					
                    my_fprintf(stdout,"δ֪Ԫ��������(0x%02X)\r\n",bType);
					
				
                    break;
                }
				
		
				printf("\r\n");
				
				
				
				
                f_lseek(fp,CurrentPos+bBytes);//��ȡ��ɺ���ȷ������һ���¼���λ�á�
            }
            else//�����¼���δ֪�¼�
            {
				
				#ifdef NDEBUG
                my_fprintf(stdout,"δ֪�¼�:0x%08X. ֹͣ��ǰ����ķ�����\r\n",bEvent);
				#endif
				
                break;
            }

            //��¼��һ���¼���
            bLastEvent=bEvent;
        }//��ȥ������ȡ�¼���

EndOfTrack:
        f_lseek(fp,TrackStartPos+dwTrackLen);//ת����һ������
    }

    my_fputs("MIDI�ļ���ȡ������\r\n",stdout);

    f_close(fp);
    return 1;
BadReturn:
    f_close(fp);
    return 0;

#   undef READ
#   undef READERR
}

#endif



u8 ScanOnlineDevice(void)
{
	INT8U err;
	
	OS_FLAGS Flags;
	
	Flags =OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
					(INT8U *)&err);
	
	/*
	Flags =	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) DEVICE_CHANGE_FLAG,
					(INT8U) OS_FLAG_WAIT_SET_ALL|OS_FLAG_CONSUME,
					(INT8U  *)&err);
	*/	
	

	if((Flags & DEVICE_CHANGE_FLAG) == DEVICE_CHANGE_FLAG)
	{

		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) DEVICE_CHANGE_FLAG,
					(INT8U) OS_FLAG_CLR,
					(INT8U  *)&err);
		
		/*
		if ((ActivityDevice == U_DISK_ID) && !(OnlineDevice & (1<<U_DISK_ID)))
		{
			return MIDI_ERR_DEVICE_CHANGE;	
		}
		
		if ((ActivityDevice == SDMMC_ID) && !(OnlineDevice & (1<<SDMMC_ID)))	
		{	
			return MIDI_ERR_DEVICE_CHANGE;
		}
		
		if (ActivityDevice == NONE_DEVICE)
		{
			return MIDI_ERR_DEVICE_CHANGE;
		}
		*/
		return MIDI_ERR_DEVICE_CHANGE;
		
		
	}
	else if((Flags & MIDI_PLAY_EN_DIS_FLAG) == MIDI_PLAY_EN_DIS_FLAG)
	{
		return MIDI_ERR_MIDI_FUNCTION_DIS;
	}

	return MIDI_ERR_NONE;
}


void MidiVolumeControl(void)
{
	INT8U err;
	OS_FLAGS Flags;
	
	
	static u8 VolFlag=0;
	Flags =OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
					(INT8U *)&err);
	if((Flags & MIDI_STOP_PLAY_FLAG) == MIDI_STOP_PLAY_FLAG)
	{
		
		//�¼�     ����      ����
		//bEvent   bNote     bVel
		
		Sand_3Byte_MidiCmd(0xb1,120,0);
	}
	else
	{
		if (0==VolFlag)
		{
			VolFlag=1;	
		}	
	}	
}



#if 0
//=============================================================================
//ParseMIDI:
//����MIDI�ļ���ʧ�ܷ����㣬�ɹ����ط���
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"��ȡ�ļ�%s��������\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s����һ��������MIDI�ļ�\r\n",pszFileName);goto BadReturn;}
	#define READ(x) if(f_read(fp,&x,sizeof(x),&br)!=FR_OK)READERR

	//int res;
    //FIL *fsrc,fdst;
	
	FIL *fp = &fdst;
	
    MIDIHeader midiHeader;
    u16 TrackNumber;
	
	u8 ReturnStatus= MIDI_ERR_NONE;
	
	OS_FLAGS Flags;
	
	//size_t CurrentEventPos[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//DWORD EventDelay[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	
	WORD ReadAllTracksEnd =0;
	BYTE Denominator;
	u8 FirstReadTrack=1;
	/*
	MIDIINFO MidiInfo[16]=
	{
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},		
	};
	*/
	
	INT8U err;
	
	memset(MidiInfo,0,sizeof(MidiInfo));
	
    //���ļ�
	res = f_open(fp,pszFileName,FA_READ);
    if (res != FR_OK)
        READERR;

    //MIDI�ļ�ͷ����һ��MIDIHeader�ṹ�塣
    //����Ҫע�����е���ֵ����Big-Endian�洢��
    //��Ҫ����ת��

    //��ȡMIDI�ļ�ͷ
    READ(midiHeader);
    
    //����ļ���ʽ
    if(midiHeader.dwFlag!=MIDI_MThd)//��ʶ������"MThd"
        FMTERR;

    //ת��ΪLittle-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //�����ļ�ͷ
    switch(midiHeader.wType)
    {
		case MIDIT_SingleTrack:
			my_fputs("����:������\r\n",stdout);
			break;
		case MIDIT_MultiSync:
			my_fputs("����:ͬ��������\r\n",stdout);
			break;
		case MIDIT_MultiAsync:
			my_fputs("����:�첽������\r\n",stdout);
			break;
		default:
			my_fprintf(stdout,"����:δ֪(0x%04X)\n",midiHeader.wType);
			break;
    }

    //��ӡ����������Ϣ
    my_fprintf(stdout,
        "������:%u\r\n"
        "ÿ�ķ�����ʱ����:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //��ȷ��ת��MIDI�����λ�ã�����midiHeader.dwRestLen�����á���
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	f_lseek (&fdst, 8+midiHeader.dwRestLen);
	
	while (ReadAllTracksEnd != ((1<<midiHeader.wNbTracks)-1))
	{		
		//׼����ȡ��������
		for(TrackNumber=0;TrackNumber<midiHeader.wNbTracks;TrackNumber++)
		{
			DWORD dwTrackFlag;
			DWORD dwTrackLen;
			size_t TrackStartPos;
			

			//ÿ������Ŀ�ͷ����һ��dwTrackFlag��һ��dwTrackLen
			//dwTrackFlag��ֵ������MIDI_MTrk
			//dwTrackLen�������һ�������λ��

			if ((ReadAllTracksEnd & (1<<TrackNumber)) == (1<<TrackNumber) ||(MidiInfo[TrackNumber].EventDelay >0))  //�жϵ�ǰ�����Ƿ��Ѿ���ȡ����
			{
				continue;
			}
			
			/*
			if (MidiInfo[TrackNumber].EventDelay >0)
			{
				//MidiInfo[TrackNumber].EventDelay--;
				break;	
			}
			*/	
				
				
			if (MidiInfo[TrackNumber].TracksFirstReadFlag==0)
			{
			
				my_fputs("��ȡ����\r\n",stdout);

				READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//����ļ���ʽ
				READ(dwTrackLen);   TrackStartPos=f_tell(fp);//��¼��ǰλ��

				//ת��Big-Endian
				dwTrackLen=BSwapD(dwTrackLen);

				MidiInfo[TrackNumber].TracksFirstReadFlag=1;
				
				//��ʾÿ��������ֽ���
				my_fprintf(stdout,"�����С:%u �ֽ�\r\n",dwTrackLen);
			}
			else
			{
				//f_lseek (&fdst, TrackStartPos+MidiInfo[TrackNumber].CurrentEventPos);
				f_lseek (&fdst,MidiInfo[TrackNumber].CurrentEventPos);
			}
			//�������Ҫ�������¼�����
			//for(;;)//ѭ����ȡ�¼�
			do
			{
				DWORD dwDelay;
				BYTE bEvent;
				
				//ÿ���¼��Ĺ��ɶ��ǣ�
				//��ʱ���¼��ţ�����
				//���е���ʱ�Ƕ�̬�ֽڣ�������С���¼��Ŷ���

				//��һ���¼���
				BYTE bLastEvent[16];
				
				/*
				if (MidiInfo[TrackNumber].EventDelay >0)
				{
					//MidiInfo[TrackNumber].EventDelay--;
					break;	
				}
				*/
				
				
				//��ȡ��ʱ
				if(!ReadDynamicBytes(fp,&dwDelay))
					READERR;
				MidiInfo[TrackNumber].EventDelay= dwDelay;
/*
				if (MidiInfo[TrackNumber].EventDelay >0)
				{
					break;	
				}
				*/
				//��ȡ�¼���
				READ(bEvent);
				//ÿ��ȡһ���¼�����ӡ����¼���ʱ��
				my_fprintf(stdout,"ʱ��:%u,",dwDelay);

				//�����¼�
ProcEvent:  	if(bEvent <= 0x7F)
				{//0��0x7F:����һ���¼����¼�����ͬ������ȡ������ֽھ�����һ���¼��ŵĲ����ĵ�һ���ֽ�
					//fseek(fp,-1,SEEK_CUR);//����һ���ֽ�
					
					f_lseek(fp,f_tell(fp)-1);//����һ���ֽ�
			
					bEvent=bLastEvent[TrackNumber];
					goto ProcEvent;
				}
				else if(bEvent <= 0x8F)
				{//0x80��0x8F:�ɿ�����
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����

					//while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
					
					
					
					/*
					OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG | MIDI_AUTO_PLAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG | MIDI_STOP_PLAY_FLAG,
								(INT8U )OS_FLAG_WAIT_SET_ANY,
								(INT32U) 0,
								(INT8U *)&err);
					
					
					PauseMidiPlay();
					*/
					//MidiVolumeControl();
					
/*
					ReturnStatus=ScanOnlineDevice();											
					if (ReturnStatus)
					{	
						FMTERR;	
					}	
					*/	
					//MIDI_StopFlag =0;
					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					
				

					my_fprintf(stdout,"(ͨ��%u)�ɿ�����:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����					
				}
				else if(bEvent <= 0x9F)
				{//0x90��0x9F:��������
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����
					
					
					

					//MIDI_AutoPlayFlag=1; //����
					
					#if 0
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);
					
					#endif
					
					//if (MIDI_AutoPlayFlag)
						
					
	
					
					Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
									(INT8U *)&err) ;
					/*
					if ((OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
									(INT8U *)&err) & MIDI_AUTO_PLAY_FLAG)==MIDI_AUTO_PLAY_FLAG)
					*/
					
					if (((Flags & MIDI_AUTO_PLAY_FLAG)==MIDI_AUTO_PLAY_FLAG) || 
						((Flags & TRAMPLE_SPEEDING_FLAG)==TRAMPLE_SPEEDING_FLAG)) 
					
					{
						//MIDI_PlayFlag =0;
						//MIDI_StopFlag =0;
						
						OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);	
					}
					//else
					{	
						
						
						//while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
						
				
						/*
						OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
									(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG | MIDI_AUTO_PLAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG | MIDI_STOP_PLAY_FLAG,
									(INT8U )OS_FLAG_WAIT_SET_ANY,
									(INT32U) 0,
									(INT8U *)&err);
						*/
						//MidiVolumeControl();
						
						
						
						/*	
						ReturnStatus=ScanOnlineDevice();
						if (ReturnStatus)
						{	
							FMTERR;	
						}	
						*/
						
						if (bVel !=0x00)
						{
							//MIDI_PlayFlag =0;
							
							if (FirstReadTrack==1)
							{
								
								f_lseek (&fdst,MidiInfo[TrackNumber].CurrentEventPos);
								break;
								//goto FirstEndOfTrack;
							}
							
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);	

							Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
							
							
							//f_lseek(fp,CurrentPos+bBytes);//��ȡ��ɺ���ȷ������һ���¼���λ�á�
							//MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
							bLastEvent[TrackNumber]=bEvent;
							//goto FirstEndOfTrack;
							break;
							
							
							
							/*
							if (!Midi_En_dis)
							{
								goto BadReturn;
							}	
							*/
							
						}
						else
						{
							//MIDI_StopFlag=0;	

							//PauseMidiPlay();
							
							
							
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_STOP_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);
							
							Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
							
						}	
					}
					/*
					ReturnStatus=ScanOnlineDevice();
					if (ReturnStatus)
					{	
						FMTERR;	
					}	
					*/
					/*
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					*/
					
					
					my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����
					
				}
				else if(bEvent <= 0xAF)
				{//0xA0��0xAF:��������
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����

					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					
					
					
					my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����
					
				}
				else if(bEvent <= 0xBF)
				{//0xB0��0xBF:������
					BYTE bReg,bVal;

					READ(bReg);//�Ĵ�����
					READ(bVal);//�Ĵ���ֵ

					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bReg,  //�Ĵ�����
								 bVal); //�Ĵ���ֵ
					
					
					
					my_fprintf(stdout,"(ͨ��%u)������:%u,ֵ=%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bReg,//�Ĵ���
						bVal);//ֵ
					
				}
				else if(bEvent <= 0xCF)
				{//0xC0��0xCF:�ı�����
					BYTE bInstrument;

					READ(bInstrument);//������
					
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	//�¼�
								 bInstrument);  //������
					*/
					Sand_3Byte_MidiCmd(bEvent,	//�¼�
								 bInstrument,  //������
										0x00);

					my_fprintf(stdout,"(ͨ��%u)�ı�����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bInstrument);//������
					
				}
				else if(bEvent <= 0xDF)
				{//0xD0��0xDF:����ͨ��
					BYTE bChannel;

					READ(bChannel);//ͨ����
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	   //�¼�
									   bChannel);  //ͨ����
					*/
					Sand_3Byte_MidiCmd(bEvent,	   //�¼�
									   bChannel,  //ͨ����
										0x00);
					
					my_fprintf(stdout,"(ͨ��%u)����ͨ��:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bChannel);//ͨ����
					
				}
				else if(bEvent <= 0xEF)
				{//0xE0��0xEF:����
					WORD wPitch;//����������Little-Endian��WORD

					READ(wPitch);
					
					
					Sand_3Byte_MidiCmd(bEvent,	    //�¼�
									   wPitch,		//��������L
									   wPitch>>8); //��������H

					
					my_fprintf(stdout,"(ͨ��%u)����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						wPitch);//����
					
					
					
					
				}
				else if(bEvent == 0xF0)
				{//0xF0:ϵͳ��
					BYTE bSysCode=0;

					
					
					my_fputs("ϵͳ��:",stdout);
					
					
					for(;;)
					{
						READ(bSysCode);
						
						if(bSysCode!=0xF7)
						{	
						
							/*
							Sand_2Byte_MidiCmd(bEvent,	    //�¼�
											   bSysCode);   //ϵͳ��
						
							*/
						
							my_fprintf(stdout,"0x%02X,",bSysCode);
							
							
						}
						else
						{
							/*
							Sand_2Byte_MidiCmd(bEvent,         //�¼�
											   bSysCode);      //ϵͳ��
							*/
							
							my_fprintf(stdout,"0x%02X\r\n",bSysCode);
							
							break;//��ȡ��0xF7����
						}
					}
				}
				else if(bEvent == 0xFF)  
				{//Ԫ�¼�
					BYTE bType,bBytes;
					size_t CurrentPos;
					
					READ(bType);//Ԫ��������
					READ(bBytes);//Ԫ�����ֽ���

					/*
					Sand_3Byte_MidiCmd(bEvent,      //�¼�
									   bType,		//Ԫ��������
									   bBytes);		//Ԫ�����ֽ���
					*/	
					CurrentPos=f_tell(fp);//��¼Ԫ���ݶ�ȡ��λ��

					
					my_fputs("Ԫ���� - ",stdout);
					
					switch(bType)
					{
					case 0x00://���ù������
						{
							WORD wTrackSeq;
							READ(wTrackSeq);
							wTrackSeq=BSwapW(wTrackSeq);
							
				
							my_fprintf(stdout,"���ù������:0x%04X\r\n",wTrackSeq);
							
							
						}
						break;
					case 0x01://������ע
						my_fputs("��ע:",stdout);
					
						
					
						ReadStringAndPrint(fp,bBytes);
					
					
						my_fputc('\n',stdout);
					
					
					
						break;
					case 0x02://��Ȩ
						my_fputs("��Ȩ:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						
						break;
					case 0x03://��������
						my_fputs("����:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x04://��������
						my_fputs("������:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x05://���
						my_fputs("���:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x06://���
						my_fputs("���:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x07://��ʼ��
						my_fputs("��ʼ��:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x21://���쿪ʼ��ʶ
						my_fputs("���쿪ʼ��ʶ.\n",stdout);
						break;
					case 0x2F://���������ʶ
						my_fputs("���������ʶ.\n",stdout);
					
					
						ReadAllTracksEnd |= (1<<TrackNumber);
					
					    //MidiInfo[TrackNumber].TracksEndFlag = TRACK_END;
						//break;	
						goto EndOfTrack;
					
					case 0x51://�ٶ�
						{
							BYTE bVelocity1,bVelocity2,bVelocity3;

							//�ٶ���24λ������һ���ķ�������΢������
							READ(bVelocity1);
							READ(bVelocity2);
							READ(bVelocity3);

							//������Big-Endian
							 my_fprintf(stdout,"�ٶ�: ÿ�ķ�����%u΢��.\r\n",
								bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
							
							UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
							
							
						}
						break;
					case 0x58://����
						{
							BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

							READ(bBeatNumerator);//����
							READ(bBeatDenominator);//��ĸ
							READ(bMetronomeTimer);//������ʱ��
							READ(bNb32ndNotesInCrotchet);//ÿ�ķ������ж�����ʮ��������
							
							Denominator = bBeatDenominator;
							
							my_fprintf(stdout,"����:%u/%u. ������ʱ��:%u. ÿ�ķ�������%u����ʮ��������.\r\n",
								bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
						}
						break;
					case 0x59://����
						{
							MIDIInt8 bTunePitch;
							BYTE bTuneType;

							READ(bTunePitch);//������
							READ(bTuneType);//�����С��

							my_fprintf(stdout,"������:%d, ",bTunePitch);
							switch(bTuneType)
							{
							case 0://���
								my_fputs("���\r\n",stdout);
								break;
							case 1://С��
								my_fputs("С��\r\n",stdout);
								break;
							default://��֪��
								my_fprintf(stdout,"δ֪��(0x%02X).\r\n",bTuneType);
								break;
							}
						}
						break;
					case 0x7F://�����ض���Ϣ
						my_fputs("�����ض���Ϣ:",stdout);
						ReadStringAndPrint(fp,bBytes);
					
						
						my_fputc('\r',stdout);
						
					
						break;
					default:
						
						
						my_fprintf(stdout,"δ֪Ԫ��������(0x%02X)\r\n",bType);
						
					
						break;
					}
					
			
					my_printf("\r\n");

					f_lseek(fp,CurrentPos+bBytes);//��ȡ��ɺ���ȷ������һ���¼���λ�á�
					
					
					MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
					//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
					
					
					
					// = CurrentPos+bBytes;
					
					
				}
				else//�����¼���δ֪�¼�
				{
					
					#ifdef NDEBUG
					my_fprintf(stdout,"δ֪�¼�:0x%08X. ֹͣ��ǰ����ķ�����\r\n",bEvent);
					#endif
					
					break;
				}
FirstEndOfTrack:
				//��¼��һ���¼���
				bLastEvent[TrackNumber]=bEvent;
				
				MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
			}while ((MidiInfo[TrackNumber].EventDelay ==0) || (midiHeader.wType==MIDIT_MultiAsync));//��ȥ������ȡ�¼���
		

 
			
			
EndOfTrack:
		
			MidiInfo[TrackNumber].CurrentEventPos =f_tell(fp);
			

			
			//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
			
			
			
			f_lseek(fp,TrackStartPos+dwTrackLen);//ת����һ������
			
			
		}
		
		FirstReadTrack=0;

		/*
		DWORD Delay;
		Delay = (UsDelay+1)/midiHeader.wTicksPerCrotchet;
		Delay_us(Delay+1,INT_DELAY);

		
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_DELAY_FLAG,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);

		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_DELAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ANY,
					(INT32U) 0,
					(INT8U *)&err);
		
		*/
		
		
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_FLAG | 
							   MIDI_STOP_FLAG | 
							   MIDI_AUTO_PLAY_FLAG | 
							   DEVICE_CHANGE_FLAG | 
							   MIDI_PLAY_EN_DIS_FLAG | 
							   MIDI_STOP_PLAY_FLAG|
							   TRAMPLE_SPEEDING_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ANY,
					(INT32U) 0,
					(INT8U *)&err);
		

		Flags =OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
	
		DWORD  MinDelay=0xffffffff;
		
		if ((Flags & (MIDI_PLAY_FLAG)) == (MIDI_PLAY_FLAG))
		{
			/*
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_PLAY_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);	
			*/
			//PauseMidiPlay();
			
			
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>0)
				{
					MinDelay = (MidiInfo[TrackNumber].EventDelay < MinDelay) ? MidiInfo[TrackNumber].EventDelay:MinDelay;
				}	
			}
			
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>=MinDelay)
				{
					MidiInfo[TrackNumber].EventDelay-=MinDelay;
				}
			}
				
			
		}	
		if ((Flags & (MIDI_STOP_PLAY_FLAG)) == (MIDI_STOP_PLAY_FLAG))
		{
				OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_STOP_PLAY_FLAG,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);
			
			
			PauseMidiPlay();
		}
		
		else
		{
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>0)
				{
					MidiInfo[TrackNumber].EventDelay--;
				}	
			}
			
			
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_DELAY_FLAG,
						(INT8U )OS_FLAG_CLR,
						(INT8U *)&err);
			
			DWORD Delay;
			Delay = (UsDelay+1)/midiHeader.wTicksPerCrotchet;
			
			
			
            switch(Denominator) 
            {
                case 0:Delay*=4;break;  // x/1
                case 1:Delay*=2;break;  // x/2
                case 3:Delay/=2;break;  // x/8
				case 4:Delay/=4;break;  // x/16
				case 5:Delay/=8;break;  // x/32
				case 6:Delay/=16;break; // x/64
                case 2:
                default :break;    
            }
			
 			Delay_us(Delay+1,INT_DELAY);


			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_DELAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG,
						(INT8U )OS_FLAG_WAIT_SET_ANY,
						(INT32U) 0,
						(INT8U *)&err);
		
		}
		/*
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_PLAY_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);	
*/
		ReturnStatus=ScanOnlineDevice();
		if (ReturnStatus)
		{	
			FMTERR;	
		}				
	}
    my_fputs("MIDI�ļ���ȡ������\r\n",stdout);
    f_close(fp);
	
	ReturnStatus=MIDI_ERR_READ_FILE_END;
	
    return ReturnStatus;
BadReturn:
    f_close(fp);
	
	
    return ReturnStatus;
#   undef READ
#   undef READERR
}
#endif





#if 1
//=============================================================================
//ParseMIDI:
//����MIDI�ļ���ʧ�ܷ����㣬�ɹ����ط���
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"��ȡ�ļ�%s��������\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s����һ��������MIDI�ļ�\r\n",pszFileName);goto BadReturn;}
	#define READ(x) if(f_read(fp,&x,sizeof(x),&br)!=FR_OK)READERR

	//int res;
    //FIL *fsrc,fdst;
	
	FIL *fp = &fdst;
	
    MIDIHeader midiHeader;
    volatile u16 TrackNumber;
	
	u8 ReturnStatus= MIDI_ERR_NONE;
	
	OS_FLAGS Flags;
	
	DWORD  MinDelay=0xffffffff;
	DWORD Delay;
	//size_t CurrentEventPos[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//DWORD EventDelay[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	
	WORD ReadAllTracksEnd =0;
	//BYTE Denominator;
	u8 FirstReadTrack=1;
	/*
	MIDIINFO MidiInfo[16]=
	{
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},		
	};
	*/
	
	INT8U err;
	
	memset(MidiInfo,0,sizeof(MidiInfo));
	
    //���ļ�
	res = f_open(fp,pszFileName,FA_READ);
    if (res != FR_OK)
        READERR;

    //MIDI�ļ�ͷ����һ��MIDIHeader�ṹ�塣
    //����Ҫע�����е���ֵ����Big-Endian�洢��
    //��Ҫ����ת��

    //��ȡMIDI�ļ�ͷ
    READ(midiHeader);
    
    //����ļ���ʽ
    if(midiHeader.dwFlag!=MIDI_MThd)//��ʶ������"MThd"
        FMTERR;

    //ת��ΪLittle-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //�����ļ�ͷ
    switch(midiHeader.wType)
    {
		case MIDIT_SingleTrack:
			my_fputs("����:������\r\n",stdout);
			break;
		case MIDIT_MultiSync:
			my_fputs("����:ͬ��������\r\n",stdout);
			break;
		case MIDIT_MultiAsync:
			my_fputs("����:�첽������\r\n",stdout);
			break;
		default:
			my_fprintf(stdout,"����:δ֪(0x%04X)\n",midiHeader.wType);
			break;
    }

    //��ӡ����������Ϣ
    my_fprintf(stdout,
        "������:%u\r\n"
        "ÿ�ķ�����ʱ����:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //��ȷ��ת��MIDI�����λ�ã�����midiHeader.dwRestLen�����á���
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	f_lseek (fp, 8+midiHeader.dwRestLen);
	
	while (ReadAllTracksEnd != ((1<<midiHeader.wNbTracks)-1))
	{		
		//׼����ȡ��������
		for(TrackNumber=0;TrackNumber<midiHeader.wNbTracks;TrackNumber++)
		{
			DWORD dwTrackFlag;
			DWORD dwTrackLen;
			size_t TrackStartPos;
			

			//ÿ������Ŀ�ͷ����һ��dwTrackFlag��һ��dwTrackLen
			//dwTrackFlag��ֵ������MIDI_MTrk
			//dwTrackLen�������һ�������λ��

			if ((ReadAllTracksEnd & (1<<TrackNumber)) == (1<<TrackNumber) ||
                (MidiInfo[TrackNumber].EventDelay >0))  //�жϵ�ǰ�����Ƿ��Ѿ���ȡ����
			{
				continue;
			}
			
			/*
			if (MidiInfo[TrackNumber].EventDelay >0)
			{
				//MidiInfo[TrackNumber].EventDelay--;
				break;	
			}
			*/	
				
				
			if (MidiInfo[TrackNumber].TracksFirstReadFlag==0)
			{
			
				my_fputs("��ȡ����\r\n",stdout);

				READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//����ļ���ʽ
				READ(dwTrackLen);   TrackStartPos=f_tell(fp);//��¼��ǰλ��

				//ת��Big-Endian
				dwTrackLen=BSwapD(dwTrackLen);

				MidiInfo[TrackNumber].TracksFirstReadFlag=1;
				
				//��ʾÿ��������ֽ���
				my_fprintf(stdout,"�����С:%u �ֽ�\r\n",dwTrackLen);
			}
			else
			{
				
				//WORK_LED(0);
				
				//f_lseek (&fdst, TrackStartPos+MidiInfo[TrackNumber].CurrentEventPos);
				f_lseek (fp,MidiInfo[TrackNumber].CurrentEventPos);
				
				//WORK_LED(1);	
			}
			//�������Ҫ�������¼�����
			//for(;;)//ѭ����ȡ�¼�
			do
			{
				DWORD dwDelay;
				BYTE bEvent;
				
				//ÿ���¼��Ĺ��ɶ��ǣ�
				//��ʱ���¼��ţ�����
				//���е���ʱ�Ƕ�̬�ֽڣ�������С���¼��Ŷ���

				//��һ���¼���
				BYTE bLastEvent[16];
				
				/*
				if (MidiInfo[TrackNumber].EventDelay >0)
				{
					//MidiInfo[TrackNumber].EventDelay--;
					break;	
				}
				*/
				
				if (MidiInfo[TrackNumber].TracksDelayFlag ==0)
                {            
    				//��ȡ��ʱ
    				if(!ReadDynamicBytes(fp,&dwDelay))
    					READERR;
    				MidiInfo[TrackNumber].EventDelay= dwDelay;
                    
    				if (MidiInfo[TrackNumber].EventDelay >0)
    				{
    					MidiInfo[TrackNumber].TracksDelayFlag =1;
                        //MidiInfo[TrackNumber].CurrentEventPos =f_tell(fp);  
    					break;	
    				}
				}
                else
                {
                    MidiInfo[TrackNumber].TracksDelayFlag =0;
                }
				//��ȡ�¼���
				READ(bEvent);
				//ÿ��ȡһ���¼�����ӡ����¼���ʱ��
				my_fprintf(stdout,"ʱ��:%u,",dwDelay);

				//�����¼�
ProcEvent:  	if(bEvent <= 0x7F)
				{//0��0x7F:����һ���¼����¼�����ͬ������ȡ������ֽھ�����һ���¼��ŵĲ����ĵ�һ���ֽ�
					//fseek(fp,-1,SEEK_CUR);//����һ���ֽ�
					
					f_lseek(fp,f_tell(fp)-1);//����һ���ֽ�
			
					bEvent=bLastEvent[TrackNumber];
					goto ProcEvent;
				}
				else if(bEvent <= 0x8F)
				{//0x80��0x8F:�ɿ�����
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����

					//while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
					
					
					
					/*
					OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG | MIDI_AUTO_PLAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG | MIDI_STOP_PLAY_FLAG,
								(INT8U )OS_FLAG_WAIT_SET_ANY,
								(INT32U) 0,
								(INT8U *)&err);
					
					
					PauseMidiPlay();
					*/
					//MidiVolumeControl();
					
/*
					ReturnStatus=ScanOnlineDevice();											
					if (ReturnStatus)
					{	
						FMTERR;	
					}	
					*/	
					//MIDI_StopFlag =0;
					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					
				

					my_fprintf(stdout,"(ͨ��%u)�ɿ�����:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����					
				}
				else if(bEvent <= 0x9F)
				{//0x90��0x9F:��������
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����
					
					
					

					//MIDI_AutoPlayFlag=1; //����
					
					#if 0
					OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_AUTO_PLAY_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);
					
					#endif
					
					//if (MIDI_AutoPlayFlag)
						
					
	
					
					Flags = OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
									(INT8U *)&err) ;
					/*
					if ((OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
									(INT8U *)&err) & MIDI_AUTO_PLAY_FLAG)==MIDI_AUTO_PLAY_FLAG)
					*/
					
					if (((Flags & MIDI_AUTO_PLAY_FLAG)==MIDI_AUTO_PLAY_FLAG) || 
						((Flags & TRAMPLE_SPEEDING_FLAG)==TRAMPLE_SPEEDING_FLAG)) 
					
					{
						//MIDI_PlayFlag =0;
						//MIDI_StopFlag =0;
						
						OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG,
								(INT8U) OS_FLAG_CLR,
								(INT8U  *)&err);	
					}
					//else
					{	
						
						
						//while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
						
				
						/*
						OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
									(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG | MIDI_AUTO_PLAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG | MIDI_STOP_PLAY_FLAG,
									(INT8U )OS_FLAG_WAIT_SET_ANY,
									(INT32U) 0,
									(INT8U *)&err);
						*/
						//MidiVolumeControl();
						
						
						
						/*	
						ReturnStatus=ScanOnlineDevice();
						if (ReturnStatus)
						{	
							FMTERR;	
						}	
						*/
						
						if (bVel !=0x00)
						{
							//MIDI_PlayFlag =0;
							
							if (FirstReadTrack==1)
							{
								
								f_lseek (fp,MidiInfo[TrackNumber].CurrentEventPos);
								break;
								//goto FirstEndOfTrack;
							}
							
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_PLAY_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);	

							Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
							
							
							//f_lseek(fp,CurrentPos+bBytes);//��ȡ��ɺ���ȷ������һ���¼���λ�á�
							//MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
							bLastEvent[TrackNumber]=bEvent;
							//goto FirstEndOfTrack;
							break;
							
							
							
							/*
							if (!Midi_En_dis)
							{
								goto BadReturn;
							}	
							*/
							
						}
						else
						{
							//MIDI_StopFlag=0;	

							//PauseMidiPlay();
							
							
							
							OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
										(OS_FLAGS) MIDI_STOP_FLAG,
										(INT8U) OS_FLAG_CLR,
										(INT8U  *)&err);
							
							Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
							
						}	
					}
					/*
					ReturnStatus=ScanOnlineDevice();
					if (ReturnStatus)
					{	
						FMTERR;	
					}	
					*/
					/*
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					*/
					
					
					my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����
					
				}
				else if(bEvent <= 0xAF)
				{//0xA0��0xAF:��������
					BYTE bNote,bVel;

					READ(bNote);//����
					READ(bVel); //����

					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bNote, //����
								 bVel); //����
					
					
					
					my_fprintf(stdout,"(ͨ��%u)��������:%s, ����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						NoteToString(bNote),//����
						bVel);//����
					
				}
				else if(bEvent <= 0xBF)
				{//0xB0��0xBF:������
					BYTE bReg,bVal;

					READ(bReg);//�Ĵ�����
					READ(bVal);//�Ĵ���ֵ

					
					Sand_3Byte_MidiCmd(bEvent,//�¼�
								 bReg,  //�Ĵ�����
								 bVal); //�Ĵ���ֵ
					
					
					
					my_fprintf(stdout,"(ͨ��%u)������:%u,ֵ=%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bReg,//�Ĵ���
						bVal);//ֵ
					
				}
				else if(bEvent <= 0xCF)
				{//0xC0��0xCF:�ı�����
					BYTE bInstrument;

					READ(bInstrument);//������
					
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	//�¼�
								 bInstrument);  //������
					*/
					Sand_3Byte_MidiCmd(bEvent,	//�¼�
								 bInstrument,  //������
										0x00);

					my_fprintf(stdout,"(ͨ��%u)�ı�����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bInstrument);//������
					
				}
				else if(bEvent <= 0xDF)
				{//0xD0��0xDF:����ͨ��
					BYTE bChannel;

					READ(bChannel);//ͨ����
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	   //�¼�
									   bChannel);  //ͨ����
					*/
					Sand_3Byte_MidiCmd(bEvent,	   //�¼�
									   bChannel,  //ͨ����
										0x00);
					
					my_fprintf(stdout,"(ͨ��%u)����ͨ��:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						bChannel);//ͨ����
					
				}
				else if(bEvent <= 0xEF)
				{//0xE0��0xEF:����
					WORD wPitch;//����������Little-Endian��WORD

					READ(wPitch);
					
					
					Sand_3Byte_MidiCmd(bEvent,	    //�¼�
									   wPitch,		//��������L
									   wPitch>>8); //��������H

					
					my_fprintf(stdout,"(ͨ��%u)����:%u\r\n",
						bEvent & 0xF,//��4λ��ʾͨ����
						wPitch);//����
					
					
					
					
				}
				else if(bEvent == 0xF0)
				{//0xF0:ϵͳ��
					BYTE bSysCode=0;

					
					
					my_fputs("ϵͳ��:",stdout);
					
					
					for(;;)
					{
						READ(bSysCode);
						
						if(bSysCode!=0xF7)
						{	
						
							/*
							Sand_2Byte_MidiCmd(bEvent,	    //�¼�
											   bSysCode);   //ϵͳ��
						
							*/
						
							my_fprintf(stdout,"0x%02X,",bSysCode);
							
							
						}
						else
						{
							/*
							Sand_2Byte_MidiCmd(bEvent,         //�¼�
											   bSysCode);      //ϵͳ��
							*/
							
							my_fprintf(stdout,"0x%02X\r\n",bSysCode);
							
							break;//��ȡ��0xF7����
						}
					}
				}
				else if(bEvent == 0xFF)  
				{//Ԫ�¼�
					BYTE bType,bBytes;
					size_t CurrentPos;
					
					READ(bType);//Ԫ��������
					READ(bBytes);//Ԫ�����ֽ���

					/*
					Sand_3Byte_MidiCmd(bEvent,      //�¼�
									   bType,		//Ԫ��������
									   bBytes);		//Ԫ�����ֽ���
					*/	
					CurrentPos=f_tell(fp);//��¼Ԫ���ݶ�ȡ��λ��

					
					my_fputs("Ԫ���� - ",stdout);
					
					switch(bType)
					{
					case 0x00://���ù������
						{
							WORD wTrackSeq;
							READ(wTrackSeq);
							wTrackSeq=BSwapW(wTrackSeq);
							
				
							my_fprintf(stdout,"���ù������:0x%04X\r\n",wTrackSeq);
							
							
						}
						break;
					case 0x01://������ע
						my_fputs("��ע:",stdout);
					
						
					
						ReadStringAndPrint(fp,bBytes);
					
					
						my_fputc('\n',stdout);
					
					
					
						break;
					case 0x02://��Ȩ
						my_fputs("��Ȩ:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						
						break;
					case 0x03://��������
						my_fputs("����:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x04://��������
						my_fputs("������:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x05://���
						my_fputs("���:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x06://���
						my_fputs("���:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x07://��ʼ��
						my_fputs("��ʼ��:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x21://���쿪ʼ��ʶ
						my_fputs("���쿪ʼ��ʶ.\n",stdout);
						break;
					case 0x2F://���������ʶ
						my_fputs("���������ʶ.\n",stdout);
					
					
						ReadAllTracksEnd |= (1<<TrackNumber);
					
					    //MidiInfo[TrackNumber].TracksEndFlag = TRACK_END;
						//break;	
						goto EndOfTrack;
					
					case 0x51://�ٶ�
						{
							BYTE bVelocity1,bVelocity2,bVelocity3;

							//�ٶ���24λ������һ���ķ�������΢������
							READ(bVelocity1);
							READ(bVelocity2);
							READ(bVelocity3);

							//������Big-Endian
							 my_fprintf(stdout,"�ٶ�: ÿ�ķ�����%u΢��.\r\n",
								bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
							
							UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
							
							
						}
						break;
					case 0x58://����
						{
							BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

							READ(bBeatNumerator);//����
							READ(bBeatDenominator);//��ĸ
							READ(bMetronomeTimer);//������ʱ��
							READ(bNb32ndNotesInCrotchet);//ÿ�ķ������ж�����ʮ��������
							
							//Denominator = bBeatDenominator;
							
							my_fprintf(stdout,"����:%u/%u. ������ʱ��:%u. ÿ�ķ�������%u����ʮ��������.\r\n",
								bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
						}
						break;
					case 0x59://����
						{
							MIDIInt8 bTunePitch;
							BYTE bTuneType;

							READ(bTunePitch);//������
							READ(bTuneType);//�����С��

							my_fprintf(stdout,"������:%d, ",bTunePitch);
							switch(bTuneType)
							{
							case 0://���
								my_fputs("���\r\n",stdout);
								break;
							case 1://С��
								my_fputs("С��\r\n",stdout);
								break;
							default://��֪��
								my_fprintf(stdout,"δ֪��(0x%02X).\r\n",bTuneType);
								break;
							}
						}
						break;
					case 0x7F://�����ض���Ϣ
						my_fputs("�����ض���Ϣ:",stdout);
						ReadStringAndPrint(fp,bBytes);
					
						
						my_fputc('\r',stdout);
						
					
						break;
					default:
						
						
						my_fprintf(stdout,"δ֪Ԫ��������(0x%02X)\r\n",bType);
						
					
						break;
					}
					
			
					my_printf("\r\n");

					f_lseek(fp,CurrentPos+bBytes);//��ȡ��ɺ���ȷ������һ���¼���λ�á�
					
					
					MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
					//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
					
					
					
					// = CurrentPos+bBytes;
					
					
				}
				else//�����¼���δ֪�¼�
				{
					
					#ifdef NDEBUG
					my_fprintf(stdout,"δ֪�¼�:0x%08X. ֹͣ��ǰ����ķ�����\r\n",bEvent);
					#endif
					
					break;
				}
//FirstEndOfTrack:
				//��¼��һ���¼���
				bLastEvent[TrackNumber]=bEvent;
				
				MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
			}while ((MidiInfo[TrackNumber].EventDelay ==0) || (midiHeader.wType==MIDIT_MultiAsync));//��ȥ������ȡ�¼���
			//}while ((midiHeader.wType==MIDIT_MultiAsync));//��ȥ������ȡ�¼���

 
			
			
EndOfTrack:
		
			MidiInfo[TrackNumber].CurrentEventPos =f_tell(fp);
			

			
			//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
			
			
			if (FirstReadTrack==1)
			{
				f_lseek(fp,TrackStartPos+dwTrackLen);//ת����һ������
			}
			
		}
		
		FirstReadTrack=0;

		/*
		DWORD Delay;
		Delay = (UsDelay+1)/midiHeader.wTicksPerCrotchet;
		Delay_us(Delay+1,INT_DELAY);

		
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_DELAY_FLAG,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);

		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_DELAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ANY,
					(INT32U) 0,
					(INT8U *)&err);
		
		*/
		
		
		OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_FLAG | 
							   MIDI_STOP_FLAG | 
							   MIDI_AUTO_PLAY_FLAG | 
							   DEVICE_CHANGE_FLAG | 
							   MIDI_PLAY_EN_DIS_FLAG | 
							   MIDI_STOP_PLAY_FLAG|
							   TRAMPLE_SPEEDING_FLAG,
					(INT8U )OS_FLAG_WAIT_SET_ANY,
					(INT32U) 0,
					(INT8U *)&err);
		



		Flags =OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
							(INT8U *)&err);
							
		MinDelay=0xffffffff;
		
		
		if (((Flags & (MIDI_PLAY_FLAG)) == (MIDI_PLAY_FLAG)) 
			&& ((Flags & (MIDI_AUTO_PLAY_FLAG)) != (MIDI_AUTO_PLAY_FLAG))
			&& ((Flags & (TRAMPLE_SPEEDING_FLAG)) != (TRAMPLE_SPEEDING_FLAG)))
		{
			/*
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_PLAY_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);	
			*/
			//PauseMidiPlay();
			
			
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>0)
				{
					MinDelay = (MidiInfo[TrackNumber].EventDelay < MinDelay) ? MidiInfo[TrackNumber].EventDelay:MinDelay;
				}	
			}
			
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>=MinDelay)
				{
					MidiInfo[TrackNumber].EventDelay-=MinDelay;
				}
			}	
		}
		
		if ((Flags & (MIDI_STOP_PLAY_FLAG)) == (MIDI_STOP_PLAY_FLAG))
		{
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) MIDI_STOP_PLAY_FLAG,
				(INT8U )OS_FLAG_CLR,
				(INT8U *)&err);
			
			
			PauseMidiPlay();
		}
		else 
		{
			for (TrackNumber=0;TrackNumber < midiHeader.wNbTracks; TrackNumber++)
			{
				if (MidiInfo[TrackNumber].EventDelay>0)
				{
					MidiInfo[TrackNumber].EventDelay--;
				}
                /*
                else
                {
                    MidiInfo[TrackNumber].TracksDelayFlag =0;
                } 
                */
			}
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_DELAY_FLAG,
						(INT8U )OS_FLAG_CLR,
						(INT8U *)&err);
			
			
			Delay = (UsDelay+1)/midiHeader.wTicksPerCrotchet;
			
			
			/*
            switch(Denominator) 
            {
                case 0:Delay*=4;break;  // x/1
                case 1:Delay*=2;break;  // x/2
                case 3:Delay/=2;break;  // x/8
				case 4:Delay/=4;break;  // x/16
				case 5:Delay/=8;break;  // x/32
				case 6:Delay/=16;break; // x/64
                case 2:
                default :break;    
            }
			*/
			
			//WORK_LED(0);
 			Delay_us(Delay+1,INT_DELAY);
			//WORK_LED(0);

			OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_DELAY_FLAG | DEVICE_CHANGE_FLAG | MIDI_PLAY_EN_DIS_FLAG,
						(INT8U )OS_FLAG_WAIT_SET_ANY,
						(INT32U) 0,
						(INT8U *)&err);
			//WORK_LED(0);
			//WORK_LED(1);
			
		    //OSTimeDlyHMSM(0, 0,0,2);
		}

		/*
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) MIDI_PLAY_FLAG,
						(INT8U) OS_FLAG_CLR,
						(INT8U  *)&err);	
*/
		ReturnStatus=ScanOnlineDevice();
		if (ReturnStatus)
		{	
			FMTERR;	
		}				
	}
    my_fputs("MIDI�ļ���ȡ������\r\n",stdout);
    f_close(fp);
	
	ReturnStatus=MIDI_ERR_READ_FILE_END;
	
    return ReturnStatus;
BadReturn:
    f_close(fp);
	
	
    return ReturnStatus;
#   undef READ
#   undef READERR
}
#endif














#if 0
//=============================================================================
//NoteToString:
//����������ת��Ϊ�ַ���
//��ʵÿ�η��صĶ���ͬһ��������ֻ��ÿ�ζ���д�˻�������ֵ��
//-----------------------------------------------------------------------------
char*NoteToString(BYTE bNote)
{
    static char szBuf[5];//����5�ַ�
    switch(bNote % 12)
    {
    case 0:
        sprintf(szBuf,"C%d",(int)(bNote / 12)-2);
        break;
    case 1:
        sprintf(szBuf,"C%d#",(int)(bNote / 12)-2);
        break;
    case 2:
        sprintf(szBuf,"D%d",(int)(bNote / 12)-2);
        break;
    case 3:
        sprintf(szBuf,"D%d#",(int)(bNote / 12)-2);
        break;
    case 4:
        sprintf(szBuf,"E%d",(int)(bNote / 12)-2);
        break;
    case 5:
        sprintf(szBuf,"F%d",(int)(bNote / 12)-2);
        break;
    case 6:
        sprintf(szBuf,"F%d#",(int)(bNote / 12)-2);
        break;
    case 7:
        sprintf(szBuf,"G%d",(int)(bNote / 12)-2);
        break;
    case 8:
        sprintf(szBuf,"G%d#",(int)(bNote / 12)-2);
        break;
    case 9:
        sprintf(szBuf,"A%d",(int)(bNote / 12)-2);
        break;
    case 10:
        sprintf(szBuf,"A%d#",(int)(bNote / 12)-2);
        break;
    case 11:
        sprintf(szBuf,"B%d",(int)(bNote / 12)-2);
        break;
    }
    return szBuf;
}
#endif

#if 1
//=============================================================================
//NoteToString:
//����������ת��Ϊ�ַ���
//��ʵÿ�η��صĶ���ͬһ��������ֻ��ÿ�ζ���д�˻�������ֵ��
//-----------------------------------------------------------------------------
char*NoteToString(BYTE bNote)
{
    static char szBuf[5];//����5�ַ�
    switch(bNote % 12)
    {
    case 0:
        sprintf(szBuf,"C%d",(int)(bNote / 12)-0);
        break;
    case 1:
        sprintf(szBuf,"C%d#",(int)(bNote / 12)-0);
        break;
    case 2:
        sprintf(szBuf,"D%d",(int)(bNote / 12)-0);
        break;
    case 3:
        sprintf(szBuf,"D%d#",(int)(bNote / 12)-0);
        break;
    case 4:
        sprintf(szBuf,"E%d",(int)(bNote / 12)-0);
        break;
    case 5:
        sprintf(szBuf,"F%d",(int)(bNote / 12)-0);
        break;
    case 6:
        sprintf(szBuf,"F%d#",(int)(bNote / 12)-0);
        break;
    case 7:
        sprintf(szBuf,"G%d",(int)(bNote / 12)-0);
        break;
    case 8:
        sprintf(szBuf,"G%d#",(int)(bNote / 12)-0);
        break;
    case 9:
        sprintf(szBuf,"A%d",(int)(bNote / 12)-0);
        break;
    case 10:
        sprintf(szBuf,"A%d#",(int)(bNote / 12)-0);
        break;
    case 11:
        sprintf(szBuf,"B%d",(int)(bNote / 12)-0);
        break;
    }
    return szBuf;
}
#endif
//=============================================================================
//ReadStringAndPrint
//��ȡ�ַ���Ȼ���ӡ�����ض�ȡ���ֽ���
//-----------------------------------------------------------------------------
size_t ReadStringAndPrint(FIL*fp,size_t szLength)
{
    char*pBuf;
    pBuf=(char*)malloc(szLength+1);
    if(pBuf)//�����ڴ�һ�ζ�ȡ
    {
        //size_t Ret=fread(pBuf,1,szLength,fp);
		
		size_t Ret=f_read(fp,pBuf,szLength,&br);
		
        pBuf[szLength]=0;//��β��\0
        my_fputs(pBuf,stdout);
        free(pBuf);
        return Ret;
    }
    else
    {
        my_fputs("�޷����ַ��������ڴ档\r\n",stderr);
        return 0;
    }
}



u8 ReadMidiFile(char* Path)
{ 
	/*
	DIR dir;
	FILINFO filinfo;
	FATFS fs;
	*/
	INT8U  err;
	
	volatile FRESULT res;
	
	u8 RetuenStatus=MIDI_ERR_NONE;
	
	const char *FolderName="midfile";
    char *PathBuff=(char*)malloc(strlen(Path)+1);
	size_t Length=strlen(Path);

	
	
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) DEVICE_CHANGE_FLAG,
				(INT8U) OS_FLAG_CLR,
				(INT8U  *)&err);
	
	//f_mount(NULL, Path ,0);
	
	if(f_mount(&fs, Path, 0) == FR_OK) 
	{
		strcpy(PathBuff,Path);
		
		Length +=strlen(FolderName);
		
		PathBuff=(char*)realloc(PathBuff,Length+1);
		
		strcpy(strchr(PathBuff, '\0'),FolderName);
		
		//if (f_opendir (&dir,PathBuff)== FR_OK)
		
		res =f_opendir (&dir,PathBuff);	
		if (res== FR_OK)
		
		{
			PathBuff[Length]='/';
			while(1)
			{
				res =f_readdir (&dir, &filinfo);
				if (filinfo.fname[0] != '\0')
				{ 				
					if (strcmp(strchr((char*)filinfo.fname, '.')+1,"MID") ==0)	
					{	
						PathBuff=(char*)realloc(PathBuff,Length + strlen(filinfo.fname)+1);
						strcpy(&PathBuff[Length+1],filinfo.fname);
						
						
						if(PathBuff[0]==USBH_Path[0])
						{	
							ActivityDevice = U_DISK_ID;
						}
						else if(PathBuff[0]==SD_Path[0])	
						{
							
							ActivityDevice = SDMMC_ID;
						}
						
						RetuenStatus = ParseMIDI(PathBuff);
						
						if (RetuenStatus!=MIDI_ERR_READ_FILE_END)
						{
							
							PauseMidiPlay();
							break;
						}	
					}									
				}
				else
				{
					RetuenStatus = MIDI_ERR_READ_FOLDER_END ;
					break;
				}	
			}
		}
		free(PathBuff);	
	}
	return RetuenStatus;
}
	

void PauseMidiPlay(void)
{
	
	u8 TrackCnt;
	for (TrackCnt=0; TrackCnt<16;TrackCnt++)
	{
		Sand_3Byte_MidiCmd(0xE0 | TrackCnt,	    //�¼�
						   0x00,				//��������L
						   0x40); 				//��������H
		
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //�¼�
						   0x01,				//����������
						   0x00); 				//����������
		
			
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //�¼�
						   0x40,				//����������
						   0x00); 			    //����������
		
			
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //�¼�
						   0x79,				//����������
						   0x00); 				//����������		
	}	
}








