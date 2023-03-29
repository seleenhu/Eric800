/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief power voltage detected interrupt sample
 **
 **   - 2021-04-16  CDT  First version for Device Driver Library of LPM.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "bsp.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DLY_MS                          (1000u)

uint8_t F1SCount;

#define PWC_PD_WAKEUP_EVENT             (PWC_PDWKEN0_WKUP01)
//#define PWC_PD_WAKEUP_EVENT             (PWC_PDWKEN2_NMI)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  Main function of power down mode wake up
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
	  uint16_t  ADC_pt;
    BSP_CLK_Init();
   // BSP_LED_Init();
		//BSP_ExIni_Init();
		bsp_Init();
	  Ddl_Delay1ms(DLY_MS);
	
		ReadRtcTime();
	
  
	      
		/*
    BSP_LED_Toggle(LED_RED);
    Ddl_Delay1ms(DLY_MS);
    BSP_LED_Toggle(LED_RED);
    Ddl_Delay1ms(DLY_MS);
    BSP_LED_Toggle(LED_RED);
    Ddl_Delay1ms(DLY_MS);
    BSP_LED_Toggle(LED_RED);
    Ddl_Delay1ms(DLY_MS);
		*/

    
		GreenLedOn();
    while(1)
    {
			if(FIFO_length(ADCFifo.bt, AD_pt, ADCbufSize/2)>=256) 	//一次增加2个点  2*64  5ms
			{
			ADCFifo.bt = FIFO_pointer(ADCFifo.bt, 64, ADCbufSize/2);
			F5mS = 1;
			//Test1Xor();
			}
			
			if(F5mS){
			F5mS = 0;
			CanDelayTim++;
			//IWDG_ReloadCounter();
			F1SCount++;
			if(F1SCount>=200){
				F1SCount = 0;
				ADC_pt=AD_pt;
				//CalRMS(AD_pt);
				CalRMS(ADC_pt);
				if(EepromSaveFlag){
					EepromSaveFlag = 0;
					FlashWrite();
				}
				
			}
			if(VerifyFlag){
				VerifyFlag = 0;
				CalVerify(AD_pt);
			}
			
			can_SetTask();
			CanBreath();
			CanSendWave();
		
		}
		
		can_Task();
		if(CanComData.FaultFlag){
			if(FIFO_length(SaveFaultPt, AD_pt, ADCbufSize/2)>=128){
				CanComData.Faultbt = AD_pt;
				StartCalRmsFlag=1;
				CanComData.FaultFlag=0;
			}
		}
		SelCalRMS();	
			  
			
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
