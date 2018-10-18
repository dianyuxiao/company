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
//从指定地址开始，往后查找空的密码保存位置，并返回首先找到的那个
//最大往后查找FindNum个保存位
//返回 0：没找到
//返回 1：存在可用区域
//返回最大为FindNum
/*
StrAddr取值：有2种
1.密码起始地址    
2.卡号起始地址	  

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

u8 AdminPwdKey_Num(Select_Area  Select)	//读取 管理员密码 / 卡号  数量
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
		OLED_ShowString(1,1,"CardNumber Full",16); //密码已满
		ms_delay(3000);
		return;
	}
	while(1)
	{
		OLED_Clear();
	    OLED_ShowString(25,3,"Put ICard",16);		//显示刷卡的界面
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
							if(CardNumberIsExists(Frist_CardNumber,Flash_CardIdAddress,20)!=0) //密码存在
							{
								Show_CardExist();
								ms_delay(1000);
								break;

							}
							else if(NewCardNumber_Record(Flash_CardIdAddress, Frist_CardNumber, AdminPwdTotal) == 0)		//正式保存密码，保存在AdminNum位置 1~5
							{
								OLED_Clear();
								OLED_ShowString(0, 2, "Add Card OK!", 16);//录入成功
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
	KeyType PwdFrist = {0};			//第一次 按键输入的密码
	KeyType PwdSecond = {0};		//第二次 按键输入的密码	
	//AdminNum 密码编号

	while(1)		//需要判断按键超时，要回到主页面
	{
		AdminPwdTotal = AdminPwdKey_Num(PwdIsSelect);	//读取密码数量
		if(AdminPwdTotal == 21)
		{	
			OLED_Clear();
			OLED_ShowString(1,1," Password  Full",16); //密码已满
			ms_delay(3000);
			return;
		}
		OLED_Clear();
		OLED_ShowString(0,0,"Add Password:",16);		//显示输入密码的界面
		OLED_ShowNum(86, 6 , AdminPwdTotal+1,2,16);
		OLED_ShowString(102, 6, "/20", 16);				
		OLED_ShowChar(0, 4, '_',16); 
		PwdFrist = KeyEnterPwdLoop(1, 4, 14, 1);	//(X,Y,限定长度,明文)
		if(PwdFrist.LastKey == '*')	//按取消键
		{
			get_key = 0x00;
			break;
		}
		if(PwdFrist.LastKey == '#')	//按确定键
		{
			if(PwdFrist.len >= 6)	//密码输入合格，进入保存密码
			{
				OLED_ShowString(0, 0, 	"  Enter again   ", 16);
				OLED_Clear_OneLine(4);
				PwdSecond = KeyEnterPwdLoop(1, 4, 14, 1);	//第二次输入密码 (X,Y,限定长度,明文)
				if(PwdSecond.LastKey == '*')	//按取消键
				{
					get_key = 0x00;
					break;
				}
				if(memcmp(PwdFrist.Num,PwdSecond.Num,sizeof(PwdFrist.Num)) != 0)	//比较两次输入不相符
				{
					Show_InputInvalid();
                    ms_delay(2000);
					//break;
				}
				else if(PwdKeyIsExists(PwdFrist, Flash_PasswordAddress, 20) != 0)	//(对比的起始地址，对比密码个数))	//此密码已存在，退出
				{
					Show_PasswordExist();
					ms_delay(2000);
				}
				else if(NewPwdKey_Record(Flash_PasswordAddress, PwdFrist, AdminPwdTotal) == 0)		//正式保存密码，保存在AdminNum位置 1~5
				{
					OLED_Clear();
					OLED_ShowString(0, 2, "Add Password OK!", 16);//录入成功
					ms_delay(2000);
				}
				//break;
			}
			else				//密码太短
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
