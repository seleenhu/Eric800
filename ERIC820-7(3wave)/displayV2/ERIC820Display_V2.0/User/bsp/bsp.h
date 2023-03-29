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

#define STM32_V5    /* V5��V6������һ���ģ���������ʹ�ܺ궨��STM32_V5���� */
//#define STM32_X3

/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_V5) && !defined (STM32_X3)
	#error "Please define the board model : STM32_X3 or STM32_V5"
#endif

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include  <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  <math.h>
#include  <ctype.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/*
	EXTI9_5_IRQHandler ���жϷ�������ɢ�ڼ��������� bsp�ļ��С�
	��Ҫ���ϵ� stm32f4xx_it.c �С�

	���������б�ʾEXTI9_5_IRQHandler��ں������зŵ� stm32f4xx_it.c��
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1	/* ��ӡGPS���ݵ�����1 */

/* ͨ��ȡ��ע�ͻ�������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_ext_io.h"
#include "bsp_uart_fifo.h"
//#include "bsp_led.h"
#include "bsp_timer.h"
//#include "bsp_key.h"
#include "bsp_dwt.h"
#include "bsp_rtc.h"
//#include "bsp_msg.h"

#include "bsp_tim_pwm.h"

//#include "bsp_cpu_flash.h"
#include "bsp_sdio_sd.h"
#include "bsp_i2c_gpio.h"
//#include "bsp_eeprom_24xx.h"
//#include "bsp_si4730.h"
//#include "bsp_hmc5883l.h"
//#include "bsp_mpu6050.h"
//#include "bsp_bh1750.h"
//#include "bsp_bmp085.h"
//#include "bsp_wm8978.h"
#include "bsp_gt811.h"

#include "bsp_fmc_sdram.h"
//#include "bsp_nand_flash.h"

#include "bsp_tft_429.h"
#include "bsp_tft_lcd.h"

#include "bsp_touch.h"
#include "TOUCH_STMPE811.h"
#include "bsp_ts_ft5x06.h"

//#include "bsp_camera.h"

//#include "bsp_ad7606.h"

//#include "bsp_gps.h"
//#include "bsp_oled.h"
////#include "bsp_mg323.h"
//#include "bsp_sim800.h"

#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
//#include "bsp_tm7705.h"
//#include "bsp_ads1256.h"
//#include "bsp_vs1053b.h"

//#include "bsp_ds18b20.h"
//#include "bsp_dac8501.h"
//#include "bsp_dht11.h"

//#include "bsp_ir_decode.h"
//#include "bsp_ps2.h"

//#include "bsp_modbus.h"
//#include "bsp_rs485_led.h"
//#include "bsp_user_lib.h"

//#include "bsp_dac8501.h"
//#include "bsp_dac8562.h"
//#include "bsp_beep.h"

//#include "bsp_adc_dso.h"
//#include "bsp_dac_wave.h"

//#include "bsp_esp8266.h"
//#include "bsp_ad9833.h"
//#include "bsp_fi.h"
//#include "arithmetic.h"

#include "bsp_RS485.h"
#include "bsp_rf433.h"
#include "bsp_UIS891X.h"
#include "bsp_4G.h"
#include "bsp_syn6288.h"

#include "diskio.h"
#include "ff.h"
#include "bsp_InOut.h"
#include "mydefine.h"
#include "param.h"
#include "LCDConf_Lin_Template.h"
#include "GUI.h"
#include "font.h"
#include "bsp_buzzer.h"
#include "bsp_moduletomodule.h"

#include "w5500_config.h"
#include "socket.h"
#include "W5500.h"
#include "utility.h"

//#define  TestOutput

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);
void bsp_RunPer1ms(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/