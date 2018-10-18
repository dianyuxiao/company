#include "ht32.h"
#include "admin.h"
#include "touchkey.h"
#include "delay.h"
#include "oled.h"
#include "system_config.h"
#include "password.h"
#include "card.h"
#include "string.h"
#include "oled_app.h"
extern vu8 get_key;
extern vu8 touch_on;
vu8 IsEnterLoop = 1;	
//��ָ����ַ��ʼ��������ҿյ����뱣��λ�ã������������ҵ����Ǹ�
//����������FindNum������λ
//���� 0��û�ҵ�
//���� 1�����ڿ�������
//�������ΪFindNum
/*
StrAddrȡֵ����2��
1.������ʼ��ַ    �P
2.������ʼ��ַ	  

*/
void DelCardNumber(void)
{
    u8 i;
	u32 Addr;
    FLASH_State FLASHState;
    Addr = Flash_CardIdAddress;
    for(i = 0;i < 4; i++)
    {
        FLASHState = FLASH_ErasePage(Addr);
        Addr += FLASH_PAGE_SIZE;
    }
    if (FLASHState != FLASH_COMPLETE)
    {
	    OLED_Clear();
		OLED_ShowString(0,0,"Del CardNumber ERR",16);
		ms_delay(3000);
    }
	else
	{
		OLED_Clear();
		OLED_ShowString(0,0,"Del CarNumber OK",16);
		ms_delay(3000);
	}

}
void DelPassword(void)
{
    u8 i;
	u32 Addr;
    FLASH_State FLASHState;
    Addr = Flash_PasswordAddress;
    for(i = 0;i < 4; i++)
    {
        FLASHState = FLASH_ErasePage(Addr);
        Addr += FLASH_PAGE_SIZE;
    }
    
    if (FLASHState != FLASH_COMPLETE)
    {
	    OLED_Clear();
		OLED_ShowString(0,0,"Del Password ERR",16);
		get_key = 0x00;
		ms_delay(3000);
    }
	else
	{
		OLED_Clear();
		OLED_ShowString(0,0,"Del Password OK",16);
		get_key = 0x00;
		ms_delay(3000);
		
	}

}

u8 AdminPwdKey_Num(Select_Area  Select)	//��ȡ ����Ա���� / ����  ����
{
	u8 PwdNum = 0;
	vu32 StrAddr = 0;
	if(Select == PwdIsSelect)
		StrAddr = Flash_PasswordAddress;
	else if(Select == CardIsSelect)	
		StrAddr = Flash_CardIdAddress;
	else
		return 0xFF;
	for(PwdNum = 0; PwdNum <20; )
	{   
	    if(rw(StrAddr) == 0xFFFFFFFF)
	        return PwdNum;
		else
			PwdNum ++;
		
		StrAddr += 16;
	}
	return 21;
}
void AddCardNumber(void)
{
    extern vu8 NeedSleep;
    extern vu32 GoSleep_1ms;
	u8 Frist_CardNumber[8];
	u8 Second_CardNumber[8];
	u8 Third_CardNumber[8];
	u8 res;
	u16 AdminPwdTotal; 
	memset(Frist_CardNumber,0,sizeof(Frist_CardNumber));
	memset(Second_CardNumber,0,sizeof(Second_CardNumber));
	memset(Third_CardNumber,0,sizeof(Third_CardNumber));
    AdminPwdTotal = AdminPwdKey_Num(CardIsSelect);
	if(AdminPwdTotal == 21)
	{	
		OLED_Clear();
		OLED_ShowString(1,1,"CardNumber Full",16); //��������
		ms_delay(3000);
		return;
	}
	while(1)
	{
		OLED_Clear();
	    OLED_ShowString(25,3,"Put ICard",16);		//��ʾˢ���Ľ���
		OLED_ShowNum(86, 6 , AdminPwdTotal+1,2,16);
		OLED_ShowString(102, 6, "/20", 16);	
		ms_delay(1000);		
		res=RC522_GetCard(Frist_CardNumber);
	    if(res == SUCCESS)
	    {
			ms_delay(50);
			res=RC522_GetCard(Second_CardNumber);
			if(res == SUCCESS)
			{
				ms_delay(50);
				res=RC522_GetCard(Third_CardNumber);
				if(res == SUCCESS)
				{
					if(memcmp(Frist_CardNumber,Second_CardNumber,sizeof(Second_CardNumber)) == 0)
					{
				        if(memcmp(Third_CardNumber,Second_CardNumber,sizeof(Second_CardNumber)) == 0)
				        {
							if(CardNumberIsExists(Frist_CardNumber,Flash_CardIdAddress,20)!=0) //�������
							{
								Show_CardExist();
								ms_delay(1000);
								break;

							}
							else if(NewCardNumber_Record(Flash_CardIdAddress, Frist_CardNumber, AdminPwdTotal) == 0)		//��ʽ�������룬������AdminNumλ�� 1~5
							{
								OLED_Clear();
								OLED_ShowString(0, 2, "Add Card OK!", 16);//¼��ɹ�
								ms_delay(1000);
								break;
							}
						}
						else
				        {
							Show_InputInvalid();
				        }
					}
				}
			}
	    }

		if(NeedSleep == 1)
		{
		    get_key = 0;
		    GoSleep_1ms= 2000;
		    break;
		}
	}
}

void AddPassword(void)
{
    u16 AdminPwdTotal;
	extern vu32 GoSleep_1ms;
	KeyType PwdFrist = {0};			//��һ�� �������������
	KeyType PwdSecond = {0};		//�ڶ��� �������������	
	//AdminNum ������

	while(1)		//��Ҫ�жϰ�����ʱ��Ҫ�ص���ҳ��
	{
		AdminPwdTotal = AdminPwdKey_Num(PwdIsSelect);	//��ȡ��������
		if(AdminPwdTotal == 21)
		{	
			OLED_Clear();
			OLED_ShowString(1,1," Password  Full",16); //��������
			ms_delay(3000);
			return;
		}
		OLED_Clear();
		OLED_ShowString(0,0,"Add Password:",16);		//��ʾ��������Ľ���
		OLED_ShowNum(86, 6 , AdminPwdTotal+1,2,16);
		OLED_ShowString(102, 6, "/20", 16);				
		OLED_ShowChar(0, 4, '_',16); 
		PwdFrist = KeyEnterPwdLoop(1, 4, 14, 1);	//(X,Y,�޶�����,����)
		if(PwdFrist.LastKey == '*')	//��ȡ����
		{
			get_key = 0x00;
			break;
		}
		if(PwdFrist.LastKey == '#')	//��ȷ����
		{
			if(PwdFrist.len >= 6)	//��������ϸ񣬽��뱣������
			{
				OLED_ShowString(0, 0, 	"  Enter again   ", 16);
				OLED_Clear_OneLine(4);
				PwdSecond = KeyEnterPwdLoop(1, 4, 14, 1);	//�ڶ����������� (X,Y,�޶�����,����)
				if(PwdSecond.LastKey == '*')	//��ȡ����
				{
					get_key = 0x00;
					break;
				}
				if(memcmp(PwdFrist.Num,PwdSecond.Num,sizeof(PwdFrist.Num)) != 0)	//�Ƚ��������벻���
				{
					Show_InputInvalid();
                    ms_delay(2000);
					//break;
				}
				else if(PwdKeyIsExists(PwdFrist, Flash_PasswordAddress, 20) != 0)	//(�Աȵ���ʼ��ַ���Ա��������))	//�������Ѵ��ڣ��˳�
				{
					Show_PasswordExist();
					ms_delay(2000);
				}
				else if(NewPwdKey_Record(Flash_PasswordAddress, PwdFrist, AdminPwdTotal) == 0)		//��ʽ�������룬������AdminNumλ�� 1~5
				{
					OLED_Clear();
					OLED_ShowString(0, 2, "Add Password OK!", 16);//¼��ɹ�
					ms_delay(2000);
				}
				//break;
			}
			else				//����̫��
			{
				OLED_Clear();	
				OLED_ShowString(0,0, "Password   Short", 16);
				ms_delay(2000);
			}
			//break;			
		}
		GoSleep_1ms = 5000;
//		break;
	}

}

void EnterAdminSet(void)
{
	extern vu8 NeedSleep;
	u8 temp = 5;
	while(temp)
	{
		Show_AdminSet();
		ms_delay(1000);
		switch(get_key)
		{
			case '1':
				touch_on = 0;
				AddPassword();
				break;
			case '2':		
				AddCardNumber();
				break;
			case '3':
				DelPassword();
				break;
			case '4':
				DelCardNumber();
				break;
			default:
				break;
			
		}
		get_key = 0;
		ms_delay(1000);
		temp--;
	}

}
