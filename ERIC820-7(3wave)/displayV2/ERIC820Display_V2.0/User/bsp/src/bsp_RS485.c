
#include "bsp.h"

#define RS485_RX_EN()			GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define RS485_TX_EN()			GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define RS485bufSize     512
uint8_t RS485buf[RS485bufSize];
uint8_t RS485TXbuf[RS485bufSize];
uint16_t RS485Pt;
uint16_t RS485bt;

//uint8_t RS485TableFlag;
static void RS485_Table(void);
/*
---------------------------------------------------------------------
Name		InitRS485
Function	初始化485
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-19
---------------------------------------------------------------------
*/
void bsp_InitRS485(void)
{
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
		USART_InitStructure.USART_BaudRate = SystemSet.RS485Set.Baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
		USART_Init(USART3, &USART_InitStructure);
	
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =5; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure); 
		
		RS485_RX_EN();
		
		//USART_SetAddress(USART3, SystemSet.RS485Set.adr);
		
		//USART_WakeUpConfig(USART3, USART_WakeUp_AddressMark);
		
		//USART_ReceiverWakeUpCmd(USART3,ENABLE);
		
		USART_Cmd(USART3, ENABLE);
		
		RS485Pt = 0;
		RS485bt = 0;
}//InitRS485

/*
---------------------------------------------------------------------
Name		IRQ_RS485
Function	485中断
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-19
---------------------------------------------------------------------
*/
void IRQ_RS485(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){		
		RS485buf[RS485Pt] = USART_ReceiveData(USART3);
		RS485Pt = FIFO_pointer(RS485Pt, 1, RS485bufSize);
	}
}//IRQ_RS485

/*
---------------------------------------------------------------------
Name		RS485Send
Function	485发送
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-19
---------------------------------------------------------------------
*/
void RS485Send(u8 *buf,u16 buf_len)
{
	RS485_TX_EN();
	
	while(buf_len--){
		USART_SendData(USART3, *buf++);	
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
	
	RS485_RX_EN();
}//RS485Send

/*
---------------------------------------------------------------------
Name		RS485_Task
Function	485任务
Input		No
OutPut	No
Edit by fanzhong
Date		2019-01-19
---------------------------------------------------------------------
*/
void RS485_Task(void)
{
	uint8_t i,len;
	uint16_t bt,checksum16,Numaddr1,NumLen;
	static uint8_t step;
	static uint8_t DelayTime;
	
	
	DelayTime++;
	switch(step){
		case 0:
			while(RS485bt != RS485Pt){
				if(SystemSet.RS485Set.adr == RS485buf[RS485bt]){					
					step++;
					DelayTime = 0;
					break;
				}else{
					RS485bt = FIFO_pointer(RS485bt,1, RS485bufSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(RS485bt, RS485Pt, RS485bufSize) >= 8){
				step=0;
				
				bt = RS485bt;
				for(i=0;i<8;i++){
					RS485TXbuf[i] = RS485buf[bt];
					bt = FIFO_pointer(bt, 1, RS485bufSize);
				}
				
				checksum16 = RS485TXbuf[7];
				checksum16 <<= 8;
				checksum16 += RS485TXbuf[6];
				bt = CRC_GetModbus16(RS485TXbuf, 6);
				
				if(bt == checksum16){
					RS485bt = FIFO_pointer(RS485bt, 8, RS485bufSize);
					
					if(0x03 == RS485TXbuf[1]){
						RS485_Table();
						
						Numaddr1 = RS485TXbuf[2];
						Numaddr1 <<= 8;
						Numaddr1 += RS485TXbuf[3];

						NumLen = RS485TXbuf[4];
						NumLen <<= 8;
						NumLen += RS485TXbuf[5];
						
						
						if((Numaddr1+NumLen <= 1024)&&(NumLen<=100)){
							len = 0;
							RS485TXbuf[len++] = SystemSet.RS485Set.adr;
							RS485TXbuf[len++] = 0x03;
							len++;
							for(i=0;i<NumLen;i++){
								RS485TXbuf[len++] = Membuf->RS485buf[Numaddr1+i]>>8;
								RS485TXbuf[len++] = Membuf->RS485buf[Numaddr1+i];								
							}
							RS485TXbuf[2] = len-3;
							checksum16 = CRC_GetModbus16(RS485TXbuf, len);
							RS485TXbuf[len++] = checksum16;
							RS485TXbuf[len++] = checksum16>>8;
							RS485Send(RS485TXbuf,len);	
						}
						
					}
				}else{
					RS485bt = FIFO_pointer(RS485bt, 1, RS485bufSize);
				}

				
			}else{
				if(DelayTime >= 20){
					step = 0;
					RS485bt = FIFO_pointer(RS485bt, 1, RS485bufSize);
				}
			}
			break;
	}
}//RS485_Task

/*
---------------------------------------------------------------------
Name		RS485_Table
Function	485填表
Input		No
OutPut	No
Edit by fanzhong
Date		2019-04-29
---------------------------------------------------------------------
*/
static void RS485_Table(void)
{
	uint8_t i,j;
	uint16_t Temlen;
	
	Temlen = 0;
	for(i=0;i<20;i++){
		for(j=0;j<12;j++){
			switch(Cupboard_P[i][j].Type){
				case Bus_A:
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempBusA);				
					break;
				case Bus_B:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempBusB);					
					break;
				case Bus_C:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempBusC);					
					break;
				case TouchUp_A:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpA);					
					break;
				case TouchUp_B:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpB);					
					break;
				case TouchUp_C:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpC);					
					break;
				case TouchDown_A:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchDownA);					
					break;
				case TouchDown_B:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchDownB);					
					break;
				case TouchDown_C:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempTouchDownC);					
					break;
				case Cupboard_T:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.TempCupboard);					
					break;
				case Cupboard_H:					
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.HumdityCupboard);					
					break;
				default:				
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA);				
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA;
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB);				
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB;				
					Membuf->RS485buf[Temlen++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC);				
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntAD;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntBD;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntCD;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntAB;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntAC;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.ActionCntBC;
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent;				
					Membuf->RS485buf[Temlen++] = Cupboard_P[i][j].Value.ProtectorData.BatteryValue;	
					break;
			}
		}
	}
	for(i=0;i<3;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecU[i]*100;
	}
	Membuf->RS485buf[Temlen++] = SystemPowerQuality->Elec3U0*100;
	for(i=0;i<3;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecI[i]*100;
	}
	Membuf->RS485buf[Temlen++] = SystemPowerQuality->Elec3I0*100;

	for(i=0;i<3;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecPowerFactor[i]*100;
	}
	for(i=0;i<4;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecActivePower[i]*100;
	}
	for(i=0;i<4;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecReactivePower[i]*100;
	}
	for(i=0;i<4;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecApparentPower[i]*100;
	}
	for(i=0;i<3;i++){
		for(j=0;j<30;j++){
			Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecThdu[i][j]*100;
		}
		
	}
	for(i=0;i<3;i++){
		for(j=0;j<30;j++){
			Membuf->RS485buf[Temlen++] = SystemPowerQuality->ElecThdi[i][j]*100;
		}
		
	}
	for(i=0;i<3;i++){
		Membuf->RS485buf[Temlen++] = SystemPowerQuality->Frequency[i];
	}
}//RS485_Table


