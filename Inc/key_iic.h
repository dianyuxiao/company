#ifndef __KEY_IIC_H
#define __KEY_IIC__H

#include "ht32.h"

#define I2C_SCL_PORT			HT_GPIOB
#define I2C_SCL_ID				GPIO_PB
#define I2C_SCL_PIN				AFIO_PIN_7

#define I2C_SDA_PORT			HT_GPIOB
#define I2C_SDA_ID				GPIO_PB
#define I2C_SDA_PIN				AFIO_PIN_8

#define SET_I2C_SCL      GPIO_WriteOutBits(I2C_SCL_PORT,I2C_SCL_PIN,SET)
#define RESET_I2C_SCL    GPIO_WriteOutBits(I2C_SCL_PORT,I2C_SCL_PIN,RESET)

#define SET_I2C_SDA      GPIO_WriteOutBits(I2C_SDA_PORT,I2C_SDA_PIN,SET)
#define RESET_I2C_SDA    GPIO_WriteOutBits(I2C_SDA_PORT,I2C_SDA_PIN,RESET)

void fun_config_sda_out(void);
void fun_iicmaster_start(void);
void fun_iicmaster_stop(void);
unsigned char fun_iic_wait_ack(void);
void fun_iic_ack(void);
void fun_iic_Nack(void);
void fun_iic_bytewrite(unsigned char lv8u_data);
uint16_t fun_iic_byteread(void);


#endif

