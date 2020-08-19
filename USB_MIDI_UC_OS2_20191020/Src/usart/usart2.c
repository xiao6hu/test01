/******************** (C) COPYRIGHT 2012 **************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：The One STM32开发板
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.5.0
 *
 * 淘宝    ：http://sztheone.taobao.com/
**********************************************************************************/
#include "includes.h"
#include "usart2.h"
#include <stdarg.h>
						
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim7;

u8 Uart2_TimeInterval;
u8 Uart2_TxBitLen=10;
u32 Uart2_baudrate=9600;
u8 Uart2_BufferIndex;

u8 Uart2_CommandBuffer[UART2_BUFFERLENTH];

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
	
	
	
  huart2.Instance = USART2;
  huart2.Init.BaudRate = Uart2_baudrate;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    //Error_Handler();
  }
  
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  
  TIM7_Configuration();
  __HAL_UART_ENABLE(&huart2);
  
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void Uart2SendData(unsigned char ch)
{
	/* 将Printf内容发往串口 */
    HAL_UART_Transmit(&huart2,&ch,1,10);
    while (!__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC)); 
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_TC);
}


void TIM7_Configuration(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
    htim7.Instance=TIM7;
    htim7.Init.Period= (((SystemCoreClock / Uart2_baudrate/120) * Uart2_TxBitLen)-1);	/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    htim7.Init.Prescaler= (120);				    /* 时钟预分频数 120M/120 */
    htim7.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; 		/* 采样分频 */
    htim7.Init.CounterMode=TIM_COUNTERMODE_UP; /* 向上计数模式 */
    HAL_TIM_Base_Init(&htim7);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);							    		/* 清除溢出中断标志 */
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE(&htim7);
}


/****************************************************************
 * 函数名：void TIM6_IRQHandler(void)
 * 描述  ：RS485接收超时中断
 * 输入  ：无
 * 输出  ：无 
 ***************************************************************/ 
void TIM7_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE))
	{	
        __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
		Uart2_TimeInterval++;
		if((Uart2_TimeInterval>4) && Uart2_BufferIndex) 
		{			
		    __HAL_TIM_DISABLE(&htim7);
			Uart2_BufferIndex=0;
			Uart2_TimeInterval=0;	
		}    
	}		
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART2_IRQHandler(void)
{  	
	u8 dat;
	
    HAL_UART_IRQHandler(&huart2);
	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
  	{ 
		__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
		dat =(u8)(huart2.Instance->DR & (uint8_t)0x00FF);
		{	
			if((Uart2_TimeInterval>2)&&(Uart2_TimeInterval<=4))
            {
                Uart2_BufferIndex=0;
            }
            Uart2_TimeInterval=0;
            __HAL_TIM_ENABLE(&htim7);
                
            Uart2_CommandBuffer[Uart2_BufferIndex++]=dat;
            if(Uart2_BufferIndex>UART2_BUFFERLENTH-1)
            {
                Uart2_BufferIndex=0;

			}
        }
	}
}
		
/*----------------------------------------------------------------------------*/
u8 analyzeUart2Packet(Message *m)
{
	//u8 temp;
	u8 res=ANALYZE_OK;
	INT8U err;
	//OS_FLAGS  Flags;
	
	/*
	Packet.sync=CommandBuffer[0];
	Packet.addr=CommandBuffer[1];
	Packet.command=CommandBuffer[2];
	Packet.data=CommandBuffer[3];
	Packet.checksum=CommandBuffer[4];
	temp=Packet.addr+Packet.command+Packet.data;
	*/

	
	//Packet.sync=m->data[0];
	Packet.addr=m->data[0];
	Packet.command=m->data[1];
	Packet.data=m->data[2];

		Post_GAME_RX_Msg(Packet);
		
		switch(Packet.command)
		{
			default:
				res=COMMAND_UNDEFINE;
				break;	
		}
		return res;
}


