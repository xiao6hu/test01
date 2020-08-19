/**
  ******************************************************************************
  * @file    can_stm32.h
  * @author  Ganhua R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_STM32_H__
#define __CAN_STM32_H__

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "can.h"
#include "stm32f2xx.h"
/* Exported types ------------------------------------------------------------*/
/*
typedef struct{
	CanRxMsg m;
}CANOpen_Message;
*/
extern CAN_HandleTypeDef CAN1_HandleStructure;
extern CAN_HandleTypeDef CAN2_HandleStructure;
extern TIM_HandleTypeDef TIM4_HandleStructure;

extern CanTxMsgTypeDef Can1TxMsg;
extern CanRxMsgTypeDef Can1RxMsg;
extern CanTxMsgTypeDef Can2TxMsg;
extern CanRxMsgTypeDef Can2RxMsg;



/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
// Number of receive MB
#define NB_MB				8
#define NB_RX_MB			4
// Number of transmit MB
#define NB_TX_MB			(NB_MB - NB_RX_MB)

#if (NB_TX_MB < 1)
#error define less RX MBs, you must have at least 1 TX MB!
#elif (NB_RX_MB < 1)
#error define at least 1 RX MBs!
#endif

#define START_TX_MB			NB_RX_MB
#define TX_INT_MSK			((0xFF << (NB_MB - NB_TX_MB)) & 0xFF)
#define RX_INT_MSK			(0xFF >> (NB_MB - NB_RX_MB))

/* Exported functions ------------------------------------------------------- */



unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);
unsigned char canSend(CAN_PORT notused, Message *m);
unsigned char canReceive(Message *m);

#endif
