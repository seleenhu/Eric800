/*
*********************************************************************************************************
*
*	模块名称 : AD7606数据采集模块
*	文件名称 : bsp_ad7606.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_AD7606_H
#define _BSP_AD7606_H

typedef struct
{
	//uint8_t ucOS;			/* 过采样倍率，0 - 6. 0表示无过采样 */
	uint8_t ucRange;		/* 输入量程，0表示正负5V, 1表示正负10V */
	int16_t sNowAdc[8];		/* 当前ADC值, 有符号数 */
}AD7606_VAR_T;



void bsp_InitAD7606(void);
void AD7606_SetInputRange(uint8_t _ucRange);
void AD7606_Reset(void);
void AD7606_StartConvst(void);
void AD7606_ReadNowAdc(void);

/* 下面的函数用于FIFO操作模式 */
void AD7606_EnterAutoMode(uint32_t _ulFreq);
void AD7606_StartRecord(uint32_t _ulFreq);
void AD7606_StopRecord(void);


/* 全局变量 */


void AD7606_ISR(void);
void ADCVerification(void);
void SaveWave(u16 bt, u16 len);
float RMS_calculate(int16_t *RMS_ADCbuf,uint16_t RMS_size,float ratio);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
