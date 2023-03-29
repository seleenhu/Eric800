/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 本实验主要学习emWin裸机方式移植。
*              实验目的：
*                1. 学习emWin裸机方式移植。
*                2. emWin功能的实现在MainTask.c文件里面。
*              注意事项：
*                1. 务必将编辑器的缩进参数和TAB设置为4来阅读本文件，要不代码显示不整齐。
*
*	修改记录 :
*		版本号   日期         作者            说明
*       V1.0    2016-07-16   Eric2013    1. ST固件库1.6.1版本
*                                        2. BSP驱动包V1.2
*                                        3. FatFS版本V0.11a
*                                        4. STemWin版本V5.32
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"			/* 底层硬件驱动 */
//#include "MainTask.h"


uint8_t testTim;
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main (void) 
{	
	
	
	/* 初始化外设 */
	bsp_Init();
		
	/* 进入emWin主函数 */
	//MainTask();
	
	AD7606_StartRecord(12800);
	InitFrequency();
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能写入PR和RLR
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //写入PR预分频值
	IWDG_SetReload(10000);  //写入RLR
	IWDG_Enable();//KR写入0xCCCC

#if 0
	FATFS fp;
	FRESULT Frt;
	UINT ByteWritten;
	
	result = f_mount(&fs, "0:/", 0);
	Frt = f_open(&fp,"This_1.txt",FA_OPEN_ALWAYS | FA_WRITE);
	Frt = f_write(&fp,(char *)(FaultFifo[0]),sizeof(FaultSaveStruct),&ByteWritten);	
	f_close(&fp);

	result = f_mount(&fs, "0:/", 0);
	Frt = f_open(&fp,"This_1.txt",FA_OPEN_ALWAYS | FA_WRITE);
	Frt = f_write(&fp,(char *)(FaultFifo[1]),sizeof(FaultSaveStruct),&ByteWritten);	
	f_close(&fp);
#endif
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
#if 0
	Out1PinSet();
	Out1PinReset();
	Out2PinSet();
	Out2PinReset();
	Out3PinSet();
	Out3PinReset();
	Out4PinSet();
	Out4PinReset();
	Out5PinSet();
	Out5PinReset();
	Out6PinSet();
	Out6PinReset();
	Out7PinSet();
	Out7PinReset();
	Out8PinSet();
	Out8PinReset();
	Out9PinSet();
	Out9PinReset();
	Out10PinSet();
	Out10PinReset();
	Out11PinSet();
	Out11PinReset();
	Out12PinSet();
	Out12PinReset();
#endif
//SiliconControl_flag = 1;
	while(1){
		IWDG_ReloadCounter();
		#ifdef UseLocal_16Current
		M1andM3_task();
		#endif
		CAN1_Task();
		if(WholeBit.Bits.F5ms){
			WholeBit.Bits.F5ms = 0;
			
			//TestPinSet();
			FaskCalTask();
			//TestPinReset();
			//bsp_RunPer1ms();
			
		}

		if(WholeBit.Bits.F10ms){
			WholeBit.Bits.F10ms = 0;
			
			InOutDisplay();
			//KeyScan();
			SaveNandTask();
			ModuleTask();
			
			Uart2M1andM3Delay++;
			SPI2M1andM3Delay++;

			CanRxDelayTim++;
			CanReadWaveDelayTim++;
			CanReadRmsDelayTim++;
			CanSBreathTimes();
			//================接地相２小时关断
			if(GndPhase2HourFlag==1){
				GndPhase2HourCount++;
				if(GndPhase2HourCount>=2*60*60*100){		//2小时
					GndPhase2HourCount=0;
					GndPhase2HourFlag=0;
					SystemState.SwitchState.AphaseState = 0x02;
					SystemState.SwitchState.BphaseState = 0x02;
					SystemState.SwitchState.CphaseState = 0x02;
					SystemState.SwitchState.DphaseState = 0x02;	
				}
			}

			//CanBreath();
			//bsp_RunPer1ms();
			//RF433Task();
			//Display4G();
			//RS232Task();
			//RS485_Task();
//==============play sound
			//PlayAudio();
			
//======================	
			
#ifdef TestOutput			
			static uint16_t testTim;
			static float Maxbuf[8];
			static float Minbuf[8];
			static float Prebuf[8];
			uint8_t test_i;
			//Test1Xor();
			for(test_i=0; test_i<8;test_i++){
				if(Maxbuf[test_i] < ADCFifo->RMS_buf[test_i]){
					Maxbuf[test_i] = ADCFifo->RMS_buf[test_i];
				}else if(ADCFifo->RMS_buf[test_i] < Minbuf[test_i]){
					Minbuf[test_i] = ADCFifo->RMS_buf[test_i];
				}				
			}
			
			testTim++;
			if(testTim>=200){
				testTim = 0;
				//RTC_GetDate(RTC_Format_BIN, &MyRTC_Date);
				//RTC_GetTime(RTC_Format_BIN, &MyRTC_Time);				
				
				printf("UA = %f,UB = %f,UC = %f,U0 = %f\r\n",ADCFifo->RMS_buf[0],ADCFifo->RMS_buf[1],ADCFifo->RMS_buf[2],ADCFifo->RMS_buf[3]);
				printf("IA = %f,IB = %f,IC = %f,I0 = %f\r\n",ADCFifo->RMS_buf[4],ADCFifo->RMS_buf[5],ADCFifo->RMS_buf[6],ADCFifo->RMS_buf[7]);
				printf("Max UA = %f,UB = %f,UC = %f,U0 = %f\r\n",Maxbuf[0],Maxbuf[1],Maxbuf[2],Maxbuf[3]);
				printf("Max IA = %f,IB = %f,IC = %f,I0 = %f\r\n",Maxbuf[4],Maxbuf[5],Maxbuf[6],Maxbuf[7]);
				printf("Min UA = %f,UB = %f,UC = %f,U0 = %f\r\n",Minbuf[0],Minbuf[1],Minbuf[2],Minbuf[3]);
				printf("Min IA = %f,IB = %f,IC = %f,I0 = %f\r\n",Minbuf[4],Minbuf[5],Minbuf[6],Minbuf[7]);

				for(test_i=0; test_i<8;test_i++){
					Prebuf[test_i] = (Maxbuf[test_i] - Minbuf[test_i])/2;
					Prebuf[test_i] =  Prebuf[test_i] * 1000/ADCFifo->RMS_buf[test_i];
					Maxbuf[test_i] = ADCFifo->RMS_buf[test_i];				
					Minbuf[test_i] = ADCFifo->RMS_buf[test_i];
				}
				printf("Pre UA = %f ,UB = %f ,UC = %f ,U0 = %f \r\n",Prebuf[0],Prebuf[1],Prebuf[2],Prebuf[3]);
				printf("Pre IA = %f ,IB = %f ,IC = %f ,I0 = %f \r\n",Prebuf[4],Prebuf[5],Prebuf[6],Prebuf[7]);
				
			
				}
#endif
				
				
		}
		//if(WholeBit.Bits.F200ms){
		//	WholeBit.Bits.F200ms = 0;
			//GUI_Exec();
		//	GUI_Delay(5);
		//}
		if(WholeBit.Bits.F500ms){
			WholeBit.Bits.F500ms = 0;
			WriteParam();
			Calculate_Task();
			if(ComData.VerifyFlag){
				ComData.VerifyFlag = 0;
				ADCVerification();
			}
		}
		
		//SaveSDFault();		
		
		//ReadSDFault();
	}

}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
