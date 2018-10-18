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
	u8 ShowX[14] = {24,32,40,48,  64,72,  88,96, 24,32,  48,56, 72,80 };		//��ʾ���� X��
	u8 InputNum = 0;		//��ǰ����ĸ���
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
				if(get_key <= '9' && InputNum < 14)		//������� 0 ~ 9  ���� û���������
				{	
					OLED_ShowChar(ShowX[InputNum % 14], 3 + (InputNum / 8) * 2, get_key,16);
					if(InputNum < 13)
						OLED_ShowChar(ShowX[(InputNum + 1) % 14], 3 + ((InputNum + 1) / 8) * 2, '_',16);
					
					input[InputNum] = get_key - '0';
					InputNum++;
				}
				if(get_key == '*')	//�����Ƿ��ذ���
				{
					if(InputNum > 0)		//���������֣������������
					{	
						InputNum = 0;
						break;
					}
					else if(InputNum == 0)		//û���������֣��˳�
					{
						return 0;
					}
				}
				else if(get_key == '#')	//������ ȷ������
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
					//******ʱ����Ч***����ʱ��***************************
					RTC_Set(Time);
					Show_TimeSetOK();		//ʱ�� ���óɹ�	
					RTC_Cmd(DISABLE);
					RTC_Configuration();
					RTC_Cmd(ENABLE);		
					ms_delay(3000);
					return SUCCESS;
					//******ʱ�䱣�����********************************
					
				TimeInvalid:
					{
						Show_InputInvalid();		//ʱ�� ������Ч
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
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��2018��1��1��Ϊ��׼
//2018~2100��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
void RTC_Set(calendar save_time)
{
	u16 t;
	u32 seccount=0;
	for(t=2018;t<save_time.year;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	save_time.mon-=1;
	for(t=0;t<save_time.mon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(save_time.year)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(save_time.day-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)save_time.hour*3600;//Сʱ������
    seccount+=(u32)save_time.min*60;	 //����������
	seccount+=save_time.sec;//�������Ӽ���ȥ
	FLASH_ErasePage(RTC_DATA_ADDR);
	FLASH_ProgramWordData(RTC_DATA_ADDR,seccount); 
	
}
//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
	u32 timecount=0;
calendar RTC_Get(u32 Addr)
{
	calendar refresh;
	volatile u16 daycnt=0;
 
	u32 temp=0;
	u16 temp1=0;
	memcpy(&timecount,(u8 *)Addr,4); 	
    timecount=timecount + RTC_GetCounter();	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=2018;	//��2018�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		refresh.year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(refresh.year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		refresh.mon=temp1+1;	//�õ��·�
		refresh.day=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	refresh.hour=temp/3600;     	//Сʱ
	refresh.min=(temp%3600)/60; 	//����	
	refresh.sec=(temp%3600)%60; 	//����
	return refresh;
}




