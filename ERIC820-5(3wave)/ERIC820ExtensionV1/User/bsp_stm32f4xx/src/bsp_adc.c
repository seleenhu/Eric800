
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
Function	无
Input		无
OutPut		无
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



	
    
    //时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    //DMA设置
    DMA_InitStructure.DMA_Channel = DMA_Channel_0; //选择通道号
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); //外围设备地址,ADC_DR_DATA规则数据寄存器
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(ADCFifo.AD_Value); //DMA存储器地址,自己设置的缓存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //传输方向,外设->存储器
    DMA_InitStructure.DMA_BufferSize = ADCbufSize; //DMA缓存大小,数据传输量32*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设是否为增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储器是否为增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度半个字(16位)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储器数据长度半字(16位)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA模式,普通模式、循环模式,还有双缓冲模式,需要特殊设置
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA优先级,高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发,单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发,单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); //初始化DMA2_Stream0,对应为ADC1

    //双缓冲模式设置
    //DMA_DoubleBufferModeConfig(DMA1_Stream0,(uint32_t)(u16 *)(uAD_Buff_B),DMA_Memory_0);    //DMA_Memory_0首先被传输
    //DMA_DoubleBufferModeCmd(DMA1_Stream0,ENABLE);
    
    //设置DMA中断
    //DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //清除中断标志   
    //DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //传输完成中断
    
    DMA_Cmd(DMA2_Stream0, ENABLE); //使能DMA


	
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);      //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); //复位结束     

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟(三重模式或双重模式下使用)
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能(对于多个ADC模式)
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式(多通道ADC采集要用扫描模式)    
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
//    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //软件触发
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //上升沿触发
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  //定时器事件2触发ADC
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐    
    ADC_InitStructure.ADC_NbrOfConversion = ADC_N;    //9个转换在规则序列中
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
    
    //连续模式下,通道的配置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_15Cycles); 
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_15Cycles);
 

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //连续使能DMA
    ADC_DMACmd(ADC1, ENABLE);  //使能ADC_DMA
    ADC_Cmd(ADC1, ENABLE);//开启AD转换器 

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  

	//定时器设置12.8K
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //分频系数,TIM2时钟在分频系数不为1时,时钟是APB1时钟的两倍84M
	TIM_TimeBaseStructure.TIM_Period = 13125 - 1; //周期值1M/32=31250,CurrentFreq单位为0.1hz,因此需扩大10倍
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //初始化定时器2

	TIM_ARRPreloadConfig(TIM8, ENABLE); //允许TIM2定时重载
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);  //选择TIM2的UPDATA事件更新为触发源
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); //配置TIM2中断类型
	
	TIM_Cmd(TIM8, ENABLE);
#if 0	
	//时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          

	//失能定时器
	TIM_Cmd(TIM2, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //初始化定时器

	//定时器设置6.4K
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //分频系数,TIM2时钟在分频系数不为1时,时钟是APB1时钟的两倍84M
	TIM_TimeBaseStructure.TIM_Period = 13125 - 1; //周期值1M/32=31250,CurrentFreq单位为0.1hz,因此需扩大10倍
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器2


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//下面详细说明  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;  
	TIM_OCInitStructure.TIM_Pulse = 5000;  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//如果是PWM1要为Low，PWM2则为High  
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);
	TIM_InternalClockConfig(TIM2); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	//使能定时器中断    
	//TIM_ARRPreloadConfig(TIM2, ENABLE); //允许TIM2定时重载
	//TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);  //选择TIM2的UPDATA事件更新为触发源
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //配置TIM2中断类型

	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //③允许定时器 3 更新中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器 3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ④初始化

	//使能TIM2
	TIM_Cmd(TIM2, ENABLE); 

	//TIM_InternalClockConfig(TIM2);  
	//TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	//TIM_UpdateDisableConfig(TIM2, DISABLE);
#endif	
}//ADCInit
//ADCInit




