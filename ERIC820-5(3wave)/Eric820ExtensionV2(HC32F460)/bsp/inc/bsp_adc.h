#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_


#define ADC_CH_REMAP                (0u)

/* ADC clock selection definition. */
#define ADC_CLK_PCLK                (1u)
#define ADC_CLK_MPLLQ               (2u)
#define ADC_CLK_UPLLR               (3u)

/* Select MPLLQ as ADC clock. */
//#define ADC_CLK                     (ADC_CLK_MPLLQ)
#define ADC_CLK                     (ADC_CLK_UPLLR)

/* ADC1 channel definition for this example. */
#define ADC1_SA_NORMAL_CHANNEL      (ADC1_CH6 | ADC1_CH7)
#define ADC1_SA_CHANNEL             (ADC1_SA_NORMAL_CHANNEL)
#define ADC1_SA_CHANNEL_COUNT       (2u)


#define ADC1_CHANNEL                (ADC1_SA_CHANNEL)

/* ADC1 channel sampling time.      ADC1_CH0  ADC1_CH1 */
//#define ADC1_SA_CHANNEL_SAMPLE_TIME { 0x40,     0x40 }
#define ADC1_SA_CHANNEL_SAMPLE_TIME { 0x40,     0x40 }



/* DMA definition for ADC1. */
#define ADC1_SA_DMA_UNIT            (M4_DMA2)
#define ADC1_SA_DMA_PWC             (PWC_FCG0_PERIPH_DMA2)
#define ADC1_SA_DMA_CH              (DmaCh3)
#define ADC1_SA_DMA_TRGSRC          (EVT_ADC1_EOCA)

#define ADC1_SA_DMA_IRQ_NUM         (INT_DMA2_TC3)
#define ADC1_SA_DMA_INT_CB          (Dma2Tc3_IrqHandler)

//=========================================================

/* Timer definition for this example. */
#define TMR_UNIT                    (M4_TMR02)
#define TMR_INI_GCMA        				(INT_TMR02_GCMA)
#define TMR_INI_GCMB        				(INT_TMR02_GCMB)






#define ADC_N   	 2
#define ADC_Period   256
#define ADC_M   (256*20*4)
#define ADCbufSize		(ADC_N*ADC_M)

typedef struct{	
	uint16_t bt;
	float RMSbuf;
	int16_t AD_Value[ADCbufSize];
}ADCStruct;
extern ADCStruct ADCFifo;

//#define AD_pt  (ADC_N*ADC_M - DMA_ADC1_GetCurrDataCounter(DMA2_Stream0))
#define AD_pt  (ADC_N*ADC_M/2 - DMA_ADC1_GetCurrDataCounter())

void ADCInit(void);
uint16_t DMA_ADC1_GetCurrDataCounter(void);


#endif





