
/*--------------------------------------------------------------------------*/
/**@file     key.h
   @brief
   @details
   @author
   @date   2011-4-26
   @note
*/
/*----------------------------------------------------------------------------*/

#include "ir_cpu.h"

/*
#if 0
void IR_Configuration(void)
{
    PORT_Init(IR_PORT, IR_PIN, FUNMUX_PULSE_IN, DIGIT_IN_EN);
    
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 0xFFFFFFFF, 0);
	TIMRG-> PCTRL = 0x00;
	//TIMRG-> PCTRL &= ~TIMRG_PCTRL_EN_Msk; 
    //TIMRG-> PCTRL &= ~TIMRG_PCTRL_HIGH_Msk;
	
	TIMRG-> PCTRL = TIMRG_PCTRL_HIGH_Msk;
	
	IRQ_Connect(IRQ16_31_TIMR, IRQ16_IRQ | IRQ_INT0 ,0);
	
    //IRQ_Connect(IRQ0_15_TIMR, IRQ15_IRQ, 1);
	
	
	
    TIMRG->IE |= TIMRG_IE_PULSE_Msk;
    TIMRG-> PCTRL |= TIMRG_PCTRL_EN_Msk;  
	
	TIMR_Start(TIMR0);
}
#endif

*/


//红外解码初始化
#if 1
void IR_Configuration(void)
{
	//PORT_Init(IR_PORT, IR_PIN, PORTA_PIN4_GPIO, DIGIT_IN_EN);
    GPIO_Init(IR_GPIO, IR_PIN, DIR_IN, PULL_UP_EN, PULL_DOWM_DIS, OPEN_DRAIN_DIS);
	//EXTI_Init(IR_GPIO, IR_PIN, EXTI_FALL_EDGE);
	EXTI_Init(IR_GPIO, IR_PIN, EXTI_RISE_EDGE);
	TIMR_Init(IR_TIMR, TIMR_MODE_TIMER, 0xFFFFFFFF, 0);
	IRQ_Connect(IRQ0_15_GPIOA4, IRQ15_IRQ ,0);
	IR_TIMR_Stop();
	EXTI_Open(IR_GPIO, IR_PIN);
	
}
#endif



//红外解码中断
void IRQ15_Handler(void)
{
	if (EXTI_State(IR_GPIO, IR_PIN))	
	{
		EXTI_Clear(IR_GPIO, IR_PIN);
		IR_decode();	
	}						
}




