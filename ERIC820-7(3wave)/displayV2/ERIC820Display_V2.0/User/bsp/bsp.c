/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32F4XX)
*	文件名称 : bsp.c
*	版    本 : V1.1
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-03-01  armfly   正式发布
*		V1.1    2013-06-20  armfly   规范注释，添加必要说明
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "MainTask.h"



/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	u8 i;
	/*
	由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
	启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

	系统时钟缺省配置为168MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/
	/* 使能CRC 因为使用STemWin前必须要使能 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	/* 优先级分组设置为4，可配置0-15级抢占式优先级，0级子优先级，即不存在子优先级。*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	SystemCoreClockUpdate();	/* 根据PLL配置更新系统时钟频率变量 SystemCoreClock */
	
	bsp_InitDWT();
	
	bsp_InitExtIO(); 
	
	bsp_InitUart(); 	/* 初始化串口 */
	//bsp_InitKey();		/* 初始化按键变量（必须在 bsp_InitTimer() 之前调用） */

	bsp_InitRTC();
	   
	//bsp_InitLed(); 		/* 初始LED指示灯端口 */

	bsp_InitI2C();		/* 配置I2C总线 */
	bsp_InitTimer();

	bsp_InitExtSDRAM();      /* 初始化SDRAM */
	ClrSDRAM();
	
	for(i=0;i<FaultSaveSIZE;i++){
		//FaultFifo[i] = (FaultSaveStruct *)(FaultFifo_SDRAM + i*4*(sizeof(FaultSaveStruct)/4 +1));
		FaultFifo[i] = (FaultSaveStruct *)(FaultFifo_SDRAM + i*(sizeof(FaultSaveStruct)));
		//FaultSelFifo[i] = (FaultSelSaveStruct *)(FaultSelFifo_SDRAM + i*4*(sizeof(FaultSelSaveStruct)/4 +1));
		//FaultAllSelFifo[i] = (FaultAllSelSaveStruct *)(FaultAllSelFifo_SDRAM + i*4*(sizeof(FaultAllSelSaveStruct)/4 +1));
		FaultAllSelFifo[i] = (FaultAllSelSaveStruct *)(FaultAllSelFifo_SDRAM + i*(sizeof(FaultAllSelSaveStruct)));
	}
	FaultFifo_pt = 0;	
	
	//bsp_InitAD7606();
	bsp_InitSFlash();
	sf_ReadInfo();
	InitRF433();
	bsp_InitSyn6288();

	LoadParam();
	InitGUISystem();
	
	bsp_DetectLcdType();	/* 检测触摸板和LCD面板型号, 结果存在全局变量 g_TouchType, g_LcdType */	
	TOUCH_InitHard();		/* 初始化配置触摸芯片 */
	LCD_ConfigLTDC();       /* 初始化配置LTDC */
	
	//MemInit();

	Init4G();

	bsp_InitRS485();
	
	//Earth_LED_OFF();
	Com_LED_OFF();
	PutOP_LED_OFF();
	Power_LED_ON();
	Ground_LED_OFF();
	Alarm_LED_OFF();
	Decompression_LED_OFF();

	W5500_Init();
	

	//PhaseAStateOFF();
	//PhaseBStateOFF();
	//PhaseCStateOFF();
	//SwitchAOFF();	
	//SwitchBOFF();
	//SwitchCOFF();

	//BlackOutON();
	BuzzerOFF();

	ModuleComInit();
	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的
*			任务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	//bsp_KeyScan();		/* 按键扫描 */
	
	/* 电容触摸屏GT811 */
	if(g_GT811.Enable == 1)
	{
		GT811_OnePiontScan();
	}

	/* 电阻触摸屏FT5X06 */
	if(g_tFT5X06.Enable == 1)
	{
		FT5X06_OnePiontScan();
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的事务
*			 可以放在此函数。比如：触摸坐标扫描。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
	/* 电阻触摸屏 */
	if(g_tTP.Enable == 1)
	{
		TOUCH_Scan();	
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 */

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 例如 uIP 协议，可以插入uip轮询函数 */

}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
