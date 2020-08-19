#include "includes.h"
#include "stm32f2xx_hal.h"
#include "sys.h"
#include "usart1.h"	
#include "ff.h"
#include "midi_cfg.h"

#include "midi.h"

void midi_timer1_ISR(void)
{
	static unsigned char Cnt=0;
	INT8U err;
	
	Cnt++;
	if ((Cnt %=((1000/TICKS_CNT_PERSEC)))==0)
	{
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_DELAY_FLAG,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);
		midi_heart_beat();
	}
}


#if 0
/*
设置相应的COM口31.25K的波特率
*/
void midi_uart_init()
{
}





/*
	产生一个1000/(TICKS_CNT_PERSEC)毫秒的定时器
*/
void midi_timer_init()
{

}
#endif

void midi_timer_start(void)
{
	MidiStartstopFlag=1;
}

void midi_timer_stop(void)
{
	MidiStartstopFlag=0;
}

void midi_write_uart(MIDIUInt8 *dat,MIDIUInt16 len)
{
	HAL_UART_Transmit(&huart4,dat,len,10); 
	/*
	INT8U  err;
	HAL_UART_Transmit_IT(&huart4,dat,len); 
    OSFlagPend ((OS_FLAG_GRP *)pFlagGrpMidi,
                (OS_FLAGS) UART4_TX_OK_FLAG ,
                (INT8U )OS_FLAG_WAIT_SET_ALL| OS_FLAG_CONSUME,
                (INT32U) 0,
                (INT8U *)&err);
	*/
}


#if 0
/*
	words 要绘制的歌词
	time 绘制这些歌词所需要的时间
*/
void Draw_Bit_Words(char *words,unsigned long time)
{
	Lcd_1HalfClearScr( (0x00<<11) | (0x00<<5) | (0x1f)  )  ;
	lcd_draw_string(16,16,words);
}

void Draw_Whole_Words(char *words)
{
	Lcd_2HalfClearScr( (0x00<<11) | (0x1f<<5) | (0x00)  )  ;
	lcd_draw_string(16,136,words);
}
#endif
