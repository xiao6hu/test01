
#include "midi.h"

MIDI_HEADER midi_header;
TRACK_CONTROL	track[MAX_TRACKS_CNT] = {0};	
//base_count为1/4音符需要的硬件tick数
unsigned short base_count=0;
//unsigned long idata time_wait=0xffff;
volatile unsigned long overflow_count=0;

//unsigned long idata  byte_of_track;
static unsigned char  com_data[4];

int res;  
FIL fsrc,fdst;
FATFS fs;
UINT br, bw;            // File R/W count
BYTE buffer[512];       // file copy buffer


DIR dir;
FILINFO filinfo;
FATFS fs;

FIL *fp = &fdst;


MIDIUInt8 MidiStartstopFlag=0;

BYTE MIDI_PlayFlag =0;

BYTE MIDI_StopFlag =0;

BYTE MIDI_AutoPlayFlag =0;

BYTE MIDI_StopPlayFlag=0;


u32 MIDI_AutoPlayCnt=0;
u32  Midi_AutoLightsCnt=0;



void midi_heart_beat(void)
{
	if (MidiStartstopFlag==1)
	{
		overflow_count++;
	}
}



/*
static unsigned char read1byte(unsigned char *ptr)
{
	return *ptr;
}

static unsigned short read2byte(unsigned char *ptr)
{
	unsigned short ret;
	ret = *ptr;
	ret = ret << 8;
	ret |= *(ptr+1);
 
	return ret;
}

static unsigned long read4byte(unsigned char *ptr)
{
	unsigned long ret;
	ret = *ptr;
	ret = ret<<8;
 
	ret |= *(ptr+1);
	ret = ret<<8;
 
	ret |= *(ptr+2);
	ret = ret<<8;
 
	ret |= *(ptr+3); 
	return ret; 
}

*/

/*
读取一个长度值，返回值为长度
jmp中返回这个长度变量所占用的字节数
长度变量格式为b1XXXXXXX b1XXXXXXX b0XXXXXXX(最高位为0时截至)
*/
MIDIUInt32 GetVLE (MIDIUInt32 ptr_origin,MIDIUInt8 *jmp)
{
	MIDIUInt8 ByteRead;
	MIDIUInt32 Result;
	MIDIUInt32 ptr = ptr_origin;

	//Assume zero
	Result = 0;
	do{
		//Read first byte
		//ByteRead = read1byte(ptr++);
		READ(ByteRead);
		ptr ++;
		//Store 7bit part
		Result = (Result << 7) + (ByteRead & 0x7F);
    }while ((ByteRead & 0x80) != 0);
    
    *jmp = (MIDIUInt8)(ptr - ptr_origin);
	return Result;
} 

void send_midi_command(MIDIUInt8 *dat, MIDIUInt16 len)
{
	midi_write_uart(dat,len);
}

MIDIInt16 search_whole_word(TRACK_CONTROL *ptrack,MIDIInt8 *dst)
{
	MIDIUInt32 *ptr;//,*ptr1;
	MIDIUInt8 jmp_len;
	MIDIUInt32 mydata;
	
	MIDIUInt8 MIDIByte,/*MIDIByte1,*/MetaEvent,/*MetaEvent1,*/MIDICommand,MIDIChannel;
	MIDIUInt32 DataLength/*,DataLength1*/,Counter1/*,Counter2*/;
	MIDIUInt8 n1,n2;
	//歌词
 	MIDIUInt8 words[32];

	MIDIUInt8 Velocity;
	
	MIDIUInt8 	TmpBuffByte;
	MIDIUInt16 	TmpBuffWord;
	
 	ptr = &ptrack->ptr;
	
 	while(1)
 	{
		//MIDIByte = read1byte(ptr);
		READ(MIDIByte);
		if((MIDIByte & 0x80) == 0x00)
		{
			MIDIByte = ptrack->last_command;
			(*ptr) ++;
			F_LSEEK(*ptr);//回退一个字节
		}
		else
		{
			(*ptr) ++;
			ptrack->last_command = MIDIByte;
		}

		if(MIDIByte == 0xFF) 
		{
	    	//MetaEvent  = read1byte(ptr++);
	 		READ(MetaEvent);
	    	(*ptr) ++;
			DataLength = GetVLE(*ptr,&jmp_len);
			(*ptr) += jmp_len;
			
			if(MetaEvent == 0x2F) //End of track}
			{
				ptrack->track_end = 1;
				return 0;
			}
			else if (MetaEvent == 0x05)  //歌词
			{	
				for (Counter1 = 0; Counter1 < DataLength; Counter1++) 
				{
					//words[Counter1] = read1byte(ptr++);
					READ(words[Counter1]);
					(*ptr) ++;
				}
				
				memcpy(dst,words,DataLength);
				if(DataLength == 1 && dst[0] == 0x0d)
				{
					dst[0] = 0;
					return 0;
				}
				dst+=DataLength;
			}
			else if (MetaEvent == 0x51) 
			{ //Tempo change
				/*
				mydata = read1byte(ptr++);
				mydata = (mydata << 8) + read1byte(ptr++);
				mydata = (mydata << 8) + read1byte(ptr++);
				*/
				READ(Velocity);
				(*ptr) ++;
				mydata = Velocity;
				READ(Velocity);
				(*ptr) ++;
				mydata = (mydata << 8) + Velocity;
				READ(Velocity);
				(*ptr) ++;
				mydata = (mydata << 8) + Velocity;
				
				ChangeSpeed (mydata);
			}
			else if(MetaEvent == 0x58)
			{
				/*
				n1 = read1byte(ptr++);
				n2 = read1byte(ptr++);
				mydata = read1byte(ptr++);
				mydata = read1byte(ptr++);
				*/
				READ(n1);
				(*ptr) ++;
				READ(n2);
				(*ptr) ++;
				READ(TmpBuffByte);
				(*ptr) ++;
				READ(TmpBuffByte);
				(*ptr) ++;
			}
			else
			{
				//Others (text events, track sequence numbers etc. - ignore
				for (Counter1 = 1; Counter1 <= DataLength; Counter1++) 
				{
					//read1byte(ptr++);
					READ(TmpBuffByte);
					(*ptr) ++;
				}
			}
		}
	 
		/*
		CHANNEL COMMANDS
		================
		Upper nibble contains command, lower contains channel
		*/
		MIDIChannel = MIDIByte & 0xF;
		MIDICommand = MIDIByte >> 4;
		if (MIDICommand == 8 || MIDICommand == 9 || MIDICommand == 0x0a) 
		{ //Note off
			/*
			n1 = read1byte(ptr++);
			n2 = read1byte(ptr++);
			*/
			READ(n1);
			(*ptr) ++;
			READ(n2);
			(*ptr) ++;
		}
		if(MIDICommand == 0x0b || MIDICommand == 0x0e)     
		{
			/*
			n1 = read1byte(ptr++);
			n2 = read1byte(ptr++);
			*/
			READ(n1);
			(*ptr) ++;
			READ(n2);
			(*ptr) ++;
		}
		if(MIDICommand == 0x0c || MIDICommand == 0x0d)
		{
			//n1 = read1byte(ptr++);
			READ(n1);
			(*ptr) ++;
		}
		/*
		SYSTEM COMMANDS
	    ===============
	    These are ignored.
		*/
		if (MIDICommand == 0xF) 
		{
			if (MIDIChannel == 0) 
			{
				//DataLength = GetVLE(ptr++,&jmp_len);
				DataLength = GetVLE(*ptr,&jmp_len);
				(*ptr) ++;
				(*ptr) += jmp_len;
				for (Counter1 = 1; Counter1 <= DataLength; Counter1++) 
					//read1byte(ptr++);
					READ(TmpBuffByte);
					(*ptr) ++;
			}
			if (MIDIChannel == 2) 
			{
				//read2byte(ptr);
				READ(TmpBuffWord);
				(*ptr) += 2;
					
			} //Song Position Pointer
			if (MIDIChannel == 3) 
			{ 
				//read1byte(ptr++); 
				READ(TmpBuffByte);
				(*ptr) ++;
			} // Song Select
		}
		
		
		GetVLE(*ptr,&jmp_len);
		(*ptr) += jmp_len;
	}
}

MIDIInt16 do_event(TRACK_CONTROL *ptrack)
{
	MIDIUInt32	*ptr;//,*ptr1;
	MIDIUInt8 	jmp_len;
	MIDIUInt32 	mydata;
	MIDIUInt8 	Velocity;
	MIDIUInt8 	MIDIByte/*,MIDIByte1*/,MetaEvent/*,MetaEvent1*/,MIDICommand,MIDIChannel;
	MIDIUInt32 	DataLength/*,DataLength1*/,Counter1/*,Counter2*/;
	MIDIUInt8 	n1,n2;

	MIDIUInt8 	TmpBuffByte;
	MIDIUInt16 	TmpBuffWord;
		
	//歌词
 	MIDIUInt8 words[32];
  	MIDIUInt8 whole_words[256];
  	
 	ptr = &ptrack->ptr;
	
	//MIDIByte = read1byte(ptr);
	READ(MIDIByte);
	if((MIDIByte & 0x80) == 0x00)
	{
		MIDIByte = ptrack->last_command;
		F_LSEEK(*ptr);//回退一个字节
	}
	else
	{
		(*ptr) ++;
		ptrack->last_command = MIDIByte;
	}
 
	if(MIDIByte == 0xFF) 
	{
		//MetaEvent  = read1byte(ptr++);
 		READ(MetaEvent);
		(*ptr) ++;
		DataLength = GetVLE(*ptr,&jmp_len);
		(*ptr) += jmp_len;
		if(MetaEvent == 0x2F) //End of track}
		{
			ptrack->track_end = 1;
			return 0;
		}
		else if (MetaEvent == 0x05)  //歌词
		{
			if(ptrack->search_whole_words)
			{
				//midi_printf("search_whole_word 1\n");
				search_whole_word(ptrack,(MIDIInt8 *)whole_words);
				//midi_printf("search_whole_word 2\n");								
				//midi_printf("whole_words = %s\n",whole_words);
				//Draw_Whole_Words(whole_words);
			}		

			for (Counter1 = 0; Counter1 < DataLength; Counter1++) 
			{
				//words[Counter1] = read1byte(ptr++);
				READ(words[Counter1]); 
				(*ptr) ++;
			}
			words[Counter1] = 0;
			//显示歌词
			//midi_printf("words = %s\n",(char *)words);
			//Draw_Bit_Words(words,GetVLE(ptr,&jmp_len));
			
			
			if(words[0] == 0x0d)
			{
				ptrack->search_whole_words = 1;
			}
			else
				ptrack->search_whole_words = 0;
			
		}
		else if (MetaEvent == 0x51) 
		{ //Tempo change
			/*
			mydata = read1byte(ptr++);
			mydata = (mydata << 8) + read1byte(ptr++);
			mydata = (mydata << 8) + read1byte(ptr++);
			*/
			READ(Velocity); 
			(*ptr) ++;
			mydata = Velocity;
			READ(Velocity); 
			(*ptr) ++;
			mydata = (mydata << 8) + Velocity;
			READ(Velocity); 
			(*ptr) ++;
			mydata = (mydata << 8) + Velocity;
			ChangeSpeed (mydata);
		}
		else if(MetaEvent == 0x58)
		{	
			/*
			n1 = read1byte(ptr++);
			n2 = read1byte(ptr++);
			mydata = read1byte(ptr++);
			mydata = read1byte(ptr++);
			*/
			READ(n1);
			(*ptr) ++;
			READ(n2);
			(*ptr) ++;
			READ(TmpBuffByte);
			(*ptr) ++;
			READ(TmpBuffByte);
			(*ptr) ++;
		}
		else
		{
			//Others (text events, track sequence numbers etc. - ignore
			for (Counter1 = 1; Counter1 <= DataLength; Counter1++) 
			{
				//read1byte(ptr++);
				READ(TmpBuffByte);
				(*ptr) ++;
			}
		}
	}
 
	/*
	CHANNEL COMMANDS
	================
	Upper nibble contains command, lower contains channel
	*/
	MIDIChannel = MIDIByte & 0xF;
	MIDICommand = MIDIByte >> 4;
	if (MIDICommand == 8 || MIDICommand == 9 || MIDICommand == 0x0a) 
	{ //Note off
	
		/*
		n1 = read1byte(ptr++);
		n2 = read1byte(ptr++);
		*/
		READ(n1);
		(*ptr) ++;
		READ(n2);
		(*ptr) ++;
		/*This allows the use of a wavetable General MIDI instrument (such
		as the Roland SCC1 (or an emulation thereof) or the FM synthesizer*/
		if(n2 > 127)
			n2 = 127;
		com_data[0] = MIDIByte;
		com_data[1] = n1;
		com_data[2] = n2;
		send_midi_command(com_data,3);
	}
	if(MIDICommand == 0x0b || MIDICommand == 0x0e)     
	{
		/*
		n1 = read1byte(ptr++);
		n2 = read1byte(ptr++);
		*/
		READ(n1);
		(*ptr) ++;
		READ(n2);
		(*ptr) ++;
		/*This allows the use of a wavetable General MIDI instrument (such
		as the Roland SCC1 (or an emulation thereof) or the FM synthesizer*/

		com_data[0] = MIDIByte;
		com_data[1] = n1;
		com_data[2] = n2;
		send_midi_command(com_data,3); 
	}
	if(MIDICommand == 0x0c || MIDICommand == 0x0d)
	{
		
		//n1 = read1byte(ptr++);
		READ(n1);
		(*ptr) ++;
		com_data[0] = MIDIByte;
		com_data[1] = n1;
		send_midi_command(com_data,2); 
	}
	/*
	SYSTEM COMMANDS
    ===============
    These are ignored.
	*/
	if (MIDICommand == 0xF) 
	{
		if (MIDIChannel == 0) 
		{
			//DataLength = GetVLE(ptr++,&jmp_len);
			DataLength = GetVLE(*ptr,&jmp_len);
			(*ptr) ++;
			(*ptr) += jmp_len;
			for (Counter1 = 1; Counter1 <= DataLength; Counter1++) 
				//read1byte(ptr++);
				READ(TmpBuffByte);
				(*ptr) ++;
		}
		if (MIDIChannel == 2) 
		{
			//read2byte(ptr);
			READ(TmpBuffWord);
			(*ptr) += 2;
		} //Song Position Pointer
		if (MIDIChannel == 3) 
		{ 
			//read1byte(ptr++); 
			READ(TmpBuffByte);
			(*ptr) ++;
		} // Song Select
	}
	
 	ptrack->ptr = *ptr;
	return 1;
}

/*返回下一个track的地址，格式错误时返回0*/
static MIDIUInt32 parser_track_header(MIDIUInt32 *ptr,TRACK_CONTROL *ptrack)
{
	//0x4d,0x54,0x72,0x6b,0x00,0x00,
	//0x00,0xa4,0x00,0xff,0x03,0x08,0x75,0x6e,0x74,0x69,
	//0x74,0x6c,0x65,0x64,0x00,0xff,0x58,0x04,0x04,0x02,
	MIDIUInt32 TrackFlag;
	//unsigned long x;

	F_LSEEK(*ptr);
	//x = read4byte(ptr);
	READ(TrackFlag);
	TrackFlag = BSwapD(TrackFlag);
	if(TrackFlag!= MIDI_MTrk)
		return 0;
	*ptr += 4;
	//ptrack->len = read4byte(ptr);
	READ(ptrack->len);
	*ptr += 4;
	ptrack->len = BSwapD(ptrack->len);
	ptrack->ptr = *ptr;
	return *ptr + ptrack->len;
}





MIDIInt16 parser_midifile_header(void)
{
	//0x4d,0x54,0x68,0x64,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x78
	MIDIUInt8  i;
	//MIDIUInt16 y;
 	MIDIUInt32 ptr;
	
	ptr	= f_tell(fp);
	READ(midi_header.HeaderFlag);
	midi_header.HeaderFlag=BSwapD(midi_header.HeaderFlag);
	if(midi_header.HeaderFlag != MIDI_MThd)
		return MIDI_HEADER_FORMAT_ERROR;
	ptr +=4;

	READ(midi_header.RestLen);
	midi_header.RestLen	=BSwapD(midi_header.RestLen);
	if(midi_header.RestLen!= 0x00000006)
		return MIDI_HEADER_FORMAT_ERROR;
	ptr +=4;
	//format
	READ(midi_header.tracks_mode);
	midi_header.tracks_mode = BSwapW(midi_header.tracks_mode);
	if(midi_header.tracks_mode == TRACK_MODE_MUL_ASYNC)
		return MIDI_NOTSUPPORT_ASYNC;
	ptr +=2;
	//number of tracks 
	READ(midi_header.tracks_cnt);
	midi_header.tracks_cnt = BSwapW(midi_header.tracks_cnt);
	ptr +=2;
	READ(midi_header.tempo);
	midi_header.tempo = BSwapW(midi_header.tempo);
	ptr +=2;

	
	for(i=0;i<midi_header.tracks_cnt;i++)
	{
		track[i].track_end = 0;
		track[i].flow_cnt = 0;
		track[i].needTLV = 1;
		track[i].search_whole_words = 1;
		
		ptr = parser_track_header(&ptr,&track[i]);
		if(!ptr)
			return	MIDI_TRACK_PARSER_ERROR;
	}	
	return 1;
}


/*
	wait：	midi格式中需要等待的时长（相对于midi_header.tempo而言）
	return
			对于硬件定时器的而言需要等待的ticks
*/
MIDIUInt16 get_wait_time(MIDIUInt32 wait)
{
	if(wait == 0)
		return 0; 
	else 
		return (MIDIUInt16)(1.00*base_count*wait/midi_header.tempo);
}
MIDIUInt32 get_end(MIDIUInt32 begin,MIDIUInt32 wait)
{
	MIDIUInt32 end;
	end = begin + wait;
	return end;
}

/*
	定时器每TICKS_MS_CNT个ticks经历1秒
	time参数的含义为1/4音符需要的时间，单位为微秒
*/

void ChangeSpeed (MIDIUInt32 time)
{
	base_count = time*TICKS_CNT_PERSEC/1000000;
}

/*
	该函数用于播放midi音乐
	调用该函数之前请确保执行过parser_midifile_header
*/
MIDIInt16 play_midi_file()
{
	MIDIUInt8 	jmp_len;
	MIDIUInt8 	i;
	volatile MIDIUInt32 begin=0;
	MIDIUInt32 	wait;
	MIDIUInt32 	end;
	MIDIUInt8 	flag=0;

	int  res;

	//给base_count附上初值,假设每拍0.5s
	ChangeSpeed (500000);
	
	overflow_count=0;
	//midi_timer_start();
	midi_timer_stop();
	while(1)
	{
		flag = 0;
		for(i=0;i<midi_header.tracks_cnt;i++)
		{
			if(track[i].track_end ==1)
			{
				continue;
			}
			F_LSEEK(track[i].ptr);
			
			flag = 1;
			if(track[i].needTLV)
			{ 
				wait = GetVLE(track[i].ptr,&jmp_len);
				wait = get_wait_time(wait);
				begin = track[i].flow_cnt;
				end = get_end(begin,wait);
				track[i].ptr += jmp_len;
				track[i].flow_cnt = end;
				
				//midi_printf("track[%d].flow_cnt = %x\n",i,track[i].flow_cnt);
			}				
			if(track[i].flow_cnt <= overflow_count)
			{
				//midi_printf("do_event--------track[%d]\n",i);
				
				do_event(&track[i]);
				track[i].needTLV = 1;
			}
			else
			{
				track[i].needTLV = 0;
			}
		}
		//all tracks ended
		if(flag ==0)
		{
			break;
		}
		res = MidiPlayControl();
		if (res !=0)
		{
			return 0;
		}
				
	}
	midi_timer_stop();
	return MIDI_ERR_READ_FILE_END;
}


void ResetTracksStatus(void)
{
	MIDIUInt8 	i;
	for(i=0;i<midi_header.tracks_cnt;i++)
	{
		track[i].flow_cnt = overflow_count;
		//track[i].needTLV = 1;
	}
}
//=============================================================================
//MidiPlayControl:
//MIDI文件播放控制。失败返回零，成功返回非零
//-----------------------------------------------------------------------------
int MidiPlayControl(void) 
{
	OS_FLAGS Flags;
	INT8U err;
	static MIDIUInt8 PlayFlag=0;
	static MIDIUInt16 PlayCount=0;
	
	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
							(OS_FLAGS) MIDI_DELAY_FLAG ,
							(INT8U )OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME,
							(INT32U) ((1000/TICKS_CNT_PERSEC)+1),
							(INT8U *)&err);
	if (PlayFlag ==1)
	{
		//if (++PlayCount>(500/(1000/TICKS_CNT_PERSEC)))	// x/5ms
		if (++PlayCount > base_count)	// x/5ms
		
		{
			PlayFlag=0;
			midi_timer_stop();
			PauseMidiPlay();
		}
		return 0;
	}

	OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
						(OS_FLAGS) (MIDI_AUTO_PLAY_FLAG   |
									TRAMPLE_SPEEDING_FLAG |
									MIDI_PLAY_FLAG		  | 
									MIDI_STOP_FLAG		  |
									DEVICE_CHANGE_FLAG	  | 
									MIDI_PLAY_EN_DIS_FLAG),
						(INT8U ) OS_FLAG_WAIT_SET_ANY,
						(INT32U) 0,
						(INT8U *)&err);

	
	Flags =OSFlagQuery ((OS_FLAG_GRP *)pFlagGrpMidi,
						(INT8U *)&err);

	if ((Flags & DEVICE_CHANGE_FLAG)==DEVICE_CHANGE_FLAG)
	{
		return MIDI_ERR_DEVICE_CHANGE;
	}
	else if ((Flags & MIDI_PLAY_EN_DIS_FLAG)==MIDI_PLAY_EN_DIS_FLAG)
	{
		return MIDI_ERR_PLAY_EN_DIS;
	}
	else if (((Flags & MIDI_AUTO_PLAY_FLAG)==MIDI_AUTO_PLAY_FLAG) || 
		((Flags & TRAMPLE_SPEEDING_FLAG)==TRAMPLE_SPEEDING_FLAG))//||
		//((Flags & MIDI_PLAY_FLAG)==MIDI_PLAY_FLAG))
	{	
		midi_timer_start();
		PlayFlag=0;
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) MIDI_PLAY_FLAG | MIDI_STOP_FLAG,
				(INT8U )OS_FLAG_CLR,
				(INT8U	*)&err);
		//break;
	}
		
	else if ((Flags & MIDI_PLAY_FLAG)==MIDI_PLAY_FLAG)  
	{	
		midi_timer_start();
		PlayFlag = 1;
		PlayCount=0;
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
				(OS_FLAGS) MIDI_PLAY_FLAG,
				(INT8U )OS_FLAG_CLR,
				(INT8U	*)&err);
		//break;
	}
		
	else if ((Flags & MIDI_STOP_FLAG)==MIDI_STOP_FLAG)
	{
		//PauseMidiPlay();
		midi_timer_stop();
		//PlayFlag=0;
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
			(OS_FLAGS)  MIDI_STOP_FLAG,
			(INT8U )OS_FLAG_CLR,
			(INT8U	*)&err);	
	}	
	return 0;	
}

//=============================================================================
//ParseMIDI:
//分析MIDI文件。失败返回零，成功返回非零
//-----------------------------------------------------------------------------
int ParseMIDI(char*pszFileName)
{
	int res=0;
	
    //打开文件
	if (f_open(fp,pszFileName,FA_READ) != FR_OK)
		READERR;
	if(parser_midifile_header() > 0)
	{
	 	res = play_midi_file();
	}
	f_close(fp);
	return res;
}


u8 ReadMidiFile(char* Path)
{ 
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
					if ((strcmp(strchr((char*)filinfo.fname, '.')+1,"MID") ==0) || (strcmp(strchr((char*)filinfo.fname, '.')+1,"mid") ==0))	
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
	MIDIUInt8 TrackCnt;
	MIDIUInt8 DataBuff[3];
	
	for (TrackCnt=0; TrackCnt<16;TrackCnt++)
	{
		DataBuff[0] = 0xE0 | TrackCnt;	//事件
		DataBuff[1] = 0x00;				//滑音参数L
		DataBuff[2] = 0x40; 			//滑音参数H
		send_midi_command(DataBuff,3);
			
		DataBuff[0] = 0xB0 | TrackCnt;	//事件
		DataBuff[0] = 0x01;				//控制器号码
		DataBuff[0] = 0x00; 			//控制器参数
		send_midi_command(DataBuff,3);
			
		DataBuff[0] = 0xB0 | TrackCnt;	//事件
		DataBuff[0] = 0x40;				//控制器号码
		DataBuff[0] = 0x00; 			//控制器参数
		send_midi_command(DataBuff,3);
			
		DataBuff[0] = 0xB0 | TrackCnt;	//事件
		DataBuff[0] = 0x79;				//控制器号码
		DataBuff[0] = 0x00; 			//控制器参数		
		send_midi_command(DataBuff,3);
	}	
}


void Sand_3Byte_MidiCmd(BYTE Byte1,BYTE Byte2,BYTE Byte3)
{
    INT8U  err;
	#if 1   
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


