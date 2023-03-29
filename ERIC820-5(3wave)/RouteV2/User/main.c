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
uint8_t SendFlag=0;
uint8_t SendCount=0;
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

	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能写入PR和RLR
	IWDG_SetPrescaler(IWDG_Prescaler_256);  //写入PR预分频值
	IWDG_SetReload(1000);  //写入RLR
	IWDG_Enable();//KR写入0xCCCC

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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
