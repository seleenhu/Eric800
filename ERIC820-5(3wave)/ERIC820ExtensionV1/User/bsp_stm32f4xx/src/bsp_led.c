/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	该程序适用于安富莱STM32-X4、STM32-F4开发板

	如果用于其它硬件，请修改GPIO定义和 IsKeyDown1 - IsKeyDown8 函数

	如果用户的LED指示灯个数小于4个，可以将多余的LED全部定义为和第1个LED一样，并不影响程序功能
*/


	/*
		安富莱STM32-X4 LED口线分配：
			LED1       : PE2 (低电平点亮，高电平熄灭)
			LED2       : PE3 (低电平点亮，高电平熄灭)
			
	*/
	#define RCC_ALL_LED 	RCC_AHB1Periph_GPIOE	/* 按键口对应的RCC时钟 */

	#define GPIO_PORT_LED1  GPIOE
	#define GPIO_PIN_LED1	GPIO_Pin_2

	#define GPIO_PORT_LED2  GPIOE
	#define GPIO_PIN_LED2	GPIO_Pin_3	


	

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
	bsp_LedOff(0);
	bsp_LedOff(1);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{

	if (_no == 0)
	{
		GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
		
	}
	else if (_no == 1)
	{
		GPIO_ResetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
		
	}	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{


	if (_no == 0)
	{
		GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);		
	}
	else if (_no == 1)
	{
		GPIO_SetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);		
	}	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 0)
	{
		GPIO_ToggleBits(GPIO_PORT_LED1, GPIO_PIN_LED1);			
	}
	else if (_no == 1)
	{
		GPIO_ToggleBits(GPIO_PORT_LED2, GPIO_PIN_LED2);			
	}
	
}



/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
