#ifndef __OLED_APP_H
#define __OLED_APP_H

#include  "ht32.h"
#include "touchkey.h"


void Show_SetDatetime(void);

void Show_TimeSetOK(void);

void Show_InputInvalid(void);

void Show_Warning(void);

void Show_DoorOpen(void);

void Show_FuncDisable(FuncDisableType UnlockWay);

void Show_CardExist(void);

void Show_PasswordExist(void);

void Show_AdminSet(void);

void Show_PasswordError(void);

#endif




