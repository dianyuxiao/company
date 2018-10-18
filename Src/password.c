#include "ht32.h"
#include "password.h"
#include "touchkey.h"
#include "ht32_board_config.h"
#include "oled.h"
#include "buzzer.h"
#include "string.h"
#include "delay.h"
#include "oled_app.h"
extern  vu8 IsEnterLoop;
extern  vu8 KeyValue;
extern  vu8 get_key;
extern vu8 touch_on;
KeyType key;
KeyType Show_Password(u8 X, u8 Y,u8 KeyValue, u8 LimtLen, u8 Plaintext)    
{

	key.LastKey = 0xff;
	if(KeyValue == '#')        //�� # Ϊȷ�ϼ�
	{
		key.LastKey = KeyValue;          
	}
	else if(KeyValue == '*')   //�� * Ϊ�˸�
	{
		if(key.len == 0)
		{
			key.LastKey = KeyValue;

		}
		else
		{   
			OLED_ShowChar(key.len * 8, Y, ' ',16);
			key.len--;
			key.Num[key.len] = 0;
			OLED_ShowChar(key.len * 8, Y, '_',16);                                     
		}
	}
	else if(key.len < LimtLen)      //����δ�������Ƴ��ȣ����¼����ֵ
	{
		if(key.len == 0)
		{
			OLED_Clear();
			OLED_ShowString(24,0, "Password: " ,16);
		}
		
		key.Num[key.len] =  KeyValue;
	 
		if(Plaintext)       //����
			OLED_ShowChar(key.len * 8, Y, KeyValue,16);
		else                //����
			OLED_ShowChar(key.len * 8, Y, '*',16);	
		OLED_ShowChar(key.len * 8 + 8, Y, '_',16);
		key.len++;
	}
	else
	{
		Show_PasswordError();
		key.len = 0;		
	} 
	return key;
	
}

u8 VirtualPassword(KeyType InputPwd, u32 StrAddr)    //����Ƚ�
{
	u8 comparekey[16];
	u8 keylen = 0;
	u16 i,j;	//С��Χ��ʱ����
	if(InputPwd.len <6)
	{
		return 0;
	}
	for(i = 1; i <= 20;i++)
	{
		memset(comparekey,0,sizeof(comparekey));
		keylen = 0;
		memcpy(comparekey,(u8 *)StrAddr,16);
		for(j = 0;j <= 15;j++)
		{
			if(comparekey[j] != '#' )
				keylen ++;
			else
				break;
		}

		for(j=0;j <= (InputPwd.len - keylen );j++)
		{
			if(memcmp(comparekey,InputPwd.Num + j,keylen) == 0)
				return 1;

		}
		
		StrAddr += 16;
	}
	return 0;
}
u8 CardNumberIsExists(u8* InputCard,u32 StrAddr,u16 CompNum)
{
	u8 CardNum[8];
	u16 i;	//С��Χ��ʱ����
	for(i = 1; i <= CompNum;i++)
	{
		memcpy(CardNum,(u8 *)StrAddr,sizeof(CardNum));
		if(memcmp(CardNum,InputCard,sizeof(CardNum)) == 0)
			return i;
		StrAddr += 16;	
	}
	return 0;
}
u8 PwdKeyIsExists(KeyType InputPwdKey, u32 StrAddr, u16 CompNum)	//(�Աȵ���ʼ��ַ���Ա�����/KEY����)
{
	u8 comparekey[16];
	u16 i;	//С��Χ��ʱ����
	if(InputPwdKey.len <6)
	{
		return 0;
	}
	for(i = 1; i <= CompNum;i++)
	{
		memcpy(comparekey,(u8 *)StrAddr,InputPwdKey.len + 1);
		if(comparekey[InputPwdKey.len] == '#')
			if(memcmp(comparekey,InputPwdKey.Num,InputPwdKey.len) == 0)
				return i;
		StrAddr += 16;
	}
	return 0;
}

u16 NewCardNumber_Record(u32 StrAddr, u8* NewCardNum,u16 ID)
{
    u8 res;
    u8 i=0,j;
	u32 data;
	StrAddr = StrAddr + ID * 16;
 	//ָ��λ��д��������
    for(j=0;j<4;j++)
	{
		data = ((u32) NewCardNum[i+3] )<< 24 | ((u32)NewCardNum[i+2] << 16) | ((u32)NewCardNum[i+1] << 8) | ((u32)NewCardNum[i]);
		res= FLASH_ProgramWordData(StrAddr, data);
		i = i + 4;
		StrAddr+=4;
	}
	return res;
      
}
u16 NewPwdKey_Record(u32 StrAddr, KeyType NewPwd, u16 ID)	//�����뱣���λ�� 1~20	
{
	u16 res = 0;		//����ֵ
	u8 i=0,j;
    u32 data;
	StrAddr = StrAddr + ID * 16;
	NewPwd.Num[NewPwd.len] = '#' ;
	//ָ��λ��д��������
    for(j=0;j<4;j++)
	{
		data = ((u32) NewPwd.Num[i+3] )<< 24 | ((u32)NewPwd.Num[i+2] << 16) | ((u32)NewPwd.Num[i+1] << 8) | ((u32)NewPwd.Num[i]);
		res= FLASH_ProgramWordData(StrAddr, data);
		i = i+4;
		StrAddr+=4;

	}
	return res;
}
 
KeyType KeyEnterPwdLoop(u8 X, u8 Y, u8 LimtLen, u8 Plaintext)    //����һ����ѭ�����˳�����������*��#
{
    KeyType key = {0};
    key.LastKey = 0xff;
	IsEnterLoop = 1;
    while(1)
    {
        if(touch_on == 1)
        {
            touch_on = 0;
			
            if(get_key== '#')        //�� # Ϊȷ�ϼ�
            {
                key.LastKey = get_key;
                break;               
            }
            else if(get_key == '*')   //�� * Ϊ�˸��˵�0���ٰ����˳�
            {
                if(key.len == 0)
                {
                    key.LastKey = get_key;
                    break;                   
                }
                else
                {   
                    OLED_ShowChar(key.len * 8, Y, ' ',16);
                    key.len--;
					key.Num[key.len] = 0;
                    OLED_ShowChar(key.len * 8, Y, '_',16);                                     
                }
            }
            else if(key.len < LimtLen)      //����δ�������Ƴ��ȣ����¼����ֵ
            {
//				if(key.len == 0)
//				{
//					OLED_Clear();
//					OLED_ShowString(0,0,"Add Password:",16);
//				}
				if( get_key != 0x00)
				{
					key.Num[key.len] =  get_key;
					if(Plaintext)       //����
						OLED_ShowChar(key.len * 8, Y, get_key,16);
					else                //����
						OLED_ShowChar(key.len * 8, Y, '*',16);
						
					OLED_ShowChar(key.len * 8 + 8, Y, '_',16);
					key.len++;
				}
            }
			else
			{
				OLED_Clear();
				OLED_ShowString(0,0,"overlength",16);
				OLED_ShowString(0,Y,"Reenter password",16);
				key.len = 0;
				ms_delay(2000);
				break;
			}
        } 
		if(IsEnterLoop == 0)		//�г�����Ҫ�˳���������ѭ��
		{	
			key.LastKey = 0xff;
			key.len = 0;
			return key;
		}
    } 
 //   OLED_Clear_Oneline(Y);    
    return key;
}





