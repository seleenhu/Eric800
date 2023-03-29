/*
*********************************************************************************************************
*
	ģ������ : ����ʽ����оƬNS2009����ģ��
	�ļ����� : TOUCH_NS2009,h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _TOUCH_NS2009_H
#define _TOUCH_NS2009_H



/* I2C���ߵ�ַ  0x90 */
#define NS2009_I2C_ADDRESS	0x90


extern uint8_t NS2009InitFlag;

/* �ɹ��ⲿģ����õĺ��� */

void NS2009_InitHard(void);


uint8_t NS2009_ReadBytes(uint8_t *_pReadBuf, uint8_t _ucAddress, uint8_t _ucSize);
uint8_t NS2009_WriteBytes(uint8_t *_pWriteBuf, uint8_t _ucAddress, uint8_t _ucSize);



uint16_t NS2009_ReadX(void);
uint16_t NS2009_ReadY(void);



uint8_t NS2009_PenInt(void);
void NS2009_ClearInt(void);

#endif
