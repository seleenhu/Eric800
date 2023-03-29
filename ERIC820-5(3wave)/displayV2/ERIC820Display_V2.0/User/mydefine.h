
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
	uint32_t ParamVer;			/* �������汾���ƣ������ڳ�������ʱ�������Ƿ�Բ��������������� */
	u8 flag;
	uint8_t ParamSaveFlag;
	u8 DriveTestFlag;//�������鿪��/�ж� 
	u8 DistanceFlag;//Զ��/�͵�
	u8 Passwords[4];//��������
	
	u8 BackLightTime;	//�ر���ʱ�� ��λ����
	u8 SafeSoundTime;	//��ȫ��ʾ�� ʱ�� ��λ��
	u8 TouchToneFlag;//���������� 0:�� 1����

	

	/* LCD�������� */
	//uint8_t ucBackLight;

	/* ������У׼���� */
	//{
		uint8_t TouchDirection;	/* ��Ļ���� 0-3  0��ʾ������1��ʾ����180�� 2��ʾ���� 3��ʾ����180�� */
		
		uint8_t XYChange;		/* X, Y �Ƿ񽻻��� 1��ʾi�л���0��ʾ���л�  */
		
		uint16_t usAdcX1;	/* ���Ͻ� */
		uint16_t usAdcY1;
		uint16_t usAdcX2;	/* ���½� */
		uint16_t usAdcY2;
		uint16_t usAdcX3;	/* ���½� */
		uint16_t usAdcY3;
		uint16_t usAdcX4;	/* ���Ͻ� */
		uint16_t usAdcY4;
		
		uint16_t usLcdX1;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdY1;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdX2;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdY2;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdX3;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdY3;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdX4;	/* У׼ʱ����Ļ���� */
		uint16_t usLcdY4;	/* У׼ʱ����Ļ���� */	
	//}
	
	
	
	uint8_t  ProtectorType[20];//���������� 0: ��վ�� 1������� 2�������� 
	float LineTempMax;
	/*����ȥ��*/
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
	//u8 RunFlag; //bit1 1 OK 0 û��Ͷ���ź� bit0 1 PT OK 0 PT ERR
	struct _ADCParameter{
		float ratio[8];
		float fftratio[8];
	}ADCParameter;

	
	struct _Net{
		u8 IPadrr[4]; /*��������IP��ַ*/
		u16 Port;			/*�������Ķ˿ں�*/
		u16 Modbusadr; /*MODBUSTͨѶ�õĵ�ַ*/
		u8 ModbudID[20];/*MODBUSTͨѶ�õ�ID*/
		u8 NetSelectFlag;//0: 4G 1:��������
	}NetSet;
	
	struct _RS485{
		u32 Baud;			/*485�Ĳ�����*/
		u8  adr;			/*485�ӻ���ַ*/
	}RS485Set;
	
	struct _RS232{
		u32 Baud;			/*232�Ĳ�����*/		
	}RS232Set;
	
	/*����ȥ��*/
	struct _Current{
		float MaxValue; /*���ֵ*/
		float MinValue; /*��Сֵ*/
	}CurrentSet;
	
	struct _Voltage{
		uint32_t MaxRatedValue; /*�����ѹֵ*/
		float Modulus; /*ϵ��*/
	}VoltageSet;
	struct _Temp{
		float MaxValue; /*���ֵ*/
		float MinValue; /*��Сֵ*/
	}TempSet;
	
	struct _Humidity{
		float MaxValue; /*���ֵ*/
		float MinValue; /*��Сֵ*/
	}HumiditySet;
	
	
	struct _LeakageCurrent{
		float LeakageValue;
	}LeakageCurrentSet;
	
	struct _Ratio{
		u16 RatioPT;
		u16 RatioCT;		
	}RatioStruct;
	
	struct _Lcd{
		u16 LcdOnTim; /*Lcd��ʱ���趨*/
		u8 LcdFlag;  /*0 �ر�   1 ��*/
	}LcdSet;
	
	struct _syntony{
		float SyntonyV[3][4];/*A B C�����5��г�� 10��г�� 15��г�� 20��г��*/
	}SyntonySet;
	
	struct _ShortGND{
		float I0;
	}ShortGNDSet;
	uint8_t LineNamebuf[50];
	uint8_t CupboardSIZE;
	//uint16_t SelRatioCT;
	uint16_t SelRatioCTBuf[16];

	uint8_t SelMasterSlaveFlag; //ѡ�����ӱ�־
	uint8_t SelSlaveNumSet; //�ӻ�̨��
	
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
		int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		u32 ExceedValue[8];
		u8 ValueFlag;   /*��ѹ����λ BIT0:A BIT1:B BIT3:C*/
	}VoltageAlarm;
	
	
	struct _TempAlarm{
		AlarmTimStruct Tim;		
		float ExceedTemp;
		u8 ID[9]; 							/*�¶ȳ����ID*/  
	}TempAlarm;
	
	struct _HumidityAlarm{
		AlarmTimStruct Tim;
		//const u16 *Wave;			/*ǰʮ��ʮ�Ĳ��λ���*/
		float ExceedTemp;
		u8 ID[9]; 							/*�¶ȳ����ID*/  
	}HumidityAlarm;
	
	struct _GroundAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		u8 ValueFlag;   /*��λ BIT0:A BIT1:B BIT3:C BIT7 1 ���� 0 ����*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		u8 ResetFlag;	// 1 ResetButton on
	}GroundAlarm;
	
	struct _GridAlarm{		
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD,BIT2 LCD*/
		AlarmTimStruct Tim;		
		u8 ValueFlag;   /*��λ BIT0:17hz BIT1:25hz BIT3:150hz*/
		u32 GridValue[3];
		u32 RMSValue[8];
		int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		//u8	Grid;							/*г�����*/
		//float GridAValue;
		//float GridBValue;
		//float GridCValue;		
	}GridAlarm;
	
	struct _ShortAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		//u8 ValueFlag;   							/*��λ BIT0:AB BIT1:BC BIT3:AC*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		//u32 U0Value;
	}ShortAlarm;
	
	struct _PTAlarm{
		u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
		AlarmTimStruct Tim;
		int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		u8 ValueFlag;   							/*��λ BIT0:A BIT1:B BIT2:C*/
		u32 RMSValue[8];
		//u32 UaValue;
		//u32 UbValue;
		//u32 UcValue;
		//u32 UlValue;
	}PTAlarm;
	
	char filenameV[32];	//save ��ѹ�����洢�ļ��� 
	char filenameG[32];	//save �ӵر����洢�ļ���  
	char filenameS[32]; //save ��·�����洢�ļ���  
	char filenameR[32];//Save resonate г�񱨾��ļ���
	char filenameT[32];//PT ���߱����ļ���
	
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
	u8 FaultFlag; 			/*������*/
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
	u8 FaultStyle;          /*��������*/
	AlarmTimStruct Tim;
	float ratio[8];
	float fftratio[8];
	u8 ValueFlag; 					/*��λ BIT0:A BIT1:B BIT2:C*/
	float RMSValue[8][15]; 	/*���ϵ���Ч0��14*/
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
	float ElecU[3]; /*��ѹ��Чֵ*/
	float Elec3U0;
	float ElecI[3]; /*������Чֵ*/
	float Elec3I0;
	float ElecPowerFactor[3]; /*��������*/
	float ElecActivePower[4];/*�й�����*/
  float ElecReactivePower[4];/*�޹�����*/
  float ElecApparentPower[4];/*���ڹ���*/
	float ElecThdu[3][30];/*��ѹг��*/
  float ElecThdi[3][30];/*����г��*/	 
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
ϵͳ״ָ̬ʾ�ṹ
*/
typedef struct{
	struct{
		uint8_t phaseAction;  /*bit0 A���־ bit1 B���־ bit2 C���־*/
		uint8_t AphaseState;  /*1��,2�ر�*/
		uint8_t BphaseState;  /*1��,2�ر�*/
		uint8_t CphaseState;  /*1��,2�ر�*/
	}SwitchState;
	
}SystemStateStruct;

//=================
typedef struct{
	int16_t TempBusA;		//A ��ĸ��
	int16_t TempBusB;		//B ��ĸ��
	int16_t TempBusC;		//C ��ĸ��
	int16_t TempTouchUpA;	//A ���ϴ���
	int16_t TempTouchUpB;	//B ���ϴ���
	int16_t TempTouchUpC;	//C ���ϴ���
	int16_t TempTouchDownA; //A ���´���
	int16_t TempTouchDownB; //B ���´���	
	int16_t TempTouchDownC; //C ���´���
	int16_t TempCupboard;	 //�����¶�
	int16_t	HumdityCupboard;	 //����ʪ��
struct{
	uint16_t LeakgaeCurrent;//������©����
	int16_t OverVoltageTA;//����ѹ������A���¶�
	int16_t OverVoltageTB;//����ѹ������B���¶�
	int16_t OverVoltageTC;//����ѹ������C���¶�
	uint8_t OverVoltageHA;//����ѹ������A��ʪ��	
	uint8_t OverVoltageHB;//����ѹ������B��ʪ��	
	uint8_t OverVoltageHC;//����ѹ������C��ʪ��	
	uint16_t ActionCntAD;	//AD�ද������
	uint16_t ActionCntBD;	//BD�ද������
	uint16_t ActionCntCD;	//CD�ද������
	uint16_t ActionCntAB;	//AB�ද������
	uint16_t ActionCntAC;	//AC�ද������
	uint16_t ActionCntBC;	//BC�ද������
	uint16_t ActionTimeAD;	//AD�ද��ʱ��
	uint16_t ActionTimeBD;	//BD�ද��ʱ��
	uint16_t ActionTimeCD;	//CD�ද��ʱ��
	uint16_t ActionTimeAB;	//AB�ද��ʱ��
	uint16_t ActionTimeAC;	//AC�ද��ʱ��
	uint16_t ActionTimeBC;	//BC�ද��ʱ��
	uint16_t BatteryValue;		//��������ص���
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
	char filenameS[32];//�����������洢�ļ�
	SensorData Value;
	uint32_t KeepTim; //����ʱ��
}CupboardParameter;


typedef struct{
	/*�¶�*/
	struct _RFTem{
		uint32_t Id;
		AlarmTimStruct TimeStamp;
		int16_t TemValue;
	}RFTem;
	
	/*������*/
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
		}ProtectorTem;/*��ʱ����*/
		
		struct _ProtectorAction{
			u8 DislayFlag;               /*BIT0 4G,BIT1 SD, BIT1 LCD*/
			u8	Phase;
			uint32_t Id;
			AlarmTimStruct Tim;
			uint16_t ActionTime;	//BC�ද��ʱ��
			
			int16_t TemA;
			uint8_t HumA;
			int16_t TemB;
			uint8_t HumB;
			int16_t TemC;
			uint8_t HumC;
			uint16_t LeakageCurrent;
			//int16_t Wave[8][20*256];			/*ǰʮ��ʮ�Ĳ��λ���*/
		}ProtectorAction;/*ͻ������*/
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
	Bus_A=0,		//A ��ĸ��
	Bus_B,		//B ��ĸ��
	Bus_C,		//C ��ĸ��
	TouchUp_A,	//A ���ϴ���
	TouchUp_B,	//B ���ϴ���
	TouchUp_C,	//C ���ϴ���
	TouchDown_A, //A ���´���
	TouchDown_B, //B ���´���	
	TouchDown_C, //C ���´���
	Cupboard_T,	 //�����¶�
	Cupboard_H,	 //����ʪ��
	OverVoltage_P
	/*
	Leakgae_Current,//©����
	OverVoltage_TA,//����ѹ������A���¶�
	OverVoltage_TB,//����ѹ������B���¶�
	OverVoltage_TC,//����ѹ������C���¶�
	OverVoltage_HA,//����ѹ������A��ʪ��
	OverVoltage_HB,//����ѹ������B��ʪ��
	OverVoltage_HC,//����ѹ������C��ʪ��
	Action_CntA,	//����ѹ������A�ද������
	Action_CntB,	//����ѹ������B�ද������
	Action_CntC,	//����ѹ������C�ද������
	Action_CntAB,	//����ѹ������AB�ද������
	Action_CntAC,	//����ѹ������AC�ද������
	Action_CntBC,	//����ѹ������BC�ද������
	Action_TimeA,	//����ѹ������A�ද��ʱ��
	Action_TimeB,	//����ѹ������B�ද��ʱ��
	Action_TimeC,	//����ѹ������C�ද��ʱ��
	Action_TimeAB,	//����ѹ������AB�ද��ʱ��
	Action_TimeAC,	//����ѹ������AC�ද��ʱ��
	Action_TimeBC,	//����ѹ������BC�ද��ʱ��
	Battery_Value		//��������ص���
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
	float RMSValue[16]; // add by seleen  2022 1 11 У׼ֵ
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
