
#include "key.h"

#if 0
void io_key_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( KEY1_RCC 
						  | KEY2_RCC 
						  | KEY3_RCC 
						  | KEY4_RCC , ENABLE);
	/* Key */ 
	GPIO_InitStructure.GPIO_Pin =  KEY1_GPIO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin =  KEY2_GPIO; 
	GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  KEY3_GPIO; 
	GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  KEY4_GPIO; 
	GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
}
u8 get_io_key_val(void)
{ 
	/* Add your application code here
	*/
	uint8_t Key=NO_KEY;

	 if((!GPIO_ReadInputDataBit(KEY1_PORT,KEY1_GPIO)) && (!GPIO_ReadInputDataBit(KEY2_PORT,KEY2_GPIO)))
		Key = KEY_ESC_UP;
	else if((!GPIO_ReadInputDataBit(KEY1_PORT,KEY1_GPIO)) && (!GPIO_ReadInputDataBit(KEY3_PORT,KEY3_GPIO)))	
		Key = KEY_ESC_DOWN;
	else if((!GPIO_ReadInputDataBit(KEY1_PORT,KEY1_GPIO)) && (!GPIO_ReadInputDataBit(KEY4_PORT,KEY4_GPIO)))
		Key = KEY_ESC_OK;	
	else if(!GPIO_ReadInputDataBit(KEY1_PORT,KEY1_GPIO))
		Key = KEY_ESC;
	else if(!GPIO_ReadInputDataBit(KEY2_PORT,KEY2_GPIO))
		Key = KEY_UP;	
	else if(!GPIO_ReadInputDataBit(KEY3_PORT,KEY3_GPIO))
		Key = KEY_DOWN;
	else if(!GPIO_ReadInputDataBit(KEY4_PORT,KEY4_GPIO))
		Key = KEY_OK;
	return Key;
}



#endif

#if 1
void io_key_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	/* Key */ 
	GPIO_InitStructure.Pin =  KEY1_GPIO;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 
    GPIO_InitStructure.Pull = GPIO_PULLUP;  
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin =  KEY2_GPIO; 
	HAL_GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin =  KEY3_GPIO; 
	HAL_GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin =  KEY4_GPIO; 
	HAL_GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
	
	
}


#if 0
u8 get_io_key_val(void)
{ 
	/* Add your application code here
	 */
	uint8_t Key=NO_KEY;

	 if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY2_PORT,KEY2_GPIO)))
		Key = KEY_ESC_DOWN;
	else if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY3_PORT,KEY3_GPIO)))	
		Key = KEY_ESC_UP;
	else if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY4_PORT,KEY4_GPIO)))
		Key = KEY_ESC_OK;	
	else if(! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO))
		Key = KEY_ESC;
	else if(! HAL_GPIO_ReadPin(KEY2_PORT,KEY2_GPIO))
		Key = KEY_DOWN;	
	else if(! HAL_GPIO_ReadPin(KEY3_PORT,KEY3_GPIO))
		Key = KEY_UP;
	else if(! HAL_GPIO_ReadPin(KEY4_PORT,KEY4_GPIO))
		Key = KEY_OK;
	return Key;
}

#endif

#if 1
u8 get_io_key_val(void)
{ 
	/* Add your application code here
	 */
	uint8_t Key=NO_KEY;

	if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY2_PORT,KEY2_GPIO)))
		Key = KEY_ESC_UP;
	else if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY3_PORT,KEY3_GPIO)))	
		Key = KEY_ESC_DOWN;
	else if((! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO)) && (! HAL_GPIO_ReadPin(KEY4_PORT,KEY4_GPIO)))
		Key = KEY_ESC_OK;	
	else if(! HAL_GPIO_ReadPin(KEY1_PORT,KEY1_GPIO))
		Key = KEY_ESC;
	else if(! HAL_GPIO_ReadPin(KEY2_PORT,KEY2_GPIO))
		Key = KEY_UP;	
	else if(! HAL_GPIO_ReadPin(KEY3_PORT,KEY3_GPIO))
		Key = KEY_DOWN;
	else if(! HAL_GPIO_ReadPin(KEY4_PORT,KEY4_GPIO))
		Key = KEY_OK;
	return Key;
}

#endif


#endif
