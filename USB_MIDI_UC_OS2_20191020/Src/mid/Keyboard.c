

#include "Keyboard.h"

u8 Keyboard_Enable_Flag=0;

u8 Keyboard_Init_Flag=1;


const u8 Keyboard_21key_NoteTable[21]=
{
	48,	//C-1
	50,	//D-1
	52,	//E-1
	53,	//F-1
	55,	//G-1
	57,	//A-1
	59,	//B-1

	60,	//C
	62,	//D
	64,	//E
	65,	//F
	67,	//G
	69,	//A
	71,	//B

	72,	//C+1
	74,	//D+1
	76,	//E+1
	77,	//F+1
	79,	//G+1
	81,	//A+1
	83,	//B+1	
};




void Keyboard_21key(void)
{
	static u8 PreviousNote=60;
	u8 CurrentNote;
	u8 NoteNumber;
	
	if (1==Keyboard_Init_Flag)
	{	
		Keyboard_Init_Flag=0;
		
		Sand_3Byte_MidiCmd(0xB0 | 1,	 	//�¼�
						   0x07, 			//����������
						   0x7F);        	//����������
		/*
		Sand_3Byte_MidiCmd(0xB0 | 1,	 	//�¼�
						   0x40, 			//����������
						   119);        	//����������
		*/
		
		/*
		Sand_3Byte_MidiCmd(0xB0 | 1,	    //�¼�
						   0x00,			//����������
						   0x01); 			//����������
		*/
		Sand_3Byte_MidiCmd(0xC0 | 1,	    //�¼�
						   0x01,			//����������
						   0x00); 			//����������
		
	}
	
	if (Packet.addr>0)
	{
		NoteNumber = (Packet.addr-1)%(21);
		
		Sand_3Byte_MidiCmd(NOTE_OFF|1,	 //�¼�
						   PreviousNote, //����
						   0x00);        //����

		CurrentNote=Keyboard_21key_NoteTable[NoteNumber];
		PreviousNote=CurrentNote;
		
		Sand_3Byte_MidiCmd(NOTE_ON|1,	 //�¼�
		                   CurrentNote,  //����
						   0x7F);	     //����
	}
}



