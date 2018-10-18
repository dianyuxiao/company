#include "Motor.h"
#include "ht32_board.h"

extern vu16   Motor_Action_1ms;		//蜂鸣器倒计时
extern vu16   UnlockHold_1ms;			//开锁保持时间
u8 	LockIsOpen = 0;		//门锁 状态 = 1，当前门锁打开，=0当前门锁未打开
u8 	Lock_State_Flag	=0;		//门状态标志位		已经关闭=0,     已经打开=1

						//			 SRR  = 1                        RR = 0
#define		MotorON1	{HT_GPIOB->SRR = GPIO_PIN_15; HT_GPIOC->RR = GPIO_PIN_0;}
#define		MotorON2	{HT_GPIOC->SRR = GPIO_PIN_0; HT_GPIOB->RR = GPIO_PIN_15;}
#define		MotorOFF	{HT_GPIOB->RR  = GPIO_PIN_15; HT_GPIOC->RR = GPIO_PIN_0;}

void Motor_Ctrl_Init(void)
{
	
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_15, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0, AFIO_FUN_GPIO);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_15, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);		
	MotorOFF
}


void Motor_Lock_Action(void)	//马达执行 上锁 动作
{
	Motor_Action_1ms = Motor_Action_Time;
	Lock_State_Flag =0;
	HT32F_DVB_LEDOn(HT_LED1);
	MotorON1
	
}

void Motor_UnLock_Action(void)	//马达执行 开锁 动作
{
	Motor_Action_1ms = Motor_Action_Time;
	if(Lock_State_Flag == 0)
	{
		Lock_State_Flag =1;
		HT32F_DVB_LEDOn(HT_LED1);
		MotorON2
	}
}

void Motor_OFF_Action(void)	//马达执行待机动作
{
	HT32F_DVB_LEDOff(HT_LED1);
	MotorOFF
}


void Unlock(void)	//开锁：保持打开一段时间然后自动关锁
{
	Motor_UnLock_Action();
	LockIsOpen = 0;
	UnlockHold_1ms   = UnlockHold_Time;		//锁保持打开状态时间	
}





