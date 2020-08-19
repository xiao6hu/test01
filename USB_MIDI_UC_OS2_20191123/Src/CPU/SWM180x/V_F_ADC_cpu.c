
#include "V_F_ADC_cpu.h"

/****************************************************************
 * 函数名：void ALM_GPIO_Config(void) 
 * 描述  ：报警输出I/O口配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void ALM_GPIO_Config(void) 
{
    GPIO_Init(AL_GPIO, AL_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(AH_GPIO, AH_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
	AL_Out(OFF);
	AH_Out(OFF);
}

#if 0
/****************************************************************
 * 函数名：void V_F_ADC_GPIO_Config(void) 
 * 描述  ：V_F_ADC    I/O口配置 
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void V_F_ADC_GPIO_Config(void)  
{ 

  GPIO_InitTypeDef GPIO_InitStructure; 

  /* GPIOA and GPIOB clock enable */ 
  RCC_APB2PeriphClockCmd(CF_RCC | CP_RCC | RCC_APB2Periph_AFIO, ENABLE ); 

  /*GPIOA Configuration: CF_TIM_NUM channel 1 and 2 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin = CF_GPIO; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 上拉输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(CF_PORT, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = CP_GPIO; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出 
  GPIO_Init(CP_PORT, &GPIO_InitStructure); 
} 

//-------------------------------------------------
//产生2MHZ的方波信号
//-------------------------------------------------
void V_F_ADC_Configuration(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure; 

	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	
	
	V_F_ADC_GPIO_Config();

	TIM_DeInit(CP_TIM_NUM);
    /*PCLK1经过2倍频后作为TIM3的时钟源等于72MHz*/  
    RCC_APB2PeriphClockCmd(CP_TIM_RCC, ENABLE);  

    //GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5

    //TIM1->CR2 = 0x0000;	
	
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period = ((SystemCoreClock / CP_FREQ)-1);  

	TIM_TimeBaseStructure.TIM_Prescaler = 0;                                    //设置预分频：预分频=0，即为72/0=72MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     //设置时钟分频系数：不分频  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //向上计数溢出模式  
    TIM_TimeBaseInit(CP_TIM_NUM, &TIM_TimeBaseStructure);  

    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //配置为PWM模式1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = ((SystemCoreClock / CP_FREQ) /2);           //设置跳变值，当计数器计数到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //当定时器计数值小于CCR1时为高电平  
    TIM_OC1Init(CP_TIM_NUM, &TIM_OCInitStructure);                                    //使能通道1     
    TIM_OC1PreloadConfig(CP_TIM_NUM, TIM_OCPreload_Enable); 
	 
	 
	TIM_BDTRInitStruct.TIM_OSSIState= TIM_OSSIState_Disable; 
	TIM_BDTRInitStruct.TIM_OSSRState= TIM_OSSRState_Disable; 
	TIM_BDTRInitStruct.TIM_LOCKLevel= TIM_LOCKLevel_3; 
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	 
	TIM_BDTRConfig(CP_TIM_NUM, &TIM_BDTRInitStruct); 
	 
	
	 
	TIM_CtrlPWMOutputs(CP_TIM_NUM, ENABLE); 

    TIM_ARRPreloadConfig(CP_TIM_NUM, ENABLE);                                         //使能TIM1重载寄存器ARR  
    /* CF_TIM_NUM enable counter */  
    TIM_Cmd(CP_TIM_NUM, ENABLE);                                                      //使能TIM1  

	
	
}




/****************************************************************
 * 函数名：void  V_F_ADC_Counter_Config(void) 
 * 描述  ：配置复用输出PWM时用到的I/O 
 * 输入  ：无 
 * 输出  ：无 
 * 调用  ：main()调用 
 ***************************************************************/ 
void  V_F_ADC_Counter_Config(void) 
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(CF_RCC , ENABLE);
	GPIO_InitStructure.GPIO_Pin = CF_GPIO; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           // 推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(CF_PORT, &GPIO_InitStructure); 

	GPIO_WriteBit(CP_PORT, CP_GPIO, Bit_RESET);
	GPIO_WriteBit(CF_PORT, CF_GPIO, Bit_RESET);
	
	
	
    RCC_APB1PeriphClockCmd(CF_TIM_RCC , ENABLE);
	TIM_DeInit(CF_TIM_NUM);
	
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;	/* 自动重装载寄存器周期的值(计数值) */

    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (0);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */

    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */

    TIM_TimeBaseInit(CF_TIM_NUM, &TIM_TimeBaseStructure);


	TIM_TIxExternalClockConfig(CF_TIM_NUM, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0);

	TIM_SelectInputTrigger(CF_TIM_NUM, TIM_TS_ETRF);
	//TIM2_NVIC_Configuration();

	//TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Repetitive);//TIM_OPMode_Single);

	//TIM_ClearFlag(Field_TIM_NUM, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    //TIM_ITConfig(Field_TIM_NUM,TIM_IT_Update,ENABLE);

	TIM_Cmd(CF_TIM_NUM, DISABLE);

 }
#endif





/****************************************************************
 * 函数名：void Field_Out_GPIO_Config(void) 
 * 描述  ：励磁信号I/O口配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 
void Field_Out_GPIO_Config(void)  
{ 
    GPIO_Init(FieldPositive_GPIO, FieldPositive_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(FieldNegative_GPIO, FieldNegative_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
	
    GPIO_Init(MtsnsrPositive_GPIO, MtsnsrPositive_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
    GPIO_Init(MtsnsrNegative_GPIO, MtsnsrNegative_PIN, DIR_OUT, PULL_UP_DIS, PULL_DOWM_DIS, OPEN_DRAIN_DIS);

	FieldPositiveOut(1);
	FieldNegativeOut(1);
	
	MtsnsrPositiveOut(0);
	MtsnsrNegativeOut(0);
	
	ALM_GPIO_Config(); 	 
} 


/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
/*
void Field_Out_TIM_NVIC_Configuration(void)
{
    //IRQ_Connect(IRQ16_31_UART3, IRQ16_IRQ, 0);
}
*/

/****************************************************************
 * 函数名：void Field_Out_TIM_Configuration(void)
 * 描述  ：励磁信号定时器配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 

void Field_Out_TIM_Configuration(void)
{
    TIMR_Init(Field_TIM_NUM, TIMR_MODE_TIMER , SystemCoreClock/100, 1);
	
	//IRQ_Connect(IRQ16_31_TIMR, IRQ16_IRQ | IRQ_INT1 ,2);
	IRQ_Connect(IRQ0_15_TIMR, IRQ5_IRQ,2);
    TIMR_Start(Field_TIM_NUM);
}



/****************************************************************
 * 函数名：void Field_Out_Configuration(void)
 * 描述  ：励磁信号配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 

void Field_Out_Configuration(void)
{
	Field_Out_GPIO_Config();
	Field_Out_TIM_Configuration();
	//Mtsnsr_Out_Configuration();
	FieldSampling_Configuration();	
}

#if 0
/****************************************************************
 * 函数名：void Field_Out_Configuration(void)
 * 描述  ：励磁信号配置
 * 输入  ：无 
 * 输出  ：无 
 ***************************************************************/ 

void Mtsnsr_Out_Configuration(void)
{
	PWM_InitStructure InitStruct;
	
	PORT_Init(MtsnsrPositive_PORT, MtsnsrPositive_PIN, MtsnsrPositive_FUNMUX, DIGIT_IN_DIS);
	PORT_Init(MtsnsrNegative_PORT, MtsnsrNegative_PIN, MtsnsrNegative_FUNMUX, DIGIT_IN_DIS);

    //-----------------------------------------------------------
    InitStruct.clk_div =PWM_CLKDIV_16;      //PWM_CLKDIV_1、PWM_CLKDIV_2、... ... 、PWM_CLKDIV_128
    InitStruct.mode =PWM_MODE_COMPL;        //PWM_MODE_INDEP、PWM_MODE_COMPL、PWM_MODE_INDEP_CALIGN、PWM_MODE_COMPL_CALIGN
    InitStruct.cycleA =(SystemCoreClock/16/MtsnsrFreque-1);      //A路周期
    InitStruct.hdutyA =(SystemCoreClock/16/MtsnsrFreque/2);       //A路占空比
    InitStruct.deadzoneA =0;        //A路死区时长，取值0--63
    InitStruct.initLevelA =0;   //A路初始输出电平，0 低电平    1 高电平
    
    InitStruct.cycleB =(SystemCoreClock /16/MtsnsrFreque-1);       //B路周期
    InitStruct.hdutyB =(SystemCoreClock /16/MtsnsrFreque/2);       //B路占空比
    InitStruct.deadzoneB =0;        //B路死区时长，取值0--63
    InitStruct.initLevelB =0;   //B路初始输出电平，0 低电平    1 高电平
    
    InitStruct.HEndAIEn =DISABLE;       //A路高电平结束中断使能
    InitStruct.NCycleAIEn =DISABLE; //A路新周期开始中断使能
    InitStruct.HEndBIEn =DISABLE;       //B路高电平结束中断使能
    InitStruct.NCycleBIEn =DISABLE; //B路新周期开始中断使能

    PWM_Init(Mtsnsr_PWM_GROUP, &InitStruct);
	Mtsnsr_Out_Stop();

}
#endif




