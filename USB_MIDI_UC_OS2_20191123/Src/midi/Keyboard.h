#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "stm32f2xx_hal.h"
#include "sys.h"
#include "usart1.h"	

#include "midi.h"	


#define NOTE_ON 0x90
#define NOTE_OFF 0x80

extern const u8 Keyboard_21key_NoteTable[21];
extern u8 Keyboard_Enable_Flag;
extern u8 Keyboard_Init_Flag;


void Keyboard_21key(void);

#endif


