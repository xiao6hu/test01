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
#include "includes.h"
#include "uart4.h"
#include <stdarg.h>

UART_HandleTypeDef huart4;	

u8 Uart4_TxBuff[3];

//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void UART4_NVIC_Configuration(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_NVIC_SetPriority(UART4_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

/*
 * 函数名：UART4_Config
 * 描述  ：UART4 GPIO 配置,工作模式配置。31250 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void UART4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* config UART4 clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();
	/* UART4 GPIO config */
    /* Configure USART1 Tx (PA.0) as alternate function push-pull */
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate= GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
    /* Configure USART4 Rx (PA.1) as input floating */
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate= GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    huart4.Instance=UART4;   
    huart4.Init.BaudRate = 31250;
	//huart4.Init.BaudRate = 38400;
    huart4.Init.WordLength = USART_WORDLENGTH_8B;
    huart4.Init.StopBits = USART_STOPBITS_1;
    huart4.Init.Parity = USART_PARITY_NONE ;
    huart4.Init.Mode = USART_MODE_RX | USART_MODE_TX;
    HAL_UART_Init(&huart4);
    UART4_NVIC_Configuration();
    __HAL_UART_ENABLE(&huart4);
}

void UART4_IRQHandler(void)
{  	
    HAL_UART_IRQHandler(&huart4);	
	
}

void Uart4SendData(unsigned char ch)
{
	/* 将Printf内容发往串口 */
    HAL_UART_Transmit(&huart4,&ch,1,10);
    while (!__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TC));
}
#if 0
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到UART4
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
    HAL_UART_Transmit(&huart4,(uint8_t*)&ch,1,10);
    while (!__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TC));
    return (ch);
}
#endif


/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    INT8U  err;
    if (huart->Instance == UART4)
    {

        OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
                    (OS_FLAGS) UART4_TX_OK_FLAG,
                    (INT8U) OS_FLAG_SET,
                    (INT8U  *)&err);    
    }  
}
*/
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

