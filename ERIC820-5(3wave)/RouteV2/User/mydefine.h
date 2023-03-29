
#ifndef _MYDEFINE_H_
#define	_MYDEFINE_H_

#define   TemMaxValue    12500
#define   TemMinValue    -4000


#define   ADCFifo_SDRAM_SIZE     				50*256*2
#define   ADCFifo_SDRAM    	 						(SDRAM_APP1_BUF)
#define   FaultFifo_SDRAM    						(ADCFifo_SDRAM+(sizeof(ADCFifoStruct)/4 +1)*4)
#define   ADCWhole_SDRAM     						(FaultFifo_SDRAM + FaultFifoSIZE * 4 * (sizeof(FaultSaveStruct)/4 +1))
//#define   SystemAlarm_SDRAM     					(ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)	
#define   Mem_SDRAM			    				    (ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)
//#define   Mem_SDRAM									(SystemPowerQuality_SDRAM+(sizeof(PowerQualityStruct)/4 + 1)*4)
#define   ModuleCom_SDRAM							(Mem_SDRAM + (sizeof(MemStruct)/4 + 1)*4)
#define   SDRAM_APP1_END							(ModuleCom_SDRAM + (sizeof(ModuleComStruct)/4 + 1)*4)

//#define   AppTaskSDStk_SDRAM						(Mem_SDRAM + (sizeof(MemStruct)/4 + 1)*4)
//#define   AppTaskCalStk_SDRAM						(AppTaskSDStk_SDRAM	+ APP_CFG_TASK_SD_STK_SIZE*4)				
//#define   AppTaskCOMStk_SDRAM						(AppTaskCalStk_SDRAM + APP_CFG_TASK_Cal_STK_SIZE*4)
//#define   SDRAM_APP1_END							(AppTaskCOMStk_SDRAM + APP_CFG_TASK_COM_STK_SIZE*4)



typedef struct{
	uint16_t pt;
	uint16_t bt;
	uint16_t Calbt;
	uint8_t CalFlag;
	//uint16_t Instantbt;
	//uint16_t Calbt;
	//uint16_t Faultbt;
	uint16_t FifoSize;	
	
	float fftratio[16];	
	float RMS_buf[16];	
	int16_t ADCFIFO[16][ADCFifo_SDRAM_SIZE/2];	
}ADCFifoStruct;





typedef struct{
	int16_t RMSbuf[8][1024];
	float RMS_Elec[4][8];
	
	
	int16_t FFTBuf[16][1024];
	long FFTOutBuf[16][1024];
	
	int16_t FFTBuf1[8][1024];
	long FFTOutBuf1[8][1024];
	
	int16_t VerifyBuf[16][1024];
	
	float fft_RMS[4][8];
}ADCWholeStruct;

//typedef struct{
//	u8 buf[1024];
//	u8 flag;
//}UnionRamStruct;

typedef union{
	uint8_t UnionData;
	struct{
		uint8_t F1ms:1;
		uint8_t F5ms:1;
		uint8_t F10ms:1;
		uint8_t F20ms:1;
		uint8_t F200ms:1;
		uint8_t F500ms:1;		
		uint8_t bit6:1;
		uint8_t bit7:1;
	}Bits;	
}BitUnion;

typedef struct{
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	u32 us;
}AlarmTimStruct;

extern AlarmTimStruct SystemTime;

typedef struct{
	u8 DislayFlag;         	/*BIT0 4G,BIT1 SD, BIT2 LCD BIT3 CAL*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/
	float RMSValue[16];
	int16_t Wave[16][20*128];
}FaultSaveStruct;





/*
系统状态指示结构
*/
typedef struct{
	struct{
		uint8_t phaseAction;  /*bit0 A相标志 bit1 B相标志 bit2 C相标志*/
		uint8_t AphaseState;  /*1打开,2关闭*/
		uint8_t BphaseState;  /*1打开,2关闭*/
		uint8_t CphaseState;  /*1打开,2关闭*/
	}SwitchState;
	
}SystemStateStruct;

#define PARAM_ADDR		16*1024*1024
#define PARAM_VER			0xC100

typedef struct{
	uint16_t ParamVer;
	struct _ADCParameter{		
		float fftratio[16];
	}ADCParameter;
	
}PoramStruct;


typedef struct{
	
	uint8_t VerifyFlag; //接地复位标志

	uint8_t SPIBackCom;

	//uint8_t UartTxCom;
	uint8_t SelNum;
}ComStruct;
extern ComStruct ComData;

extern uint8_t PoramFlag;
extern PoramStruct PoramData; //参数数据


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
#if 0
enum PhaseType
{
	PhaseA=0x01,
	PhaseB=0x02,
	PhaseC=0x04,
	PhaseAB = 0x08,
	PhaseBC = 0x10,
	PhaseAC = 0x20
};

typedef enum 
{
	Bus_A=0,		//A 相母线
	Bus_B,		//B 相母线
	Bus_C,		//C 相母线
	TouchUp_A,	//A 相上触点
	TouchUp_B,	//B 相上触点
	TouchUp_C,	//C 相上触点
	TouchDown_A, //A 相下触点
	TouchDown_B, //B 相下触点	
	TouchDown_C, //C 相下触点
	Cupboard_T,	 //柜体温度
	Cupboard_H,	 //柜体湿度
	OverVoltage_P
	/*
	Leakgae_Current,//漏电流
	OverVoltage_TA,//过电压保护器A相温度
	OverVoltage_TB,//过电压保护器B相温度
	OverVoltage_TC,//过电压保护器C相温度
	OverVoltage_HA,//过电压保护器A相湿度
	OverVoltage_HB,//过电压保护器B相湿度
	OverVoltage_HC,//过电压保护器C相湿度
	Action_CntA,	//过电压保护器A相动作次数
	Action_CntB,	//过电压保护器B相动作次数
	Action_CntC,	//过电压保护器C相动作次数
	Action_CntAB,	//过电压保护器AB相动作次数
	Action_CntAC,	//过电压保护器AC相动作次数
	Action_CntBC,	//过电压保护器BC相动作次数
	Action_TimeA,	//过电压保护器A相动作时长
	Action_TimeB,	//过电压保护器B相动作时长
	Action_TimeC,	//过电压保护器C相动作时长
	Action_TimeAB,	//过电压保护器AB相动作时长
	Action_TimeAC,	//过电压保护器AC相动作时长
	Action_TimeBC,	//过电压保护器BC相动作时长
	Battery_Value		//保护器电池电量
	*/
}CupboardID_No;
#endif
#define SDindexSize    80


typedef struct{
	u32 ChipID[3];
	u16 ChipFlashSize;
}ChipICStruct;

extern ChipICStruct ChipICID;

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

typedef struct{
	//u8 RF433buf[2048];
	//u8 Net4Gbuf[4][2048];
	//u8 SDbuf[2048];
	uint8_t Modulebuf[512];
	FaultSaveStruct NandRead;
	ModuleComFaultStruct FaultComBuf;
}MemStruct;
extern MemStruct *Membuf;

extern u8 FaultFifo_pt;
extern u8 FaultFifo_bt;
extern u8 FaultFifo_busy;

//extern uint16_t Frequency[3];	

#define FaultFifoSIZE	30
extern FaultSaveStruct *FaultFifo[];

//extern Setstruct SystemSet;
//extern AlarmStruct *SystemAlarm;
//extern PowerQualityStruct *SystemPowerQuality;
//extern ActualStruct *SystemActual;

//extern ADCFifoStruct ADCFifo;
extern ADCFifoStruct *ADCFifo;
extern ADCWholeStruct *ADCWholeVar;

#define FlagSystemState		0xFF
#define FlagSystemStateNet	0x01
#define FlagSystemStateSave 0x02
extern SystemStateStruct 	SystemState;

//extern RFStruct *RFReception;

//extern CupboardParameter Cupboard_P[20][12];	

//extern UnionRamStruct UnionRam[8];

//extern CriteriaStruct CriteriaValue;

extern const float radical3;
extern const float radical2;	

extern const char Ver[];	

extern uint16_t uSCount;

//extern RelayStruct Relay[];

extern BitUnion WholeBit;

extern NAND_BLOCK_INFO_T nand_info;

//extern OS_MEM GlobalMem;
#define MCUSoft_RST()   do{__set_FAULTMASK(1);NVIC_SystemReset();}while(0)



extern u16 FIFO_length(u16 bt,u16 pt,u16 len);
extern u16 FIFO_pointer(u16 pointer,u16 len,u16 sizelen);
extern u16 FIFO_Backpointer(u16 pointer,u16 len,u16 sizelen);
extern u8 FIFO_bufcmp(u8 *FIFObuf,u8 *buf,u16 bt,u16 len,u16 sizelen);
//extern u8 HexToAscii(u32 hexdata,u8 *buf);
extern u16 CRC_GetModbus16(u8 *pdata, u16 len);
extern u32 AsciiToHex(u8 *Asciibuf,u8 Asciibuf_len);
extern u16 Get_pointer(u16 pointer,u16 CycleNum);
extern AlarmTimStruct SysGetTime(void);
extern void SysSetTime(void);

//extern void RFTemSend(u8 i,u8 j,struct _RFTem *RFbuf);

//extern void RFProtectorTemSend(u8 i,u8 j,struct _ProtectorTem *RFbuf);
//extern void RFProtectorActionSend(u8 i,u8 j,struct _ProtectorAction *RFbuf);
//extern void SaveSDFault(void);
//extern void ReadSDFault(void);
//void	GetChipID(void);
//void PlayAudio(void);

void KeyScan(void);
uint8_t InitSaveNand(void);

void ReadParam(uint16_t PARAM_VERData);
void WriteParam(void);

void SaveNandTask(void);
void SaveNandFaultNum(void);
//extern void MemInit(void);
//extern void *MemGet(void);
//extern void MemPut(void *pt);
#endif
/*---------end-------------------------------*/
