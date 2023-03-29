/*
*********************************************************************************************************
*
*	ģ������ : AD7606���ݲɼ�ģ��
*	�ļ����� : bsp_ad7606.c
*	��    �� : V1.0
*	˵    �� : AD7606����STM32��FMC�����ϡ�
*
*			������ʹ���� TIM3 ��ΪӲ����ʱ������ʱ����ADCת��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2017-11-16 JIM KATE  ��ʽ����
*
*
*********************************************************************************************************
*/


#include "bsp.h"
//#include "param.h"

/*
	AD7606ģ�飬 ���Ʋɼ���IO:
	
	PC6/TIM3_CH1/TIM8_CH1     ----> AD7606_CONVST  ���PWM��������ΪADC�����ź�
	PB1/AD7606_BUSY           <---- AD7606_BUSY    , CPU��BUSY�жϷ�������ж�ȡ�ɼ����
	
	�������IO��STM32-V5�������ǲ�ͬ��
*/

/* ����ADת����GPIO : PC6 */
#define CONVST_1()	GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define CONVST_0()	GPIO_ResetBits(GPIOC,GPIO_Pin_6)

/* �����������̵�GPIO: PG10 */
#define RANGE_1()	GPIO_SetBits(GPIOG,GPIO_Pin_10)
#define RANGE_0()	GPIO_ResetBits(GPIOG,GPIO_Pin_10)

/* AD7606��λ����, PG7*/
#define RESET_1()	GPIO_SetBits(GPIOG,GPIO_Pin_7)
#define RESET_0()	GPIO_ResetBits(GPIOG,GPIO_Pin_7)

/* AD7606 FSMC���ߵ�ַ��ֻ�ܶ�������д */
#define AD7606_RESULT()	*(__IO uint16_t *)0x6C000000

//AD7606_VAR_T g_tAD7606;		/* ����1��ȫ�ֱ���������һЩ���� */

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
*	�� �� ��: bsp_InitAD7606
*	����˵��: ���������ⲿSRAM��GPIO��FSMC
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitAD7606(void)
{
	//AD7606_CtrlLinesConfig();
	AD7606_FSMCConfig();
	
	AD7606_SetInputRange(1);	/* 0��ʾ��������Ϊ����5V, 1��ʾ����10V */

	AD7606_Reset();

	CONVST_1();					/* ����ת����GPIOƽʱ����Ϊ�� */
	ChannelSelSet();
	//ADCParameterSet();
}


#if 0
/*
*********************************************************************************************************
*	�� �� ��: AD7606_CtrlLinesConfig
*	����˵��: ����GPIO���ߣ�FMC�ܽ�����Ϊ���ù���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
/*
	������STM32-V6��������߷�����
	PD0/FMC_D2
	PD1/FMC_D3
	PD4/FMC_NOE		---- �������źţ�OE = Output Enable �� N ��ʾ����Ч
	PD5/FMC_NWE		-XX- д�����źţ�AD7606 ֻ�ж�����д�ź�
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
	
	PG0/FMC_A10		--- ����ƬѡFMC_NE2һ������
	PG1/FMC_A11		--- ����ƬѡFMC_NE2һ������
	PG9/FMC_NE2		--- ��Ƭѡ��TFT, OLED �� AD7606��	
*/

/* 
	����AD7606����������IO��������չ��74HC574��
	D13 - AD7606_OS0
	D14 - AD7606_OS1
	D15 - AD7606_OS2
	D24 - AD7606_RESET
	D25 - AD7606_RAGE	
*/
static void AD7606_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��FMCʱ�� */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/* ʹ�� GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	/* ���� GPIOD ��ص�IOΪ����������� */
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

	/* ���� GPIOE ��ص�IOΪ����������� */
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

	/* ���� GPIOG ��ص�IOΪ����������� */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* ��bps����Ҫ���� 74HC574���״̬ */
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: AD7606_FSMCConfig
*	����˵��: ����FSMC���ڷ���ʱ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD7606_FSMCConfig(void)
{
	FMC_NORSRAMInitTypeDef  init;
	FMC_NORSRAMTimingInitTypeDef  timing;

	/*
		AD7606�����Ҫ��(3.3Vʱ)��RD���źŵ͵�ƽ���������21ns���ߵ�ƽ������̿��15ns��

		������������ ������������Ϊ�˺�ͬBANK��LCD������ͬ��ѡ��3-0-6-1-0-0
		3-0-5-1-0-0  : RD�߳���75ns�� �͵�ƽ����50ns.  1us���ڿɶ�ȡ8·�������ݵ��ڴ档
		1-0-1-1-0-0  : RD��75ns���͵�ƽִ��12ns���ң��½��ز��Ҳ12ns.  ���ݶ�ȡ��ȷ��
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
	init.FMC_ContinousClock = FMC_CClock_SyncOnly;	/* 429��407���һ������ */

	init.FMC_ReadWriteTimingStruct = &timing;
	init.FMC_WriteTimingStruct = &timing;

	FMC_NORSRAMInit(&init);

	/* - BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM4, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_SetInputRange
*	����˵��: ����AD7606ģ���ź��������̡�
*	��    ��: _ucRange : 0 ��ʾ����5V   1��ʾ����10V
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		//g_tAD7606.ucRange = 0;
		RANGE_0();	/* ����Ϊ����5V */
	}
	else
	{
		//g_tAD7606.ucRange = 1;
		RANGE_1();	/* ����Ϊ����10V */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_Reset
*	����˵��: Ӳ����λAD7606����λ֮��ָ�����������״̬��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	u8 i;
	RESET_0();	/* �˳���λ״̬ */

	RESET_1();	/* ���븴λ״̬ */
	//RESET_1();	/* �������ӳ١� RESET��λ�ߵ�ƽ��������С50ns�� */
	//RESET_1();
	//RESET_1();
	for(i=0;i<20;i++);

	RESET_0();	/* �˳���λ״̬ */
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_StartConvst
*	����˵��: ����1��ADCת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartConvst(void)
{
	u8 i;
	/* page 7��  CONVST �ߵ�ƽ�����Ⱥ͵͵�ƽ��������� 25ns */
	/* CONVSTƽʱΪ�� */
	CONVST_0();
	//CONVST_0();
	//CONVST_0();
	for(i=0;i<10;i++);

	CONVST_1();
}
#if 0
/*
*********************************************************************************************************
*	�� �� ��: AD7606_ReadNowAdc
*	����˵��: ��ȡ8·�������������洢��ȫ�ֱ��� g_tAD7606
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_ReadNowAdc(void)
{
	g_tAD7606.sNowAdc[0] = AD7606_RESULT();	/* ����1·���� */
	g_tAD7606.sNowAdc[1] = AD7606_RESULT();	/* ����2·���� */
	g_tAD7606.sNowAdc[2] = AD7606_RESULT();	/* ����3·���� */
	g_tAD7606.sNowAdc[3] = AD7606_RESULT();	/* ����4·���� */
	g_tAD7606.sNowAdc[4] = AD7606_RESULT();	/* ����5·���� */
	g_tAD7606.sNowAdc[5] = AD7606_RESULT();	/* ����6·���� */
	g_tAD7606.sNowAdc[6] = AD7606_RESULT();	/* ����7·���� */
	g_tAD7606.sNowAdc[7] = AD7606_RESULT();	/* ����8·���� */
}
#endif
/*
*********************************************************************************************************
*		����ĺ������ڶ�ʱ�ɼ�ģʽ�� TIM5Ӳ����ʱ�ж��ж�ȡADC���������ȫ��FIFO
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	�� �� ��: AD7606_EnterAutoMode
*	����˵��: ����Ӳ���������Զ��ɼ�ģʽ������洢��FIFO��������
*	��    ��:  _ulFreq : ����Ƶ�ʣ���λHz��	1k��2k��5k��10k��20K��50k��100k��200k
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_EnterAutoMode(uint32_t _ulFreq)
{
	/* ����PC6ΪTIM8_CH1���ܣ����ռ�ձ�50%�ķ��� */
	bsp_SetTIMOutPWM(GPIOC, GPIO_Pin_6, TIM8, 1, _ulFreq, 5000);

	/* ����PB1, BUSY ��Ϊ�ж�����ڣ��½��ش��� */
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
*	�� �� ��: AD7606_StartRecord
*	����˵��: ��ʼ�ɼ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartRecord(uint32_t _ulFreq)
{
	AD7606_StopRecord();

	AD7606_Reset();					/* ��λӲ�� */
	AD7606_StartConvst();			/* ���������������1������ȫ0������ */


	AD7606_EnterAutoMode(_ulFreq);
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_StopRecord
*	����˵��: ֹͣ�ɼ���ʱ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StopRecord(void)
{
	TIM_Cmd(TIM8, DISABLE);
#if 0
	/* ��PE5 ��������Ϊ��ͨ����� */
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* ʹ�� GPIOʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
#endif
	
	/* ����PB1, ��ֹ BUSY ��Ϊ�ж������ */
	{
		EXTI_InitTypeDef   EXTI_InitStructure;

		/* Configure EXTI Line5 */
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	CONVST_1();					/* ����ת����GPIOƽʱ����Ϊ�� */

}
//extern u16 scanV,scanSDV,scanG,scanSDG;
/*
*********************************************************************************************************
*	�� �� ��: AD7606_ISR
*	����˵��: ��ʱ�ɼ��жϷ������
*	��    ��:  ��
*	�� �� ֵ: ��
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
*	�� �� ��: RMS_calculate
*	����˵��: ��Чֵ����
*	��    ��:  ��
*	�� �� ֵ: ��
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
*	�� �� ��: ADCRMS_calculate
*	����˵��: ��Чֵ����
*	��    ��:  ��
*	�� �� ֵ: ��
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
*	�� �� ��: ADCVerification
*	����˵��: У��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADCVerification(void)
{
	
	FFT_VerifyDisplay();
	memcpy(PoramData.ADCParameter.fftratio,ADCFifo->fftratio,16*sizeof(float));
	PoramFlag++;
} //ADCVerification

#endif

