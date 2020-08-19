/******************** (C) COPYRIGHT 2012 **************************
 * �ļ���  ��usart4.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��The One STM32������
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.5.0
 *
 * �Ա�    ��http://sztheone.taobao.com/
**********************************************************************************/

#include "usart4.h"
#include <stdarg.h>

USART_HandleTypeDef husart4;	

u8 Uart4_TxBuff[3];



//����ʸ���жϣ�ʸ������˼������˳�����Ⱥ����˼��
void USART4_NVIC_Configuration(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_NVIC_SetPriority(UART4_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

/*
 * ��������USART4_Config
 * ����  ��USART4 GPIO ����,����ģʽ���á�31250 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
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
	/* ��Printf���ݷ������� */
    HAL_USART_Transmit(&husart4,&ch,1,10);
    while (!__HAL_USART_GET_FLAG(&husart4, USART_FLAG_TC));
}
#if 0
/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
    HAL_USART_Transmit(&husart4,(uint8_t*)&ch,1,10);
    while (!__HAL_USART_GET_FLAG(&husart4, USART_FLAG_TC));
    return (ch);
}
#endif

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

