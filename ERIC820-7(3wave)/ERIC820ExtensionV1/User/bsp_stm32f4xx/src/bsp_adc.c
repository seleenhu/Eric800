
#include "mydefine.h"

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
Function	��
Input		��
OutPut		��
Edit by fanzhong
Date		2018-10-29
---------------------------------------------------------------------
*/
void ADCInit(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;

	DMA_InitTypeDef  DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	
	
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	
    
    //ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    //DMA����
    DMA_InitStructure.DMA_Channel = DMA_Channel_0; //ѡ��ͨ����
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); //��Χ�豸��ַ,ADC_DR_DATA�������ݼĴ���
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(ADCFifo.AD_Value); //DMA�洢����ַ,�Լ����õĻ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //���䷽��,����->�洢��
    DMA_InitStructure.DMA_BufferSize = ADCbufSize; //DMA�����С,���ݴ�����32*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�洢���Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݳ��Ȱ����(16λ)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�洢�����ݳ��Ȱ���(16λ)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMAģʽ,��ͨģʽ��ѭ��ģʽ,����˫����ģʽ,��Ҫ��������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA���ȼ�,�����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //��ʹ��FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //�洢��ͻ��,���δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ��,���δ���
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); //��ʼ��DMA2_Stream0,��ӦΪADC1

    //˫����ģʽ����
    //DMA_DoubleBufferModeConfig(DMA1_Stream0,(uint32_t)(u16 *)(uAD_Buff_B),DMA_Memory_0);    //DMA_Memory_0���ȱ�����
    //DMA_DoubleBufferModeCmd(DMA1_Stream0,ENABLE);
    
    //����DMA�ж�
    //DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //����жϱ�־   
    //DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //��������ж�
    
    DMA_Cmd(DMA2_Stream0, ENABLE); //ʹ��DMA


	
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);      //ADC1��λ
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); //��λ����     

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���������׶�֮����ӳ�5��ʱ��(����ģʽ��˫��ģʽ��ʹ��)
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��(���ڶ��ADCģʽ)
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ(��ͨ��ADC�ɼ�Ҫ��ɨ��ģʽ)    
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
//    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //�������
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //�����ش���
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  //��ʱ���¼�2����ADC
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���    
    ADC_InitStructure.ADC_NbrOfConversion = ADC_N;    //9��ת���ڹ���������
    ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
    
    //����ģʽ��,ͨ��������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_15Cycles); 
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);
 

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //����ʹ��DMA
    ADC_DMACmd(ADC1, ENABLE);  //ʹ��ADC_DMA
    ADC_Cmd(ADC1, ENABLE);//����ADת���� 

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  

	//��ʱ������12.8K
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //��Ƶϵ��,TIM2ʱ���ڷ�Ƶϵ����Ϊ1ʱ,ʱ����APB1ʱ�ӵ�����84M
	TIM_TimeBaseStructure.TIM_Period = 13125 - 1; //����ֵ1M/32=31250,CurrentFreq��λΪ0.1hz,���������10��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //��ʼ����ʱ��2

	TIM_ARRPreloadConfig(TIM8, ENABLE); //����TIM2��ʱ����
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);  //ѡ��TIM2��UPDATA�¼�����Ϊ����Դ
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); //����TIM2�ж�����
	
	TIM_Cmd(TIM8, ENABLE);
#if 0	
	//ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          

	//ʧ�ܶ�ʱ��
	TIM_Cmd(TIM2, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //��ʼ����ʱ��

	//��ʱ������6.4K
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //��Ƶϵ��,TIM2ʱ���ڷ�Ƶϵ����Ϊ1ʱ,ʱ����APB1ʱ�ӵ�����84M
	TIM_TimeBaseStructure.TIM_Period = 13125 - 1; //����ֵ1M/32=31250,CurrentFreq��λΪ0.1hz,���������10��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ����ʱ��2


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//������ϸ˵��  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;  
	TIM_OCInitStructure.TIM_Pulse = 5000;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�����PWM1ҪΪLow��PWM2��ΪHigh  
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);
	TIM_InternalClockConfig(TIM2); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	//ʹ�ܶ�ʱ���ж�    
	//TIM_ARRPreloadConfig(TIM2, ENABLE); //����TIM2��ʱ����
	//TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);  //ѡ��TIM2��UPDATA�¼�����Ϊ����Դ
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //����TIM2�ж�����

	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //������ʱ�� 3 �����ж�
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ�� 3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// �ܳ�ʼ��

	//ʹ��TIM2
	TIM_Cmd(TIM2, ENABLE); 

	//TIM_InternalClockConfig(TIM2);  
	//TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	//TIM_UpdateDisableConfig(TIM2, DISABLE);
#endif	
}//ADCInit
//ADCInit




