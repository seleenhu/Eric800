/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For STM32F4XX)
*	�ļ����� : bsp.c
*	��    �� : V1.1
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-03-01  armfly   ��ʽ����
*		V1.1    2013-06-20  armfly   �淶ע�ͣ���ӱ�Ҫ˵��
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "MainTask.h"



/*
*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{
	u8 i;
	/*
	����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
	�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

	ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
	*/
	/* ʹ��CRC ��Ϊʹ��STemWinǰ����Ҫʹ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	/* ���ȼ���������Ϊ4��������0-15����ռʽ���ȼ���0�������ȼ����������������ȼ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	SystemCoreClockUpdate();	/* ����PLL���ø���ϵͳʱ��Ƶ�ʱ��� SystemCoreClock */
	
	bsp_InitDWT();
	
	bsp_InitExtIO(); 
	
	bsp_InitUart(); 	/* ��ʼ������ */
	//bsp_InitKey();		/* ��ʼ������������������ bsp_InitTimer() ֮ǰ���ã� */

	bsp_InitRTC();
	   
	//bsp_InitLed(); 		/* ��ʼLEDָʾ�ƶ˿� */

	bsp_InitI2C();		/* ����I2C���� */
	bsp_InitTimer();

	bsp_InitExtSDRAM();      /* ��ʼ��SDRAM */
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
	
	bsp_DetectLcdType();	/* ��ⴥ�����LCD����ͺ�, �������ȫ�ֱ��� g_TouchType, g_LcdType */	
	TOUCH_InitHard();		/* ��ʼ�����ô���оƬ */
	LCD_ConfigLTDC();       /* ��ʼ������LTDC */
	
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
*	�� �� ��: bsp_RunPer10ms
*	����˵��: �ú���ÿ��10ms��Systick�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������һЩ����ʱ��Ҫ���ϸ��
*			������Է��ڴ˺��������磺����ɨ�衢���������п��Ƶȡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	//bsp_KeyScan();		/* ����ɨ�� */
	
	/* ���ݴ�����GT811 */
	if(g_GT811.Enable == 1)
	{
		GT811_OnePiontScan();
	}

	/* ���败����FT5X06 */
	if(g_tFT5X06.Enable == 1)
	{
		FT5X06_OnePiontScan();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RunPer1ms
*	����˵��: �ú���ÿ��1ms��Systick�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ��������
*			 ���Է��ڴ˺��������磺��������ɨ�衣
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
	/* ���败���� */
	if(g_tTP.Enable == 1)
	{
		TOUCH_Scan();	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Idle
*	����˵��: ����ʱִ�еĺ�����һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
*			 ������ȱʡΪ�ղ������û��������ι��������CPU��������ģʽ�Ĺ��ܡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- ι�� */

	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */
	//GUI_Exec();

	/* ���� uIP Э�飬���Բ���uip��ѯ���� */

}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
