/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	修改记录 :
*		版本号   日期        作者       说明
*		V1.0    2016-01-20  Eric2013   正式发布
*
*	Copyright (C), 2016-2020, 安富莱电子 www.armfly.com
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
*  函 数 名: bsp_InitRTC 
*  功能说明: 初始化RTC 
*  形 参：无  *  返 回 值: 无   
*********************************************************************************************************
*/ 
void bsp_InitRTC(void) 
{ 
		
	
	
	/* 用于检测是否已经配置过RTC，如果配置过的话，会在配置结束时 
	设置RTC备份寄存器为0x32F2。如果检测RTC备份寄存器不是0x32F2   那么表示没有配置过，需要配置RTC.   */ 
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2) 
	{ 
		RTC_Config(); 
		/* 打印调试信息 */ 
		printf_rtcdbg("第一次使用RTC \n\r"); 
		
		/* 检测上电复位标志是否设置 */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* 发生上电复位 */ 
			printf_rtcdbg("发生上电复位 \n\r"); 
		} 
	} 
	else 
	{ 
		/* 打印调试信息 */ 
		printf_rtcdbg("第n次使用RTC \n\r"); 
		
		/* 检测上电复位标志是否设置 */ 
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) 
		{ 
			/* 发生上电复位 */ 
			printf_rtcdbg("发生上电复位 \n\r"); 
		} 
		/* 检测引脚复位标志是否设置 */ 
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) 
		{ 
			/* 发生引脚复位 */ 
			printf_rtcdbg("发生引脚复位 \n\r");     
		} 
		
		/* 使能PWR时钟 */ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
		/* 允许访问RTC */ 
		PWR_BackupAccessCmd(ENABLE); 
		/* 等待 RTC APB 寄存器同步 */ 
		RTC_WaitForSynchro(); 

	} 
}

/*
*********************************************************************************************************
*	函 数 名: RTC_Config
*	功能说明: 1. 选择不同的RTC时钟源LSI或者LSE。
*             2. 配置RTC时钟。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_Config(void)
{
	RTC_InitTypeDef  RTC_InitStructure;
	uint32_t uwAsynchPrediv = 0;
    uint32_t uwSynchPrediv = 0;
	
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* 使能LSE振荡器  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* 等待就绪 */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);

	/* 等待RTC APB寄存器同步 */
	RTC_WaitForSynchro();

	/* 配置RTC数据寄存器和分频器  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	/* 设置年月日和星期 */
	//RTC_DateStructure.RTC_Year = 17;
	//RTC_DateStructure.RTC_Month = 11;
	//RTC_DateStructure.RTC_Date = 13;
	//RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

	/* 设置时分秒，以及显示格式 */
	//RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	//RTC_TimeStructure.RTC_Hours   = 9;
	//RTC_TimeStructure.RTC_Minutes = 42;
	//RTC_TimeStructure.RTC_Seconds = 30; 
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);   

	/* 配置备份寄存器，表示已经设置过RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CalcWeek
*	功能说明: 根据日期计算星期几
*	形    参: _year _mon _day  年月日  (年是2字节整数，月和日是字节整数）
*	返 回 值: 周几 （1-7） 7表示周日
*********************************************************************************************************
*/
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day)
{
	/*
	蔡勒（Zeller）公式
		历史上的某一天是星期几？未来的某一天是星期几？关于这个问题，有很多计算公式（两个通用计算公式和
	一些分段计算公式），其中最著名的是蔡勒（Zeller）公式。
	    即w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

		公式中的符号含义如下，
	     w：星期；
	     c：年的高2位，即世纪-1
	     y：年（两位数）；
	     m：月（m大于等于3，小于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，
	  	    比如2003年1月1日要看作2002年的13月1日来计算）；
	     d：日；
	     [ ]代表取整，即只要整数部分。

	    算出来的W除以7，余数是几就是星期几。如果余数是0，则为星期日。
        如果结果是负数，负数求余数则需要特殊处理：
            负数不能按习惯的余数的概念求余数，只能按数论中的余数的定义求余。为了方便
        计算，我们可以给它加上一个7的整数倍，使它变为一个正数，然后再求余数

		以2049年10月1日（100周年国庆）为例，用蔡勒（Zeller）公式进行计算，过程如下：
		蔡勒（Zeller）公式：w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
		=49+[49/4]+[20/4]-2×20+[26× (10+1)/10]+1-1
		=49+[12.25]+5-40+[28.6]
		=49+12+5-40+28
		=54 (除以7余5)
		即2049年10月1日（100周年国庆）是星期5。
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
	else	/* 某年的1、2月要看作上一年的13、14月来计算 */
	{
		m = _mon + 12;
		y = (_year - 1) % 100;
		c = (_year - 1) / 100;
		d = _day;
	}

	w = y + y / 4 +  c / 4 - 2 * c + ((uint16_t)26*(m+1))/10 + d - 1;
	if (w == 0)
	{
		w = 7;	/* 表示周日 */
	}
	else if (w < 0)	/* 如果w是负数，则计算余数方式不同 */
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
		w = 7;	/* 表示周日 */
	}
	
	return w;
}

/***************************** end *********************************/

