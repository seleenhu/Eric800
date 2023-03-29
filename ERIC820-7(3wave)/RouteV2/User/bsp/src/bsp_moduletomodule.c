#include "bsp.h"



ModuleComStruct *ModuleGlobal = (ModuleComStruct *)ModuleCom_SDRAM;
#define ModuleRxPt       (ModuleRxBufSize-DMA_GetCurrDataCounter(DMA1_Stream5))

#define SPI2CS_SET()	GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define SPI2CS_CLR()	GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI2CS_PIN()	GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)

uint8_t ModuleTxFlag;



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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	DMA_DeInit(DMA1_Stream4);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(SPI2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleGlobal->SPI2Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = SPI2BufSize;
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
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);

	
	
	
	
	SPI_InitTypeDef  SPI_InitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); /* 配置SPI模式 */
	
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
	SPI_Init(SPI2, &SPI_InitStructure);


	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
	
	DMA_Cmd(DMA1_Stream4, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	   
  

	
	SPI_Cmd(SPI2, ENABLE);				/* 使能SPI  */



	USART_InitTypeDef USART_InitStructure;




	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);		
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ; 
	USART_Init(USART2, &USART_InitStructure);

	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	//USART_Cmd(USART2, ENABLE);
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Stream5);
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);



	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);
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
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	DMA_Cmd(DMA1_Stream5, ENABLE);


	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);
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
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);		


	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);

	//SPI4CS_SET();
	//ModuleTxFlag = 0;
	//ModuleGlobal->ModuleRxbt = ModuleRxPt;
	ModuleGlobal->ModuleRxbt = ModuleRxPt;
	SPI2CS_SET();
	
}//ModuleComInit

/*
---------------------------------------------------------------------
Name		Usart2Send
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-08-28
---------------------------------------------------------------------
*/
uint8_t Usart2Send(uint16_t Txbuf_len)
{
	if(0 == ModuleTxFlag){
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream6, DISABLE);
		while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE); 	
		DMA_SetCurrDataCounter(DMA1_Stream6,Txbuf_len); 
		DMA_Cmd(DMA1_Stream6, ENABLE);
		ModuleTxFlag = 1;
		return 0;
	}
	return 1;
}//Usart2Send
/*
---------------------------------------------------------------------
Name		Usart2DMATxIRQ
Function	Usart2DMA发送中断
Input		无
OutPut		无
Edit by fanzhong
Date		2019-04-09
---------------------------------------------------------------------
*/
void Usart2DMATxIRQ(void)
{
	ModuleTxFlag = 0;
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF1);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,DISABLE);  
}//Usart2DMATxIRQ
#if 0
/*
---------------------------------------------------------------------
Name		Usart2IRQDisplay
Function	串口2中断
Input		无
OutPut		无
Edit by fanzhong
Date		2019-08-28
---------------------------------------------------------------------
*/
void Usart2IRQDisplay(void)
{
	if(SET == USART_GetFlagStatus(USART2,USART_FLAG_RXNE)){
		USART_ClearFlag(USART2,USART_FLAG_RXNE);		
		ModuleGlobal->ModuleRxBuf[ModuleGlobal->ModuleRxpt] = USART_ReceiveData(USART2);
		ModuleGlobal->ModuleRxpt = FIFO_pointer(ModuleGlobal->ModuleRxpt, 1,ModuleRxBufSize);
		ModuleTask();
	}
}//Usart2IRQDisplay
#endif

/*
---------------------------------------------------------------------
Name		ModuleSPITxStart
Function	ModuleTx发送
Input		无
OutPut		无
Edit by fanzhong
Date		2019-08-28
---------------------------------------------------------------------
*/
uint8_t ModuleSPITxStart(uint16_t SPITx_len)
{
	uint8_t rt;
	rt = 0;
	if(SPI2CS_PIN()){
		SPI2CS_CLR();
		
		SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Stream4, DISABLE);
		while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);
		
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
		
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE); 
		
		DMA_SetCurrDataCounter(DMA1_Stream4,SPITx_len); 
		DMA_Cmd(DMA1_Stream4, ENABLE); 	
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
	if(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4) != RESET){
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,DISABLE);
		
		
		//while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE);
		//while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);
		
		SPI2CS_SET();
	}
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
	uint16_t i,bt;
	static uint8_t Step;
	static uint16_t Rx_len;
	
	
	switch(Step){
		case 0:
			while(FIFO_length(ModuleGlobal->ModuleRxbt, ModuleRxPt, ModuleRxBufSize)>=2){
				bt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);				
				Rx_len = ModuleGlobal->ModuleRxBuf[bt];
				Rx_len <<= 8;
				bt = ModuleGlobal->ModuleRxbt;
				Rx_len += ModuleGlobal->ModuleRxBuf[bt];
				if(0xCCFF == Rx_len){
					Step++;
					break;
				}else{
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(ModuleGlobal->ModuleRxbt, ModuleRxPt, ModuleRxBufSize)>=5){
				bt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 4, ModuleRxBufSize);
				Rx_len = ModuleGlobal->ModuleRxBuf[bt];
				Rx_len <<= 8;
				bt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 3, ModuleRxBufSize);
				Rx_len += ModuleGlobal->ModuleRxBuf[bt];
				Rx_len += 7;
				Step++;
				if(Rx_len>=256){
					Step = 0;
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
				
			}
			break;
		case 2:
			if(FIFO_length(ModuleGlobal->ModuleRxbt, ModuleRxPt, ModuleRxBufSize)>=Rx_len){
				bt = ModuleGlobal->ModuleRxbt;
				for(i=0;i<Rx_len;i++){
					ModuleGlobal->ModuleBuf[i] = ModuleGlobal->ModuleRxBuf[bt];
					bt = FIFO_pointer(bt, 1, ModuleRxBufSize);
				}
				bt = ModuleGlobal->ModuleBuf[Rx_len-1];
				bt <<= 8;
				bt += ModuleGlobal->ModuleBuf[Rx_len-2];

				if(bt == CRC_GetModbus16(ModuleGlobal->ModuleBuf,Rx_len-2)){
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt,Rx_len, ModuleRxBufSize);
					switch(ModuleGlobal->ModuleBuf[2]){
						case 0x10:
							SystemTime.year = ModuleGlobal->ModuleBuf[5];
							SystemTime.month = ModuleGlobal->ModuleBuf[5];
							SystemTime.day = ModuleGlobal->ModuleBuf[5];
							SystemTime.hour = ModuleGlobal->ModuleBuf[5];
							SystemTime.minute = ModuleGlobal->ModuleBuf[5];
							SystemTime.second = ModuleGlobal->ModuleBuf[5];
							SysSetTime();							
							break;
						case 0x60:
							if(0x01 == ModuleGlobal->ModuleBuf[5]){
								ADCFifo->Calbt = ADCFifo->pt;
								ADCFifo->CalFlag = 1;
								FaultFifo[FaultFifo_pt]->Tim = SysGetTime();
								ModuleUartTXTask(0x01);
							}else if(0x02 == ModuleGlobal->ModuleBuf[5]){
								ComData.VerifyFlag = 1;
								
							}
							break;
						case 0x61:
							ComData.SPIBackCom = 0;
							break;
					}

					
				}else{
					ModuleGlobal->ModuleRxbt = FIFO_pointer(ModuleGlobal->ModuleRxbt, 1, ModuleRxBufSize);
				}
				Step=0;				
			}
			break;
		default:
			ModuleGlobal->ModuleRxbt = ModuleRxPt;
			
			
	}

	
}//ModuleTask

/*
---------------------------------------------------------------------
Name		ModuleUartTXTask
Function	Module任务处理
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-18
---------------------------------------------------------------------
*/
void ModuleUartTXTask(uint8_t sendcmd)
{
	uint8_t i;
	uint16_t Crc16Data;
	switch(sendcmd){
		case 0x01:
		case 0x02:
		case 0x03:
			if(0 == ModuleTxFlag){
				
				i = 0;
				ModuleGlobal->ModuleTxBuf[i++] = 0xFF;
				ModuleGlobal->ModuleTxBuf[i++] = 0xCC;
				ModuleGlobal->ModuleTxBuf[i++] = 0xE0;
				i++;
				i++;
				if(0x01 == sendcmd){
					ModuleGlobal->ModuleTxBuf[i++] = 0x01;
					ModuleGlobal->ModuleTxBuf[i++] = 0x01;
				}else if(0x02 == sendcmd){
					ModuleGlobal->ModuleTxBuf[i++] = 0x01;
					ModuleGlobal->ModuleTxBuf[i++] = 0x02;
				//	memcpy(ModuleGlobal->ModuleTxBuf,ADCFifo->RMS_buf,16*sizeof(float));
				//	i+=16*sizeof(float);
				}else{
					ModuleGlobal->ModuleTxBuf[i++] = 0x02;					
					ModuleGlobal->ModuleTxBuf[i++] = ComData.SelNum;
				}
				ModuleGlobal->ModuleTxBuf[3] = i-5;
				ModuleGlobal->ModuleTxBuf[4] = 0;
				Crc16Data = CRC_GetModbus16(ModuleGlobal->ModuleTxBuf,i);
				ModuleGlobal->ModuleTxBuf[i++] = Crc16Data;
				ModuleGlobal->ModuleTxBuf[i++] = Crc16Data>>8;
				Usart2Send(i);
				
			}
			break;
	} 
}//ModuleUartTXTask
/*
---------------------------------------------------------------------
Name		ModuleSPITXTask
Function	Module任务处理
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-18
---------------------------------------------------------------------
*/
void ModuleSPITXTask(void)
{	
	uint16_t len,Crc16Data;
	static uint8_t i,step = 0,bagNum,sendtim;
	switch(step){
		case 0:
			for(i=0;i<FaultFifoSIZE;i++){
				if(FaultFifo[i]->DislayFlag & 0x80){
					step++;
					break;
				}
			}
			break;
		case 1:
			if(SPI2CS_PIN()){
				len = 0;
				bagNum = 0;
				ModuleGlobal->SPI2Buf[len++] = 0xFF;
				ModuleGlobal->SPI2Buf[len++] = 0xCC;
				ModuleGlobal->SPI2Buf[len++] = 0xE1;
				len++;
				len++;
				ModuleGlobal->SPI2Buf[len++] = 9;
				ModuleGlobal->SPI2Buf[len++] = bagNum;
				memcpy(ModuleGlobal->SPI2Buf+len,&FaultFifo[i]->Tim,sizeof(AlarmTimStruct));
				len += sizeof(AlarmTimStruct);
				ModuleGlobal->SPI2Buf[len++] = FaultFifo[i]->FaultFlag;
				memcpy(ModuleGlobal->SPI2Buf+len,FaultFifo[i]->RMSValue,16*sizeof(float));
				len += 16*sizeof(float);
				memcpy(ModuleGlobal->SPI2Buf+len,FaultFifo[i]->fftratio,16*sizeof(float));
				len += 16*sizeof(float);
				ModuleGlobal->SPI2Buf[3] = (len-5);
				ModuleGlobal->SPI2Buf[4] = (len-5)>>8;
				Crc16Data = CRC_GetModbus16(ModuleGlobal->SPI2Buf,len);
				ModuleGlobal->SPI2Buf[len++] = Crc16Data;
				ModuleGlobal->SPI2Buf[len++] = Crc16Data>>8;
				len += 4;
				ModuleSPITxStart(len);
				ComData.SPIBackCom = 0x01;				
				step++;
				sendtim = 0;
			}
			break;
		case 2:
			if(0 == ComData.SPIBackCom){
				step++;
				bagNum++;
			}else{
				sendtim++;
				if(sendtim >=100){
					step = 0;
				}
			}
			break;
		case 3:
			if(SPI2CS_PIN()){
				len = 0;				
				ModuleGlobal->SPI2Buf[len++] = 0xFF;
				ModuleGlobal->SPI2Buf[len++] = 0xCC;
				ModuleGlobal->SPI2Buf[len++] = 0xE1;
				len++;
				len++;
				ModuleGlobal->SPI2Buf[len++] = 9;
				ModuleGlobal->SPI2Buf[len++] = bagNum;				
				memcpy(ModuleGlobal->SPI2Buf+len,(FaultFifo[i]->Wave[2*(bagNum-1)]),20*128*sizeof(int16_t));				
				len += 20*128*sizeof(int16_t);
				memcpy(ModuleGlobal->SPI2Buf+len,(FaultFifo[i]->Wave[2*(bagNum-1)+1]),20*128*sizeof(int16_t));				
				len += 20*128*sizeof(int16_t);				
				ModuleGlobal->SPI2Buf[3] = (len-5);
				ModuleGlobal->SPI2Buf[4] = (len-5)>>8;
				Crc16Data = CRC_GetModbus16(ModuleGlobal->SPI2Buf,len);
				ModuleGlobal->SPI2Buf[len++] = Crc16Data;
				ModuleGlobal->SPI2Buf[len++] = Crc16Data>>8;
				len += 4;
				ModuleSPITxStart(len);
				ComData.SPIBackCom = 0x01;				
				step++;
				sendtim = 0;
			}
			break;
		case 4:
			if(0 == ComData.SPIBackCom){
				bagNum++;
				if(bagNum<9){
					step--;					
				}else{
					FaultFifo[i]->DislayFlag &= ~0x80;
					step = 0;
				}
			}else{
				sendtim++;
				if(sendtim >=100){
					step = 0;
				}
			}
			break;
		default:
			step = 0;
	}
}//ModuleSPITXTask

void ModuleSPIRMS(void)
{	
	uint16_t len,Crc16Data;
	if(SPI2CS_PIN()){
				len = 0;
				ModuleGlobal->SPI2Buf[len++] = 0xFF;
				ModuleGlobal->SPI2Buf[len++] = 0xCC;
				ModuleGlobal->SPI2Buf[len++] = 0xE2;
				len++;
				len++;
				memcpy(ModuleGlobal->SPI2Buf+len,&ADCFifo->RMS_buf,16*sizeof(float));
				len += 16*sizeof(float);
				ModuleGlobal->SPI2Buf[3] = (len-5);
				ModuleGlobal->SPI2Buf[4] = (len-5)>>8;
				Crc16Data = CRC_GetModbus16(ModuleGlobal->SPI2Buf,len);
				ModuleGlobal->SPI2Buf[len++] = Crc16Data;
				ModuleGlobal->SPI2Buf[len++] = Crc16Data>>8;
				len += 4;
				ModuleSPITxStart(len);
			}
}




