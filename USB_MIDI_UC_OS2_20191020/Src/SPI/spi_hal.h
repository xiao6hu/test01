#ifndef __SPI_HAL_H
#define __SPI_HAL_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#if 0									  
void SPI3_Init(void);			 //��ʼ��SPI��
void SPI3_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI3_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
extern SPI_HandleTypeDef SPI3_HandleStructure;	
#endif





#if 1
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
extern SPI_HandleTypeDef SPI1_HandleStructure;	
#endif
	 
#endif

