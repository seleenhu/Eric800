/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H
//extern RTC_TimeTypeDef  RTC_TimeStructure;
//extern RTC_DateTypeDef  RTC_DateStructure;
extern RTC_TimeTypeDef  RTC_TimeStructure;
extern RTC_DateTypeDef  RTC_DateStructure;

void RTC_Config(void);
void bsp_InitRTC(void);

uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day);

#endif

/***************************** end *********************************/

