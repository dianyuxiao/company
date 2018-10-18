#include "ht32.h"
#include "delay.h"
#include "system_config.h"
#include "buzzer.h"
#include "admin.h"
#include "card.h"
#include "rtc.h"
#include "time.h"
#include "oled_app.h"
#include "oled.h"

/* Private function prototypes -----------------------------------------------------------------------------*/
void NVIC_Configuration(void);
void CKCU_Configuration(void);
void fun_RCCInit_HSI8M_PLL40M(void);
/* Private constants ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
	extern vu8 get_key;
	extern vu8 	NeedSleep;
	extern u16 WakeUpSrc;
	extern vu8 Search_Card;
	extern u8 Start_GetKeyValue;

	fun_RCCInit_HSI8M_PLL40M();
	
	/* 初始化参数*/
	CKCU_Configuration();
	NVIC_Configuration();

	
	System_Init();

	//GotoSleep();

	while(1)
	{			

        switch(WakeUpSrc)
        {
            case WakeUp_SRC_Touch:           //刷卡  按键
				if(Search_Card == 1)
				 {
				    Search_Card = 0;
			        Search_Compare_Card();
			     } 

			
                    Search_Compare_KeyValue();
				 
            break;
            case WakeUp_SRC_Admin:           //注册
				 EnterAdminSet();
            break;
            case WakeUp_SRC_Warning:         //报警
                 BUZZ_ON_SET(5000);
                 Show_Warning();
				 ms_delay(5000);
            break;
			default:
			break;
        
		}
		if(NeedSleep == 1)
		{
			NeedSleep = 0;
			//GotoSleep();
		}
	}
	
}

/*********************************************************************************************************//**
  * @brief  Configure the NVIC vector table.
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x00);     /* Set the Vector Table Offset          */	
}

/*********************************************************************************************************//**
  * @brief  Configure the system clocks.
  * @retval None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.PDMA       = 0;
  CKCUClock.Bit.USBD       = 0;
  CKCUClock.Bit.CKREF      = 0;
  CKCUClock.Bit.EBI        = 0;
  CKCUClock.Bit.CRC        = 0;
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 0;
  CKCUClock.Bit.I2C0       = 0;
  CKCUClock.Bit.I2C1       = 0;
  CKCUClock.Bit.SPI0       = 1;
  CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 0;
  CKCUClock.Bit.USART1     = 0;
  CKCUClock.Bit.UART0      = 0;
  CKCUClock.Bit.UART1      = 0;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.EXTI       = 1;
  CKCUClock.Bit.SCI0       = 0;
  CKCUClock.Bit.SCI1       = 0;
  CKCUClock.Bit.I2S        = 0;
  CKCUClock.Bit.MCTM0      = 0;
  CKCUClock.Bit.WDT        = 0;
  CKCUClock.Bit.BKP        = 1;
  CKCUClock.Bit.GPTM0      = 0;
  CKCUClock.Bit.GPTM1      = 0;
  CKCUClock.Bit.BFTM0      = 0;
  CKCUClock.Bit.BFTM1      = 0;
  CKCUClock.Bit.CMP        = 0;
  CKCUClock.Bit.ADC        = 0;
  CKCUClock.Bit.SCTM0      = 0;
  CKCUClock.Bit.SCTM1      = 0;
  CKCUClock.Bit.SCTM2      = 0;
  CKCUClock.Bit.SCTM3      = 0;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

}









