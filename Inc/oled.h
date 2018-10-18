#include "ht32.h"
#include "touchkey.h"
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 

#define OLED_MODE 0  //SPI模式
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/********************SPI模式端口定义*********************************************/

#define OLED_SCK_RESET    GPIO_WriteOutBits(OLED_SCK_ID,OLED_SCK_PIN,RESET)//CLK
#define OLED_SCK_SET      GPIO_WriteOutBits(OLED_SCK_ID,OLED_SCK_PIN,SET)

#define OLED_SDIN_RESET    GPIO_WriteOutBits(OLED_SDIN_ID,OLED_SDIN_PIN,RESET)//DIN
#define OLED_SDIN_SET      GPIO_WriteOutBits(OLED_SDIN_ID,OLED_SDIN_PIN,SET)

#define OLED_CS_RESET      GPIO_WriteOutBits(OLED_CS_ID,OLED_CS_PIN,RESET)
#define OLED_CS_SET        GPIO_WriteOutBits(OLED_CS_ID,OLED_CS_PIN,SET)

#define OLED_DC_RESET      GPIO_WriteOutBits(OLED_DC_ID,OLED_DC_PIN,RESET)
#define OLED_DC_SET        GPIO_WriteOutBits(OLED_DC_ID,OLED_DC_PIN,SET)

#define OLED_RST_RESET     GPIO_WriteOutBits(OLED_RST_ID,OLED_RST_PIN,RESET)
#define OLED_RST_SET       GPIO_WriteOutBits(OLED_RST_ID,OLED_RST_PIN,SET)

#define OLED_SCK_PORT     GPIO_PA
#define OLED_SCK_ID		  HT_GPIOA
#define OLED_SCK_PIN	  AFIO_PIN_15

#define OLED_SDIN_PORT     GPIO_PB
#define OLED_SDIN_ID	   HT_GPIOB
#define OLED_SDIN_PIN      AFIO_PIN_0

#define OLED_CS_PORT      GPIO_PA
#define OLED_CS_ID        HT_GPIOA
#define OLED_CS_PIN       AFIO_PIN_14

#define OLED_DC_PORT      GPIO_PA
#define OLED_DC_ID        HT_GPIOA
#define OLED_DC_PIN       AFIO_PIN_0

#define OLED_RST_PORT     GPIO_PA
#define OLED_RST_ID       HT_GPIOA
#define OLED_RST_PIN      AFIO_PIN_1


/************************************I2C模式端口定义***********************************************************************************/

//#define OLED_SCLK_PORT     GPIO_PA
//#define OLED_SCLK_ID	  HT_GPIOA
//#define OLED_SCLK_PIN	  AFIO_PIN_15

//#define OLED_SDA_PORT     GPIO_PB
//#define OLED_SDA_ID	      HT_GPIOB
//#define OLED_SDA_PIN      AFIO_PIN_0   

//#define OLED_SCLK_RESET    GPIO_WriteOutBits(OLED_SCLK_ID,OLED_SCK_PIN,RESET)//CLK
//#define OLED_SCLK_SET      GPIO_WriteOutBits(OLED_SCLK_ID,OLED_SCK_PIN,SET)

//#define OLED_SDA_RESET    GPIO_WriteOutBits(OLED_SDA_ID,OLED_SDIN_PIN,RESET)//DIN
//#define OLED_SDA_SET      GPIO_WriteOutBits(OLED_SDA_ID,OLED_SDIN_PIN,SET)



#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
	     

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Power_Init(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);	 
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ShowCHinese(u8 x,u8 y,u8 start_num,u8 num);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void OLED_Clear_OneLine(u8 y);
#endif  
	 



