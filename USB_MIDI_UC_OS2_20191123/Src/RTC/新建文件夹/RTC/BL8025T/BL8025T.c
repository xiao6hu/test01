

#include "RTC\BL8025T\BL8025T.h"

TimeStructTypeDef TimeStruct=
{
	0x59,
	0x58,
	0x14, 
	0x40,
	0x16,
	0x09,
	0x17,
};

//在BL8025T指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void BL8025T_SpecifyAddressWrite(u8 WriteAddr, u8 *DataToWrite, u8 Len)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //发送低地址
	IIC_Wait_Ack();
	while (Len--)
	{
		IIC_Send_Byte(*DataToWrite++);     //发送字节							   
		IIC_Wait_Ack();  
	}
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}


//在BL8025T指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
void BL8025T_SpecifyAddressRead(u8 ReadAddr, u8 *DataToRead, u8 Len)
{				  
    IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //进入接收模式			   
	IIC_Wait_Ack();	 
	
	while (Len--)
	{
		*DataToRead++ =IIC_Read_Byte(0);
		IIC_Wait_Ack();
	}
    IIC_Stop();//产生一个停止条件	    
}

u8 BL8025T_SpecifyAddressReadOneByte(u8 ReadAddr)
{
	u8 Tmp;
	IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //进入接收模式			   
	IIC_Wait_Ack();	 
	Tmp =IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件	    
	return Tmp;
}


void BL8025T_SpecifyAddressRead_N_Byte(u8 ReadAddr, u8 *DataToRead, u8 Len)
{
	u8 cnt;
	for (cnt =0;cnt< Len; cnt++)
	{
		*DataToRead++ = BL8025T_SpecifyAddressReadOneByte(ReadAddr+cnt);
	}
}


//在BL8025T指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
void BL8025T_NotSpecifyAddressRead(u8 *DataToRead, u8 Len)
{				  
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //进入接收模式			   
	IIC_Wait_Ack();	 
	
	while (Len--)
	{
		*DataToRead++=IIC_Read_Byte(0);	
		IIC_Wait_Ack();
	}
    IIC_Stop();//产生一个停止条件	    
}



void SetTime(void)
{

	
	BL8025T_SpecifyAddressWrite(BL8025T_REG_ADDR_SEC, (u8*)&TimeStruct, 7);
}

void GetTime(void)
{
	//BL8025T_SpecifyAddressRead(BL8025T_REG_ADDR_SEC, (u8*)&TimeStruct, 7);
	BL8025T_SpecifyAddressRead_N_Byte(BL8025T_REG_ADDR_SEC, (u8*)&TimeStruct, 7);
	
}


