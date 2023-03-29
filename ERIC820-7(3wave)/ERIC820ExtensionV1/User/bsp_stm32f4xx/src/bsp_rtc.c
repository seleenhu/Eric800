
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ�� PWR ʱ��
	PWR_BackupAccessCmd(ENABLE); //ʹ�ܺ󱸼Ĵ�������
	data32 = RTC_ReadBackupRegister(RTC_BKP_DR0);
	if((data32 !=0x5050)||setflag)//�Ƿ��һ������?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE ����
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		//���ָ���� RCC ��־λ�������,�ȴ����پ������
		{ 	
			retry++;
			
		}
		if(retry==0)return 1; //LSE ����ʧ��.
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ�� LSE ��Ϊ RTC ʱ��STM32F4 ����ָ��(�⺯����)	
		RCC_RTCCLKCmd(ENABLE); //ʹ�� RTC ʱ��
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC �첽��Ƶϵ��(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;//RTC ͬ����Ƶϵ��(0~7FFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24 Сʱ��ʽ
		RTC_Init(&RTC_InitStructure);//��ʼ�� RTC ����

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
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);//����Ѿ���ʼ������
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //��ʹ�� TIM3 ʱ��
	TIM_TimeBaseInitStructure.TIM_Period = 62500-1; //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= 168*16-1; //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);// �ڳ�ʼ����ʱ�� TIM3

#if 0	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //������ʱ�� 3 �����ж�
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ�� 3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// �ܳ�ʼ�� NVIC
#endif
	
	TIM_Cmd(TIM3,ENABLE); //��ʹ�ܶ�ʱ�� 3
}




