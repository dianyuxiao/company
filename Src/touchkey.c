#include "ht32.h"
#include "key_iic.h"
#include "touchkey.h"
#include "delay.h"
#include "OLED.h"
#include "ht32_board_config.h"




void KEY_GPIO_Init(void)
{
	//SDA SCL
	GPIO_DirectionConfig(I2C_SCL_PORT,I2C_SCL_PIN,GPIO_DIR_OUT);
	GPIO_DirectionConfig(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_DIR_OUT);
	AFIO_GPxConfig(I2C_SCL_ID,I2C_SCL_PIN,AFIO_MODE_1);
	AFIO_GPxConfig(I2C_SDA_ID,I2C_SDA_PIN,AFIO_MODE_1);
	/* Configure GPIO pull resistor of input pins                                                             */
	GPIO_PullResistorConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PR_UP);
	GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, ENABLE);


}
void KEY_IRQ_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	//开中断
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3,AFIO_FUN_GPIO);
	GPIO_PullResistorConfig(HT_GPIOC, AFIO_PIN_3, GPIO_PR_UP);
	GPIO_DirectionConfig(HT_GPIOC, AFIO_PIN_3, GPIO_DIR_IN);
	GPIO_InputConfig(HT_GPIOC, AFIO_PIN_3, ENABLE);
	
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_3, AFIO_ESS_PC);
	EXTI_InitStruct.EXTI_Channel = AFIO_EXTI_CH_3;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType =  EXTI_NEGATIVE_EDGE;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_IntConfig(AFIO_EXTI_CH_3, ENABLE);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

}

int16_t key_config_init(void)
{  
    int16_t result=0;
    uint8_t temp_arrays[22];
    uint8_t i=0,temp=0;

    for(temp=0;temp<3;)
    {
        fun_BS83B16A_WriteData(KEY_WRITE_SET, sensitivity_arrays,22);
        ms_delay(1000);

        fun_BS83B16A_ReadData(KEY_WRITE_SET,temp_arrays,22); 

        for(i=0;i<22;i++)
        {
            if(temp_arrays[i]!=sensitivity_arrays[i])
            {
                temp++;
                result=100;
                break;
            }
            else
            {
                result=0;
                continue;
            }
        }

        if(result==0)
            break;

        ms_delay(1);
    }

    if(result>5)
        return ERROR;
    
    return SUCCESS;
}


//volatile uint8_t  key_flags=0xFF;
volatile uint16_t key_value=0; 
uint16_t    uwTouchBits=0;
uint8_t   SameKey_Cnt=0;
uint16_t KeyOld;
int16_t touchkey_scan(void)         // 定时扫描程序负责生成KEY信息
{
    uint8_t     ucKey=0; 
    uint8_t     i=0;   
    uint16_t    uwBit;
	uint16_t KeyCurrent = 0;
	uint8_t     key_BS83B16C[2];
    fun_BS83B16A_ReadData(KEY_OUT_ONE,key_BS83B16C,2); 
  //  uwTouchBits =((uint16_t)key_BS83B16C[1]<<8) | ((uint16_t)key_BS83B16C[0]);
	KeyCurrent =((uint16_t)key_BS83B16C[1]<<8) | ((uint16_t)key_BS83B16C[0]);
//	uwTouchBits = KeyCurrent & (KeyCurrent ^ KeyOld);
//	KeyOld = KeyCurrent;
	if(KeyCurrent==KeyOld)
	{
		SameKey_Cnt++;
		if(SameKey_Cnt>3)
		{
			uwTouchBits=KeyCurrent;
		}
	}
	else
	{
		KeyOld=KeyCurrent;
		SameKey_Cnt=0;
	}
    for(i=0;i<MAX_KEY_NUM;i++)
    {
        uwBit=(uwTouchBits>>i)&0x0001; 
        if(uwBit)
        {
            ucKey=ucKey_index[i];
            break;
        }    
    }
	return ucKey;
}


//***************************************************//
int16_t fun_BS83B16A_WriteData(uint8_t Register_Address, uint8_t *buff, uint8_t count)
{
	unsigned char i;
	int16_t result;
	fun_iicmaster_start();

	fun_iic_bytewrite(WR_ADDR);	//
	if(fun_iic_wait_ack()==SUCCESS)
	{
		fun_iic_bytewrite(Register_Address);		//
		if(fun_iic_wait_ack()==SUCCESS)		
		{
			for(i=0;i<count;i++)
			{
				fun_iic_bytewrite(buff[i]);
				fun_iic_wait_ack();
			}	
			result=SUCCESS;
		}
	}		
	else  result=ERROR;		
		
	fun_iicmaster_stop();
	return result;
}


//***************************************************//
int16_t fun_BS83B16A_ReadData(uint8_t Register_Address, uint8_t *buff, uint8_t count)
{
	uint8_t   i;
	int16_t   result=ERROR;
	fun_iicmaster_start();
		fun_iic_bytewrite(WR_ADDR);
		if(fun_iic_wait_ack()==SUCCESS)
		{
			
			fun_iic_bytewrite(Register_Address);		//
			fun_iic_wait_ack();
			fun_iicmaster_stop();
			us_delay(20);
			fun_iicmaster_start();
			fun_iic_bytewrite(RD_ADDR);
			
			if(fun_iic_wait_ack()==SUCCESS)
			{
			
				
				for(i=0;i<count;i++)
				{
					if(i+1<count)
					{
						us_delay(100);
						buff[i]=fun_iic_byteread();
						fun_iic_ack();
		  
					}
					else
					{
					    us_delay(100);
						buff[i]=fun_iic_byteread();
						fun_iic_Nack();
					}
				}
			}
			
			result=SUCCESS;
				
		}
		else
		    result=ERROR;
		fun_iicmaster_stop();
	
	return result;
}




