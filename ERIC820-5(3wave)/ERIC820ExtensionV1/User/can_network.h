/*
*********************************************************************************************************
*	                                  
*	ģ������ : CAN������ʾ����
*	�ļ����� : can_network.h
*	��    �� : V1.3
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��  ����       ����      ˵��
*		v1.3    2015-01-29 Eric2013  �׷�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _CAN_NETWORK_H
#define _CAN_NETWORK_H

#define iCanMainID	0x300
#define iCanSetID	0x3F1

extern uint16_t CanDelayTim;


/* ���ⲿ���õĺ������� */
void can_Init(void);
void can_NVIC_Config(void);
uint8_t SendCanMsg(uint16_t iddata,uint8_t *buf,uint8_t buf_len);
uint8_t SendCanbuf(uint16_t iddata,uint8_t *buf,uint16_t buf_len);
void can_SetTask(void);
void can_Task(void);
void CanBreath(void);
void CanSendWave(void);


#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
