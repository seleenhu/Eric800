#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_


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

#define AD_pt  (ADC_N*ADC_M - DMA_GetCurrDataCounter(DMA2_Stream0))

void ADCInit(void);
#endif





