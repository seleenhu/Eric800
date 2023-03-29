/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	�޸ļ�¼ :
*		�汾��   ����        ����       ˵��
*		V1.0    2016-01-20  Eric2013   ��ʽ����
*
*	Copyright (C), 2016-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"



#if 0
	#define printf_rtcdbg  printf
#else
	#define printf_rtcdbg(...)
#endif





RTC_TimeTypeDef  RTC_TimeStructure = {
	.RTC_H12     = RTC_H12_AM,
	.RTC_Hours   = 9,
	.RTC_Minutes = 42,
	.RTC_Seconds = 30,
};
RTC_DateTypeDef  RTC_DateStructure = {
	.RTC_Year = 17,
	.RTC_Month = 11,
	.RTC_Date = 13,
	.RTC_WeekDay = RTC_Weekday_Monday,
};

/*
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_DateTypeDef  RTC_DateStructure;
*/


/* 
*********************************************************************************************************
*  �� �� ��: bsp_InitRTC 
*  ����˵��: ��ʼ��RTC 
*  �� �Σ���  *  �� �� ֵ: ��   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{ 
		
	
	
	/* ���ڼ���Ƿ��Ѿ����ù�RTC��������ù��Ļ����������ý���ʱ 
	����RTC���ݼĴ���Ϊ0x32F2��������RTC���ݼĴ�������0x32F2   ��ô��ʾû�����ù�����Ҫ����RTC.   */ 
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2) 
	{ 
		RTC_Config(); 
		/* ��ӡ������Ϣ */ 
		printf_rtcdbg("��һ��ʹ��RTC \n\r"); 
		
		/* ����ϵ縴λ��־�Ƿ����� */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* �����ϵ縴λ */ 
			printf_rtcdbg("�����ϵ縴λ \n\r"); 
		} 
	} 
	else 
	{ 
		/* ��ӡ������Ϣ */ 
		printf_rtcdbg("��n��ʹ��RTC \n\r"); 
		
		/* ����ϵ縴λ��־�Ƿ����� */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* �����ϵ縴λ */ 
			printf_rtcdbg("�����ϵ縴λ \n\r"); 
		} 
		/* ������Ÿ�λ��־�Ƿ����� */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{ 
			/* �������Ÿ�λ */ 
			printf_rtcdbg("�������Ÿ�λ \n\r");     
		} 
		
		/* ʹ��PWRʱ�� */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* �������RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* �ȴ� RTC APB �Ĵ���ͬ�� */ 
		RTC_WaitForSynchro(); 

	} 
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: 1. ѡ��ͬ��RTCʱ��ԴLSI����LSE��
*             2. ����RTCʱ�ӡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_Config(void)
{
	RTC_InitTypeDef  RTC_InitStructure;
	uint32_t uwAsynchPrediv = 0;
    uint32_t uwSynchPrediv = 0;
	
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* ʹ��LSE����  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* �ȴ����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);

	/* �ȴ�RTC APB�Ĵ���ͬ�� */
	RTC_WaitForSynchro();

	/* ����RTC���ݼĴ����ͷ�Ƶ��  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	/* ���������պ����� */
	//RTC_DateStructure.RTC_Year = 17;
	//RTC_DateStructure.RTC_Month = 11;
	//RTC_DateStructure.RTC_Date = 13;
	//RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

	/* ����ʱ���룬�Լ���ʾ��ʽ */
	//RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	//RTC_TimeStructure.RTC_Hours   = 9;
	//RTC_TimeStructure.RTC_Minutes = 42;
	//RTC_TimeStructure.RTC_Seconds = 30; 
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);   

	/* ���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CalcWeek
*	����˵��: �������ڼ������ڼ�
*	��    ��: _year _mon _day  ������  (����2�ֽ��������º������ֽ�������
*	�� �� ֵ: �ܼ� ��1-7�� 7��ʾ����
*********************************************************************************************************
*/
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day)
{
	/*
	���գ�Zeller����ʽ
		��ʷ�ϵ�ĳһ�������ڼ���δ����ĳһ�������ڼ�������������⣬�кܶ���㹫ʽ������ͨ�ü��㹫ʽ��
	һЩ�ֶμ��㹫ʽ�����������������ǲ��գ�Zeller����ʽ��
	    ��w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

		��ʽ�еķ��ź������£�
	     w�����ڣ�
	     c����ĸ�2λ��������-1
	     y���꣨��λ������
	     m���£�m���ڵ���3��С�ڵ���14�����ڲ��չ�ʽ�У�ĳ���1��2��Ҫ������һ���13��14�������㣬
	  	    ����2003��1��1��Ҫ����2002���13��1�������㣩��
	     d���գ�
	     [ ]����ȡ������ֻҪ�������֡�

	    �������W����7�������Ǽ��������ڼ������������0����Ϊ�����ա�
        �������Ǹ�������������������Ҫ���⴦��
            �������ܰ�ϰ�ߵ������ĸ�����������ֻ�ܰ������е������Ķ������ࡣΪ�˷���
        ���㣬���ǿ��Ը�������һ��7����������ʹ����Ϊһ��������Ȼ����������

		��2049��10��1�գ�100������죩Ϊ�����ò��գ�Zeller����ʽ���м��㣬�������£�
		���գ�Zeller����ʽ��w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
		=49+[49/4]+[20/4]-2��20+[26�� (10+1)/10]+1-1
		=49+[12.25]+5-40+[28.6]
		=49+12+5-40+28
		=54 (����7��5)
		��2049��10��1�գ�100������죩������5��
	*/
	uint8_t y, c, m, d;
	int16_t w;

	if (_mon >= 3)
	{
		m = _mon;
		y = _year % 100;
		c = _year / 100;
		d = _day;
	}
	else	/* ĳ���1��2��Ҫ������һ���13��14�������� */
	{
		m = _mon + 12;
		y = (_year - 1) % 100;
		c = (_year - 1) / 100;
		d = _day;
	}

	w = y + y / 4 +  c / 4 - 2 * c + ((uint16_t)26*(m+1))/10 + d - 1;
	if (w == 0)
	{
		w = 7;	/* ��ʾ���� */
	}
	else if (w < 0)	/* ���w�Ǹ����������������ʽ��ͬ */
	{
		w = 7 - (-w) % 7;
	}
	else
	{
		w = w % 7;
	}
	
	/* modified by eric2013 -- 2016-12-25 */
	if (w == 0)
	{
		w = 7;	/* ��ʾ���� */
	}
	
	return w;
}

/***************************** end *********************************/

