/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.3
*	说    明 : 这个例子演示如何配置CAN外设在正常模式下发送和接收CAN帧。发送的帧用来控制LED灯。通过K1键启动发送，
*	           通过K2键启动自动定时发送。自动定时发送时，可以观察到板子上的LED灯依次点亮，即跑马灯的效果。	
*
*	CAN设置如下：
*		- 位速率  = 1M bit/s 
*		- CAN时钟 = 外部时钟(HSE)
*		- ID过滤  = 所有的ID均允许
*		- RTR = Data
*		- DLC = 1 字节
*		- Data: 将被点亮的LED序号(0-3)
*
*		CAN串行通信连接是一种总线，可以挂接多个CAN设备。理论上节点个数没有限制。实际上节点个数将受总线的延迟
*	时间和电气负载的限制。
*	
*	程序具有如下功能：
*		- 复位后LD1灯点亮
*		- 按下K1键后，挂在总线上的所有板子的LD2点亮其它的LED灯熄灭
*		- 反复按K1键会依次点亮板子上的LED灯
*		- 在主程序发送，在CAN1_RX0_IRQHandler 中断服务程序处理接收到的数据
*
*   实验内容：
*	        K1  = 按1次发送一个CAN帧，按下不放时，会自动连发
*			K2  = 启动定时发送，缺省周期是100ms
*			K3  = 停止定时发送		
*			摇杆上键 = 提高定时发送的速度，定时发送的周期减10ms
*			摇杆下键 = 降低定时发送的速度，定时发送的周期加10ms
*
*   注意事项：
*		1. 这个例子通过3个板子测试过。每个板子的程序是相同的，具体接线看本工程Doc文件中的截图。
*		2. 在CAN总线上的每个板子均可以扮演发送者或接受者角色。
*		3. 通常情况下，只需其实节点和终端节点需要接120欧的电阻，中间节点不需要接，实际测试三个板子都
*		   接了120欧的电阻也是没问题的。开发板默认都焊接上了120欧的电阻。
*		4. 可以修改程序适当提高CAN的波特率最大至1M，但是对通信双绞线的要求较高。
*		5. 板载的CAN收发器型号是 SN65HVD230，该芯片的RS引脚可以串联电阻实现信号的斜率抑制功能。0电阻是信号上升最快，
*		   也就是速度最高。安富莱开发板缺省配置的电阻是 1K。
*                  
*	修改记录 :
*		版本号   日期       作者       说明
*		v1.3    2014-01-29 Eric2013    首发
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

//#include "bsp.h"			/* 底层硬件驱动 */

#include "mydefine.h"

uint32_t MainDelayTime;
uint8_t F1SCount;
uint8_t CalFlag;
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{

	for(MainDelayTime = 0; MainDelayTime<26250000;MainDelayTime++);

	bsp_Init();		/* 硬件初始化 */

	
	TestPinInit();

	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能写入PR和RLR
	IWDG_SetPrescaler(IWDG_Prescaler_256);	//写入PR预分频值
	IWDG_SetReload(1000);  //写入RLR
	IWDG_Enable();//KR写入0xCCCC
	//bsp_LedOn(0);	
	//bsp_LedOff(0);
	//bsp_LedOn(1);
	//bsp_LedOff(1);
	//bsp_LedToggle(0);
	//bsp_LedToggle(1);
	bsp_LedOff(iGreenLed);
	bsp_LedOn(iRedLed);
	//bsp_LedOn(iRedLed);

	//SendCanMsg(0x321,"12345678",8);	
	while(1){
		if(FIFO_length(ADCFifo.bt, AD_pt, ADCbufSize)>=512){
			ADCFifo.bt = FIFO_pointer(ADCFifo.bt, 128, ADCbufSize);
			F5mS = 1;
			//Test1Xor();
		}
		if(F5mS){
			F5mS = 0;
			CanDelayTim++;
			IWDG_ReloadCounter();
			F1SCount++;
			if(F1SCount>=200){
				F1SCount = 0;
				CalRMS(AD_pt);
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
		SelCalRMS();		
	}
}



/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
