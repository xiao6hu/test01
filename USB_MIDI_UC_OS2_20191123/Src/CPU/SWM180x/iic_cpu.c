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
    GPIO_Init(IIC_SDA_GPIO, IIC_SDA_PIN, DIR_IN, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
}


/****************************************************************
 * 函数名：void set_iic_sda_out(void)
 * 描述  ：设置IIC数据端为输出状态
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void set_iic_sda_out(void)
{
	GPIO_Init(IIC_SDA_GPIO, IIC_SDA_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
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
    GPIO_Init(IIC_SCL_GPIO, IIC_SCL_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
	set_iic_sda_out();
	GPIO_SetBit(IIC_SCL_GPIO, IIC_SCL_PIN); 
	GPIO_SetBit(IIC_SDA_GPIO, IIC_SDA_PIN); 
}



