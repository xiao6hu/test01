
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
//读取动态字节，最多读取4字节。返回读取的字节数
//-----------------------------------------------------------------------------
//事件     音符      力度
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
//读取动态字节，最多读取4字节。返回读取的字节数
//-----------------------------------------------------------------------------
size_t ReadDynamicBytes(FIL*fp,DWORD*pOut)
{
    size_t bBytesRead;//已读取的字节数
    *pOut=0;
    for(bBytesRead=1;bBytesRead<=4;bBytesRead++)//最多读取4字节
    {
        //int iValue=fgetc(fp);
		
		//int iValue=fgetc(fp);
		
		char iValue;
		
		//f_gets(&iValue,1,fp);
		
		f_read(fp,&iValue,1,&br);
		
		if (f_eof(fp) ==1)
		
        //if(iValue==EOF)
            return 0;
        *pOut=(*pOut<<7)|(iValue & 0x7F);//新读入的是低位
        if(!(iValue & 0x80))//如果没有后续字节
            break;//就停止读取。
    }
    return bBytesRead;//返回读取的字节数
}


#if 0
//=============================================================================
//ParseMIDI:
//分析MIDI文件。失败返回零，成功返回非零
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"读取文件%s遇到错误\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s不是一个正常的MIDI文件\r\n",pszFileName);goto BadReturn;}
	#define READ(x) if(f_read(fp,&x,sizeof(x),&br)!=FR_OK)READERR

	
	
	
	//int res;
    //FIL *fsrc,fdst;
	
	FIL *fp = &fdst;
	
    MIDIHeader midiHeader;
    size_t i;

    //打开文件
    //fp=fopen(pszFileName,"rb");
	
	res = f_open(fp,pszFileName,FA_READ);
	//res = f_open(fp,"0:midfile\baga01.mid",FA_READ);
	
    if (res != FR_OK)
        READERR;

    //MIDI文件头就是一个MIDIHeader结构体。
    //但是要注意其中的数值都是Big-Endian存储的
    //需要进行转换

    //读取MIDI文件头
    READ(midiHeader);
    
    //检查文件格式
    if(midiHeader.dwFlag!=MIDI_MThd)//标识必须是"MThd"
        FMTERR;

    //转换为Little-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //分析文件头
    switch(midiHeader.wType)
    {
    case MIDIT_SingleTrack:
        my_fputs("类型:单音轨\r\n",stdout);
        break;
    case MIDIT_MultiSync:
        my_fputs("类型:同步多音轨\r\n",stdout);
        break;
    case MIDIT_MultiAsync:
        my_fputs("类型:异步多音轨\r\n",stdout);
        break;
    default:
        my_fprintf(stdout,"类型:未知(0x%04X)\n",midiHeader.wType);
        break;
    }

    //打印音轨数等信息
    my_fprintf(stdout,
        "音轨数:%u\r\n"
        "每四分音符时钟数:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //正确跳转到MIDI音轨的位置，体现midiHeader.dwRestLen的作用……
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	
	f_lseek (&fdst, 8+midiHeader.dwRestLen);
	

    //准备读取各个音轨
    for(i=0;i<midiHeader.wNbTracks;i++)
    {
        DWORD dwTrackFlag;
        DWORD dwTrackLen;

        //每个音轨的开头都是一个dwTrackFlag和一个dwTrackLen
        //dwTrackFlag的值必须是MIDI_MTrk
        //dwTrackLen标记了下一个音轨的位置

        size_t TrackStartPos;

        my_fputs("读取音轨\r\n",stdout);

        READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//检查文件格式
        READ(dwTrackLen);   TrackStartPos=f_tell(fp);//记录当前位置

        //转换Big-Endian
        dwTrackLen=BSwapD(dwTrackLen);

        //显示每个音轨的字节数
        my_fprintf(stdout,"音轨大小:%u 字节\r\n",dwTrackLen);

        //音轨的重要内容是事件数据
        for(;;)//循环读取事件
        {
            DWORD dwDelay;
            BYTE bEvent;
			DWORD Delay;
            //每个事件的构成都是：
            //延时，事件号，参数
            //其中的延时是动态字节，参数大小随事件号而定

            //上一个事件号
            BYTE bLastEvent;

			
            //读取延时
            if(!ReadDynamicBytes(fp,&dwDelay))
                READERR;
			
			
			

			Delay = UsDelay/midiHeader.wTicksPerCrotchet*dwDelay/2;
			
			//Delay_us(Delay+1);
			
			HAL_Delay(Delay+1);
			
            //读取事件号
            READ(bEvent);

            //每读取一个事件，打印这个事件的时间
            my_fprintf(stdout,"时间:%u,",dwDelay);

            //分析事件
ProcEvent:  if(bEvent <= 0x7F)
            {//0到0x7F:和上一个事件的事件号相同，而读取的这个字节就是上一个事件号的参数的第一个字节
                //fseek(fp,-1,SEEK_CUR);//回退一个字节
				
				f_lseek(fp,f_tell(fp)-1);//回退一个字节
				
				
                bEvent=bLastEvent;
                goto ProcEvent;
            }
            else if(bEvent <= 0x8F)
            {//0x80到0x8F:松开音符
                BYTE bNote,bVel;

                READ(bNote);//音符
                READ(bVel); //力度

				
				while ((!MIDI_PlayFlag) && (!MIDI_StopFlag) && (!MIDI_AutoPlayFlag));
				MIDI_StopFlag =0;
				
				Sand_3Byte_MidiCmd(bEvent,//事件
							 bNote, //音高
							 bVel); //力度

                my_fprintf(stdout,"(通道%u)松开音符:%s, 力度:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    NoteToString(bNote),//音高
                    bVel);//力度
				
				
            }
            else if(bEvent <= 0x9F)
            {//0x90到0x9F:按下音符
                BYTE bNote,bVel;

                READ(bNote);//音符
                READ(bVel); //力度

				
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
				
				Sand_3Byte_MidiCmd(bEvent,//事件
							 bNote, //音高
						     bVel); //力度
				

                my_fprintf(stdout,"(通道%u)按下音符:%s, 力度:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    NoteToString(bNote),//音高
                    bVel);//力度
				
            }
            else if(bEvent <= 0xAF)
            {//0xA0到0xAF:触后音符
                BYTE bNote,bVel;

                READ(bNote);//音符
                READ(bVel); //力度

				
				Sand_3Byte_MidiCmd(bEvent,//事件
							 bNote, //音高
							 bVel); //力度
				
				
				
                my_fprintf(stdout,"(通道%u)触后音符:%s, 力度:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    NoteToString(bNote),//音高
                    bVel);//力度
				
            }
            else if(bEvent <= 0xBF)
            {//0xB0到0xBF:控制器
                BYTE bReg,bVal;

                READ(bReg);//寄存器号
                READ(bVal);//寄存器值

				
				Sand_3Byte_MidiCmd(bEvent,//事件
							 bReg,  //寄存器号
							 bVal); //寄存器值
				
				
				
                my_fprintf(stdout,"(通道%u)控制器:%u,值=%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    bReg,//寄存器
                    bVal);//值
				
            }
            else if(bEvent <= 0xCF)
            {//0xC0到0xCF:改变乐器
                BYTE bInstrument;

                READ(bInstrument);//乐器号
                
				
				
				Sand_2Byte_MidiCmd(bEvent,	//事件
						     bInstrument);  //乐器号
							
				
                my_fprintf(stdout,"(通道%u)改变乐器:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    bInstrument);//乐器号
				
            }
            else if(bEvent <= 0xDF)
            {//0xD0到0xDF:触后通道
                BYTE bChannel;

                READ(bChannel);//通道号
                
				Sand_2Byte_MidiCmd(bEvent,	   //事件
						           bChannel);  //通道号
				
				
                my_fprintf(stdout,"(通道%u)触后通道:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    bChannel);//通道号
				
            }
            else if(bEvent <= 0xEF)
            {//0xE0到0xEF:滑音
                WORD wPitch;//滑音参数是Little-Endian的WORD

                READ(wPitch);
                
				
				Sand_3Byte_MidiCmd(bEvent,	    //事件
						           wPitch,		//滑音参数L
								   wPitch>>8); //滑音参数H

				
                my_fprintf(stdout,"(通道%u)滑音:%u\r\n",
                    bEvent & 0xF,//低4位表示通道数
                    wPitch);//滑音
				
				
				
				
            }
            else if(bEvent == 0xF0)
            {//0xF0:系统码
                BYTE bSysCode=0;

				
				
                my_fputs("系统码:",stdout);
				
				
                for(;;)
                {
                    READ(bSysCode);
					
                    if(bSysCode!=0xF7)
					{	
					
						Sand_2Byte_MidiCmd(bEvent,	    //事件
								           bSysCode);   //系统码
					
						
					
                        my_fprintf(stdout,"0x%02X,",bSysCode);
						
						
					}
                    else
                    {
						
						Sand_2Byte_MidiCmd(bEvent,         //事件
								           bSysCode);      //系统码
						
						
                        my_fprintf(stdout,"0x%02X\r\n",bSysCode);
						
                        break;//读取到0xF7结束
                    }
                }
            }
            else if(bEvent == 0xFF)
            {//元事件
                BYTE bType,bBytes;
                size_t CurrentPos;
                
                READ(bType);//元数据类型
                READ(bBytes);//元数据字节数

				
				Sand_3Byte_MidiCmd(bEvent,      //事件
								   bType,		//元数据类型
								   bBytes);		//元数据字节数

                CurrentPos=f_tell(fp);//记录元数据读取的位置

				
                my_fputs("元数据 - ",stdout);
				
                switch(bType)
                {
                case 0x00://设置轨道音序
                    {
                        WORD wTrackSeq;
                        READ(wTrackSeq);
                        wTrackSeq=BSwapW(wTrackSeq);
						
			
                        my_fprintf(stdout,"设置轨道音序:0x%04X\r\n",wTrackSeq);
						
						
                    }
                    break;
                case 0x01://歌曲备注
                    my_fputs("备注:",stdout);
				
					
				
                    ReadStringAndPrint(fp,bBytes);
				
				
                    fputc('\n',stdout);
				
				
				
                    break;
                case 0x02://版权
                    my_fputs("版权:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
					
                    break;
                case 0x03://歌曲标题
                    my_fputs("标题:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x04://乐器名称
                    my_fputs("乐器名:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x05://歌词
                    my_fputs("歌词:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x06://标记
                    my_fputs("标记:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x07://开始点
                    my_fputs("开始点:",stdout);
                    ReadStringAndPrint(fp,bBytes);
                    fputc('\n',stdout);
                    break;
                case 0x21://音轨开始标识
                    my_fputs("音轨开始标识.\n",stdout);
                    break;
                case 0x2F://音轨结束标识
                    my_fputs("音轨结束标识.\n",stdout);
                    goto EndOfTrack;
                case 0x51://速度
                    {
                        BYTE bVelocity1,bVelocity2,bVelocity3;

                        //速度是24位整数，一个四分音符的微秒数。
                        READ(bVelocity1);
                        READ(bVelocity2);
                        READ(bVelocity3);

                        //而且是Big-Endian
                         my_fprintf(stdout,"速度: 每四分音符%u微秒.\r\n",
                            bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
						
						UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
						
						
                    }
                    break;
                case 0x58://节拍
                    {
                        BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

                        READ(bBeatNumerator);//分子
                        READ(bBeatDenominator);//分母
                        READ(bMetronomeTimer);//节拍器时钟
                        READ(bNb32ndNotesInCrotchet);//每四分音符有多少三十二分音符

                        my_fprintf(stdout,"节拍:%u/%u. 节拍器时钟:%u. 每四分音符有%u个三十二分音符.\r\n",
                            bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
                    }
                    break;
                case 0x59://调号
                    {
                        MIDIInt8 bTunePitch;
                        BYTE bTuneType;

                        READ(bTunePitch);//升降号
                        READ(bTuneType);//大调、小调

                        my_fprintf(stdout,"升降号:%d, ",bTunePitch);
                        switch(bTuneType)
                        {
                        case 0://大调
                            my_fputs("大调\r\n",stdout);
                            break;
                        case 1://小调
                            my_fputs("小调\r\n",stdout);
                            break;
                        default://不知道
                            my_fprintf(stdout,"未知调(0x%02X).\r\n",bTuneType);
                            break;
                        }
                    }
                    break;
                case 0x7F://音序特定信息
                    my_fputs("音序特定信息:",stdout);
                    ReadStringAndPrint(fp,bBytes);
				
					
                    fputc('\r',stdout);
					
				
                    break;
                default:
					
					
                    my_fprintf(stdout,"未知元数据类型(0x%02X)\r\n",bType);
					
				
                    break;
                }
				
		
				printf("\r\n");
				
				
				
				
                f_lseek(fp,CurrentPos+bBytes);//读取完成后正确跳到下一个事件的位置。
            }
            else//其它事件，未知事件
            {
				
				#ifdef NDEBUG
                my_fprintf(stdout,"未知事件:0x%08X. 停止当前音轨的分析。\r\n",bEvent);
				#endif
				
                break;
            }

            //记录上一个事件号
            bLastEvent=bEvent;
        }//回去继续读取事件。

EndOfTrack:
        f_lseek(fp,TrackStartPos+dwTrackLen);//转到下一个音轨
    }

    my_fputs("MIDI文件读取结束。\r\n",stdout);

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
		
		//事件     音符      力度
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
//分析MIDI文件。失败返回零，成功返回非零
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"读取文件%s遇到错误\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s不是一个正常的MIDI文件\r\n",pszFileName);goto BadReturn;}
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
	
    //打开文件
	res = f_open(fp,pszFileName,FA_READ);
    if (res != FR_OK)
        READERR;

    //MIDI文件头就是一个MIDIHeader结构体。
    //但是要注意其中的数值都是Big-Endian存储的
    //需要进行转换

    //读取MIDI文件头
    READ(midiHeader);
    
    //检查文件格式
    if(midiHeader.dwFlag!=MIDI_MThd)//标识必须是"MThd"
        FMTERR;

    //转换为Little-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //分析文件头
    switch(midiHeader.wType)
    {
		case MIDIT_SingleTrack:
			my_fputs("类型:单音轨\r\n",stdout);
			break;
		case MIDIT_MultiSync:
			my_fputs("类型:同步多音轨\r\n",stdout);
			break;
		case MIDIT_MultiAsync:
			my_fputs("类型:异步多音轨\r\n",stdout);
			break;
		default:
			my_fprintf(stdout,"类型:未知(0x%04X)\n",midiHeader.wType);
			break;
    }

    //打印音轨数等信息
    my_fprintf(stdout,
        "音轨数:%u\r\n"
        "每四分音符时钟数:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //正确跳转到MIDI音轨的位置，体现midiHeader.dwRestLen的作用……
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	f_lseek (&fdst, 8+midiHeader.dwRestLen);
	
	while (ReadAllTracksEnd != ((1<<midiHeader.wNbTracks)-1))
	{		
		//准备读取各个音轨
		for(TrackNumber=0;TrackNumber<midiHeader.wNbTracks;TrackNumber++)
		{
			DWORD dwTrackFlag;
			DWORD dwTrackLen;
			size_t TrackStartPos;
			

			//每个音轨的开头都是一个dwTrackFlag和一个dwTrackLen
			//dwTrackFlag的值必须是MIDI_MTrk
			//dwTrackLen标记了下一个音轨的位置

			if ((ReadAllTracksEnd & (1<<TrackNumber)) == (1<<TrackNumber) ||(MidiInfo[TrackNumber].EventDelay >0))  //判断当前音轨是否已经读取结束
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
			
				my_fputs("读取音轨\r\n",stdout);

				READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//检查文件格式
				READ(dwTrackLen);   TrackStartPos=f_tell(fp);//记录当前位置

				//转换Big-Endian
				dwTrackLen=BSwapD(dwTrackLen);

				MidiInfo[TrackNumber].TracksFirstReadFlag=1;
				
				//显示每个音轨的字节数
				my_fprintf(stdout,"音轨大小:%u 字节\r\n",dwTrackLen);
			}
			else
			{
				//f_lseek (&fdst, TrackStartPos+MidiInfo[TrackNumber].CurrentEventPos);
				f_lseek (&fdst,MidiInfo[TrackNumber].CurrentEventPos);
			}
			//音轨的重要内容是事件数据
			//for(;;)//循环读取事件
			do
			{
				DWORD dwDelay;
				BYTE bEvent;
				
				//每个事件的构成都是：
				//延时，事件号，参数
				//其中的延时是动态字节，参数大小随事件号而定

				//上一个事件号
				BYTE bLastEvent[16];
				
				/*
				if (MidiInfo[TrackNumber].EventDelay >0)
				{
					//MidiInfo[TrackNumber].EventDelay--;
					break;	
				}
				*/
				
				
				//读取延时
				if(!ReadDynamicBytes(fp,&dwDelay))
					READERR;
				MidiInfo[TrackNumber].EventDelay= dwDelay;
/*
				if (MidiInfo[TrackNumber].EventDelay >0)
				{
					break;	
				}
				*/
				//读取事件号
				READ(bEvent);
				//每读取一个事件，打印这个事件的时间
				my_fprintf(stdout,"时间:%u,",dwDelay);

				//分析事件
ProcEvent:  	if(bEvent <= 0x7F)
				{//0到0x7F:和上一个事件的事件号相同，而读取的这个字节就是上一个事件号的参数的第一个字节
					//fseek(fp,-1,SEEK_CUR);//回退一个字节
					
					f_lseek(fp,f_tell(fp)-1);//回退一个字节
			
					bEvent=bLastEvent[TrackNumber];
					goto ProcEvent;
				}
				else if(bEvent <= 0x8F)
				{//0x80到0x8F:松开音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度

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
					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					
				

					my_fprintf(stdout,"(通道%u)松开音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度					
				}
				else if(bEvent <= 0x9F)
				{//0x90到0x9F:按下音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度
					
					
					

					//MIDI_AutoPlayFlag=1; //测试
					
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

							Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
							
							
							//f_lseek(fp,CurrentPos+bBytes);//读取完成后正确跳到下一个事件的位置。
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
							
							Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
							
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
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					*/
					
					
					my_fprintf(stdout,"(通道%u)按下音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度
					
				}
				else if(bEvent <= 0xAF)
				{//0xA0到0xAF:触后音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度

					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					
					
					
					my_fprintf(stdout,"(通道%u)触后音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度
					
				}
				else if(bEvent <= 0xBF)
				{//0xB0到0xBF:控制器
					BYTE bReg,bVal;

					READ(bReg);//寄存器号
					READ(bVal);//寄存器值

					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bReg,  //寄存器号
								 bVal); //寄存器值
					
					
					
					my_fprintf(stdout,"(通道%u)控制器:%u,值=%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bReg,//寄存器
						bVal);//值
					
				}
				else if(bEvent <= 0xCF)
				{//0xC0到0xCF:改变乐器
					BYTE bInstrument;

					READ(bInstrument);//乐器号
					
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	//事件
								 bInstrument);  //乐器号
					*/
					Sand_3Byte_MidiCmd(bEvent,	//事件
								 bInstrument,  //乐器号
										0x00);

					my_fprintf(stdout,"(通道%u)改变乐器:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bInstrument);//乐器号
					
				}
				else if(bEvent <= 0xDF)
				{//0xD0到0xDF:触后通道
					BYTE bChannel;

					READ(bChannel);//通道号
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	   //事件
									   bChannel);  //通道号
					*/
					Sand_3Byte_MidiCmd(bEvent,	   //事件
									   bChannel,  //通道号
										0x00);
					
					my_fprintf(stdout,"(通道%u)触后通道:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bChannel);//通道号
					
				}
				else if(bEvent <= 0xEF)
				{//0xE0到0xEF:滑音
					WORD wPitch;//滑音参数是Little-Endian的WORD

					READ(wPitch);
					
					
					Sand_3Byte_MidiCmd(bEvent,	    //事件
									   wPitch,		//滑音参数L
									   wPitch>>8); //滑音参数H

					
					my_fprintf(stdout,"(通道%u)滑音:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						wPitch);//滑音
					
					
					
					
				}
				else if(bEvent == 0xF0)
				{//0xF0:系统码
					BYTE bSysCode=0;

					
					
					my_fputs("系统码:",stdout);
					
					
					for(;;)
					{
						READ(bSysCode);
						
						if(bSysCode!=0xF7)
						{	
						
							/*
							Sand_2Byte_MidiCmd(bEvent,	    //事件
											   bSysCode);   //系统码
						
							*/
						
							my_fprintf(stdout,"0x%02X,",bSysCode);
							
							
						}
						else
						{
							/*
							Sand_2Byte_MidiCmd(bEvent,         //事件
											   bSysCode);      //系统码
							*/
							
							my_fprintf(stdout,"0x%02X\r\n",bSysCode);
							
							break;//读取到0xF7结束
						}
					}
				}
				else if(bEvent == 0xFF)  
				{//元事件
					BYTE bType,bBytes;
					size_t CurrentPos;
					
					READ(bType);//元数据类型
					READ(bBytes);//元数据字节数

					/*
					Sand_3Byte_MidiCmd(bEvent,      //事件
									   bType,		//元数据类型
									   bBytes);		//元数据字节数
					*/	
					CurrentPos=f_tell(fp);//记录元数据读取的位置

					
					my_fputs("元数据 - ",stdout);
					
					switch(bType)
					{
					case 0x00://设置轨道音序
						{
							WORD wTrackSeq;
							READ(wTrackSeq);
							wTrackSeq=BSwapW(wTrackSeq);
							
				
							my_fprintf(stdout,"设置轨道音序:0x%04X\r\n",wTrackSeq);
							
							
						}
						break;
					case 0x01://歌曲备注
						my_fputs("备注:",stdout);
					
						
					
						ReadStringAndPrint(fp,bBytes);
					
					
						my_fputc('\n',stdout);
					
					
					
						break;
					case 0x02://版权
						my_fputs("版权:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						
						break;
					case 0x03://歌曲标题
						my_fputs("标题:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x04://乐器名称
						my_fputs("乐器名:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x05://歌词
						my_fputs("歌词:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x06://标记
						my_fputs("标记:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x07://开始点
						my_fputs("开始点:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x21://音轨开始标识
						my_fputs("音轨开始标识.\n",stdout);
						break;
					case 0x2F://音轨结束标识
						my_fputs("音轨结束标识.\n",stdout);
					
					
						ReadAllTracksEnd |= (1<<TrackNumber);
					
					    //MidiInfo[TrackNumber].TracksEndFlag = TRACK_END;
						//break;	
						goto EndOfTrack;
					
					case 0x51://速度
						{
							BYTE bVelocity1,bVelocity2,bVelocity3;

							//速度是24位整数，一个四分音符的微秒数。
							READ(bVelocity1);
							READ(bVelocity2);
							READ(bVelocity3);

							//而且是Big-Endian
							 my_fprintf(stdout,"速度: 每四分音符%u微秒.\r\n",
								bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
							
							UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
							
							
						}
						break;
					case 0x58://节拍
						{
							BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

							READ(bBeatNumerator);//分子
							READ(bBeatDenominator);//分母
							READ(bMetronomeTimer);//节拍器时钟
							READ(bNb32ndNotesInCrotchet);//每四分音符有多少三十二分音符
							
							Denominator = bBeatDenominator;
							
							my_fprintf(stdout,"节拍:%u/%u. 节拍器时钟:%u. 每四分音符有%u个三十二分音符.\r\n",
								bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
						}
						break;
					case 0x59://调号
						{
							MIDIInt8 bTunePitch;
							BYTE bTuneType;

							READ(bTunePitch);//升降号
							READ(bTuneType);//大调、小调

							my_fprintf(stdout,"升降号:%d, ",bTunePitch);
							switch(bTuneType)
							{
							case 0://大调
								my_fputs("大调\r\n",stdout);
								break;
							case 1://小调
								my_fputs("小调\r\n",stdout);
								break;
							default://不知道
								my_fprintf(stdout,"未知调(0x%02X).\r\n",bTuneType);
								break;
							}
						}
						break;
					case 0x7F://音序特定信息
						my_fputs("音序特定信息:",stdout);
						ReadStringAndPrint(fp,bBytes);
					
						
						my_fputc('\r',stdout);
						
					
						break;
					default:
						
						
						my_fprintf(stdout,"未知元数据类型(0x%02X)\r\n",bType);
						
					
						break;
					}
					
			
					my_printf("\r\n");

					f_lseek(fp,CurrentPos+bBytes);//读取完成后正确跳到下一个事件的位置。
					
					
					MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
					//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
					
					
					
					// = CurrentPos+bBytes;
					
					
				}
				else//其它事件，未知事件
				{
					
					#ifdef NDEBUG
					my_fprintf(stdout,"未知事件:0x%08X. 停止当前音轨的分析。\r\n",bEvent);
					#endif
					
					break;
				}
FirstEndOfTrack:
				//记录上一个事件号
				bLastEvent[TrackNumber]=bEvent;
				
				MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
			}while ((MidiInfo[TrackNumber].EventDelay ==0) || (midiHeader.wType==MIDIT_MultiAsync));//回去继续读取事件。
		

 
			
			
EndOfTrack:
		
			MidiInfo[TrackNumber].CurrentEventPos =f_tell(fp);
			

			
			//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
			
			
			
			f_lseek(fp,TrackStartPos+dwTrackLen);//转到下一个音轨
			
			
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
    my_fputs("MIDI文件读取结束。\r\n",stdout);
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
//分析MIDI文件。失败返回零，成功返回非零
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	#define READERR {my_fprintf(stderr,"读取文件%s遇到错误\r\n",pszFileName);goto BadReturn;}
	#define FMTERR {my_fprintf(stderr,"%s不是一个正常的MIDI文件\r\n",pszFileName);goto BadReturn;}
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
	
    //打开文件
	res = f_open(fp,pszFileName,FA_READ);
    if (res != FR_OK)
        READERR;

    //MIDI文件头就是一个MIDIHeader结构体。
    //但是要注意其中的数值都是Big-Endian存储的
    //需要进行转换

    //读取MIDI文件头
    READ(midiHeader);
    
    //检查文件格式
    if(midiHeader.dwFlag!=MIDI_MThd)//标识必须是"MThd"
        FMTERR;

    //转换为Little-Endian
    midiHeader.dwRestLen=           BSwapD(midiHeader.dwRestLen);
    midiHeader.wType=               BSwapW(midiHeader.wType);
    midiHeader.wNbTracks=           BSwapW(midiHeader.wNbTracks);
    midiHeader.wTicksPerCrotchet=   BSwapW(midiHeader.wTicksPerCrotchet);

    //分析文件头
    switch(midiHeader.wType)
    {
		case MIDIT_SingleTrack:
			my_fputs("类型:单音轨\r\n",stdout);
			break;
		case MIDIT_MultiSync:
			my_fputs("类型:同步多音轨\r\n",stdout);
			break;
		case MIDIT_MultiAsync:
			my_fputs("类型:异步多音轨\r\n",stdout);
			break;
		default:
			my_fprintf(stdout,"类型:未知(0x%04X)\n",midiHeader.wType);
			break;
    }

    //打印音轨数等信息
    my_fprintf(stdout,
        "音轨数:%u\r\n"
        "每四分音符时钟数:%u\r\n",
        midiHeader.wNbTracks,
        midiHeader.wTicksPerCrotchet);

    //正确跳转到MIDI音轨的位置，体现midiHeader.dwRestLen的作用……
    //fseek(fp,8+midiHeader.dwRestLen,SEEK_SET);
	f_lseek (fp, 8+midiHeader.dwRestLen);
	
	while (ReadAllTracksEnd != ((1<<midiHeader.wNbTracks)-1))
	{		
		//准备读取各个音轨
		for(TrackNumber=0;TrackNumber<midiHeader.wNbTracks;TrackNumber++)
		{
			DWORD dwTrackFlag;
			DWORD dwTrackLen;
			size_t TrackStartPos;
			

			//每个音轨的开头都是一个dwTrackFlag和一个dwTrackLen
			//dwTrackFlag的值必须是MIDI_MTrk
			//dwTrackLen标记了下一个音轨的位置

			if ((ReadAllTracksEnd & (1<<TrackNumber)) == (1<<TrackNumber) ||
                (MidiInfo[TrackNumber].EventDelay >0))  //判断当前音轨是否已经读取结束
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
			
				my_fputs("读取音轨\r\n",stdout);

				READ(dwTrackFlag);  if(dwTrackFlag!=MIDI_MTrk)FMTERR;//检查文件格式
				READ(dwTrackLen);   TrackStartPos=f_tell(fp);//记录当前位置

				//转换Big-Endian
				dwTrackLen=BSwapD(dwTrackLen);

				MidiInfo[TrackNumber].TracksFirstReadFlag=1;
				
				//显示每个音轨的字节数
				my_fprintf(stdout,"音轨大小:%u 字节\r\n",dwTrackLen);
			}
			else
			{
				
				//WORK_LED(0);
				
				//f_lseek (&fdst, TrackStartPos+MidiInfo[TrackNumber].CurrentEventPos);
				f_lseek (fp,MidiInfo[TrackNumber].CurrentEventPos);
				
				//WORK_LED(1);	
			}
			//音轨的重要内容是事件数据
			//for(;;)//循环读取事件
			do
			{
				DWORD dwDelay;
				BYTE bEvent;
				
				//每个事件的构成都是：
				//延时，事件号，参数
				//其中的延时是动态字节，参数大小随事件号而定

				//上一个事件号
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
    				//读取延时
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
				//读取事件号
				READ(bEvent);
				//每读取一个事件，打印这个事件的时间
				my_fprintf(stdout,"时间:%u,",dwDelay);

				//分析事件
ProcEvent:  	if(bEvent <= 0x7F)
				{//0到0x7F:和上一个事件的事件号相同，而读取的这个字节就是上一个事件号的参数的第一个字节
					//fseek(fp,-1,SEEK_CUR);//回退一个字节
					
					f_lseek(fp,f_tell(fp)-1);//回退一个字节
			
					bEvent=bLastEvent[TrackNumber];
					goto ProcEvent;
				}
				else if(bEvent <= 0x8F)
				{//0x80到0x8F:松开音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度

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
					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					
				

					my_fprintf(stdout,"(通道%u)松开音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度					
				}
				else if(bEvent <= 0x9F)
				{//0x90到0x9F:按下音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度
					
					
					

					//MIDI_AutoPlayFlag=1; //测试
					
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

							Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
							
							
							//f_lseek(fp,CurrentPos+bBytes);//读取完成后正确跳到下一个事件的位置。
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
							
							Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
							
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
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					*/
					
					
					my_fprintf(stdout,"(通道%u)按下音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度
					
				}
				else if(bEvent <= 0xAF)
				{//0xA0到0xAF:触后音符
					BYTE bNote,bVel;

					READ(bNote);//音符
					READ(bVel); //力度

					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bNote, //音高
								 bVel); //力度
					
					
					
					my_fprintf(stdout,"(通道%u)触后音符:%s, 力度:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						NoteToString(bNote),//音高
						bVel);//力度
					
				}
				else if(bEvent <= 0xBF)
				{//0xB0到0xBF:控制器
					BYTE bReg,bVal;

					READ(bReg);//寄存器号
					READ(bVal);//寄存器值

					
					Sand_3Byte_MidiCmd(bEvent,//事件
								 bReg,  //寄存器号
								 bVal); //寄存器值
					
					
					
					my_fprintf(stdout,"(通道%u)控制器:%u,值=%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bReg,//寄存器
						bVal);//值
					
				}
				else if(bEvent <= 0xCF)
				{//0xC0到0xCF:改变乐器
					BYTE bInstrument;

					READ(bInstrument);//乐器号
					
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	//事件
								 bInstrument);  //乐器号
					*/
					Sand_3Byte_MidiCmd(bEvent,	//事件
								 bInstrument,  //乐器号
										0x00);

					my_fprintf(stdout,"(通道%u)改变乐器:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bInstrument);//乐器号
					
				}
				else if(bEvent <= 0xDF)
				{//0xD0到0xDF:触后通道
					BYTE bChannel;

					READ(bChannel);//通道号
					
					/*
					Sand_2Byte_MidiCmd(bEvent,	   //事件
									   bChannel);  //通道号
					*/
					Sand_3Byte_MidiCmd(bEvent,	   //事件
									   bChannel,  //通道号
										0x00);
					
					my_fprintf(stdout,"(通道%u)触后通道:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						bChannel);//通道号
					
				}
				else if(bEvent <= 0xEF)
				{//0xE0到0xEF:滑音
					WORD wPitch;//滑音参数是Little-Endian的WORD

					READ(wPitch);
					
					
					Sand_3Byte_MidiCmd(bEvent,	    //事件
									   wPitch,		//滑音参数L
									   wPitch>>8); //滑音参数H

					
					my_fprintf(stdout,"(通道%u)滑音:%u\r\n",
						bEvent & 0xF,//低4位表示通道数
						wPitch);//滑音
					
					
					
					
				}
				else if(bEvent == 0xF0)
				{//0xF0:系统码
					BYTE bSysCode=0;

					
					
					my_fputs("系统码:",stdout);
					
					
					for(;;)
					{
						READ(bSysCode);
						
						if(bSysCode!=0xF7)
						{	
						
							/*
							Sand_2Byte_MidiCmd(bEvent,	    //事件
											   bSysCode);   //系统码
						
							*/
						
							my_fprintf(stdout,"0x%02X,",bSysCode);
							
							
						}
						else
						{
							/*
							Sand_2Byte_MidiCmd(bEvent,         //事件
											   bSysCode);      //系统码
							*/
							
							my_fprintf(stdout,"0x%02X\r\n",bSysCode);
							
							break;//读取到0xF7结束
						}
					}
				}
				else if(bEvent == 0xFF)  
				{//元事件
					BYTE bType,bBytes;
					size_t CurrentPos;
					
					READ(bType);//元数据类型
					READ(bBytes);//元数据字节数

					/*
					Sand_3Byte_MidiCmd(bEvent,      //事件
									   bType,		//元数据类型
									   bBytes);		//元数据字节数
					*/	
					CurrentPos=f_tell(fp);//记录元数据读取的位置

					
					my_fputs("元数据 - ",stdout);
					
					switch(bType)
					{
					case 0x00://设置轨道音序
						{
							WORD wTrackSeq;
							READ(wTrackSeq);
							wTrackSeq=BSwapW(wTrackSeq);
							
				
							my_fprintf(stdout,"设置轨道音序:0x%04X\r\n",wTrackSeq);
							
							
						}
						break;
					case 0x01://歌曲备注
						my_fputs("备注:",stdout);
					
						
					
						ReadStringAndPrint(fp,bBytes);
					
					
						my_fputc('\n',stdout);
					
					
					
						break;
					case 0x02://版权
						my_fputs("版权:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						
						break;
					case 0x03://歌曲标题
						my_fputs("标题:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x04://乐器名称
						my_fputs("乐器名:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x05://歌词
						my_fputs("歌词:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x06://标记
						my_fputs("标记:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x07://开始点
						my_fputs("开始点:",stdout);
						ReadStringAndPrint(fp,bBytes);
						my_fputc('\n',stdout);
						break;
					case 0x21://音轨开始标识
						my_fputs("音轨开始标识.\n",stdout);
						break;
					case 0x2F://音轨结束标识
						my_fputs("音轨结束标识.\n",stdout);
					
					
						ReadAllTracksEnd |= (1<<TrackNumber);
					
					    //MidiInfo[TrackNumber].TracksEndFlag = TRACK_END;
						//break;	
						goto EndOfTrack;
					
					case 0x51://速度
						{
							BYTE bVelocity1,bVelocity2,bVelocity3;

							//速度是24位整数，一个四分音符的微秒数。
							READ(bVelocity1);
							READ(bVelocity2);
							READ(bVelocity3);

							//而且是Big-Endian
							 my_fprintf(stdout,"速度: 每四分音符%u微秒.\r\n",
								bVelocity3|(bVelocity2<<8)|(bVelocity1<<16));
							
							UsDelay = bVelocity3|(bVelocity2<<8)|(bVelocity1<<16);
							
							
						}
						break;
					case 0x58://节拍
						{
							BYTE bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet;

							READ(bBeatNumerator);//分子
							READ(bBeatDenominator);//分母
							READ(bMetronomeTimer);//节拍器时钟
							READ(bNb32ndNotesInCrotchet);//每四分音符有多少三十二分音符
							
							//Denominator = bBeatDenominator;
							
							my_fprintf(stdout,"节拍:%u/%u. 节拍器时钟:%u. 每四分音符有%u个三十二分音符.\r\n",
								bBeatNumerator,bBeatDenominator,bMetronomeTimer,bNb32ndNotesInCrotchet);
						}
						break;
					case 0x59://调号
						{
							MIDIInt8 bTunePitch;
							BYTE bTuneType;

							READ(bTunePitch);//升降号
							READ(bTuneType);//大调、小调

							my_fprintf(stdout,"升降号:%d, ",bTunePitch);
							switch(bTuneType)
							{
							case 0://大调
								my_fputs("大调\r\n",stdout);
								break;
							case 1://小调
								my_fputs("小调\r\n",stdout);
								break;
							default://不知道
								my_fprintf(stdout,"未知调(0x%02X).\r\n",bTuneType);
								break;
							}
						}
						break;
					case 0x7F://音序特定信息
						my_fputs("音序特定信息:",stdout);
						ReadStringAndPrint(fp,bBytes);
					
						
						my_fputc('\r',stdout);
						
					
						break;
					default:
						
						
						my_fprintf(stdout,"未知元数据类型(0x%02X)\r\n",bType);
						
					
						break;
					}
					
			
					my_printf("\r\n");

					f_lseek(fp,CurrentPos+bBytes);//读取完成后正确跳到下一个事件的位置。
					
					
					MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
					//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
					
					
					
					// = CurrentPos+bBytes;
					
					
				}
				else//其它事件，未知事件
				{
					
					#ifdef NDEBUG
					my_fprintf(stdout,"未知事件:0x%08X. 停止当前音轨的分析。\r\n",bEvent);
					#endif
					
					break;
				}
//FirstEndOfTrack:
				//记录上一个事件号
				bLastEvent[TrackNumber]=bEvent;
				
				MidiInfo[TrackNumber].CurrentEventPos=f_tell(fp);
					
			}while ((MidiInfo[TrackNumber].EventDelay ==0) || (midiHeader.wType==MIDIT_MultiAsync));//回去继续读取事件。
			//}while ((midiHeader.wType==MIDIT_MultiAsync));//回去继续读取事件。

 
			
			
EndOfTrack:
		
			MidiInfo[TrackNumber].CurrentEventPos =f_tell(fp);
			

			
			//MidiInfo[TrackNumber].CurrentEventPos -=TrackStartPos;
			
			
			if (FirstReadTrack==1)
			{
				f_lseek(fp,TrackStartPos+dwTrackLen);//转到下一个音轨
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
    my_fputs("MIDI文件读取结束。\r\n",stdout);
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
//将音符音高转换为字符串
//其实每次返回的都是同一缓冲区，只是每次都改写了缓冲区的值。
//-----------------------------------------------------------------------------
char*NoteToString(BYTE bNote)
{
    static char szBuf[5];//顶多5字符
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
//将音符音高转换为字符串
//其实每次返回的都是同一缓冲区，只是每次都改写了缓冲区的值。
//-----------------------------------------------------------------------------
char*NoteToString(BYTE bNote)
{
    static char szBuf[5];//顶多5字符
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
//读取字符串然后打印。返回读取的字节数
//-----------------------------------------------------------------------------
size_t ReadStringAndPrint(FIL*fp,size_t szLength)
{
    char*pBuf;
    pBuf=(char*)malloc(szLength+1);
    if(pBuf)//分配内存一次读取
    {
        //size_t Ret=fread(pBuf,1,szLength,fp);
		
		size_t Ret=f_read(fp,pBuf,szLength,&br);
		
        pBuf[szLength]=0;//结尾的\0
        my_fputs(pBuf,stdout);
        free(pBuf);
        return Ret;
    }
    else
    {
        my_fputs("无法将字符串读入内存。\r\n",stderr);
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
		Sand_3Byte_MidiCmd(0xE0 | TrackCnt,	    //事件
						   0x00,				//滑音参数L
						   0x40); 				//滑音参数H
		
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //事件
						   0x01,				//控制器号码
						   0x00); 				//控制器参数
		
			
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //事件
						   0x40,				//控制器号码
						   0x00); 			    //控制器参数
		
			
		Sand_3Byte_MidiCmd(0xB0 | TrackCnt,	    //事件
						   0x79,				//控制器号码
						   0x00); 				//控制器参数		
	}	
}








