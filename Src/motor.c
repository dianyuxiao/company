#include "Motor.h"
#include "ht32_board.h"

extern vu16   Motor_Action_1ms;		//����������ʱ
extern vu16   UnlockHold_1ms;			//��������ʱ��
u8 	LockIsOpen = 0;		//���� ״̬ = 1����ǰ�����򿪣�=0��ǰ����δ��
u8 	Lock_State_Flag	=0;		//��״̬��־λ		�Ѿ��ر�=0,     �Ѿ���=1

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


void Motor_Lock_Action(void)	//���ִ�� ���� ����
{
	Motor_Action_1ms = Motor_Action_Time;
	Lock_State_Flag =0;
	HT32F_DVB_LEDOn(HT_LED1);
	MotorON1
	
}

void Motor_UnLock_Action(void)	//���ִ�� ���� ����
{
	Motor_Action_1ms = Motor_Action_Time;
	if(Lock_State_Flag == 0)
	{
		Lock_State_Flag =1;
		HT32F_DVB_LEDOn(HT_LED1);
		MotorON2
	}
}

void Motor_OFF_Action(void)	//���ִ�д�������
{
	HT32F_DVB_LEDOff(HT_LED1);
	MotorOFF
}


void Unlock(void)	//���������ִ�һ��ʱ��Ȼ���Զ�����
{
	Motor_UnLock_Action();
	LockIsOpen = 0;
	UnlockHold_1ms   = UnlockHold_Time;		//�����ִ�״̬ʱ��	
}





