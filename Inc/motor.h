#ifndef __MOTOR_h__
#define __MOTOR_h__

#include "ht32.h"

#define 	Motor_Action_Time			200						//门锁马达的动作时间，开锁或关锁	200ms
#define     UnlockHold_Time			5000					//开锁保持时间 ms

void Motor_Ctrl_Init(void);
void Motor_Lock_Action(void);	//马达执行 上锁 动作
void Motor_UnLock_Action(void);	//马达执行 开锁 动作
void Motor_OFF_Action(void);	//马达执行 待机 动作
void Unlock(void);	//开锁：保持打开一段时间然后自动关锁

#endif
