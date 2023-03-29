
#include "mydefine.h"

AlarmTimStruct SystemTime = {
.year=20,
.month=10,
.day=12,
.hour=9,
.minute=47,
.second=30,
.us=0,
};


uint8_t My_RTC_Init(uint8_t setflag)
{
	RTC_InitTypeDef RTC_InitStructure;
	uint32_t data32;
	u16 retry=0x01;
	RTC_TimeTypeDef settime;
	RTC_DateTypeDef setdate;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能 PWR 时钟
	PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问
	data32 = RTC_ReadBackupRegister(RTC_BKP_DR0);
	if((data32 !=0x5050)||setflag)//是否第一次配置?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		//检查指定的 RCC 标志位设置与否,等待低速晶振就绪
		{ 	
			retry++;
			
		}
		if(retry==0)return 1; //LSE 开启失败.
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //选择 LSE 作为 RTC 时钟STM32F4 开发指南(库函数版)	
		RCC_RTCCLKCmd(ENABLE); //使能 RTC 时钟
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC 异步分频系数(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;//RTC 同步分频系数(0~7FFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24 小时格式
		RTC_Init(&RTC_InitStructure);//初始化 RTC 参数

		settime.RTC_H12 = RTC_H12_AM;
		settime.RTC_Hours=SystemTime.hour;
		settime.RTC_Minutes=SystemTime.minute;
		settime.RTC_Seconds=SystemTime.second;
		setdate.RTC_Year = SystemTime.year;
		setdate.RTC_Month= SystemTime.month;
		setdate.RTC_Date= SystemTime.day;
		setdate.RTC_WeekDay = 0;
		RTC_SetTime(RTC_Format_BIN, &settime);
		RTC_SetDate(RTC_Format_BIN, &setdate);	
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);//标记已经初始化过了
	}
	return 0;
}//My_RTC_Init

void ReadRtcTime(void)
{
	RTC_TimeTypeDef settime;
	RTC_DateTypeDef setdate;

	RTC_GetTime(RTC_Format_BIN, &settime);
	RTC_GetDate(RTC_Format_BIN, &setdate);
	SystemTime.hour = settime.RTC_Hours;
	SystemTime.minute = settime.RTC_Minutes;
	SystemTime.second = settime.RTC_Seconds;
	SystemTime.year = setdate.RTC_Year;
	SystemTime.month = setdate.RTC_Month;
	SystemTime.day = setdate.RTC_Date;
	SystemTime.us = TIM_GetCounter(TIM3);
	SystemTime.us <<= 4;
}//ReadRtcTime

void TIM3_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //①使能 TIM3 时钟
	TIM_TimeBaseInitStructure.TIM_Period = 62500-1; //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= 168*16-1; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);// ②初始化定时器 TIM3

#if 0	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //③允许定时器 3 更新中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器 3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ④初始化 NVIC
#endif
	
	TIM_Cmd(TIM3,ENABLE); //⑤使能定时器 3
}




