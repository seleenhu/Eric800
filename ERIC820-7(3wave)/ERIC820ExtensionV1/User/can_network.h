/*
*********************************************************************************************************
*	                                  
*	模块名称 : CAN网络演示程序。
*	文件名称 : can_network.h
*	版    本 : V1.3
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者      说明
*		v1.3    2015-01-29 Eric2013  首发
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _CAN_NETWORK_H
#define _CAN_NETWORK_H

#define iCanMainID	0x300
#define iCanSetID	0x3F1

extern uint16_t CanDelayTim;


/* 供外部调用的函数声明 */
void can_Init(void);
void can_NVIC_Config(void);
uint8_t SendCanMsg(uint16_t iddata,uint8_t *buf,uint8_t buf_len);
uint8_t SendCanbuf(uint16_t iddata,uint8_t *buf,uint16_t buf_len);
void can_SetTask(void);
void can_Task(void);
void CanBreath(void);
void CanSendWave(void);


#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
