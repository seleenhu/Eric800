/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H


#include "hc32_ddl.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define u8 	uint8_t 
#define u16 uint16_t
#define u32 uint32_t

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* ͨ��ȡ��ע�ͻ�������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
//#include "bsp_uart_fifo.h"
//#include "bsp_led.h"
//#include "bsp_timer.h"
//#include "bsp_key.h"


#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_rtc.h"
#include "bsp_can_network.h"
#include "arithmetic.h"
#include "mydefine.h"





/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/