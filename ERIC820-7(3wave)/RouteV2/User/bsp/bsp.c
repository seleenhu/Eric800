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

//uint32_t testadr = 19*128*1024;
//uint8_t  test_rt;
//uint32_t testbuf[512];
//FATFS fs;

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

	/* ���ȼ���������Ϊ4��������0-3����ռʽ���ȼ���0-3�������ȼ����������������ȼ���*/
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
	
	for(i=0;i<FaultFifoSIZE;i++){
		FaultFifo[i] = (FaultSaveStruct *)(FaultFifo_SDRAM + i*4*(sizeof(FaultSaveStruct)/4 +1));
	}
	FaultFifo_pt = 0;	
	
	bsp_InitAD7606();
	bsp_InitSFlash();
	sf_ReadInfo();
	ReadParam(PARAM_VER);
	//InitRF433();
	//bsp_InitSyn6288();

	//LoadParam();
	//InitGUISystem();
	
	//bsp_DetectLcdType();	/* ��ⴥ�����LCD����ͺ�, �������ȫ�ֱ��� g_TouchType, g_LcdType */	
	//TOUCH_InitHard();		/* ��ʼ�����ô���оƬ */
	//LCD_ConfigLTDC();       /* ��ʼ������LTDC */
	
	//MemInit();

	//Init4G();

	//bsp_InitRS485();
	
	//Earth_LED_OFF();
	//Com_LED_OFF();
	//OverVoltage_LED_OFF();
	//OverTem_LED_OFF();
	//Decompression_LED_OFF();
	//Fault_LED_OFF();
	//PutOP_LED_OFF();

	//PhaseAStateOFF();
	//PhaseBStateOFF();
	//PhaseCStateOFF();
	//SwitchAOFF();	
	//SwitchBOFF();
	//SwitchCOFF();

	//BlackOutON();
	
	ModuleComInit();
	//while(0 != InitSaveNand());
	
	
#if	0
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL fp;
	DIR DirInf;

	//sf_EraseChip();
	

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, FS_VOLUME_SPIFLASH, 1);
	result = f_mkfs(FS_VOLUME_SPIFLASH, 0, 0);
	result = f_opendir(&DirInf, FS_VOLUME_SPIFLASH);
	result = f_open(&fp,"1:/test.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	f_close(&fp);

	
	//NAND_Init();
	//NAND_Format();
	//result = f_mkfs(FS_VOLUME_NAND, 0, 0);
	//NAND_Format();
	//NAND_Init();
	result = f_opendir(&DirInf, FS_VOLUME_SPIFLASH);
	result = f_open(&fp,"1:/test.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	

#endif	
	
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
