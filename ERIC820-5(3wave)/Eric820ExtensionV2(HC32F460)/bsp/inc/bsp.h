/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
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

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
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





/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
