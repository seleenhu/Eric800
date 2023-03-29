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
#include "MainTask.h"



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
	MainTask();
	
	//AD7606_StartRecord(12800);
	//InitFrequency();
	
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ��д��PR��RLR
	//IWDG_SetPrescaler(IWDG_Prescaler_256);  //д��PRԤ��Ƶֵ
	//IWDG_SetReload(1000);  //д��RLR
	//IWDG_Enable();//KRд��0xCCCC

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
	//ModuleTxLen = 0;
	//ModuleTxLen = sprintf((char *)ModuleTxBuf,"This is test\r\n");
	//ModuleTxBuf[ModuleTxLen++]=0x55;
	//ModuleTxBuf[ModuleTxLen++]=0xAA;
	//ModuleTxStart();
	
	while(1){
		//IWDG_ReloadCounter();		
		
		//if(WholeBit.Bits.F5ms){
		//	WholeBit.Bits.F5ms = 0;
			
		
		
		//}		
		if(WholeBit.Bits.F10ms){
			WholeBit.Bits.F10ms = 0;			
			InOutDisplay();
			bsp_RunPer1ms();
			RF433Task();
			Display4G();
			RS232Task();
			RS485_Task();
//==============play sound
			PlayAudio();			
//======================
			ModuleComSPIRX();
			ReadSDFault();

			//W5500_task();
			
		}
		if(WholeBit.Bits.F100ms){
			WholeBit.Bits.F100ms = 0;
			//GUI_Exec();
			GUI_Delay(5);
		}
		if(WholeBit.Bits.F500ms){
			
			WholeBit.Bits.F500ms = 0;
			if(SystemSet.ParamSaveFlag & FlagSystemStateSave){
				SaveParam();
				SystemSet.ParamSaveFlag &= ~FlagSystemStateSave;
			}
			//Calculate_Task();
		}
		
		//SaveSDFault();		
		
		//ReadSDFault();
	}

}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
