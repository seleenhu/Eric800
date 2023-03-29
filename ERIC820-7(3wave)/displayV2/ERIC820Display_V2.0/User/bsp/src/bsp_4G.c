

#include "bsp.h"	

#define Net_Neoway		0
#define Net_UIS891X		1

#define   NetIP						SystemSet.NetSet.IPadrr
#define   NetPort					SystemSet.NetSet.Port
#define   NetModbusID			SystemSet.NetSet.ModbudID
#define   NetModbusAdr		SystemSet.NetSet.Modbusadr


#define		Power_4G_ON()		GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define   Power_4G_OFF()	GPIO_ResetBits(GPIOF,GPIO_Pin_8)

#define RX4Gbuff_len		2048
#define Buf4GRxSize     (RX4Gbuff_len-DMA_GetCurrDataCounter(DMA1_Stream3))
u8 	RX4Gbuff[RX4Gbuff_len];
u16 RX4G_bt;								
#define TX4Gbuff_len		2048
u8 		TX4Gbuff[TX4Gbuff_len];
u8 		TX4Gbuff2[TX4Gbuff_len];

u8 	RX4GDisplaybuff[RX4Gbuff_len];
u16 RX4GDisplaybuff_len;





u8 Mode4G;
u8 Step4G;
#define  POWER_4G_ON 							0
#define  RST_4G 								1
#define  Connect_4G_Login 						2
#define  Connect_4G_Normal 						3	
#define  Connect_4G_breath						4
#define  Connect_4G_Tem							5
#define  Connect_4G_TemaP						6
#define  Connect_4G_quality						7
#define  Connect_4G_XBAG						8
#define  Connect_4G_exceedV						9
#define  Connect_4G_PTBag						10
#define  Connect_4G_Short						11
#define  Connect_ProtectorAction				12
#define  Connect_4G_SystemSet					13
#define  Connect_4G_SystemSetDown				14
#define  Connect_4G_ReviceDisplay				15
#define  Connect_4G_Protector					16
#define  POWER_4G_OFF 							17
#define  Connect_4G_XBAGSel						18

u8 Com4GLedState;
//static void  U4GSend(u8 *buf,u16 buflen);
//static void  U4GSendTxbuf(u16 buflen);
static void Modbus4G_LEDDisply(uint8_t LedS);

static u8 Cmd4G_AT(void);
static u8 Cmd4G_CGDCONT(void);
static u8 Cmd4G_XIIC(u8 chardata);
static u8 Cmd4G_TCPCLOSE(void);
static u8 Cmd4G_TCPSETUP(u8 *IPbuf,u16 port);
static u8 Cmd4G_IPSTATUS(u8 socketdata);
static u8 Cmd4G_NormalDisplay(u8 *buf, u16 *length);
static u8 Cmd4G_NormalRxDisplay(u8 *buf, u16 *length);
static u8 Cmd4G_TCPSEND(u8 socketdata,u8 *buf, u16 buflen);
static u8 Modbus4G_Login(u8 Modbuscom);
static u8 Modbus4G_quality(void);
//static u8 Modbus4G_send(u8 Modbuscom,u8 *buf, u16 buf_len);



static u8 Modbus4G_Tim(void);
static u8 Modbus4G_XBag(u8 Pointer);
static u8 Modbus4G_XBagSel(u8 Pointer);
//static u8 Modbus4G_ExceedV(void);
//static u8 Modbus4G_PTBag(void);
//static u8 Modbus4G_ShortBag(void);
static u8 Modbus4G_UpSetBag(void);
static u8 Modbus4G_DownSetBag(void);
static u8 Modbus4G_ReviceAnalyze(u8 *buf, u16 *len);
static u8 Modbus4G_Protector(void);

static uint16_t Modbus4G_SetBag(uint8_t *buf);
static void Modbus4G_DownBagValue(uint8_t *buf);

/*
---------------------------------------------------------------------
Name		Modbus4G_LEDDisply
Function	突发包
Input		No
OutPut	0 off  1 ON  2 100mS  3 200ms  4 500mS
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static void Modbus4G_LEDDisply(uint8_t LedS)
{
	static uint8_t LedTims;
	
	switch(LedS){
		case 0:			
			Com_LED_OFF();
			break;
		case 1:
			Com_LED_ON();
			break;
		case 2:
			LedTims++;
			if(LedTims == 10){
				Com_LED_ON();				
			}else if(LedTims>=20){
				Com_LED_OFF();
				LedTims = 0;
			}
			break;
		case 3:
			LedTims++;
			if(LedTims == 20){
				Com_LED_ON();				
			}else if(LedTims>= 40){
				Com_LED_OFF();
				LedTims = 0;
			}
			break;
		case 4:
			LedTims++;
			if(LedTims == 50){
				Com_LED_ON();				
			}else if(LedTims>= 100){
				Com_LED_OFF();
				LedTims = 0;
			}
			break;
		default:
			Com_LED_OFF();
	}
}//Modbus4G_LEDDisply

/*
---------------------------------------------------------------------
Name		Init4G
Function	初始化4G模块
Input		No
OutPut	No
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
void	Init4G(void)
{
		USART_InitTypeDef USART_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE);
	
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
		USART_Init(UART7, &USART_InitStructure);
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
		DMA_DeInit(DMA1_Stream3);
		while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);
	
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART7->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RX4Gbuff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = RX4Gbuff_len;
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
		
		DMA_DeInit(DMA1_Stream1);
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);		
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(UART7->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)TX4Gbuff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_BufferSize = TX4Gbuff_len;
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
		
		USART_Cmd(UART7, ENABLE);
		
		
		Mode4G = RST_4G;
		RX4G_bt = Buf4GRxSize;
		Step4G = 0;
}//Init4G


/*
---------------------------------------------------------------------
Name		U4GSendTxbuf
Function	4G数据发送TXBUF
Input		数据buf 数据长度
OutPut	No
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
void  U4GSendTxbuf(u16 buflen)
{	
	
	USART_DMACmd(UART7,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA1_Stream1, DISABLE);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
	DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
	
	DMA_SetCurrDataCounter(DMA1_Stream1,buflen); 
	DMA_Cmd(DMA1_Stream1, ENABLE); 	
	//while(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)!=RESET);
	//while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
	
	
}//U4GSendTxbuf

/*
---------------------------------------------------------------------
Name		Display4G
Function	整体处理
Input		No
OutPut	No
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
void Display4G(void)
{
#if Net_Neoway
	u8 rt;
	static u8 Bag_i,Ledtim;
	static u16 StepTime,StepTimeS;

	Modbus4G_LEDDisply(Com4GLedState);
	
	Ledtim++;
	switch(Mode4G){		
		case RST_4G:
			Com4GLedState = 0;
			//Com_LED_OFF();
			//if(Ledtim>=20){
			//	Ledtim = 0;
			//	Com_LED_XOR();
			//}
			switch(Step4G){
				case 0:
					Power_4G_ON();
					StepTime = 0;
					StepTimeS = 0;
					Step4G++;
					break;
				case 1:
					StepTime++;
					if(StepTime>=500){
						StepTime = 0;
						Power_4G_OFF();
						Step4G++;
					}
					break;
				case 2:			
					StepTime++;
					if(StepTime>=1500){
						StepTime = 0;
						Power_4G_ON();
						Step4G++;
					}
					break;
				case 3:
					StepTime++;
					if(StepTime>=2000){
						StepTime = 0;						
						Step4G++;
						RX4G_bt = Buf4GRxSize;
					}
					break;
				case 4:					
					if(StepTime<3){
						rt = Cmd4G_AT();
						if(0 == rt){
							StepTime = 0;
							Step4G++;
						}else if(2 == rt){
							StepTime++;
						}
					}else{
						StepTime = 0; 
						Step4G = 0;
					}
					break;
				case 5:
					rt = Cmd4G_CGDCONT();
					if(0 == rt){						
						Step4G++;
					}else if(2 == rt){						
						Step4G = 0;
					}
					break;
				case 6:	
					rt = Cmd4G_XIIC('1');
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
				case 7:
					
					if(StepTime < 100){
						StepTime++;	
					}else{
						rt = Cmd4G_XIIC('?'); 
						if(0 == rt){						
							Step4G++;
							StepTime = 0;
						}else if(2 == rt){
							StepTime = 0;
							if(StepTimeS<20){
								StepTimeS++;
							}else{
								Step4G = 0;
								StepTimeS = 0;
							}
						}
					}
					break;
				case 8:
					rt = Cmd4G_TCPCLOSE();
					if(0 == rt){
						Step4G++;
						StepTimeS = 0;
					}else if(2 == rt){
						Step4G = 0;
					}
					break;
				case 9:					
					rt = Cmd4G_TCPSETUP(NetIP,NetPort);
					if(0 == rt){
						Mode4G = Connect_4G_Login;
						Step4G = 0;
						StepTimeS = 0;						
					}else if(2 == rt){
						StepTimeS++;
						if(StepTimeS>=3){
							Step4G = 0;
							StepTimeS = 0;
						}
					}
					
					break;
			}
			break;
		case Connect_4G_Login:
			Com4GLedState = 4;
			//if(Ledtim>=50){
			//	Ledtim = 0;
			//	Com_LED_XOR();
			//}
			rt = Modbus4G_Login(0x01);
			if(0 == rt){				
				Mode4G = Connect_4G_Normal;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}
			break;
		case Connect_4G_Normal:
			//Com_LED_ON();
			Com4GLedState = 1;
			rt = Cmd4G_NormalRxDisplay(RX4GDisplaybuff,&RX4GDisplaybuff_len);
			if(0 == rt){
				if(RX4GDisplaybuff_len > 7){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_ReviceDisplay;
				}else{
					RX4GDisplaybuff_len = 0;
				}
			}else if(2 == rt){
				Mode4G = RST_4G;
			}			
			if(StepTime <= 60001){
				StepTime++;
				if(0 == StepTime%999){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_quality;				
				}else if(0 == StepTime%6000){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_Tem;
				}
			}else{
				StepTime = 0;
			}
			for(Bag_i=0;Bag_i<FaultSaveSIZE;Bag_i++){
				if(FaultFifo[Bag_i]->DislayFlag & 0x01){
					StepTimeS = 0;
					//Com_LED_OFF();
					Com4GLedState = 2;
					if(GroundFault != FaultFifo[Bag_i]->FaultStyle){
						Mode4G = Connect_4G_XBAG;
					}else{
						Mode4G = Connect_4G_XBAGSel;
					}
					break;
				}
			}
			if(RFReception->Protector.ProtectorAction.DislayFlag & 0x01){
				StepTimeS = 0;
				Com4GLedState = 2;
				Mode4G = Connect_4G_Protector;					
			}
#if 0
			if(SystemAlarm->VoltageAlarm.DislayFlag & 0x01){
				StepTimeS = 0;
				//Com_LED_OFF();
				Com4GLedState = 2;
				Mode4G = Connect_4G_exceedV;
			}
			if(SystemAlarm->PTAlarm.DislayFlag & 0x01){
				StepTimeS = 0;
				//Com_LED_OFF();
				Com4GLedState = 2;
				Mode4G = Connect_4G_PTBag;
			}
			if(SystemAlarm->ShortAlarm.DislayFlag & 0x01){
				StepTimeS = 0;
				//Com_LED_OFF();
				Com4GLedState = 2;
				Mode4G = Connect_4G_Short;
			}
#endif
			if(SystemSet.ParamSaveFlag & FlagSystemStateNet){
				StepTimeS = 0;
				//Com_LED_OFF();
				Com4GLedState = 2;
				Mode4G = Connect_4G_SystemSet;
			}
			break;
		case Connect_4G_Tem:
			rt = Modbus4G_Tim();
			if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				//StepTimeS++;
				//if(StepTimeS >=3){
				//	StepTimeS = 0;
				//	Mode4G = Connect_4G_Normal;
				//}
			}else if(0 == rt){
				Mode4G = Connect_4G_Normal;
			}
			break;
		case Connect_4G_quality:
			rt = Modbus4G_quality();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				//StepTimeS++;
				//if(StepTimeS >=3 ){
				//	StepTimeS = 0;
				//	Mode4G = Connect_4G_Normal;
				//}
			}
			break;
		case Connect_4G_XBAG:			
			rt = Modbus4G_XBag(Bag_i);
			if(0 == rt){				
				Mode4G = Connect_4G_Normal;
				FaultFifo[Bag_i]->DislayFlag &= ~0x01;				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;					
				}
			}
			break;
		case Connect_4G_XBAGSel:
			rt = Modbus4G_XBagSel(Bag_i);
			if(0 == rt){				
				Mode4G = Connect_4G_Normal;
				FaultFifo[Bag_i]->DislayFlag &= ~0x01;				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;					
				}
			}
			break;
#if 0
		case Connect_4G_exceedV:
			rt = Modbus4G_ExceedV();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				SystemAlarm->VoltageAlarm.DislayFlag &= ~0x01;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					SystemAlarm->VoltageAlarm.DislayFlag &= ~0x01;
				}
			}
			break;
		case Connect_4G_PTBag:
			rt = Modbus4G_PTBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				SystemAlarm->PTAlarm.DislayFlag &= ~0x01;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					SystemAlarm->PTAlarm.DislayFlag &= ~0x01;
				}
			}
			break;
		case Connect_4G_Short:
			rt = Modbus4G_ShortBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				SystemAlarm->ShortAlarm.DislayFlag &= ~0x01;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					SystemAlarm->ShortAlarm.DislayFlag &= ~0x01;
				}
			}
			break;
#endif
		case Connect_4G_SystemSet:
			rt = Modbus4G_UpSetBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				SystemSet.ParamSaveFlag &= ~FlagSystemStateNet;				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					SystemSet.ParamSaveFlag &= ~FlagSystemStateNet;
				}
			}
			break;
		case Connect_4G_SystemSetDown:
			rt = Modbus4G_DownSetBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;							
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;					
				}
			}
			break;
		case Connect_4G_ReviceDisplay:
			rt = Modbus4G_ReviceAnalyze(RX4GDisplaybuff,&RX4GDisplaybuff_len);
			if(0 == rt){
				Mode4G = Connect_4G_Normal;							
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if(3 == rt){				
				Mode4G = Connect_4G_Normal;
			}
			break;
		case Connect_4G_Protector:
			rt = Modbus4G_Protector();
			if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
				}
			}else if(0 == rt){
				RFReception->Protector.ProtectorAction.DislayFlag &= ~0x01;
				Mode4G = Connect_4G_Normal;
			}
			break;
	}
#else
	u8 rt;
	static u8 Bag_i,Ledtim;
	static u16 StepTime,StepTimeS,breathtim;

	Modbus4G_LEDDisply(Com4GLedState);
	
	Ledtim++;
	switch(Mode4G){		
		case RST_4G:
			Com4GLedState = 0;
			//Com_LED_OFF();
			//if(Ledtim>=20){
			//	Ledtim = 0;
			//	Com_LED_XOR();
			//}
			switch(Step4G){
				case 0:
					Power_4G_OFF();
					StepTime = 0;
					StepTimeS = 0;
					Step4G++;
					break;
				case 1:
					StepTime++;
					if(StepTime>=500){
						StepTime = 0;
						Power_4G_ON();
						Step4G++;
					}
					break;
				case 2:			
					StepTime++;
					if(StepTime>=500){
						StepTime = 0;
						StepTimeS = 0;
						Step4G++;
						RX4G_bt = Buf4GRxSize;
					}
					break;				
				case 3:					
					if(0 == StepTime){
						rt = Cmd4G_AT();
						if(0 == rt){
							StepTime = 0;
							StepTimeS = 0;
							Step4G++;
						}else if(2 == rt){
							StepTime++;
							StepTimeS++;
							if(StepTimeS>=15){
								StepTimeS = 0;
								Step4G = 0;
							}
							
						}
					}else if(StepTime<200){
						StepTime++;
					}else{
						StepTime = 0;
					}
					break;
				#if 0	
				case 4:					
					if(0 == StepTime){
						rt = UIS891X_CPIN();
						if(0 == rt){
							StepTime = 0;
							StepTimeS = 0;
							Step4G++;
						}else if(2 == rt){
							StepTime++;
							StepTimeS++;
							if(StepTimeS>=5){
								StepTimeS = 0;
								Step4G = 0;
							}
							
						}
					}else if(StepTime<100){
						StepTime++;
					}else{
						StepTime = 0;
					}
					break;
				case 5:					
					if(0 == StepTime){
						rt = UIS891X_CREG();
						if(0 == rt){
							StepTime = 0;
							StepTimeS = 0;
							Step4G++;
						}else if(2 == rt){
							StepTime++;
							StepTimeS++;
							if(StepTimeS>=12){
								StepTimeS = 0;
								Step4G = 0;
							}
							
						}
					}else if(StepTime<1000){
						StepTime++;
					}else{
						StepTime = 0;
					}
					break;
				case 6:
					rt = UIS891X_CGATT();
					if(0 == rt){						
						Step4G++;
					}else if(2 == rt){						
						Step4G = 0;
					}
					break;
				case 7:	
					rt = UIS891X_CIPMUX(1);
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
				case 8:	
					rt = UIS891X_CIPHEAD(1);
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
				case 9:	
					rt = UIS891X_CSTT();
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
			
				case 10:	
					rt = UIS891X_CIICR();
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
				case 11:	
					rt = UIS891X_CIFSR();
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;
				case 12:					
					rt = UIS891X_CIPSTART(NetIP,NetPort);
					if(0 == rt){
						Mode4G = Connect_4G_Login;
						Step4G = 0;
						StepTimeS = 0;						
					}else if(2 == rt){
						StepTimeS++;
						if(StepTimeS>=3){
							Step4G = 0;
							StepTimeS = 0;
						}
					}
					
					break;

			#else
				case 4:	
					rt = UIS891X_CIPMUX(0);
					if(0 == rt){						
						Step4G++;
						StepTime = 0;
						StepTimeS = 0;
					}else if(2 == rt){						
						Step4G = 0;						
					}
					break;	
			/*	
				case 4:					
					if(0 == StepTime){
						rt =  UIS891X_CIPMUX(0);
						if(0 == rt){
							StepTime = 0;
							StepTimeS = 0;
							Step4G++;
						}else if(2 == rt){
							StepTime++;
							StepTimeS++;
							if(StepTimeS>=5){
								StepTimeS = 0;
								Step4G = 0;
							}
							
						}
					}else if(StepTime<100){
						StepTime++;
					}else{
						StepTime = 0;
					}
					break;
					*/
			case 5:					
					rt = UIS891X_CIPSTART(NetIP,NetPort);
					if(0 == rt){
						Mode4G = Connect_4G_Login;
						Step4G = 0;
						StepTimeS = 0;						
					}else if(2 == rt){
						StepTimeS++;
						if(StepTimeS>=3){
							Step4G = 0;
							StepTimeS = 0;
						}
					}
					
					break;		
      					
			#endif		
					
			
				
				
					
			}
			break;
		case Connect_4G_Login:
#if	1
			Com4GLedState = 4;
			//if(Ledtim>=50){
			//	Ledtim = 0;
			//	Com_LED_XOR();
			//}
			rt = Modbus4G_Login(0x01);
			if(0 == rt){				
				Mode4G = Connect_4G_Normal;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}
#else			
			uint8_t testbuf[12];
			if(0 == StepTime){	
				for(rt =0; rt < 12;rt++){
					testbuf[rt] = rt+1;
				}
				//rt = UIS891X_CIPSEND(1,testbuf,12);
				rt = UIS891X_TCPSEND(1,testbuf,12);
				if(0 == rt){
					StepTime++;
				}else if(rt>=2){
					Mode4G = RST_4G;
				}
			}else if(StepTime<2000){
				StepTime++;
			}else{
				StepTime = 0;
			}
			
			rt = UIS891X_NormalRxDisplay(1,RX4GDisplaybuff,&RX4GDisplaybuff_len);
			if(3 == rt){
				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}
#endif
			break;
		case Connect_4G_Normal:
			//Com_LED_ON();
			Com4GLedState = 1;
#if 0
			rt = UIS891X_NormalRxDisplay(1,RX4GDisplaybuff,&RX4GDisplaybuff_len);		
			if(3 == rt){
				if(RX4GDisplaybuff_len > 7){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_ReviceDisplay;
				}else{
					RX4GDisplaybuff_len = 0;
				}
			}else if(4 == rt){
				Mode4G = RST_4G;
			}
#endif
			if(breathtim <= 60001){
				breathtim++;
				if(0 == breathtim%999){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_quality;				
				}else if(0 == breathtim%6000){
					//Com_LED_OFF();
					Com4GLedState = 2;
					Mode4G = Connect_4G_Tem;
				}
			}else{
				breathtim = 0;
			}
			for(Bag_i=0;Bag_i<FaultSaveSIZE;Bag_i++){
				if(FaultFifo[Bag_i]->DislayFlag & 0x01){
					StepTimeS = 0;
					//Com_LED_OFF();
					Com4GLedState = 2;
					if(GroundFault != FaultFifo[Bag_i]->FaultStyle){
						Mode4G = Connect_4G_XBAG;
					}else{
						Mode4G = Connect_4G_XBAGSel;
					}

					break;
				}
			}
			if(RFReception->Protector.ProtectorAction.DislayFlag & 0x01){
				StepTimeS = 0;
				Com4GLedState = 2;
				Mode4G = Connect_4G_Protector;					
			}

			if(SystemSet.ParamSaveFlag & FlagSystemStateNet){
				StepTimeS = 0;
				//Com_LED_OFF();
				Com4GLedState = 2;
				Mode4G = Connect_4G_SystemSet;
			}
			break;
		case Connect_4G_Tem:
			rt = Modbus4G_Tim();
			if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				//StepTimeS++;
				//if(StepTimeS >=3){
				//	StepTimeS = 0;
				//	Mode4G = Connect_4G_Normal;
				//}
			}else if(0 == rt){
				Mode4G = Connect_4G_Normal;
			}
			break;
		case Connect_4G_quality:
			rt = Modbus4G_quality();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				//StepTimeS++;
				//if(StepTimeS >=3 ){
				//	StepTimeS = 0;
				//	Mode4G = Connect_4G_Normal;
				//}
			}
			break;
		case Connect_4G_XBAG:			
			rt = Modbus4G_XBag(Bag_i);
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				FaultFifo[Bag_i]->DislayFlag &= ~0x01;
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					//FaultFifo[Bag_i]->DislayFlag &= ~0x01;
				}
			}
			break;
		 case Connect_4G_XBAGSel:
			rt = Modbus4G_XBagSel(Bag_i);
			if(0 == rt){				
				Mode4G = Connect_4G_Normal;
				FaultFifo[Bag_i]->DislayFlag &= ~0x01;				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;					
				}
			}
			break;
	
		case Connect_4G_SystemSet:
			rt = Modbus4G_UpSetBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;
				SystemSet.ParamSaveFlag &= ~FlagSystemStateNet;				
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
					SystemSet.ParamSaveFlag &= ~FlagSystemStateNet;
				}
			}
			break;
		case Connect_4G_SystemSetDown:
			rt = Modbus4G_DownSetBag();
			if(0 == rt){
				Mode4G = Connect_4G_Normal;							
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3 ){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;					
				}
			}
			break;
		case Connect_4G_ReviceDisplay:
			rt = Modbus4G_ReviceAnalyze(RX4GDisplaybuff,&RX4GDisplaybuff_len);
			if(0 == rt){
				Mode4G = Connect_4G_Normal;							
			}else if(2 == rt){
				Mode4G = RST_4G;
			}else if(3 == rt){				
				Mode4G = Connect_4G_Normal;
			}
			break;
		case Connect_4G_Protector:
			rt = Modbus4G_Protector();
			if(2 == rt){
				Mode4G = RST_4G;
			}else if((4 == rt)||(3 == rt)){
				StepTimeS++;
				if(StepTimeS >=3){
					StepTimeS = 0;
					Mode4G = Connect_4G_Normal;
				}
			}else if(0 == rt){
				RFReception->Protector.ProtectorAction.DislayFlag &= ~0x01;
				Mode4G = Connect_4G_Normal;
			}
			break;
	}

#endif

}//Display4G


/*
---------------------------------------------------------------------
Name		Cmd4G_AT
Function	 命令AT
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
static u8 Cmd4G_AT(void)
{

	static u8 step,stepTim;;
	u8 com_len;
	u8 rt,flag;
	u16 pt;
	

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"AT",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;	
}//Cmd4G_AT


/*
---------------------------------------------------------------------
Name		Cmd4G_CGDCONT
Function	 命令CGDCONT
Input		No
OutPut	0 OK  =1 Delay  =2 ERR;
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
static u8 Cmd4G_CGDCONT(void)
{
	static u8 step,stepTim;;
	static u8 com_len;
	u8 rt,flag;
	u16 pt;
	

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len-2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"AT+CGDCONT=1,\"IP\",\"CMNET\"",RX4G_bt,com_len-2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len-2,RX4Gbuff_len);
						flag = 1;
						break;
					}
					RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					rt = 0;
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;
}//Cmd4G_CGDCONT

/*
---------------------------------------------------------------------
Name		Cmd4G_XIIC
Function	 命令CGDCONT
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
static u8 Cmd4G_XIIC(u8 chardata)
{
	static u8 step,stepTim;;
	static u8 com_len;
	static u16 bt;
	u8 i,rt,flag;
	u16 pt;
	u8 *buf;
	u8 ipaddr[4];	
	
	buf = Membuf->Net4Gbuf[0];
	
	if('1' == chardata){
		switch(step){
			case 0:
				com_len = sprintf((char *)TX4Gbuff,"AT+XIIC=1\r\n");
				U4GSendTxbuf(com_len);
				stepTim = 0;
				step++;
				rt = 1;
				break;
			case 1:
				stepTim++;
				if(stepTim<=100){
					flag = 0;
					pt = Buf4GRxSize;
					while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len-2){
						if(0 == FIFO_bufcmp(RX4Gbuff,"AT+XIIC=1",RX4G_bt,com_len-2,RX4Gbuff_len)){
							RX4G_bt = FIFO_pointer(RX4G_bt,com_len-2,RX4Gbuff_len);							
							flag = 1;
							break;
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					if(flag){
						step++;
						stepTim = 0;
					}
					rt = 1;
				}else{
					step = 0;
					rt = 2;
				}
				break;
			case 2:
				stepTim++;
				if(stepTim<=100){
					flag = 0;
					pt = Buf4GRxSize;
					while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
						if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){							
							RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
							flag = 1;
							break;
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					if(flag){
						com_len = 0;
						
						step = 0;
						stepTim = 0;
						rt = 0;
					}else{
						rt = 1;
					}				
				}else{
					step = 0;
					rt = 2;
				}
				break;
			}
	
	}else{
		switch(step){
			case 0:
				com_len = sprintf((char *)TX4Gbuff,"AT+XIIC?\r\n");
				U4GSendTxbuf(com_len);
				stepTim = 0;
				step++;
				rt = 1;
				break;
			case 1:
				stepTim++;
				if(stepTim<=100){
					flag = 0;
					pt = Buf4GRxSize;
					while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len-2){
						if(0 == FIFO_bufcmp(RX4Gbuff,"AT+XIIC?",RX4G_bt,com_len-2,RX4Gbuff_len)){
							RX4G_bt = FIFO_pointer(RX4G_bt,com_len-2,RX4Gbuff_len);
							bt = RX4G_bt;
							flag = 1;
							break;
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					if(flag){
						step++;
						stepTim = 0;
					}
					rt = 1;
				}else{
					step = 0;
					rt = 2;
				}
				break;
			case 2:
				stepTim++;
				if(stepTim<=100){
					flag = 0;
					pt = Buf4GRxSize;
					while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
						if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){
							RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
							flag = 1;
							break;
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					if(flag){
						com_len = 6;
						flag = 0;
						memcpy(buf,"+XIIC:",com_len);						
						while(FIFO_length(bt,RX4G_bt,RX4Gbuff_len)>= com_len){
							if(0 == FIFO_bufcmp(RX4Gbuff,buf,bt,com_len,RX4Gbuff_len)){
								bt = FIFO_pointer(bt,com_len,RX4Gbuff_len);				
								flag = 1;
								break;
							}
							bt = FIFO_pointer(bt,1,RX4Gbuff_len);
						}
						if(flag){
							com_len = 2;
							flag = 0;
							memcpy(buf,"1,",com_len);						
							while(FIFO_length(bt,RX4G_bt,RX4Gbuff_len)>= com_len){
								if(0 == FIFO_bufcmp(RX4Gbuff,buf,bt,com_len,RX4Gbuff_len)){
									bt = FIFO_pointer(bt,com_len,RX4Gbuff_len);				
									flag = 1;
									break;
								}
								bt = FIFO_pointer(bt,1,RX4Gbuff_len);
							}
							if(flag){
								for(i=0;i<4;i++){									
									while(bt != RX4G_bt){
										 if(isdigit(RX4Gbuff[bt])){
											 ipaddr[i] = 0;											 
											 break;
										 }
										 bt = FIFO_pointer(bt,1,RX4Gbuff_len);										 
									}
									while(bt != RX4G_bt){
										if(isdigit(RX4Gbuff[bt])){											
											ipaddr[i] = ipaddr[i] * 10 + (RX4Gbuff[bt] - '0');
										}else{											
											break;
										}
										bt = FIFO_pointer(bt,1,RX4Gbuff_len);										 
									}
								}
								flag = 0;
								for(i=0;i<4;i++){
									if(ipaddr[i]){
										flag++;
									}
								}
								if(flag){
									rt = 0;
								}else{
									rt = 2;
								}
							}else{
								rt = 2;
							}
						}else{
							rt = 2; 
						} 
						step = 0;
						stepTim = 0;						
					}else{
						rt = 1;
					}				
				}else{
					step = 0;
					rt = 2;
				}
				break;
			}
	}
	
	return rt;	
}//Cmd4G_XIIC


/*
---------------------------------------------------------------------
Name		Cmd4G_TCPCLOSE
Function	 命令TCPCLOSE
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
static u8 Cmd4G_TCPCLOSE(void)
{
	static u8 step,stepTim;;
	static u8 com_len;
	u8 rt,flag;
	u16 pt;
	

	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+TCPCLOSE=1\r\n");
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len-2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"AT+TCPCLOSE=1",RX4G_bt,com_len-2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len-2,RX4Gbuff_len);
						flag = 1;
						break;
					}
					RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=10){
					if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPCLOSE:",RX4G_bt,10,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,10,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;					
				}
				rt = 1;								
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"\r\n",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}
					RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
				}
				if(flag){
					rt = 0;
					step = 0;
					stepTim = 0;											
				}else{
					rt = 1;
				}				
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;

}//Cmd4G_TCPCLOSE

/*
---------------------------------------------------------------------
Name		Cmd4G_TCPSETUP
Function	 命令TCPCLOSE
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
static u8 Cmd4G_TCPSETUP(u8 *IPbuf,u16 port)
{

	static u8 step;
	static u16 stepTim,bt;
	u8 com_len;
	u8 rt,flag;
	u16 pt;

	u8 *buf;
	
	buf = Membuf->Net4Gbuf[0];

	
	switch(step){
		case 0:
			com_len = sprintf((char *)TX4Gbuff,"AT+TCPSETUP=1,%d.%d.%d.%d,%d\r\n",IPbuf[0],IPbuf[1],IPbuf[2],IPbuf[3],port);						
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				com_len = sprintf((char *)buf,"AT+TCPSETUP=1,%d.%d.%d.%d,%d",IPbuf[0],IPbuf[1],IPbuf[2],IPbuf[3],port);
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
								
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=3000){
				flag = 0;
				com_len = sprintf((char *)buf,"+TCPSETUP:");
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,buf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						bt = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
								
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 4:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"\r\n",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					flag = 0;
					while(FIFO_length(bt,RX4G_bt,RX4Gbuff_len)>=2){
						if(0 == FIFO_bufcmp(RX4Gbuff,"OK",bt,2,RX4Gbuff_len)){
							bt = FIFO_pointer(bt,2,RX4Gbuff_len);
							flag = 1;
							break;
						}else{
							bt = FIFO_pointer(bt,1,RX4Gbuff_len);
						}
						
					}
					if(flag){						
						rt = 0;	
					}else{
						rt = 2;
					}
					step = 0;
					stepTim = 0;
				}else{
					rt = 1;	
				}
			}else{
				step = 0;
				rt = 2;
			}
			break;
			
	}
	
	return rt;
}//Cmd4G_TCPSETUP

/*
---------------------------------------------------------------------
Name		Cmd4G_IPSTATUS
Function	 命令IPSTATUS
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
static u8 Cmd4G_IPSTATUS(u8 socketdata)
{
	static u8 step,stepTim;
	static u16 bt;
	u8 com_len;
	u8 rt,flag;
	u16 pt,len;
	

	switch(step){
		case 0:
			com_len = 15;
			memcpy(TX4Gbuff,"AT+IPSTATUS=1\r\n",com_len);
			TX4Gbuff[12] = socketdata;	
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=13){
					if(0 == FIFO_bufcmp(RX4Gbuff,"AT+IPSTATUS=1",RX4G_bt,13,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,13,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=12){
					if(0 == FIFO_bufcmp(RX4Gbuff,"+IPSTATUS: 1",RX4G_bt,12,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,12,RX4Gbuff_len);
						bt = RX4G_bt;
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;					
				}
				rt = 1;
								
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=10){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"\r\n",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);						
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step = 0;
					stepTim = 0;
					flag = 0;					
					while(FIFO_length(bt,RX4G_bt,RX4Gbuff_len)>=7){
						if(0 == FIFO_bufcmp(RX4Gbuff,"CONNECT",bt,7,RX4Gbuff_len)){
							bt = FIFO_pointer(bt,7,RX4Gbuff_len);
							while(bt != RX4G_bt){
								if(isdigit(RX4Gbuff[bt])){
									break;
								}
								bt = FIFO_pointer(bt,1,RX4Gbuff_len);
							}
							len = 0;
							while(bt != RX4G_bt){
								if(isdigit(RX4Gbuff[bt])){
									len = len*10 + (RX4Gbuff[bt] - '0');
								}else{
									break;
								}
								bt = FIFO_pointer(bt,1,RX4Gbuff_len);
							}
							if(len >= 2048){
								flag = 1;
							}else if(len){
								flag = 2;
							}
							break;
						}else{
							bt = FIFO_pointer(bt,1,RX4Gbuff_len);
						}
					}
					if(1 == flag){
						rt = 0;
					}else if(2 == flag){
						rt = 1;
					}else{
						rt = 2;
					}
				}else{
					rt = 1;
				}
								
			}else{
				step = 0;
				rt = 2;
			}
			break;
	}
	return rt;
	
}//Cmd4G_IPSTATUS

/*
---------------------------------------------------------------------
Name		Cmd4G_TCPSEND
Function	 命令TCPSEND
Input		No
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
static u8 Cmd4G_TCPSEND(u8 socketdata,u8 *buf, u16 buflen)
{
	static u8 step,sendok;
	static u16 stepTim;
	u16 com_len;
	u8 rt,flag;
	u16 pt;
	u8 *Nembuf;
	
	Nembuf = Membuf->Net4Gbuf[0];

	switch(step){		
		case 0:
			rt = Cmd4G_IPSTATUS(socketdata);
			if(0 == rt){
				step++;
				rt = 1;
			}else if(2 == rt){
				rt = 2;
			}
			break;
		case 1:
			com_len = sprintf((char *)TX4Gbuff,"AT+TCPSEND=1,%d\r\n",buflen);
			TX4Gbuff[11] = socketdata;
			U4GSendTxbuf(com_len);
			stepTim = 0;
			step++;
			rt = 1;
			break;
		case 2:
			stepTim++;
			if(stepTim<=10){
				com_len = sprintf((char *)Nembuf,"AT+TCPSEND=1,%d",buflen);
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=com_len){
					if(0 == FIFO_bufcmp(RX4Gbuff,Nembuf,RX4G_bt,com_len,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,com_len,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;
				}
				rt = 1;
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=100){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if('>' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					stepTim = 0;					
				}				
			}else{
				step++;
				stepTim = 0;				
			}
			rt = 1;
			break;
		case 4:
			memcpy(TX4Gbuff,buf,buflen);
			U4GSendTxbuf(buflen);
			step++;
			rt = 1;
			sendok = 0;	
			break;
		case 5:
			stepTim++;
			if(stepTim < 6000){
				flag = 0;
				pt = Buf4GRxSize;				
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=9){
					if(0 == FIFO_bufcmp(RX4Gbuff,"OK",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						sendok = 1;
					}else if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPSEND:",RX4G_bt,9,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,9,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
				}
				if(flag){
					step++;	
				}
				rt = 1;				
			}else{
				step = 0;
				rt = 2;
			}
			break;
		case 6:
			stepTim++;
			if(stepTim < 6000){
				flag = 0;
				pt = Buf4GRxSize;				
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>= 2){
					if(0 == FIFO_bufcmp(RX4Gbuff,"\r\n",RX4G_bt,2,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,2,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
				}
				if(flag){
					step = 0;
					stepTim = 0;
					if(sendok){
						rt = 0;
					}else{
						rt = 2;
					}
				}else{
					rt = 1;	
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 2;
			}
			break;
	
	}
	
	return rt;
}//Cmd4G_TCPSEND


/*
---------------------------------------------------------------------
Name		Cmd4G_NormalDisplay
Function	 链接正常处理
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-01
---------------------------------------------------------------------
*/
static u8 Cmd4G_NormalDisplay(u8 *buf, u16 *length)
{
	static u8 step;
	static u16 stepTim;
	u8 com_len;
	u8 rt,flag;
	u16 pt;
	
	
	

	switch(step){
		case 0:
			stepTim++;
			if(stepTim < 500){
				flag = 0;
				pt = Buf4GRxSize;
				while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=9){
					if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPRECV:",RX4G_bt,9,RX4Gbuff_len)){
						RX4G_bt = FIFO_pointer(RX4G_bt,9,RX4Gbuff_len);
						flag = 1;
						break;
					}else if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPCLOSE",RX4G_bt,9,RX4Gbuff_len)){						
						flag = 2;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(1 == flag){
					step++;					
					rt = 1;
				}else if(2 == flag){
					rt = 2;
					stepTim = 0;
				}else{	
					rt = 1;
				}
			}else{
				stepTim = 0;
				rt = 3;
			}
			break;
		case 1:			
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if(',' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					*length = 0;
				}
				rt = 1;
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if(',' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						if(isdigit(RX4Gbuff[RX4G_bt])){
							*length = (*length) * 10 + (RX4Gbuff[RX4G_bt] - '0');
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;					
				}
				rt = 1;
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=500){				
				pt = Buf4GRxSize;
				if(FIFO_length(RX4G_bt,pt,RX4Gbuff_len) >= *length){					
					for(com_len = 0; com_len < *length; com_len++){
						buf[com_len] = RX4Gbuff[RX4G_bt];
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					rt = 0;					
				}else{				
					rt = 1;
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
	}
	
	
	
	return rt;
}//Cmd4G_NormalDisplay
/*
---------------------------------------------------------------------
Name		Cmd4G_NormalRxDisplay
Function	 数据接收处理
Input		No
OutPut	No
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Cmd4G_NormalRxDisplay(u8 *buf, u16 *length)
{
	static u8 step;
	static u16 stepTim;
	u8 com_len;
	u8 rt,flag;
	u16 pt;
	switch(step){
		case 0:
			flag = 0;
			pt = Buf4GRxSize;
			while(FIFO_length(RX4G_bt,pt,RX4Gbuff_len)>=9){
				if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPRECV:",RX4G_bt,9,RX4Gbuff_len)){
					RX4G_bt = FIFO_pointer(RX4G_bt,9,RX4Gbuff_len);
					flag = 1;
					break;
				}else if(0 == FIFO_bufcmp(RX4Gbuff,"+TCPCLOSE",RX4G_bt,9,RX4Gbuff_len)){						
					flag = 2;
					break;
				}else{
					RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
				}
				
			}
			if(1 == flag){
				step++;					
				rt = 1;
			}else if(2 == flag){
				rt = 2;
				stepTim = 0;
			}else{	
				rt = 1;
			}			
			break;
		case 1:			
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if(',' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;
					*length = 0;
				}
				rt = 1;
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
		case 2:
			stepTim++;
			if(stepTim<=500){
				flag = 0;
				pt = Buf4GRxSize;
				while(RX4G_bt != pt){
					if(',' == RX4Gbuff[RX4G_bt]){
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
						flag = 1;
						break;
					}else{
						if(isdigit(RX4Gbuff[RX4G_bt])){
							*length = (*length) * 10 + (RX4Gbuff[RX4G_bt] - '0');
						}
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					
				}
				if(flag){
					step++;					
				}
				rt = 1;
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
		case 3:
			stepTim++;
			if(stepTim<=500){				
				pt = Buf4GRxSize;
				if(FIFO_length(RX4G_bt,pt,RX4Gbuff_len) >= *length){					
					for(com_len = 0; com_len < *length; com_len++){
						buf[com_len] = RX4Gbuff[RX4G_bt];
						RX4G_bt = FIFO_pointer(RX4G_bt,1,RX4Gbuff_len);
					}
					rt = 0;					
				}else{				
					rt = 1;
				}
			}else{
				step = 0;
				stepTim = 0;
				rt = 3;
			}
			break;
	}
	
	
	
	return rt;
}//Cmd4G_NormalRxDisplay


/*
---------------------------------------------------------------------
Name			Modbus4G_Login
Function	 4G登录
Input		No
OutPut	No
Edit by fanzhong
Date		2017-12-01
---------------------------------------------------------------------
*/

static u8 Modbus4G_Login(u8 Modbuscom)
{
	
	static u8 step;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];

	switch(step){
		case 0:		
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = Modbuscom;	
			com_len++;
			com_len++;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		 rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){
				step++;				
				rt = 1;
			}else if (2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						if((Modbuscom|0x80) == buf[2]){
							crcdata = CRC_GetModbus16(buf,com_len-2);
							crcdata1 = buf[com_len-1]<<8;
							crcdata1 += buf[com_len-2];							
							if(crcdata == crcdata1){
								SystemTime.year = buf[11];
								SystemTime.month = buf[12];
								SystemTime.day = buf[13];
								SystemTime.hour = buf[14];
								SystemTime.minute = buf[15];
								SystemTime.second = buf[16];									
								SysSetTime();
								SPIBitFlag.Bits.S_12 = 1;
								flag = 1;
							}
						}
					}

				}
				if(flag){
					rt = 0;
				}else{
					rt = 4;
				}				
			}else if(2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}
	
	return rt;
}//Modbus4G_Login

/*
---------------------------------------------------------------------
Name			Modbus4G_quality
Function	电能质量上传
Input		No
OutPut	No
Edit by fanzhong
Date		2018-03-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_quality(void)
{
	static u8 step;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];

	switch(step){
		case 0:		
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x41;
			com_len++;
			com_len++;
			buf[com_len++] = 1;
			buf[com_len++] = 1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;	
			memcpy(buf+com_len,SystemPowerQuality->ElecU,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,&SystemPowerQuality->Elec3U0,sizeof(float));
			com_len	+=	sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecI,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,&SystemPowerQuality->Elec3I0,sizeof(float));
			com_len	+=	sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecPowerFactor,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecActivePower,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecReactivePower,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecApparentPower,3*sizeof(float));
			com_len	+=	3*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecThdu,3*30*sizeof(float));
			com_len	+=	3*30*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->ElecThdi,3*30*sizeof(float));
			com_len	+=	3*30*sizeof(float);
			memcpy(buf+com_len,SystemPowerQuality->Frequency,3*sizeof(uint16_t));
			com_len	+=	3*sizeof(uint16_t);
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
			rt=UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){
				step++;				
				rt = 1;
			}else if (2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif	
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						if((0x41|0x80) == buf[2]){
							crcdata = CRC_GetModbus16(buf,com_len-2);
							crcdata1 = buf[com_len-1]<<8;
							crcdata1 += buf[com_len-2];							
							if(crcdata == crcdata1){
								flag = 1;
							}
						}
					}

				}
				if(flag){
					rt = 0;
				}else{
					rt = 4;
				}				
			}else if(2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}
	
	return rt;

}//Modbus4G_quality

/*
---------------------------------------------------------------------
Name		Modbus4G_Tim
Function	定时包
Input		No
OutPut	No
Edit by fanzhong
Date		2018-04-10
---------------------------------------------------------------------
*/
static u8 Modbus4G_Tim(void)
{
	static u8 step;	
	static u16 com_len;
	u8 i,j,rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];

	switch(step){
		case 0:		
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x40;
			com_len++;
			com_len++;
			buf[com_len++] = 1;
			buf[com_len++] = 1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			for(i=0;i<20;i++){
				for(j=0;j<12;j++){
					switch(Cupboard_P[i][j].Type){
						case Bus_A:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusA);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusA) >> 8;
							break;
						case Bus_B:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusB);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusB) >> 8;
							break;
						case Bus_C:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusC);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempBusC) >> 8;
							break;
						case TouchUp_A:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpA);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpA) >> 8;
							break;
						case TouchUp_B:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpB);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpB) >> 8;
							break;
						case TouchUp_C:
							buf[com_len++] = Cupboard_P[i][j].Id;
							buf[com_len++] = Cupboard_P[i][j].Id >> 8;
							buf[com_len++] = Cupboard_P[i][j].Id >> 16;
							buf[com_len++] = Cupboard_P[i][j].Id >> 24;
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpC);
							buf[com_len++] = (u16)(Cupboard_P[i][j].Value.TempTouchUpC) >> 8;
							break;
					}
				}
			}
			for(i=0;i<20;i++){
				for(j=0;j<12;j++){
					if(OverVoltage_P == Cupboard_P[i][j].Type){						
						buf[com_len++] = Cupboard_P[i][j].Id;
						buf[com_len++] = Cupboard_P[i][j].Id >> 8;
						buf[com_len++] = Cupboard_P[i][j].Id >> 16;
						buf[com_len++] = Cupboard_P[i][j].Id >> 24;
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA);
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA) >> 8;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA;
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB);
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB) >> 8;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB;
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC);
						buf[com_len++] = (u16)(Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC) >> 8;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent >> 8;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.BatteryValue;
						buf[com_len++] = Cupboard_P[i][j].Value.ProtectorData.BatteryValue >> 8;
					}
				}
			}			
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt=UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){
				step++;				
				rt = 1;
			}else if (2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif

			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						if((0x40|0x80) == buf[2]){
							crcdata = CRC_GetModbus16(buf,com_len-2);
							crcdata1 = buf[com_len-1]<<8;
							crcdata1 += buf[com_len-2];							
							if(crcdata == crcdata1){
								SystemTime.year = buf[27];
								SystemTime.month = buf[28];
								SystemTime.day = buf[29];
								SystemTime.hour = buf[30];
								SystemTime.minute = buf[31];
								SystemTime.second = buf[32];									
								SysSetTime();
								SPIBitFlag.Bits.S_12 = 1;
								flag = 1;
							}
						}
					}

				}
				if(flag){
					rt = 0;
				}else{
					rt = 4;
				}				
			}else if(2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}
	
	return rt;

}//Modbus4G_TimTem

/*
---------------------------------------------------------------------
Name		Modbus4G_XBag
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_XBag(u8 Pointer)
{
	static u8 step,BagLen,Bag_i,StartBag;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];
	BagLen = 71;
	switch(step){
		case 0:	
			
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			switch(FaultFifo[Pointer]->FaultStyle){
				case SysExceedValueFault:
					buf[com_len++] = 0x61;
					break;
				case ExceedValueFault:
					buf[com_len++] = 0x62;
					break;
				case GroundFault:					
					buf[com_len++] = 0x60;
					break;
				case PTFault:
					if(FaultFifo[Pointer]->ValueFlag & 0x80){
						buf[com_len++] = 0x64;
					}else{
						buf[com_len++] = 0x63;
					}
					break;				
				case ShortFault:
					buf[com_len++] = 0x65;
					break;
				default:
					buf[com_len++] = 0x61;
			}			
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = FaultFifo[Pointer]->Tim.year;
			buf[com_len++] = FaultFifo[Pointer]->Tim.month;
			buf[com_len++] = FaultFifo[Pointer]->Tim.day;
			buf[com_len++] = FaultFifo[Pointer]->Tim.hour;
			buf[com_len++] = FaultFifo[Pointer]->Tim.minute;
			buf[com_len++] = FaultFifo[Pointer]->Tim.second;
			
			buf[com_len++] = FaultFifo[Pointer]->ValueFlag;

			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->fftratio),sizeof(float)*8);
			com_len += sizeof(float)*8;
			
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->RMSValue),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->VabuleCos),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->VabuleSin),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			StartBag = 0;
			step++;
		case 1:
			if(0 == StartBag){
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,1024);
#else
			//rt = UIS891X_CIPSEND(1,buf,1024);
			rt=UIS891X_TCPSEND(1,buf,1024);
#endif
				if(0 == rt){
					StartBag = 1;								
					rt = 1;
				}else if (2 == rt){
					step = 0;
					rt = 2;
				}else if(3 == rt){
					
					step = 0;
					rt = 3;
				}else{
					rt = 1;
				}
			}else{
#if Net_Neoway
				rt = Cmd4G_TCPSEND('1',buf+1024,com_len-1024);
#else
				//rt = UIS891X_CIPSEND(1,buf+1024,com_len-1024);
				rt=UIS891X_TCPSEND(1,buf+1024,com_len-1024);
#endif
				if(0 == rt){					
					step++;				
					rt = 1;
					Bag_i++;
				}else if (2 == rt){					
					step = 0;
					rt = 2;
				}else if(3 == rt){					
					step = 0;
					rt = 3;
				}else{
					rt = 1;
				}
			}
			break;
		case 2:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			switch(FaultFifo[Pointer]->FaultStyle){
				case SysExceedValueFault:
					buf[com_len++] = 0x61;
					break;
				case ExceedValueFault:
					buf[com_len++] = 0x62;
					break;
				case GroundFault:					
					buf[com_len++] = 0x60;
					break;
				case PTFault:
					if(FaultFifo[Pointer]->ValueFlag & 0x80){
						buf[com_len++] = 0x64;
					}else{
						buf[com_len++] = 0x63;
					}
					break;				
				case ShortFault:
					buf[com_len++] = 0x65;
					break;
				default:
					buf[com_len++] = 0x61;
			}			
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			memcpy(buf+com_len,((u8 *)(FaultFifo[Pointer]->Wave) + (Bag_i-1) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 3:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < BagLen){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 4:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							//SystemTime.year = buf[15];
							//SystemTime.month = buf[16];
							//SystemTime.day = buf[17];
							//SystemTime.hour = buf[18];
							//SystemTime.minute = buf[19];
							//SystemTime.second = buf[20];									
							//SysSetTime();
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_XBag
/*
---------------------------------------------------------------------
Name		Modbus4G_XBagSel
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_XBagSel(u8 Pointer)
{
	static u8 step,BagLen,Bag_i,StartBag;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	//float fdata;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];
	BagLen = 71+11;
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x60;						
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = FaultFifo[Pointer]->Tim.year;
			buf[com_len++] = FaultFifo[Pointer]->Tim.month;
			buf[com_len++] = FaultFifo[Pointer]->Tim.day;
			buf[com_len++] = FaultFifo[Pointer]->Tim.hour;
			buf[com_len++] = FaultFifo[Pointer]->Tim.minute;
			buf[com_len++] = FaultFifo[Pointer]->Tim.second;
			
			buf[com_len++] = FaultFifo[Pointer]->ValueFlag;

			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->fftratio),sizeof(float)*8);
			com_len += sizeof(float)*8;
			
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->RMSValue),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->VabuleCos),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			memcpy(buf+com_len,(u8 *)(FaultFifo[Pointer]->VabuleSin),sizeof(float)*8*15);
			com_len += sizeof(float)*8*15;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			StartBag = 0;
			step++;
		case 1:
			if(0 == StartBag){
				//rt = Cmd4G_TCPSEND('1',buf,1024);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,1024);
#else
			//rt = UIS891X_CIPSEND(1,buf,1024);
				rt = UIS891X_TCPSEND(1,buf,1024);
#endif
				if(0 == rt){
					StartBag = 1;								
					rt = 1;
				}else if (2 == rt){
					step = 0;
					rt = 2;
				}else if(3 == rt){
					
					step = 0;
					rt = 3;
				}else{
					rt = 1;
				}
			}else{
				
				//rt = Cmd4G_TCPSEND('1',buf+1024,com_len-1024);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf+1024,com_len-1024);
#else
			//rt = UIS891X_CIPSEND(1,buf+1024,com_len-1024);
			rt = UIS891X_TCPSEND(1,buf+1024,com_len-1024);
#endif

				if(0 == rt){					
					step++;				
					rt = 1;
					Bag_i++;
				}else if (2 == rt){					
					step = 0;
					rt = 2;
				}else if(3 == rt){					
					step = 0;
					rt = 3;
				}else{
					rt = 1;
				}
			}
			break;
		case 2:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x60;						
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			memcpy(buf+com_len,((u8 *)(FaultFifo[Pointer]->Wave) + (Bag_i-1) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 3:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		 rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < 71){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 4:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x60;						
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = FaultAllSelFifo[Pointer]->FaultFlag[0]+1;
			buf[com_len++] = FaultAllSelFifo[Pointer]->CanId[0];
			buf[com_len++] = 0x03;			
			
			memcpy(buf+com_len,(u8 *)(&FaultAllSelFifo[Pointer]->ratio),sizeof(float));
			com_len += sizeof(float);
			memcpy(buf+com_len,(u8 *)(&FaultAllSelFifo[Pointer]->RMSValue),sizeof(float));
			com_len += sizeof(float);
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;			
			step++;			
		case 5:			
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif	
			if(0 == rt){												
				rt = 1;
				Bag_i++;
				step++;
			}else if (2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;			
		case 6:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x60;						
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			memcpy(buf+com_len,((u8 *)(FaultAllSelFifo[Pointer]->Wave) + (Bag_i-72) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 7:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < BagLen){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 8:
			rt = Cmd4G_NormalDisplay(buf,&com_len);
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							//SystemTime.year = buf[15];
							//SystemTime.month = buf[16];
							//SystemTime.day = buf[17];
							//SystemTime.hour = buf[18];
							//SystemTime.minute = buf[19];
							//SystemTime.second = buf[20];									
							//SysSetTime();
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_XBagSel

#if 0
/*
---------------------------------------------------------------------
Name		Modbus4G_ExceedV
Function	发送过电压波形
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-04-11
---------------------------------------------------------------------
*/
static u8 Modbus4G_ExceedV(void)
{
	static u8 step,BagLen,Bag_i;	
	static u16 com_len;
	u8 i,rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	float RMSbuf[8];
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];
	BagLen = 81;
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;			
			buf[com_len++] = 0x62;
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;

			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.year;
			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.month;
			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.day;
			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.hour;
			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.minute;
			buf[com_len++] = SystemAlarm->VoltageAlarm.Tim.second;
			
			buf[com_len++] = SystemAlarm->VoltageAlarm.ValueFlag;
			for(i=0;i<8;i++){
				RMSbuf[i] = SystemAlarm->VoltageAlarm.ExceedValue[i];
				RMSbuf[i] /= 100;
			}
			
			memcpy(buf+com_len,((u8 *)RMSbuf),sizeof(u32)*8);
			com_len += sizeof(u32)*8;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif		
			if(0 == rt){
				step++;				
				rt = 1;
				Bag_i++;
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;			
			buf[com_len++] = 0x62;						
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;			
			memcpy(buf+com_len,((u8 *)(SystemAlarm->VoltageAlarm.Wave) + (Bag_i-1) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 3:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		 rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < BagLen){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 4:
			rt = Cmd4G_NormalDisplay(buf,&com_len);
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_XBag
#endif
#if 0
/*
---------------------------------------------------------------------
Name		Modbus4G_PTBag
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_PTBag(void)
{
	static u8 step,BagLen,Bag_i;	
	static u16 com_len;
	u8 i,rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	float RMSbuf[8];
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];
	BagLen = 81;
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
		
			if(SystemAlarm->PTAlarm.ValueFlag & 0x80){				
				buf[com_len++] = 0x64;
			}else{
				buf[com_len++] = 0x63;
			}
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.year;
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.month;
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.day;
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.hour;
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.minute;
			buf[com_len++] = SystemAlarm->PTAlarm.Tim.second;
			
			buf[com_len++] = SystemAlarm->PTAlarm.ValueFlag & 0x7F;
			for(i=0;i<8;i++){
				RMSbuf[i] = SystemAlarm->PTAlarm.RMSValue[i];
				RMSbuf[i] /= 100;
			}
			memcpy(buf+com_len,(u8 *)(RMSbuf),sizeof(u32)*8);
			com_len += sizeof(u32)*8;
			
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){
				step++;				
				rt = 1;
				Bag_i++;
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			if(SystemAlarm->PTAlarm.ValueFlag & 0x80){				
				buf[com_len++] = 0x64;
			}else{
				buf[com_len++] = 0x63;
			}			
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			memcpy(buf+com_len,((u8 *)(SystemAlarm->PTAlarm.Wave) + (Bag_i-1) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 3:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		 rt = UIS891X_TCPSEND(1,buf,com_len);
#endif		
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < BagLen){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 4:
			rt = Cmd4G_NormalDisplay(buf,&com_len);
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_PTBag
#endif
#if 0
/*
---------------------------------------------------------------------
Name		Modbus4G_ShortBag
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_ShortBag(void)
{
	static u8 step,BagLen,Bag_i;	
	static u16 com_len;
	u8 i,rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	float RMSbuf[8];
	buf = Membuf->Net4Gbuf[1];
	BagLen = 81;
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;	
			buf[com_len++] = 0x65;			
			com_len++;
			com_len++;			
			Bag_i = 0;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.year;
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.month;
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.day;
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.hour;
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.minute;
			buf[com_len++] = SystemAlarm->ShortAlarm.Tim.second;
			
			buf[com_len++] = 0;
			for(i=0;i<8;i++){
				RMSbuf[i] = SystemAlarm->ShortAlarm.RMSValue[i];
				RMSbuf[i] /= 100;
			}
			memcpy(buf+com_len,(u8 *)(RMSbuf),sizeof(u32)*8);
			com_len += sizeof(u32)*8;
			
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif		
			if(0 == rt){
				step++;				
				rt = 1;
				Bag_i++;
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;						
			buf[com_len++] = 0x65;
			com_len++;
			com_len++;
			buf[com_len++] = BagLen;
			buf[com_len++] = Bag_i+1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			memcpy(buf+com_len,((u8 *)(SystemAlarm->ShortAlarm.Wave) + (Bag_i-1) * 1024),1024);
			com_len += 1024;
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			//break;
		case 3:
			//rt = Cmd4G_TCPSEND('1',buf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
			rt = UIS891X_TCPSEND(1,buf,com_len);
#endif	
			if(0 == rt){				
				Bag_i++;
				if(Bag_i < BagLen){
					step -= 1;	
				}else{					
					step++;
				}
				rt = 1;				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 4:
			rt = Cmd4G_NormalDisplay(buf,&com_len);
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_ShortBag
#endif
/*
---------------------------------------------------------------------
Name		Modbus4G_UpSetBag
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_UpSetBag(void)
{
	static u8 step,bagNum;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];	
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;	
			buf[com_len++] = 0x10;			
			com_len++;
			com_len++;
			buf[com_len++] = 1;
			buf[com_len++] = 1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;

			//memcpy(buf+com_len,&SystemSet,sizeof(Setstruct));			
			//com_len += sizeof(Setstruct);
			
			com_len += Modbus4G_SetBag(buf+com_len);
			
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
			bagNum = 0;
		case 1:
			if(0 == bagNum){
#if Net_Neoway
				rt = Cmd4G_TCPSEND('1',buf,1024);
#else
				//rt = UIS891X_CIPSEND(1,buf,1024);
				rt = UIS891X_TCPSEND(1,buf,1024);
#endif
			}else{
#if Net_Neoway
				rt = Cmd4G_TCPSEND('1',buf+1024,com_len-1024);
#else
				//rt = UIS891X_CIPSEND(1,buf+1024,com_len-1024);
				rt = UIS891X_TCPSEND(1,buf+1024,com_len-1024);
#endif		
			}
			if(0 == rt){
				if(0 == bagNum){
					bagNum++;
				}else{
					step++;	
				}
				rt = 1;
				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						//if((0x40|0x80) == buf[2]){
						crcdata = CRC_GetModbus16(buf,com_len-2);
						crcdata1 = buf[com_len-1]<<8;
						crcdata1 += buf[com_len-2];							
						if(crcdata == crcdata1){
							flag = 1;
						}
						//}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_UpSetBag
/*
---------------------------------------------------------------------
Name		Modbus4G_DownSetBag
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME  4 BAG ERR
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_DownSetBag(void)
{
	static u8 step;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];	
	switch(step){
		case 0:
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;	
			buf[com_len++] = 0x11;			
			com_len++;
			com_len++;
			buf[com_len++] = 1;
			buf[com_len++] = 1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif
			if(0 == rt){
				step++;				
				rt = 1;
				
			}else if (2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						
						if((0x11|0x80) == buf[2]){
							crcdata = CRC_GetModbus16(buf,com_len-2);
							crcdata1 = buf[com_len-1]<<8;
							crcdata1 += buf[com_len-2];							
							if(crcdata == crcdata1){
								flag = 1;
								Modbus4G_DownBagValue(buf+33);
								//memcpy(&SystemSet,buf,sizeof(Setstruct));
							}
						}
					}

				}
				if(flag){					
					rt = 0;
				}else{
					
					rt = 4;
				}
				
			}else if(2 == rt){
				
				step = 0;
				rt = 2;
			}else if(3 == rt){
				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}	
	return rt;
}//Modbus4G_DownSetBag
/*
---------------------------------------------------------------------
Name		Modbus4G_ReviceAnalyze
Function	突发包
Input		No
OutPut	0 OK 1 WAIT 2 ERR 3 OVER TIME
Edit by fanzhong
Date		2018-10-15
---------------------------------------------------------------------
*/
static u8 Modbus4G_ReviceAnalyze(u8 *buf, u16 *len)
{
	static u8 step;
	u8 rt;
	u16 rxlen,crcdata,crcdata1;
	static u16 com_len;
	u8 *Numbuf;
	AlarmTimStruct SysTim;
	
	Numbuf = Membuf->Net4Gbuf[2];
	
	switch(step){
		case 0:
			rxlen = buf[4];
			rxlen <<= 8;
			rxlen += buf[3];
			rxlen += 7;
			if(rxlen == (*len)){
				if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
					
					crcdata = CRC_GetModbus16(buf,(*len)-2);
					crcdata1 = buf[(*len)-1]<<8;
					crcdata1 += buf[(*len)-2];							
					if(crcdata == crcdata1){						
						switch(buf[2]){
							case 0x92:
								if(sizeof(Setstruct)+21 == *len){
									//memcpy(&SystemSet,buf+19,sizeof(Setstruct));
									Modbus4G_DownBagValue(buf+19);
								}
								*len = 0;
								rt = 0;	
								break;
							case 0x93:
								step++;
								*len = 0;
								rt = 1;	
								break;
							default:
								*len = 0;
								rt = 0;	
						}
						
					}else{
						*len = 0;
						rt = 0;
					}
					
				}else{
					*len = 0;
					rt = 0;
				}
			}else{
				*len = 0;
				rt = 0;
			}
			
			break;
		case 1:
			com_len = 0;
			Numbuf[com_len++] = NetModbusAdr;
			Numbuf[com_len++] = (NetModbusAdr)>>8;	
			Numbuf[com_len++] = 0x13;			
			com_len++;
			com_len++;
			Numbuf[com_len++] = 1;
			Numbuf[com_len++] = 1;
			memcpy(Numbuf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			Numbuf[com_len++] = SysTim.year;
			Numbuf[com_len++] = SysTim.month;
			Numbuf[com_len++] = SysTim.day;
			Numbuf[com_len++] = SysTim.hour;
			Numbuf[com_len++] = SysTim.minute;
			Numbuf[com_len++] = SysTim.second;

			memcpy(Numbuf+com_len,&SystemSet,sizeof(Setstruct));
			com_len += sizeof(Setstruct);
			
			Numbuf[3] = (com_len-5);
			Numbuf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(Numbuf,com_len);			
			Numbuf[com_len++] = crcdata;
			Numbuf[com_len++] = crcdata>>8;
			step++;
		case 2:
			//rt = Cmd4G_TCPSEND('1',Numbuf,com_len);
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',Numbuf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,Numbuf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif		
			if(0 == rt){
				step = 0;				
				rt = 0;				
			}else if (2 == rt){				
				step = 0;
				rt = 2;
			}else if(3 == rt){				
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
	}
	return rt;
}//Modbus4G_ReviceAnalyze

#if 0
/*
---------------------------------------------------------------------
Name		Modbus4G_SetbagPack
Function	设定包打包
Input		封装地址
OutPut	    总长度
Edit by fanzhong
Date		2019-03-07
---------------------------------------------------------------------
*/
static uint16_t Modbus4G_SetbagPack(uint8_t *Inputbuf)
{
	uint16_t len;
	len = 0;	
	memcpy(Inputbuf,(uint8_t *)&(SystemSet.ParamVer),sizeof(uint32_t));
	len += sizeof(uint32_t);
	Inputbuf[len++] = SystemSet.flag;
	Inputbuf[len++] = SystemSet.ParamSaveFlag;
	Inputbuf[len++] = SystemSet.DriveTestFlag;
	Inputbuf[len++] = SystemSet.DistanceFlag;
	memcpy(Inputbuf,(uint8_t *)&(SystemSet.ParamVer),4*sizeof(uint8_t));
	len += 4*sizeof(uint8_t);
	
	len += sizeof(uint32_t);
	
}
#endif

/*
---------------------------------------------------------------------
Name		Modbus4G_Protector
Function	保护器突发包
Input		No
OutPut	No
Edit by fanzhong
Date		2018-04-10
---------------------------------------------------------------------
*/
static u8 Modbus4G_Protector(void)
{
	static u8 step;	
	static u16 com_len;
	u8 rt,flag;
	u16 crcdata,crcdata1,len;
	u8 *buf;
	AlarmTimStruct SysTim;
	
	buf = Membuf->Net4Gbuf[1];

	switch(step){
		case 0:		
			com_len = 0;
			buf[com_len++] = NetModbusAdr;
			buf[com_len++] = (NetModbusAdr)>>8;
			buf[com_len++] = 0x66;
			com_len++;
			com_len++;
			buf[com_len++] = 1;
			buf[com_len++] = 1;
			memcpy(buf+com_len,NetModbusID,20);
			com_len += 20;
			SysTim = SysGetTime();
			buf[com_len++] = SysTim.year;
			buf[com_len++] = SysTim.month;
			buf[com_len++] = SysTim.day;
			buf[com_len++] = SysTim.hour;
			buf[com_len++] = SysTim.minute;
			buf[com_len++] = SysTim.second;
			
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.year;
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.month;
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.day;
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.hour;
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.minute;
			buf[com_len++] = RFReception->Protector.ProtectorAction.Tim.second;
			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.Id,sizeof(uint32_t));
			com_len += sizeof(uint32_t);
			buf[com_len++] = RFReception->Protector.ProtectorAction.Phase;
			
			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.ActionTime,sizeof(uint16_t));
			com_len += sizeof(uint16_t);
			
			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.TemA,sizeof(int16_t));
			com_len += sizeof(int16_t);
			buf[com_len++] = RFReception->Protector.ProtectorAction.HumA;
			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.TemB,sizeof(int16_t));
			com_len += sizeof(int16_t);
			buf[com_len++] = RFReception->Protector.ProtectorAction.HumB;
			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.TemC,sizeof(int16_t));
			com_len += sizeof(int16_t);
			buf[com_len++] = RFReception->Protector.ProtectorAction.HumC;

			memcpy(buf+com_len,&RFReception->Protector.ProtectorAction.LeakageCurrent,sizeof(uint16_t));
			com_len += sizeof(uint16_t);
		
			buf[3] = (com_len-5);
			buf[4] = (com_len-5)>>8;			
			crcdata = CRC_GetModbus16(buf,com_len);			
			buf[com_len++] = crcdata;
			buf[com_len++] = crcdata>>8;
			step++;
		case 1:
#if Net_Neoway
			rt = Cmd4G_TCPSEND('1',buf,com_len);
#else
			//rt = UIS891X_CIPSEND(1,buf,com_len);
		  rt = UIS891X_TCPSEND(1,buf,com_len);
#endif	
			if(0 == rt){
				step++;				
				rt = 1;
			}else if (2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;
		case 2:
#if Net_Neoway
			rt = Cmd4G_NormalDisplay(buf,&com_len);
#else
			rt = UIS891X_NormalDisplay(1,buf,&com_len);
#endif	
			if(0 == rt){
				step = 0;
				len = buf[4];
				len <<= 8;
				len += buf[3];
				len += 7;
				flag = 0;
				if(len == com_len){
					if((buf[0] == (u8)(NetModbusAdr))&&
					(buf[1] == (u8)((NetModbusAdr)>>8))){
						if((0x66|0x80) == buf[2]){
							crcdata = CRC_GetModbus16(buf,com_len-2);
							crcdata1 = buf[com_len-1]<<8;
							crcdata1 += buf[com_len-2];							
							if(crcdata == crcdata1){
								flag = 1;
							}
						}
					}

				}
				if(flag){
					rt = 0;
				}else{
					rt = 4;
				}				
			}else if(2 == rt){
				step = 0;
				rt = 2;
			}else if(3 == rt){
				step = 0;
				rt = 3;
			}else{
				rt = 1;
			}
			break;		
	}
	
	return rt;

}//Modbus4G_Protector

/*
---------------------------------------------------------------------
Name		Modbus4G_SetBag
Function	保护器突发包
Input		No
OutPut	No
Edit by fanzhong
Date		2019-09-19
---------------------------------------------------------------------
*/
static uint16_t Modbus4G_SetBag(uint8_t *buf)
{
	uint16_t len;
	len = 0;
	/*传动试验开启/中断*/
	buf[len++] = SystemSet.DriveTestFlag;
	/*远方/就地*/
	buf[len++] = SystemSet.DistanceFlag;
	/*本地密码*/
	memcpy(buf+len,SystemSet.Passwords,4*sizeof(uint8_t));
	len += 4*sizeof(uint8_t);
	/*保护器类型*/
	memcpy(buf+len,SystemSet.ProtectorType,20*sizeof(uint8_t));
	len += 20*sizeof(uint8_t);
	
	/*线温度值*/
	memcpy(buf+len,&SystemSet.LineTempMax,sizeof(float));
	len += sizeof(float);

	/*RFID*/
	memcpy(buf+len,SystemSet.CupboardID,20*12*sizeof(uint32_t));
	//len += sizeof(float);
	len += 20*12*sizeof(uint32_t);

	/*有效柜子数*/
	buf[len++] = SystemSet.CupboardSIZE;

	/*主机ID*/
	memcpy(buf+len,SystemSet.NetSet.ModbudID,20*sizeof(uint8_t));
	len += 20*sizeof(uint8_t);

	/*RS485的波特率*/
	memcpy(buf+len,&SystemSet.RS485Set.Baud,sizeof(uint32_t));
	len += sizeof(uint32_t);
	/*RS485的地址*/
	buf[len++] = SystemSet.RS485Set.adr;

	/*额定电压值*/
	memcpy(buf+len,&SystemSet.VoltageSet.MaxRatedValue,sizeof(uint32_t));
	len += sizeof(uint32_t);
	/*额定电压系数*/
	memcpy(buf+len,&SystemSet.VoltageSet.Modulus,sizeof(float));
	len += sizeof(float);

	/*保护器温度最大值*/
	memcpy(buf+len,&SystemSet.TempSet.MaxValue,sizeof(float));
	len += sizeof(float);

	/*保护器温度最小值*/
	memcpy(buf+len,&SystemSet.TempSet.MinValue,sizeof(float));
	len += sizeof(float);

	/*保护器湿度最大值*/
	memcpy(buf+len,&SystemSet.HumiditySet.MaxValue,sizeof(float));
	len += sizeof(float);

	/*保护器湿度最小值*/
	memcpy(buf+len,&SystemSet.HumiditySet.MinValue,sizeof(float));
	len += sizeof(float);

	/*保护器漏电流最大值*/
	memcpy(buf+len,&SystemSet.LeakageCurrentSet.LeakageValue,sizeof(float));
	len += sizeof(float);

    /*PT变比*/
	memcpy(buf+len,&SystemSet.RatioStruct.RatioPT,sizeof(uint16_t));
	len += sizeof(uint16_t);
	/*CT变比*/
	memcpy(buf+len,&SystemSet.RatioStruct.RatioCT,sizeof(uint16_t));
	len += sizeof(uint16_t);
	/*选择CT变比*/
	memcpy(buf+len,SystemSet.SelRatioCTBuf,16*sizeof(uint16_t));
	len += 16*sizeof(uint16_t);
	
	/*短路电流*/
	memcpy(buf+len,&SystemSet.ShortGNDSet.I0,sizeof(float));
	len += sizeof(float);
	return len;
}//Modbus4G_SetBag

/*
---------------------------------------------------------------------
Name		Modbus4G_DownBagValue
Function	保护器突发包
Input		No
OutPut	No
Edit by fanzhong
Date		2019-09-19
---------------------------------------------------------------------
*/
static void Modbus4G_DownBagValue(uint8_t *buf)
{
	uint16_t len;
	len = 0;
	/*传动试验开启/中断*/
	SystemSet.DriveTestFlag = buf[len++];	
	/*远方/就地*/
	SystemSet.DistanceFlag = buf[len++];	
	/*本地密码*/	
	memcpy(SystemSet.Passwords,buf+len,4*sizeof(uint8_t));
	len += 4*sizeof(uint8_t);	
	/*保护器类型*/
	memcpy(SystemSet.ProtectorType,buf+len,20*sizeof(uint8_t));
	len += 20*sizeof(uint8_t);
	
	/*线温度最大值*/
	memcpy(&SystemSet.LineTempMax,buf+len,sizeof(float));
	len += sizeof(float);

	/*RFID*/
	memcpy(SystemSet.CupboardID,buf+len,20*12*sizeof(uint32_t));
	//len += sizeof(float);
	len += 20*12*sizeof(uint32_t);
	/*有效柜子数*/
	SystemSet.CupboardSIZE = buf[len++];
	
	/*主机ID*/
	memcpy(SystemSet.NetSet.ModbudID,buf+len,20*sizeof(uint8_t));
	len += 20*sizeof(uint8_t);

	/*RS485的波特率*/
	memcpy(&SystemSet.RS485Set.Baud,buf+len,sizeof(uint32_t));
	len += sizeof(uint32_t);
	/*RS485的地址*/
	SystemSet.RS485Set.adr = buf[len++];

	/*额定电压值*/
	memcpy(&SystemSet.VoltageSet.MaxRatedValue,buf+len,sizeof(uint32_t));
	len += sizeof(uint32_t);
	/*额定电压系数*/
	memcpy(&SystemSet.VoltageSet.Modulus,buf+len,sizeof(float));
	len += sizeof(float);

	/*保护器温度最大值*/
	memcpy(&SystemSet.TempSet.MaxValue,buf+len,sizeof(float));
	len += sizeof(float);

	/*保护器温度最小值*/
	memcpy(&SystemSet.TempSet.MinValue,buf+len,sizeof(float));
	len += sizeof(float);

	/*保护器湿度最大值*/
	memcpy(&SystemSet.HumiditySet.MaxValue,buf+len,sizeof(float));
	len += sizeof(float);

	/*保护器湿度最小值*/
	memcpy(&SystemSet.HumiditySet.MinValue,buf+len,sizeof(float));
	len += sizeof(float);

	/*保护器漏电流最大值*/
	memcpy(&SystemSet.LeakageCurrentSet.LeakageValue,buf+len,sizeof(float));
	len += sizeof(float);

    /*PT变比*/
	memcpy(&SystemSet.RatioStruct.RatioPT,buf+len,sizeof(uint16_t));
	len += sizeof(uint16_t);
	/*CT变比*/
	memcpy(&SystemSet.RatioStruct.RatioCT,buf+len,sizeof(uint16_t));
	len += sizeof(uint16_t);
	/*选择CT变比*/
	memcpy(SystemSet.SelRatioCTBuf,buf+len,16*sizeof(uint16_t));
	len += 16*sizeof(uint16_t);
	
	/*短路电流*/
	memcpy(&SystemSet.ShortGNDSet.I0,buf+len,sizeof(float));
	len += sizeof(float);
}//Modbus4G_DownBagValue


//----------------------end--------------------------------------------


