#ifndef __MIDI_CFG_H__
#define __MIDI_CFG_H__

#define MIDI_DBBUG

#ifdef MIDI_DBBUG
	#define midi_printf Uart_Printf
#else
	#define midi_printf(a,...)	
#endif

//һ���Ӳ�����Ӳ��tick��
#define TICKS_CNT_PERSEC	200

#endif /*__MIDI_CFG_H__*/
