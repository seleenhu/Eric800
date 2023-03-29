#ifndef _BSP_MODULETOMODULE_H_
#define _BSP_MODULETOMODULE_H_

#define ModuleRxPin()	GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_10)
#define ModuleTxSet()	GPIO_SetBits(GPIOI, GPIO_Pin_11)
#define ModuleTxClr()	GPIO_ResetBits(GPIOI, GPIO_Pin_11)



#define SPI4BufSize		(60*256*2)
#define ModuleTxBufSize		512
#define ModuleRxBufSize		512

typedef struct{
	uint8_t SPI4Buf[SPI4BufSize];
	uint16_t SPI4BufRxbt;
	uint8_t ModuleTxBuf[ModuleTxBufSize];
	uint16_t ModuleTxLen;
	uint8_t ModuleRxBuf[ModuleRxBufSize];
	uint16_t ModuleRxpt;
	uint16_t ModuleRxbt;

	uint16_t LEDState;
	uint8_t ResetFlag;
	uint16_t OutState;
	uint8_t DataResetFlag;


	float RMS_buf[8];
	float ratio[8];
	float fftratio[8];
	int16_t wavebuf[8][12*64];

	CanStruct CanComdata;
	ModuleComFaultAllSelSaveStruct FaultAllSelComBuf;
}ModuleComStruct;
extern ModuleComStruct *ModuleCombuf;


typedef struct{
	u8 DislayFlag;         	/*BIT0 4G,BIT1 SD, BIT1 LCD*/
	u8 FaultStyle;          /*故障类型*/
	AlarmTimStruct Tim;
	float ratio[8];
	float fftratio[8];
	u8 ValueFlag; 			/*相位 BIT0:A BIT1:B BIT2:C*/
	float RMSValue[8][15]; 	/*故障点有效0～14*/
	float VabuleSin[8][15];
	float VabuleCos[8][15];
	int16_t Wave[7][12*64];
}ModuleComFaultStruct;
extern ModuleComFaultStruct *ModuleComFaultbuf;

#if 0
typedef struct{
	u8 DislayFlag;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/
	float RMSValue[16];
	int16_t Wave[16][12*32];
}ModuleComFaultSelStruct;
extern ModuleComFaultSelStruct *ModuleComSelFaultbuf;
#endif

typedef union{
	uint8_t UnionData;
	struct{
		uint8_t S_10:1;
		uint8_t S_11:1;
		uint8_t S_12:1;
		uint8_t S_13:1;
		uint8_t S_E0:1;
		uint8_t S_E1:1;
		uint8_t S_E2:1;		
		uint8_t S_E3:1;		
	}Bits;	
}SPIBitUnion;

typedef struct{	
	uint8_t GndPtNew;
	uint8_t TotalGndPt;
	
	uint8_t ExPtNew;
	uint8_t TotalExPt;

	uint8_t SExPtNew;
	uint8_t TotalSExPt;

	uint8_t PTPtNew;
	uint8_t TotalPTPT;

	uint8_t DEPtNew;
	uint8_t TotalDEPT;

	uint8_t SHPtNew;
	uint8_t TotalSHPT;
}NandCfgStruct;
#define NandFileSize  60
extern NandCfgStruct NandCfgData;

extern SPIBitUnion SPIBitFlag;

extern void ModuleComInit(void);
extern void ModuleTIM4_IRQn(void);
extern void ModuleTIM3_IRQn(void);
extern void ModuleEXTI10_IRQn(void);
extern uint8_t ModuleTxStart(void);
extern uint8_t ModuleComSPIRX(void);
extern uint8_t ModuleComReadFault(uint8_t FaultType,uint8_t TypeNum);
extern uint8_t ModuleComSet10Fault(uint8_t ComOrder,uint8_t VerifyData,uint8_t SwData);
extern uint8_t ModuleComSet11Fault(void);
extern uint8_t ModuleComSet12Fault(void);
extern uint8_t ModuleComSet13Fault(uint16_t outdata,uint8_t Resetdata);

#endif



