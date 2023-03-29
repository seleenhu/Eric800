
#include "bsp.h"
#define SampleFreq 12800
#define TransCnt ADCbufSize/2
//#define debug

static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcTriggerConfig(void);
static void AdcDmaConfig(void);
static void AdcSetChannelPinMode(const M4_ADC_TypeDef *ADCx,
                                 uint32_t u32Channel,
                                 en_pin_mode_t enMode);
static void AdcSetPinMode(uint8_t u8AdcPin, en_pin_mode_t enMode);
static void TimerConfig(void);
static void DmaIrqConfig(void);
#if 0
static stc_clk_sysclk_cfg_t m_stcSysclkCfg =
{
    /* Default system clock division. */
    .enHclkDiv  = ClkSysclkDiv1,  // 200MHz
    .enExclkDiv = ClkSysclkDiv2,  // 100MHz
    .enPclk0Div = ClkSysclkDiv1,  // 200MHz
    .enPclk1Div = ClkSysclkDiv2,  // 100MHz
    .enPclk2Div = ClkSysclkDiv4,  // 50MHz
    .enPclk3Div = ClkSysclkDiv4,  // 50MHz
    .enPclk4Div = ClkSysclkDiv2,  // 100MHz
};
#endif
ADCStruct ADCFifo;


//int16_t AD_Value[ADC_N*ADC_M];
//uint16_t AD_bt;
/*
//UA <-> PA0  ADC1_IN0
I1 <-> PA1  ADC1_IN1
VA <-> PA2	ADC1_IN2
*/



/*
---------------------------------------------------------------------
Name		ADCInit
Function	无
Input		无
OutPut		无
Edit by fanzhong
Date		2018-10-29
---------------------------------------------------------------------
*/
void ADCInit(void)
{
	AdcClockConfig();
  AdcInitConfig();
  AdcChannelConfig();
  AdcTriggerConfig();
	TimerConfig();//trigger time config
  AdcDmaConfig();
	DmaIrqConfig();
	
		
}//ADCInit
//ADCInit


/**
 *******************************************************************************
 ** \brief  ADC clock configuration.
 **
 ** \note   1) ADCLK max frequency is 60MHz.
 **         2) If PCLK2 and PCLK4 are selected as the ADC clock,
 **            the following conditions must be met:
 **            a. ADCLK(PCLK2) max 60MHz;
 **            b. PCLK4 : ADCLK = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4
 **
 ******************************************************************************/
static void AdcClockConfig(void)
{
#if (ADC_CLK == ADC_CLK_PCLK)
    stc_clk_sysclk_cfg_t stcSysclkCfg;

    /* Set bus clock division, depends on the system clock frequency. */
    stcSysclkCfg.enHclkDiv  = ClkSysclkDiv1;  // 200MHz
    stcSysclkCfg.enExclkDiv = ClkSysclkDiv2;  // 100MHz
    stcSysclkCfg.enPclk0Div = ClkSysclkDiv1;  // 200MHz
    stcSysclkCfg.enPclk1Div = ClkSysclkDiv2;  // 100MHz
    stcSysclkCfg.enPclk2Div = ClkSysclkDiv4;  // 50MHz
    stcSysclkCfg.enPclk3Div = ClkSysclkDiv4;  // 50MHz
    stcSysclkCfg.enPclk4Div = ClkSysclkDiv1;  // 100MHz.
    CLK_SysClkConfig(&stcSysclkCfg);
    CLK_SetPeriClkSource(ClkPeriSrcPclk);

#elif (ADC_CLK == ADC_CLK_MPLLQ)
    stc_clk_xtal_cfg_t stcXtalCfg;
    stc_clk_mpll_cfg_t stcMpllCfg;

    if (CLKSysSrcMPLL == CLK_GetSysClkSource())
    {
        /*
         * Configure MPLLQ(same as MPLLP and MPLLR) when you
         * configure MPLL as the system clock.
         */
    }
    else
    {
        /* Use XTAL as MPLL source. */
        stcXtalCfg.enFastStartup = Enable;
        stcXtalCfg.enMode = ClkXtalModeOsc;
        stcXtalCfg.enDrv  = ClkXtalLowDrv;
        CLK_XtalConfig(&stcXtalCfg);
        CLK_XtalCmd(Enable);

        /* Set MPLL out 240MHz. */
        stcMpllCfg.pllmDiv = 1u;
        /* mpll = 8M / pllmDiv * plln */
        stcMpllCfg.plln    = 30u;
        stcMpllCfg.PllpDiv = 16u;
        stcMpllCfg.PllqDiv = 16u;
        stcMpllCfg.PllrDiv = 16u;
        CLK_SetPllSource(ClkPllSrcXTAL);
        CLK_MpllConfig(&stcMpllCfg);
        CLK_MpllCmd(Enable);
    }
    CLK_SetPeriClkSource(ClkPeriSrcMpllp);

#elif (ADC_CLK == ADC_CLK_UPLLR)
    stc_clk_xtal_cfg_t stcXtalCfg;
    stc_clk_upll_cfg_t stcUpllCfg;

    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcUpllCfg);

    /* Use XTAL as UPLL source. */
    stcXtalCfg.enFastStartup = Enable;
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv  = ClkXtalLowDrv;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* Set UPLL out 240MHz. */
    stcUpllCfg.pllmDiv = 2u;
    /* upll = 8M(XTAL) / pllmDiv * plln */
    stcUpllCfg.plln    = 60u;
    stcUpllCfg.PllpDiv = 16u;
    stcUpllCfg.PllqDiv = 16u;
    stcUpllCfg.PllrDiv = 16u;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_UpllConfig(&stcUpllCfg);
    CLK_UpllCmd(Enable);
    CLK_SetPeriClkSource(ClkPeriSrcUpllr);
#endif
}

/**
 *******************************************************************************
 ** \brief  ADC initial configuration.
 **
 ******************************************************************************/
static void AdcInitConfig(void)
{
    stc_adc_init_t stcAdcInit;

    MEM_ZERO_STRUCT(stcAdcInit);

    stcAdcInit.enResolution = AdcResolution_12Bit;	//12位数据
    stcAdcInit.enDataAlign  = AdcDataAlign_Right;
    stcAdcInit.enAutoClear  =	AdcClren_Enable;
    stcAdcInit.enScanMode   = AdcMode_SAOnce;//AdcMode_SAContinuous; //AdcMode_SAOnceSBOnce; 序列A连续
    //stcAdcInit.enRschsel    = AdcRschsel_Continue; //
    /* 1. Enable ADC1. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC1, Enable);
    /* 2. Initialize ADC1. */
    ADC_Init(M4_ADC1, &stcAdcInit);
  
}

/**
 *******************************************************************************
 ** \brief  ADC channel configuration.
 **
 ******************************************************************************/
static void AdcChannelConfig(void)
{
    stc_adc_ch_cfg_t stcChCfg;
    uint8_t au8Adc1SaSampTime[ADC1_SA_CHANNEL_COUNT] = ADC1_SA_CHANNEL_SAMPLE_TIME;
  
    MEM_ZERO_STRUCT(stcChCfg);

    /**************************** Add ADC1 channels ****************************/
    /* 1. Set the ADC pin to analog mode. */
    AdcSetChannelPinMode(M4_ADC1, ADC1_CHANNEL, Pin_Mode_Ana);

    stcChCfg.u32Channel  = ADC1_SA_CHANNEL;
    stcChCfg.u8Sequence  = ADC_SEQ_A;
    stcChCfg.pu8SampTime = au8Adc1SaSampTime;
    /* 2. Add ADC channel. */
    ADC_AddAdcChannel(M4_ADC1, &stcChCfg);

    
}

/**
 *******************************************************************************
 ** \brief  ADC trigger source configuration.
 **
 ******************************************************************************/
static void AdcTriggerConfig(void)
{
    stc_adc_trg_cfg_t stcTrgCfg;

    MEM_ZERO_STRUCT(stcTrgCfg);

    /*
     * If select an event(@ref en_event_src_t) to trigger ADC,
     * AOS must be enabled first.
     */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_AOS, Enable);

    /* ADC1 sequence A will be started by software. */
		stcTrgCfg.u8Sequence = ADC_SEQ_A;
    stcTrgCfg.enTrgSel   = AdcTrgsel_TRGX0;
    stcTrgCfg.enInTrg0   = EVT_TMR02_GCMA;
    ADC_ConfigTriggerSrc(M4_ADC1, &stcTrgCfg);
    ADC_TriggerSrcCmd(M4_ADC1, ADC_SEQ_A, Enable);
	
    //ADC_TriggerSrcCmd(M4_ADC1, ADC_SEQ_A, Disable);
	  
 
}

/**
 *******************************************************************************
 ** \brief  DMA configuration for ADC1 and ADC2.
 **
 ******************************************************************************/
static void AdcDmaConfig(void)
{
    stc_dma_config_t stcDmaCfg;

    MEM_ZERO_STRUCT(stcDmaCfg);

    stcDmaCfg.u16BlockSize   = 2;//ADC1_CH_COUNT;
    stcDmaCfg.u16TransferCnt = TransCnt;
    stcDmaCfg.u32SrcAddr     = (uint32_t)(&M4_ADC1->DR6);
    stcDmaCfg.u32DesAddr     = (uint32_t)(&ADCFifo.AD_Value);
		stcDmaCfg.u16DesRptSize  = 0;// 0: 1024 repeat
    stcDmaCfg.u16SrcRptSize  = 2;//ADCbufSize;
    stcDmaCfg.u32DmaLlp      = 0u;
    stcDmaCfg.stcSrcNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcSrcNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDesNseqCfg.u16Cnt    = 0u;
    stcDmaCfg.stcDesNseqCfg.u32Offset = 0u;
    stcDmaCfg.stcDmaChCfg.enSrcInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enDesInc    = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enSrcRptEn  = Enable;
    stcDmaCfg.stcDmaChCfg.enDesRptEn  = Disable;
    stcDmaCfg.stcDmaChCfg.enSrcNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enDesNseqEn = Disable;
    stcDmaCfg.stcDmaChCfg.enTrnWidth  = Dma16Bit;
    stcDmaCfg.stcDmaChCfg.enLlpEn     = Disable;
    stcDmaCfg.stcDmaChCfg.enIntEn     = Enable;

    PWC_Fcg0PeriphClockCmd(ADC1_SA_DMA_PWC, Enable);
    DMA_InitChannel(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, &stcDmaCfg);
    DMA_Cmd(ADC1_SA_DMA_UNIT, Enable);
    DMA_ChannelCmd(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, Enable);
    DMA_ClearIrqFlag(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, TrnCpltIrq);
    /* AOS must be enabled to use DMA */
    /* AOS enabled at first. */
    /* If you have enabled AOS before, then the following statement is not needed. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_AOS, Enable);
    DMA_SetTriggerSrc(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, ADC1_SA_DMA_TRGSRC);
		
		
    
}

/**
 *******************************************************************************
 ** \brief  Config the pin which is mapping the channel to analog or digit mode.
 **
 ******************************************************************************/
static void AdcSetChannelPinMode(const M4_ADC_TypeDef *ADCx,
                                 uint32_t u32Channel,
                                 en_pin_mode_t enMode)
{
    uint8_t u8ChIndex;
#if (ADC_CH_REMAP)
    uint8_t u8AdcPin;
#else
    uint8_t u8ChOffset = 0u;
#endif

    if (M4_ADC1 == ADCx)
    {
        u32Channel &= ADC1_PIN_MASK_ALL;
    }
    else
    {
        u32Channel &= ADC2_PIN_MASK_ALL;
#if (!ADC_CH_REMAP)
        u8ChOffset = 4u;
#endif
    }

    u8ChIndex = 0u;
    while (0u != u32Channel)
    {
        if (u32Channel & 0x1ul)
        {
#if (ADC_CH_REMAP)
            u8AdcPin = ADC_GetChannelPinNum(ADCx, u8ChIndex);
            AdcSetPinMode(u8AdcPin, enMode);
#else
            AdcSetPinMode((u8ChIndex + u8ChOffset), enMode);
#endif
        }

        u32Channel >>= 1u;
        u8ChIndex++;
    }
}

/**
 *******************************************************************************
 ** \brief  Set an ADC pin as analog input mode or digit mode.
 **
 ******************************************************************************/
static void AdcSetPinMode(uint8_t u8AdcPin, en_pin_mode_t enMode)
{
    en_port_t enPort = PortA;
    en_pin_t enPin   = Pin00;
    bool bFlag       = true;
    stc_port_init_t stcPortInit;

    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = enMode;
    stcPortInit.enPullUp  = Disable;

    switch (u8AdcPin)
    {
    case ADC1_IN0:
        enPort = PortA;
        enPin  = Pin00;
        break;

    case ADC1_IN1:
        enPort = PortA;
        enPin  = Pin01;
        break;

    case ADC1_IN2:
        enPort = PortA;
        enPin  = Pin02;
        break;

    case ADC1_IN3:
        enPort = PortA;
        enPin  = Pin03;
        break;

    case ADC12_IN4:
        enPort = PortA;
        enPin  = Pin04;
        break;

    case ADC12_IN5:
        enPort = PortA;
        enPin  = Pin05;
        break;

    case ADC12_IN6:
        enPort = PortA;
        enPin  = Pin06;
        break;

    case ADC12_IN7:
        enPort = PortA;
        enPin  = Pin07;
        break;

    case ADC12_IN8:
        enPort = PortB;
        enPin  = Pin00;
        break;

    case ADC12_IN9:
        enPort = PortB;
        enPin  = Pin01;
        break;

    case ADC12_IN10:
        enPort = PortC;
        enPin  = Pin00;
        break;

    case ADC12_IN11:
        enPort = PortC;
        enPin  = Pin01;
        break;

    case ADC1_IN12:
        enPort = PortC;
        enPin  = Pin02;
        break;

    case ADC1_IN13:
        enPort = PortC;
        enPin  = Pin03;
        break;

    case ADC1_IN14:
        enPort = PortC;
        enPin  = Pin04;
        break;

    case ADC1_IN15:
        enPort = PortC;
        enPin  = Pin05;
        break;

    default:
        bFlag = false;
        break;
    }

    if (true == bFlag)
    {
        PORT_Init(enPort, enPin, &stcPortInit);
    }
}
#ifdef debug
void Timer0_TriggerCallBack(void)
{
    
   // GreenLedToggle();
	 RedLedToggle();
}
#endif

/**
 *******************************************************************************
 ** \brief  Timer configuration, for generating event EVT_TMR02_GCMA every second.
 **
 ** \param  None.
 **
 ** \retval None.
 **
 ******************************************************************************/
static void TimerConfig(void)
{
    stc_tim0_base_init_t stcTimerCfg;
    stc_clk_freq_t stcClkTmp;
    uint32_t u32Pclk1;
	//========debug
		#ifdef debug
		stc_irq_regi_conf_t stcIrqRegiConf;
		#endif

    MEM_ZERO_STRUCT(stcTimerCfg);

    /* The new system clock frequency is 200MHz, PCLK1 is 100MHz by default. */
	  /* The new system clock frequency is 168MHz, PCLK1 is 168MHz by default. */
	  #if 0
    m_stcSysclkCfg.enPclk1Div = ClkSysclkDiv16;
    CLK_SysClkConfig(&m_stcSysclkCfg);
		#endif
    /* Get PCLK1. */
    CLK_GetClockFreq(&stcClkTmp);
    u32Pclk1 = stcClkTmp.pclk1Freq;

    /* Timer0 peripheral enable. */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM02, Enable);
    /* Config register for channel A. */
    stcTimerCfg.Tim0_CounterMode = Tim0_Sync;
    stcTimerCfg.Tim0_SyncClockSource = Tim0_Pclk1;
    //stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv1024;
		stcTimerCfg.Tim0_ClockDivision = Tim0_ClkDiv0;
    /* Tim0_CmpValue's type is uint16_t!!! Be careful!!! */
    stcTimerCfg.Tim0_CmpValue = (uint16_t)(u32Pclk1/SampleFreq - 1u);
    TIMER0_BaseInit(TMR_UNIT, Tim0_ChannelA, &stcTimerCfg);

    /* Start timer0. */
    TIMER0_Cmd(TMR_UNIT, Tim0_ChannelA, Enable);
		
		#ifdef debug
		/* Enable channel A interrupt */
		TIMER0_IntCmd(TMR_UNIT,Tim0_ChannelA,Enable);
		/* Register TMR_INI_GCMB Int to Vect.No.002 */
    stcIrqRegiConf.enIRQn = Int001_IRQn;
    /* Select TIMER channalB interrupt as source */
    stcIrqRegiConf.enIntSrc = TMR_INI_GCMA;
		/* Callback function */
    stcIrqRegiConf.pfnCallback = &Timer0_TriggerCallBack;
		/* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);
		 /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
		#endif
}

/**
 *******************************************************************************
 ** \brief  DMA IRQ register function.
 **
 ******************************************************************************/
static void DmaIrqRegister(stc_irq_regi_conf_t *pstcCfg, uint32_t u32Priority)
{
    int16_t s16Vnum = pstcCfg->enIRQn;

    if (((s16Vnum >= Int000_IRQn) && (s16Vnum <= Int031_IRQn)) ||
        ((s16Vnum >= Int038_IRQn) && (s16Vnum <= Int043_IRQn)))
    {
        if (Ok != enIrqRegistration(pstcCfg))
        {
            return;
        }
    }
    else if (Int129_IRQn == s16Vnum)
    {
        enShareIrqEnable(pstcCfg->enIntSrc);
    }
    else
    {
        return;
    }
    NVIC_ClearPendingIRQ(pstcCfg->enIRQn);
    NVIC_SetPriority(pstcCfg->enIRQn, u32Priority);
    NVIC_EnableIRQ(pstcCfg->enIRQn);
}


static void DmaIrqConfig(void)
{
		stc_irq_regi_conf_t stcAdcIrqCfg;
		stcAdcIrqCfg.enIntSrc    = ADC1_SA_DMA_IRQ_NUM;
    stcAdcIrqCfg.enIRQn      = Int030_IRQn;
    stcAdcIrqCfg.pfnCallback = &ADC1_SA_DMA_INT_CB;
    DmaIrqRegister(&stcAdcIrqCfg, DDL_IRQ_PRIORITY_03);
}


void  Dma2Tc3_IrqHandler(void)
{
		DMA_ClearIrqFlag(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, TrnCpltIrq);
    DMA_SetTransferCnt(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, TransCnt);//传输次数重置
	  DMA_SetDesAddress(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, (uint32_t)(&ADCFifo.AD_Value)); //目标地址重设
    DMA_ChannelCmd(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH, Enable);
		
}

uint16_t DMA_ADC1_GetCurrDataCounter(void)
{
	uint16_t DataCounter;
	DMA_GetMonDTC_CNT(ADC1_SA_DMA_UNIT, ADC1_SA_DMA_CH,&DataCounter);
	return DataCounter;
}

