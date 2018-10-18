#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H



#define Flash_PasswordAddress    1024*56
#define Flash_CardIdAddress      1024*60
#define GoSleepTime               10000


#define DISABLE_DEBUG_PIN       (1)

#define WakeUp_SRC_Touch         0xf0
#define WakeUp_SRC_Admin         0xf1
#define WakeUp_SRC_Warning       0xf2
#define WakeUp_SRC_SetDate       0xf3

#define IRQ_Key                  0xe0
#define IRQ_Card                 0xe1

#define 	MaxErrNum					    5							//某开锁方式最大连续错误次数，超过则
#define 	FuncDisTime						20					//失能该开锁功能，这么多 秒


void GotoSleep(void);
void System_Init(void);
void fun_RCCInit_HSI8M_PLL40M(void);
void Flash_Erase(void);
void Search_Compare_KeyValue(void);
void Search_Compare_Card(void);

#endif
