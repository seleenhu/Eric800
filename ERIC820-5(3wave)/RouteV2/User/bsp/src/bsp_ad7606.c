/*
*********************************************************************************************************
*
*	模块名称 : AD7606数据采集模块
*	文件名称 : bsp_ad7606.c
*	版    本 : V1.0
*	说    明 : AD7606挂在STM32的FMC总线上。
*
*			本例子使用了 TIM3 作为硬件定时器，定时启动ADC转换
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2017-11-16 JIM KATE  正式发布
*
*
*********************************************************************************************************
*/


#include "bsp.h"
//#include "param.h"

/*
	AD7606模块， 控制采集的IO:
	
	PC6/TIM3_CH1/TIM8_CH1     ----> AD7606_CONVST  输出PWM方波，作为ADC启动信号
	PB1/AD7606_BUSY           <---- AD7606_BUSY    , CPU在BUSY中断服务程序中读取采集结果
	
	这个两个IO和STM32-V5开发板是不同的
*/

/* 启动AD转换的GPIO : PC6 */
#define CONVST_1()	GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define CONVST_0()	GPIO_ResetBits(GPIOC,GPIO_Pin_6)

/* 设置输入量程的GPIO: PG10 */
#define RANGE_1()	GPIO_SetBits(GPIOG,GPIO_Pin_10)
#define RANGE_0()	GPIO_ResetBits(GPIOG,GPIO_Pin_10)

/* AD7606复位口线, PG7*/
#define RESET_1()	GPIO_SetBits(GPIOG,GPIO_Pin_7)
#define RESET_0()	GPIO_ResetBits(GPIOG,GPIO_Pin_7)

/* AD7606 FSMC总线地址，只能读，无需写 */
#define AD7606_RESULT()	*(__IO uint16_t *)0x6C000000

//AD7606_VAR_T g_tAD7606;		/* 定义1个全局变量，保存一些参数 */

#define ChannelSelSet()		GPIO_SetBits(GPIOH,GPIO_Pin_9)
#define ChannelSelClr()		GPIO_ResetBits(GPIOH,GPIO_Pin_9)
#define ChannelSelPin()		GPIO_ReadOutputDataBit(GPIOH,GPIO_Pin_9)

//static void AD7606_CtrlLinesConfig(void);
static void AD7606_FSMCConfig(void);
//static u8 ADCRMS_calculate(int16_t *buf);
#if 0
static u8 ADC_JudgePT(void);
static u8 ADC_JudgeExceedV(void);
static u8 ADC_JudgeGnd(void);
static u8 ADC_ShortCircuit(void);
static void ADCParameterSet(void);
static u8 ADC_PTFault(void);
#endif
//extern OS_TID HandleTaskGUI;
//extern OS_TID HandleTaskInOutIF;
//extern OS_TID HandleTaskRF;
//extern OS_TID HandleTaskGPS;
//extern OS_TID HandleTaskStart;

//extern OS_TID HandleTask4G;
//extern OS_TID HandleTaskNet;


/*
*********************************************************************************************************
*	函 数 名: bsp_InitAD7606
*	功能说明: 配置连接外部SRAM的GPIO和FSMC
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAD7606(void)
{
	//AD7606_CtrlLinesConfig();
	AD7606_FSMCConfig();
	
	AD7606_SetInputRange(1);	/* 0表示输入量程为正负5V, 1表示正负10V */

	AD7606_Reset();

	CONVST_1();					/* 启动转换的GPIO平时设置为高 */
	ChannelSelSet();
	//ADCParameterSet();
}


#if 0
/*
*********************************************************************************************************
*	函 数 名: AD7606_CtrlLinesConfig
*	功能说明: 配置GPIO口线，FMC管脚设置为复用功能
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
/*
	安富莱STM32-V6开发板接线方法：
	PD0/FMC_D2
	PD1/FMC_D3
	PD4/FMC_NOE		---- 读控制信号，OE = Output Enable ， N 表示低有效
	PD5/FMC_NWE		-XX- 写控制信号，AD7606 只有读，无写信号
	PD8/FMC_D13
	PD9/FMC_D14
	PD10/FMC_D15
	PD14/FMC_D0
	PD15/FMC_D1

	PE7/FMC_D4
	PE8/FMC_D5
	PE9/FMC_D6
	PE10/FMC_D7
	PE11/FMC_D8
	PE12/FMC_D9
	PE13/FMC_D10
	PE14/FMC_D11
	PE15/FMC_D12
	
	PG0/FMC_A10		--- 和主片选FMC_NE2一起译码
	PG1/FMC_A11		--- 和主片选FMC_NE2一起译码
	PG9/FMC_NE2		--- 主片选（TFT, OLED 和 AD7606）	
*/

/* 
	控制AD7606参数的其他IO分配在扩展的74HC574上
	D13 - AD7606_OS0
	D14 - AD7606_OS1
	D15 - AD7606_OS2
	D24 - AD7606_RESET
	D25 - AD7606_RAGE	
*/
static void AD7606_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能FMC时钟 */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/* 使能 GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	/* 设置 GPIOD 相关的IO为复用推挽输出 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* 设置 GPIOE 相关的IO为复用推挽输出 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* 设置 GPIOG 相关的IO为复用推挽输出 */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* 在bps中需要配置 74HC574输出状态 */
}
#endif
/*
*********************************************************************************************************
*	函 数 名: AD7606_FSMCConfig
*	功能说明: 配置FSMC并口访问时序
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AD7606_FSMCConfig(void)
{
	FMC_NORSRAMInitTypeDef  init;
	FMC_NORSRAMTimingInitTypeDef  timing;

	/*
		AD7606规格书要求(3.3V时)：RD读信号低电平脉冲宽度最短21ns，高电平脉冲最短宽度15ns。

		按照如下配置 读数均正常。为了和同BANK的LCD配置相同，选择3-0-6-1-0-0
		3-0-5-1-0-0  : RD高持续75ns， 低电平持续50ns.  1us以内可读取8路样本数据到内存。
		1-0-1-1-0-0  : RD高75ns，低电平执行12ns左右，下降沿差不多也12ns.  数据读取正确。
	*/
	/* FMC_Bank1_NORSRAM4 configuration */
	timing.FMC_AddressSetupTime = 3;
	timing.FMC_AddressHoldTime = 0;
	timing.FMC_DataSetupTime = 6;
	timing.FMC_BusTurnAroundDuration = 1;
	timing.FMC_CLKDivision = 0;
	timing.FMC_DataLatency = 0;
	timing.FMC_AccessMode = FMC_AccessMode_A;

	/*
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable
	*/
	init.FMC_Bank = FMC_Bank1_NORSRAM4;//FMC_Bank1_NORSRAM2;
	init.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	init.FMC_MemoryType = FMC_MemoryType_SRAM;
	//init.FMC_MemoryType = FMC_MemoryType_PSRAM;
	init.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
	//init.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_32b;
	init.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	init.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	init.FMC_WrapMode = FMC_WrapMode_Disable;
	init.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
	init.FMC_WriteOperation = FMC_WriteOperation_Enable;
	//init.FMC_WriteOperation = FMC_WriteOperation_Disable;
	init.FMC_WaitSignal = FMC_WaitSignal_Disable;
	init.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	init.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;	
	init.FMC_WriteBurst = FMC_WriteBurst_Disable;
	init.FMC_ContinousClock = FMC_CClock_SyncOnly;	/* 429比407多的一个参数 */

	init.FMC_ReadWriteTimingStruct = &timing;
	init.FMC_WriteTimingStruct = &timing;

	FMC_NORSRAMInit(&init);

	/* - BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM4, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_SetInputRange
*	功能说明: 配置AD7606模拟信号输入量程。
*	形    参: _ucRange : 0 表示正负5V   1表示正负10V
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		//g_tAD7606.ucRange = 0;
		RANGE_0();	/* 设置为正负5V */
	}
	else
	{
		//g_tAD7606.ucRange = 1;
		RANGE_1();	/* 设置为正负10V */
	}
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_Reset
*	功能说明: 硬件复位AD7606。复位之后恢复到正常工作状态。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	u8 i;
	RESET_0();	/* 退出复位状态 */

	RESET_1();	/* 进入复位状态 */
	//RESET_1();	/* 仅用于延迟。 RESET复位高电平脉冲宽度最小50ns。 */
	//RESET_1();
	//RESET_1();
	for(i=0;i<20;i++);

	RESET_0();	/* 退出复位状态 */
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_StartConvst
*	功能说明: 启动1次ADC转换
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StartConvst(void)
{
	u8 i;
	/* page 7：  CONVST 高电平脉冲宽度和低电平脉冲宽度最短 25ns */
	/* CONVST平时为高 */
	CONVST_0();
	//CONVST_0();
	//CONVST_0();
	for(i=0;i<10;i++);

	CONVST_1();
}
#if 0
/*
*********************************************************************************************************
*	函 数 名: AD7606_ReadNowAdc
*	功能说明: 读取8路采样结果。结果存储在全局变量 g_tAD7606
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_ReadNowAdc(void)
{
	g_tAD7606.sNowAdc[0] = AD7606_RESULT();	/* 读第1路样本 */
	g_tAD7606.sNowAdc[1] = AD7606_RESULT();	/* 读第2路样本 */
	g_tAD7606.sNowAdc[2] = AD7606_RESULT();	/* 读第3路样本 */
	g_tAD7606.sNowAdc[3] = AD7606_RESULT();	/* 读第4路样本 */
	g_tAD7606.sNowAdc[4] = AD7606_RESULT();	/* 读第5路样本 */
	g_tAD7606.sNowAdc[5] = AD7606_RESULT();	/* 读第6路样本 */
	g_tAD7606.sNowAdc[6] = AD7606_RESULT();	/* 读第7路样本 */
	g_tAD7606.sNowAdc[7] = AD7606_RESULT();	/* 读第8路样本 */
}
#endif
/*
*********************************************************************************************************
*		下面的函数用于定时采集模式。 TIM5硬件定时中断中读取ADC结果，存在全局FIFO
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	函 数 名: AD7606_EnterAutoMode
*	功能说明: 配置硬件工作在自动采集模式，结果存储在FIFO缓冲区。
*	形    参:  _ulFreq : 采样频率，单位Hz，	1k，2k，5k，10k，20K，50k，100k，200k
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_EnterAutoMode(uint32_t _ulFreq)
{
	/* 配置PC6为TIM8_CH1功能，输出占空比50%的方波 */
	bsp_SetTIMOutPWM(GPIOC, GPIO_Pin_6, TIM8, 1, _ulFreq, 5000);

	/* 配置PB1, BUSY 作为中断输入口，下降沿触发 */
	{
		EXTI_InitTypeDef   EXTI_InitStructure;
		//GPIO_InitTypeDef   GPIO_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		/* Enable GPIOE clock */
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		/* Enable SYSCFG clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

		/* Configure PB1 pin as input floating */
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		//GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Connect EXTI Line1 to PB1 pin */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

		/* Configure EXTI Line1 */
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

		//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		/* Enable and set EXTI Line1 Interrupt to the lowest priority */
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}


/*
*********************************************************************************************************
*	函 数 名: AD7606_StartRecord
*	功能说明: 开始采集
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StartRecord(uint32_t _ulFreq)
{
	AD7606_StopRecord();

	AD7606_Reset();					/* 复位硬件 */
	AD7606_StartConvst();			/* 启动采样，避免第1组数据全0的问题 */


	AD7606_EnterAutoMode(_ulFreq);
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_StopRecord
*	功能说明: 停止采集定时器
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StopRecord(void)
{
	TIM_Cmd(TIM8, DISABLE);
#if 0
	/* 将PE5 重新配置为普通输出口 */
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* 使能 GPIO时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
#endif
	
	/* 配置PB1, 禁止 BUSY 作为中断输入口 */
	{
		EXTI_InitTypeDef   EXTI_InitStructure;

		/* Configure EXTI Line5 */
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	CONVST_1();					/* 启动转换的GPIO平时设置为高 */

}
//extern u16 scanV,scanSDV,scanG,scanSDG;
/*
*********************************************************************************************************
*	函 数 名: AD7606_ISR
*	功能说明: 定时采集中断服务程序
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_ISR(void)
{
//	u8 rt;
	uint8_t i;
	int16_t sNowAdc[8];
	//uint16_t bt;
	//ADCParameterSet();
	//TestPinXor();
	
	sNowAdc[7] = AD7606_RESULT();	
	sNowAdc[6] = AD7606_RESULT();
	sNowAdc[5] = AD7606_RESULT();
	sNowAdc[4] = AD7606_RESULT();
	sNowAdc[3] = AD7606_RESULT();
	sNowAdc[2] = AD7606_RESULT();
	sNowAdc[1] = AD7606_RESULT();
	sNowAdc[0] = AD7606_RESULT();

	if(ChannelSelPin()){		
		for(i=0;i<8;i++){		
			ADCFifo->ADCFIFO[i][ADCFifo->pt] = sNowAdc[i];
		}		
		ChannelSelClr();
	}else{
		for(i=0;i<8;i++){		
			ADCFifo->ADCFIFO[i+8][ADCFifo->pt] = sNowAdc[i];
		}
		ADCFifo->pt = FIFO_pointer(ADCFifo->pt,1,ADCFifo->FifoSize);
		ChannelSelSet();	
	}
	uSCount++;
	if(uSCount>=256*50){
		uSCount = 0;
	}
	
	static uint8_t F5msCount,F10msCount,F200msCount,F500msCount;
	
	F5msCount++;
	if(F5msCount>=64){
		F5msCount = 0;
		WholeBit.Bits.F5ms = 1;
		
	}
	F10msCount++;
	if(F10msCount>=128){
		F10msCount = 0;
		WholeBit.Bits.F10ms = 1;
		
		F200msCount++;
		if(F200msCount>=20){
			F200msCount = 0;
			WholeBit.Bits.F200ms = 1;
			
		}
		F500msCount++;
		if(F500msCount>=50){
			F500msCount = 0;
			WholeBit.Bits.F500ms = 1;
			
		}
	}
}//AD7606_ISR

void SaveWave(u16 bt, u16 len)
{
	u16 i,j;
					
	for(j=0;j<len;j++){
		for(i=0;i<16;i++){
			FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt];
		}
		bt = FIFO_pointer(bt,1,ADCFifo->FifoSize);	
	}
	FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
	FaultFifo_pt++;	
	if(FaultFifo_pt >=FaultFifoSIZE){
		FaultFifo_pt = 0;
	}

}//SaveWave




#if 0
/*
*********************************************************************************************************
*	函 数 名: RMS_calculate
*	功能说明: 有效值计算
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
float RMS_calculate(int16_t *RMS_ADCbuf,uint16_t RMS_size,float ratio)
{
	static uint16_t i;
	static float RMS_val,RMSData;
	
	RMS_val = 0;
	for(i=0;i<RMS_size;i++){
		RMSData  = ((float)RMS_ADCbuf[i])/ratio;
		RMS_val += RMSData*RMSData;
	}
	RMS_val /= RMS_size;
	
	RMS_val = sqrt(RMS_val);
	
	return RMS_val;
}//RMS_calculate

#endif

#if 0
/*
*********************************************************************************************************
*	函 数 名: ADCRMS_calculate
*	功能说明: 有效值计算
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADCRMS_calculate(int16_t *buf)
{
	u8 i,j;
	static u8 RMS_i;
	static u8 RMS_Flag = 0x01;

	for(i=0;i<8;i++){
		if(64 == RMS_i){
			RMS_Flag |= 0x02;
		}else if(64 * 2 == RMS_i){
			RMS_Flag |= 0x04;
		}else if(64*3 == RMS_i){
			RMS_Flag |= 0x08;
		}
		for(j=0;j<4;j++){
			if(RMS_Flag & (1<<j)){			
				ADCWholeVar->RMS_Elec[j][i] += buf[i]*buf[i];
			}
		}
		
	}	
	RMS_i++;
	if(0 == RMS_i%64){
		j = RMS_i/64;
		if(RMS_Flag & (1<<j)){
			for(i=0;i<8;i++){				
				ADCWholeVar->RMS_Elec[j][i] /= 256;
				ADCFifo.RMS_buf[i] = sqrt(ADCWholeVar->RMS_Elec[j][i]);
				ADCFifo.RMS_buf[i] /= ADCFifo.rati0[i];
				ADCWholeVar->RMS_Elec[j][i] = 0;				
			}
			return 0;
		}
	}

	return 1;
}
#endif




#if 1
/*
*********************************************************************************************************
*	函 数 名: ADCVerification
*	功能说明: 校验
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADCVerification(void)
{
	
	FFT_VerifyDisplay();
	memcpy(PoramData.ADCParameter.fftratio,ADCFifo->fftratio,16*sizeof(float));
	PoramFlag++;
} //ADCVerification

#endif

