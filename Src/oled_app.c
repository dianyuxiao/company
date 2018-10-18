#include "ht32.h"
#include "oled.h"
#include "system_config.h"

void Show_SetDatetime(void)
{
	OLED_Clear();	//清屏
	OLED_ShowString(1, 0, 	"Set Datetime", 16);
	OLED_ShowString(24, 3, 	"_   /  /", 16);     //年月日
	OLED_ShowString(40, 5, 	":  :", 16);      //时分秒
} 

void Show_TimeSetOK()
{

	OLED_Clear();	//清屏
	OLED_ShowString(0, 2, 	"  Set Success!", 16);

}

void Show_InputInvalid(void)		//错误 输入无效
{
    OLED_Clear();//清屏
	OLED_ShowString(0,1,"   Error!",16);
	OLED_ShowString(0,3,"Invalid input.",16);

}


void Show_Warning(void)		//
{
	OLED_Clear();//清屏
	OLED_ShowString(0, 1, 	"    Illegal", 16);
	OLED_ShowString(0, 3, 	"   operation!", 16);
}

void Show_DoorOpen(void)
{
//	OLED_Clear();
//	OLED_ShowCHinese(50,0,11,3);//门已开
    OLED_Clear();//清屏
    OLED_ShowString(0, 1, 	"Door is Open", 16);

}
void Show_FuncDisable(FuncDisableType UnlockWay)
{
	
	OLED_Clear();
	OLED_ShowString(0,0,"Warning!",16);
	OLED_ShowString(0,3,"Retry after    s",16);
	if(UnlockWay.PwdErrNum >= MaxErrNum)
	{
        OLED_ShowNum(96, 3, (UnlockWay.DisabEndTime - (HT_RTC->CNT)), 3, 16);
	}
    if(UnlockWay.IcardErrNum >= MaxErrNum)
	{
        OLED_ShowNum(96, 3, (UnlockWay.DisabEndTime - (HT_RTC->CNT)), 3, 16);
	}

//	ms_delay(3000);

}
void Show_CardExist(void)
{
	OLED_Clear();
	OLED_ShowString(0, 0,"Error!", 16);
	OLED_ShowString(0, 3, "Card Exist", 16);//密码已经存在
}

void Show_PasswordExist()
{

	OLED_Clear();
	OLED_ShowString(0, 0,"Error!", 16);
	OLED_ShowString(0, 3, "Password Exist", 16);//密码已经存在


}

void Show_AdminSet(void)
{

	OLED_Clear();
	OLED_ShowString(0,0,"1.Add Password",16);
	OLED_ShowString(0,2,"2.Add CardNumber",16);
	OLED_ShowString(0,4,"3.Del Password",16);
	OLED_ShowString(0,6,"4.Del CardNumber",16);

}

void Show_PasswordError(void)
{

	OLED_Clear();
	OLED_ShowString(0,1,"overlength",16);
	OLED_ShowString(0,3,"Reenter password",16);

}





