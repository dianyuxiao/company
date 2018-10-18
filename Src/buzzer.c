#include "buzzer.h"
#include "delay.h"
extern  vu16 BUZZ_1ms;								//·äÃùÆ÷µ¹¼ÆÊ±
void BUZZ_PWM_OUT_Config(void)
{
	TM_TimeBaseInitTypeDef TimeBaseInit;
	TM_OutputInitTypeDef   OutputInit;
	uint32_t wCRR = 0, wPSCR = 0;
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.AFIO       	= 1;
	CKCUClock.Bit.PA      		= 1;
	CKCUClock.Bit.GPTM0     	= 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	/* Configure the GPIO_PIN as TM channel output AFIO function                                       */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_MODE_4);

	//**GPTM0**************************************************************************	
	//4KHZ
	wCRR = (SystemCoreClock / BEE_FREQ) - 1;
	while ((wCRR / (wPSCR + 1)) > 0xFFFF)
	{
		wPSCR++;
	}
	wCRR = wCRR / (wPSCR + 1);
  TimeBaseInit.CounterReload = wCRR;
  TimeBaseInit.Prescaler = wPSCR;
  TimeBaseInit.RepetitionCounter = 0;
  TimeBaseInit.CounterMode = TM_CNT_MODE_UP;
  TimeBaseInit.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
	TM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);
	//---------------------------------------------	
	//Init GPTM0 Channel 0 to output PWM waveform with 20% duty */
	OutputInit.Channel = TM_CH_2;
	OutputInit.OutputMode = TM_OM_PWM2;
	OutputInit.Control = TM_CHCTL_ENABLE;
	OutputInit.ControlN = TM_CHCTL_DISABLE;
	OutputInit.Polarity = TM_CHP_NONINVERTED;
	OutputInit.PolarityN = TM_CHP_NONINVERTED;
	OutputInit.IdleState = MCTM_OIS_LOW;
	OutputInit.IdleStateN = MCTM_OIS_HIGH;
	OutputInit.Compare = ((wCRR + 1) * (100 - 33)) / 100;
	TM_OutputInit(HT_GPTM0, &OutputInit);
	
	//TM_IntConfig(HT_GPTM1, TM_INT_UEV, ENABLE);
	//TM_Cmd(HT_GPTM1, ENABLE);
}

void BUZZ_ON_SET(u16 tim)
{
	BUZZ_1ms = tim;
	BUZZ_ON();
}


void BUZZ_ON(void)
{
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_MODE_4);

	TM_Cmd(HT_GPTM0, ENABLE);
}

void  BUZZ_OFF(void)	 
{
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_MODE_DEFAULT);
	TM_Cmd(HT_GPTM0, DISABLE); 
}
















