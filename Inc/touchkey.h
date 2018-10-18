#ifndef __TOUCHKEY_H
#define __TOUCHKEY_H

#include "ht32.h"


#define MAX_KEY_NUM             13                  //  按键数量
#define LONG_KEY_MASK           0x80


//用户可更改，这里按照按键实际的矩阵排列
/***********************************************************************************************************
key9    key8    key4
key10   key7    key3
key11   key6    key2
key12   key5    key1
***********************************************************************************************************/
static const uint8_t ucKey_index[MAX_KEY_NUM]={
'#','9','6',
'3','0','8',
'5','2','1',
'4','7','*'
};



//*****开锁功能锁闭变量*******************
typedef struct {		
	u8  PwdErrNum;			// 密码连续错误次数
	u8  IcardErrNum;      //刷卡连续错误次数
	u32	DisabEndTime;	//失能结束时间
} FuncDisableType;

typedef  struct 
{
	u8 LastKey;			//退出按键循环之前的最后一个按键值，为 * ：代表用户想后退，为#：代表用户按确定
	u8 len;
	u8 Num[16];
}KeyType;


#define WR_ADDR             0xA0
#define RD_ADDR             0xA1
#define KEY_OUT_ONE         0x08
#define KEY_OUT_TWO         0x09
#define KEY_WRITE_SET       0xB0
#define NO_KEY_DOWN         0xFF
#define KEY_DOWN            0x00


//用户可更改，灵敏度,最大值为0x3F   
//写入设置时，必须从0xB0开始连续分别写入14/18个数据字节，最后字节是校验和
//所有DATA数据相加取低八位为CheckSum，即下列数组(除最后一个字节)相加
//最后一个字节为CheckSum
static uint8_t sensitivity_arrays[22]=
{
    0x00,
    0x00,
    0x83,
    0xF3,
    0x98,
    
    0x12,                               //用户可更改，灵敏度 
    0x12,                               //用户可更改，灵敏度 
    0x12,                               //用户可更改，灵敏度 
    0x12,                               //用户可更改，灵敏度 
    0x12,                               //用户可更改，灵敏度                                    
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
	0x0C,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x12,                               //用户可更改，灵敏度
    0x28                                //用户可更改，灵敏度
};

void KEY_GPIO_Init(void);
void KEY_IRQ_Init(void);
int16_t touchkey_scan(void);
int16_t key_config_init(void);
int16_t fun_BS83B16A_ReadData(uint8_t Register_Address, uint8_t *buff, uint8_t count);
int16_t fun_BS83B16A_WriteData(uint8_t Register_Address, uint8_t *buff, uint8_t count);
#endif


