#include  <bsp.h>

//#define RX2buf_len 256
//static u8 RX2buf[RX2buf_len];
//static u8 RX2buf_bt;

#define RF433BuffSize			1024
static u16 RF433RX1Bt;
#define  RF433RX1Size()     (RF433BuffSize-DMA_GetCurrDataCounter(DMA1_Stream5))
u8 RF433RX1Buff[RF433BuffSize];
static u8 RF433TX1Buff[RF433BuffSize];

//static u16 RF433RX2Bt;
//#define  RF433RX2Size()     (RF433BuffSize-DMA_GetCurrDataCounter(DMA1_Stream2))
//u8 RF433RX2Buff[RF433BuffSize];
//static u8 RF433TX2Buff[RF433BuffSize];

RF433Struct RF433SD[16];
uint8_t RF433SD_pt;



#define RFPowerON()			GPIO_SetBits(GPIOD,GPIO_Pin_7)
#define RFPowerOFF()		GPIO_ResetBits(GPIOD,GPIO_Pin_7)

#define SPI4_CS_SET()		GPIO_SetBits(GPIOE,GPIO_Pin_4)
#define SPI4_CS_CLR()		GPIO_ResetBits(GPIOE,GPIO_Pin_4)

//static void RF433Send(u8 *TxU4buf,u16 TxU4buf_len);

static void  RF433SendTx1buf(u16 buflen);
//static void  RF433SendTx2buf(u16 buflen);
static void RF433Eric200(uint8_t *buf, uint8_t buflen,u8 flag);
static void RFOntimeJudge(void);
/*
---------------------------------------------------------------------
Name		InitRF433
Function	初始化RF
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-19
---------------------------------------------------------------------
*/
void InitRF433(void)
{
		USART_InitTypeDef USART_InitStructure;
		
		RFPowerON();
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ; 
		USART_Init(USART2, &USART_InitStructure);
#if	 0
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
		USART_Init(UART4, &USART_InitStructure);
#endif		
		
		
		//NVIC_InitTypeDef NVIC_InitStructure;
		
		//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		
		//NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; 
		//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		//NVIC_Init(&NVIC_InitStructure); 
		
		
		

		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
#if 0	
		DMA_DeInit(DMA1_Stream2);
		while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
	
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART4->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RF433RX2Buff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = RF433BuffSize;
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
		DMA_Init(DMA1_Stream2, &DMA_InitStructure);
		
		USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(DMA1_Stream2, ENABLE);
		
		
		DMA_DeInit(DMA1_Stream4);
		while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);		
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART4->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RF433TX2Buff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_BufferSize = RF433BuffSize;
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
#endif		
		
		
		
		
		
		
		DMA_DeInit(DMA1_Stream5);
		while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
	
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RF433RX1Buff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = RF433BuffSize;
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
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RF433TX1Buff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_BufferSize = RF433BuffSize;
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
		
		
		USART_Cmd(USART2, ENABLE);
		
		USART_Cmd(UART4, ENABLE);

}	

/*
---------------------------------------------------------------------
Name		RF433SendTx1buf
Function	数据发送TXBUF
Input		数据buf 数据长度
OutPut	No
Edit by fanzhong
Date		2018-08-21
---------------------------------------------------------------------
*/
static void  RF433SendTx1buf(u16 buflen)
{	
	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA1_Stream6, DISABLE);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	
	DMA_SetCurrDataCounter(DMA1_Stream6,buflen); 
	DMA_Cmd(DMA1_Stream6, ENABLE); 	
	//while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET);
	//while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
	
	
}//RF433SendTx1buf
#if 0
/*
---------------------------------------------------------------------
Name		RF433SendTx1buf
Function	数据发送TXBUF
Input		数据buf 数据长度
OutPut	No
Edit by fanzhong
Date		2018-08-21
---------------------------------------------------------------------
*/
static void  RF433SendTx2buf(u16 buflen)
{	
	
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA1_Stream4, DISABLE);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	
	DMA_SetCurrDataCounter(DMA1_Stream4,buflen); 
	DMA_Cmd(DMA1_Stream4, ENABLE); 	
	//while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)!=RESET);
	//while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);
	
	
}//RF433SendTx2buf


/*
---------------------------------------------------------------------
Name		RF433Send
Function	发送数据
Input		数据buf，数据长度
OutPut	No
Edit by fanzhong
Date		2017-12-19
---------------------------------------------------------------------
*/
static void RF433Send(u8 *TxU4buf,u16 TxU4buf_len)
{
	u16 i;
	for(i=0;i<TxU4buf_len;i++){
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC) == RESET);
		USART_SendData(UART4, TxU4buf[i]);		
	}
}
#endif

/*
---------------------------------------------------------------------
Name		RF433Task
Function	RF433任务处理
Input		No
OutPut	No
Edit by fanzhong
Date		2018-08-22
---------------------------------------------------------------------
*/
void RF433Task(void)
{
	
	u8 i;
	//u8 cup_i,cup_j;
	u16 bt,Crcdata1,Crcdata2;//,data16;
	//s16 value;
	u32 data32;
	static u8 RX1Sequence,RX1_tim;
	static u8 RX1len;
	//static u8 RX2Sequence,RX2_tim;
	//static u8 RX2len;
	u8 CMPbuf[] = {0xFF,0xBB};
	u8 *buf,*TXpt,len;	
	//AlarmTimStruct RFTim;
	static u8 JudeTime;
	
	
	
	buf = Membuf->RF433buf;
	

	if(Relay[0].OnLineTime < 65000){
		Relay[0].OnLineTime++;
		if(6000 == Relay[0].OnLineTime){
			Relay[0].RelayState = 0;
		}
	}
	if(Relay[1].OnLineTime < 65000){
		Relay[1].OnLineTime++;
		if(6000 == Relay[1].OnLineTime){
			Relay[1].RelayState = 0;
		}
	}
	
	/*ERIC200*/
	RX1_tim++;
	switch(RX1Sequence){
		case 0:			
			while(FIFO_length(RF433RX1Bt,RF433RX1Size(),RF433BuffSize)>=2){
				if(0 == FIFO_bufcmp(RF433RX1Buff,CMPbuf,RF433RX1Bt,2,RF433BuffSize)){
					RX1Sequence++;
					break;
				}
				RF433RX1Bt = FIFO_pointer(RF433RX1Bt,1,RF433BuffSize);
			}
			RX1_tim = 0;
			break;
		case 1:
			if(FIFO_length(RF433RX1Bt,RF433RX1Size(),RF433BuffSize) >= 4){
				bt = RF433RX1Bt;
				bt = FIFO_pointer(bt,3,RF433BuffSize);
				RX1len = RF433RX1Buff[bt];
				RX1Sequence++;
				RX1_tim = 0;
				if(RX1len>=128){
					RX1Sequence =0;
					RF433RX1Bt = FIFO_pointer(RF433RX1Bt,2,RF433BuffSize);
				}
			}else{
				if(RX1_tim>=5){
					RX1Sequence =0;
					RF433RX1Bt = FIFO_pointer(RF433RX1Bt,2,RF433BuffSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(RF433RX1Bt,RF433RX1Size(),RF433BuffSize) >= RX1len+6){
				bt = RF433RX1Bt;
				for(i=0;i<(RX1len+6);i++){
					buf[i] = RF433RX1Buff[bt];
					bt = FIFO_pointer(bt,1,RF433BuffSize);
				}
				
				Crcdata1 = CRC_GetModbus16(buf, RX1len+4);
				Crcdata2 = (((u16)buf[RX1len+5]) << 8) + buf[RX1len+4];
				if(Crcdata1 == Crcdata2){				
					RF433RX1Bt = FIFO_pointer(RF433RX1Bt,RX1len+6,RF433BuffSize);
					data32 = buf[7];
					data32 <<= 8;
					data32 += buf[6];
					data32 <<= 8;
					data32 += buf[5];
					data32 <<= 8;
					data32 += buf[4];
					if((data32 == Relay[0].RelayId)||(data32 == Relay[1].RelayId)){
						switch(buf[2]){
							case 0xF1:
								TXpt = RF433TX1Buff;
								i=0;
								TXpt[i++] = 0xFF;
								TXpt[i++] = 0xBB;
								TXpt[i++] = 0x21;
								i++;
								TXpt[i++] = data32;
								TXpt[i++] = data32>>8;
								TXpt[i++] = data32>>16;
								TXpt[i++] = data32>>24;
								TXpt[i++] = 0;
								TXpt[3] = i-4;
								Crcdata1 = CRC_GetModbus16(TXpt, i);
								TXpt[i++] = Crcdata1;
								TXpt[i++] = Crcdata1>>8;
								RF433SendTx1buf(i);
								if(data32 == Relay[0].RelayId){
									Relay[0].RelayState = 1;
									Relay[0].OnLineTime = 0;
								}else{
									Relay[1].RelayState = 1;
									Relay[1].OnLineTime = 0;
								}
								break;
							case 0xF2:
								if(data32 == Relay[0].RelayId){
									Relay[0].RelayState = 1;
									Relay[0].OnLineTime = 0;
								}else{
									Relay[1].RelayState = 1;
									Relay[1].OnLineTime = 0;
								}
								break;
							case 0xC0:
								len = buf[3];
								for(i=0;i<len;i++){
									buf[i] = buf[i+4];
								}
								if((0xFF == buf[0])&&(0xBB == buf[1])){
									data32 = buf[7];
									data32 <<= 8;
									data32 += buf[6];
									data32 <<= 8;
									data32 += buf[5];
									data32 <<= 8;
									data32 += buf[4];									
								}
								RF433Eric200(buf, buf[3]+6,0);
								break;													
						}
					}else if( 0 ==  (0x80 & buf[2])){
						RF433Eric200(buf, buf[3]+6,1);						
					}					
				}else{
					RF433RX1Bt = FIFO_pointer(RF433RX1Bt,2,RF433BuffSize);
				}					
				RX1Sequence = 0;
			}else{
				if(RX1_tim>=5){
					RX1Sequence =0;
					RF433RX1Bt = FIFO_pointer(RF433RX1Bt,2,RF433BuffSize);
				}
			}
			break;
	}
#if 0
	/*ERIC300*/
	RX2_tim++;
	switch(RX2Sequence){
		case 0:
			while(FIFO_length(RF433RX2Bt,RF433RX2Size(),RF433BuffSize)>=2){
				if(0 == FIFO_bufcmp(RF433RX2Buff,CMPbuf,RF433RX2Bt,2,RF433BuffSize)){
					RX2Sequence++;
					break;
				}
				RF433RX2Bt = FIFO_pointer(RF433RX2Bt,1,RF433BuffSize);
			}
			RX2_tim = 0;
			break;
		case 1:
			if(FIFO_length(RF433RX2Bt,RF433RX2Size(),RF433BuffSize) >= 4){
				bt = RF433RX2Bt;
				bt = FIFO_pointer(bt,3,RF433BuffSize);
				RX2len = RF433RX2Buff[bt];
				RX2Sequence++;
				RX2_tim = 0;
				if(RX2len >= 128){
					RX2Sequence =0;
					RF433RX2Bt = FIFO_pointer(RF433RX2Bt,2,RF433BuffSize);
				}
			}else{
				if(RX2_tim>=5){
					RX2Sequence =0;
					RF433RX2Bt = FIFO_pointer(RF433RX2Bt,2,RF433BuffSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(RF433RX2Bt,RF433RX2Size(),RF433BuffSize) >= RX2len+6){
				bt = RF433RX2Bt;
				for(i=0;i<(RX2len+6);i++){
					buf[i] = RF433RX2Buff[bt];
					bt = FIFO_pointer(bt,1,RF433BuffSize);
				}
				
				Crcdata1 = CRC_GetModbus16(buf, RX2len+4);
				Crcdata2 = (((u16)buf[RX2len+5]) << 8) + buf[RX2len+4];
				if(Crcdata1 == Crcdata2){
					RF433RX2Bt = FIFO_pointer(RF433RX2Bt,RX2len+6,RF433BuffSize);
					if(0 == (0x80 & buf[2])){
						data32 = buf[7];
						data32 <<= 8;
						data32 += buf[6];
						data32 <<= 8;
						data32 += buf[5];
						data32 <<= 8;
						data32 += buf[4];
						
						for(cup_i=0;cup_i<SystemSet.CupboardSIZE;cup_i++){
							for(cup_j=0;cup_j<12;cup_j++){
								if(data32 == (Cupboard_P[cup_i][cup_j]).Id){
									goto RF433RX2_Normal;
								}
							}
						}	
						
					RF433RX2_Normal:
					if(cup_i<20){
						if('B' == (data32>>24)){
								RFReception->RFTem.Id = data32;
								RFTim = SysGetTime();
								TXpt = RF433TX1Buff;
								i=0;
								TXpt[i++] = 0xFF;
								TXpt[i++] = 0xBB;
								if((0x40 == buf[2])||(0x41 == buf[2])){
									TXpt[i++] = 0x40 | 0x80;
								}else{
									TXpt[i++] = buf[2] | 0x80;
								}
								TXpt[i++] = 0x0A;
								TXpt[i++] = data32;
								TXpt[i++] = data32>>8;
								TXpt[i++] = data32>>16;
								TXpt[i++] = data32>>24;
								TXpt[i++] = RFTim.year;
								TXpt[i++] = RFTim.month;
								TXpt[i++] = RFTim.day;
								TXpt[i++] = RFTim.hour;
								TXpt[i++] = RFTim.minute;
								TXpt[i++] = RFTim.second;
								Crcdata1 = CRC_GetModbus16(TXpt, i);
								TXpt[i++] = Crcdata1;
								TXpt[i++] = Crcdata1>>8;
								RF433SendTx1buf(i);
								
															
								switch(buf[2]){									
									case 0x40:
										data16 = buf[9];
										data16 <<=8;
										data16 += buf[8];
										value = (s16)data16;
										RFReception->Protector.ProtectorTem.TemA = value;																
										RFReception->Protector.ProtectorTem.HumA = buf[10];
										
										data16 = buf[12];
										data16 <<=8;
										data16 += buf[11];
										value = (s16)data16;
										RFReception->Protector.ProtectorTem.TemB = value;									
										RFReception->Protector.ProtectorTem.HumB = buf[13];
										
										data16 = buf[15];
										data16 <<=8;
										data16 += buf[14];
										value = (s16)data16;								
										RFReception->Protector.ProtectorTem.TemC = value;									
										RFReception->Protector.ProtectorTem.HumC = buf[16];
										
										RFReception->Protector.ProtectorTem.LeakageCurrent = buf[18];
										RFReception->Protector.ProtectorTem.LeakageCurrent <<= 8;
										RFReception->Protector.ProtectorTem.LeakageCurrent += buf[17];
										
										RFReception->Protector.ProtectorTem.BatteryVoltage = buf[20];
										RFReception->Protector.ProtectorTem.BatteryVoltage <<= 8;
										RFReception->Protector.ProtectorTem.BatteryVoltage += buf[19];
										
										RFTim.year = buf[21];
										RFTim.month = buf[22];
										RFTim.day = buf[23];
										RFTim.hour = buf[24];
										RFTim.minute = buf[25];
										RFTim.second = buf[26];
										
										RFReception->Protector.ProtectorTem.Id = (Cupboard_P[cup_i][cup_j]).Id;
										RFReception->Protector.ProtectorTem.Tim = RFTim;
										RFProtectorTemSend(cup_i,cup_j,&RFReception->Protector.ProtectorTem);
										
										RF433SD[RF433SD_pt].i = cup_i;
										RF433SD[RF433SD_pt].j = cup_j;
										RF433SD[RF433SD_pt].flag = SaveProtectorCurrentTime;									
										RF433SD_pt++;
										RF433SD_pt &= 0x0F;
										break;										
									case 0x60:
										RFTim.year = buf[8];
										RFTim.month = buf[9];
										RFTim.day = buf[10];
										RFTim.hour = buf[11];
										RFTim.minute = buf[12];
										RFTim.second = buf[13];
										RFReception->Protector.ProtectorAction.Tim = RFTim;
										switch(buf[14] & 0x0F){
											case 0:
												RFReception->Protector.ProtectorAction.Phase = PhaseA;
												break;
											case 1:
												RFReception->Protector.ProtectorAction.Phase = PhaseB;
												break;
											default:
												RFReception->Protector.ProtectorAction.Phase = PhaseC;
										}
										RFReception->Protector.ProtectorAction.ActionTime = buf[16];
										RFReception->Protector.ProtectorAction.ActionTime <<= 8;
										RFReception->Protector.ProtectorAction.ActionTime += buf[15];
										RFReception->Protector.ProtectorAction.Id = (Cupboard_P[cup_i][cup_j]).Id;
										
										RFProtectorActionSend(cup_i,cup_j,&RFReception->Protector.ProtectorAction);
										RFReception->Protector.ProtectorAction.DislayFlag = 0xFF;
										
										RF433SD[RF433SD_pt].i = cup_i;
										RF433SD[RF433SD_pt].j = cup_j;
										RF433SD[RF433SD_pt].flag = SaveProtectorSudden;									
										RF433SD_pt++;
										RF433SD_pt &= 0x0F;
										break;
									default:;
								}
							}
						}
					}
				}else{
					RF433RX2Bt = FIFO_pointer(RF433RX2Bt,2,RF433BuffSize);
				}					
				RX2Sequence = 0;
			}else{
				if(RX2_tim>=5){
					RX2Sequence =0;
					RF433RX2Bt = FIFO_pointer(RF433RX2Bt,2,RF433BuffSize);
					
				}
			}
			break;
	}
#endif
	JudeTime++;
	if(JudeTime>=10){
		JudeTime = 0;
		
		TemExcessJudge();
	}
	RFOntimeJudge();
}//RF433Task

/*
--------------------------------------------------------------------
Name		RF433Eric200
Function	解析
Input		No
OutPut	No
Edit by fanzhong
Date		2018-09-27
--------------------------------------------------------------------
*/
static void RF433Eric200(uint8_t *buf, uint8_t buflen,u8 flag)
{
	u8 cup_i,cup_j,i;
	u32 data32;
	uint16_t data16,Crcdata1;
	u8 rssidata;
	u8 *TXpt;
	s16 value;
	AlarmTimStruct RFTim;
	if((0 ==  (0x80 & buf[2]) )&& buflen >= 12){
		data32 = buf[7];
		data32 <<= 8;
		data32 += buf[6];
		data32 <<= 8;
		data32 += buf[5];
		data32 <<= 8;
		data32 += buf[4];		
		for(cup_i=0;cup_i<SystemSet.CupboardSIZE;cup_i++){
			for(cup_j=0;cup_j<12;cup_j++){				
				if(data32 == (Cupboard_P[cup_i][cup_j]).Id){
						RFReception->RFTem.Id = data32;
						if('B' == (data32>>24)){							
							RFReception->RFTem.Id = data32;
							RFTim = SysGetTime();
							TXpt = RF433TX1Buff;
							i=0;
							TXpt[i++] = 0xFF;
							TXpt[i++] = 0xBB;
							if((0x40 == buf[2])||(0x41 == buf[2])){
								TXpt[i++] = 0x40 | 0x80;
							}else{
								TXpt[i++] = buf[2] | 0x80;
							}
							TXpt[i++] = 0x0A;
							TXpt[i++] = data32;
							TXpt[i++] = data32>>8;
							TXpt[i++] = data32>>16;
							TXpt[i++] = data32>>24;
							TXpt[i++] = RFTim.year;
							TXpt[i++] = RFTim.month;
							TXpt[i++] = RFTim.day;
							TXpt[i++] = RFTim.hour;
							TXpt[i++] = RFTim.minute;
							TXpt[i++] = RFTim.second;
							Crcdata1 = CRC_GetModbus16(TXpt, i);
							TXpt[i++] = Crcdata1;
							TXpt[i++] = Crcdata1>>8;
							RF433SendTx1buf(i);		
							switch(buf[2]){									
								case 0x40:
									data16 = buf[9];
									data16 <<=8;
									data16 += buf[8];
									value = (s16)data16;
									RFReception->Protector.ProtectorTem.TemA = value;																
									RFReception->Protector.ProtectorTem.HumA = buf[10];
									
									data16 = buf[12];
									data16 <<=8;
									data16 += buf[11];
									value = (s16)data16;
									RFReception->Protector.ProtectorTem.TemB = value;									
									RFReception->Protector.ProtectorTem.HumB = buf[13];
									
									data16 = buf[15];
									data16 <<=8;
									data16 += buf[14];
									value = (s16)data16;								
									RFReception->Protector.ProtectorTem.TemC = value;									
									RFReception->Protector.ProtectorTem.HumC = buf[16];
									
									RFReception->Protector.ProtectorTem.LeakageCurrent = buf[18];
									RFReception->Protector.ProtectorTem.LeakageCurrent <<= 8;
									RFReception->Protector.ProtectorTem.LeakageCurrent += buf[17];
									
									RFReception->Protector.ProtectorTem.BatteryVoltage = buf[20];
									RFReception->Protector.ProtectorTem.BatteryVoltage <<= 8;
									RFReception->Protector.ProtectorTem.BatteryVoltage += buf[19];
									
									RFTim.year = buf[21];
									RFTim.month = buf[22];
									RFTim.day = buf[23];
									RFTim.hour = buf[24];
									RFTim.minute = buf[25];
									RFTim.second = buf[26];
									
									RFReception->Protector.ProtectorTem.Id = (Cupboard_P[cup_i][cup_j]).Id;
									RFReception->Protector.ProtectorTem.Tim = RFTim;
									RFProtectorTemSend(cup_i,cup_j,&RFReception->Protector.ProtectorTem);
									
									RF433SD[RF433SD_pt].i = cup_i;
									RF433SD[RF433SD_pt].j = cup_j;
									RF433SD[RF433SD_pt].flag = SaveProtectorCurrentTime;									
									RF433SD_pt++;
									RF433SD_pt &= 0x0F;
									break;										
								case 0x60:
									RFTim.year = buf[8];
									RFTim.month = buf[9];
									RFTim.day = buf[10];
									RFTim.hour = buf[11];
									RFTim.minute = buf[12];
									RFTim.second = buf[13];
									RFReception->Protector.ProtectorAction.Tim = RFTim;
									switch(buf[14] & 0x0F){
										case 0:
											RFReception->Protector.ProtectorAction.Phase = PhaseA;
											break;
										case 1:
											RFReception->Protector.ProtectorAction.Phase = PhaseB;
											break;
										default:
											RFReception->Protector.ProtectorAction.Phase = PhaseC;
									}
									RFReception->Protector.ProtectorAction.ActionTime = buf[16];
									RFReception->Protector.ProtectorAction.ActionTime <<= 8;
									RFReception->Protector.ProtectorAction.ActionTime += buf[15];
									RFReception->Protector.ProtectorAction.Id = (Cupboard_P[cup_i][cup_j]).Id;
									
									RFProtectorActionSend(cup_i,cup_j,&RFReception->Protector.ProtectorAction);
									RFReception->Protector.ProtectorAction.DislayFlag = 0xFF;
									
									RF433SD[RF433SD_pt].i = cup_i;
									RF433SD[RF433SD_pt].j = cup_j;
									RF433SD[RF433SD_pt].flag = SaveProtectorSudden;									
									RF433SD_pt++;
									RF433SD_pt &= 0x0F;
									break;
								default:;
							}
						}else if('E' == (data32>>24)){
							//rssidata = buf[12];
							data16 = buf[9];
							data16 <<= 8;
							data16 += buf[8];
							Cupboard_P[cup_i][cup_j].Value.TempCupboard = (int16_t)data16;
							data16 = buf[11];
							data16 <<= 8;
							data16 += buf[10];
							Cupboard_P[cup_i][cup_j].OnlineFlag=1;
							Cupboard_P[cup_i][cup_j].KeepTim = 0;
							Cupboard_P[cup_i][cup_j].Value.HumdityCupboard = (int16_t)data16;
							Cupboard_P[cup_i][cup_j].TimeStamp = SysGetTime();
						}else if(('A' == (data32>>24))||('C' == (data32>>24))||('D' == (data32>>24))){
							rssidata = buf[12];
							data16 = buf[9];
							data16 <<= 8;
							data16 += buf[8];
							RFReception->RFTem.TemValue = (int16_t)data16;									
							RFReception->RFTem.TimeStamp = SysGetTime();								
							RFTemSend(cup_i,cup_j,&(RFReception->RFTem));
															
							RF433SD[RF433SD_pt].i = cup_i;
							RF433SD[RF433SD_pt].j = cup_j;
							RF433SD[RF433SD_pt].flag = SaveTem;
										
							RF433SD_pt++;
							RF433SD_pt &= 0x0F;								
							if((rssidata<90)&&(0 == (Cupboard_P[cup_i][cup_j]).RelayFlag) && flag){
								(Cupboard_P[cup_i][cup_j]).RelayFlag = 1;
								if(Relay[0].RelayState){
										i=0;
										RF433TX1Buff[i++] = 0xFF;
										RF433TX1Buff[i++] = 0xBB;
										RF433TX1Buff[i++] = 0x11;
										i++;
										RF433TX1Buff[i++] = Relay[0].RelayId;
										RF433TX1Buff[i++] = Relay[0].RelayId>>8;
										RF433TX1Buff[i++] = Relay[0].RelayId>>16;
										RF433TX1Buff[i++] = Relay[0].RelayId>>24;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>8;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>16;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>24;											
										RF433TX1Buff[3] = i-4;
										data16 = CRC_GetModbus16(RF433TX1Buff, i);
										RF433TX1Buff[i++] = data16;
										RF433TX1Buff[i++] = data16>>8;
										RF433SendTx1buf(i);
								}
								if(Relay[1].RelayState){
										i=0;
										RF433TX1Buff[i++] = 0xFF;
										RF433TX1Buff[i++] = 0xBB;
										RF433TX1Buff[i++] = 0x11;
										i++;
										RF433TX1Buff[i++] = Relay[1].RelayId;
										RF433TX1Buff[i++] = Relay[1].RelayId>>8;
										RF433TX1Buff[i++] = Relay[1].RelayId>>16;
										RF433TX1Buff[i++] = Relay[1].RelayId>>24;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>8;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>16;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>24;											
										RF433TX1Buff[3] = i-4;
										data16 = CRC_GetModbus16(RF433TX1Buff, i);
										RF433TX1Buff[i++] = data16;
										RF433TX1Buff[i++] = data16>>8;
										RF433SendTx1buf(i);
								}
							}
						}else if(isalpha(data32>>24)){
							rssidata = buf[12];
							data16 = buf[9];
							data16 <<= 8;
							data16 += buf[8];
							RFReception->RFTem.TemValue = (int16_t)data16;									
							RFReception->RFTem.TimeStamp = SysGetTime();								
							RFTemSend(cup_i,cup_j,&(RFReception->RFTem));
															
							RF433SD[RF433SD_pt].i = cup_i;
							RF433SD[RF433SD_pt].j = cup_j;
							RF433SD[RF433SD_pt].flag = SaveTem;
										
							RF433SD_pt++;
							RF433SD_pt &= 0x0F;								
							if((rssidata<90)&&(0 == (Cupboard_P[cup_i][cup_j]).RelayFlag) && flag){
								(Cupboard_P[cup_i][cup_j]).RelayFlag = 1;
								if(Relay[0].RelayState){
										i=0;
										RF433TX1Buff[i++] = 0xFF;
										RF433TX1Buff[i++] = 0xBB;
										RF433TX1Buff[i++] = 0x11;
										i++;
										RF433TX1Buff[i++] = Relay[0].RelayId;
										RF433TX1Buff[i++] = Relay[0].RelayId>>8;
										RF433TX1Buff[i++] = Relay[0].RelayId>>16;
										RF433TX1Buff[i++] = Relay[0].RelayId>>24;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>8;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>16;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>24;											
										RF433TX1Buff[3] = i-4;
										data16 = CRC_GetModbus16(RF433TX1Buff, i);
										RF433TX1Buff[i++] = data16;
										RF433TX1Buff[i++] = data16>>8;
										RF433SendTx1buf(i);
								}
								if(Relay[1].RelayState){
										i=0;
										RF433TX1Buff[i++] = 0xFF;
										RF433TX1Buff[i++] = 0xBB;
										RF433TX1Buff[i++] = 0x11;
										i++;
										RF433TX1Buff[i++] = Relay[1].RelayId;
										RF433TX1Buff[i++] = Relay[1].RelayId>>8;
										RF433TX1Buff[i++] = Relay[1].RelayId>>16;
										RF433TX1Buff[i++] = Relay[1].RelayId>>24;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>8;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>16;
										RF433TX1Buff[i++] = (Cupboard_P[cup_i][cup_j]).Id>>24;											
										RF433TX1Buff[3] = i-4;
										data16 = CRC_GetModbus16(RF433TX1Buff, i);
										RF433TX1Buff[i++] = data16;
										RF433TX1Buff[i++] = data16>>8;
										RF433SendTx1buf(i);
								}
							}

						}
				}	
			}
		}
	}
	
}






/*
---------------------------------------------------------------------
Name		TemExcessJudge
Function	超温判断
Input		No
OutPut	No
Edit by fanzhong
Date		2018-04-18
---------------------------------------------------------------------
*/
extern int HaveAlarm[];
void TemExcessJudge(void)
{
	u8 i,j,flag;
	u16 Temflag,Poroterflag;
	float TemValue;
	
	
	Temflag = 0;
	for(i=0;i<SystemSet.CupboardSIZE;i++){
		HaveAlarm[i] = 0;
		for(j=0;j<12;j++){			
			switch(Cupboard_P[i][j].Type){
				case Bus_A:
						TemValue = Cupboard_P[i][j].Value.TempBusA;
						flag++;
						break;				
				case Bus_B:
						TemValue = Cupboard_P[i][j].Value.TempBusB;
						flag++;
						break;
				case Bus_C:
						TemValue = Cupboard_P[i][j].Value.TempBusC;	
						flag++;
						break;
				case TouchUp_A:
						TemValue = Cupboard_P[i][j].Value.TempTouchUpA;	
						flag++;
						break;
				case TouchUp_B:
						TemValue = Cupboard_P[i][j].Value.TempTouchUpB;
						flag++;
						break;
				case TouchUp_C:
						TemValue = Cupboard_P[i][j].Value.TempTouchUpC;
						flag++;
						break;
				case TouchDown_A:
						TemValue = Cupboard_P[i][j].Value.TempTouchDownA;	
						flag++;
						break;
				case TouchDown_B:
						TemValue = Cupboard_P[i][j].Value.TempTouchDownB;
						flag++;
						break;
				case TouchDown_C:				
						TemValue = Cupboard_P[i][j].Value.TempTouchDownC;
						flag++;
						break;
				case Cupboard_T:
						TemValue = Cupboard_P[i][j].Value.TempCupboard;
						flag++;
						break;
				case Cupboard_H:
						flag = 0;
						//TemValue = Cupboard_P[i][j].Value.HumdityCupboard;
						//if((TemValue < SystemSet.HumiditySet.MinValue)||
						//   (TemValue > SystemSet.HumiditySet.MaxValue)){
						//   HaveAlarm[i] = 1;
						//}
						break;
				default:
					flag = 0;
					  
				
			}
			
			if((Cupboard_P[i][j].OnlineFlag) && flag){
				TemValue = TemValue/RfTemRatio;
				if(TemValue> SystemSet.LineTempMax){
						
					Temflag++;
					HaveAlarm[i] = 1;
									
				}
			}
		}
	}
	Poroterflag = 0;
	for(i=0;i<20;i++){
		for(j=0;j<12;j++){
			if(Cupboard_P[i][j].OnlineFlag){
				if(OverVoltage_P == Cupboard_P[i][j].Type){
					TemValue = Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA;
					TemValue = TemValue/RfProtectorTemRatio;
					if((TemValue>=-40) && (TemValue<130)){
						if((TemValue> SystemSet.TempSet.MaxValue)||
							 (TemValue< SystemSet.TempSet.MinValue)){
							
							Poroterflag++;
							HaveAlarm[i] = 1;
							
						}
					}
					TemValue = Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB;
					TemValue = TemValue/RfProtectorTemRatio;
					if((TemValue>=-40) && (TemValue<130)){
						if((TemValue> SystemSet.TempSet.MaxValue)||
							 (TemValue< SystemSet.TempSet.MinValue)){
							
							Poroterflag++;
							HaveAlarm[i] = 1;
							
						}
					}
					TemValue = Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC;
					TemValue = TemValue/RfProtectorTemRatio;
					if((TemValue>=-40) && (TemValue<130)){
						if((TemValue> SystemSet.TempSet.MaxValue)||
							 (TemValue< SystemSet.TempSet.MinValue)){							
							Poroterflag++;
							HaveAlarm[i] = 1;
							
						}
					}
					if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA < SystemSet.HumiditySet.MinValue) ||
					   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA > SystemSet.HumiditySet.MaxValue)){
					   Poroterflag++;
						HaveAlarm[i] = 1;
					}
					if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB < SystemSet.HumiditySet.MinValue) ||
					   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB > SystemSet.HumiditySet.MaxValue)){
					   Poroterflag++;
						HaveAlarm[i] = 1;
					}
					if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC < SystemSet.HumiditySet.MinValue) ||
					   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC > SystemSet.HumiditySet.MaxValue)){
					   Poroterflag++;
					   HaveAlarm[i] = 1;
					} 
					if(Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent > SystemSet.LeakageCurrentSet.LeakageValue){
					   Poroterflag++;
					   HaveAlarm[i] = 1;
					}

					
				}
				
			}
		}
	}
	if(Temflag){
		AlarmOverTemON();
	}else{
		AlarmOverTemOFF();
	}
	
	if(Poroterflag){
		ProtectorON();
	}else{
		ProtectorOFF();
	}
	
	if(Temflag || Poroterflag){
		Alarm_LED_ON();	
	}else{
		Alarm_LED_OFF();
	}
}//TemExcessJudge

/*
---------------------------------------------------------------------
Name		RFRXJudge
Function	RF接收数据判断
Input		No
OutPut	No
Edit by fanzhong
Date		2019-05-30
---------------------------------------------------------------------
*/
uint8_t RFRXJudge(CupboardParameter CupboardData,float *TemData,uint8_t *HemData)
{
	uint8_t flag;
	float TemValue;
	
	flag = 0;
	*TemData = 0;
	*HemData = 0;
	switch(CupboardData.Type){
		case Bus_A:
			TemValue = CupboardData.Value.TempBusA;
			flag = 1;
			break;				
		case Bus_B:
			TemValue = CupboardData.Value.TempBusB;
			flag = 1;
			break;
		case Bus_C:
			TemValue = CupboardData.Value.TempBusC;	
			flag = 1;
			break;
		case TouchUp_A:
			TemValue = CupboardData.Value.TempTouchUpA;	
			flag = 1;
			break;
		case TouchUp_B:
			TemValue = CupboardData.Value.TempTouchUpB;
			flag = 1;
			break;
		case TouchUp_C:
			TemValue = CupboardData.Value.TempTouchUpC;
			flag = 1;
			break;
		case TouchDown_A:
			TemValue = CupboardData.Value.TempTouchDownA;	
			flag = 1;
			break;
		case TouchDown_B:
			TemValue = CupboardData.Value.TempTouchDownB;
			flag = 1;
			break;
		case TouchDown_C:				
			TemValue = CupboardData.Value.TempTouchDownC;
			flag = 1;
			break;
		case Cupboard_T:
			TemValue = CupboardData.Value.TempCupboard;
			flag = 1;
			break;		
		case Cupboard_H:
			if(CupboardData.OnlineFlag){
				TemValue = CupboardData.Value.HumdityCupboard;
				if((TemValue < SystemSet.HumiditySet.MinValue)||
					 (TemValue > SystemSet.HumiditySet.MaxValue)){
					 flag = 2;
					 *HemData = CupboardData.Value.HumdityCupboard;
				}
			}
			break;
		case OverVoltage_P:
			if(CupboardData.OnlineFlag){				
				TemValue = CupboardData.Value.ProtectorData.OverVoltageTA;
				TemValue = TemValue/RfProtectorTemRatio;
				if((TemValue>=-40) && (TemValue<130)){
					if((TemValue> SystemSet.TempSet.MaxValue)||
						 (TemValue< SystemSet.TempSet.MinValue)){						
						flag = 3;
					}
				}
				TemValue = CupboardData.Value.ProtectorData.OverVoltageTB;
				TemValue = TemValue/RfProtectorTemRatio;
				if((TemValue>=-40) && (TemValue<130)){
					if((TemValue> SystemSet.TempSet.MaxValue)||
						 (TemValue< SystemSet.TempSet.MinValue)){
						flag = 4;
						
					}
				}
				TemValue = CupboardData.Value.ProtectorData.OverVoltageTC;
				TemValue = TemValue/RfProtectorTemRatio;
				if((TemValue>=-40) && (TemValue<130)){
					if((TemValue> SystemSet.TempSet.MaxValue)||
						 (TemValue< SystemSet.TempSet.MinValue)){
						flag = 5;
						
					}
				}
				if((CupboardData.Value.ProtectorData.OverVoltageHA < SystemSet.HumiditySet.MinValue) ||
				   (CupboardData.Value.ProtectorData.OverVoltageHA > SystemSet.HumiditySet.MaxValue)){
				   flag = 6;
				}
				if((CupboardData.Value.ProtectorData.OverVoltageHB < SystemSet.HumiditySet.MinValue) ||
				   (CupboardData.Value.ProtectorData.OverVoltageHB > SystemSet.HumiditySet.MaxValue)){
				   flag = 7;
				}
				if((CupboardData.Value.ProtectorData.OverVoltageHC < SystemSet.HumiditySet.MinValue) ||
				   (CupboardData.Value.ProtectorData.OverVoltageHC > SystemSet.HumiditySet.MaxValue)){
				   flag = 8;
				} 
				if(CupboardData.Value.ProtectorData.LeakgaeCurrent > SystemSet.LeakageCurrentSet.LeakageValue){
				   flag = 9;
				}				
			}
			break;		
		default:
			flag = 0;


	}
	if(1 == flag){
		flag = 0;
		if(CupboardData.OnlineFlag){
			TemValue = TemValue/RfTemRatio;
			if(TemValue> SystemSet.LineTempMax){
				flag = 1;
				*TemData = TemValue;
			}
		}
	}
	return flag;
}//RFRXJudge

/*
---------------------------------------------------------------------
Name		RFOntimeJudge
Function	RF在线时间判断
Input		No
OutPut		No
Edit by fanzhong
Date		2019-07-08
---------------------------------------------------------------------
*/
static void RFOntimeJudge(void)
{
	uint8_t i,j;
	for(i=0;i<20;i++){
		for(j=0;j<12;j++){
			if(Cupboard_P[i][j].OnlineFlag){
				Cupboard_P[i][j].KeepTim++;
				if('B' != (Cupboard_P[i][j].Id>>24)){
					if(Cupboard_P[i][j].KeepTim >= 2*60*100){
						Cupboard_P[i][j].OnlineFlag = 0;
						Cupboard_P[i][j].KeepTim = 0;

						Cupboard_P[i][j].Value.TempBusA = 0x7FFF;
						Cupboard_P[i][j].Value.TempBusB = 0x7FFF;
						Cupboard_P[i][j].Value.TempBusC = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchUpA = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchUpB = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchUpC = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchDownA = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchDownB = 0x7FFF;
						Cupboard_P[i][j].Value.TempTouchDownC = 0x7FFF;
						Cupboard_P[i][j].Value.TempCupboard = 0x7FFF;
						Cupboard_P[i][j].Value.HumdityCupboard = 0x00FF;						
						
					}
				}else{
					if(Cupboard_P[i][j].KeepTim >= 24*60*100){
						Cupboard_P[i][j].OnlineFlag = 0;
						Cupboard_P[i][j].KeepTim = 0;
						
						Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent = 0;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA = 0x7FFF;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB = 0x7FFF;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC = 0x7FFF;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA = 0xFF;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB = 0xFF;
						Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC = 0xFF;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntAD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntBD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntCD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntAB = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntAC = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionCntBC = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeAD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeBD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeCD = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeAB = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeAC = 0;
						Cupboard_P[i][j].Value.ProtectorData.ActionTimeBC = 0;
						Cupboard_P[i][j].Value.ProtectorData.BatteryValue = 0;
						
						
					}
				}
			}else{
				Cupboard_P[i][j].KeepTim = 0;
			}
		}
	}
}//RFOntimeJudge




