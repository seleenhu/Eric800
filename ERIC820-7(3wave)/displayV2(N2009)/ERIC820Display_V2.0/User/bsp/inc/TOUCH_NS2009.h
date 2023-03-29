/*
*********************************************************************************************************
*
	模块名称 : 电阻式触摸芯片NS2009驱动模块
	文件名称 : TOUCH_NS2009,h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _TOUCH_NS2009_H
#define _TOUCH_NS2009_H



/* I2C总线地址  0x90 */
#define NS2009_I2C_ADDRESS	0x90


extern uint8_t NS2009InitFlag;

/* 可供外部模块调用的函数 */

void NS2009_InitHard(void);


uint8_t NS2009_ReadBytes(uint8_t *_pReadBuf, uint8_t _ucAddress, uint8_t _ucSize);
uint8_t NS2009_WriteBytes(uint8_t *_pWriteBuf, uint8_t _ucAddress, uint8_t _ucSize);



uint16_t NS2009_ReadX(void);
uint16_t NS2009_ReadY(void);



uint8_t NS2009_PenInt(void);
void NS2009_ClearInt(void);

#endif
