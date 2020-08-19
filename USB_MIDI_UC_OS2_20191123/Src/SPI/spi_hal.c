#include "spi_hal.h"
#include "usart.h"

//#include "stm32f10x_spi.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SPI 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI3的初始化



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

	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  //PB10/11/12复用推挽输出
	
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_11; 
	GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;  //PB10/11/12复用推挽输出
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
 	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);  //PB10/11/12上拉
	
	
	
	
    __HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_SPI3_CLK_ENABLE();
	
    __HAL_AFIO_REMAP_SPI3_ENABLE();
    
	
    //HAL_SPI_DeInit(&SPI3_HandleStructure);
    SPI3_HandleStructure.Instance = SPI3;
    SPI3_HandleStructure.Init.Mode=SPI_MODE_MASTER;  //设置SPI工作模式:设置为主SPI        
    SPI3_HandleStructure.Init.Direction= SPI_DIRECTION_2LINES; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工        
    SPI3_HandleStructure.Init.DataSize=SPI_DATASIZE_8BIT;     //设置SPI的数据大小:SPI发送接收8位帧结构                                   
    SPI3_HandleStructure.Init.CLKPolarity=SPI_POLARITY_HIGH;  //选择了串行时钟的稳态:时钟悬空高    
    SPI3_HandleStructure.Init.CLKPhase=SPI_PHASE_2EDGE;    //数据捕获于第二个时钟沿     
    SPI3_HandleStructure.Init.NSS=SPI_NSS_SOFT;          //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制    
    SPI3_HandleStructure.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//定义波特率预分频的值:波特率预分频值为16
    SPI3_HandleStructure.Init.FirstBit=SPI_FIRSTBIT_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始      
    SPI3_HandleStructure.Init.TIMode=SPI_TIMODE_DISABLE;                                           
    //SPI3_HandleStructure.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;   
    SPI3_HandleStructure.Init.CRCPolynomial=7;   //CRC值计算的多项式
   
    HAL_SPI_Init(&SPI3_HandleStructure);
	
	
	__HAL_SPI_ENABLE(&SPI3_HandleStructure);//使能SPI外设
    

    HAL_SPI_TransmitReceive(&SPI3_HandleStructure,&TxData,&RxData,1,10);//启动传输	

}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI3_SetSpeed(u8 SpeedSet)
{
	SPI3->CR1&=0XFFC7; 
	SPI3->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&SPI3_HandleStructure);//使能SPI外设 
} 


#if 1
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
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
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  //PA5=SCK PA7=MOSI复用推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate=GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	/*
	GPIO_InitStructure.Pin = GPIO_PIN_6; //PA6=MISO 复用输入
	//GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	
	
	GPIO_InitStructure.Alternate=GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_PIN_SET);  //PB5/6/7上拉
	
	
	__HAL_RCC_SPI1_CLK_ENABLE();
	*/



    SPI1_HandleStructure.Instance = SPI1;
    SPI1_HandleStructure.Init.Mode=SPI_MODE_MASTER;  //设置SPI工作模式:设置为主SPI        
    SPI1_HandleStructure.Init.Direction= SPI_DIRECTION_2LINES; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工        
    SPI1_HandleStructure.Init.DataSize=SPI_DATASIZE_8BIT;     //设置SPI的数据大小:SPI发送接收8位帧结构                                   
    SPI1_HandleStructure.Init.CLKPolarity=SPI_POLARITY_HIGH;  //选择了串行时钟的稳态:时钟悬空高    
    SPI1_HandleStructure.Init.CLKPhase=SPI_PHASE_2EDGE;    //数据捕获于第二个时钟沿     
    SPI1_HandleStructure.Init.NSS=SPI_NSS_SOFT;          //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制    
    SPI1_HandleStructure.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//定义波特率预分频的值:波特率预分频值为16
    SPI1_HandleStructure.Init.FirstBit=SPI_FIRSTBIT_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始      
    SPI1_HandleStructure.Init.TIMode=SPI_TIMODE_DISABLE;                                           
    SPI1_HandleStructure.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;   
    SPI1_HandleStructure.Init.CRCPolynomial=7;   //CRC值计算的多项式
   
    HAL_SPI_Init(&SPI1_HandleStructure);
	__HAL_SPI_ENABLE(&SPI1_HandleStructure);//使能SPI外设
    HAL_SPI_TransmitReceive(&SPI1_HandleStructure,&TxData,&RxData,1,10);//启动传输	
}   

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&SPI1_HandleStructure);//使能SPI外设 
} 

#if 1
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{	
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&SPI1_HandleStructure, &TxData, &RxData, 1, 10);	
    return RxData;
}
#endif

#endif


/*
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	//while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位

	while (__HAL_SPI_GET_FLAG(&SPI3_HandleStructure, SPI_FLAG_TXE))
	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	//SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	
	SPI3_HandleStructure.Instance->DR =TxData;
	
	
	retry=0;

	//while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	while (__HAL_SPI_GET_FLAG(&SPI3_HandleStructure, SPI_FLAG_RXNE))
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	//return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据		

	return SPI3_HandleStructure.Instance->DR;
}
*/






























