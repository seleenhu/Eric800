#ifndef _BSP_MODULETOMODULE_H_
#define _BSP_MODULETOMODULE_H_

//#define ModuleRxPin()	GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_10)
//#define ModuleTxSet()	GPIO_SetBits(GPIOI, GPIO_Pin_11)
//#define ModuleTxClr()	GPIO_ResetBits(GPIOI, GPIO_Pin_11)


#define SPI2BufSize		(60*256*2)
#define ModuleTxBufSize		512
#define ModuleRxBufSize		512

typedef struct{
	uint8_t SPI2Buf[SPI2BufSize];
	uint16_t SPI2BufTxLen;
	uint8_t ModuleTxBuf[ModuleTxBufSize];
	uint16_t ModuleTxLen;
	uint8_t ModuleRxBuf[ModuleRxBufSize];
	uint8_t ModuleBuf[ModuleRxBufSize];	
	//uint16_t ModuleRxpt;
	uint16_t ModuleRxbt;
}ModuleComStruct;

extern ModuleComStruct *ModuleGlobal;



extern void ModuleComInit(void);
extern void Usart2IRQDisplay(void);
extern uint8_t Usart2Send(uint16_t Txbuf_len);
extern uint8_t ModuleSPITxStart(uint16_t SPITx_len);
extern void Usart2DMATxIRQ(void);
extern void ModuleSPITxStartIRQ(void);
extern void ModuleTask(void);
extern void ModuleUartTXTask(uint8_t sendcmd);
extern void ModuleSPITXTask(void);
void ModuleSPIRMS(void);
#endif



