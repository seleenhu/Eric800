#ifndef InitSystem_H_INCLUDED
#define InitSystem_H_INCLUDED
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "struct.h"
#include "mydefine.h"
#include "emwin_eric800.h"	
	
#define Use_AllData	


//extern FAULT_RECORD_WAVE FaultRecordWave;
//extern systemtime Settime;
	
#define MaxActionSave 20	
#define Setting SystemSet
#define PowerQuality SystemPowerQuality	
#define SaveTempDataLength 177	
	
//#define AlarmVALength	81*1024
#define AlarmVALength	81963	
#define AlarmGALength 81963
#define AlarmSALength 81962	
#define AlarmRALength 81975
#define AlarmTALength 81963	
	
#define WaveLength  81920	
	
#define VoltageDataOffset 	0	
#define G_VoltageDataOffset 0		
#define S_VoltageDataOffset 0
#define T_VoltageDataOffset 0	//27
#define R_VoltageDataOffset 0

	
extern u8 StartReadSD_VA_Flag;
extern u8 StartReadSD_SVA_Flag;
extern u8 StartReadSD_GA_Flag;
extern u8 StartReadSD_SA_Flag;
extern u8 StartReadSD_TA_Flag;
extern u8 StartReadSD_UVA_Flag;
extern u8 StartReadSD_RA_Flag;
extern u8 StartReadSD_UA_Flag;

extern u8 EndReadSDFlag;
extern uint32_t AlarmVaCount;
extern uint32_t AlarmGaCount;		
extern uint32_t AlarmSaCount;
extern uint32_t AlarmTaCount;
extern uint32_t AlarmRaCount;

extern uint32_t VA_WavePosition;
extern uint32_t SVA_WavePosition;
extern uint32_t GA_WavePosition;	
extern uint32_t SA_WavePosition;
extern uint32_t TA_WavePosition;
extern uint32_t UVA_WavePosition;
extern uint32_t RA_WavePosition;
extern uint32_t UA_WavePosition;

extern AlarmStruct *SDAlarmData;
extern FaultSaveStruct *SDSuddentAlarmData;

extern RFStruct ActionProtectionSave[MaxActionSave];


	
extern void InitGUISystem(void);
#if 0	
extern int16_t CalculateMaxVoltage(void);	
#endif
	
extern void  ReadSDAlarmVALenght(void);
extern void  ReadSDAlarmVAData(int32_t n);
extern void  ReadSDAlarmGALenght(void);
extern void  ReadSDAlarmGAData(int32_t n);
extern void  ReadSDAlarmSALenght(void);
extern void  ReadSDAlarmSAData(int32_t n);
extern void  ReadSDAlarmRALenght(void);
extern void  ReadSDAlarmRAData(int32_t n);
extern void  ReadSDAlarmTALenght(void);
extern void  ReadSDAlarmTAData(int32_t n);
void  ReadSDAlarmSVAData(int32_t n);
void  ReadSDAlarmUVAData(int32_t n);
void ReadSdAlarmInfo(void);

extern float sort(float *src, int len);

	
#endif /* __InitSystem_CONF_H */




