
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
	u8 FaultFlag; 			/*������*/
	float RMSValue[16];
	int16_t Wave[16][20*128];
}FaultSaveStruct;





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

#define PARAM_ADDR		16*1024*1024
#define PARAM_VER			0xC100

typedef struct{
	uint16_t ParamVer;
	struct _ADCParameter{		
		float fftratio[16];
	}ADCParameter;
	
}PoramStruct;


typedef struct{
	
	uint8_t VerifyFlag; //�ӵظ�λ��־

	uint8_t SPIBackCom;

	//uint8_t UartTxCom;
	uint8_t SelNum;
}ComStruct;
extern ComStruct ComData;

extern uint8_t PoramFlag;
extern PoramStruct PoramData; //��������


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
#endif
#define SDindexSize    80


typedef struct{
	u32 ChipID[3];
	u16 ChipFlashSize;
}ChipICStruct;

extern ChipICStruct ChipICID;

typedef struct{
	u8 DislayFlag;         	/*BIT0 4G,BIT1 SD, BIT1 LCD*/
	u8 FaultStyle;          /*��������*/
	AlarmTimStruct Tim;
	float ratio[8];
	float fftratio[8];
	u8 ValueFlag; 			/*��λ BIT0:A BIT1:B BIT2:C*/
	float RMSValue[8][15]; 	/*���ϵ���Ч0��14*/
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
