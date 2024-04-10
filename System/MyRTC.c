#include "stm32f10x.h"                  // Device header
#include <time.h>
#include "MyRTC.h"

MyRTC_TimeStructTypeDef MyRTC_TimeStruct = {2023, 7, 25, 16, 44, 57};
void MyRTC_Init(void)
{
	//����PWR��BKPʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	//д��һ������Ϊ���õ�Դ�Ƿ�����־λ
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x000F) 
	{
		//����LSE����
		RCC_LSEConfig(RCC_LSE_ON);
		while(!RCC_GetFlagStatus(RCC_FLAG_LSERDY));

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);

		//��ֹʱ�Ӳ�ͬ��
		RTC_WaitForSynchro(); //�ȴ�ͬ��
		RTC_WaitForLastTask(); //�ȴ���һ��д��������

		//ÿ�ζ�RTCд���Ҫ�ȴ�д�����
		RTC_SetPrescaler(32768-1);
		RTC_WaitForLastTask(); //�ȴ���һ�β������

		MyRTC_SetTime();
		BKP_WriteBackupRegister(BKP_DR1,0x000F);
	}
	else
	{
		//��ֹʱ�Ӳ�ͬ��
		RTC_WaitForSynchro(); //�ȴ�ͬ��
		RTC_WaitForLastTask(); //�ȴ���һ��д��������
	}

}

void MyRTC_SetTime(void)
{
	time_t Time_Cnt;
	struct tm Time_Data;
	Time_Data.tm_year = MyRTC_TimeStruct.Year-1900;
	Time_Data.tm_mon = MyRTC_TimeStruct.Month-1;
	Time_Data.tm_mday = MyRTC_TimeStruct.Day;
	Time_Data.tm_hour = MyRTC_TimeStruct.Hour;
	Time_Data.tm_min = MyRTC_TimeStruct.Minute;
	Time_Data.tm_sec = MyRTC_TimeStruct.Second;

	Time_Cnt = mktime(&Time_Data)-8*60*60;
	RTC_SetCounter(Time_Cnt);
	RTC_WaitForLastTask();
}

void MyRTC_ReadTime(void)
{
	time_t Time_Cnt;
	struct tm Time_Data;
	Time_Cnt = RTC_GetCounter()+8*60*60;
	Time_Data = *localtime(&Time_Cnt);
	MyRTC_TimeStruct.Year = Time_Data.tm_year + 1900;
	MyRTC_TimeStruct.Month = Time_Data.tm_mon + 1;
	MyRTC_TimeStruct.Day = Time_Data.tm_mday;
	MyRTC_TimeStruct.Hour = Time_Data.tm_hour;
	MyRTC_TimeStruct.Minute = Time_Data.tm_min;
	MyRTC_TimeStruct.Second = Time_Data.tm_sec;
}
