#ifndef __BUZZ_H__
#define __BUZZ_H__

#include "ht32.h"
#define BEE_FREQ          4000

void BUZZ_PWM_OUT_Config(void);
void BUZZ_ON_SET(u16 tim);		//开启蜂鸣器，一段时间，ms
void BUZZ_ON(void);
void BUZZ_OFF(void);
void DoorOpen_Success(void);

#endif














