#include "bsp.h"	

static u16 FrequencyBuf[3][2];
/*
---------------------------------------------------------------------
Name		InitFrequency
Function	初始化频率测量
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-18
---------------------------------------------------------------------
*/
void InitFrequency(void)
{
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_ICInitTypeDef  TIM5_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
   
    
       
  TIM_TimeBaseStructure.TIM_Period = 65536-1; 
  TIM_TimeBaseStructure.TIM_Prescaler =168-1;    
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  
  
	
  //TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;
  //TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  //TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  //TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
  //TIM5_ICInitStructure.TIM_ICFilter = 0x00;
  //TIM_ICInit(TIM5, &TIM5_ICInitStructure);
    
   
  //NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  //NVIC_Init(&NVIC_InitStructure);  
    
  //TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE); 
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource12);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
  TIM_Cmd(TIM5,ENABLE );    
	
}//InitFrequency
#if 0
/*
---------------------------------------------------------------------
Name		FrequencyBISR
Function频率计算
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-18
---------------------------------------------------------------------
*/
void FrequencyBISR(void)
{
		u16 TIM5_CAPTURE_VAL;
		u16 data16;
		float fdata;
		static u16 TIM5CH1_CAPTURE_VAL;
		
		
	
    if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){
			TIM5_CAPTURE_VAL = TIM_GetCapture1(TIM5);
			if(TIM5_CAPTURE_VAL >= TIM5CH1_CAPTURE_VAL){
				data16 = TIM5_CAPTURE_VAL - TIM5CH1_CAPTURE_VAL;
			}else{
				data16 = 65536 - (TIM5CH1_CAPTURE_VAL - TIM5_CAPTURE_VAL);
			}
			TIM5CH1_CAPTURE_VAL = TIM5_CAPTURE_VAL;
			fdata = data16;
			fdata = 500000/fdata;
			if((fdata >= 45) && (fdata <= 55)){
				SystemPowerQuality->FrequencyB = fdata;
			}else{
				SystemPowerQuality->FrequencyB = 0;
			}
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
    }
  
		
}//FrequencyBISR

#endif
/*
---------------------------------------------------------------------
Name		FrequencyACISR
Function频率计算
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-18
---------------------------------------------------------------------
*/
void FrequencyABCISR(void)
{
#if 0
		u16 TIM5_CAPTURE_VAL;
		u16 data16;		
		static u16 TIM5CH1_CAPTURE_VAL;
		static u16 TIM5CH2_CAPTURE_VAL;
		static u16 TIM5CH3_CAPTURE_VAL;
#endif	
		if(EXTI_GetITStatus(EXTI_Line10) != RESET){
#if 0
			
			TIM5_CAPTURE_VAL = TIM_GetCounter(TIM5);
			if(TIM5_CAPTURE_VAL >= TIM5CH1_CAPTURE_VAL){
				data16 = TIM5_CAPTURE_VAL - TIM5CH1_CAPTURE_VAL;
			}else{
				data16 = 65536 - (TIM5CH1_CAPTURE_VAL - TIM5_CAPTURE_VAL);
			}
			TIM5CH1_CAPTURE_VAL = TIM5_CAPTURE_VAL;
			
			data16 = 50000000/data16;
			if((data16 >= 4500) && (data16 <= 5500)){
				SystemPowerQuality->FrequencyB = data16;
			}else{
				SystemPowerQuality->FrequencyB = 0;
			}
#else
			FrequencyBuf[1][0] = FrequencyBuf[1][1];
			FrequencyBuf[1][1] = TIM_GetCounter(TIM5);
#endif
			EXTI_ClearITPendingBit(EXTI_Line10);
    }	
		if(EXTI_GetITStatus(EXTI_Line11) != RESET){
			FrequencyBuf[0][0] = FrequencyBuf[0][1];
			FrequencyBuf[0][1] = TIM_GetCounter(TIM5);
			EXTI_ClearITPendingBit(EXTI_Line11);
    }
		if(EXTI_GetITStatus(EXTI_Line12) != RESET){
			FrequencyBuf[2][0] = FrequencyBuf[2][1];
			FrequencyBuf[2][1] = TIM_GetCounter(TIM5);
			EXTI_ClearITPendingBit(EXTI_Line12);
    }
  
		
}//FrequencyABCISR

/*
---------------------------------------------------------------------
Name		FrequencyCal
Function频率计算
Input		No
OutPut	No
Edit by fanzhong
Date		2018-10-10
---------------------------------------------------------------------
*/
void FrequencyCal(void)
{
	u8 i;
	u16 data1,data2;
	for(i=0;i<3;i++){
		data1 = FrequencyBuf[i][0];
		data2 = FrequencyBuf[i][1];
		if(data2  >=  data1){
			data1 = data2-data1;
		}else{
			data1 = 65536 - (data1 - data2);
		}
		data1 = 50000000/data1;
		if((data1 >= 4500) && (data1 <= 5500)){
			SystemPowerQuality->Frequency[i] = data1;			
		}else{
			SystemPowerQuality->Frequency[i] = 0;
		}
	}
}//FrequencyCal
/*--------------------------end--------------------------*/

