#include "ht32.h"
#include "oled.h"
#include "touchkey.h"
#include "card.h"
#include "card_spi.h"
#include "system_config.h"
#include "buzzer.h"
#include "motor.h"
#include "delay.h"
#include "password.h"
#include "rtc.h"
#include "string.h"
#include "oled_app.h"
#include "rtc.h"
extern vu8  touch_on;
extern  vu8 get_key;
//u8 WakeUp_Src;
//extern u16 WakeUpSrc;
extern	vu32  GoSleep_1ms;	
extern KeyType key;
extern vu8 NeedSleep;
extern u16 WakeUpSrc;
FuncDisableType  DisableFunction = {0};	//�������
void Warning_IRQ_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	
    EXTI_DeInit(EXTI_CHANNEL_6);
	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_6,AFIO_FUN_GPIO);
	GPIO_PullResistorConfig(HT_GPIOC,GPIO_PIN_6,GPIO_PR_DISABLE);
	GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_6,GPIO_DIR_IN);
	GPIO_InputConfig(HT_GPIOC,GPIO_PIN_6,ENABLE);
	
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_6, AFIO_ESS_PC);
	EXTI_InitStructure.EXTI_Channel = AFIO_EXTI_CH_6;
	EXTI_InitStructure.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
	EXTI_InitStructure.EXTI_DebounceCnt = 0;
	EXTI_InitStructure.EXTI_IntType =  EXTI_NEGATIVE_EDGE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_IntConfig(AFIO_EXTI_CH_6, ENABLE);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

}

void Login_GPIO_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	
    EXTI_DeInit(EXTI_CHANNEL_2);
	AFIO_GPxConfig(GPIO_PA,AFIO_PIN_2,AFIO_FUN_GPIO);
	GPIO_PullResistorConfig(HT_GPIOA,GPIO_PIN_2,GPIO_PR_DISABLE);
	GPIO_DirectionConfig(HT_GPIOA,GPIO_PIN_2,GPIO_DIR_IN);
	GPIO_InputConfig(HT_GPIOA,GPIO_PIN_2,ENABLE);
	
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PA);
	EXTI_InitStructure.EXTI_Channel = AFIO_EXTI_CH_2;
	EXTI_InitStructure.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
	EXTI_InitStructure.EXTI_DebounceCnt = 0;
	EXTI_InitStructure.EXTI_IntType =  EXTI_NEGATIVE_EDGE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_IntConfig(AFIO_EXTI_CH_2, ENABLE);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

}
	

	
void System_Init(void)
{
    
//========RTC=================
	RTC_Configuration();
//=======OLED��ʼ��============
	OLED_Power_Init();
	OLED_Init();
	
//=======����IC��ʼ��===================	
	
	KEY_GPIO_Init();
	KEY_IRQ_Init();
	
//=======ˢ����ʼ��===================	
    
	IC_GPIO_Init();
	InitRC522();
	
//======= ������ ======================
	BUZZ_PWM_OUT_Config();	//PA6
	
//=========	������� ==================
	Motor_Ctrl_Init();

//===========����=======================
	
	Warning_IRQ_Init();

//==========ע�ᰴ��=======================

	Login_GPIO_Init();

//=================ϵͳʱ��======================
	SysTick_Init(SYS_1ms);
	Enable_SysTick();
	

//============flash��ʼ��===========================
	Flash_Erase();
	Time_Init();
}

void Flash_Erase(void)
{
    u8 i;
	u32 Addr;
    Addr = Flash_CardIdAddress;
    for(i = 0;i < 4; i++)
    {
       FLASH_ErasePage(Addr);
       Addr += FLASH_PAGE_SIZE;
    }
	
	Addr = Flash_PasswordAddress;
    for(i = 0;i < 4; i++)
    {
        FLASH_ErasePage(Addr);
        Addr += FLASH_PAGE_SIZE;
    }
	
	FLASH_ErasePage(RTC_DATA_ADDR);


}
/**************************************************/
void fun_RCCInit_HSI8M_PLL40M(void)		//ϵͳʱ�ӳ�ʼ�� �ڲ�8MHz PLL��40MHz
{
	ErrStatus ClockStatus;
	CKCU_PLLInitTypeDef PLLInit;
	/* Reset CKCU, SYSCLK = HSI */
	CKCU_DeInit();

	/* PLL configuration, PLLCLK = 40 MHz  */
	PLLInit.ClockSource = CKCU_PLLSRC_HSI;   //CKCU_PLLSRC_HSI;
	PLLInit.CFG = CKCU_PLL_8M_40M;
	PLLInit.BYPASSCmd = DISABLE;
	CKCU_PLLInit(&PLLInit);

	CKCU_PLLCmd(ENABLE);

	/* Wait until PLL is ready */
	while(CKCU_GetClockReadyStatus(CKCU_FLAG_PLLRDY) == RESET);

	/* FLASH wait state configuration *///ht32f520xx_flash.c
	FLASH_SetWaitState(FLASH_WAITSTATE_1);  /* FLASH one wait clock */

	/* HCLK = SYSCLK/1  AHB��ʱ��Դ*/
	CKCU_SetHCLKPrescaler(CKCU_SYSCLK_DIV1);

	/* Configure PLL as system clock */
	ClockStatus = CKCU_SysClockConfig(CKCU_SW_PLL);

	if(ClockStatus != SUCCESS)
	{
	while(1);
	}
	
}

void Search_Compare_Card(void)
{
	u8 res;
	u8 GET_ICard[8];
	res=RC522_GetCard(GET_ICard);//ˢ������
	if(res == SUCCESS)
	{
		ms_delay(50);
		res=RC522_GetCard(GET_ICard);
		if(res == SUCCESS)
		{
			ms_delay(50);
			res=RC522_GetCard(GET_ICard);
			if(res == SUCCESS)
			{
				if(DisableFunction.IcardErrNum < MaxErrNum)
				{
					if(CardNumberIsExists(GET_ICard,Flash_CardIdAddress,20)!=0)
					{
						Unlock();                   //����
						Show_DoorOpen();
						ms_delay(500);
						DisableFunction.IcardErrNum = 0;
					}
					else
					{
						Show_InputInvalid();
						ms_delay(1000);
						DisableFunction.IcardErrNum++;
						if(DisableFunction.IcardErrNum >= MaxErrNum)
						{
							//RTC_Cmd(ENABLE);
							//ms_delay(2);
							DisableFunction.DisabEndTime =  RTC_GetCounter() + FuncDisTime;
						}	
					}
				}
				else
				{
					Show_FuncDisable(DisableFunction);		//��ʾ �ã���������ʽ��ʱʧ��
					GoSleep_1ms = 3000;
				}
			}
		}
	}
	if(DisableFunction.IcardErrNum >= MaxErrNum)
	{
		if(RTC_GetCounter() >= DisableFunction.DisabEndTime)
		{
			DisableFunction.IcardErrNum = 0;
		}

	}

}

vu8 KeyValue = 0;
vu8 GetValue_Flag = 0;
void Search_Compare_KeyValue(void)
{
	KeyType EnterPwd = {0};
	KeyType AdminPwd = {0};
	u16 Unlocker;
	KeyValue = touchkey_scan();

	if(KeyValue != 0)
	{
		if(GetValue_Flag==0)
		{
			GetValue_Flag=0x01;
			if(DisableFunction.PwdErrNum < MaxErrNum)				//��������û�е����ƴ���
			{
				EnterPwd = Show_Password(1, 3,KeyValue, 15, 1);	//(X,Y,�޶�����,����)
				//ֱ�ӿ��� ��������
				if(EnterPwd.LastKey == '#')
				{
					if(EnterPwd.len == 1 && EnterPwd.Num[0] == '0')
					{
						OLED_Clear();
						OLED_ShowString(0, 0, "Admin Password:", 16);
						AdminPwd = KeyEnterPwdLoop(0, 4, 14, 1);
						if(AdminPwd.LastKey == '*')	//��ȡ����
						{
							KeyValue = 0x00;
						}
						if(AdminPwd.LastKey == '#')	//��ȡ����
						{
							if(PwdKeyIsExists(AdminPwd, Flash_PasswordAddress, 20) != 0)
								Set_DateTime();

						}
						
					}
					else
					{
						Unlocker = VirtualPassword(EnterPwd, Flash_PasswordAddress);
						//u16 Unlocker = PwdKeyIsExists(EnterPwd,Flash_PasswordAddress,20);
						if(Unlocker == 0)		//У�鲻ͨ��		
						{
							Show_InputInvalid();
							key.len = 0;							
							GoSleep_1ms = GoSleepTime;
							DisableFunction.PwdErrNum++;
							if(DisableFunction.PwdErrNum >= MaxErrNum)	
							{
								DisableFunction.DisabEndTime = (HT_RTC->CNT) + FuncDisTime;
							}
						}	
						else 				//�����ɹ�					
						{	
							Unlock();                   //����
							Show_DoorOpen();
							ms_delay(500);
							key.len = 0;
							DisableFunction.PwdErrNum = 0;

						}

					}
				}
				else if(EnterPwd.LastKey == '*')		//������
				{	
					OLED_Clear();
					OLED_ShowString(0,3,"ReInput Password",16);
				}
			
			}
			else
			{
				Show_FuncDisable(DisableFunction);		//��ʾ �ã���������ʽ��ʱʧ��
				GoSleep_1ms = 3000;
			}
		}
	}
	else
	{
		GetValue_Flag=0;
	}

    if(DisableFunction.PwdErrNum >= MaxErrNum)
    {
        if(RTC_GetCounter() >= DisableFunction.DisabEndTime)
        {
	  		DisableFunction.PwdErrNum= 0;
	  	}

    }

}

void GotoSleep(void)		//���˯�� 1 ģʽ
{
	get_key = 0;
	key.len = 0;
	GoSleep_1ms = 0;
	OLED_Display_Off();
	TM_DeInit(HT_GPTM0);
	GPIO_WriteOutBits(RES_IO_GPIO_PORT,RES_IO_PIN,RESET); //ˢ������˯��
	ms_delay(1);
	{
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7, AFIO_MODE_DEFAULT);
		GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7, GPIO_PR_DISABLE);
		GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7, GPIO_DIR_IN);					
		GPIO_InputConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7, DISABLE);
		
		AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_7, AFIO_MODE_DEFAULT);
		GPIO_PullResistorConfig(HT_GPIOC, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_7, GPIO_PR_DISABLE);
		GPIO_DirectionConfig(HT_GPIOC, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_7, GPIO_DIR_IN);					
		GPIO_InputConfig(HT_GPIOC, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_7, DISABLE);
		
		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, AFIO_MODE_DEFAULT);
		GPIO_PullResistorConfig(HT_GPIOB, AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, GPIO_PR_DISABLE);
		GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, GPIO_DIR_IN);					
		GPIO_InputConfig(HT_GPIOB, AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, DISABLE);
		
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, AFIO_MODE_DEFAULT);
		GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, GPIO_PR_DISABLE);
		GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, GPIO_DIR_IN);					
		GPIO_InputConfig(HT_GPIOA, AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_10|AFIO_PIN_11|AFIO_PIN_12|AFIO_PIN_13|AFIO_PIN_14|AFIO_PIN_15, DISABLE);
		
		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8, AFIO_MODE_DEFAULT);
		GPIO_PullResistorConfig(HT_GPIOB, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8, GPIO_PR_DISABLE);
		GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8, GPIO_DIR_IN);					
		GPIO_InputConfig(HT_GPIOB, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_4|AFIO_PIN_5|AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8, DISABLE);
	
	
	}
//	{
//		//******************ˢ��*********PA8/10/11*******PB2/3/5**********����******PB7/8*****���**PB15****PC0***
//		
//		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8|AFIO_PIN_10|AFIO_PIN_11, AFIO_MODE_DEFAULT);
//		GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11, GPIO_DIR_IN);					
//		GPIO_InputConfig(HT_GPIOA, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11, DISABLE);
//		
//		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2|AFIO_PIN_3|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_15, AFIO_MODE_DEFAULT);						 
//		GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8, GPIO_DIR_IN);
//		GPIO_InputConfig(HT_GPIOB, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8, DISABLE);
//		
//		AFIO_GPxConfig(GPIO_PC, AFIO_PIN_0, AFIO_MODE_DEFAULT);
//		GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_IN);					
//		GPIO_InputConfig(HT_GPIOC, GPIO_PIN_0, DISABLE);
//		
//		//***********OLED����Ϊ����ʡ��*******PA14/15****PB0*****PA0/1******************************
//		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_14|AFIO_PIN_15, AFIO_MODE_DEFAULT);
//		GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14|GPIO_PIN_15, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14|GPIO_PIN_15, GPIO_DIR_IN);					
//		GPIO_InputConfig(HT_GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14|GPIO_PIN_15, DISABLE);
//		
//		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_MODE_DEFAULT);
//		GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_0, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_IN);					
//		GPIO_InputConfig(HT_GPIOB, GPIO_PIN_0, DISABLE);
//		
//		//******************LED****PA3/4/5*******************Bell****PA6***********************
//		
//		AFIO_GPxConfig(GPIO_PA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|AFIO_PIN_6, AFIO_MODE_DEFAULT);
//		GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PR_DISABLE);
//		GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_DIR_IN);					
//		GPIO_InputConfig(HT_GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, DISABLE);
//			
//	}
	
	NVIC_EnableIRQ(EVWUP_IRQn);
	/**************************************���� �жϻ���*********PB12**************************** */
	EXTI_WakeupEventConfig(EXTI_CHANNEL_12, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	/**************************���˹��ܻ���******************PC6**************************/
	EXTI_WakeupEventConfig(EXTI_CHANNEL_6, EXTI_WAKEUP_LOW_LEVEL, ENABLE);

	/**********************************ע�Ṧ�ܻ���************PA2****************/
	EXTI_WakeupEventConfig(EXTI_CHANNEL_2, EXTI_WAKEUP_LOW_LEVEL, ENABLE);

	EXTI_WakeupEventIntConfig(ENABLE);	

	PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFE);

//	{
//	    OLED_Init();
//        KEY_GPIO_Init();
//        IC_GPIO_Init();
//		GPIO_WriteOutBits(RES_IO_GPIO_PORT,RES_IO_PIN,RESET);
//        BUZZ_PWM_OUT_Config();
//        Motor_Ctrl_Init();
//	}

	{
		//************************OLED�˳�ʡ��ģʽ*******************************************
		OLED_Init();

		//**************************����****************************************************
		AFIO_GPxConfig(GPIO_PB,AFIO_PIN_7|AFIO_PIN_8,AFIO_MODE_1);	
		GPIO_DirectionConfig(HT_GPIOB,GPIO_PIN_7|GPIO_PIN_8,GPIO_DIR_OUT);

		GPIO_PullResistorConfig(HT_GPIOB, AFIO_PIN_8, GPIO_PR_UP);
		GPIO_InputConfig(HT_GPIOB, AFIO_PIN_8, ENABLE);

		//*********************************ˢ��**************************************************
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8|AFIO_PIN_10|AFIO_PIN_11, AFIO_FUN_GPIO);
		AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_FUN_GPIO);

		GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_8|AFIO_PIN_10, GPIO_DIR_OUT);
		GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_2|AFIO_PIN_3, GPIO_DIR_OUT);
		GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_11, GPIO_DIR_IN);
		GPIO_InputConfig(HT_GPIOA,AFIO_PIN_11,ENABLE);

		/* Configure GPIO pull resistor of input pins                                                             */
		GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_11, GPIO_PR_UP);
		SET_SPI_CS;//CS�˿�Ϊ�ߵ�ƽ��cs��Ч 
		PcdReset();//оƬ��λ
		//*********************Bell**********���******************************
		BUZZ_PWM_OUT_Config();
		AFIO_GPxConfig(GPIO_PB, GPIO_PIN_15, AFIO_FUN_GPIO);
		AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0, AFIO_FUN_GPIO);
		GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_15, GPIO_DIR_OUT);
		GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
	
	}
	OLED_Display_On();
	OLED_Clear();
	OLED_ShowString(4,2,"  HT DoorLock",16);
	Show_Time(0, 6, RTC_DATA_ADDR);
	GoSleep_1ms = 10000;
	 
}








