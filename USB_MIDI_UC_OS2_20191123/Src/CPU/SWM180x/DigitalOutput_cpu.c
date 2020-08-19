/*--------------------------------------------------------------------------*/
/**@file     DigitalOutput_cpu.c
   @brief
   @details
   @author   hjt
   @date     2017-6-12
   @note
*/
/*----------------------------------------------------------------------------*/

#include "DigitalOutput_cpu.h"

/****************************************************************
 * 函数名：void DigitalOutput_GPIO_Config(void)  
 * 描述  ：数字输出I/O口配置            频率\脉冲输出
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void DigitalOutput_GPIO_Config(void)  
{ 
	PORT_Init(P_F_PORT, P_F_PIN, P_F_FUNMUX, DIGIT_IN_DIS);
} 

/****************************************************************
 * 函数名：void FrequeOutput_Configuration(void) 
 * 描述  ：频率输出配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void FrequeOutput_Configuration(void)
{  
    PWM_InitStructure InitStruct;
	
	DigitalOutput_GPIO_Config();
    //-----------------------------------------------------------
    InitStruct.clk_div =PWM_CLKDIV_16;      //PWM_CLKDIV_1、PWM_CLKDIV_2、... ... 、PWM_CLKDIV_128
    InitStruct.mode =PWM_MODE_INDEP;        //PWM_MODE_INDEP、PWM_MODE_COMPL、PWM_MODE_INDEP_CALIGN、PWM_MODE_COMPL_CALIGN
    InitStruct.cycleA =0xFFFF;      //A路周期
    InitStruct.hdutyA =0x8000;       //A路占空比
    InitStruct.deadzoneA =0;        //A路死区时长，取值0--63
    InitStruct.initLevelA =0;   //A路初始输出电平，0 低电平    1 高电平
    
    InitStruct.cycleB =0;       //B路周期
    InitStruct.hdutyB =0;       //B路占空比
    InitStruct.deadzoneB =0;        //B路死区时长，取值0--63
    InitStruct.initLevelB =0;   //B路初始输出电平，0 低电平    1 高电平
    
    InitStruct.HEndAIEn =DISABLE;       //A路高电平结束中断使能
    InitStruct.NCycleAIEn =DISABLE; //A路新周期开始中断使能
    InitStruct.HEndBIEn =DISABLE;       //B路高电平结束中断使能
    InitStruct.NCycleBIEn =DISABLE; //B路新周期开始中断使能

    PWM_Init(P_F_PWM_GROUP, &InitStruct);
	FrequePulseOutput_Stop();
}


/****************************************************************
 * 函数名：void PulseOutput_Configuration(void)
 * 描述  ：脉冲输出配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void PulseOutput_Configuration(void)
{  
    PWM_InitStructure InitStruct;
	DigitalOutput_GPIO_Config();
    //-----------------------------------------------------------
    InitStruct.clk_div =PWM_CLKDIV_16;      //PWM_CLKDIV_1、PWM_CLKDIV_2、... ... 、PWM_CLKDIV_128
    InitStruct.mode =2;        //PWM_MODE_INDEP、PWM_MODE_COMPL、PWM_MODE_INDEP_CALIGN、PWM_MODE_COMPL_CALIGN
    InitStruct.cycleA =(SystemCoreClock / 16 / 20);      //A路周期
    InitStruct.hdutyA =10;       //A路占空比
    InitStruct.deadzoneA =0;        //A路死区时长，取值0--63
    InitStruct.initLevelA =0;   //A路初始输出电平，0 低电平    1 高电平

    InitStruct.cycleB =0;       //B路周期
    InitStruct.hdutyB =0;       //B路占空比
    InitStruct.deadzoneB =0;        //B路死区时长，取值0--63
    InitStruct.initLevelB =0;   //B路初始输出电平，0 低电平    1 高电平

    InitStruct.HEndAIEn =DISABLE;       //A路高电平结束中断使能
    InitStruct.NCycleAIEn =DISABLE; //A路新周期开始中断使能
    InitStruct.HEndBIEn =DISABLE;       //B路高电平结束中断使能
    InitStruct.NCycleBIEn =DISABLE; //B路新周期开始中断使能

    PWM_Init(P_F_PWM_GROUP, &InitStruct); 
	FrequePulseOutput_Stop();

}




