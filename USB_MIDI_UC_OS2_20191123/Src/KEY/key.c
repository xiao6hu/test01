/*--------------------------------------------------------------------------*/
/**@file    key.c
   @brief   按键驱动
   @details
   @author  hjt
   @date    2017-6-12
   @note    
*/


/*----------------------------------------------------------------------------*/

#include "includes.h"
#include "io_key_tab.h"
#include "key.h"
#include "lights.h"

#include "Keyboard.h"


//#include "V_F_ADC_cpu.h"

u8 Midi_En_dis=0;
u8 Lights_En_dis=0;


static u8  key_type;
static u8 key_filter(u8 key);

u8 GlobalKeyVal =NO_KEY;

static u8  used_key = NO_KEY;
static u8  old_key = NO_KEY;
static u8  key_counter =0;
		
/*
//定时获取按键状态
void TimingGetKeyStatus(void)
{
   	//if (Field_TIM_InitFlag)
	{

		//Field_TIM_InitFlag=0;


		CheckKey();
	}
}
*/




void KEY_Init(void)
{
	io_key_GPIO_Configuration();
	midi_linghts_key_init();
	start_reset_key_init();
	
}

/*----------------------------------------------------------------------------*/
/**@brief   按键去抖函数，输出稳定键值
   @param   key：键值
   @return  稳定按键
   @author  Change.tsai
   @note    u8 keyFilter(u8 key)
*/
/*----------------------------------------------------------------------------*/
static u8 key_filter(u8 key) 
{/*
    static u8  used_key = NO_KEY;
    static u8  old_key = NO_KEY;
    static u8  key_counter =0;
*/
    if (old_key != key)
    {
        key_counter = 0;
        old_key = key;
    }
    else
    {
        key_counter++;
        if (key_counter == KEY_BASE_CNT)
        {
            used_key = key;
        }
    }
    return used_key;
}

/*
U8 IR_KeyVal_Change(U8 key_val)
{
	switch (key_val)
	{
		case 0x02: key_val = KEY_ESC; break;

		case 0xa8: key_val = KEY_DOWN; break;
			      
		case 0x90: key_val = KEY_UP; break;
			      
		case 0xc2: key_val = KEY_OK; break;
			
		default: key_val = NO_KEY; break;
	}
		
	return key_val;
}
*/


void PostMsg_key_scan(void)
{
	u8 KeyMsg=NO_KEY;
	KeyMsg = key_scan();
	OSQPost(Key_Q,&KeyMsg);	
}


/*----------------------------------------------------------------------------*/
/**@brief   按键-消息转换函数,按键产生顺序：短按抬起/长按-->连按
   @param   key：按键序号
   @param   key_type：按键类型
   @return  void
   @author  Change.tsai
   @note    void key2msg(u8 key, u8 key_type)
*/
/*----------------------------------------------------------------------------*/
u8 key_scan(void) 
{
    volatile static u8 last_key = NO_KEY;
    volatile static u8 key_press_counter;
    volatile u8 cur_key, key_status, back_last_key;
	
	  volatile u8 key_val;
	
	//static u8 KeyMsgCnt=0;
	
    cur_key = get_io_key_val();
    if (cur_key == NO_KEY)
    {
		
		#ifdef IR_REMOTE_EN
        cur_key = IR_key();                             //检查是否有遥控

        if (cur_key != NO_KEY)
        {
            key_type = KEY_TYPE_IR;
        }
		#endif
		//return NO_KEY;
    }
    else
    {
        key_type = KEY_TYPE_IO;
    }

    cur_key = key_filter(cur_key);                    //过滤按键，相当于去抖

    back_last_key = last_key;

    if (cur_key == last_key)                            //长时间按键
    {
        if (cur_key == NO_KEY)
            //return;  
		    return NO_KEY;
		
        key_press_counter++;

        if (key_press_counter == KEY_LONG_CNT)          //长按
        {
            
			//kick_key_voice();  
            key_status = KEY_LONG;
        }
        else if (key_press_counter == (KEY_LONG_CNT + KEY_HOLD_CNT))        //连按
        {
            key_status = KEY_HOLD;
            key_press_counter = KEY_LONG_CNT;
			 	
        }
        else
        {
            //return;
			return NO_KEY;
        }
    }
    else  //cur_key = NO_KEY, 抬键
    {
        last_key = cur_key;

        //if ((key_press_counter < KEY_LONG_CNT) && (cur_key == NO_KEY))      //短按抬起
		if ((key_press_counter < KEY_LONG_CNT) && (cur_key != back_last_key))      //短按抬起	
		
        {
            //kick_key_voice();
            key_press_counter = 0;
            key_status = KEY_SHORT_UP;
        }
        else
        {
            key_press_counter = 0;
            //return;
			return NO_KEY;
			
        }
    }
    //kick_key_voice();
    //key_msg_filter(key_status, back_last_key);

	
	
	
	
	if (key_type == KEY_TYPE_IO)
    {
		
		//key_val = io_key_table[key_status][back_last_key];    
    }
	
	#ifdef IR_REMOTE_EN
    else if (key_type == KEY_TYPE_IR)
    {
        //key_val = ir_key_table[key_status][back_last_key];
		
		
		//back_last_key = IR_KeyVal_Change(back_last_key);
	
    }
	#endif
	
	
	else
	{
		key_val = NO_KEY;	
	}
	
	//key_val =back_last_key;
	
	key_val = Key_Type_Change(key_status, back_last_key);
	
	
	
	/*
	KeyMsg[KeyMsgCnt] =key_val;
	OSQPost(Key_Q,(u8*)&KeyMsg[KeyMsgCnt]);
	KeyMsgCnt++;								//执行下个缓冲区，避免覆盖原来的按键数据
	if(KeyMsgCnt==KEY_MESSAGES)
	{
		KeyMsgCnt=0;
	}
	*/
	return key_val;
}



U8 Key_Type_Change(U8 key_status, U8 key_val)
{

			//FieldPositiveOut(1);
	//FieldNegativeOut(1);
	
	
	switch (key_status)
	{
		case KEY_SHORT_UP: key_val |= (KEY_SHORT_UP <<6 ); break;

		case KEY_LONG: key_val |= (KEY_LONG <<6 ); break;
			       
		case KEY_HOLD: key_val |= (KEY_HOLD <<6 ); break;
			      
		case KEY_LONG_UP: key_val |= (KEY_LONG_UP <<6 ); break;
			
		default: key_val = NO_KEY; break;
	}

			//FieldPositiveOut(1);
	//FieldNegativeOut(0);

	return key_val;
}


void midi_linghts_key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    __HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitStructure.Pin = MIDI_KEY_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(MIDI_KEY_GPIO, &GPIO_InitStructure); 
    HAL_GPIO_WritePin(MIDI_KEY_GPIO, MIDI_KEY_PIN, GPIO_PIN_SET);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.Pin = LINGHTS_KEY_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(LINGHTS_KEY_GPIO, &GPIO_InitStructure);
    HAL_GPIO_WritePin(LINGHTS_KEY_GPIO, LINGHTS_KEY_PIN, GPIO_PIN_SET); 
}


void start_reset_key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    __HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitStructure.Pin = START_KEY_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(START_KEY_GPIO, &GPIO_InitStructure); 
    HAL_GPIO_WritePin(START_KEY_GPIO, START_KEY_PIN, GPIO_PIN_SET);
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.Pin = RESET_KEY_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(RESET_KEY_GPIO, &GPIO_InitStructure);
    HAL_GPIO_WritePin(RESET_KEY_GPIO, RESET_KEY_PIN, GPIO_PIN_SET); 
}










/*
void get_midi_linghts_key(void)
{
	static u8 lights_key_cnt=0;
	static u8 midi_key_cnt=0;
	INT8U err;
	
	
    if (!HAL_GPIO_ReadPin(LINGHTS_KEY_GPIO,LINGHTS_KEY_PIN)) 
	{	
		if (++lights_key_cnt>10)
		{	
			lights_key_cnt=0;
			Lights_En_dis=0;
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHTS_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
			
		}
	}
	else
	{
		lights_key_cnt=0;
		if (!Lights_En_dis)
		{
			TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,0);
			Lights_En_dis=1;
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHTS_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);		
		}		
	}

    if (!HAL_GPIO_ReadPin(MIDI_KEY_GPIO,MIDI_KEY_PIN))  
	{	
		if (++midi_key_cnt>10)
		{	
			midi_key_cnt=0;
			Midi_En_dis=0;
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
			
		}
	}
	else
	{
		midi_key_cnt=0;
		
		if (!Midi_En_dis)
		{
			Midi_En_dis=1;
		
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);	
			
		}
	}
}
*/
void get_midi_linghts_key(void)
{
	static u8 lights_key_cnt=0;
	static u8 midi_key_cnt=0;
	INT8U err;
	
	
    if (!HAL_GPIO_ReadPin(LINGHTS_KEY_GPIO,LINGHTS_KEY_PIN)) 
	{	
		if (++lights_key_cnt>10)
		{	
			lights_key_cnt=0;
			
			Lights_En_dis=0;

			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHTS_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
			
			
		}
	}
	else
	{
		lights_key_cnt=0;
		if (!Lights_En_dis)
		{
			Lights_En_dis=1;

			TxDeviceCmd(DEFAULT_SLAVE_ADDR,LIGHTS_OFF,0);
			Lights_En_dis=1;
			
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) AUTO_LIGHTS_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);		
		}		
	}

	
	
    if (!HAL_GPIO_ReadPin(MIDI_KEY_GPIO,MIDI_KEY_PIN))  
	{	
		if (++midi_key_cnt>10)
		{	
			midi_key_cnt=0;
			
			Keyboard_Enable_Flag=0;
			
			
			Keyboard_Init_Flag=0;
			
			
			Midi_En_dis=0;
			/*
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
			*/
			
		}
	}
	else
	{
		midi_key_cnt=0;
		
		if (!Midi_En_dis)
		{
			Midi_En_dis=1;
			
			Keyboard_Enable_Flag=1;
			
			
			Keyboard_Init_Flag=1;
			
		/*
			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);	
			*/
			
		}
	}
	
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) MIDI_PLAY_EN_DIS_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
}





void get_start_reset_key(void)
{
	static u8 start_key_cnt=0;
	static u8 reset_key_cnt=0;
	INT8U err;
	
	
    if (!HAL_GPIO_ReadPin(START_KEY_GPIO,START_KEY_PIN)) 
	{	
		if (++start_key_cnt>10)
		{	
			start_key_cnt=0;
			
			//start_En_dis=0;

			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) START_KEY_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);	
				
			//OSTimeDlyHMSM(0, 0,10,0);
			
		}
	}
	else
	{
		//start_En_dis=0;
		start_key_cnt=0;
		
	}	
	
 
    if (!HAL_GPIO_ReadPin(RESET_KEY_GPIO,RESET_KEY_PIN)) 
	{	
		if (++reset_key_cnt>10)
		{	
			reset_key_cnt=0;
			
			//reset_En_dis=0;

			OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) RESET_KEY_FLAG ,
					(INT8U )OS_FLAG_SET,
					(INT8U *)&err);		
		}
	}
	else
	{
		//reset_En_dis=0;
		reset_key_cnt=0;
		
		OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
					(OS_FLAGS) RESET_KEY_FLAG ,
					(INT8U )OS_FLAG_CLR,
					(INT8U *)&err);	
		
	}	
}

