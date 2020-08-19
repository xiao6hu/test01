

#include "timr.h"
#include "mid.h"
#include "lights.h"

#include "timr_delay.h"

#include "key.h"

TIM_HandleTypeDef htim7;

/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void TIM7_NVIC_Configuration(void)
{
/*
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    */
    //HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1:);
    HAL_NVIC_SetPriority(TIM7_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

void TIM7_Configuration(void)
{

#if 0
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);


    TIM_DeInit(TIM7);

    TIM_TimeBaseStructure.TIM_Period = ((SystemCoreClock / 100 /72)-1);	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */

    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72-1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */

    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIM7, TIM_UpdateSource_Regular);
	
	TIM7_NVIC_Configuration();

	//TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Repetitive);//TIM_OPMode_Single);
	
	TIM_ARRPreloadConfig(TIM7, ENABLE);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM7, ENABLE);
#endif



#if 1
    //TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);

    __HAL_RCC_TIM7_CLK_ENABLE();
    
    htim7.Instance=TIM7;
    htim7.Init.Period= ((SystemCoreClock / 50 /72)-1);	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    htim7.Init.Prescaler= (72-1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    htim7.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; 		/* ������Ƶ */

    htim7.Init.CounterMode=TIM_COUNTERMODE_UP; /* ���ϼ���ģʽ */

    HAL_TIM_Base_Init(&htim7);

	//TIM_UpdateRequestConfig(TIM7, TIM_UpdateSource_Regular);
	
	TIM7_NVIC_Configuration();

	//TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Repetitive);//TIM_OPMode_Single);
	
	//TIM_ARRPreloadConfig(TIM7, ENABLE);
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_IT_UPDATE);							    		/* �������жϱ�־ */
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&htim7);
#endif
    
}




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles Field_TIM_NUM interrupt request.
  * @param  None
  * @retval : None
  */
void TIM7_IRQHandler(void)
{	
	#define MIDI_AutoPlayCntVal    500          //10s=10ms*1000
	#define MIDI_AutoLightsCntVal  1500//9000        //3min=10ms*18000
    #define IR_Rx_Delay_Cnt_VAL   10
	//static u16 MIDI_AutoPlayCnt=0;
	static u8 IR_Rx_Delay_Cnt=0;
	
	//if ( TIM_GetITStatus(TIM7 , TIM_IT_Update) != RESET ) 
	if(__HAL_TIM_GET_FLAG(&htim7, TIM_IT_UPDATE)!= RESET)
	{	
		//TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);  
        __HAL_TIM_CLEAR_FLAG(&htim7, TIM_IT_UPDATE);

	
		//CheckKey();
		
		get_midi_linghts_key();
		
		
		if (++IR_Rx_Delay_Cnt >IR_Rx_Delay_Cnt_VAL)             //200ms
	    {
			IR_Rx_Delay_Cnt =0;
			IR_Rx_Cnt =0;
		}

		//AutoLightsFlag=1;
		if ((AutoLightsFlag || !Midi_En_dis) && Lights_En_dis)
		{
			if (++LightsCnt > LightsCntVal)             //250ms
			{
				LightsCnt =0;
				lights_control();
			}
		}
		else
		{
			LightsCnt=0;
		}
		
		if (Midi_En_dis)
		{
			MIDI_AutoPlayCnt++;
			
			if ((MIDI_AutoPlayCnt % MIDI_AutoPlayCntVal) == 0)    //10s
			{	
				MIDI_AutoPlayFlag =1;
			}

			if ((MIDI_AutoPlayCnt % MIDI_AutoLightsCntVal) == 0)  //3����
			{	
				MIDI_AutoPlayCnt=0;
				AutoLightsFlag =1;
			}	
		}		
		else
		{
			MIDI_AutoPlayFlag =0;
			MIDI_AutoPlayCnt=0;
			AutoLightsFlag =0;
		}
	} 	              			 	
}

