#include "bsp.h"


SPIBitUnion SPIBitTxFolag;
SPIBitUnion M1TOM3BitTxFalg;

ModuleComStruct *ModuleGlobal = (ModuleComStruct *)ModuleCom_SDRAM;
//#define ModuleRxPt       (ModuleRxBufSize-DMA_GetCurrDataCounter(DMA1_Stream3))
#define ModuleRxPt       ModuleGlobal->ModuleRxpt

#define Uart2RxPt       (ModuleRxBufSize-DMA_GetCurrDataCounter(DMA1_Stream5))

#define SPI2RxPt        (SPI2BufSize-DMA_GetCurrDataCounter(DMA1_Stream3))

#define SPI4CS_SET()	GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define SPI4CS_CLR()	GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define SPI4CS_PIN()	GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_4)

uint8_t ModuleTxFlag;
uint8_t Uart2TxFlag;

uint8_t Module62bag;
uint8_t Uart2M1andM3Delay;
uint8_t SPI2M1andM3Delay;

static uint8_t ModuleTimCom(uint8_t comorder);
static void ModuleRxCom(uint8_t *buf);
static uint8_t ModuleBurstCom(uint8_t comorder);
static uint8_t NandRead(uint8_t *Namebuf);
static uint8_t NandRead2(uint8_t *Namebuf);
/*
---------------------------------------------------------------------
Name		ModuleComInit
Function	模块通讯初始化
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-08
---------------------------------------------------------------------
*/
void ModuleComInit(void)
{

	
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_DeInit(DMA2_Stream4);
	while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(SPI4->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->SPI4Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = SPI4BufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	
	
	
	
	
	SPI_InitTypeDef  SPI_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE); /* 配置SPI模式 */
	
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 SPI_BaudRatePrescaler_8 ，实测SCK周期 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */	
	SPI_Init(SPI4, &SPI_InitStructure);


	//SPI_I2S_DMACmd(SPI4, SPI_I2S_DMAReq_Tx, ENABLE);
	
	//DMA_Cmd(DMA2_Stream4, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	   
  

	
	SPI_Cmd(SPI4, ENABLE);				/* 使能SPI  */


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(SPI2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->SPI2Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = SPI2BufSize;
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
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); /* 配置SPI模式 */
	
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 SPI_BaudRatePrescaler_8 ，实测SCK周期 96ns, 10.4MHz */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */	
	SPI_Init(SPI2, &SPI_InitStructure);


	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA1_Stream3, ENABLE);	
	SPI_Cmd(SPI2, ENABLE);				/* 使能SPI  */
	



	USART_InitTypeDef USART_InitStructure;




	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ; 
	USART_Init(UART7, &USART_InitStructure);

#if 1
	
	NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);
#else
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);



	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART7->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->ModuleRxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ModuleRxBufSize;
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
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);

	USART_DMACmd(UART7,USART_DMAReq_Rx,ENABLE);

	DMA_Cmd(DMA1_Stream3, ENABLE);
#endif

	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART7->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->ModuleTxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = ModuleTxBufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);		


	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(UART7, ENABLE);



	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);		
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ; 
	USART_Init(USART2, &USART_InitStructure);


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Stream5);
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);



	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->UART2RxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ModuleRxBufSize;
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
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	DMA_Cmd(DMA1_Stream5, ENABLE);


	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->UART2TxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = ModuleTxBufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);		


	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);
	

	SPI4CS_SET();
	ModuleTxFlag = 0;
	ModuleGlobal->ModuleRxbt = ModuleRxPt;

	
}//ModuleComInit


/*
---------------------------------------------------------------------
Name		ModuleTxStart
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-08
---------------------------------------------------------------------
*/
uint8_t ModuleTxStart(void)
{
	if(0 == ModuleTxFlag){
		USART_DMACmd(UART7,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream1, DISABLE);
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
		DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE); 
		DMA_SetCurrDataCounter(DMA1_Stream1,ModuleGlobal->ModuleTxLen); 
		DMA_Cmd(DMA1_Stream1, ENABLE); 	
		ModuleTxFlag = 1;
		//while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET);
		//while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
	}
	return ModuleTxFlag;

}//ModuleTxStart

/*
---------------------------------------------------------------------
Name		ModuleSPITxStart
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-08
---------------------------------------------------------------------
*/
uint8_t ModuleSPITxStart(void)
{
	uint8_t rt;
	rt = 0;
	if(SPI4CS_PIN()){
		SPI4CS_CLR();
		
		SPI_I2S_DMACmd(SPI4, SPI_I2S_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA2_Stream4, DISABLE);
		while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE);
		
		DMA_ClearFlag(DMA2_Stream4,DMA_FLAG_TCIF4);
		
		DMA_ITConfig(DMA2_Stream4,DMA_IT_TC,ENABLE); 
		
		DMA_SetCurrDataCounter(DMA2_Stream4,ModuleGlobal->SPI4BufTxLen); 
		DMA_Cmd(DMA2_Stream4, ENABLE); 	
		//while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET);
		//while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
		
	}else{
		rt = 1;
	}
	return rt;

}//ModuleTxStart

/*
---------------------------------------------------------------------
Name		ModuleSPITxStartIRQ
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-09
---------------------------------------------------------------------
*/
void ModuleSPITxStartIRQ(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream4,DMA_FLAG_TCIF4) != RESET){
		DMA_ClearFlag(DMA2_Stream4,DMA_FLAG_TCIF4);
		DMA_ITConfig(DMA2_Stream4,DMA_IT_TC,DISABLE);
		
		//while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE);
		//while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);
		
		SPI4CS_SET();
	}
}//ModuleSPITxStartIRQ

/*
---------------------------------------------------------------------
Name		ModuleSPITxStartIRQ
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-09
---------------------------------------------------------------------
*/
void ModuleTxStartIRQ(void)
{
	ModuleTxFlag = 0;
	DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,DISABLE);  
}//ModuleSPITxStartIRQ

/*
---------------------------------------------------------------------
Name		ModuleTask
Function	Module任务处理
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-18
---------------------------------------------------------------------
*/
void ModuleTask(void)
{

	uint16_t i,bt,crcdata;
	static uint8_t Step,DelayTim;
	static uint16_t rxlen;
	static uint16_t Rx62DelayTim;
	
	switch(Step){
		case 0:
			while(FIFO_length(ModuleGlobal->ModuleRxbt,ModuleRxPt,ModuleRxBufSize) >= 2){
				if(0 == FIFO_bufcmp(ModuleGlobal->ModuleRxBuf,"\xFF\xCC",ModuleGlobal->ModuleRxbt,2,ModuleRxBufSize)){
					Step++;
					DelayTim = 0;
					break;
				}else{
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(ModuleGlobal->ModuleRxbt,ModuleRxPt,ModuleRxBufSize) >= 5){
				
				bt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 4, ModuleRxBufSize);				
				rxlen = ModuleGlobal->ModuleRxBuf[bt];
				rxlen <<= 8;
				bt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 3, ModuleRxBufSize);				
				rxlen += ModuleGlobal->ModuleRxBuf[bt];

				Step++;
				DelayTim = 0;
				if(rxlen >= 512-7){					
					Step = 0;
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}				
			}else{
				DelayTim++;
				if(DelayTim>20){
					DelayTim = 0;
					Step = 0;
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(ModuleGlobal->ModuleRxbt,ModuleRxPt,ModuleRxBufSize) >= (rxlen+7)){
				
				bt = ModuleGlobal->ModuleRxbt;				
				for(i=0;i<(rxlen+7);i++){
					Membuf->Modulebuf[i] = ModuleGlobal->ModuleRxBuf[bt];
					bt = FIFO_pointer(bt, 1, ModuleRxBufSize);
				}
				bt = Membuf->Modulebuf[rxlen+6];
				bt <<= 8;
				bt += Membuf->Modulebuf[rxlen+5];
				crcdata = CRC_GetModbus16(Membuf->Modulebuf, rxlen+5);
				if(bt == crcdata){					
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, rxlen+7, ModuleRxBufSize);
					ModuleRxCom(Membuf->Modulebuf);
				}else{
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}

				Step = 0;
				DelayTim = 0;
			}else{
				DelayTim++;
				if(DelayTim>20){
					DelayTim = 0;
					Step = 0;
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		default:
			Step = 0;
			break;
	}
	static uint8_t TXDelaytim;

	if(TXDelaytim < 49){
		TXDelaytim++;
	}else{
		TXDelaytim = 0;
		switch(ComData.ComOrder){
			case 0x01:
				SPIBitTxFolag.Bits.S_C1 = 1;
				
				break;
			case 0x02:
				SPIBitTxFolag.Bits.S_C2 = 1;
				
				break;
			default:
				SPIBitTxFolag.Bits.S_C0 = 1;
				
		}
	}
	
		if(SPIBitTxFolag.Bits.S_E5){				//add by seleen 2022 1 11  
			SPIBitTxFolag.Bits.S_E5=0;
			ModuleBurstCom(0xE5);
		}
	
	if(SPIBitTxFolag.UnionData){
		if(SPIBitTxFolag.Bits.S_C0){
			if(0 == ModuleTimCom(0xC0)){
				SPIBitTxFolag.Bits.S_C0 = 0;			
			}
		}else if(SPIBitTxFolag.Bits.S_C1){
			if(0 == ModuleTimCom(0xC1)){
				SPIBitTxFolag.Bits.S_C1 = 0;			
			}
		}else if(SPIBitTxFolag.Bits.S_C2){
			if(0 == ModuleTimCom(0xC2)){
				SPIBitTxFolag.Bits.S_C2 = 0;			
			}
		}else if(SPIBitTxFolag.Bits.S_E0){		
			if(0 == ModuleBurstCom(0xE0)){
				SPIBitTxFolag.Bits.S_E0 = 0;			
			}
		}else if(SPIBitTxFolag.Bits.S_E1){		
			if(0 == ModuleBurstCom(0xE1)){
				SPIBitTxFolag.Bits.S_E1 = 0;			
			}
		}else if(SPIBitTxFolag.Bits.S_E2){			
			if(20 == Module62bag){
				Rx62DelayTim++;
				if(Rx62DelayTim >= 200){
					Rx62DelayTim = 0;
					Module62bag = 30;
					ComData.bagNums = 0;
				}
			}else if(30 == Module62bag){
				ModuleBurstCom(0xE2);
				Module62bag = 20;
			}else{				
				if(ComData.bagNums < 7){
					if(ComData.bagNums == Module62bag){
						ComData.bagNums++;
					}
					ModuleBurstCom(0xE2);
					Module62bag = 20;
				}else{
					SPIBitTxFolag.Bits.S_E2 = 0;
					Module62bag = 0;					
				}
			}
		}else if(SPIBitTxFolag.Bits.S_E3){
			if(20 == Module62bag){
				Rx62DelayTim++;
				if(Rx62DelayTim >= 200){
					Rx62DelayTim = 0;
					Module62bag = 30;
					ComData.bagNums = 0;
				}
			}else if(30 == Module62bag){
				ModuleBurstCom(0xE3);
				Module62bag = 20;
				Rx62DelayTim=0;// add by seleen 21 09 07
			}else{				
				if(ComData.bagNums < 10){
					if(ComData.bagNums == Module62bag){
						ComData.bagNums++;
					}
					ModuleBurstCom(0xE3);
					Module62bag = 20;
					Rx62DelayTim=0; //add by seleen 21 09 07
				}else{
					SPIBitTxFolag.Bits.S_E3 = 0;
					Module62bag = 0;					
				}
			}
			
		}
		
	}
	
}//ModuleTask




/*
---------------------------------------------------------------------
Name		ModuleBurstCom
Function	突发命令发送
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-19
---------------------------------------------------------------------
*/
static uint8_t ModuleBurstCom(uint8_t comorder)
{
	uint8_t rt;
	uint16_t i,crcdata;	
//  uint8_t k;	
	rt = 0;
	if(SPI4CS_PIN()){
		i = 0;
		ModuleGlobal->SPI4Buf[i++] = 0xFF;
		ModuleGlobal->SPI4Buf[i++] = 0xCC;
		ModuleGlobal->SPI4Buf[i++] = comorder;
		i++;
		i++;
		switch(comorder){
			case 0xE0:				
				memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&NandCfgData),sizeof(NandCfgStruct));
				i += sizeof(NandCfgStruct);
				break;
			case 0xE1:
				if(ComData.FaultSelFlag){
					ModuleGlobal->SPI4Buf[i++] = 0x02;
					if(0 == NandRead2(ComData.FaultNamebuf)){
						memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)&Membuf->FaultAllSelComBuf,sizeof(ModuleComFaultAllSelSaveStruct));
						i += sizeof(ModuleComFaultStruct);
					}
				}else{
					ModuleGlobal->SPI4Buf[i++] = 0x01;
					if(0 == NandRead(ComData.FaultNamebuf)){
						memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)&Membuf->FaultComBuf,sizeof(ModuleComFaultStruct));
						i += sizeof(ModuleComFaultStruct);
					}
				}
				break;
			case 0xE2:
				if(0 == ComData.bagNums){
					ModuleGlobal->SPI4Buf[i++] = 8;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->FaultStyle;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->ValueFlag;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultFifo[FaultFifo_bt]->Tim),sizeof(AlarmTimStruct));
					i += sizeof(AlarmTimStruct);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->rati0),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->fftratio),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->RMSValue),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleSin),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleCos),8*15*sizeof(float));
					i += 8*15*sizeof(float);
				}else{
					ModuleGlobal->SPI4Buf[i++] = 8;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->Wave[ComData.bagNums-1]),20*256*sizeof(int16_t));
					i += 20*256*sizeof(int16_t);
				}							
				break;
			case 0xE3:
#if 1
				if(0 == ComData.bagNums){
					ModuleGlobal->SPI4Buf[i++] = 10;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->FaultStyle;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->ValueFlag;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultFifo[FaultFifo_bt]->Tim),sizeof(AlarmTimStruct));
					i += sizeof(AlarmTimStruct);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->rati0),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->fftratio),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->RMSValue),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleSin),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleCos),8*15*sizeof(float));
					i += 8*15*sizeof(float);
				}else if(ComData.bagNums<8){
					ModuleGlobal->SPI4Buf[i++] = 10;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->Wave[ComData.bagNums-1]),20*256*sizeof(int16_t));
					i += 20*256*sizeof(int16_t);
				}else if(ComData.bagNums<9){					
					ModuleGlobal->SPI4Buf[i++] = 10;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->FaultFlag[0];
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->FaultFlag[1];
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->FaultFlag[2];
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->CanId[0];
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->CanId[1];
					ModuleGlobal->SPI4Buf[i++] = FaultAllSelFifo[FaultFifo_bt]->CanId[2];
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultAllSelFifo[FaultFifo_bt]->Tim),3*sizeof(AlarmTimStruct));
					i += 3*sizeof(AlarmTimStruct);					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultAllSelFifo[FaultFifo_bt]->ratio),3*sizeof(float));
					i += 3*sizeof(float);					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultAllSelFifo[FaultFifo_bt]->RMSValue),3*sizeof(float));
					i += 3*sizeof(float);
				}else{
					ModuleGlobal->SPI4Buf[i++] = 10;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;
					/*
					if(0 == FaultAllSelFifo[FaultFifo_bt]->CanId){
						memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave),20*128*sizeof(int16_t));
						i += 20*128*sizeof(int16_t);	
					}else{
						memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave),20*256*sizeof(int16_t));
						i += 20*256*sizeof(int16_t);
					}
					*/
					#if 0
					for(k=0;k<3;k++){
						if(0 == FaultAllSelFifo[FaultFifo_bt]->CanId[k]){
							memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave[k]),20*128*sizeof(int16_t));
							i += 20*128*sizeof(int16_t);	
						}else{
							memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave[k]),20*256*sizeof(int16_t));
						i += 20*256*sizeof(int16_t);
						}
							
					}
					#else
					if(0 == FaultAllSelFifo[FaultFifo_bt]->CanId[0]){
							memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave[0]),20*128*sizeof(int16_t));
							i += 20*128*sizeof(int16_t);	
						}else{
							memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultAllSelFifo[FaultFifo_bt]->Wave[0]),20*256*sizeof(int16_t));
						i += 20*256*sizeof(int16_t);
						}
					#endif
					
				}
#else
				if(0 == ComData.bagNums){
					ModuleGlobal->SPI4Buf[i++] = 17;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->FaultStyle;
					ModuleGlobal->SPI4Buf[i++] = FaultFifo[FaultFifo_bt]->ValueFlag;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultFifo[FaultFifo_bt]->Tim),sizeof(AlarmTimStruct));
					i += sizeof(AlarmTimStruct);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->rati0),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->fftratio),8*sizeof(float));
					i += 8*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->RMSValue),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleSin),8*15*sizeof(float));
					i += 8*15*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->VabuleCos),8*15*sizeof(float));
					i += 8*15*sizeof(float);
				}else if(ComData.bagNums<8){
					ModuleGlobal->SPI4Buf[i++] = 17;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultFifo[FaultFifo_bt]->Wave[ComData.bagNums-1]),20*256*sizeof(int16_t));
					i += 20*256*sizeof(int16_t);
				}else if(ComData.bagNums<9){					
					ModuleGlobal->SPI4Buf[i++] = 17;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;					
					ModuleGlobal->SPI4Buf[i++] = FaultSelFifo[FaultFifo_bt]->FaultFlag;									
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&FaultSelFifo[FaultFifo_bt]->Tim),sizeof(AlarmTimStruct));
					i += sizeof(AlarmTimStruct);					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultSelFifo[FaultFifo_bt]->fftratio),16*sizeof(float));
					i += 16*sizeof(float);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultSelFifo[FaultFifo_bt]->RMSValue),16*sizeof(float));
					i += 16*sizeof(float);
				}else{
					ModuleGlobal->SPI4Buf[i++] = 17;
					ModuleGlobal->SPI4Buf[i++] = ComData.bagNums;					
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultSelFifo[FaultFifo_bt]->Wave[2*(ComData.bagNums-9)]),20*128*sizeof(int16_t));
					i += 20*128*sizeof(int16_t);
					memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(FaultSelFifo[FaultFifo_bt]->Wave[2*(ComData.bagNums-9)+1]),20*128*sizeof(int16_t));
					i += 20*128*sizeof(int16_t);
				}
	#endif
				break;
				//===========增加0xE5指令作校准RMS显示用
			case 0xE5:
					memcpy(ModuleGlobal->SPI4Buf+i,Membuf->RMSValue,16*sizeof(float));
					i += 16*sizeof(float);
					break;
				
				
			
			default:
				rt = 2;
		}
		if(0 == rt){
			ModuleGlobal->SPI4Buf[3] = (i-5);
			ModuleGlobal->SPI4Buf[4] = (i-5)>>8;
			crcdata = CRC_GetModbus16(ModuleGlobal->SPI4Buf, i);
			ModuleGlobal->SPI4Buf[i++] = crcdata;
			ModuleGlobal->SPI4Buf[i++] = crcdata>>8;
			ModuleGlobal->SPI4BufTxLen = i+4;
			
			ModuleSPITxStart();
		}
		rt = 0;
		//if(0xE2 == comorder){
		//	rt = 1;
			//if(ComData.bagNums < 7){
			//	ComData.bagNums++;
			//	rt = 1;
			//}else{				
			//	rt = 0;
			//}
		//}
	}else{
		rt = 1;
	}
	
	return rt;
}//ModuleBurstCom

/*
---------------------------------------------------------------------
Name		ModuleTimCom
Function	定时命令发送
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-19
---------------------------------------------------------------------
*/
static uint8_t ModuleTimCom(uint8_t comorder)
{
	uint8_t rt;
	uint16_t i,j,k,bt,bt1,crcdata;
	
	rt = 0;
	if(SPI4CS_PIN()){
		i = 0;
		ModuleGlobal->SPI4Buf[i++] = 0xFF;
		ModuleGlobal->SPI4Buf[i++] = 0xCC;
		ModuleGlobal->SPI4Buf[i++] = comorder;
		i++;
		i++;
		ModuleGlobal->SPI4Buf[i++]  = ComData.LEDState;
		ModuleGlobal->SPI4Buf[i++]  = ComData.LEDState>>8;
		ModuleGlobal->SPI4Buf[i++] = SystemState.SwitchState.phaseAction;
		memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(&NandCfgData),sizeof(NandCfgStruct));
		i += sizeof(NandCfgStruct);
		memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->RMS_buf),8*sizeof(float));
		i += 8*sizeof(float);
		memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->rati0),8*sizeof(float));
		i += 8*sizeof(float);
		memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(ADCFifo->fftratio),8*sizeof(float));			
		i += 8*sizeof(float);
		memcpy(ModuleGlobal->SPI4Buf+i,(uint8_t *)(SystemPowerQuality),sizeof(PowerQualityStruct));
		i += sizeof(PowerQualityStruct);
		switch(comorder){			
			case 0xC1:				
				bt1 = FIFO_Backpointer(ADCFifo->pt, 12*256, ADCFifo->FifoSize);				
				for(k=0;k<7;k++){
					bt = bt1;
					for(j=0;j<12*64;j++){
						memcpy((uint8_t *)(ModuleGlobal->SPI4Buf+i+j*2),(uint8_t *)&(ADCFifo->ADCFIFO[k][bt]),2);
						bt =  FIFO_pointer(bt, 4, ADCFifo->FifoSize);
					}
					i += 12*64*2;					
				}
				break;			
		}
		memcpy(ModuleGlobal->SPI4Buf+i,Membuf->CanOnlineTable,16);
		i+=16;
		ModuleGlobal->SPI4Buf[3] = (i-5);
		ModuleGlobal->SPI4Buf[4] = (i-5)>>8;
		crcdata = CRC_GetModbus16(ModuleGlobal->SPI4Buf, i);
		ModuleGlobal->SPI4Buf[i++] = crcdata;
		ModuleGlobal->SPI4Buf[i++] = crcdata>>8;
		ModuleGlobal->SPI4BufTxLen = i+4;
		ModuleSPITxStart();
	}else{
		rt = 1;
	}
	return rt;
}//ModuleTimCom


/*
---------------------------------------------------------------------
Name		ModuleRxCom
Function	接收命令处理
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-19
---------------------------------------------------------------------
*/
static void ModuleRxCom(uint8_t *buf)
{
	uint16_t i;
	switch(buf[2]){
		case 0x10:
			ComData.ComOrder = 0;
			if(0x01 == buf[5]){
				ComData.ComOrder = 0x00;
			}else if(0x02 == buf[5]){
				ComData.ComOrder = 0x01;
			}
			if(0x01 == buf[6]){
				ComData.VerifyFlag = 0x01;
				M1TOM3BitTxFalg.Bits.S_E1 = 1;
				

			}
			switch(buf[7]){
				case 0x01:
					SystemState.SwitchState.AphaseState = 0x01;
					SystemState.SwitchState.BphaseState = 0x00;
					SystemState.SwitchState.CphaseState = 0x00;
					break;
				case 0x02:
					SystemState.SwitchState.AphaseState = 0x00;
					SystemState.SwitchState.BphaseState = 0x01;
					SystemState.SwitchState.CphaseState = 0x00;
					break;
				case 0x03:
					SystemState.SwitchState.AphaseState = 0x00;
					SystemState.SwitchState.BphaseState = 0x00;
					SystemState.SwitchState.CphaseState = 0x01;
					break;
				case 0x04:
					SystemState.SwitchState.AphaseState = 0x00;
					SystemState.SwitchState.BphaseState = 0x00;
					SystemState.SwitchState.CphaseState = 0x00;
				  SystemState.SwitchState.DphaseState = 0x01;
					break;
				case 0x05:
					SystemState.SwitchState.AphaseState = 0x02;
					SystemState.SwitchState.BphaseState = 0x02;
					SystemState.SwitchState.CphaseState = 0x02;
					SystemState.SwitchState.DphaseState = 0x02;
				  GndPhase2HourFlag=0;
					if(ADCParameter.GndStep >= 8){
						ADCParameter.GndStep = 0;
						Ground_LED_OFF();
					}
					break;
			}
						
			break;
		case 0x11:
			i = 5;
			memcpy((uint8_t *)&PoramData.VoltageSet.MaxRatedValue,buf+i,sizeof(uint32_t));
			i += sizeof(uint32_t);
			memcpy((uint8_t *)&PoramData.VoltageSet.Modulus,buf+i,sizeof(float));
			i += sizeof(float);
			memcpy((uint8_t *)&PoramData.RatioStruct.RatioPT,buf+i,sizeof(uint16_t));
			i += sizeof(uint16_t);
			memcpy((uint8_t *)&PoramData.RatioStruct.RatioCT,buf+i,sizeof(uint16_t));
			i += sizeof(uint16_t);
			memcpy((uint8_t *)&PoramData.ShortGNDSet.I0,buf+i,sizeof(float));
			i += sizeof(float);
			PoramFlag = 1;
			break;
		case 0x12:
			RTC_DateStructure.RTC_Year = buf[5];
			RTC_DateStructure.RTC_Month = buf[6];
			RTC_DateStructure.RTC_Date = buf[7];
			RTC_TimeStructure.RTC_Hours = buf[8];
			RTC_TimeStructure.RTC_Minutes = buf[9];
			RTC_TimeStructure.RTC_Seconds = buf[10];
			RTC_Config();
			M1TOM3Uart2_H10Tx(RTC_DateStructure,RTC_TimeStructure);
			break;
		case 0x13:
			//data16 = buf[6];
			//data16 <<= 8;
			//data16 += buf[5];
			
			switch(buf[7]){
				case 1:
					NandCfgData.GndPtNew = 0;
					NandCfgData.TotalGndPt = 0;					
					NandCfgData.ExPtNew = 0;
					NandCfgData.TotalExPt = 0;
					NandCfgData.SExPtNew = 0;
					NandCfgData.TotalSExPt = 0;
					NandCfgData.PTPtNew = 0;
					NandCfgData.TotalPTPT = 0;
					NandCfgData.DEPtNew = 0;
					NandCfgData.TotalDEPT = 0;
					NandCfgData.SHPtNew = 0;
					NandCfgData.TotalSHPT = 0;
					SaveNandFaultNum();
					break;
				case 2:
					break;
			}			
			break;
		case 0x60:			
			SPIBitTxFolag.Bits.S_E0 = 1;
			break;
		case 0x61:
			memset((char *)ComData.FaultNamebuf,0,32);
			switch(buf[5]){
				case 0x01:				
					sprintf((char *)ComData.FaultNamebuf,"1:/GD%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x02:
					sprintf((char *)ComData.FaultNamebuf,"1:/SEV%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x03:
					sprintf((char *)ComData.FaultNamebuf,"1:/EV%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x04:
					sprintf((char *)ComData.FaultNamebuf,"1:/PT%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x05:
					sprintf((char *)ComData.FaultNamebuf,"1:/DE%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x06:
					sprintf((char *)ComData.FaultNamebuf,"1:/SH%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 0;
					break;
				case 0x07:
					sprintf((char *)ComData.FaultNamebuf,"1:/GD%d.txt\0",buf[6]);
					SPIBitTxFolag.Bits.S_E1 = 1;
					ComData.FaultSelFlag = 1;
					break;
			}			
			break;
		case 0x62:
			Module62bag = buf[5];
			break;
		case 0x63:
			Module62bag = buf[5];
			//SPIBitTxFolag.Bits.S_E3 = 1;
			break;
	}
}//ModuleRxCom
/*
---------------------------------------------------------------------
Name		NandRead
Function	Nand阅读
Input		目录
OutPut		0 OK >=1 ERR
Edit by 	fanzhong
Date		2019-04-24
---------------------------------------------------------------------
*/
static uint8_t NandRead(uint8_t *Namebuf)
{
	uint8_t rt,i;
	uint16_t j;	
	FATFS fs;
	FIL fp;
	DIR DirInf;
	FRESULT result;
	UINT ByteWritten;
	
	rt = 1;
	result = f_mount(&fs, FS_VOLUME_NAND, 1);
	if(FR_OK == result){
		result = f_opendir(&DirInf, FS_VOLUME_NAND);
		if(FR_OK == result){			
			result = f_open(&fp, (char *)Namebuf, FA_READ);			
			if(FR_OK == result){
				result = f_read(&fp,(uint8_t *)(&Membuf->NandRead),sizeof(FaultSaveStruct),&ByteWritten);
				
				f_close(&fp);
				Membuf->FaultComBuf.DislayFlag = Membuf->NandRead.DislayFlag;
				Membuf->FaultComBuf.FaultStyle = Membuf->NandRead.FaultStyle;
				Membuf->FaultComBuf.Tim = Membuf->NandRead.Tim;
				Membuf->FaultComBuf.ValueFlag = Membuf->NandRead.ValueFlag;
				memcpy((uint8_t *)Membuf->FaultComBuf.ratio,(uint8_t *)Membuf->NandRead.ratio,8*sizeof(float));
				memcpy((uint8_t *)Membuf->FaultComBuf.fftratio,(uint8_t *)Membuf->NandRead.fftratio,8*sizeof(float));
				memcpy((uint8_t *)Membuf->FaultComBuf.RMSValue,(uint8_t *)Membuf->NandRead.RMSValue,8*15*sizeof(float));
				memcpy((uint8_t *)Membuf->FaultComBuf.VabuleSin,(uint8_t *)Membuf->NandRead.VabuleSin,8*15*sizeof(float));
				memcpy((uint8_t *)Membuf->FaultComBuf.VabuleCos,(uint8_t *)Membuf->NandRead.VabuleCos,8*15*sizeof(float));
				for(i=0;i<7;i++){
					for(j=0;j<12*64;j++){
						Membuf->FaultComBuf.Wave[i][j] = Membuf->NandRead.Wave[i][6*256+j*4];
					}
				}				
				rt = 0;
			}					
			f_closedir(&DirInf);
		}
		f_mount(NULL,FS_VOLUME_NAND, 0);
	}
	return rt;
}//NandRead
/*
---------------------------------------------------------------------
Name		NandRead2
Function	Nand阅读
Input		目录
OutPut		0 OK >=1 ERR
Edit by 	fanzhong
Date		2019-09-04
---------------------------------------------------------------------
*/
static uint8_t NandRead2(uint8_t *Namebuf)
{
	uint8_t rt;
	uint16_t j;
  uint8_t k;	
	FATFS fs;
	FIL fp;
	DIR DirInf;
	FRESULT result;
	UINT ByteWritten;
	
	rt = 1;
	result = f_mount(&fs, FS_VOLUME_NAND, 1);
	if(FR_OK == result){
		result = f_opendir(&DirInf, FS_VOLUME_NAND);
		if(FR_OK == result){			
			result = f_open(&fp, (char *)Namebuf, FA_READ);			
			if(FR_OK == result){
				f_lseek(&fp,sizeof(FaultSaveStruct));
				
				result = f_read(&fp,(uint8_t *)(&Membuf->NandAllSelRead),sizeof(FaultAllSelSaveStruct),&ByteWritten);
				
				f_close(&fp);
				/*
				Membuf->FaultAllSelComBuf.FaultFlag = Membuf->NandAllSelRead.FaultFlag;
				Membuf->FaultAllSelComBuf.CanId = Membuf->NandAllSelRead.CanId;
				Membuf->FaultAllSelComBuf.Tim = Membuf->NandAllSelRead.Tim;
				Membuf->FaultAllSelComBuf.ratio = Membuf->NandAllSelRead.ratio;
				Membuf->FaultAllSelComBuf.RMSValue = Membuf->NandAllSelRead.RMSValue;
				*/
				memcpy(Membuf->FaultAllSelComBuf.FaultFlag,Membuf->NandAllSelRead.FaultFlag,3);
				memcpy(Membuf->FaultAllSelComBuf.CanId ,Membuf->NandAllSelRead.CanId,3);
				memcpy(Membuf->FaultAllSelComBuf.Tim,Membuf->NandAllSelRead.Tim,3*sizeof(AlarmTimStruct));
				memcpy(Membuf->FaultAllSelComBuf.ratio,Membuf->NandAllSelRead.ratio,3*sizeof(float));
				memcpy(Membuf->FaultAllSelComBuf.RMSValue , Membuf->NandAllSelRead.RMSValue,3*sizeof(float));
				/*
				if(0 == Membuf->FaultAllSelComBuf.CanId){
					for(j=0;j<12*32;j++){
						Membuf->FaultAllSelComBuf.Wave[j] = Membuf->NandAllSelRead.Wave[6*128+j*4];
					}
				}else{
					for(j=0;j<12*32;j++){
						Membuf->FaultAllSelComBuf.Wave[j] = Membuf->NandAllSelRead.Wave[6*256+j*8];
					}
				}		
					*/
					for(k=0;k<3;k++){
						if(0 == Membuf->FaultAllSelComBuf.CanId[k]){
							for(j=0;j<12*32;j++){
							Membuf->FaultAllSelComBuf.Wave[k][j] = Membuf->NandAllSelRead.Wave[k][6*128+j*4];
							}
						}else{
							for(j=0;j<12*32;j++){
							Membuf->FaultAllSelComBuf.Wave[k][j] = Membuf->NandAllSelRead.Wave[k][6*256+j*8];
					}
				}		
					}
				rt = 0;
			}					
			f_closedir(&DirInf);
		}
		f_mount(NULL,FS_VOLUME_NAND, 0);
	}
	return rt;
}//NandRead2
/*
---------------------------------------------------------------------
Name		Uart7_M1ANDLCD_IRQ
Function	Uart7中断
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-09-02
---------------------------------------------------------------------
*/
void Uart7_M1ANDLCD_IRQ(void)
{
	if(SET == USART_GetFlagStatus(UART7, USART_FLAG_RXNE)){
		ModuleGlobal->ModuleRxBuf[ModuleGlobal->ModuleRxpt] = USART_ReceiveData(UART7);
		ModuleGlobal->ModuleRxpt = FIFO_pointer(ModuleGlobal->ModuleRxpt, 1, ModuleRxBufSize);
		USART_ClearFlag(UART7, USART_FLAG_RXNE);
	}
}//Uart7_M1ANDLCD_IRQ

/*
---------------------------------------------------------------------
Name		Uart2TxStartIRQ
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-09
---------------------------------------------------------------------
*/
void Uart2TxStartIRQ(void)
{
	Uart2TxFlag = 0;
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,DISABLE);  
}//Uart2TxStartIRQ

/*
---------------------------------------------------------------------
Name		Uart2TxStart
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-09-02
---------------------------------------------------------------------
*/
uint8_t Uart2TxStart(void)
{
	if(0 == Uart2TxFlag){	
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream6, DISABLE);
		while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE); 
		DMA_SetCurrDataCounter(DMA1_Stream6,ModuleGlobal->UART2TxLen); 
		DMA_Cmd(DMA1_Stream6, ENABLE); 	
		Uart2TxFlag = 1;
		//while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET);
		//while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
	}
	return Uart2TxFlag;

}//Uart2TxStart

/*
---------------------------------------------------------------------
Name		M1andM3_task
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-09-02
---------------------------------------------------------------------
*/
void M1andM3_task(void)
{
	uint16_t i,bt,data16;
	static uint8_t Uart2_step,SPI2_step,SendBagNum;
	static uint16_t rxlen,SPI2_rxlen;
	
	switch(Uart2_step){
		case 0:			
			while(FIFO_length(ModuleGlobal->UART2Rxbt, Uart2RxPt, ModuleRxBufSize)>=2){
				bt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				data16 = ModuleGlobal->UART2RxBuf[bt];
				bt = ModuleGlobal->UART2Rxbt;
				data16 <<= 8;
				data16 |= ModuleGlobal->UART2RxBuf[bt];
				if(0xCCFF == data16){
					Uart2_step++;
					Uart2M1andM3Delay = 0;
					break;
				}else{
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(ModuleGlobal->UART2Rxbt, Uart2RxPt, ModuleRxBufSize) >= 5){
				
				bt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 4, ModuleRxBufSize);				
				rxlen = ModuleGlobal->UART2RxBuf[bt];
				rxlen <<= 8;
				bt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 3, ModuleRxBufSize);								
				rxlen += ModuleGlobal->UART2RxBuf[bt];
				rxlen += 7;

				Uart2_step++;
				Uart2M1andM3Delay = 0;
				if(rxlen >= 256){					
					Uart2_step = 0;
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				}				
			}else{				
				if(Uart2M1andM3Delay>20){
					Uart2M1andM3Delay = 0;
					Uart2_step = 0;
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(ModuleGlobal->UART2Rxbt, Uart2RxPt, ModuleRxBufSize) >= rxlen){
				
				bt = ModuleGlobal->UART2Rxbt;				
				for(i=0;i<rxlen;i++){
					Membuf->Modulebuf[i] = ModuleGlobal->UART2RxBuf[bt];
					bt = FIFO_pointer(bt, 1, ModuleRxBufSize);
				}
				bt = Membuf->Modulebuf[rxlen-1];
				bt <<= 8;
				bt += Membuf->Modulebuf[rxlen-2];
				
				data16 = CRC_GetModbus16(Membuf->Modulebuf, rxlen-2);
				if(bt == data16){					
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt,rxlen, ModuleRxBufSize);
					if(0xE0 == Membuf->Modulebuf[2]){
						if(0x01 == Membuf->Modulebuf[5]){
							if(0x01 == Membuf->Modulebuf[6]){
								
							}else if(0x02 == Membuf->Modulebuf[6]){
								//SiliconControl_OFF();
								SiliconControl_flag = 0;
								#if 0
								//==================================================  // add by seleen 2022 1 11 
								memcpy(Membuf->RMSValue,&(Membuf->Modulebuf[7]),16*sizeof(float));
								SPIBitTxFolag.Bits.S_E5=1;
								//==================================================
								#endif
							}
						}else if(0x02 == Membuf->Modulebuf[5]){
							
						}
					}
				}else{
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				}				
				Uart2_step = 0;	
			}else{				
				if(Uart2M1andM3Delay>20){
					Uart2M1andM3Delay = 0;
					Uart2_step = 0;
					ModuleGlobal->UART2Rxbt = FIFO_pointer(ModuleGlobal->UART2Rxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		default:
			Uart2_step = 0;
			break;
	}

	switch(SPI2_step){
		case 0:			
			while(FIFO_length(ModuleGlobal->SPI2BufRxbt, SPI2RxPt, SPI2BufSize)>=2){
				bt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);
				data16 = ModuleGlobal->SPI2Buf[bt];
				bt = ModuleGlobal->SPI2BufRxbt;
				data16 <<= 8;
				data16 |= ModuleGlobal->SPI2Buf[bt];
				if(0xCCFF == data16){
					SPI2_step++;
					SPI2M1andM3Delay = 0;
					break;
				}else{
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(ModuleGlobal->SPI2BufRxbt, SPI2RxPt, SPI2BufSize) >= 5){
				
				bt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 4, SPI2BufSize);				
				SPI2_rxlen = ModuleGlobal->SPI2Buf[bt];
				SPI2_rxlen <<= 8;
				bt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 3, SPI2BufSize);							
				SPI2_rxlen += ModuleGlobal->SPI2Buf[bt];
				SPI2_rxlen += 7;
				
				 

				SPI2_step++;
				SPI2M1andM3Delay = 0;
				if(SPI2_rxlen >= 20*1024){					
					SPI2_step = 0;
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);					
				}	

			
			}else{				
				if(SPI2M1andM3Delay>100){
					SPI2M1andM3Delay = 0;
					SPI2_step = 0;
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);					
				}
			}
			break;
		case 2:
			if(FIFO_length(ModuleGlobal->SPI2BufRxbt, SPI2RxPt, SPI2BufSize) >= SPI2_rxlen){
				
				bt = ModuleGlobal->SPI2BufRxbt;				
				for(i=0;i<SPI2_rxlen;i++){
					Membuf->Modulebuf[i] = ModuleGlobal->SPI2Buf[bt];
					bt = FIFO_pointer(bt, 1, SPI2BufSize);
				}
				bt = Membuf->Modulebuf[SPI2_rxlen-1];
				bt <<= 8;
				bt += Membuf->Modulebuf[SPI2_rxlen-2];
				
				data16 = CRC_GetModbus16(Membuf->Modulebuf, SPI2_rxlen-2);
				if(bt == data16){
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, SPI2_rxlen, SPI2BufSize);
					if(Membuf->Modulebuf[2]==0xE2){					//校准的RMS[16]
						memcpy(Membuf->RMSValue,&(Membuf->Modulebuf[5]),16*sizeof(float));
						SPIBitTxFolag.Bits.S_E5=1;
						
					}

					else{
					M1TOM3BitTxFalg.Bits.S_E3 = 1;
					SendBagNum = Membuf->Modulebuf[6];
					if(0 == Membuf->Modulebuf[6]){
						i = 7;
						memcpy(&FaultSelFifo[FaultFifo_pt]->Tim,Membuf->Modulebuf+i,sizeof(AlarmTimStruct));
						i += sizeof(AlarmTimStruct);
						FaultSelFifo[FaultFifo_pt]->FaultFlag = Membuf->Modulebuf[i++];
						memcpy(FaultSelFifo[FaultFifo_pt]->RMSValue,Membuf->Modulebuf+i,16*sizeof(float));
						i += 16*sizeof(float);
						memcpy(FaultSelFifo[FaultFifo_pt]->fftratio,Membuf->Modulebuf+i,16*sizeof(float));
						i += 16*sizeof(float);	
					}else{
						i = 7;						
						memcpy(FaultSelFifo[FaultFifo_pt]->Wave[2*(Membuf->Modulebuf[6]-1)],Membuf->Modulebuf+i,20*128*sizeof(int16_t));
						i += 20*128*sizeof(int16_t);
						memcpy(FaultSelFifo[FaultFifo_pt]->Wave[2*(Membuf->Modulebuf[6]-1)+1],Membuf->Modulebuf+i,20*128*sizeof(int16_t));						
					}
					if(Membuf->Modulebuf[6]+1 >= Membuf->Modulebuf[5]){
						FaultFifo[FaultFifo_pt]->DislayFlag |= 0x80;
						//FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20; //save //add by seleen 21 09 08
						AllSelDisplay();
						/*
						if(0xE0 == (FaultFifo[FaultFifo_pt]->DislayFlag & 0xE0)){
							FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
							FaultFifo_busy = 0;
							FaultFifo_pt++;
							if(FaultFifo_pt>= FaultFifoSIZE){
								FaultFifo_pt = 0;
							}
						}
						*/
					}
				}
				}else{
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);
				
				}				
				SPI2_step = 0;
			}else{				
				if(SPI2M1andM3Delay>100){
					SPI2M1andM3Delay = 0;
					SPI2_step = 0;
					ModuleGlobal->SPI2BufRxbt = FIFO_pointer(ModuleGlobal->SPI2BufRxbt, 1, SPI2BufSize);					
				}
			}
			break;
		default:
			SPI2_step = 0;
			break;
	}
	if(M1TOM3BitTxFalg.UnionData){
		if(M1TOM3BitTxFalg.Bits.S_E0){
			if(0 == M1TOM3Uart2Tx(0x60,0x01)){
				M1TOM3BitTxFalg.Bits.S_E0 = 0;
			}
		}else if(M1TOM3BitTxFalg.Bits.S_E1){
			if(0 == M1TOM3Uart2Tx(0x60,0x02)){
				M1TOM3BitTxFalg.Bits.S_E1 = 0;
			}			
		}else if(M1TOM3BitTxFalg.Bits.S_E3){
			if(0 == M1TOM3Uart2Tx(0x61,SendBagNum)){
				M1TOM3BitTxFalg.Bits.S_E3 = 0;
			}			
		}
	}
	
}//M1andM3_task

/*
---------------------------------------------------------------------
Name		M1TOM3Uart2Tx
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-09-03
---------------------------------------------------------------------
*/
uint8_t M1TOM3Uart2Tx(uint8_t SendCmd,uint8_t Senddata)
{
	uint16_t crcdata16;
	if(0 == Uart2TxFlag){
		ModuleGlobal->UART2TxLen = 0;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = 0xFF;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = 0xCC;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = SendCmd;
		ModuleGlobal->UART2TxLen++;
		ModuleGlobal->UART2TxLen++;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = Senddata;
		ModuleGlobal->UART2TxBuf[3] = (ModuleGlobal->UART2TxLen-5);
		ModuleGlobal->UART2TxBuf[4] = (ModuleGlobal->UART2TxLen-5)>>8;
		crcdata16 = CRC_GetModbus16(ModuleGlobal->UART2TxBuf, ModuleGlobal->UART2TxLen);
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = crcdata16;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = crcdata16>>8;
		Uart2TxStart();
		return 0;
	}
	return 1;
}//M1TOM3Uart2Tx

/*
---------------------------------------------------------------------
Name		M1TOM3Uart2_H10Tx
Function	对时
Input		无
OutPut		无
Edit by fanzhong
Date		2019-09-04
---------------------------------------------------------------------
*/
uint8_t M1TOM3Uart2_H10Tx(RTC_DateTypeDef DateData,RTC_TimeTypeDef TimeData)
{
	uint16_t crcdata16;
	if(0 == Uart2TxFlag){
		ModuleGlobal->UART2TxLen = 0;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = 0xFF;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = 0xCC;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = 0x10;
		ModuleGlobal->UART2TxLen++;
		ModuleGlobal->UART2TxLen++;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = DateData.RTC_Year;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = DateData.RTC_Month;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = DateData.RTC_Date;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = TimeData.RTC_Hours;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = TimeData.RTC_Minutes;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = TimeData.RTC_Seconds;
		
		ModuleGlobal->UART2TxBuf[3] = (ModuleGlobal->UART2TxLen-5);
		ModuleGlobal->UART2TxBuf[4] = (ModuleGlobal->UART2TxLen-5)>>8;
		crcdata16 = CRC_GetModbus16(ModuleGlobal->UART2TxBuf, ModuleGlobal->UART2TxLen);
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = crcdata16;
		ModuleGlobal->UART2TxBuf[ModuleGlobal->UART2TxLen++] = crcdata16>>8;
		Uart2TxStart();
		return 0;
	}
	return 1;
}//M1TOM3Uart2_H10Tx




