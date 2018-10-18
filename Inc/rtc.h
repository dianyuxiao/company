#ifndef __RTC_H__
#define __RTC_H__

#include "ht32.h"
//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 year;
	vu8  mon;
	vu8  day;		 
}calendar;		

#define RTC_DATA_ADDR  1024 * 40

void RTC_Configuration(void);
u32 Set_DateTime(void);
void Show_Time(u8 x, u8 y,u32 Addr);
void RTC_Set(calendar save_time);
calendar RTC_Get(u32 Addr);
void Time_Init(void);
#endif


