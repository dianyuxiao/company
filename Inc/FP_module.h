#ifndef __FP_PROTOCOL_H__
#define __FP_PROTOCOL_H__

#include "ht32.h"



#define TA_BREAK						0x18
#define TA_NOSENSOR						0x28
#define TA_FCLK_ERR						0x38

#define STM32F0_FLASH_BASE		    0x08000000UL
#define STM32F030K6					0x10000UL


/*----------------------------------------------------------------------
人为定义 扇区 一个区8k
扇区        区域说明        f1xxx flash页段     起始地址
sector 0-1  bootloader      page0-page15        0x08000000UL
sector 2    para1           page16-page23       0x08004000UL
sector 3    para2           page24-page31       0x08006000UL
sector 4-5  m3_app          page32-page47       0x08008000UL
sector 6-7  backup          page48-page63       0x0800c000UL
----------------------------------------------------------------------*/

#define FLASH_LEN				        STM32F030K6
#define BOOTLOADER_START		        STM32F0_FLASH_BASE
#define BOOTLOADER_LEN			        0x2000UL		//bootloader长度 8K  sector0 sector1 


#define TA_ALGO_PARAM_START	            (BOOTLOADER_START+BOOTLOADER_LEN)
#define TA_ALGO_PARAM_LEN		        0x1000UL		//参数区 4K 



#define API_LOGO_START		            (TA_ALGO_PARAM_START+TA_ALGO_PARAM_LEN)//0x08003000
#define API_LOGO_LEN		            0x4000UL//协议层大小为16k   sector3~ sector6


#define DB_START_ADDR                   (API_LOGO_START+API_LOGO_LEN)//0x08007000
#define DB_LEN                          (0x1000UL)      //备份falsh 4k	sector7

#define BOOT_VERSIONSIZE                 100


//return 
#define TA_SUCCESS                  0
#define TA_FAIL                     1

//upg
#define TA_PRO_FORMAT_ERR			(0x10)		//程序格式错误
#define TA_CHECK_ERROR				(0x11)		//数据校验出错
#define TA_LEN_OVERFLOW				(0x12)		//长度超长
#define TA_ADDR_FORMAT_ERR			(0x13)		//地址格式错误
#define TA_PKGR_LEN_ERR				(0x14)		//数据包长度不对
#define TA_FLASH_ERR				(0x15)		//flash编程出错
#define FINISHED_UPG			    (0x80)


#define  PROGRAM_LEN_SIGN			(4)

#define  PKG_LEN					(0x78+1)	//120+1
#define  REAL_PRO_PKG_LEN			(0x78)	    //120


//buad id
#define TA_BAUD_9600    			1
#define TA_BAUD_19200				2                          
#define TA_BAUD_38400				3                          
#define TA_BAUD_57600				4                          
#define TA_BAUD_115200				5  

//cmd buf

//#define CMD_GET_VERSION				0x26
//#define CMD_PROG_UPGRADE		    0x57
//#define CMD_AUTHORIZE			    0x98
#define CMD_REGISTER_USER					(0x01)
#define CMD_REGISTER_TWO					(0X02)
#define CMD_REGISTER_THREE					(0X03)

#define CMD_DELETE_SPECIFIC_USER			(0x04)
#define CMD_DELETE_ALL_USER				(0x05)
#define CMD_RESET_MODULE					(0x06)
#define CMD_AUTO_SET_LIGHT					(0X07)
#define CMD_SET_MODULE_SERIAL_NUM	   	(0x08)
#define CMD_GET_TOTAL_USER_NUM	    		(0x09)
#define CMD_GET_USER_ROLE					(0x0A)
#define CMD_VERIFY_ONE_TO_ONE				(0x0B)
#define CMD_VERIFY_ONE_TO_ALL				(0x0C)
#define CMD_SEARCH_NOT_USE_ID				(0x0D)
#define CMD_SET_BAUD						(0x21)
#define CMD_GET_FEA						(0x23)
#define CMD_GET_FINGER_IMAGE			(0x24) 
#define CMD_IDENTIFY_TEMPLATE			(0x25)
#define CMD_GET_MODULE_VERSION			(0x26)
#define CMD_READ_MODULE_SET_DATA		(0x27)
#define CMD_SET_MATCH_LEVEL				(0x28)
#define CMD_READ_LIGHT_LEVEL			(0x29)
#define CMD_READ_SERIAL_NUM				(0x2A)
#define CMD_READ_ALL_USER_INFO			(0x2B)
#define CMD_ENTER_LOW_POWER_MODE		(0x2C)  
#define CMD_SET_FINGER_ADD_MODE			(0x2D)



#define CMD_CHECK_FINGER_EXIST			(0x30)
#define CMD_READ_USER_FEATURE		    (0x31)
#define CMD_W_FEA_TO_MODULE_STORE		(0x41)
#define CMD_W_FEA_TO_M_VERIFY_1_TO_1	(0x42)
#define CMD_W_FEA_TO_M_VERIFY_1_TO_N	(0x43)
#define CMD_W_FEA_TO_M_VERIFY_TO_IMG	(0x44)
#define CMD_VERIFY_IMG_AND_W_FEA_N		(0x45)
#define CMD_SET_MODULE_CONFIG_DATA		(0x46)   

#define CMD_DELETE_SPECIFIC_ROLE        0x54
#define CMD_GET_SPECIFIC_ROLE_NUM       0x55
#define CMD_BROADCAST_VOICE             0x56

#define CMD_PROG_UPGRADE				(0x57)
#define CMD_GET_DEV_ID					(0x58)
#define CMD_GET_UNIQUEID                (0x60)

#define CMD_Calibration					(0x80)
#define CMD_LOCK						(0x81)
#define CMD_UNLOCK						(0x82)
#define CMD_GET_Authorize				(0x87)
#define CMD_FEED_DOG					(0xC2)
#define CMD_Get_Video					(0xf0)
#define CMD_GET_IMG					    (0xf1)
#define CMD_QUIT_GET_IMG				(0xf2)


#define CMD_UNLOCK_FLASH_CODE			0xee
#define CMD_UNLOCK_JLINK        			0xef

/////////////////////////////////////////
#define TA_ALL_USER     			0x00
#define TA_GUEST_USER   			0x01
#define TA_NORMAL_USER 			    0x02
#define TA_MASTER_USER				0x03
#define TA_FULL         			0x04				
#define TA_NO_USER					0x05
#define TA_USER_EXIST 				0x07
#define TA_TIME_OUT					0x08
#define TA_FLASH_ERROR				0x09
#define TA_BREAK					0x18


//中断
#define BSP_INT_SRC_NBR                                     38

#define INTID_MEM_FAULT										0
#define INTID_BUS_FAULT										1
#define INTID_USAGE_FAULT									2
#define INTID_SYS_SVC										3
#define INTID_SYS_DEBUGMON									4
#define INTID_SYS_PEND_SV									5
#define INTID_SYSTICK										6

#define BSP_INT_ID_WWDG                                     7    
#define BSP_INT_ID_PVD                                      8 
#define BSP_INT_ID_RTC                                      9    
#define BSP_INT_ID_FLASH                                    10    
#define BSP_INT_ID_RCC                                      11
#define BSP_INT_ID_EXTI0_1                                  12 
#define BSP_INT_ID_EXTI2_3                                  13
#define BSP_INT_ID_EXTI4_15                                 14  
#define BSP_INT_ID_TS                                       15   
#define BSP_INT_ID_DMA1_CH1                                 16  
#define BSP_INT_ID_DMA1_CH2_3                               17   
#define BSP_INT_ID_DMA1_CH4_5                               18   
#define BSP_INT_ID_ADC1_COMP                                19 
#define BSP_INT_ID_TIM1_BRK_UP_CTRG_COM                     20
#define BSP_INT_ID_TIM1_CC                                  21
#define BSP_INT_ID_TIM2                                     22
#define BSP_INT_ID_TIM3                                     23
#define BSP_INT_ID_TIM6_DAC                                 24
#define BSP_INT_ID_RESERVED1                                25
#define BSP_INT_ID_TIM14                                    26
#define BSP_INT_ID_TIM15                                    27
#define BSP_INT_ID_TIM16                                    28
#define BSP_INT_ID_TIM17                                    29
#define BSP_INT_ID_I2C1                                     30  
#define BSP_INT_ID_I2C2                                     31
#define BSP_INT_ID_SPI1                                     32
#define BSP_INT_ID_SPI2                                     33
#define BSP_INT_ID_USART1                                   34
#define BSP_INT_ID_USART2                                   35
#define BSP_INT_ID_RESERVED2                                36
#define BSP_INT_ID_CEC                                      37
#define BSP_INT_ID_RESERVED3                                38



#endif





