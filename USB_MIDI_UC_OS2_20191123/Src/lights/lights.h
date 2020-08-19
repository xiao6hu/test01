

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "stm32f2xx.h"
#include "sys.h"
#include "usart1.h"	


#define  GROUP1_R_LED_BIT BIT(0) 
#define  GROUP1_R_LED_BIT_ON  GROUP1_R_LED_BIT 
#define  GROUP1_R_LED_BIT_OFF ~GROUP1_R_LED_BIT 

#define  GROUP1_G_LED_BIT BIT(1) 
#define  GROUP1_G_LED_BIT_ON  GROUP1_G_LED_BIT 
#define  GROUP1_G_LED_BIT_OFF ~GROUP1_G_LED_BIT 

#define  GROUP1_B_LED_BIT BIT(2) 
#define  GROUP1_B_LED_BIT_ON  GROUP1_B_LED_BIT 
#define  GROUP1_B_LED_BIT_OFF ~GROUP1_B_LED_BIT 

#define  GROUP1_W_LED_BIT BIT(3) 
#define  GROUP1_W_LED_BIT_ON  GROUP1_W_LED_BIT 
#define  GROUP1_W_LED_BIT_OFF ~GROUP1_W_LED_BIT 


#define  GROUP2_R_LED_BIT BIT(4) 
#define  GROUP2_R_LED_BIT_ON  GROUP2_R_LED_BIT 
#define  GROUP2_R_LED_BIT_OFF ~GROUP2_R_LED_BIT

#define  GROUP2_G_LED_BIT BIT(5) 
#define  GROUP2_G_LED_BIT_ON  GROUP2_G_LED_BIT 
#define  GROUP2_G_LED_BIT_OFF ~GROUP2_G_LED_BIT 


#define  GROUP2_B_LED_BIT BIT(6) 
#define  GROUP2_B_LED_BIT_ON  GROUP2_B_LED_BIT 
#define  GROUP2_B_LED_BIT_OFF ~GROUP2_B_LED_BIT 

#define  GROUP2_W_LED_BIT BIT(7) 
#define  GROUP2_W_LED_BIT_ON  GROUP2_W_LED_BIT 
#define  GROUP2_W_LED_BIT_OFF ~GROUP2_W_LED_BIT 






#define LIGHTS_CNT_DEFAULT_MAX_VAL        250

#define LIGHTS_CNT_DEFAULT_MIN_VAL        100
#define LIGHTS_CNT_SETP_VAL 50

#define SPEED1_LIGHTS_CNT_DEFAULT_MAX_VAL  (250)  // 1S   //250

#define SPEED1_LIGHTS_CNT_DEFAULT_MIN_VAL  (250)  // 1S   //100

#define SPEED2_LIGHTS_CNT_DEFAULT_MAX_VAL  (125)  //0.5S   //125

#define SPEED2_LIGHTS_CNT_DEFAULT_MIN_VAL  (125)  //0.5S   //50


#define LIGHTS_ON    0x01
#define LIGHTS_OFF   0x02


#define SYNC_CMD    0xff



#define MIN_LIGHTS_ADDR  0x01
#define MAX_LIGHTS_ADDR  0x05


#define MIDI_AutoPlayCntVal    500          //10s=10ms*1000
#define MIDI_AutoLightsCntVal  1500//9000        //3min=10ms*18000
#define IR_Rx_Delay_Cnt_VAL   10
/*
typedef enum
{
	WHITE,
	RED,		
	GREEN,	
	BLUE,
}LightsColourTypeDef;

extern LightsColourTypeDef lights_colour_status;
*/
#define WHITE	0
#define RED		1		
#define	GREEN	2
#define	BLUE	3


#define LIGHTS_UP_DELAY_TIME_CNT_VAL      500 //10MS*500,无人踩时灯亮的时间计数值


extern u16 LightsCntVal;
extern u8 LightsCnt;
extern u8 AutoLightsFlag;

extern u8 CurrentLightsAddr;
extern u8 LastLightsAddr;

extern u8 Mid_Max_CurrentLightsAddr;
extern u8 Mid_Max_LastLightsAddr;

extern u8 Mid_Min_CurrentLightsAddr;
extern u8 Mid_Min_LastLightsAddr;

extern u8 lights_mode;

extern u8 LightsColour; 
extern u8 lights_colour_status;

extern u8 min_lights_addr;
extern u8 max_lights_addr;

extern u8 Cycles;
extern u8 Status;
extern u8 flag;


extern u8 RGB_LightsStatus;
extern u16 RGB_Lights_Cnt;

//extern u8 Cycles;


//void AllLightsOff(void);
void lights_control(void);


void lights_control_cmd(u8 lights_addr,u8 lights_status);
void AutoLightsDelay(void);


void lights_colour_select(void);


void RGB_Lights_deal(void);
void RGB_LightsStatusSelect(void);

void LightsMode5(void);
#endif

