#include "HT32.h"
#include "key_iic.h"
#include "delay.h"

//===================================================================//
void fun_config_sda_out(void)
{                                                         
  GPIO_DirectionConfig(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_DIR_OUT);
}
void fun_config_sda_in(void)	
{
  GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_IN);
}

//===================================================================//

void fun_iicmaster_start(void)
{	
    fun_config_sda_out();
	
	SET_I2C_SCL;
	IIC_DELAY();
	SET_I2C_SDA;
	IIC_DELAY();
	IIC_DELAY();	
	RESET_I2C_SDA;
	IIC_DELAY();
	RESET_I2C_SCL;
	IIC_DELAY();
		

}
//===================================================================//
void fun_iicmaster_stop(void)
{		
	fun_config_sda_out();
	    
	RESET_I2C_SCL;   
	IIC_DELAY();
	RESET_I2C_SDA;	
	IIC_DELAY();
	SET_I2C_SCL;	
	IIC_DELAY();
	SET_I2C_SDA;
	IIC_DELAY();
	IIC_DELAY();
}
unsigned char fun_iic_wait_ack(void)
{
	uint8_t ucErrTime=0;	
	fun_config_sda_in();

	IIC_DELAY();
	SET_I2C_SCL;
	IIC_DELAY();

	while(GPIO_ReadInBit(I2C_SDA_PORT,I2C_SDA_PIN)==SET)
	{
		ucErrTime++;
		if(ucErrTime>200)
			fun_iicmaster_stop();
			return ERROR;
	}
	RESET_I2C_SCL;
	IIC_DELAY();
	return SUCCESS;
}
//===================================================================//
void fun_iic_ack(void)
{
	fun_config_sda_out();
	
	RESET_I2C_SDA;	
	IIC_DELAY();
	SET_I2C_SCL;	
	IIC_DELAY();
	RESET_I2C_SCL;
	IIC_DELAY();
}
//===================================================================//
void fun_iic_Nack(void)
{
	fun_config_sda_out();
	SET_I2C_SDA;	
	IIC_DELAY();
	SET_I2C_SCL;	
	IIC_DELAY();
	RESET_I2C_SCL;
	IIC_DELAY();
}


//===================================================================//
void fun_iic_bytewrite(unsigned char lv8u_data)
{
	short       i;
	fun_config_sda_out();
	
		for(i=0;i<8;i++)
		{	
			IIC_DELAY();
			if(lv8u_data&0x80)
			{
				SET_I2C_SDA;
			}
			else
			{
				RESET_I2C_SDA;	
			}
			IIC_DELAY();
			SET_I2C_SCL;			
			IIC_DELAY();
			RESET_I2C_SCL;
			lv8u_data<<=1;
		}
		IIC_DELAY();
	
}

//===================================================================//
uint16_t fun_iic_byteread(void)
{
	unsigned char i,lv8u_data;

    fun_config_sda_in();
    lv8u_data = 0;
	for(i=0;i<8;i++)
	{	
		IIC_DELAY();
		SET_I2C_SCL;
		IIC_DELAY();
		lv8u_data = lv8u_data<<1;	
	
		if(GPIO_ReadInBit(I2C_SDA_PORT,I2C_SDA_PIN)==SET)
		{
			lv8u_data|=0x01;
		}
		RESET_I2C_SCL;
		IIC_DELAY();
	}
	return lv8u_data;
}





