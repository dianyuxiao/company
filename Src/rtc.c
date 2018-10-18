#include "ht32.h"
#include "ht32_board.h"
#include "RTC.h"
#include "time.h"
#include "oled.h"
#include "delay.h"
#include "string.h"
#include "oled_app.h"
#include "time.h"

/*********************************************************************************************************//**
 * @brief Configures the RTC.
 * @retval None
 * @details RTC configuration as following:

 ************************************************************************************************************/
void RTC_Configuration(void)
{                                                                               	
	/* Reset Backup Domain     */
//	printf("PWRCU_DeInit\n");
    if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
    {
      while (1);
    }

    /* Enable NVIC RTC interrupt                                                                              */
 //   NVIC_EnableIRQ(RTC_IRQn);
    
	PWRCU_DeInit();
	
	/* Configure Low Speed External clock (LSI)                                                               */
//	RTC_LSESMConfig(RTC_LSESM_NORMAL);//RTC_LSESM_NORMAL  RTC_LSESM_FAST
//	RTC_LSECmd(ENABLE);
	while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSIRDY) == RESET);
	
	/* Configure the RTC                                                                                      */
	RTC_ClockSourceConfig(RTC_SRC_LSI);
//	RTC_IntConfig(RTC_INT_CSEC, ENABLE);  
	RTC_SetPrescaler(RTC_RPRE_32768);

	RTC_Cmd(ENABLE);
	
}
void Time_Init(void)
{
    calendar Time;
    Time.year = 2018;
    Time.mon = 10;
    Time.day = 1;
    Time.hour = 10;
    Time.min = 0;
    Time.sec = 0;
	RTC_Set(Time);
}
void Show_Time(u8 x, u8 y,u32 Addr)
{
	calendar ShowTime;
	
	ShowTime = RTC_Get(Addr);
	OLED_ShowNum  (x, 		y,  ShowTime.year, 4,12);
	OLED_ShowChar(x + 24, 	y, '/',12);
	OLED_ShowNum  (x + 30, 	y,  ShowTime.mon, 2,12);
	OLED_ShowChar(x + 42, 	y, '/',12);
	OLED_ShowNum  (x + 48, 	y, ShowTime.day, 2,12);

	OLED_ShowNum  (x + 80, 	y ,  ShowTime.hour, 2,12);
	OLED_ShowChar(x + 92, y , ':',12);
	OLED_ShowNum  (x + 98, y , ShowTime.min, 2,12);
	OLED_ShowChar(x + 110, y , ':',12);
	OLED_ShowNum  (x + 116, y , ShowTime.sec, 2,12);
	
}

extern vu8 touch_on,get_key;
extern vu32 GoSleep_1ms;
extern vu8 IsEnterLoop;
extern vu8 NeedSleep;

u32 Set_DateTime(void)
{
	u8 test = 0;
	u8 input[14] = {0};
	calendar Time = {0};
	u8 ShowX[14] = {24,32,40,48,  64,72,  88,96, 24,32,  48,56, 72,80 };		//显示坐标 X轴
	u8 InputNum = 0;		//当前输入的个数
	while(1)		
	{	
		IsEnterLoop = 1;
		//GoSleep_1ms = 8000;
	    Show_SetDatetime();
		while(1)
		{
			if(touch_on == 1)
			{
				touch_on = 0;
				if(get_key <= '9' && InputNum < 14)		//输入的是 0 ~ 9  并且 没有输入完成
				{	
					OLED_ShowChar(ShowX[InputNum % 14], 3 + (InputNum / 8) * 2, get_key,16);
					if(InputNum < 13)
						OLED_ShowChar(ShowX[(InputNum + 1) % 14], 3 + ((InputNum + 1) / 8) * 2, '_',16);
					
					input[InputNum] = get_key - '0';
					InputNum++;
				}
				if(get_key == '*')	//按的是返回按键
				{
					if(InputNum > 0)		//有输入数字，清除重新输入
					{	
						InputNum = 0;
						break;
					}
					else if(InputNum == 0)		//没有输入数字，退出
					{
						return 0;
					}
				}
				else if(get_key == '#')	//按的是 确定按键
				{
					Time.year = input[0] * 1000 + input[1] * 100 + input[2] * 10 + input[3];
					Time.mon = input[4] * 10 + input[5];
					Time.day = input[6] * 10 + input[7];
					Time.hour = input[8] * 10 + input[9];
					Time.min =  input[10] * 10 + input[11];
					Time.sec = input[12] * 10 + input[12];
				    if(Time.year<2018||Time.year>2100)
						goto TimeInvalid;
					if(Time.mon> 12 || Time.mon== 0)
						goto TimeInvalid;
					if(Time.day> 31 || Time.day== 0)
						goto TimeInvalid;
					if(Time.hour> 24)	
						goto TimeInvalid;
					if(Time.min> 60)
						goto TimeInvalid;
					if(Time.sec>60)
						goto TimeInvalid;
					//******时间有效***保存时间***************************
					RTC_Set(Time);
					Show_TimeSetOK();		//时间 设置成功	
					RTC_Cmd(DISABLE);
					RTC_Configuration();
					RTC_Cmd(ENABLE);		
					ms_delay(3000);
					return SUCCESS;
					//******时间保存完毕********************************
					
				TimeInvalid:
					{
						Show_InputInvalid();		//时间 输入无效
						ms_delay(3000);
						InputNum = 0;
						break;
					}
				}
				
			}
			if(IsEnterLoop == 0)
			{
				test = 1;
				__nop();
				break;
			}
		}
		if(test == 1)
		{
			__nop();
			return 0;
		}
	}
}
//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//设置时钟
//把输入的时钟转换为秒钟
//以2018年1月1日为基准
//2018~2100年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
void RTC_Set(calendar save_time)
{
	u16 t;
	u32 seccount=0;
	for(t=2018;t<save_time.year;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	save_time.mon-=1;
	for(t=0;t<save_time.mon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(save_time.year)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(save_time.day-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)save_time.hour*3600;//小时秒钟数
    seccount+=(u32)save_time.min*60;	 //分钟秒钟数
	seccount+=save_time.sec;//最后的秒钟加上去
	FLASH_ErasePage(RTC_DATA_ADDR);
	FLASH_ProgramWordData(RTC_DATA_ADDR,seccount); 
	
}
//得到当前的时间
//返回值:0,成功;其他:错误代码.
	u32 timecount=0;
calendar RTC_Get(u32 Addr)
{
	calendar refresh;
	volatile u16 daycnt=0;
 
	u32 temp=0;
	u16 temp1=0;
	memcpy(&timecount,(u8 *)Addr,4); 	
    timecount=timecount + RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=2018;	//从2018年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		refresh.year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(refresh.year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		refresh.mon=temp1+1;	//得到月份
		refresh.day=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	refresh.hour=temp/3600;     	//小时
	refresh.min=(temp%3600)/60; 	//分钟	
	refresh.sec=(temp%3600)%60; 	//秒钟
	return refresh;
}




