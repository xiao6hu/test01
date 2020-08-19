/*--------------------------------------------------------------------------*/
/**@file     key.h
   @brief
   @details
   @author
   @date   2011-4-26
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef __KEY_H__
#define __KEY_H__

//#include "includes.h"
#include "key_cpu.h"


//#include "io_key_tab.h"

/*按键类型*/
enum
{
    KEY_TYPE_IO =0x00,
    KEY_TYPE_IR,
};


/*按键门槛值*/
#define KEY_BASE_CNT  3
#define KEY_LONG_CNT  75
#define KEY_HOLD_CNT  15


/*按键状态*/
enum
{
    KEY_SHORT_UP = 0x00,
    KEY_LONG,        
    KEY_HOLD,        
    KEY_LONG_UP,    
};


enum
{
    KEY_NUM_0=0,
    KEY_NUM_1,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_4,
    KEY_NUM_5,
    KEY_NUM_6,
    KEY_NUM_7,
    KEY_NUM_8,
    KEY_NUM_9,
    //-------------------
    KEY_NUM_DP,

    //-------------------
    KEY_ESC,
	KEY_DOWN,
    KEY_UP,
    KEY_OK,
    //-------------------
	KEY_ESC_DOWN,
    KEY_ESC_UP,
    KEY_ESC_OK,
    //-------------------

    NO_KEY =0xFF,
};

#define Key_Up     KEY_OK
#define Key_Down   KEY_ESC    
#define Key_Right  KEY_DOWN
#define Key_Left   KEY_UP



#define AutoReturnTopLayerTime 150   //30S,200ms为单位
#define ManualReturnTopLayerTime 20 //3S, 10ms*15为单位

extern void KEY_Init(void);
extern void io_key_GPIO_Configuration(void);

extern u8 key_scan(void);
extern u8 get_io_key_val(void);
extern const u8 io_key_table[3][7];


//extern U8 CheckKey(void);
extern U8 CheckKey(u8 KeyVal);


extern U8 Key_Type_Change(U8 key_status, U8 key_val);

extern void TimingGetKeyStatus(void);


#define LINGHTS_KEY_RCC 	RCC_APB2Periph_GPIOD
#define LINGHTS_KEY_PIN  	GPIO_PIN_12
#define LINGHTS_KEY_GPIO 	GPIOD

#define MIDI_KEY_RCC 		RCC_APB2Periph_GPIOD 
#define MIDI_KEY_PIN    	GPIO_PIN_11
#define MIDI_KEY_GPIO 	 	GPIOD


#define START_KEY_PIN	GPIO_PIN_0  //启动
#define START_KEY_GPIO	GPIOE
#define START_KEY_RCC	RCC_APB2Periph_GPIOE

#define RESET_KEY_PIN	GPIO_PIN_1  //复位
#define RESET_KEY_GPIO	GPIOE
#define RESET_KEY_RCC	RCC_APB2Periph_GPIOE


extern  u8 Midi_En_dis;
extern  u8 Lights_En_dis;
void midi_linghts_key_init(void);
void get_midi_linghts_key(void);

void start_reset_key_init(void);
void get_start_reset_key(void);

void PostMsg_key_scan(void);


extern u8 GlobalKeyVal;


#endif



