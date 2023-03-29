/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.c
*	版    本 : V1.0
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2013-02-01 armfly  正式发布
*		V1.1    2013-06-09 armfly  FiFo结构增加TxCount成员变量，方便判断缓冲区满; 增加 清FiFo的函数
*		V1.2	2014-09-29 armfly  增加RS485 MODBUS接口。接收到新字节后，直接执行回调函数。
*		V1.3	2015-07-23 armfly  增加 UART_T 结构的读写指针几个成员变量必须增加 __IO 修饰,否则优化后
*					会导致串口发送函数死机。
*		V1.4	2015-08-04 armfly  解决UART4配置bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  增加修改波特率的接口函数
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 串口1的GPIO --- RS323 */
#define UART1_TX_PORT      GPIOA
#define UART1_TX_PIN       GPIO_Pin_9
#define UART1_TX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_TX_SOURCE    GPIO_PinSource9

#define UART1_RX_PORT      GPIOA
#define UART1_RX_PIN       GPIO_Pin_10
#define UART1_RX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_RX_SOURCE    GPIO_PinSource10









static void InitHardUart(void);

#define RS232bufSize   512
uint8_t RS232RXbuf[RS232bufSize];
uint16_t RS232bt;
#define  RS232Pt     (RS232bufSize-DMA_GetCurrDataCounter(DMA2_Stream5))

uint8_t RS232TXbuf[RS232bufSize];
uint16_t RS232TXbuflen;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	//UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */

	InitHardUart();		/* 配置串口的硬件参数(波特率等) */

	//RS485_InitTXE();	/* 配置RS485芯片的发送使能硬件，配置为推挽输出 */

	//ConfigUartNVIC();	/* 配置串口中断 */
}


/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;



	/* 第1步： 配置GPIO */
	#if 1	/* TX = PA9   RX = PA10 */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 将 PA9 映射为 USART1_TX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

		/* 将 PA10 映射为 USART1_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#else	/* TX = PB6   RX = PB7  */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 将 PB6 映射为 USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* 将 PB7 映射为 USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = 9600;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_DeInit(DMA2_Stream5);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RS232RXbuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RS232bufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);

	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	DMA_Cmd(DMA2_Stream5, ENABLE);
	

	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */

	RS232bt = RS232Pt;


}










/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 0	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	comSendChar(COM1, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 0	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}
/*
*********************************************************************************************************
*	函 数 名: RS232Task
*	功能说明: RS232任务处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS232Task(void)
{
	uint8_t i,flag;
	uint16_t bt,Data16;
	static uint8_t Step,DelayTime;
	static uint16_t btbackup;
	DelayTime++;
	switch(Step){
		case 0:
			while(FIFO_length(RS232bt, RS232Pt, RS232bufSize)>=2){
				if(0 == FIFO_bufcmp(RS232RXbuf, "AT", RS232bt, 2, RS232bufSize)){
					Step++;
					
					DelayTime = 0;
					btbackup = RS232bt;
					RS232bt = FIFO_pointer(RS232bt,2,RS232bufSize);
					break;
				}else{
					RS232bt = FIFO_pointer(RS232bt,1,RS232bufSize);
				}
			}			
			break;
		case 1:
			flag = 0;
			while(FIFO_length(RS232bt, RS232Pt, RS232bufSize)>=2){
				if(0 == FIFO_bufcmp(RS232RXbuf, "\r\n", RS232bt, 2, RS232bufSize)){
					Step++;
					flag = 1;
					RS232bt = FIFO_pointer(RS232bt,2,RS232bufSize);	
					break;
				}else{
					RS232bt = FIFO_pointer(RS232bt,1,RS232bufSize);
				}
			}
			if(flag){
				RS232TXbuflen = 0;
				bt = btbackup;
				while(bt != RS232bt){
					RS232TXbuf[RS232TXbuflen++] = RS232RXbuf[bt];
					bt = FIFO_pointer(bt,1,RS232bufSize);
				}
				RS232TXbuf[RS232TXbuflen++] = '\0';
				printf("%s",RS232TXbuf);
				bt = btbackup; 
				if(0 == FIFO_bufcmp(RS232RXbuf, "AT+EQUIPID=", bt, 11, RS232bufSize)){
					bt = FIFO_pointer(bt,11,RS232bufSize);
					
					while(bt != RS232bt){
						if(isalnum(RS232RXbuf[bt])){
							btbackup = bt;
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
					}
					flag = 0;
					if(FIFO_length(bt, RS232bt, RS232bufSize) >= 22){						
						for(i=0;i<20;i++){
							if(0 == isalnum(RS232RXbuf[bt])){
								break;
							}
							bt = FIFO_pointer(bt,1,RS232bufSize);
						}
						if(i == 20){
							flag = 1;
						}
						if(flag){
							bt = btbackup;
							for(i=0;i<20;i++){
								SystemSet.NetSet.ModbudID[i] = RS232RXbuf[bt];
								bt = FIFO_pointer(bt,1,RS232bufSize);
							}
							SystemSet.ParamSaveFlag |= FlagSystemStateSave;
							
						}
						
						
					}
					if(flag){
						printf("+EQUIPID:SET OK\r\n");
					}else{
						printf("+EQUIPID:SET Eorror\r\n");
					}
					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+EQUIPID?", bt, 11, RS232bufSize)){
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+EQUIPID:");
					memcpy(RS232TXbuf+RS232TXbuflen,SystemSet.NetSet.ModbudID,20);
					RS232TXbuflen += 20;
					RS232TXbuf[RS232TXbuflen++] = '\r';
					RS232TXbuf[RS232TXbuflen++] = '\n';
					RS232TXbuf[RS232TXbuflen++] = '\0';
					printf("%s",RS232TXbuf);
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+TOUCH=1", bt, 10, RS232bufSize)){
					SaveParamTouch();
					MCUSoft_RST();
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+CLRFAULT=1", bt, 10, RS232bufSize)){
					ModuleComSet13Fault(0x8000,0x01);					
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+CLRFAULT: OK\r\n");
					printf("%s",RS232TXbuf);					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+VERSION?", bt, 11, RS232bufSize)){
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+VERSION:\r\n");
					memcpy((char *)(RS232TXbuf+RS232TXbuflen),Ver,5);
					RS232TXbuflen += 5;
					RS232TXbuflen += sprintf((char *)(RS232TXbuf+RS232TXbuflen),"\r\nEdit by fanzhong\r\nDate:2019-10-11\r\n\0");
					printf("%s",RS232TXbuf);
					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+SETNAME=", bt, 11, RS232bufSize)){
					bt = FIFO_pointer(bt,11,RS232bufSize);

					while(bt != RS232bt){
						if(isxdigit(RS232RXbuf[bt])){							
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
					}					
					i = 0;
					while(i<20*2){
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 = RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 = RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 = RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}							
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						memcpy(SystemSet.LineNamebuf+i,&Data16,2);
						i += 2;
					}
					if(i){
						printf("+SETNAME:SET OK\r\n");
						SystemSet.LineNamebuf[i++] = 0;
						SystemSet.LineNamebuf[i++] = 0;
						SystemSet.ParamSaveFlag = FlagSystemState;
					}else{
						printf("+SETNAME:SET Eorror\r\n");
					}
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+TSET", bt, 7, RS232bufSize)){
					//ModuleTxLen = sprintf((char *)ModuleTxBuf,"This is test\r\n");					
					//ModuleTxStart();
					if(Messge_idle == MessgeFlag){
						MessgeFlag = Messge_Start;
					}
					//ModuleComSet10Fault(0x02,0x00,0x00);
				}
				Step = 0;
			}else if(DelayTime >= 20){
				
				RS232bt = RS232Pt;
				Step = 0;
			}			
			break;
	}
}//RS232Task


/***************************** (END OF FILE) *********************************/
