/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	�ó��������ڰ�����STM32-X4��STM32-F4������

	�����������Ӳ�������޸�GPIO����� IsKeyDown1 - IsKeyDown8 ����

	����û���LEDָʾ�Ƹ���С��4�������Խ������LEDȫ������Ϊ�͵�1��LEDһ��������Ӱ�������
*/


	/*
		������STM32-X4 LED���߷��䣺
			LED1       : PE2 (�͵�ƽ�������ߵ�ƽϨ��)
			LED2       : PE3 (�͵�ƽ�������ߵ�ƽϨ��)
			
	*/
	#define RCC_ALL_LED 	RCC_AHB1Periph_GPIOE	/* �����ڶ�Ӧ��RCCʱ�� */

	#define GPIO_PORT_LED1  GPIOE
	#define GPIO_PIN_LED1	GPIO_Pin_2

	#define GPIO_PORT_LED2  GPIOE
	#define GPIO_PIN_LED2	GPIO_Pin_3	


	

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
	bsp_LedOff(0);
	bsp_LedOff(1);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{

	if (_no == 0)
	{
		GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
		
	}
	else if (_no == 1)
	{
		GPIO_ResetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
		
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{


	if (_no == 0)
	{
		GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);		
	}
	else if (_no == 1)
	{
		GPIO_SetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);		
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 0)
	{
		GPIO_ToggleBits(GPIO_PORT_LED1, GPIO_PIN_LED1);			
	}
	else if (_no == 1)
	{
		GPIO_ToggleBits(GPIO_PORT_LED2, GPIO_PIN_LED2);			
	}
	
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
