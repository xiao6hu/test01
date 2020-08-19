/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can_stm32_hal.h"
#include "canfestival.h"
#include "SEGGER_RTT.h"
#include "GlobalVar.h"

#include "app.h"
#include "usart1.h"

#include  "ucos_ii.h"  		//uC/OS-II系统函数头文件

//#define UART1_RX_OK_FLAG  (1<<9)//BIT(9) 
//#define UART1_TX_OK_FLAG  (1<<10)//BIT(10) 


extern OS_FLAG_GRP *pFlagGrpMidi;


Message RxMSG = Message_Initializer;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  canInit
  * @param  CANx:CAN1 or CAN2 bitrate
  * @retval 0：Success
  */

CAN_HandleTypeDef CAN1_HandleStructure;
CAN_HandleTypeDef CAN2_HandleStructure;
//CAN_HandleTypeDef*  CANx_Handle;
CanTxMsgTypeDef Can1TxMsg;
CanRxMsgTypeDef Can1RxMsg;
CanTxMsgTypeDef Can2TxMsg;
CanRxMsgTypeDef Can2RxMsg;





unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate)
{
	CAN_HandleTypeDef CAN_HandleStructure;
	CAN_FilterConfTypeDef CAN_FilterConfStructure;	
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* CAN GPIOs configuration **************************************************/
	if(CANx == CAN1)
	{
		/* Enable GPIO clock */
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/* Configure CAN RX and TX pins */
		GPIO_InitStructure.Pin = CAN1_TX_PIN | CAN1_RX_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.Alternate=GPIO_AF9_CAN1;
		HAL_GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

		/* CAN configuration ********************************************************/  
		/* Enable CAN clock */
		__HAL_RCC_CAN1_CLK_ENABLE();

		/* CAN register init */
		HAL_CAN_DeInit(&CAN1_HandleStructure);
		/* CAN cell init */

		CAN1_HandleStructure.Instance=CAN1;
		CAN1_HandleStructure.Init.TTCM = DISABLE;    
		CAN1_HandleStructure.Init.ABOM = ENABLE;  
		CAN1_HandleStructure.Init.AWUM = DISABLE;		
		CAN1_HandleStructure.Init.NART = DISABLE;
		//CAN1_HandleStructure.Init.NART = ENABLE;
		CAN1_HandleStructure.Init.RFLM = DISABLE;	
		CAN1_HandleStructure.Init.TXFP = DISABLE;		
		CAN1_HandleStructure.Init.Mode = CAN_MODE_NORMAL;	
		CAN1_HandleStructure.Init.SJW = CAN_SJW_1TQ;  
		CAN1_HandleStructure.Init.BS1 = CAN_BS1_6TQ;
		CAN1_HandleStructure.Init.BS2 = CAN_BS2_8TQ;
		CAN1_HandleStructure.Init.Prescaler = bitrate;   
		CAN1_HandleStructure.pTxMsg=&Can1TxMsg;
		CAN1_HandleStructure.pRxMsg=&Can1RxMsg;
		HAL_CAN_Init(&CAN1_HandleStructure);

		CAN_FilterConfStructure.FilterNumber=0;
		CAN_FilterConfStructure.FilterMode=CAN_FILTERMODE_IDMASK;
		CAN_FilterConfStructure.FilterScale=CAN_FILTERSCALE_32BIT;
		CAN_FilterConfStructure.FilterIdHigh=0x0000;
		CAN_FilterConfStructure.FilterIdLow=0x0000;
		CAN_FilterConfStructure.FilterMaskIdHigh=0x0000;
		CAN_FilterConfStructure.FilterMaskIdLow=0x0000;
		CAN_FilterConfStructure.FilterFIFOAssignment=0;
		CAN_FilterConfStructure.FilterActivation=ENABLE;          
		HAL_CAN_ConfigFilter(&CAN1_HandleStructure,&CAN_FilterConfStructure);
		
		/* Enable FIFO 0 message pending Interrupt */
		__HAL_CAN_ENABLE_IT(&CAN1_HandleStructure,CAN_IT_FMP0);
		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	  	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 1);
	  	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);


	}
	else
	{
		/* Enable GPIO clock */
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/* Configure CAN RX and TX pins */
		GPIO_InitStructure.Pin = CAN2_TX_PIN | CAN2_RX_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.Alternate=GPIO_AF9_CAN2;
		HAL_GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
		
		/* CAN configuration ********************************************************/	
		/* Enable CAN clock */
		__HAL_RCC_CAN2_CLK_ENABLE();

		/* CAN register init */
		HAL_CAN_DeInit(&CAN2_HandleStructure);
		/* CAN cell init */

		CAN2_HandleStructure.Instance=CAN2;
		CAN2_HandleStructure.Init.TTCM = DISABLE;	
		CAN2_HandleStructure.Init.ABOM = ENABLE;  
		CAN2_HandleStructure.Init.AWUM = DISABLE;		
		CAN2_HandleStructure.Init.NART = DISABLE;
		//CAN2_HandleStructure.Init.NART = ENABLE;
		CAN2_HandleStructure.Init.RFLM = DISABLE;	
		CAN2_HandleStructure.Init.TXFP = DISABLE;		
		CAN2_HandleStructure.Init.Mode = CAN_MODE_NORMAL;	
		CAN2_HandleStructure.Init.SJW = CAN_SJW_1TQ;  
		CAN2_HandleStructure.Init.BS1 = CAN_BS1_6TQ;
		CAN2_HandleStructure.Init.BS2 = CAN_BS2_8TQ;
		CAN2_HandleStructure.Init.Prescaler = bitrate;	
		CAN2_HandleStructure.pTxMsg=&Can2TxMsg;
		CAN2_HandleStructure.pRxMsg=&Can2RxMsg;
		HAL_CAN_Init(&CAN2_HandleStructure);

		CAN_FilterConfStructure.FilterNumber=15;
		CAN_FilterConfStructure.FilterMode=CAN_FILTERMODE_IDMASK;
		CAN_FilterConfStructure.FilterScale=CAN_FILTERSCALE_32BIT;
		CAN_FilterConfStructure.FilterIdHigh=0x0000;
		CAN_FilterConfStructure.FilterIdLow=0x0000;
		CAN_FilterConfStructure.FilterMaskIdHigh=0x0000;
		CAN_FilterConfStructure.FilterMaskIdLow=0x0000;
		CAN_FilterConfStructure.FilterFIFOAssignment=0;
		CAN_FilterConfStructure.FilterActivation=ENABLE;		  
		HAL_CAN_ConfigFilter(&CAN2_HandleStructure,&CAN_FilterConfStructure);
		
		/* Enable FIFO 0 message pending Interrupt */
		__HAL_CAN_ENABLE_IT(&CAN2_HandleStructure,CAN_IT_FMP0);
		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	}
	return 0;
}


/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0：Success
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
		unsigned char ret;
		unsigned char i;
		CAN_HandleTypeDef*  CANx_Handle;
		/*
		CanTxMsg TxMessage;
		TxMessage.StdId = (uint32_t)(m->cob_id);
		TxMessage.ExtId = 0x00;
		TxMessage.RTR = m->rtr;								  
		TxMessage.IDE = CAN_ID_STD;                           
		TxMessage.DLC = m->len;    
		for(i=0;i<m->len;i++)                                 
		{
			TxMessage.Data[i] = m->data[i];
		}
		*/

		if ((CAN_TypeDef*)CANx== CAN1)
		{
			CANx_Handle = &CAN1_HandleStructure;
		}
		else
		{
			CANx_Handle = &CAN2_HandleStructure;
		}
	
		CANx_Handle->pTxMsg->StdId = (uint32_t)(m->cob_id);
		CANx_Handle->pTxMsg->ExtId = 0x00;
		CANx_Handle->pTxMsg->RTR= m->rtr;								  
		CANx_Handle->pTxMsg->IDE = CAN_ID_STD;                           
		CANx_Handle->pTxMsg->DLC = m->len;    

		
		for(i=0;i<m->len;i++)                                 
		{
			CANx_Handle->pTxMsg->Data[i] = m->data[i];
		}
		
		__HAL_CAN_DISABLE_IT(CANx_Handle, CAN_IT_FMP0);
		
		ret = HAL_CAN_Transmit(CANx_Handle, 10);
		
		__HAL_CAN_ENABLE_IT(CANx_Handle, CAN_IT_FMP0);
		if(ret == HAL_ERROR)
		{
				return 1;	
		}
		else 
		{
				return 0;    
		}
}
/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{  
    unsigned int i = 0;
	//INT8U err;
	
	CAN_HandleTypeDef*  CANx_Handle;
	CANx_Handle= &CAN1_HandleStructure;
	
	//Message RxMSG = Message_Initializer;
	
	//CANOpen_Message CAN1_Rx_m;

	//HAL_CAN_Receive(CAN1, CAN_FIFO0, &(CAN1_Rx_m.m));	        //从CAN1 FIFO0接收数据
	HAL_CAN_Receive(CANx_Handle, CAN_FIFO0, 10);	

	/*
	RxMSG.cob_id = (uint16_t)(CAN1_Rx_m.m.StdId);
	RxMSG.rtr = CAN1_Rx_m.m.RTR;
	RxMSG.len = CAN1_Rx_m.m.DLC;
	for(i=0;i<RxMSG.len;i++)
	{
		RxMSG.data[i] = CAN1_Rx_m.m.Data[i];
	}
	*/
	
	RxMSG.cob_id = (uint16_t)(CANx_Handle->pRxMsg->StdId);
	RxMSG.rtr = CANx_Handle->pRxMsg->RTR;
	RxMSG.len = CANx_Handle->pRxMsg->DLC;
	for(i=0;i<RxMSG.len;i++)
	{
		RxMSG.data[i] = CANx_Handle->pRxMsg->Data[i];
	}
	//SEGGER_RTT_printf(0, "can master revcive data!!!!!!!!!!!!\n");
	
	SetCanHandle(CANOpenMasterObject, CAN1);
	canDispatch(CANOpenMasterObject, &(RxMSG));
	
	/*
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) UART1_RX_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);	
	*/
	//Post_CAN_IRQ_RX_Msg(RxMSG);
	//analyzeUartPacket(&(RxMSG));
	
	
}

/**
  * @}
  */ 
/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
	unsigned int i = 0;
	//INT8U err;
	
	
	//Message RxMSG = Message_Initializer;
	
	CAN_HandleTypeDef*  CANx_Handle;
	CANx_Handle=&CAN2_HandleStructure;

	//CANOpen_Message CAN2_Rx_m;  
	//CAN_Receive(CAN2, CAN_FIFO0, &(CAN2_Rx_m.m));	  
	
	HAL_CAN_Receive(CANx_Handle, CAN_FIFO0, 10); 
	
	/*
	RxMSG.cob_id = (uint16_t)(CAN1_Rx_m.m.StdId);
	RxMSG.rtr = CAN1_Rx_m.m.RTR;
	RxMSG.len = CAN1_Rx_m.m.DLC;
	for(i=0;i<RxMSG.len;i++)
	{
		RxMSG.data[i] = CAN1_Rx_m.m.Data[i];
	}
	*/
	
	RxMSG.cob_id = (uint16_t)(CANx_Handle->pRxMsg->StdId);
	RxMSG.rtr = CANx_Handle->pRxMsg->RTR;
	RxMSG.len = CANx_Handle->pRxMsg->DLC;
	for(i=0;i<RxMSG.len;i++)
	{
		RxMSG.data[i] = CANx_Handle->pRxMsg->Data[i];
	} 

	//SEGGER_RTT_printf(0, "can master revcive data!!!!!!!!!!!!\n");
	
	SetCanHandle(CANOpenMasterObject, CAN2);
	canDispatch(CANOpenMasterObject, &(RxMSG));

	/*
	OSFlagPost ((OS_FLAG_GRP *)pFlagGrpMidi,
								(OS_FLAGS) UART1_RX_OK_FLAG,
								(INT8U) OS_FLAG_SET,
								(INT8U  *)&err);	
	*/
	
	//Post_CAN_IRQ_RX_Msg(RxMSG);
	
	
	//analyzeUartPacket(&(RxMSG));
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/



