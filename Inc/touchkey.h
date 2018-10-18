#ifndef __TOUCHKEY_H
#define __TOUCHKEY_H

#include "ht32.h"


#define MAX_KEY_NUM             13                  //  ��������
#define LONG_KEY_MASK           0x80


//�û��ɸ��ģ����ﰴ�հ���ʵ�ʵľ�������
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



//*****�����������ձ���*******************
typedef struct {		
	u8  PwdErrNum;			// ���������������
	u8  IcardErrNum;      //ˢ�������������
	u32	DisabEndTime;	//ʧ�ܽ���ʱ��
} FuncDisableType;

typedef  struct 
{
	u8 LastKey;			//�˳�����ѭ��֮ǰ�����һ������ֵ��Ϊ * �������û�����ˣ�Ϊ#�������û���ȷ��
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


//�û��ɸ��ģ�������,���ֵΪ0x3F   
//д������ʱ�������0xB0��ʼ�����ֱ�д��14/18�������ֽڣ�����ֽ���У���
//����DATA�������ȡ�Ͱ�λΪCheckSum������������(�����һ���ֽ�)���
//���һ���ֽ�ΪCheckSum
static uint8_t sensitivity_arrays[22]=
{
    0x00,
    0x00,
    0x83,
    0xF3,
    0x98,
    
    0x12,                               //�û��ɸ��ģ������� 
    0x12,                               //�û��ɸ��ģ������� 
    0x12,                               //�û��ɸ��ģ������� 
    0x12,                               //�û��ɸ��ģ������� 
    0x12,                               //�û��ɸ��ģ�������                                    
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
	0x0C,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x12,                               //�û��ɸ��ģ�������
    0x28                                //�û��ɸ��ģ�������
};

void KEY_GPIO_Init(void);
void KEY_IRQ_Init(void);
int16_t touchkey_scan(void);
int16_t key_config_init(void);
int16_t fun_BS83B16A_ReadData(uint8_t Register_Address, uint8_t *buff, uint8_t count);
int16_t fun_BS83B16A_WriteData(uint8_t Register_Address, uint8_t *buff, uint8_t count);
#endif


