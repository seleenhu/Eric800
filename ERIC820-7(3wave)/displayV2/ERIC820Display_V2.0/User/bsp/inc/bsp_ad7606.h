/*
*********************************************************************************************************
*
*	ģ������ : AD7606���ݲɼ�ģ��
*	�ļ����� : bsp_ad7606.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_AD7606_H
#define _BSP_AD7606_H

typedef struct
{
	//uint8_t ucOS;			/* ���������ʣ�0 - 6. 0��ʾ�޹����� */
	uint8_t ucRange;		/* �������̣�0��ʾ����5V, 1��ʾ����10V */
	int16_t sNowAdc[8];		/* ��ǰADCֵ, �з����� */
}AD7606_VAR_T;



void bsp_InitAD7606(void);
void AD7606_SetInputRange(uint8_t _ucRange);
void AD7606_Reset(void);
void AD7606_StartConvst(void);
void AD7606_ReadNowAdc(void);

/* ����ĺ�������FIFO����ģʽ */
void AD7606_EnterAutoMode(uint32_t _ulFreq);
void AD7606_StartRecord(uint32_t _ulFreq);
void AD7606_StopRecord(void);


/* ȫ�ֱ��� */


void AD7606_ISR(void);
void ADCVerification(void);
void SaveWave(u16 bt, u16 len);
float RMS_calculate(int16_t *RMS_ADCbuf,uint16_t RMS_size,float ratio);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
