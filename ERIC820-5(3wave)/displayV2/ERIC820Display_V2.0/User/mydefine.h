
#ifndef _MYDEFINE_H_
#define	_MYDEFINE_H_

#define   TemMaxValue    12500
#define   TemMinValue    -4000


#define   ADCFifo_SDRAM_SIZE     				50*256*2
//#define   ADCFifo_SDRAM    	 						(SDRAM_APP1_BUF)
#define   FaultFifo_SDRAM    						(SDRAM_APP1_BUF)
#define   ADCWhole_SDRAM     						(FaultFifo_SDRAM + FaultSaveSIZE * 4 * (sizeof(FaultSaveStruct)/4 +1))
//#define   SystemAlarm_SDRAM     					(ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)	
//#define   SystemPowerQuality_SDRAM     				(SystemAlarm_SDRAM+(sizeof(AlarmStruct)/4+1)*4)	
#define   SystemPowerQuality_SDRAM					(ADCWhole_SDRAM+(sizeof(ADCWholeStruct)/4 + 1)*4)
#define   SystemActual_SDRAM     					(SystemPowerQuality_SDRAM+(sizeof(PowerQualityStruct)/4 + 1)*4)
#define   RFReception_SDRAM     					(SystemActual_SDRAM+(sizeof(ActualStruct)/4+1)*4)

#define   Mem_SDRAM									(RFReception_SDRAM+(sizeof(RFStruct)/4+1)*4)
#define   ModuleCom_SDRAM							(Mem_SDRAM + (sizeof(MemStruct)/4 + 1)*4)
#define   ModuleComFault_SDRAM						(ModuleCom_SDRAM + (sizeof(ModuleComStruct)/4 + 1)*4)
//#define   FaultSelFifo_SDRAM   						(ModuleComFault_SDRAM + (sizeof(ModuleComFaultStruct)/4 + 1)*4)
#define   FaultAllSelFifo_SDRAM 					(ModuleComFault_SDRAM + (sizeof(ModuleComFaultStruct)/4 + 1)*4)
//#define   ModuleComSelFault_SDRAM					(FaultSelFifo_SDRAM + FaultSaveSIZE * 4 * (sizeof(FaultAllSelSaveStruct)/4 +1))
#define   SDRAM_APP1_END							(FaultAllSelFifo_SDRAM + FaultSaveSIZE * 4 * (sizeof(FaultAllSelSaveStruct)/4 +1))



#if 0
typedef struct{
	uint16_t pt;
	uint16_t bt;
	uint16_t Instantbt;
	uint16_t Calbt;
	uint16_t Faultbt;
	uint16_t FifoSize;	
	float rati0[8];	
	float fftratio[8];	
	float RMS_buf[8];
	//float Display_buf[8];
	int16_t ADCFIFO[8][ADCFifo_SDRAM_SIZE/2];	
}ADCFifoStruct;
#endif


typedef struct{
	//int16_t RMSbuf[8][1024];
	//float RMS_Elec[4][8];
	
	
	//int16_t FFTBuf[8][1024];
	//long FFTOutBuf[8][1024];
	
	//int16_t FFTBuf1[8][1024];
	//long FFTOutBuf1[8][1024];
	
	//int16_t VerifyBuf[8][1024];
	
	//float fft_RMS[4][8];
	//uint8_t SPI4Buf[SPI4BufSize];
	uint8_t SPI4Buf[60*256*2];
	float floatRMS[128];
}ADCWholeStruct;

#pragma pack (1)
typedef struct{	
	uint32_t ParamVer;			/* 参数区版本控制（可用于程序升级时，决定是否对参数区进行升级） */
	u8 flag;
	uint8_t ParamSaveFlag;
	u8 DriveTestFlag;//传动试验开启/中断 
	u8 DistanceFlag;//远方/就地
	u8 Passwords[4];//设置密码
	
	u8 BackLightTime;	//关背光时间 单位分钟
	u8 SafeSoundTime;	//安全提示音 时间 单位秒
	u8 TouchToneFlag;//触摸按键音 0:开 1：关

	

	/* LCD背光亮度 */
	//uint8_t ucBackLight;

	/* 触摸屏校准参数 */
	//{
		uint8_t TouchDirection;	/* 屏幕方向 0-3  0表示横屏，1表示横屏180° 2表示竖屏 3表示竖屏180° */
		
		uint8_t XYChange;		/* X, Y 是否交换， 1表示i切换，0表示不切换  */
		
		uint16_t usAdcX1;	/* 左上角 */
		uint16_t usAdcY1;
		uint16_t usAdcX2;	/* 右下角 */
		uint16_t usAdcY2;
		uint16_t usAdcX3;	/* 左下角 */
		uint16_t usAdcY3;
		uint16_t usAdcX4;	/* 右上角 */
		uint16_t usAdcY4;
		
		uint16_t usLcdX1;	/* 校准时，屏幕坐标 */
		uint16_t usLcdY1;	/* 校准时，屏幕坐标 */
		uint16_t usLcdX2;	/* 校准时，屏幕坐标 */
		uint16_t usLcdY2;	/* 校准时，屏幕坐标 */
		uint16_t usLcdX3;	/* 校准时，屏幕坐标 */
		uint16_t usLcdY3;	/* 校准时，屏幕坐标 */
		uint16_t usLcdX4;	/* 校准时，屏幕坐标 */
		uint16_t usLcdY4;	/* 校准时，屏幕坐标 */	
	//}
	
	
	
	uint8_t  ProtectorType[20];//保护器类型 0: 电站型 1：电机型 2：电容型 
	float LineTempMax;
	/*可以去掉*/
	float LeakageCurrentMax;

	uint16_t ActionCnAD[20];
	uint16_t ActionCnBD[20];
	uint16_t ActionCnCD[20];
	uint16_t ActionCnAB[20];
	uint16_t ActionCnBC[20];
	uint16_t ActionCnAC[20];
	uint16_t ActionTimeAD[20];
	uint16_t ActionTimeBD[20];
	uint16_t ActionTimeCD[20];
	uint16_t ActionTimeAB[20];
	uint16_t ActionTimeBC[20];
	uint16_t ActionTimeAC[20];
	
	uint32_t CupboardID[20][12];
	
	//uint8_t NetSelectFlag;
	//u8 RunFlag; //bit1 1 OK 0 没有投运信号 bit0 1 PT OK 0 PT ERR
	struct _ADCParameter{
		float ratio[8];
		float fftratio[8];
	}ADCParameter;

	
	struct _Net{
		u8 IPadrr[4]; /*服务器的IP地址*/
		u16 Port;			/*服务器的端口号*/
		u16 Modbusadr; /*MODBUST通讯用的地址*/
		u8 ModbudID[20];/*MODBUST通讯用的ID*/
		u8 NetSelectFlag;//0: 4G 1:有线网络
	}NetSet;
	
	struct _RS485{
		u32 Baud;			/*485的波特率*/
		u8  adr;			/*485从机地址*/
	}RS485Set;
	
	struct _RS232{
		u32 Baud;			/*232的波特率*/		
	}RS232Set;
	
	/*可以去掉*/
	struct _Current{
		float MaxValue; /*最大值*/
		float MinValue; /*最小值*/
	}CurrentSet;
	
	struct _Voltage{
		uint32_t MaxRatedValue; /*最大额定电压值*/
		float Modulus; /*系数*/
	}VoltageSet;
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
	
	struct _Ratio{
		u16 RatioPT;
		u16 RatioCT;		
	}RatioStruct;
	
	struct _Lcd{
		u16 LcdOnTim; /*Lcd打开时间设定*/
		u8 LcdFlag;  /*0 关闭   1 打开*/
	}LcdSet;
	
	struct _syntony{
		float SyntonyV[3][4];/*A B C三相的5次谐振 10次谐振 15次谐振 20次谐振*/
	}SyntonySet;
	
	struct _ShortGND{
		float I0;
	}ShortGNDSet;
	uint8_t LineNamebuf[50];
	uint8_t CupboardSIZE;
	//uint16_t SelRatioCT;
	uint16_t SelRatioCTBuf[16];

	uint8_t SelMasterSlaveFlag; //选线主从标志
	uint8_t SelSlaveNumSet; //从机台数
	
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
		uint8_t F100ms:1;
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
	LowValueFault = 7,
}FaultEnum;

#if 0
typedef struct{
	u8 DislayFlag;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float fftratio[16];
	u8 FaultFlag; 			/*故障相*/
	float RMSValue[16];
	int16_t Wave[16][20*128];
}FaultSelSaveStruct;
#endif
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

typedef struct{
	u8 FaultFlag[SelWaveNum];
	u8 CanId[SelWaveNum];         	/*BIT0 rx*/	
	AlarmTimStruct Tim[SelWaveNum];	
	float ratio[SelWaveNum];	
	float RMSValue[SelWaveNum];
	int16_t Wave[SelWaveNum][12*32];
}ModuleComFaultAllSelSaveStruct;


#pragma pack (1)
typedef struct{
	u8 DislayFlag;         	/*BIT0 4G,BIT1 SD, BIT2 LCD*/
	u8 FaultStyle;          /*故障类型*/
	AlarmTimStruct Tim;
	float ratio[8];
	float fftratio[8];
	u8 ValueFlag; 					/*相位 BIT0:A BIT1:B BIT2:C*/
	float RMSValue[8][15]; 	/*故障点有效0～14*/
	float VabuleSin[8][15];
	float VabuleCos[8][15];
	int16_t Wave[8][20*256];
}FaultSaveStruct;
#pragma pack ()

#define FaultSaveSIZE   10
extern FaultSaveStruct *FaultFifo[];
//extern FaultSelSaveStruct *FaultSelFifo[];
extern FaultAllSelSaveStruct *FaultAllSelFifo[];

extern u8 FaultFifo_pt;

//#pragma pack (1)
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
//#pragma pack ()

typedef struct{
	float ElecU[4];
	struct _RF{
		AlarmTimStruct Tim;
		u8 ID[9];
		float TempValue;
		float HumidityValue;
		float LeakageValue;		
	}ActualRF;
}ActualStruct;

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

//=================
typedef struct{
	int16_t TempBusA;		//A 相母线
	int16_t TempBusB;		//B 相母线
	int16_t TempBusC;		//C 相母线
	int16_t TempTouchUpA;	//A 相上触点
	int16_t TempTouchUpB;	//B 相上触点
	int16_t TempTouchUpC;	//C 相上触点
	int16_t TempTouchDownA; //A 相下触点
	int16_t TempTouchDownB; //B 相下触点	
	int16_t TempTouchDownC; //C 相下触点
	int16_t TempCupboard;	 //柜体温度
	int16_t	HumdityCupboard;	 //柜体湿度
struct{
	uint16_t LeakgaeCurrent;//保护器漏电流
	int16_t OverVoltageTA;//过电压保护器A相温度
	int16_t OverVoltageTB;//过电压保护器B相温度
	int16_t OverVoltageTC;//过电压保护器C相温度
	uint8_t OverVoltageHA;//过电压保护器A相湿度	
	uint8_t OverVoltageHB;//过电压保护器B相湿度	
	uint8_t OverVoltageHC;//过电压保护器C相湿度	
	uint16_t ActionCntAD;	//AD相动作次数
	uint16_t ActionCntBD;	//BD相动作次数
	uint16_t ActionCntCD;	//CD相动作次数
	uint16_t ActionCntAB;	//AB相动作次数
	uint16_t ActionCntAC;	//AC相动作次数
	uint16_t ActionCntBC;	//BC相动作次数
	uint16_t ActionTimeAD;	//AD相动作时长
	uint16_t ActionTimeBD;	//BD相动作时长
	uint16_t ActionTimeCD;	//CD相动作时长
	uint16_t ActionTimeAB;	//AB相动作时长
	uint16_t ActionTimeAC;	//AC相动作时长
	uint16_t ActionTimeBC;	//BC相动作时长
	uint16_t BatteryValue;		//保护器电池电量
}ProtectorData;
}SensorData;

typedef struct{
	uint32_t RelayId;	
	u32 RFIDbuf[20];
	u8 RelayState;
	u16 OnLineTime;
}RelayStruct;

typedef struct{
	uint32_t Id;
	uint8_t RelayFlag;
	uint8_t OnlineFlag;//add by seleen 2018 10 24
	AlarmTimStruct TimeStamp;
	uint8_t Type;
	char filename[32];
	char filenameS[32];//保护器动作存储文件
	SensorData Value;
	uint32_t KeepTim; //保持时间
}CupboardParameter;


typedef struct{
	/*温度*/
	struct _RFTem{
		uint32_t Id;
		AlarmTimStruct TimeStamp;
		int16_t TemValue;
	}RFTem;
	
	/*保护器*/
	struct{
		struct _ProtectorTem{			
			uint32_t Id;
			AlarmTimStruct Tim;	
			int16_t TemA;
			uint8_t HumA;
			int16_t TemB;
			uint8_t HumB;
			int16_t TemC;
			uint8_t HumC;
			uint16_t LeakageCurrent;
			uint16_t BatteryVoltage;
		}ProtectorTem;/*定时数据*/
		
		struct _ProtectorAction{
			u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
			u8	Phase;
			uint32_t Id;
			AlarmTimStruct Tim;
			uint16_t ActionTime;	//BC相动作时长
			
			int16_t TemA;
			uint8_t HumA;
			int16_t TemB;
			uint8_t HumB;
			int16_t TemC;
			uint8_t HumC;
			uint16_t LeakageCurrent;
			//int16_t Wave[8][20*256];			/*前十后十的波形缓存*/
		}ProtectorAction;/*突发数据*/
	}Protector;
}RFStruct;





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

#define SDindexSize    80

typedef struct{
	uint8_t CanOnlineTable[16];
}CanStruct;


typedef struct{
	u32 ChipID[3];
	u16 ChipFlashSize;
}ChipICStruct;

extern ChipICStruct ChipICID;

typedef struct{
	u16 RS485buf[1024];
	u8 RF433buf[2048];
	u8 Net4Gbuf[4][2048];
	u8 SDbuf[2048];
	u8 ComMembuf[20*128*2];
	float RMSValue[16]; // add by seleen  2022 1 11 校准值
}MemStruct;
extern MemStruct *Membuf;

#define  SDGroundFault_index  NandCfgData.GndPtNew
#define  SDGroundFault_sum 	NandCfgData.TotalGndPt
#define  SDExceedValue_index NandCfgData.ExPtNew
#define  SDExceedValue_sum  NandCfgData.TotalExPt
#define  SDInstantaneousVoltageFault_index NandCfgData.SExPtNew
#define  SDInstantaneousVoltageFault_sum  NandCfgData.TotalSExPt
#define  SDPTFault_index NandCfgData.PTPtNew
#define  SDPTFault_sum	NandCfgData.TotalPTPT
#define  SDUnderValFault_index NandCfgData.DEPtNew
#define  SDUnderValFault_sum NandCfgData.TotalDEPT
#define  SDShortFault_index NandCfgData.SHPtNew
#define  SDShortFault_sum NandCfgData.TotalSHPT

//extern u8 SDExceedValue_index;
//extern u8 SDExceedValue_sum;										 
//extern u8 SDGroundFault_index;
//extern u8 SDGroundFault_sum;												 
//extern u8 SDShortFault_index;
//extern u8 SDShortFault_sum;											 
//extern u8 SDInstantaneousVoltageFault_index;
//extern u8 SDInstantaneousVoltageFault_sum;

//extern u8 SDInstantaneousXFault_index;
//extern u8 SDInstantaneousXFault_sum;

//extern u8 SDPTFault_index;
//extern u8 SDPTFault_sum;	
//extern u8 SDUnderValFault_index;
//extern u8 SDUnderValFault_sum;	




extern Setstruct SystemSet;
//extern AlarmStruct *SystemAlarm;
extern PowerQualityStruct *SystemPowerQuality;
extern ActualStruct *SystemActual;

//extern ADCFifoStruct ADCFifo;
//extern ADCFifoStruct *ADCFifo;
extern ADCWholeStruct *ADCWholeVar;

#define FlagSystemState		0xFF
#define FlagSystemStateNet	0x01
#define FlagSystemStateSave 0x02
extern SystemStateStruct 	SystemState;

extern RFStruct *RFReception;
#define RfTemRatio  			10
#define RfProtectorTemRatio  	100
extern CupboardParameter Cupboard_P[20][12];	

//extern UnionRamStruct UnionRam[8];

extern CriteriaStruct CriteriaValue;

extern const float radical3;
extern const float radical2;	

extern uint16_t uSCount;

extern RelayStruct Relay[];

extern BitUnion WholeBit;
#define iScreenCloseTim		60
extern uint16_t ScreenCloseTim;
#define ScreenClrTim()	(ScreenCloseTim = 0)
uint8_t ScreenCloseDisplay(void);
//extern OS_MEM GlobalMem;
#define MCUSoft_RST()   do{__set_FAULTMASK(1);NVIC_SystemReset();}while(0)



typedef enum{
	Messge_idle = 0,
	Messge_Start,
	Messge_SkipWave,
	Messge_Bussy,
}MessgeFlagEnum;
extern MessgeFlagEnum MessgeFlag;

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

extern void RFTemSend(u8 i,u8 j,struct _RFTem *RFbuf);

extern void RFProtectorTemSend(u8 i,u8 j,struct _ProtectorTem *RFbuf);
extern void RFProtectorActionSend(u8 i,u8 j,struct _ProtectorAction *RFbuf);
extern void SaveSDFault(void);
extern void ReadSDFault(void);
void	GetChipID(void);
void PlayAudio(void);
int UnicodeToUtf8(char* pInput, char *pOutput);

//extern void MemInit(void);
//extern void *MemGet(void);
//extern void MemPut(void *pt);
#endif
/*---------end-------------------------------*/
