/******************** (C) COPYRIGHT 2012 **************************
 * 文件名  ：usart4.c
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

#include "usart4.h"
#include <stdarg.h>

USART_HandleTypeDef husart4;	

u8 Uart4_TxBuff[3];



//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void USART4_NVIC_Configuration(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_NVIC_SetPriority(UART4_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

/*
 * 函数名：USART4_Config
 * 描述  ：USART4 GPIO 配置,工作模式配置。31250 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* config USART1 clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();
	/* USART1 GPIO config */
    /* Configure USART1 Tx (PA.0) as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate= GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
    /* Configure USART4 Rx (PA.1) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    husart4.Instance=UART4;   
    husart4.Init.BaudRate = 31250;
	//husart4.Init.BaudRate = 38400;
    husart4.Init.WordLength = USART_WORDLENGTH_8B;
    husart4.Init.StopBits = USART_STOPBITS_1;
    husart4.Init.Parity = USART_PARITY_NONE ;
    husart4.Init.Mode = USART_MODE_RX | USART_MODE_TX;
    HAL_USART_Init(&husart4);
    USART4_NVIC_Configuration();
    __HAL_UART_ENABLE(&husart4);
}

void USART4_IRQHandler(void)
{  	
    HAL_USART_IRQHandler(&husart4);	
	
}



void Uart4SendData(unsigned char ch)
{
	/* 将Printf内容发往串口 */
    HAL_USART_Transmit(&husart4,&ch,1,10);
    while (!__HAL_USART_GET_FLAG(&husart4, USART_FLAG_TC));
}
#if 0
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
    HAL_USART_Transmit(&husart4,(uint8_t*)&ch,1,10);
    while (!__HAL_USART_GET_FLAG(&husart4, USART_FLAG_TC));
    return (ch);
}
#endif

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

