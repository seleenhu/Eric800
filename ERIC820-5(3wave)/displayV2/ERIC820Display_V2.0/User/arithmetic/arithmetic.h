#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

typedef struct{	
	float UMaxValue;	
	float MaxI0;
	
	float NormalUMax;
	float NormalUMin;
	float NormalU0Max;
	
	float SysOverVal;
	
	float UnderVoltage;
	
	//int16_t InstantUADC[4];
	//uint8_t InstantUADCFlag;
	
	u8 flag; /*bit 0: 失压  bit1 ：投运 bit2 : 接地 bit3:过压 bit4:PT断线 bit5:短路*/
	u16 GndTims;
	u8 GndStep;
	
	u8 ExceedStep;
	//u8 ExceedFaultTimes;

	u8 SysExceedStep;
	
	u8 PTStep;
	
	u8 ShortStep;
	
	//u8 ADCFaskFlag; 
	//u16 ADCFaskGndTime;
	//u8 ADCFaskArcGndTimes;
	//u8 ADCFaskCalTimes;
}ADCParameterStruct;

//ADCParameter.flag
#define JudgeDecompressionBit     0
#define JudgePutOPBit   					1
#define JudgeGndBit    						2
#define JudgeExceedVBit    				3
#define PTFaultBit    						4
#define ShortCircuitBit    				5

//ADCParameter.InstantUADCFlag
#define ADCCalculateBit						0
#define ADCOverVoltageBit					1
#define ADCGndBit									2
#define ADCExceedBit							3



extern ADCParameterStruct ADCParameter;
//ADCParameter.ADCFaskFlag
#define  FaskFaultIdle   0x00
#define  FaskFaultX      0x01
#define  FaskFaultGnd    0x02
#define  FaskFaultArcGnd 0x03
#define  FaskFaultV      0x04

//extern void  dsp_asm_init(void);
//extern void  dsp_asm_test(void);
//extern RMS_ELEC_VALUE SDarithicdata;
//extern void RMS_ELECDisplay(RMS_ELEC_VALUE *RMS_ELECbuf,unsigned short **buf);
//extern void PowerQuality_Task(void);
extern void FFT_VerifyDisplay(void);

extern void Calculate_Task(void);
extern void InitCalTim(void);
extern void CalTimISR(void);
extern void ADCParameterSet(void);
extern void FaskCalTask(void);
#endif
/*----------------------------end---------------------------*/

