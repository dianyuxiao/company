#ifndef _DELAY_H
#define _DELAY_H

#include "ht32.h"

#define SYS_1us 1000000      // 1us不准确导致Systick初始化失败死到while(1);
#define SYS_10us 100000        
#define SYS_1ms 1000

void SysTick_Init(uint32_t ticks);
void Disable_SysTick(void);
void Enable_SysTick(void);
//void delay_us(uint32_t times);
//void delay_10us(uint32_t times);
//void delay_ms(uint32_t times);
void IIC_DELAY(void);
void us_delay(uint32_t times);
void ms_delay(uint32_t times);
#endif


