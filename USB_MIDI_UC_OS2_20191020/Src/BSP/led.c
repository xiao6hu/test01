/******************** (C) COPYRIGHT 2012 ***************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库       
 * 库版本  ：ST3.5.0
**********************************************************************************/

#include "led.h"

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    WORK_LED_GPIO_CLK_ENABLE();
    GPIO_InitStructure.Pin = WORK_LED_PIN;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;       
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(WORK_LED_GPIO, &GPIO_InitStructure);
    WORK_LED(OFF);
}

/******************* (C) COPYRIGHT 2012*****END OF FILE************/
