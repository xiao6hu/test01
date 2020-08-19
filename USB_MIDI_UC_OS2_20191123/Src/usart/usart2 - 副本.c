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

#include "usart2.h"
#include <stdarg.h>
						

u8 CommandBuffer[BUFFERLENTH];

u8 bufferIndex;

UartProtocl Packet;

u8 uartRecieveRes=ANALYZE_OK;

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
#ifdef USART2_IRQ_EN
void USART2_IRQHandler(void)
{  	
	u8 dat;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{ 
  		USART_ClearITPendingBit(USART2,USART_IT_RXNE);

		dat=USART_ReceiveData(USART2);
		
		if(dat==0xff)
		{
			bufferIndex=0;
		}
		else if(dat==0xfe)
		{
			CommandBuffer[bufferIndex++]=dat;
			if(bufferIndex>BUFFERLENTH)
				bufferIndex=0;
			else
			{
				uartRecieveRes=analyzeUartPacket();
				
				if(uartRecieveRes!=ANALYZE_OK)
				{	
					//put_msg_fifo(MSG_COMMAND_ERROR);					
				}
			}
			return;
		}

		CommandBuffer[bufferIndex++]=dat;
		if(bufferIndex>BUFFERLENTH-1)
		{
			bufferIndex=0;	
		}
	}
}
		
#endif



/*----------------------------------------------------------------------------*/
u8 analyzeUartPacket(void)
{
	u8 temp;
	u8 res=ANALYZE_OK;
	//u16 files;
	Packet.sync=CommandBuffer[0];
	
	Packet.start=CommandBuffer[1];
	
	Packet.lenth=CommandBuffer[2];
	
	Packet.command=CommandBuffer[3];
	
	Packet.checksum=CommandBuffer[3+Packet.lenth];


	temp=Packet.lenth+Packet.command;
	
	
	
	if(Packet.lenth>1)
	{
		u8 i;
		u16 sum=0;
		sum=temp;
		for(i=0;i<Packet.lenth-1;i++)
			sum+=CommandBuffer[4+i];
		temp=sum%100;
	}
	temp%=100;
	temp=((temp/10)<<4)+(temp%10);
	
	
	if(Packet.sync!=UART_SYNC)
		res=SYNC_ERROR;
	
	else if(Packet.start!=UART_START)
		res=START_BYTE_LOSE;
	
	else if((Packet.lenth<1)||(Packet.lenth>BUFFERLENTH-4))
	    res=LENTH_ERROR;

	else if(temp!=Packet.checksum)
		res=CHECKSUM_ERROR;
	else
	{
      	//status_flash=1;
		switch(Packet.command)
		{
			case MIDI_PLAY:	
				//put_msg_fifo(MSG_MUSIC_PLAY);
				break;
			case MIDI_STOP:
				//put_msg_fifo(MSG_MUSIC_STOP);
				break;  
			default:
				res=COMMAND_UNDEFINE;
				break;	
			}
		}
		return res;
}



/*
 * 函数名：USART2_Config
 * 描述  ：USART2 GPIO 配置,工作模式配置。9600 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART1 clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	//USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

    USART_Cmd(USART2, ENABLE);
}

void Uart2SendData(unsigned char ch)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART2, (unsigned char) ch);
	//while (!(USART2->SR & USART_FLAG_TXE));
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TC));
}

#ifdef USART2_IRQ_EN 
//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	//定义数据结构体
  
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//将中断矢量放到Flash的0地址

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级配置的模式，详情请阅读原材料中的文章

  //使能串口中断，并设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);	//将结构体丢到配置函数，即写入到对应寄存器中
}
#endif


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
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}
#endif
