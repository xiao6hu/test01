

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

//��BL8025Tָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void BL8025T_SpecifyAddressWrite(u8 WriteAddr, u8 *DataToWrite, u8 Len)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //���͵͵�ַ
	IIC_Wait_Ack();
	while (Len--)
	{
		IIC_Send_Byte(*DataToWrite++);     //�����ֽ�							   
		IIC_Wait_Ack();  
	}
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}


//��BL8025Tָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
void BL8025T_SpecifyAddressRead(u8 ReadAddr, u8 *DataToRead, u8 Len)
{				  
    IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //�������ģʽ			   
	IIC_Wait_Ack();	 
	
	while (Len--)
	{
		*DataToRead++ =IIC_Read_Byte(0);
		IIC_Wait_Ack();
	}
    IIC_Stop();//����һ��ֹͣ����	    
}

u8 BL8025T_SpecifyAddressReadOneByte(u8 ReadAddr)
{
	u8 Tmp;
	IIC_Start();  
	IIC_Send_Byte(BL8025T_SlaveAddress | WriteMode);   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //�������ģʽ			   
	IIC_Wait_Ack();	 
	Tmp =IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����	    
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


//��BL8025Tָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
void BL8025T_NotSpecifyAddressRead(u8 *DataToRead, u8 Len)
{				  
	IIC_Start(); 
	IIC_Send_Byte(BL8025T_SlaveAddress | ReadMode);           //�������ģʽ			   
	IIC_Wait_Ack();	 
	
	while (Len--)
	{
		*DataToRead++=IIC_Read_Byte(0);	
		IIC_Wait_Ack();
	}
    IIC_Stop();//����һ��ֹͣ����	    
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


