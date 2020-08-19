/*--------------------------------------------------------------------------*/
/**@file     iic_cpu.c
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/
#include "iic_cpu.h"

#if 1
/****************************************************************
 * 函数名：void set_iic_sda_in(void) 
 * 描述  ：设置IIC数据端为输入状态
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void set_iic_sda_in(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.Pin =  IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_GPIO, &GPIO_InitStructure);
    IIC_SDA(1);
}


/****************************************************************
 * 函数名：void set_iic_sda_out(void)
 * 描述  ：设置IIC数据端为输出状态
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void set_iic_sda_out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.Pin =  IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  
    HAL_GPIO_Init(IIC_SDA_GPIO, &GPIO_InitStructure);
}

#endif

/****************************************************************
 * 函数名：void IIC_Init(void)
 * 描述  ：初始化IIC
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStructure.Pin =  IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_GPIO, &GPIO_InitStructure);
    GPIO_InitStructure.Pin =  IIC_SCL_PIN;
    HAL_GPIO_Init(IIC_SCL_GPIO, &GPIO_InitStructure);
}



