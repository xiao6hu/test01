#include "stm32f2xx.h"
#include "mco/mco.h"
#include "can.h"
#include "config.h"
u8 gMessagePending;	
extern CAN_MSG gRxCAN;	

/****************************************************************************
* 名称：CAN_Configration(s32 BaudRate)
* 功能：CAN配置
* 入口参数：BaudRate
* 出口参数：无
****************************************************************************/
void CAN_Configration(u32 BaudRate)
{
	CAN_InitTypeDef CAN_InitStructure;
	//CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

	CAN_HandleTypeDef CAN_HandleStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;


	
	/* CAN GPIOs configuration **************************************************/
	NVIC_InitStructure.NVIC_IRQChannelPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(CAN1_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;
	
	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_RX_SOURCE, CAN1_AF_PORT);
	GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_TX_SOURCE, CAN1_AF_PORT); 
	
	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN | CAN1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

	/* CAN configuration ********************************************************/  
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN1_CLK, ENABLE);

	CAN_DeInit(CAN);
	CAN_StructInit(&CAN_InitStructure);
	//关闭时间触发模式
	CAN_InitStructure.CAN_TTCM=DISABLE;
	//关闭自动离线管理
	CAN_InitStructure.CAN_ABOM=DISABLE;
	//关闭自动唤醒模式
	CAN_InitStructure.CAN_AWUM=DISABLE;
	//禁止报文自动重传
	CAN_InitStructure.CAN_NART=DISABLE;
	//FIFO 溢出时报文覆盖源文件
	CAN_InitStructure.CAN_RFLM=DISABLE;
	//报文发送优先级取决于ID号
	CAN_InitStructure.CAN_TXFP=DISABLE;
	//正常的工作模式
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	//设置CAN 波特率=36000000/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2)
	switch(BaudRate)
	{
	case 1:
		CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;//5
		CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;
		CAN_InitStructure.CAN_Prescaler = 600;
		break;
	case 2:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//10
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 600;
		break;
	case 3:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//20
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 300;
		break;
	case 4:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//25
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 240;
		break;
	case 5:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//40
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 150;
		break;
	case 6:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//50
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 120;
		break;
		/**
	case 62.5:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 96;
		break;
		**/
	case 7:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//80
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 75;
		break;
	case 8:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//100
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 60;
		break;
	case 9:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//125
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 48;
		break;
	case 10:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//200
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 30;
		break;
	case 11:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//250
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 24;
		break;
	case 12:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//400
		CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
		CAN_InitStructure.CAN_Prescaler = 10;
		break;
	case 13:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//500
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 12;
		break;
	case 14:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//800
		CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
		CAN_InitStructure.CAN_Prescaler = 5;
		break;
	case 15:
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//1000
		CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
		CAN_InitStructure.CAN_Prescaler = 6;
		break;
	default:break;
	}
	//初始化CAN
	CAN_Init(CAN,&CAN_InitStructure);
	
	//使能接收中断
	CAN_ITConfig(CAN,CAN_IT_FMP0,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority=0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




#if 0
/****************************************************************************
* 名称：Crx(void)
* 功能：CAN接收数据
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  Crx(void)
{
  u8 i;
  u32 DataL,DataH;
  for(i = 0; i < gMessagePending; i++)
  {
    CAN_Receive_UC(CAN,CAN_FIFO0,(u32*)(&gRxCAN.ID), (u32*)(&gRxCAN.LEN), &DataH, &DataL);
    gRxCAN.BUF[0] = (u32)0x000000FF & DataL;
    gRxCAN.BUF[1] = (u32)0x000000FF & (DataL >> 8);
    gRxCAN.BUF[2] = (u32)0x000000FF & (DataL >> 16);
    gRxCAN.BUF[3] = (u32)0x000000FF & (DataL >> 24);
    
    gRxCAN.BUF[4] = (u32)0x000000FF & DataH;
    gRxCAN.BUF[5] = (u32)0x000000FF & (DataH >> 8);
    gRxCAN.BUF[6] = (u32)0x000000FF & (DataH >> 16);
    gRxCAN.BUF[7] = (u32)0x000000FF & (DataH >> 24);
    CanOpenRxPres();
  }
  gMessagePending = 0;
}
#endif


/****************************************************************************
* 名称：Crx(void)
* 功能：CAN接收数据
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  Crx(void)
{
	u8 i;
	//u32 DataL,DataH;
	CanRxMsg RxMessage;
	for(i = 0; i < gMessagePending; i++)
	{
		//CAN_Receive_UC(CAN,CAN_FIFO0,(u32*)(&gRxCAN.ID), (u32*)(&gRxCAN.LEN), &DataH, &DataL);

		CAN_Receive(CAN, CAN_FIFO0, &RxMessage);
		gRxCAN.ID=RxMessage.StdId;
		gRxCAN.LEN=RxMessage.DLC;
		gRxCAN.BUF[0] = RxMessage.Data[0];
		gRxCAN.BUF[1] = RxMessage.Data[1];
		gRxCAN.BUF[2] = RxMessage.Data[2];
		gRxCAN.BUF[3] = RxMessage.Data[3];
		gRxCAN.BUF[4] = RxMessage.Data[4];
		gRxCAN.BUF[5] = RxMessage.Data[5];
		gRxCAN.BUF[6] = RxMessage.Data[6];
		gRxCAN.BUF[7] = RxMessage.Data[7];
		CanOpenRxPres();
	}
	gMessagePending = 0;
}

/****************************************************************************
* 名称：Ctx(void)
* 功能：CAN发送数据
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  Ctx(void)
{
  MCO_ProcessStack();
}

/****************************************************************************
* 名称：USB_LP_CAN1_RX0_IRQHandler(void)
* 功能：CAN接受中断服务函数
* 入口参数：无
* 出口参数：无
****************************************************************************/
void CEC_CAN_IRQHandler(void)
{
  gMessagePending = CAN_MessagePending(CAN,CAN_FIFO0);
  Crx();
}

