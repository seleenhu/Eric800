/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����ҪѧϰemWin�����ʽ��ֲ��
*              ʵ��Ŀ�ģ�
*                1. ѧϰemWin�����ʽ��ֲ��
*                2. emWin���ܵ�ʵ����MainTask.c�ļ����档
*              ע�����
*                1. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����            ˵��
*       V1.0    2016-07-16   Eric2013    1. ST�̼���1.6.1�汾
*                                        2. BSP������V1.2
*                                        3. FatFS�汾V0.11a
*                                        4. STemWin�汾V5.32
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"			/* �ײ�Ӳ������ */
//#include "MainTask.h"


uint8_t testTim;
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main (void) 
{	
	
	
	/* ��ʼ������ */
	bsp_Init();
		
	/* ����emWin������ */
	//MainTask();
	
	AD7606_StartRecord(12800);
	InitFrequency();
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ��д��PR��RLR
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //д��PRԤ��Ƶֵ
	IWDG_SetReload(10000);  //д��RLR
	IWDG_Enable();//KRд��0xCCCC

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
			//================�ӵ��ࣲСʱ�ض�
			if(GndPhase2HourFlag==1){
				GndPhase2HourCount++;
				if(GndPhase2HourCount>=2*60*60*100){		//2Сʱ
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
