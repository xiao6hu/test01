#include "spi_hal.h"
#include "usart.h"

//#include "stm32f10x_spi.h"


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
 

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI3�ĳ�ʼ��



#if 0

SPI_HandleTypeDef SPI3_HandleStructure;

void SPI3_Init(void)
{
    uint8_t TxData=0xff;
    uint8_t RxData=0xff;

 	GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : */
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;

	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  //PB10/11/12�����������
	
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_11; 
	GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;  //PB10/11/12�����������
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
 	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);  //PB10/11/12����
	
	
	
	
    __HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_SPI3_CLK_ENABLE();
	
    __HAL_AFIO_REMAP_SPI3_ENABLE();
    
	
    //HAL_SPI_DeInit(&SPI3_HandleStructure);
    SPI3_HandleStructure.Instance = SPI3;
    SPI3_HandleStructure.Init.Mode=SPI_MODE_MASTER;  //����SPI����ģʽ:����Ϊ��SPI        
    SPI3_HandleStructure.Init.Direction= SPI_DIRECTION_2LINES; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��        
    SPI3_HandleStructure.Init.DataSize=SPI_DATASIZE_8BIT;     //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ                                   
    SPI3_HandleStructure.Init.CLKPolarity=SPI_POLARITY_HIGH;  //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�    
    SPI3_HandleStructure.Init.CLKPhase=SPI_PHASE_2EDGE;    //���ݲ����ڵڶ���ʱ����     
    SPI3_HandleStructure.Init.NSS=SPI_NSS_SOFT;          //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
    SPI3_HandleStructure.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    SPI3_HandleStructure.Init.FirstBit=SPI_FIRSTBIT_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ      
    SPI3_HandleStructure.Init.TIMode=SPI_TIMODE_DISABLE;                                           
    //SPI3_HandleStructure.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;   
    SPI3_HandleStructure.Init.CRCPolynomial=7;   //CRCֵ����Ķ���ʽ
   
    HAL_SPI_Init(&SPI3_HandleStructure);
	
	
	__HAL_SPI_ENABLE(&SPI3_HandleStructure);//ʹ��SPI����
    

    HAL_SPI_TransmitReceive(&SPI3_HandleStructure,&TxData,&RxData,1,10);//��������	

}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI3_SetSpeed(u8 SpeedSet)
{
	SPI3->CR1&=0XFFC7; 
	SPI3->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&SPI3_HandleStructure);//ʹ��SPI���� 
} 


#if 1
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{	
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&SPI3_HandleStructure, &TxData, &RxData, 1, 10);	
    return RxData;
}
#endif

#endif




#if 1

SPI_HandleTypeDef SPI1_HandleStructure;

void SPI1_Init(void)
{
	uint8_t TxData=0xff;
	uint8_t RxData=0xff;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	/*Configure GPIO pin : */
	GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  //PA5=SCK PA7=MOSI�����������
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate=GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	/*
	GPIO_InitStructure.Pin = GPIO_PIN_6; //PA6=MISO ��������
	//GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	
	
	GPIO_InitStructure.Alternate=GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_PIN_SET);  //PB5/6/7����
	
	
	__HAL_RCC_SPI1_CLK_ENABLE();
	*/



    SPI1_HandleStructure.Instance = SPI1;
    SPI1_HandleStructure.Init.Mode=SPI_MODE_MASTER;  //����SPI����ģʽ:����Ϊ��SPI        
    SPI1_HandleStructure.Init.Direction= SPI_DIRECTION_2LINES; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��        
    SPI1_HandleStructure.Init.DataSize=SPI_DATASIZE_8BIT;     //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ                                   
    SPI1_HandleStructure.Init.CLKPolarity=SPI_POLARITY_HIGH;  //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�    
    SPI1_HandleStructure.Init.CLKPhase=SPI_PHASE_2EDGE;    //���ݲ����ڵڶ���ʱ����     
    SPI1_HandleStructure.Init.NSS=SPI_NSS_SOFT;          //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����    
    SPI1_HandleStructure.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    SPI1_HandleStructure.Init.FirstBit=SPI_FIRSTBIT_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ      
    SPI1_HandleStructure.Init.TIMode=SPI_TIMODE_DISABLE;                                           
    SPI1_HandleStructure.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;   
    SPI1_HandleStructure.Init.CRCPolynomial=7;   //CRCֵ����Ķ���ʽ
   
    HAL_SPI_Init(&SPI1_HandleStructure);
	__HAL_SPI_ENABLE(&SPI1_HandleStructure);//ʹ��SPI����
    HAL_SPI_TransmitReceive(&SPI1_HandleStructure,&TxData,&RxData,1,10);//��������	
}   

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&SPI1_HandleStructure);//ʹ��SPI���� 
} 

#if 1
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{	
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&SPI1_HandleStructure, &TxData, &RxData, 1, 10);	
    return RxData;
}
#endif

#endif


/*
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	//while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ

	while (__HAL_SPI_GET_FLAG(&SPI3_HandleStructure, SPI_FLAG_TXE))
	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	//SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	
	SPI3_HandleStructure.Instance->DR =TxData;
	
	
	retry=0;

	//while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	while (__HAL_SPI_GET_FLAG(&SPI3_HandleStructure, SPI_FLAG_RXNE))
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	//return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����		

	return SPI3_HandleStructure.Instance->DR;
}
*/






























