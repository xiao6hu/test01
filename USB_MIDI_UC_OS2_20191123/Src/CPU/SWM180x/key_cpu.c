
#include "key.h"
#include "key_cpu.h"

/****************************************************************
 * 函数名：void key_GPIO_Configuration(void) 
 * 描述  ：I/O口按键I/O口配置  
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void io_key_GPIO_Configuration(void)
{	
    GPIO_Init(KEY1_GPIO, KEY1_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(KEY2_GPIO, KEY2_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(KEY3_GPIO, KEY3_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(KEY4_GPIO, KEY4_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS); 
}

/****************************************************************
 * 函数名：u8 get_io_key_val(void)
 * 描述  ：读取I/O口按键值 
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
u8 get_io_key_val(void)
{ 
	uint8_t Key=NO_KEY;

	if((!GPIO_GetBit(KEY1_GPIO, KEY1_PIN)) && (!GPIO_GetBit(KEY2_GPIO, KEY2_PIN)))
		Key = KEY_ESC_DOWN;
	else if((!GPIO_GetBit(KEY1_GPIO, KEY1_PIN)) && (!GPIO_GetBit(KEY3_GPIO, KEY3_PIN)))	
		Key = KEY_ESC_UP;
	else if((!GPIO_GetBit(KEY1_GPIO, KEY1_PIN)) && (!GPIO_GetBit(KEY4_GPIO, KEY4_PIN)))
		Key = KEY_ESC_OK;	
	else if(!GPIO_GetBit(KEY1_GPIO, KEY1_PIN))
		Key = KEY_ESC;
	else if(!GPIO_GetBit(KEY2_GPIO, KEY2_PIN))
		Key = KEY_DOWN;	
	else if(!GPIO_GetBit(KEY3_GPIO, KEY3_PIN))
		Key = KEY_UP;
	else if(!GPIO_GetBit(KEY4_GPIO, KEY4_PIN))
		Key = KEY_OK;
	return Key;
}

