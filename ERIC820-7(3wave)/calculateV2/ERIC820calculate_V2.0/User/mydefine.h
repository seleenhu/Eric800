
#ifndef _MYDEFINE_H_
#define	_MYDEFINE_H_

#define   TemMaxValue    12500
#define   TemMinValue    -4000

//#define UseLocal_16Current

#define   ADCFifo_SDRAM_SIZE     				50*256*2
#define   ADCFifo_SDRAM    	 						(SDRAM_APP1_BUF)
#define   FaultFifo_SDRAM    						(ADCFifo_SDRAM+(sizeof(ADCFifoStruct)/4 +1)*4)
#define   ADCWhole_SDRAM     						(FaultFifo_SDRAM + FaultFifoSIZE * 4 * (sizeof(FaultSaveStruct)/4 +1))
//#define   SystemAlarm_SDRAM     					(ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)	
#define   SystemPowerQuality_SDRAM     				(ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)
#define   Mem_SDRAM									(SystemPowerQuality_SDRAM+(sizeof(PowerQualityStruct)/4 + 1)*4)
#define   ModuleCom_SDRAM							(Mem_SDRAM + (sizeof(MemStruct)/4 + 1)*4)
#define   FaultSelFifo_SDRAM    					(ModuleCom_SDRAM + (sizeof(ModuleComStruct)/4 + 1)*4)
#define   FaultAllSelFifo_SDRAM						(FaultSelFifo_SDRAM + FaultFifoSIZE * 4 * (sizeof(FaultSelSaveStruct)/4 +1))
#define   SDRAM_APP1_END    						(FaultAllSelFifo_SDRAM + FaultFifoSIZE * 4 * (sizeof(FaultAllSelSaveStruct)/4 +1))



typedef struct{
	uint16_t pt;
	uint16_t bt;	
	uint16_t FifoSize;	
	float rati0[8];	
	float fftratio[8];	
	float RMS_buf[8];	
	int16_t ADCFIFO[8][ADCFifo_SDRAM_SIZE/2];	
}ADCFifoStruct;





typedef struct{
	int16_t RMSbuf[8][1024];
	float RMS_Elec[4][8];
	
	
	int16_t FFTBuf[8][1024];
	long FFTOutBuf[8][1024];
	
	int16_t FFTBuf1[8][1024];
	long FFTOutBuf1[8][1024];
	
	int16_t VerifyBuf[8][1024];
	
	float fft_RMS[4][8];
}ADCWholeStruct;

#pragma pack (1)
typedef struct{	
	uint32_t ParamVer;			/* 参数区版本控制（可用于程序升级时，决定是否对参数区进行升级） */
	u8 flag;


	


	
	
	
	
	

	
	
	
	
	//uint8_t NetSelectFlag;
	//u8 RunFlag; //bit1 1 OK 0 没有投运信号 bit0 1 PT OK 0 PT ERR
	//struct _ADCParameter{
	//	float ratio[8];
	//	float fftratio[8];
	//}ADCParameter;

	
	
	

	
	struct _Current{
		float MaxValue; /*最大值*/
		float MinValue; /*最小值*/
	}CurrentSet;
	//struct _Voltage{
	//	uint32_t MaxRatedValue; /*最大额定电压值*/
	//	float Modulus; /*系数*/
	//}VoltageSet;
	struct _Temp{
		float MaxValue; /*最大值*/
		float MinValue; /*最小值*/
	}TempSet;
	
	struct _Humidity{
		float MaxValue; /*最大值*/
		float MinValue; /*最小值*/
	}HumiditySet;
	
	
	struct _LeakageCurrent{
		float LeakageValue;
	}LeakageCurrentSet;
	
	
	
	
	
	struct _syntony{
		float SyntonyV[3][4];/*A B C三相的5次谐振 10次谐振 15次谐振 20次谐振*/
	}SyntonySet;
	
	
}Setstruct;
#pragma pack ()

typedef struct{
	u8 buf[1024];
	u8 flag;
}UnionRamStruct;

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

typedef struct{	
	struct{
		const float MaxValue;
		const float MinValue;	
		const float ErrValue;
		const float MaxUl;
	}Disconnection;
	struct{		
		const float MaxValue;
		const float Ul;
		const float MinValue;
	}Gnd;
	struct{
		const float MaxValue1_3;
		const float MaxValue1_2;
		const float MaxValue1;
		const float MaxValue2;
		const float MaxValue3;
	}Resonance;
	const float DecompressionValue;
}CriteriaStruct;

#if 0
typedef struct{
	struct _VoltageAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		u32 ExceedValue[8];
		u8 ValueFlag;   /*过压的相位 BIT0:A BIT1:B BIT3:C*/
	}VoltageAlarm;
	
	
	struct _TempAlarm{
		AlarmTimStruct Tim;		
		float ExceedTemp;
		u8 ID[9]; 							/*温度超标的ID*/  
	}TempAlarm;
	
	struct _HumidityAlarm{
		AlarmTimStruct Tim;
		//const u16 *Wave;			/*前十后十的波形缓存*/
		float ExceedTemp;
		u8 ID[9]; 							/*温度超标的ID*/  
	}HumidityAlarm;
	
	struct _GroundAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		u8 ValueFlag;   /*相位 BIT0:A BIT1:B BIT3:C BIT7 1 弧光 0 金属*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		u8 ResetFlag;	// 1 ResetButton on
	}GroundAlarm;
	
	struct _GridAlarm{		
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD,BIT2 LCD*/
		AlarmTimStruct Tim;		
		u8 ValueFlag;   /*相位 BIT0:17hz BIT1:25hz BIT3:150hz*/
		u32 GridValue[3];
		u32 RMSValue[8];
		int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		//u8	Grid;							/*谐振次数*/
		//float GridAValue;
		//float GridBValue;
		//float GridCValue;		
	}GridAlarm;
	
	struct _ShortAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		//u8 ValueFlag;   							/*相位 BIT0:AB BIT1:BC BIT3:AC*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		//u32 U0Value;
	}ShortAlarm;
	
	struct _PTAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		u8 ValueFlag;   							/*相位 BIT0:A BIT1:B BIT2:C*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		//u32 UlValue;
	}PTAlarm;
	
	char filenameV[32];	//save 过压报警存储文件名 
	char filenameG[32];	//save 接地报警存储文件名  
	char filenameS[32]; //save 短路报警存储文件名  
	char filenameR[32];//Save resonate 谐振报警文件名
	char filenameT[32];//PT 断线报警文件名
	
}AlarmStruct;
#endif

typedef enum{
	InstantaneousVoltageFault = 0,
	SysExceedValueFault = 1,
	ExceedValueFault = 2,	
	GroundFault = 3,
	PTFault = 4,
	ShortFault = 5,
	xFault = 6,
}FaultEnum;

#pragma pack (1)
typedef struct{
	u8 DislayFlag;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/	
	float RMSValue[16];
	int16_t Wave[16][20*128];
}FaultSelSaveStruct;
#pragma pack ()
#if 0
typedef struct{
	u8 FaultFlag;
	u8 CanId;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float ratio;	
	float RMSValue;
	int16_t Wave[20*256];
}FaultAllSelSaveStruct;
#else
#define SelWaveNum 3
#pragma pack (1)
typedef struct{
	u8 FaultFlag[SelWaveNum];
	u8 CanId[SelWaveNum];         	/*BIT0 rx*/	
	AlarmTimStruct Tim[SelWaveNum];	
	float ratio[SelWaveNum];	
	float RMSValue[SelWaveNum];
	int16_t Wave[SelWaveNum][20*256];
}FaultAllSelSaveStruct;
#pragma pack ()
#endif

#pragma pack (1)
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
	int16_t Wave[7][20*256];
	
}FaultSaveStruct;
#pragma pack ()



typedef struct{
	float ElecU[3]; /*电压有效值*/
	float Elec3U0;
	float ElecI[3]; /*电流有效值*/
	float Elec3I0;
	float ElecPowerFactor[3]; /*功率因数*/
	float ElecActivePower[4];/*有功功率*/
  float ElecReactivePower[4];/*无功功率*/
  float ElecApparentPower[4];/*视在功率*/
	float ElecThdu[3][30];/*电压谐波*/
  float ElecThdi[3][30];/*电流谐波*/	 
	u16 Frequency[3];
	 
}PowerQualityStruct;



/*
系统状态指示结构
*/
typedef struct{
	struct{
		uint8_t phaseAction;  /*bit0 A相标志 bit1 B相标志 bit2 C相标志*/
		uint8_t AphaseState;  /*1打开,2关闭*/
		uint8_t BphaseState;  /*1打开,2关闭*/
		uint8_t CphaseState;  /*1打开,2关闭*/
		uint8_t DphaseState;  /*1打开,2关闭*/	
	}SwitchState;
	
}SystemStateStruct;

#define PARAM_ADDR		16*1024*1024
#define PARAM_VER			0xA117

typedef struct{
	uint16_t ParamVer;
	struct _ADCParameter{
		float ratio[8];
		float fftratio[8];
	}ADCParameter;
	
	struct _Voltage{
		uint32_t MaxRatedValue; /*最大额定电压值*/
		float Modulus; /*系数*/
	}VoltageSet;

	struct _Ratio{
		u16 RatioPT;
		u16 RatioCT;		
	}RatioStruct;
	struct _ShortGND{
		float I0;
	}ShortGNDSet;	
}PoramStruct;


typedef struct{
	//Display -> Cal	
	uint8_t ComOrder;  //命令
	uint8_t PhaseSwitch;
	uint8_t VerifyFlag; //接地复位标志

	uint8_t FaultNamebuf[32];
	uint8_t FaultSelFlag;
	//uint8_t	FaultNum;

	//Display <- Cal
	uint16_t LEDState; //LED状态
	uint8_t  PhaseState;

	uint8_t bagNums;

	//Display <-> Cal



	
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
#define NandFileSize   60
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
#if 0
typedef struct{
	u8 DislayFlag;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/
	float RMSValue[16];
	int16_t Wave[16][12*32];
}ModuleComFaultSelStruct;
#endif
#if 0
typedef struct{
	u8 FaultFlag;
	u8 CanId;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float ratio;	
	float RMSValue;
	int16_t Wave[12*32];
}ModuleComFaultAllSelSaveStruct;
#else
typedef struct{
	//u8 SelFaultFlag;	//选线机内的线
	//u8 SelCanId;	//选线CANID
	u8 FaultFlag[SelWaveNum];
	u8 CanId[SelWaveNum];         	/*BIT0 rx*/	
	AlarmTimStruct Tim[SelWaveNum];	
	float ratio[SelWaveNum];	
	float RMSValue[SelWaveNum];
	int16_t Wave[SelWaveNum][12*32];
}ModuleComFaultAllSelSaveStruct;

typedef struct{
	u8 SelFaultFlag;	//选线机内的线
	u8 SelCanId;	//选线CANID
	u8 FaultFlag[SelWaveNum];
	u8 CanId[SelWaveNum];         	/*BIT0 rx*/	
	AlarmTimStruct Tim[SelWaveNum];	
	float ratio[SelWaveNum];	
	float RMSValue[SelWaveNum];
	int16_t Wave[SelWaveNum][20*256*2];
}CanFaultSelSaveStruct;

#endif

typedef struct{
	u8 DislayFlag;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/
	float RMSValue[16];
	int16_t Wave[16][12*32];
}ModuleComFaultAllSelStruct;


typedef struct{
	//u8 RF433buf[2048];
	//u8 Net4Gbuf[4][2048];
	//u8 SDbuf[2048];
	uint8_t Modulebuf[20*1024];
	FaultSaveStruct NandRead;	
	ModuleComFaultStruct FaultComBuf;
	//FaultSelSaveStruct NandSelRead;
	FaultAllSelSaveStruct NandAllSelRead;
	//ModuleComFaultSelStruct FaultSelComBuf;
	ModuleComFaultAllSelSaveStruct FaultAllSelComBuf;
	uint8_t RS458U1RXbuf[512];
	uint16_t RS458U1RXbt;
	uint8_t RS458U1TXbuf[512];
	
	float RMSValue[16];			//add by seleen 2022 1/ 11 增加16路校准电流
	//uint16_t RS458U1TXBT;

	//cal <-> can ext
	uint8_t CanReadChannel;
	uint8_t CalRxbuf[iCanNum][iCanbufSize];
	uint16_t CalRxbufPt[iCanNum];
	uint16_t CalRxbufBt[iCanNum];
	float CalRxRms[iCanNum];
	uint8_t MaxCanID;
	uint8_t CanOnlineTable[16];
	uint16_t CanBreathTime[iCanNum];
	uint8_t CalTable[iCanNum];
	uint8_t CalWhole[iCanbufSize/2+512];
	uint8_t CalTxbuf[1024];
	AlarmTimStruct CalTim;
	float Calratio;
	uint8_t CalMaxWaveTble;
	uint8_t CalWave[iCanWavebufSize];
}MemStruct;
extern MemStruct *Membuf;
#define iCanBreathTime	3000
extern u8 FaultFifo_pt;
extern u8 FaultFifo_bt;
extern u8 FaultFifo_busy;

//extern uint16_t Frequency[3];	

#define FaultFifoSIZE	50
extern FaultSaveStruct *FaultFifo[];
extern FaultSelSaveStruct *FaultSelFifo[];
extern FaultAllSelSaveStruct *FaultAllSelFifo[];

extern CanFaultSelSaveStruct	CanFaultSelSave;

extern Setstruct SystemSet;
//extern AlarmStruct *SystemAlarm;
extern PowerQualityStruct *SystemPowerQuality;
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

extern CriteriaStruct CriteriaValue;

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
