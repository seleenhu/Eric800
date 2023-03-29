
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

AlarmStruct *SDAlarmData=(AlarmStruct *)SDAlarm_SDRAM;
FaultSaveStruct *SDSuddentAlarmData=(FaultSaveStruct *)SDSuddenAlarm_SDRAM;

RFStruct ActionProtectionSave[MaxActionSave];




uint8_t const DefineSensorType[12]={Bus_A,Bus_B,Bus_C,TouchUp_A,TouchUp_B,TouchUp_C,TouchDown_A,TouchDown_B,TouchDown_C,Cupboard_T,Cupboard_H,OverVoltage_P};

uint32_t AlarmVaCount;
uint32_t AlarmGaCount;
uint32_t AlarmSaCount;
uint32_t AlarmTaCount;
uint32_t AlarmRaCount;

u8 StartReadSD_VA_Flag=0;
u8 StartReadSD_GA_Flag=0;
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

#if 0
int16_t CalculateMaxVoltage(void)
{
	const float Root2=1.41421;
	const float Root3=1.73205;
	int16_t MaxVoltage;
	float LineVoltage;
	float PeakVoltage;
	float Voltage;
	float Pt;
	Pt=Setting.RatioStruct.RatioPT;
	LineVoltage=Setting.VoltageSet.Modulus*Setting.VoltageSet.MaxRatedValue/Root3;//线电压
	PeakVoltage=LineVoltage*Root2;//线电压峰值
	Voltage=PeakVoltage/Pt;
	Voltage=Voltage/ADCFifo.U_rati0;
	MaxVoltage=(int16_t)Voltage;
  return MaxVoltage;	
}
#endif




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
	
	sprintf(Filename,"VA.txt\0");//over voltage
	strcpy(SystemAlarm->filenameV,Filename);
	sprintf(Filename,"GA.txt\0");//ground
	strcpy(SystemAlarm->filenameG,Filename);
	sprintf(Filename,"SA.txt\0");//short
	strcpy(SystemAlarm->filenameS,Filename);
	sprintf(Filename,"RA.txt\0");// resonance
	strcpy(SystemAlarm->filenameR,Filename);
	sprintf(Filename,"TA.txt\0");			//PT
	strcpy(SystemAlarm->filenameT,Filename);
	
	
	
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
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHA=60;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHB=60;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageHC=60;//120;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTA=2500;//30000;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTB=2500;//30000;
				Cupboard_P[m][k].Value.ProtectorData.OverVoltageTC=2500;//30000;
				Cupboard_P[m][k].Value.HumdityCupboard=60;//120;
				Cupboard_P[m][k].Value.TempCupboard=2500;//30000;
				Cupboard_P[m][k].Value.TempBusA=2500;//30000;
				Cupboard_P[m][k].Value.TempBusB=2500;//30000;
				Cupboard_P[m][k].Value.TempBusC=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchDownA=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchDownB=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchDownC=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchUpA=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchUpB=2500;//30000;
				Cupboard_P[m][k].Value.TempTouchUpC=2500;//30000;
				Cupboard_P[m][k].TimeStamp.year=18;
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
	
	
	
	
	
	
	//=========Load system setting
#if	 0
	  Setting.TempSet.MinValue=g_tParam.TempMin;
		Setting.TempSet.MaxValue=g_tParam.TempMax;
		Setting.HumiditySet.MinValue=g_tParam.HumdityMin;
		Setting.HumiditySet.MaxValue=g_tParam.HumdityMax;
		Setting.LeakageCurrentSet.LeakageValue=g_tParam.LeakageCurrentSetMax;
		
		Setting.NetSet.IPadrr[0]=g_tParam.sip_ip[0];
		Setting.NetSet.IPadrr[1]=g_tParam.sip_ip[1];
		Setting.NetSet.IPadrr[2]=g_tParam.sip_ip[2];
		Setting.NetSet.IPadrr[3]=g_tParam.sip_ip[3];
		Setting.NetSet.Port=g_tParam.NetPort;
		
		Setting.RS485Set.adr=g_tParam.Addr485;
		Setting.RS485Set.Baud=g_tParam.Baud485;
		Setting.RatioStruct.RatioCT=g_tParam.RatioCT;
		Setting.RatioStruct.RatioPT=g_tParam.RatioPT;
		Setting.VoltageSet.Modulus=g_tParam.Modulus;
		Setting.VoltageSet.MaxRatedValue=g_tParam.MaxRatedValue;
		Setting.DriveTestFlag=g_tParam.DriveTestFlag;
		Setting.DistanceFlag=g_tParam.DistanceFlag;
		
		Setting.BackLightTime=g_tParam.BackLightTime;
		
#endif		
//=====load Calibration data
#if 0
		for(i=0;i<8;i++)
	{
		ADCFifo.fftratio[i]=g_tParam.fftratio[i];
		//ADCFifo.rati0[i]=g_tParam.rati0[i];
	}
	
	#endif
		Setting.flag=1;
	
	
	
	//Setting.flag=1;


    const double pi=acos(-1.0);
	
	
    
 
	ReadSdAlarmInfo();
	
	
}
//=========================================================

void  ReadSDAlarmVALenght(void)
{
	FIL fp;
	uint32_t len;
	FRESULT result;
	//fp=fopen("VA.txt","r");
	result = f_open(&fp, "VA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		
		AlarmVaCount=0;
		return;
	}
	
	len=f_size(&fp);
	 
	AlarmVaCount=len/AlarmVALength;
	if(AlarmVaCount<=0)
	{
		f_close(&fp);
		AlarmVaCount=0;//??????
		return;
	}		
	f_close(&fp);
}

void  ReadSDAlarmVAData(int32_t n)
{

	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	FRESULT result;


	
	
	
	
	n=SDExceedValue_index-n;
	if(n<=0)
	n+=SDExceedValue_sum;	
	
  sprintf(buf,"VA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	res=f_read(&fp,(u8 *)&SDAlarmData->VoltageAlarm,len,&br);
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);
	


}


void  ReadSDAlarmGALenght(void)
{
	
	FIL fp;
	uint32_t len;
	FRESULT result;
	
	result = f_open(&fp, "GA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		AlarmGaCount=0;
		return;
	}
	
	len=f_size(&fp);
	 
	AlarmGaCount=len/AlarmGALength;
	if(AlarmGaCount<=0)
	{
		f_close(&fp);
		AlarmGaCount=0;//??????
		return;
	}		
	f_close(&fp);
	
	
}

void  ReadSDAlarmGAData(int32_t n)
{
	
	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	FRESULT result;
	


	n=SDGroundFault_index-n;
	if(n<=0)
	n+=SDGroundFault_sum;	
	
  sprintf(buf,"GA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	//res=f_read(&fp,(u8 *)&SDAlarmData->GroundAlarm,len,&br);
	res=f_read(&fp,(u8 *)SDSuddentAlarmData,len,&br);
	
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);
	
	
}

void  ReadSDAlarmSALenght(void)
{
	
	FIL fp;
	uint32_t len;
	FRESULT result;
	
	result = f_open(&fp, "SA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		AlarmSaCount=0;
		return;
	}
	
	len=f_size(&fp);
	 
	AlarmSaCount=len/AlarmSALength;
	if(AlarmSaCount<=0)
	{
		f_close(&fp);
		AlarmSaCount=0;//??????
		return;
	}		
	f_close(&fp);
	
}

void  ReadSDAlarmSAData(int32_t n)
{
	
	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	

	FRESULT result;

	n=SDShortFault_index-n;
	if(n<=0)
	n+=SDShortFault_sum;	
	
  sprintf(buf,"SA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	res=f_read(&fp,(u8 *)&SDAlarmData->ShortAlarm,len,&br);
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);
	
	
}
#if 0
//===================================================================
void  ReadSDAlarmRALenght(void)
{
	
	FIL fp;
	uint32_t len;
	
	result = f_open(&fp, "RA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		AlarmRaCount=0;
		return;
	}
	
	len=f_size(&fp);
	 
	AlarmRaCount=len/AlarmRALength;
	if(AlarmRaCount<=0)
	{
		f_close(&fp);
		AlarmRaCount=0;//??????
		return;
	}		
	f_close(&fp);

	
}

void  ReadSDAlarmRAData(int32_t n)
{
	
	
	FIL fp;
	FRESULT res;
	UINT br;// bw;

	
	FRESULT result;
	
	
	result = f_open(&fp, "RA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
	f_lseek(&fp,(AlarmRaCount-n-1)*AlarmRALength);
	res=f_read(&fp,(u8 *)SDAlarm_SDRAM,AlarmRALength,&br);
	
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);
	
}
#endif
//================================================================

//===================================================================
void  ReadSDAlarmTALenght(void)
{
	
	FIL fp;
	uint32_t len;
	FRESULT result;
	
	result = f_open(&fp, "TA.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		AlarmTaCount=0;
		return;
	}
	
	len=f_size(&fp);
	 
	AlarmTaCount=len/AlarmTALength;
	if(AlarmTaCount<=0)
	{
		f_close(&fp);
		AlarmTaCount=0;//??????
		return;
	}		
	f_close(&fp);
	
}

void  ReadSDAlarmTAData(int32_t n)
{

	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	
	FRESULT result;


	n=SDPTFault_index-n;
	if(n<=0)
	n+=SDPTFault_sum;	
	
  sprintf(buf,"TA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	res=f_read(&fp,(u8 *)&SDAlarmData->PTAlarm,len,&br);
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);

	
}



void  ReadSDAlarmUVAData(int32_t n)
{

	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	

	FRESULT result;

	n=SDUnderValFault_index-n;
	if(n<=0)
	n+=SDUnderValFault_sum;	
	
  sprintf(buf,"UVA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	res=f_read(&fp,(u8 *)&SDAlarmData->PTAlarm,len,&br);
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);

	
}




void  ReadSDAlarmSVAData(int32_t n)
{

	FIL fp;
	FRESULT res;
	
	UINT br;// bw;	
	char buf[10];
  u32 len;
	uint32_t m;
	uint8_t k;
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	
	u8 P_year;
	u8 P_month;
	u8 P_day;
	u8 P_hour;
	u8 P_minute;
	u8 P_second;
	
	
	RFStruct ProtectorAction_Data;
	FRESULT result;
	


	n=SDInstantaneousVoltageFault_index-n;
	if(n<=0)
	n=+SDInstantaneousVoltageFault_sum;	
	
  sprintf(buf,"SVA%d.txt",n);	
	
	result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
//	len=sizeof(struct _VoltageAlarm);
	len=f_size(&fp);
	
	//f_lseek(&fp,(AlarmVaCount-n-1)*AlarmVALength);
	res=f_read(&fp,(u8 *)SDSuddentAlarmData,len,&br);
	
	//res=f_read(&fp,(u8 *)SDAlarm_SDRAM,len,&br);		
	if(res||br<=0)		//?????
	{
		//读写错误
		f_close(&fp);
		//=======add debug printf information
		return;
	}
	
	f_close(&fp);
	
	
	year=SDSuddentAlarmData->Tim.year;
	day=SDSuddentAlarmData->Tim.day;
	month=SDSuddentAlarmData->Tim.month;
	hour=SDSuddentAlarmData->Tim.hour;
	minute=SDSuddentAlarmData->Tim.minute;
	second=SDSuddentAlarmData->Tim.second;
	
	
	//=============================检查保护器
	
	for(m=0;m<MaxActionSave;m++)
	{
		memset(&ActionProtectionSave[m].Protector.ProtectorAction,0,sizeof(struct _ProtectorAction));  //清除
	}
	
	
	
	result = f_open(&fp, "PSudden.txt", FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		//printf("open VA fault \r\n")
		f_close(&fp);
		return;
	}
	len=f_size(&fp);
	for(m=0;m<(len/sizeof(struct _ProtectorAction));m++)
	{
		f_lseek(&fp,len-sizeof(struct _ProtectorAction)*m);
		res=f_read(&fp,(u8 *)&(ProtectorAction_Data.Protector.ProtectorAction),sizeof(struct _ProtectorAction),&br);
		
		P_year=ProtectorAction_Data.Protector.ProtectorAction.Tim.year;
		P_month=ProtectorAction_Data.Protector.ProtectorAction.Tim.month;
		P_day=ProtectorAction_Data.Protector.ProtectorAction.Tim.day;
		P_hour=ProtectorAction_Data.Protector.ProtectorAction.Tim.hour;
		P_minute=ProtectorAction_Data.Protector.ProtectorAction.Tim.minute;
		P_second=ProtectorAction_Data.Protector.ProtectorAction.Tim.second;
		if(P_year==year&&P_month==month&&P_day==day&&P_hour==hour)
		{
			if((P_minute*60+P_second)-(minute*60+second)>5)
				break;
			
			memcpy(&ActionProtectionSave[k].Protector.ProtectorAction,&ProtectorAction_Data.Protector.ProtectorAction,sizeof(struct _ProtectorAction));
			k++;
			if(k>=MaxActionSave)
			k=0;	
			
		}
		else
		{
			//f_close(&fp);
			break;
		}
		
		
	}
	
	f_close(&fp);
//===============================
	
}




/*===================================================================
function:读取SD卡中故障个数，以及最后一个故障的index
input:无
out:无
by:seleen hu 2018 9 19
=====================================================================*/

void ReadSdAlarmInfo(void)
{
	uint8_t i;
	FIL fp;
	FRESULT res;
	FILINFO FileInfo;
	
	FATFS fs;
	FRESULT result;
	char buf[10];
	
	uint8_t year,month,day,hour,minute,second;
	uint8_t yearSave=0,monthSave=0,daySave=0,hourSave=0,minuteSave=0,secondSave=0;
	
	
	SDExceedValue_index=0;
	SDExceedValue_sum=0;										 
	SDGroundFault_index=0;
	SDGroundFault_sum=0;												 
	SDShortFault_index=0;
	SDShortFault_sum=0;											 
	SDInstantaneousVoltageFault_index=0;
	SDInstantaneousVoltageFault_sum=0;
	SDInstantaneousXFault_index=0;
	SDInstantaneousXFault_sum=0;
	
	SDPTFault_index=0;
	SDPTFault_sum=0;									
	
	result = f_mount(&fs, "0:/", 1);
	//===============读VAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		
		
		sprintf(buf,"VA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDExceedValue_index++;
		}
		else
		{
			if(second>=secondSave)
			SDExceedValue_index++;	
		}
			
		
		yearSave=year;
		monthSave=month;
		daySave=day;
		hourSave=hour;
		minuteSave=minute;
		secondSave=second;
		
		SDExceedValue_sum++;
		f_close(&fp);	
	}
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	
	
	//result = f_mount(&fs, "0:/", 1);
	//===============读GAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"GA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDGroundFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDGroundFault_index++;	
		}
		yearSave=year;
		monthSave=month;
		daySave=day;
		hourSave=hour;
		minuteSave=minute;
		secondSave=second;
		
		SDGroundFault_sum++;
		f_close(&fp);	
	}
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	
	//result = f_mount(&fs, "0:/", 1);
	//===============读SAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"SA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDShortFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDShortFault_index++;	
		}
		yearSave=year;
		monthSave=month;
		daySave=day;
		hourSave=hour;
		minuteSave=minute;
		secondSave=second;
		
		SDShortFault_sum++;	
		f_close(&fp);
	}
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	//result = f_mount(&fs, "0:/", 1);
	//===============读TAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"TA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDPTFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDPTFault_index++;	
		}
		
		SDPTFault_sum++;	
		f_close(&fp);
	}
	
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	//result = f_mount(&fs, "0:/", 1);
	//===============读UVAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"UVA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDUnderValFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDUnderValFault_index++;	
		}
		
		SDUnderValFault_sum++;	
		f_close(&fp);
	}
	
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	
	//result = f_mount(&fs, "0:/", 1);
	//===============读SVAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"SVA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDInstantaneousVoltageFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDInstantaneousVoltageFault_index++;	
		}
		yearSave=year;
		monthSave=month;
		daySave=day;
		hourSave=hour;
		minuteSave=minute;
		secondSave=second;
		
		SDInstantaneousVoltageFault_sum++;	
		f_close(&fp);
	}
	
	
	yearSave=0;
	monthSave=0;
	daySave=0;
	hourSave=0;
	minuteSave=0;
	secondSave=0;
	
	//result = f_mount(&fs, "0:/", 1);
	//===============读XAx.txt
	for(i=0;i<SDindexSize;i++)
	{
		sprintf(buf,"XA%d.txt",i+1);
		result = f_open(&fp, buf, FA_OPEN_EXISTING | FA_READ);
	
		if (result !=  FR_OK)
		{
			f_close(&fp);
			break;
		}
		
//==============获取文件时间		
		res=f_stat(buf,&FileInfo);
		if(res !=FR_OK)
		{
		f_close(&fp);
		break;	
		}
		year=((FileInfo.fdate>>9)&0x7f)-20;
		month=(FileInfo.fdate>>5)&0x0f;
		day=FileInfo.fdate&0x1f;
		hour=(FileInfo.ftime>>11)&0x1f;
		minute=(FileInfo.ftime>>5)&0x3f;
		second=(FileInfo.ftime&0x1f)*2;
		if(year>yearSave||month>monthSave||day>daySave||hour>hourSave||minute>minuteSave)//||second>secondSave)
		{
			SDInstantaneousXFault_index++;
		}
		else
		{
			if(second>=secondSave)
			SDInstantaneousXFault_index++;	
		}
		yearSave=year;
		monthSave=month;
		daySave=day;
		hourSave=hour;
		minuteSave=minute;
		secondSave=second;
		
		SDInstantaneousXFault_sum++;	
		f_close(&fp);
	}
	
	
}




//================================================================


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

