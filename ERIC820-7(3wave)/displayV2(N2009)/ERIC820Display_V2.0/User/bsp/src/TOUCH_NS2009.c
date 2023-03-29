/*
*********************************************************************************************************
*
	模块名称 : 电阻式触摸芯片NS2009驱动模块
	文件名称 : TOUCH_NS2009.c
*	版    本 : V1.0
*	说    明 : NS2009 电阻触摸屏驱动程序。I2C接口.
*	修改记录 :
*		版本号  日期        作者    说明
*		V1.0    2021-04-02  Seleen  首发。Eric。
*
*	Copyright (C), 2021-2022, www.Eric.com
*
*********************************************************************************************************
*/

/*
	应用说明：访问STMPE811前，请先调用一次 bsp_InitI2C()函数配置好I2C相关的GPIO.
*/

#include "bsp.h"

#define touch_printf(...)
//#define touch_printf  printf

/* 定义GPIO端口 */
#define RCC_TP_INT	RCC_AHB1Periph_GPIOH
#define PORT_TP_INT	GPIOH
#define PIN_TP_INT	GPIO_Pin_7
	
//#define STMPE811

#define ReadX		0xC0     
#define ReadY		0xD0  

uint8_t NS2009InitFlag;

static uint16_t s_AdcX, s_AdcY;

/*
*********************************************************************************************************
*	函 数 名: NS2009_InitHard
*	功能说明: 配置STMPE811寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NS2009_InitHard(void)
{
	uint8_t icdata[2];
	/* 配置触摸笔中断引脚为输入 */
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* 第1步：打开GPIO时钟 */
		RCC_AHB1PeriphClockCmd(RCC_TP_INT, ENABLE);

		/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
		//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 无需上下拉电阻 */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

		GPIO_InitStructure.GPIO_Pin = PIN_TP_INT;
		GPIO_Init(PORT_TP_INT, &GPIO_InitStructure);		
	}	
	

		NS2009_ReadBytes(icdata,ReadX,2); //初始化NS2009 ;主要是使能中断
		
}

/*
*********************************************************************************************************
*	函 数 名: NS2009_PenInt
*	功能说明: 判断触摸按下
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t NS2009_PenInt(void)
{
	if ((PORT_TP_INT->IDR & PIN_TP_INT) == 0)
	{
		NS2009InitFlag=1;
		return 1;
	}
	
	return 0;
	
}

/*
*********************************************************************************************************
*	函 数 名: NS2009_ClearInt
*	功能说明: 清楚触笔中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NS2009_ClearInt(void)
{
	NS2009InitFlag=0;
}



/*
*********************************************************************************************************
*	函 数 名: NS2009_ReadBytes
*	功能说明: 连续读取若干数据
*	形    参: 
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*			 _ucAddress : 起始地址
*			 _ucSize : 数据长度，单位为字节
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t NS2009_ReadBytes(uint8_t *_pReadBuf, uint8_t _ucAddress, uint8_t _ucSize)
{
	uint8_t i;

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(NS2009_I2C_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送寄存器地址 */
	i2c_SendByte((uint8_t)_ucAddress);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(NS2009_I2C_ADDRESS | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：循环读取数据 */
	for (i = 0; i < _ucSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */

		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _ucSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: NS2009_WriteBytes
*	功能说明: 连续写入若干数据
*	形    参: 
*			 _pWriteBuf : 将要写入到芯片的数据的缓冲区指针
*			 _ucAddress : 起始地址
*			 _ucSize : 数据长度，单位为字节
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t NS2009_WriteBytes(uint8_t *_pWriteBuf, uint8_t _ucAddress, uint8_t _ucSize)
{
	uint8_t i;

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(NS2009_I2C_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送寄存器地址 */
	i2c_SendByte((uint8_t)_ucAddress);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第5步：循环写数据 */
	for (i = 0; i < _ucSize; i++)
	{
		i2c_SendByte(_pWriteBuf[i]);
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: STMPE811_WriteReg1
*	功能说明: 写1个字节到寄存器
*	形    参: _ucRegAddr : 寄存器地址
*			 _ucValue    : 寄存器值
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
void NS2009_WriteReg1(uint8_t _ucRegAddr, uint8_t _ucValue)
{
	NS2009_WriteBytes(&_ucValue, _ucRegAddr, 1);
}

/*
*********************************************************************************************************
*	函 数 名: STMPE811_WriteReg1
*	功能说明: 写2个字节到寄存器
*	形    参: _ucRegAddr : 寄存器地址
*			 _ucValue    : 寄存器值
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
void NS2009_WriteReg2(uint8_t _ucRegAddr, uint16_t _usValue)
{
	uint8_t buf[2];
	
	buf[0] = _usValue >> 8;
	buf[1] = _usValue;
	NS2009_WriteBytes(buf, _ucRegAddr, 2);
}

/*
*********************************************************************************************************
*	函 数 名: STMPE811_ReadReg1
*	功能说明: 读寄存器1字节
*	形    参: _ucRegAddr : 寄存器地址
*			 _ucValue    : 寄存器值
*	返 回 值: 寄存器值
*********************************************************************************************************
*/
uint8_t NS2009_ReadReg1(uint8_t _ucRegAddr)
{
	uint8_t read;
	
	NS2009_ReadBytes(&read, _ucRegAddr, 1);
	
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: STMPE811_ReadReg2
*	功能说明: 读2个字节
*	形    参: _ucRegAddr : 寄存器地址
*	返 回 值: 寄存器值
*********************************************************************************************************
*/
uint16_t NS2009_ReadReg2(uint8_t _ucRegAddr)
{
	uint8_t buf[2];
	
	NS2009_ReadBytes(buf, _ucRegAddr, 2);
	
	return (((uint16_t)buf[0] << 8) | buf[1]);
}

/*
*********************************************************************************************************
*	函 数 名: NS2009_ReadX
*	功能说明: 读取X坐标adc
*	形    参: 无
*	返 回 值: X坐标值adc
*********************************************************************************************************
*/
uint16_t NS2009_ReadX(void)
{
	uint8_t icdata[2];
	
	NS2009_ReadBytes(icdata,ReadX,2);
	s_AdcX =(icdata[0]<<4 | icdata[1]>>4); 
	
	return s_AdcX;
}


/*
*********************************************************************************************************
*	函 数 名: NS2009_ReadY
*	功能说明: 读取X坐标adc
*	形    参: 无
*	返 回 值: X坐标值adc
*********************************************************************************************************
*/
uint16_t NS2009_ReadY(void)
{
	uint8_t icdata[2];
	
	NS2009_ReadBytes(icdata,ReadY,2);
	s_AdcY =(icdata[0]<<4 | icdata[1]>>4); 
	return s_AdcY;
}






/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
