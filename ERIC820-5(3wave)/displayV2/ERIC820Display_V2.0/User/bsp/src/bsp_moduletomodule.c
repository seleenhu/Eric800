#include "bsp.h"

static uint8_t testCnt=0;
//uint8_t SPI4Buf[SPI4BufSize];


//uint8_t ModuleTxBuf[ModuleTxBufSize];
//uint16_t ModuleTxLen;

//uint8_t ModuleRxBuf[ModuleRxBufSize];
//uint16_t ModuleRxpt;
//uint16_t ModuleRxbt;

NandCfgStruct NandCfgData;

#define ModuleSPI4Pt     (SPI4BufSize - DMA_GetCurrDataCounter(DMA2_Stream3))

ModuleComStruct *ModuleCombuf = (ModuleComStruct *)ModuleCom_SDRAM;
ModuleComFaultStruct *ModuleComFaultbuf = (ModuleComFaultStruct *)ModuleComFault_SDRAM;
//ModuleComFaultSelStruct *ModuleComSelFaultbuf = (ModuleComFaultSelStruct *)ModuleComSelFault_SDRAM;

SPIBitUnion SPIBitFlag;
static uint8_t ModuleInitFlag;
static uint8_t ModuleRxbit,ModuleRxdata;
static uint8_t ModuleComSet62Fault(uint8_t bagnums);
static uint8_t ModuleComSet63Fault(uint8_t bagnums);
/*
---------------------------------------------------------------------
Name		ModuleComInit
Function	模块通讯初始化
Input		无
OutPut		无
Edit by seleen hu
Date		2019-04-08
---------------------------------------------------------------------
*/
void ModuleComInit(void)
{

	
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_DeInit(DMA2_Stream3);
	while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(SPI4->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ModuleCombuf->SPI4Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = SPI4BufSize;
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
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

	
	SPI_InitTypeDef  SPI_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE); /* 配置SPI模式 */
	
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
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */	
	SPI_Init(SPI4, &SPI_InitStructure);


	SPI_I2S_DMACmd(SPI4, SPI_I2S_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA2_Stream3, ENABLE);

	
	SPI_Cmd(SPI4, ENABLE);				/* 使能SPI  */



	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_ICInitTypeDef  TIM5_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  



	TIM_TimeBaseStructure.TIM_Period = 104*3/2; 
	TIM_TimeBaseStructure.TIM_Prescaler =84-1;    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
    
   
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource10);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_ClearITPendingBit(EXTI_Line10);
   
	//TIM_Cmd(TIM2,ENABLE ); 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = 104-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =84-1;    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
    
   
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	ModuleTxSet();
	//TIM_Cmd(TIM3,ENABLE);
	//TIM_Cmd(TIM4,ENABLE);
	ModuleCombuf->SPI4BufRxbt = ModuleSPI4Pt;
}//ModuleComInit



/*
---------------------------------------------------------------------
Name		ModuleTIM2_IRQn
Function	TIM3中断
Input		无
OutPut		无
Edit by seleen hu
Date		2019-04-08
---------------------------------------------------------------------
*/
void ModuleTIM4_IRQn(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET){ //溢出中断
		
		ModuleRxdata >>= 1;
		if(ModuleRxPin()){
			ModuleRxdata |= 0x80;
		}
		ModuleRxbit++;
		if(1 == ModuleRxbit){
			TIM_TimeBaseStructure.TIM_Period = 104; 
			TIM_TimeBaseStructure.TIM_Prescaler =84-1;    
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
			TIM_ClearITPendingBit(TIM4,TIM_IT_Update);			
		}else if(ModuleRxbit>=8){
			TIM_Cmd(TIM4,DISABLE);
			ModuleCombuf->ModuleRxBuf[ModuleCombuf->ModuleRxpt] = ModuleRxdata;
			ModuleCombuf->ModuleRxpt = FIFO_pointer(ModuleCombuf->ModuleRxpt, 1, ModuleRxBufSize);
			

			//ModuleRxBuf[ModuleRxpt] = ModuleRxdata;
			//ModuleRxpt = FIFO_pointer(ModuleRxpt, 1, ModuleRxBufSize);
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line10;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
			
			EXTI_ClearITPendingBit(EXTI_Line10);
		}
		//GPIO_ToggleBits(GPIOI, GPIO_Pin_11);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清除中断标志位
}//ModuleTIM4_IRQn

/*
---------------------------------------------------------------------
Name		ModuleTIM3_IRQn
Function	TIM3中断
Input		无
OutPut		无
Edit by seleen hu
Date		2019-04-08
---------------------------------------------------------------------
*/
void ModuleTIM3_IRQn(void)
{
	static uint8_t Txbit,Txdata;
	static uint16_t Txlen;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET){ //溢出中断
		if(0 == Txbit){
			Txbit++;
			Txdata = ModuleCombuf->ModuleTxBuf[Txlen];
			//Txdata = ModuleTxBuf[Txlen];
			ModuleTxClr();
		}else if(Txbit<9){
			if(Txdata & 0x01){
				ModuleTxSet();
			}else{
				ModuleTxClr();
			}
			Txbit++;
			Txdata >>= 1;
		}else if(Txbit<10){
			ModuleTxSet();
			Txbit++;
		}else{			
			Txlen++;			
			if(Txlen >= ModuleCombuf->ModuleTxLen){
				Txbit = 0;
				Txlen = 0;
				
				TIM_Cmd(TIM3,DISABLE);
			}else{
				Txbit = 1;
				Txdata = ModuleCombuf->ModuleTxBuf[Txlen];
				//Txdata = ModuleTxBuf[Txlen];
				ModuleTxClr();
			}
		}
		//GPIO_ToggleBits(GPIOI, GPIO_Pin_11);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位
}//ModuleTIM3_IRQn
/*
---------------------------------------------------------------------
Name		ModuleEXTI10_IRQn
Function	EXTI10中断
Input		无
OutPut		无
Edit by seleen hu
Date		2019-04-08
---------------------------------------------------------------------
*/
void ModuleEXTI10_IRQn(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	if(EXTI_GetITStatus(EXTI_Line10) != RESET){
		ModuleRxbit = 0;
		ModuleRxdata = 0;
		
		TIM_TimeBaseStructure.TIM_Period = 104*3/2; 
		TIM_TimeBaseStructure.TIM_Prescaler =84-1;    
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		TIM_Cmd(TIM4,ENABLE);


		EXTI_InitStructure.EXTI_Line = EXTI_Line10;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
}//ModuleEXTI10_IRQn

/*
---------------------------------------------------------------------
Name		ModuleTxStart
Function	ModuleTx发送
Input		无
OutPut		无
Edit by 	fanzhong
Date		2019-04-08
---------------------------------------------------------------------
*/
uint8_t ModuleTxStart(void)
{
	uint8_t rt;
	rt = 0;
	if(TIM3->CR1 & TIM_CR1_CEN){
		rt = 1;
	}else{		
		TIM_Cmd(TIM3,ENABLE);
	}
	return rt;
}//ModuleTxStart

/*
---------------------------------------------------------------------
Name		ModuleComReadFault
Function	读取错误波形
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-22
---------------------------------------------------------------------
*/
uint8_t ModuleComReadFault(uint8_t FaultType,uint8_t TypeNum)
{
	uint16_t len,crcchucksum;
	uint8_t rt;
	static uint8_t Step,DelayTim;
	switch(Step){
		case 0:
			len = 0;
			ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
			ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
			ModuleCombuf->ModuleTxBuf[len++] = 0x61;
			len++;
			len++;
			ModuleCombuf->ModuleTxBuf[len++] = FaultType;
			ModuleCombuf->ModuleTxBuf[len++] = TypeNum;
			ModuleCombuf->ModuleTxBuf[3] = (len-5);
			ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
			crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
			ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
			ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
			ModuleCombuf->ModuleTxLen = len;
			if(0 == ModuleTxStart()){
				Step++;
			}
			rt = 1;
			break;
		case 1:
			rt = 1;
			if(SPIBitFlag.Bits.S_E1){
				SPIBitFlag.Bits.S_E1 = 0;
				rt = 0;				
			}else{
				DelayTim++;
				if(DelayTim>20){
					Step=0;
					DelayTim = 0;					
					rt = 2;
				}
			}
			break;
	}	
	return rt;
}//ModuleComReadFault

/*
---------------------------------------------------------------------
Name		ModuleComSPIRX
Function	接收
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-22
---------------------------------------------------------------------
*/
uint8_t ModuleComSPIRX(void)
{
	uint8_t rt;
	uint16_t i,j,data16,bt;
	uint8_t *buf;
	int16_t *Ptint16;
	uint8_t k;
	static uint8_t Step,DelayTim;
	static uint16_t len;
	switch(Step){
		case 0:			
			while(FIFO_length(ModuleCombuf->SPI4BufRxbt, ModuleSPI4Pt, SPI4BufSize)>=2){
				bt = ModuleCombuf->SPI4BufRxbt;
				data16 = ModuleCombuf->SPI4Buf[bt];
				data16 <<= 8;
				bt = FIFO_pointer(bt, 1, SPI4BufSize);
				data16 += ModuleCombuf->SPI4Buf[bt];
				if(0xFFCC == data16){
					Step++;
					DelayTim = 0;
					break;
				}else{
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 1, SPI4BufSize);
				}
			}
			rt = 1;
			break;
		case 1:
			if(FIFO_length(ModuleCombuf->SPI4BufRxbt, ModuleSPI4Pt, SPI4BufSize)>=5){
				bt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 4, SPI4BufSize);
				len = ModuleCombuf->SPI4Buf[bt];
				len <<= 8;
				bt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 3, SPI4BufSize);
				len += ModuleCombuf->SPI4Buf[bt];
				Step++;
				DelayTim = 0;				
				rt = 1;
				if(len >= (65535-7)){
					Step=0;
					DelayTim = 0;
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 1, SPI4BufSize);
					rt = 2;
				}
			}else{
				DelayTim++;
				if(DelayTim>20){
					Step=0;
					DelayTim = 0;
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 1, SPI4BufSize);
					rt = 2;
				}
			}
			break;
		case 2:
			rt = 1;
			if(FIFO_length(ModuleCombuf->SPI4BufRxbt, ModuleSPI4Pt, SPI4BufSize)>=len+7){
				buf = ADCWholeVar->SPI4Buf;
				
				bt = ModuleCombuf->SPI4BufRxbt;
				for(i=0;i<len+7;i++){					
					buf[i] = ModuleCombuf->SPI4Buf[bt];
					bt = FIFO_pointer(bt, 1, SPI4BufSize);					
				}
				bt = buf[len+6];
				bt <<= 8;
				bt += buf[len+5];
				data16 = CRC_GetModbus16(buf,len+5);
				if(bt == data16){
					rt = 0;
					switch(buf[2]){
						case 0xC0:
							i = 5;
							memcpy((uint8_t *)&ModuleCombuf->LEDState,buf+i,sizeof(uint16_t));
							i += sizeof(uint16_t);
							SystemState.SwitchState.phaseAction = buf[i++];	

							memcpy((uint8_t *)(&NandCfgData),buf+i,sizeof(NandCfgStruct));
							i += sizeof(NandCfgStruct);
							memcpy((uint8_t *)ModuleCombuf->RMS_buf,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)ModuleCombuf->ratio,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)ModuleCombuf->fftratio,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)SystemPowerQuality,buf+i,sizeof(PowerQualityStruct));
							i += sizeof(PowerQualityStruct);
							memcpy((uint8_t *)ModuleCombuf->CanComdata.CanOnlineTable,buf+i,16);
							i+=16;

							
							if(0 == ModuleInitFlag){
								ModuleInitFlag++;
								SPIBitFlag.Bits.S_11 = 1;
								SPIBitFlag.Bits.S_12 = 1;
							}
							break;
						case 0xC1:
							i = 5;
							memcpy((uint8_t *)&ModuleCombuf->LEDState,buf+i,sizeof(uint16_t));
							i += sizeof(uint16_t);
							SystemState.SwitchState.phaseAction = buf[i++];	
							memcpy((uint8_t *)(&NandCfgData),buf+i,sizeof(NandCfgStruct));
							i += sizeof(NandCfgStruct);
							memcpy((uint8_t *)ModuleCombuf->RMS_buf,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)ModuleCombuf->ratio,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)ModuleCombuf->fftratio,buf+i,8*sizeof(float));
							i += 8*sizeof(float);
							memcpy((uint8_t *)SystemPowerQuality,buf+i,sizeof(PowerQualityStruct));
							i += sizeof(PowerQualityStruct);
							
							memcpy((uint8_t *)ModuleCombuf->wavebuf[0],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[1],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[2],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[3],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[4],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[5],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->wavebuf[6],buf+i,12*64*sizeof(int16_t));
							i += 12*64*sizeof(int16_t);
							memcpy((uint8_t *)ModuleCombuf->CanComdata.CanOnlineTable,buf+i,16);
							i+=16;
							break;
						case 0xC2:
							break;
						case 0xE0:
							memcpy((uint8_t *)(&NandCfgData),buf+5,sizeof(NandCfgStruct));
							//i += sizeof(NandCfgStruct);
							SPIBitFlag.Bits.S_E0 = 1;
							break;
						case 0xE1:
							if(1 == buf[5]){
								memcpy((uint8_t *)ModuleComFaultbuf,buf+6,sizeof(ModuleComFaultStruct));
							}else if(2 == buf[5]){
								memcpy((uint8_t *)&ModuleCombuf->FaultAllSelComBuf,buf+6,sizeof(ModuleComFaultAllSelSaveStruct));
							}
							SPIBitFlag.Bits.S_E1 = 1;
							break;
						case 0xE2:
							i = 7;
							if(0 == buf[6]){
								FaultFifo[FaultFifo_pt]->FaultStyle = buf[i++];
								FaultFifo[FaultFifo_pt]->ValueFlag = buf[i++];
								memcpy((uint8_t *)&FaultFifo[FaultFifo_pt]->Tim,buf+i,sizeof(AlarmTimStruct));
								i += sizeof(AlarmTimStruct);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->ratio,buf+i,8*sizeof(float));
								i += 8*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->fftratio,buf+i,8*sizeof(float));
								i += 8*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->RMSValue,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->VabuleSin,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->VabuleCos,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);								
							}else{
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->Wave[buf[6]-1],buf+7,20*256*sizeof(int16_t));								
							}
							while(0 == ModuleComSet62Fault(buf[6]));
							if(buf[6]+1 >= buf[5]){								
								FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
								FaultFifo_pt++;
								if(FaultFifo_pt>=FaultSaveSIZE){
									FaultFifo_pt = 0;
								}								
								SPIBitFlag.Bits.S_E2 = 1;
								if(Messge_idle == MessgeFlag){
									MessgeFlag = Messge_Start;
								}
							}else{
								FaultFifo[FaultFifo_pt]->DislayFlag = 0x00;
							}
							break;
						case 0xE3:	
							i = 7;
							if(0 == buf[6]){
								FaultFifo[FaultFifo_pt]->FaultStyle = buf[i++];
								FaultFifo[FaultFifo_pt]->ValueFlag = buf[i++];
								memcpy((uint8_t *)&FaultFifo[FaultFifo_pt]->Tim,buf+i,sizeof(AlarmTimStruct));
								i += sizeof(AlarmTimStruct);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->ratio,buf+i,8*sizeof(float));
								i += 8*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->fftratio,buf+i,8*sizeof(float));
								i += 8*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->RMSValue,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->VabuleSin,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->VabuleCos,buf+i,8*15*sizeof(float));
								i += 8*15*sizeof(float);								
							}else if(buf[6]<8){
								memcpy((uint8_t *)FaultFifo[FaultFifo_pt]->Wave[buf[6]-1],buf+7,20*256*sizeof(int16_t));								
							}else if(buf[6]<9){
								FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0] = buf[i++];
								FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1] = buf[i++];
								FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2] = buf[i++];
								FaultAllSelFifo[FaultFifo_pt]->CanId[0] = buf[i++];
								FaultAllSelFifo[FaultFifo_pt]->CanId[1] = buf[i++];
								FaultAllSelFifo[FaultFifo_pt]->CanId[2] = buf[i++];
								memcpy((uint8_t *) &FaultAllSelFifo[FaultFifo_pt]->Tim,buf+i,3*sizeof(AlarmTimStruct));
								i += 3*sizeof(AlarmTimStruct);
								memcpy((uint8_t *) &FaultAllSelFifo[FaultFifo_pt]->ratio,buf+i,3*sizeof(float));
								i += 3*sizeof(float);
								memcpy((uint8_t *) &FaultAllSelFifo[FaultFifo_pt]->RMSValue,buf+i,3*sizeof(float));
								i += 3*sizeof(float);		
							}else{
								/*
								if(0 == FaultAllSelFifo[FaultFifo_pt]->CanId){
									
									memcpy((uint8_t *)(Membuf->ComMembuf),buf+i,20*128*sizeof(int16_t));
									i += 20*128*sizeof(int16_t);
									Ptint16 = (int16_t *)(Membuf->ComMembuf);
									for(j=0;j<20*128;j++){
										FaultAllSelFifo[FaultFifo_pt]->Wave[2*j] = Ptint16[j];
										FaultAllSelFifo[FaultFifo_pt]->Wave[2*j+1] = Ptint16[j];
									}
									
								}else{
									memcpy((uint8_t *)(FaultAllSelFifo[FaultFifo_pt]->Wave),buf+i,20*256*sizeof(int16_t));
									i += 20*256*sizeof(int16_t);
								}
								*/
								#if 0
								for(k=0;k<3;k++){
									if(0 == FaultAllSelFifo[FaultFifo_pt]->CanId[k]){
									
									memcpy((uint8_t *)(Membuf->ComMembuf),buf+i,20*128*sizeof(int16_t));
									i += 20*128*sizeof(int16_t);
									Ptint16 = (int16_t *)(Membuf->ComMembuf);
									for(j=0;j<20*128;j++){
										FaultAllSelFifo[FaultFifo_pt]->Wave[k][2*j] = Ptint16[j];
										FaultAllSelFifo[FaultFifo_pt]->Wave[k][2*j+1] = Ptint16[j];
									}
									
								}else{
									memcpy((uint8_t *)(FaultAllSelFifo[FaultFifo_pt]->Wave[k]),buf+i,20*256*sizeof(int16_t));
									i += 20*256*sizeof(int16_t);
								}
								}
								#else
									if(0 == FaultAllSelFifo[FaultFifo_pt]->CanId[0]){
									
									memcpy((uint8_t *)(Membuf->ComMembuf),buf+i,20*128*sizeof(int16_t));
									i += 20*128*sizeof(int16_t);
									Ptint16 = (int16_t *)(Membuf->ComMembuf);
									for(j=0;j<20*128;j++){
										FaultAllSelFifo[FaultFifo_pt]->Wave[0][2*j] = Ptint16[j];
										FaultAllSelFifo[FaultFifo_pt]->Wave[0][2*j+1] = Ptint16[j];
									}
									
								}else{
									memcpy((uint8_t *)(FaultAllSelFifo[FaultFifo_pt]->Wave[0]),buf+i,20*256*sizeof(int16_t));
									i += 20*256*sizeof(int16_t);
								}
								#endif
							}
							while(0 == ModuleComSet63Fault(buf[6]));
							if(buf[6]+1 >= buf[5]){								
								FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
								FaultFifo_pt++;
								testCnt++;
								if(FaultFifo_pt>=FaultSaveSIZE){
									FaultFifo_pt = 0;
								}								
								SPIBitFlag.Bits.S_E3 = 1;
								if(Messge_idle == MessgeFlag){
									MessgeFlag = Messge_Start;
								}
							}else{
								FaultFifo[FaultFifo_pt]->DislayFlag = 0x00;
							}
							break;
						//===============add by seleen 2022 1 11 	校准RMSValue[16]	
						case 0xE5:
							memcpy(Membuf->RMSValue,buf+5,16*sizeof(float));
							break;
						
					
					}
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, len+7, SPI4BufSize);					
				}else{
					rt = 2;
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 1, SPI4BufSize);
				}
				Step=0;
				DelayTim = 0;				
			}else{
				DelayTim++;
				if(DelayTim>20){
					Step=0;
					DelayTim = 0;
					ModuleCombuf->SPI4BufRxbt = FIFO_pointer(ModuleCombuf->SPI4BufRxbt, 1, SPI4BufSize);
					rt = 2;
				}
			}
			break;
		default:
			Step = 0;
			rt = 2;
			break;
	}
	if(SPIBitFlag.UnionData){
		if(SPIBitFlag.Bits.S_10){
			SPIBitFlag.Bits.S_10 = 0;
			
		}else if(SPIBitFlag.Bits.S_11){
			if(0 == ModuleComSet11Fault()){
				SPIBitFlag.Bits.S_11 = 0;
			}
			
		}else if(SPIBitFlag.Bits.S_12){
			if(0 == ModuleComSet12Fault()){
				SPIBitFlag.Bits.S_12 = 0;
			}			
		}else if(SPIBitFlag.Bits.S_13){			
			SPIBitFlag.Bits.S_13 = 0;
		}
	}
	return rt;
}//ModuleComSPIRX

/*
---------------------------------------------------------------------
Name		ModuleComSet10Fault
Function	命令10设定发送
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-23
---------------------------------------------------------------------
*/
uint8_t ModuleComSet10Fault(uint8_t ComOrder,uint8_t VerifyData,uint8_t SwData)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x10;
		len++;
		len++;
		ModuleCombuf->ModuleTxBuf[len++] = ComOrder;
		ModuleCombuf->ModuleTxBuf[len++] = VerifyData;
		ModuleCombuf->ModuleTxBuf[len++] = SwData;
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet10Fault
/*
---------------------------------------------------------------------
Name		ModuleComSet11Fault
Function	命令11设定发送
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-23
---------------------------------------------------------------------
*/
uint8_t ModuleComSet11Fault(void)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x11;
		len++;
		len++;
		
		memcpy(ModuleCombuf->ModuleTxBuf+len,(uint8_t *)&SystemSet.VoltageSet.MaxRatedValue,sizeof(uint32_t));
		len += sizeof(uint32_t);
		memcpy(ModuleCombuf->ModuleTxBuf+len,(uint8_t *)&SystemSet.VoltageSet.Modulus,sizeof(float));
		len += sizeof(float);
		memcpy(ModuleCombuf->ModuleTxBuf+len,(uint8_t *)&SystemSet.RatioStruct.RatioPT,sizeof(uint16_t));
		len += sizeof(uint16_t);
		memcpy(ModuleCombuf->ModuleTxBuf+len,(uint8_t *)&SystemSet.RatioStruct.RatioCT,sizeof(uint16_t));
		len += sizeof(uint16_t);
		memcpy(ModuleCombuf->ModuleTxBuf+len,(uint8_t *)&SystemSet.ShortGNDSet.I0,sizeof(float));
		len += sizeof(float);	
		
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet11Fault
/*
---------------------------------------------------------------------
Name		ModuleComSet12Fault
Function	命令11设定发送
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-23
---------------------------------------------------------------------
*/
uint8_t ModuleComSet12Fault(void)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	RTC_TimeTypeDef rtctim;
	RTC_DateTypeDef rtcdate;
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		
	
		RTC_GetTime(RTC_Format_BIN,&rtctim);
		RTC_GetDate(RTC_Format_BIN,&rtcdate);
		
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x12;
		len++;
		len++;
		
		ModuleCombuf->ModuleTxBuf[len++] = rtcdate.RTC_Year;
		ModuleCombuf->ModuleTxBuf[len++] = rtcdate.RTC_Month;
		ModuleCombuf->ModuleTxBuf[len++] = rtcdate.RTC_Date;
		ModuleCombuf->ModuleTxBuf[len++] = rtctim.RTC_Hours;
		ModuleCombuf->ModuleTxBuf[len++] = rtctim.RTC_Minutes;
		ModuleCombuf->ModuleTxBuf[len++] = rtctim.RTC_Seconds;
		
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet12Fault

/*
---------------------------------------------------------------------
Name		ModuleComSet13Fault
Function	命令13设定发送
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-04-23
---------------------------------------------------------------------
*/
uint8_t ModuleComSet13Fault(uint16_t outdata,uint8_t Resetdata)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	RTC_TimeTypeDef rtctim;
	RTC_DateTypeDef rtcdate;
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		
	
		RTC_GetTime(RTC_Format_BIN,&rtctim);
		RTC_GetDate(RTC_Format_BIN,&rtcdate);
		
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x13;
		len++;
		len++;
		ModuleCombuf->ModuleTxBuf[len++] = outdata;
		ModuleCombuf->ModuleTxBuf[len++] = outdata>>8;
		ModuleCombuf->ModuleTxBuf[len++] = Resetdata;
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet12Fault


/*
---------------------------------------------------------------------
Name		ModuleComSet62Fault
Function	突发命令回复
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-06-11
---------------------------------------------------------------------
*/
static uint8_t ModuleComSet62Fault(uint8_t bagnums)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x62;
		len++;
		len++;
		ModuleCombuf->ModuleTxBuf[len++] = bagnums;
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet62Fault
/*
---------------------------------------------------------------------
Name		ModuleComSet63Fault
Function	突发命令回复
Input		读取类型,读取条数
OutPut		0 成功  1 等待 >=2 错误
Edit by     fanzhong
Date		2019-06-11
---------------------------------------------------------------------
*/
static uint8_t ModuleComSet63Fault(uint8_t bagnums)
{
	uint8_t rt;
	uint16_t len,crcchucksum;
	
	if(0 == (TIM3->CR1 & TIM_CR1_CEN)){
		len = 0;
		ModuleCombuf->ModuleTxBuf[len++] = 0xFF;
		ModuleCombuf->ModuleTxBuf[len++] = 0xCC;
		ModuleCombuf->ModuleTxBuf[len++] = 0x63;
		len++;
		len++;
		ModuleCombuf->ModuleTxBuf[len++] = bagnums;
		ModuleCombuf->ModuleTxBuf[3] = (len-5);
		ModuleCombuf->ModuleTxBuf[4] = (len-5)>>8;
		crcchucksum = CRC_GetModbus16(ModuleCombuf->ModuleTxBuf,len);
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum;
		ModuleCombuf->ModuleTxBuf[len++] = crcchucksum>>8;
		ModuleCombuf->ModuleTxLen = len;
		ModuleTxStart();
		rt = 0;
	}else{
		rt = 1;
	}
	return rt;
}//ModuleComSet63Fault


