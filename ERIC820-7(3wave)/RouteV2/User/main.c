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
uint8_t SendFlag=0;
uint8_t SendCount=0;
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

	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ��д��PR��RLR
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //д��PRԤ��Ƶֵ
	IWDG_SetReload(1000);  //д��RLR
	IWDG_Enable();//KRд��0xCCCC

	//FaultFifo[0]->DislayFlag = 0xFF;
	
	//GPIO_ResetBits(GPIOA, GPIO_Pin_1);

	while(1){
		IWDG_ReloadCounter();
		ModuleTask();
		SelRouteCalTask();
		if(WholeBit.Bits.F5ms){
			WholeBit.Bits.F5ms = 0;
			
			
			FaskCalTask();
			
			
		}

		if(WholeBit.Bits.F10ms){
			WholeBit.Bits.F10ms = 0;
			KeyScan();
			//SaveNandTask();			
			ModuleSPITXTask();
			if(SendFlag){
				SendCount++;
				if(SendCount>=5)
				{
					SendFlag=0;
					SendCount=0;
					ModuleSPIRMS();
				}
			}
							
		}		
		if(WholeBit.Bits.F500ms){
			WholeBit.Bits.F500ms = 0;
			WriteParam();			
			if(ComData.VerifyFlag){
				ComData.VerifyFlag = 0;				
				
				ADCVerification();
				
				ModuleUartTXTask(0x02);
				SendFlag=1;
				SendCount=0;
				
			}
		}
	}

}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
