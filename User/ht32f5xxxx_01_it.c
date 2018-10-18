/*********************************************************************************************************//**
 * @file    PWRCU/BackupData/ht32f5xxxx_01_it.c
 * @version $Rev:: 1704         $
 * @date    $Date:: 2017-08-17 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "system_config.h"
#include "ht32_board.h"
#include "touchkey.h"
#include "oled.h"
#include <string.h>
#include "delay.h"
#include "card.h"
#include "buzzer.h"
#include "motor.h"
u16 WakeUpSrc;
vu8 Search_Card = 0;
vu8 touch_on = 0;
vu8 get_key;
u8 timer = 0;
u8 Start_GetKeyValue = 0;
extern u8 SearchCard_Finish;
vu16 	BUZZ_1ms = 0;								//蜂鸣器倒计时
vu16 	Motor_Action_1ms = 0;				//马达动作倒计时
vu16 	UnlockHold_1ms = 0;					//开锁保持时间
vu32    GoSleep_1ms = 0;					//进入睡眠倒计时
vu8     NeedSleep = 0;						//=1：需要进入睡眠
extern vu8 IsEnterLoop;
extern vu8 LockIsOpen;
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup PWRCU_Examples PWRCU
  * @{
  */

/** @addtogroup BackupData
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}
void SysTick_Handler(void)
{

	if(BUZZ_1ms > 0)
	{
		if(BUZZ_1ms == 1)							//最后1ms。关闭蜂鸣器
			BUZZ_OFF();
		BUZZ_1ms--;
	}
	
	if(Motor_Action_1ms > 0)				//动作倒计时，打开和关闭的马达通电时间
	{
		if(Motor_Action_1ms == 1)			//最后1ms，关闭马达
		{	
			Motor_OFF_Action();
			if(LockIsOpen == 0)					//改变 门锁 状态标志位
				LockIsOpen = 1;
			else
				LockIsOpen = 0;
		}
		Motor_Action_1ms--;
	}
	else if(LockIsOpen == 1)				//如果门锁处于打开状态，开始 打开状态倒计时 ：期间，马达静止，门锁处于打开状态
	{
		if(UnlockHold_1ms == 1)				//门锁打开状态倒计时最后1ms
		{
			Motor_Lock_Action();				//执行关锁动作
		}
		UnlockHold_1ms--;
	}
	if(GoSleep_1ms > 0 )
	{
		if(GoSleep_1ms == 1)
		{
			NeedSleep = 1;
			IsEnterLoop = 0;
		}
		GoSleep_1ms--;
	}

	
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 2~3 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)				
{
	if(EXTI_GetEdgeStatus(AFIO_EXTI_CH_2,EXTI_NEGATIVE_EDGE)==SET)
	{
		
		WakeUpSrc = WakeUp_SRC_Admin;
		EXTI_ClearEdgeFlag(AFIO_EXTI_CH_2);
	}
	if(EXTI_GetEdgeStatus(AFIO_EXTI_CH_3,EXTI_NEGATIVE_EDGE)==SET)
	{	
		Search_Card = 1;
		get_key = touchkey_scan();
		touch_on = 1;
		GoSleep_1ms = 10000;
		WakeUpSrc = WakeUp_SRC_Touch;
		EXTI_ClearEdgeFlag(AFIO_EXTI_CH_3);
	}
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetEdgeStatus(AFIO_EXTI_CH_6,EXTI_NEGATIVE_EDGE)==SET)
	{
		
		WakeUpSrc = WakeUp_SRC_Warning;
		EXTI_ClearEdgeFlag(AFIO_EXTI_CH_6);
	}
	
}
/*********************************************************************************************************//**
 * @brief   This function handles EXTI Events Wakeup interrupt.
 ************************************************************************************************************/
void EVWUP_IRQHandler(void)
{
	if (EXTI_GetWakeupFlagStatus(EXTI_CHANNEL_3))
	{
		EXTI_WakeupEventConfig(EXTI_CHANNEL_3, EXTI_WAKEUP_LOW_LEVEL, DISABLE);
		EXTI_ClearWakeupFlag(EXTI_CHANNEL_3);
		WakeUpSrc = WakeUp_SRC_Touch;
	}
	if (EXTI_GetWakeupFlagStatus(EXTI_CHANNEL_6))
	{
        
		EXTI_WakeupEventConfig(EXTI_CHANNEL_6, EXTI_WAKEUP_LOW_LEVEL, DISABLE);
		EXTI_ClearWakeupFlag(EXTI_CHANNEL_6);
		WakeUpSrc = WakeUp_SRC_Warning;
		
	}
	if (EXTI_GetWakeupFlagStatus(EXTI_CHANNEL_2))
	{
        
		EXTI_WakeupEventConfig(EXTI_CHANNEL_2, EXTI_WAKEUP_LOW_LEVEL, DISABLE);
		EXTI_ClearWakeupFlag(EXTI_CHANNEL_2);
		WakeUpSrc = WakeUp_SRC_Admin;

	}

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
