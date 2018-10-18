#include "ht32.h"
#include "delay.h"
#include "buzzer.h"
#include "motor.h"
#include "ht32_board.h"

/* Private variables ---------------------------------------------------------------------------------------*/
volatile uint32_t delay_count=0;

void SysTick_Init(uint32_t ticks)
{

	/* SYSTICK configuration */
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8
	if(SysTick_Config((SystemCoreClock/2)/ticks))
	{ 
		while(1);
	}  
}

void Disable_SysTick(void)
{
     SysTick->CTRL &= (~(1<<SysTick_CTRL_ENABLE_Msk) ); 
}

void Enable_SysTick(void)
{
    SysTick->CTRL |= ((1<<SysTick_CTRL_ENABLE_Msk) ); 
}

void delay_us(uint32_t times)
{
    delay_count=0;
    while(delay_count<times);
}

void delay_10us(uint32_t times)
{
    delay_count=0;
    while(delay_count<times);
}

void delay_ms(uint32_t times)
{
    uint32_t ms_number;
    for(ms_number=0;ms_number<times;ms_number++)
        delay_10us(100);
}

void us_delay(uint32_t times)
{
    while(times--)
    {
        __nop();
        __nop();

    }
}

void ms_delay(uint32_t times)
{
    uint32_t ms_number;
    for(ms_number=0;ms_number<times;ms_number++)
        us_delay(1700);
}

void IIC_DELAY(void)
{
	 unsigned int i;
	for(i=0;i<14;i++)
	{
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	
	}
    
}

