
//******************************************************************************/

#include "ht32.h"
#include "oled.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "touchkey.h"
#include "ht32_board_config.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			
/**********************************************
//IIC Start
**********************************************/
#if OLED_MODE==1
void IIC_Start()
{
	OLED_SCK_SET;
	OLED_SDA_SET;
	IIC_DELAY();
	OLED_SDA_RESET;
	IIC_DELAY();
	OLED_SCK_RESET;
	IIC_DELAY();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCK_RESET;
	OLED_SDA_RESET;
	IIC_DELAY();
	OLED_SCK_SET;
	IIC_DELAY();
	OLED_SDA_SET;
	IIC_DELAY();
	
}
/**********************************************
// IIC_Wait_Ack
**********************************************/
void IIC_Wait_Ack()
{
	IIC_DELAY();
	OLED_SCK_SET ;
	IIC_DELAY();
	OLED_SCK_RESET;
}

/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	u8 i;
	u8 m,da;
	da=IIC_Byte;
	OLED_SCK_RESET;
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80) OLED_SDA_SET;
		else OLED_SDA_RESET;
		da=da<<1;
		OLED_SCK_SET;
		OLED_SCK_RESET;
		
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}


//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	if(cmd)
	{

		Write_IIC_Data(dat);
    }
	else 
	{
		Write_IIC_Command(dat);
		
	}				 		  	  
} 
#else
u8 SPI1_WriteByte(u8 byte)
{

	while (!(HT_SPI1->SR & SPI_FLAG_TXBE));
/* Send byte through the SPIx peripheral  */
	HT_SPI1->DR = byte;
	

	while (!(HT_SPI1->SR & SPI_FLAG_RXBNE));
///* Return the byte read from the SPI    */
	return HT_SPI1->DR;
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	
	if(cmd)
	  OLED_DC_SET;
	else 
	  OLED_DC_RESET;		  
	OLED_CS_RESET;
	SPI1_WriteByte(dat);		 		  
	OLED_CS_SET;
	OLED_DC_SET;    
	
} 
#endif
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x >> 4) & 0x0f)|0x10,OLED_CMD);
	OLED_WR_Byte(( x & 0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


void OLED_Clear_OneLine(u8 y)
{
	u8 x =128,i = 2;
	while(i--)
	{
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte(0x00,OLED_CMD);
		OLED_WR_Byte(0x10,OLED_CMD);
		while(x--)
			OLED_WR_Byte(0,OLED_DATA);
		y++;
	}

}

void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 start_num,u8 num)
{      			    
	u8 t,i;
	OLED_Set_Pos(x,y);	
	for(i=0;i<num;i++)
	{
		OLED_Set_Pos(x+16*i,y);
		for(t=0;t<16;t++)
		{
			OLED_WR_Byte(Hzk[2*(start_num+i)][t],OLED_DATA);
			
		}	
		OLED_Set_Pos(x+16*i,y+1);	
		for(t=0;t<16;t++)
		{	
			OLED_WR_Byte(Hzk[2*(start_num+i)+1][t],OLED_DATA);
			
		}		
	}	
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    	for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
void OLED_GPIO_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.PB         = 1;
	CKCUClock.Bit.SPI1       = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_SPI);  //PA15 --- SPI_SCK
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_FUN_SPI);	//PB0 --- SPI_MOSI
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_1, AFIO_FUN_SPI);	//PB1 --- SPI_MISO
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
	
	SPI_DeInit(HT_SPI1);
	SPI_InitStructure.SPI_Mode = SPI_MASTER;
	SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
	SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
	SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;    //片选脚软件选择，可以自定义片选脚
	SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
	SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_SECOND;
	SPI_InitStructure.SPI_RxFIFOTriggerLevel = 0;
	SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
	SPI_InitStructure.SPI_ClockPrescaler = 4;  //时钟速率
	SPI_Init(HT_SPI1, &SPI_InitStructure);

	SPI_SELOutputCmd(HT_SPI1, ENABLE);
	SPI_Cmd(HT_SPI1, ENABLE);
	

	AFIO_GPxConfig(OLED_CS_PORT, OLED_CS_PIN, AFIO_FUN_GPIO);
	AFIO_GPxConfig(OLED_DC_PORT, OLED_DC_PIN, AFIO_FUN_GPIO);
	AFIO_GPxConfig(OLED_RST_PORT, OLED_RST_PIN, AFIO_FUN_GPIO);
	

	GPIO_DirectionConfig(OLED_CS_ID,OLED_CS_PIN,GPIO_DIR_OUT);
	GPIO_DirectionConfig(OLED_DC_ID,OLED_DC_PIN,GPIO_DIR_OUT);
	GPIO_DirectionConfig(OLED_RST_ID,OLED_RST_PIN,GPIO_DIR_OUT);
	
	OLED_CS_SET;
	OLED_DC_SET;
	OLED_RST_SET;
	
}
void OLED_Power_Init(void)
{
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_1, AFIO_FUN_GPIO);
	GPIO_DirectionConfig(HT_GPIOB,GPIO_PIN_1,GPIO_DIR_OUT);
	GPIO_DriveConfig(HT_GPIOB,GPIO_PIN_1,GPIO_DV_8MA);
	GPIO_WriteOutBits(HT_GPIOB,GPIO_PIN_1,RESET);
}
//初始化SSD1306					    
void OLED_Init(void)
{
	OLED_GPIO_Init();
	ms_delay(20);

	OLED_RST_SET;
	ms_delay(10);
	OLED_RST_RESET;
	ms_delay(10);
	OLED_RST_SET;
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
	
}  

