#ifndef _BSP_MODULETOMODULE_H_
#define _BSP_MODULETOMODULE_H_

#define ModuleRxPin()	GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_10)
#define ModuleTxSet()	GPIO_SetBits(GPIOI, GPIO_Pin_11)
#define ModuleTxClr()	GPIO_ResetBits(GPIOI, GPIO_Pin_11)


#define SPI4BufSize		(30*1024)
#define ModuleTxBufSize		512
#define ModuleRxBufSize		512

#define SPI2BufSize		(60*256*2)

typedef struct{
	uint8_t SPI4Buf[SPI4BufSize];
	uint16_t SPI4BufTxLen;
	uint8_t ModuleTxBuf[ModuleTxBufSize];
	uint16_t ModuleTxLen;
	uint8_t ModuleRxBuf[ModuleRxBufSize];
	uint16_t ModuleRxpt;
	uint16_t ModuleRxbt;

	uint8_t SPI2Buf[SPI2BufSize];
	uint16_t SPI2BufRxbt;
	uint8_t  UART2TxBuf[ModuleTxBufSize];
	uint16_t UART2TxLen;
	uint8_t  UART2RxBuf[ModuleRxBufSize];
	uint16_t UART2Rxbt;
}ModuleComStruct;

extern ModuleComStruct *ModuleGlobal;

typedef union{
	uint8_t UnionData;
	struct{
		uint8_t S_C0:1;
		uint8_t S_C1:1;
		uint8_t S_C2:1;
		uint8_t S_C3:1;
		uint8_t S_E0:1;
		uint8_t S_E1:1;
		uint8_t S_E2:1;		
		uint8_t S_E3:1;
		
		uint8_t S_E5:1;		//ADD by seleen 2022 1 11
		
	}Bits;	
}SPIBitUnion;
extern SPIBitUnion SPIBitTxFolag;
extern SPIBitUnion M1TOM3BitTxFalg;

extern uint8_t Module62bag;
extern uint8_t Uart2M1andM3Delay;
extern uint8_t SPI2M1andM3Delay;

extern void ModuleComInit(void);
extern uint8_t ModuleTxStart(void);
extern uint8_t ModuleSPITxStart(void);
extern void ModuleTxStartIRQ(void);
extern void ModuleSPITxStartIRQ(void);
extern void ModuleTask(void);
extern void Uart7_M1ANDLCD_IRQ(void);
extern void Uart2TxStartIRQ(void);
extern uint8_t Uart2TxStart(void);
extern void M1andM3_task(void);
extern uint8_t M1TOM3Uart2Tx(uint8_t SendCmd,uint8_t Senddata);
extern uint8_t M1TOM3Uart2_H10Tx(RTC_DateTypeDef DateData,RTC_TimeTypeDef TimeData);
#endif



