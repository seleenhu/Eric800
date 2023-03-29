
#include "struct.h"
#include "mydefine.h"
#include "InitSystem.h"
#include "emwin_eric800.h"
#include <math.h>
#include "param.h"


#define InitRatedVoltage 10000
#define InitPt		100
#define InitCt		100
#define InitModulus 2
//#define Setting (SystemSet)


//Setstruct Setting;

//AlarmStruct *SDAlarmData=(AlarmStruct *)SDAlarm_SDRAM;
//FaultSaveStruct *SDSuddentAlarmData=(FaultSaveStruct *)SDSuddenAlarm_SDRAM;

RFStruct ActionProtectionSave[MaxActionSave];




uint8_t const DefineSensorType[12]={Bus_A,Bus_B,Bus_C,TouchUp_A,TouchUp_B,TouchUp_C,TouchDown_A,TouchDown_B,TouchDown_C,Cupboard_T,Cupboard_H,OverVoltage_P};

uint32_t AlarmVaCount;
uint32_t AlarmGaCount;
uint32_t AlarmSaCount;
uint32_t AlarmTaCount;
uint32_t AlarmRaCount;

u8 StartReadSD_VA_Flag=0;
u8 StartReadSD_GA_Flag=0;
u8 StartReadSD_GASEL_Flag=0;
u8 StartReadSD_SVA_Flag=0;  //系统过压
u8 StartReadSD_SA_Flag=0;
u8 StartReadSD_TA_Flag=0;
u8 StartReadSD_UVA_Flag=0;
u8 StartReadSD_RA_Flag=0;
//u8 StartReadSD_UA_Flag=0;
u8 EndReadSDFlag=0;
uint32_t VA_WavePosition;
uint32_t GA_WavePosition;
uint32_t SVA_WavePosition;
uint32_t SA_WavePosition;
uint32_t TA_WavePosition;
uint32_t UVA_WavePosition;
uint32_t RA_WavePosition;
//uint32_t UA_WavePosition;






void InitGUISystem(void)
{
	
//	FAULT_RECORD_WAVE FaultRecordWave;
//  systemtime Settime;

//
		char Filename[32];
		uint8_t m,k;
		int i;
	
	
	//=====初始化设置参数
	//Setting.VoltageSet.MaxRatedValue=InitRatedVoltage;
	//Setting.VoltageSet.Modulus=InitModulus;
	//Setting.RatioStruct.RatioCT=InitCt;
	//Setting.RatioStruct.RatioPT=InitPt;
	//==========
	//Setting.ADCUCompSet.MaxValue=CalculateMaxVoltage();
	//Setting.ADCUCompSet.MinValue=-CalculateMaxVoltage();
	//ADCParameterSet();
	//=========================
	//=====================初始化 柜体传感器
	//SystemAlarm->filenameV="VA.txt";
	//Setting.StartCompleteOK=0;
	

	
	
	
	i=0;
	//=====test
	for(m=0;m<20;m++)
	{
		for(k=0;k<12;k++)
		{
				//Cupboard_P[m][k].Id=DefineId[m][k];
			//Cupboard_P[m][k].Id=g_tParam.CupboardID[m][k];			
			Cupboard_P[m][k].Id=SystemSet.CupboardID[m][k];
			//======
			#if	1
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAB=SystemSet.ActionCnAB[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAC=SystemSet.ActionCnAC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntBC=SystemSet.ActionCnBC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAD=SystemSet.ActionCnAD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntBD=SystemSet.ActionCnBD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntCD=SystemSet.ActionCnCD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAD=SystemSet.ActionTimeAD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeBD=SystemSet.ActionTimeBD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeCD=SystemSet.ActionTimeCD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAB=SystemSet.ActionTimeAB[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeBC=SystemSet.ActionTimeBC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAC=SystemSet.ActionTimeAC[m];
			#else
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAB=g_tParam.ActionCnAB[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAC=g_tParam.ActionCnAC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntBC=g_tParam.ActionCnBC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntAD=g_tParam.ActionCnAD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntBD=g_tParam.ActionCnBD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionCntCD=g_tParam.ActionCnCD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAD=g_tParam.ActionTimeAD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeBD=g_tParam.ActionTimeBD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeCD=g_tParam.ActionTimeCD[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAB=g_tParam.ActionTimeAB[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeBC=g_tParam.ActionTimeBC[m];
				Cupboard_P[m][k].Value.ProtectorData.ActionTimeAC=g_tParam.ActionTimeAC[m];
			#endif
				Cupboard_P[m][k].Value.ProtectorData.BatteryValue=0;
				Cupboard_P[m][k].Value.ProtectorData.LeakgaeCurrent=0x7fff;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHA=0xff;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHB=0xff;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHC=0xff;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTA=0x7FFF;//30000;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTB=0x7FFF;//30000;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTC=0x7FFF;//30000;
				Cupboard_P[m][k].Value.HumdityCupboard=0xff;//120;
				Cupboard_P[m][k].Value.TempCupboard=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempBusA=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempBusB=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempBusC=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchDownA=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchDownB=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchDownC=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchUpA=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchUpB=0x7FFF;//30000;
				Cupboard_P[m][k].Value.TempTouchUpC=0x7FFF;//30000;
				Cupboard_P[m][k].TimeStamp.year=10;
				Cupboard_P[m][k].TimeStamp.month=4;
				Cupboard_P[m][k].TimeStamp.day=11;
				Cupboard_P[m][k].TimeStamp.hour=18;
				Cupboard_P[m][k].TimeStamp.minute=00;
				Cupboard_P[m][k].TimeStamp.second=20;
				Cupboard_P[m][k].TimeStamp.us=500;
				Cupboard_P[m][k].OnlineFlag=0;
				if(k==OverVoltage_P)
				{
					sprintf(Filename,"P%d.txt\0",i);
					strcpy(Cupboard_P[m][k].filename,Filename);
					sprintf(Filename,"PS%d.txt\0",i);
					strcpy(Cupboard_P[m][k].filenameS,Filename);
					
				}
					else
					{
						sprintf(Filename,"A%d.txt\0",i);
						strcpy(Cupboard_P[m][k].filename,Filename);
					}
				i++;
				
				Cupboard_P[m][k].Type=DefineSensorType[k];
			  
		}
		//Setting.ProtectorType[m]=g_tParam.ProtectorType[m];
		Setting.ProtectorType[m]=SystemSet.ProtectorType[m];
		
	}
	
	
	
	
	
	
		
//=====load Calibration data

		Setting.flag=1;
	
	
	
	//Setting.flag=1;


    const double pi=acos(-1.0);
	
	
    
 
	//ReadSdAlarmInfo();
	
	
}
//=========================================================













/*===================================================================
function:数组按从小到大排序
input:数组 个数
out:反回数组中的最大值
by:seleen hu 2018 6 1
=====================================================================*/
float sort(float *src, int len)
{
    float tem;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len - i - 1; j++) // ?????,????
            if (src[j] > src[j+1])  // ????????????,???
            {
                tem = src[j];
                src[j] = src[j+1];
                src[j+1] = tem;
            }
    }
		return src[len-1];
}

