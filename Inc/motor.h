#ifndef __MOTOR_h__
#define __MOTOR_h__

#include "ht32.h"

#define 	Motor_Action_Time			200						//�������Ķ���ʱ�䣬���������	200ms
#define     UnlockHold_Time			5000					//��������ʱ�� ms

void Motor_Ctrl_Init(void);
void Motor_Lock_Action(void);	//���ִ�� ���� ����
void Motor_UnLock_Action(void);	//���ִ�� ���� ����
void Motor_OFF_Action(void);	//���ִ�� ���� ����
void Unlock(void);	//���������ִ�һ��ʱ��Ȼ���Զ�����

#endif
