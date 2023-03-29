#ifndef _BSP_RTC_H_
#define _BSP_RTC_H_



uint8_t My_RTC_Init(uint8_t setflag);
void TIM3_Int_Init(void);
void ReadRtcTime(void);
#endif
