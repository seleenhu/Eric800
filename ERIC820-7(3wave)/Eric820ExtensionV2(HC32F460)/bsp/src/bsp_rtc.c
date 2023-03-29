
#include "bsp.h"

uint8_t u8SecIntFlag;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Rtc period callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void RtcPeriod_IrqCallback(void)
{
    u8SecIntFlag = 1u;
}


/**
 *******************************************************************************
 ** \brief Configure Rtc calendar function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Rtc_CalendarConfig(void)
{
    stc_rtc_date_time_t stcRtcDateTimeCfg;

    /* configure structure initialization */
    MEM_ZERO_STRUCT(stcRtcDateTimeCfg);

    /* calendar configuration */
    stcRtcDateTimeCfg.u8Year = 21u;
    stcRtcDateTimeCfg.u8Month = 8u;
    stcRtcDateTimeCfg.u8Day = 3u;
    stcRtcDateTimeCfg.u8Weekday = RtcWeekdayWednesday;
    stcRtcDateTimeCfg.u8Hour = 23u;
    stcRtcDateTimeCfg.u8Minute = 59u;
    stcRtcDateTimeCfg.u8Second = 55u;
    if (RTC_SetDateTime(RtcDataFormatDec, &stcRtcDateTimeCfg, Enable, Enable) != Ok)
    {
        DDL_Printf("write calendar failed!\r\n");
    }
		
		SystemTime.hour = stcRtcDateTimeCfg.u8Hour;
		SystemTime.minute = stcRtcDateTimeCfg.u8Minute;
		SystemTime.second = stcRtcDateTimeCfg.u8Second;
		SystemTime.year = stcRtcDateTimeCfg.u8Year;
		SystemTime.month = stcRtcDateTimeCfg.u8Month;
		SystemTime.day = stcRtcDateTimeCfg.u8Day;
		//SystemTime.us = TIM_GetCounter(TIM3);
		SystemTime.us = 0;	
}


/**
 *******************************************************************************
 ** \brief Xtal32 clock config
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Xtal32_ClockConfig(void)
{
    stc_clk_xtal32_cfg_t stcXtal32Cfg;

    /* configure structure initialization */
    MEM_ZERO_STRUCT(stcXtal32Cfg);

    /* Stop xtal32 */
    CLK_Xtal32Cmd(Disable);
    Ddl_Delay1ms(100u);
    /* Configuration xtal32 structure */
    stcXtal32Cfg.enDrv = ClkXtal32HighDrv;
    stcXtal32Cfg.enFilterMode = ClkXtal32FilterModeFull;
    CLK_Xtal32Config(&stcXtal32Cfg);
    /* Startup xtal32 */
    CLK_Xtal32Cmd(Enable);
    /* wait for xtal32 running */
    Ddl_Delay1ms(3000u);
}



/**
 *******************************************************************************
 ** \brief Configure Rtc peripheral function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Rtc_Config(void)
{
    stc_rtc_init_t stcRtcInit;
    stc_irq_regi_conf_t stcIrqRegiConf;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcRtcInit);
    MEM_ZERO_STRUCT(stcIrqRegiConf);

    /* Configure interrupt of rtc period */
    stcIrqRegiConf.enIntSrc = INT_RTC_PRD;
    stcIrqRegiConf.enIRQn = Int006_IRQn;
    stcIrqRegiConf.pfnCallback = &RtcPeriod_IrqCallback;
    enIrqRegistration(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

    /* Rtc stopped */
    if (0u == M4_RTC->CR1_f.START)
    {
        /* Reset rtc counter */
        if (RTC_DeInit() == ErrorTimeout)
        {
            DDL_Printf("reset rtc failed!\r\n");
        }
        else
        {
            /* Configuration rtc structure */
            stcRtcInit.enClkSource = RtcClkXtal32;
            stcRtcInit.enPeriodInt = RtcPeriodIntOneSec;
            stcRtcInit.enTimeFormat = RtcTimeFormat24Hour;
            stcRtcInit.enCompenWay = RtcOutputCompenDistributed;
            stcRtcInit.enCompenEn = Disable;
            stcRtcInit.u16CompenVal = 0u;
            RTC_Init(&stcRtcInit);

            /* Enable period interrupt */
            RTC_IrqCmd(RtcIrqPeriod, Enable);
            /* Startup rtc count */
            RTC_Cmd(Enable);

            /* wait for rtc running */
            Ddl_Delay1ms(1u);
            /* Update time after RTC startup */
            Rtc_CalendarConfig();
        }
    }
}

void BSP_RTC_Init(void)
{
	stc_rtc_date_time_t stcCurrDateTime;
	

	/* configure structure initialization */
	MEM_ZERO_STRUCT(stcCurrDateTime);
	
	Xtal32_ClockConfig();
	Rtc_Config();
}

void ReadRtcTime(void)
{
	stc_rtc_date_time_t stcRtcDateTimeCfg;
	 /* configure structure initialization */
  MEM_ZERO_STRUCT(stcRtcDateTimeCfg);
	RTC_GetDateTime(RtcDataFormatDec,&stcRtcDateTimeCfg);
	SystemTime.hour = stcRtcDateTimeCfg.u8Hour;
	SystemTime.minute = stcRtcDateTimeCfg.u8Minute;
	SystemTime.second = stcRtcDateTimeCfg.u8Second;
	SystemTime.year = stcRtcDateTimeCfg.u8Year;
	SystemTime.month = stcRtcDateTimeCfg.u8Month;
	SystemTime.day = stcRtcDateTimeCfg.u8Day;
	//SystemTime.us = TIM_GetCounter(TIM3);
	SystemTime.us = 0;
	
}


void SetRtcTime(void)
{
	stc_rtc_date_time_t stcRtcDateTimeCfg;
	 /* configure structure initialization */
  MEM_ZERO_STRUCT(stcRtcDateTimeCfg);
	
	 /* calendar configuration */
  stcRtcDateTimeCfg.u8Year = SystemTime.year;
  stcRtcDateTimeCfg.u8Month = SystemTime.month;
  stcRtcDateTimeCfg.u8Day = SystemTime.day;
  //stcRtcDateTimeCfg.u8Weekday = RtcWeekdayWednesday;
  stcRtcDateTimeCfg.u8Hour = SystemTime.hour;
  stcRtcDateTimeCfg.u8Minute = SystemTime.minute;
  stcRtcDateTimeCfg.u8Second = SystemTime.second;
  if (RTC_SetDateTime(RtcDataFormatDec, &stcRtcDateTimeCfg, Enable, Enable) != Ok)
    {
        DDL_Printf("write calendar failed!\r\n");
    }
}







