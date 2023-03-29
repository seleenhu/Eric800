


#include "bsp.h"

#include "stm32_dsp.h" 
#include "table_fft.h" 

#include <math.h>

#define DefinitionFilterV    ((float)0.15)
#define DefinitionFilterI    ((float)0.01)

#define PI2 6.28318530717959

#define radical2_half   0.707106781186548  //(2^0.5)/2
//#define Ratio_U			0.00959114
//#define Ratio_I			0.00030605

#define PI     3.141592653589793

#define Phase78			78*/180
#define Phase82			82*PI/180

#define Phase108		108*PI/180
#define Phase112		112*PI/180


#define cos120       (-0.5)
#define sin120       (0.866)

#define cos240       (-0.5)
#define sin240       (-0.866)
// N=1024,Fs/N=12.5Hz,Max(Valid)=6400Hz 
// 1024点FFt，采样率12800Hz,频率分辨率12.5Hz，测量最大有效频率6400Hz 

#define NPT 256 
//#define Fs  12800


extern  uint16_t  TableFFT[]; 
long lBUFIN[NPT];         /* Complex input vector */ 
long lBUFOUT[NPT];        /* Complex output vector */ 
long lBUFMAG[NPT];        /* Magnitude vector */ 


uint8_t SiliconControl_flag;
ADCParameterStruct ADCParameter;
uint8_t GndPhase2HourFlag;
uint32_t GndPhase2HourCount;


//long dsp_asm_waypowerMag(long zdata);
//float Elec_Display(long zdata);
//void PowerDisplay(float *PowerValue,long zdataU,long zdataI);
//void Elec_sequence_Display(float *buf,long zdataA,long zdataB,long zdataC);
//float Elec_unbalance_Display(float valueA,float valueB,float valueC);

//static void RMS_ELECDisplay(void);
static void PowerFactor(float *buf,long zdataU,long zdataI);
//static float Elec_AngleAB_Display(long zdataA,long zdataB);	
static void U0I0calculate(float *U0,float *I0,long Ua,long Ub,long Uc,long Ia,long Ib,long Ic);
static long VectorSum_Display(long zdataA,long zdataB,long zdataC);

static u8 ADC_Decompression(void);
static u8 ADC_JudgeGnd(void);
static u8 ADC_JudgeExceedV(void);
static u8 ADC_JudgeFastExceedV(float *RMSbuf);
static u8 ADC_PTFault(void);
static u8 ADC_LowValueFault(void);
static u8 ADC_PTFastFault(float *Rmsbuf);
static u8 ADC_ShortCircuit(void);
static u8 ADC_JudgeSysExceedV(void);

static u8 ADC_UNormal(void);	

static u8 FaskSysExceedVHalut(float UA,float UB,float UC);
static u8 FaskGndHalut(float UA,float UB, float UC,float U0);
static u8 CheckRightPhase(float UA,float UB, float UC,float U0);
//static u16 FaskGnd3U0bt(u16 bt3U0);
//static void DisplayNumCalculate(void);
#if 0
void dsp_asm_powerMag(void);
void  dsp_asm_test(void);
void  dsp_asm_init(void); 
/*
----------------------------------------------------
Name		dsp_asm_init
Function    模拟采样数据,采样数据中包含3种频率正弦波：
			50Hz，2500Hz，2550Hz
			lBUFIN数组中，每个单元数据高字(高16位)中
			存储采样数据的实部，低字(低16位)
			存储采样数据的虚部(总是为0) 
Edit by		fanzhong
Input		
Output		
Date		2017-09-24
----------------------------------------------------
*/

void  dsp_asm_init(void) 
{ 
	u16 i=0; 
	float fx; 
	
	for(i=0;i<NPT;i++){
		
		fx  = 4000 * sin(PI2*i*50.0/Fs) + 4000 * sin(PI2*i*2500.0/Fs) + 4000*sin(PI2*i*2550.0/Fs); 
		lBUFIN[i] = ((s16)fx)<<16; 
	} 
	
}//dsp_asm_init 

/*
----------------------------------------------------
Name		dsp_asm_test
Function    进行FFT变换，并计算各次谐波幅值
Edit by		fanzhong
Input		
Output		
Date		2017-09-24
----------------------------------------------------
*/
void  dsp_asm_test(void) 
{ 
	GPIO_SetBits(GPIOC,GPIO_Pin_13);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	cr4_fft_1024_stm32(lBUFOUT, lBUFIN, NPT);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);

	// 计算幅值 
	dsp_asm_powerMag();
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	cr4_fft_256_stm32(lBUFOUT, lBUFIN, 256);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	cr4_fft_64_stm32(lBUFOUT, lBUFIN, 64);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);



}//dsp_asm_test 
/*
----------------------------------------------------
Name		dsp_asm_powerMag
Function    计算各次谐波幅值
			先将lBUFOUT分解成实部(X)和虚部(Y)，然后计算赋值(sqrt(X*X+Y*Y) 
Edit by		fanzhong
Input		
Output		
Date		2017-09-24
----------------------------------------------------
*/
void dsp_asm_powerMag(void) 
{ 
	s16 lX,lY; 
	u32 i; 
	for(i=0;i<NPT/2;i++) 
	{ 
		lX  = (lBUFOUT[i] << 16) >> 16; 
		lY  = (lBUFOUT[i] >> 16); 
		{ 
			float X    = NPT * ((float)lX) /32768; 
			float Y    = NPT * ((float)lY) /32768; 
			float Mag = sqrt(X*X + Y*Y)/NPT; 
			lBUFMAG[i]    = (u32)(Mag * 65536); 
		} 
	} 
}//dsp_asm_powerMag 

#endif



/*
----------------------------------------------------
Name		dsp_asm_waypowerMag
Function    计算指定c谐波幅值
Edit by		fanzhong
Input		指定谐波的FFT变换值
Output		幅值
Date		2017-09-24
----------------------------------------------------
*/
static long dsp_asm_waypowerMag(long zdata)
{

	s16 lX,lY; 
	long MagValue;
	
	lX  = (zdata << 16) >> 16; 
	lY  = (zdata >> 16); 
	 
	float X    = NPT * ((float)lX) /32768; 
	float Y    = NPT * ((float)lY) /32768; 
	float Mag = sqrt(X*X + Y*Y)/NPT; 
	MagValue    = (u32)(Mag * 65536); 
	 
	return  MagValue;
}//dsp_asm_waypowerMag
#if 0
/*
----------------------------------------------------
Name		Elec_AngleAB_Display
Function    计算夹角
Edit by		fanzhong
Input		指定谐波的FFT变换值
Output		相位角
Date		2018-08-25
----------------------------------------------------
*/
static float Elec_AngleAB_Display(long zdataA,long zdataB)	
{
	s16 lXA,lYA;
	s16 lXB,lYB;	
	
	float MagA,MagB;
	float PhaseCos;
	float PhaseSin;
	float Phase;
	
	lXA  = (zdataA << 16) >> 16; 
	lYA  = (zdataA >> 16);
	MagA = sqrt(lXA*lXA + lYA * lYA);
	
	lXB  = (zdataB << 16) >> 16; 
	lYB  = (zdataB >> 16);
	MagB = sqrt(lXB*lXB + lYB * lYB);
	
	PhaseSin = (lYA * lXB - lXA * lYB);
	PhaseCos = (lXA * lXB + lYA * lYB);
	
	
	Phase = PhaseCos /(MagA * MagB);
	
	Phase = acos(Phase);
	
	if(PhaseSin < 0){
		Phase = -Phase;
	}
	
	return Phase;	
}//Elec_AngleAB_Display
#endif

#if 0
/*
----------------------------------------------------
Name		Elec_sequence_Display
Function    计算指定谐零序分量，正序分量，负序分量
Edit by		fanzhong
Input		指定谐波的FFT变换值
Output		buf[0] 零序分量
			buf[1] 正序分量
			buf[2] 负序分量
Date		2017-10-11
----------------------------------------------------
*/

static void Elec_sequence_Display(float *buf,long zdataA,long zdataB,long zdataC)
{
	

	s16 lXA,lYA; 
	s16 lXB,lYB; 
	s16 lXC,lYC;

	float XA,YA;
	float XB,YB,XB1,YB1;
	float XC,YC,XC1,YC1;
	
	
	lXA  = (zdataA << 16) >> 16; 
	lYA  = (zdataA >> 16); 

	XA    = NPT * ((float)lXA) /32768; 
	YA    = NPT * ((float)lYA) /32768; 

	lXB  = (zdataA << 16) >> 16; 
	lYB  = (zdataA >> 16); 

	XB    = NPT * ((float)lXB) /32768; 
	YB    = NPT * ((float)lYB) /32768; 

	lXC  = (zdataA << 16) >> 16; 
	lYC  = (zdataA >> 16); 

	XC    = NPT * ((float)lXC) /32768; 
	YC    = NPT * ((float)lYC) /32768; 

	buf[0] = sqrt((XA+XB+XC)*(XA+XB+XC) +  (YA+YB+YC)*(YA+YB+YC))/NPT;
	
	XB1 = XB * (float)cos240 - YB *(float)sin240;
	YB1 = XB * (float)sin240 + YB *(float)cos240;

	XC1 = XC * (float)cos120 - YC *(float)sin120;
	YC1 = XC * (float)sin120 + YC *(float)cos120;

	buf[1] = sqrt((XA+XB1+XC1)*(XA+XB1+XC1) +  (YA+YB1+YC1)*(YA+YB1+YC1))/NPT;

	XB1 = XB * (float)cos120 - YB *(float)sin120;
	YB1 = XB * (float)sin120 + YB *(float)cos120;

	XC1 = XC * (float)cos240 - YC *(float)sin240;
	YC1 = XC * (float)sin240 + YC *(float)cos240;

	buf[2] = sqrt((XA+XB1+XC1)*(XA+XB1+XC1) +  (YA+YB1+YC1)*(YA+YB1+YC1))/NPT;

	

}//Elec_sequence_Display

/*
----------------------------------------------------
Name		Elec_unbalance_Display
Function    计算不平衡度
Edit by		fanzhong
Input		基波的有效值
Output		不平衡度
Date		2017-10-11
----------------------------------------------------
*/
static float Elec_unbalance_Display(float valueA,float valueB,float valueC)
{
	float valueMAX,valueMIN,value;

	valueMAX = valueA;
	valueMIN = valueA;

	if(valueMAX<valueB){
		valueMAX = valueB;
	}else if(valueMIN > valueB){
		valueMIN = valueB;
	}

	if(valueMAX<valueC){
		valueMAX = valueC;
	}else if(valueMIN > valueC){
		valueMIN = valueC;
	}

	value = (valueA+valueB+valueC)/3;

	value = (valueMAX-valueMIN)/value;

	return value;	
	
}//Elec_unbalance_Display
#endif
/*
----------------------------------------------------
Name		Elec_U_Display
Function    计算的有效值
Edit by		fanzhong
Input		指定谐波的FFT变换值
Output		有效值
Date		2017-09-24
----------------------------------------------------
*/
static float Elec_Display(long zdata)
{
	float RMSData;
	RMSData = radical2_half*dsp_asm_waypowerMag(zdata);	
	return RMSData;
}
#if 0
/*
----------------------------------------------------
Name		PowerDisplay
Function    功率计算
Edit by		fanzhong
Input		
			有功功率,无功功率,视在功率,功率因数
			指定谐波电压的FFT变换值
			指定谐波电流的FFT变换值
Output		No
Date		2017-09-24
----------------------------------------------------
*/
static void PowerDisplay(float *PowerValue,long zdataU,long zdataI)
{
	s16 lXU,lYU,lXI,lYI; 	
	float Urms,Irms;	

	float pcos,psin;

	float MagU,MagI;
	
	
	lXU  = (zdataU << 16) >> 16; 
	lYU  = (zdataU >> 16); 
	MagU = sqrt(lXU*lXU + lYU*lYU);
	

	lXI  = (zdataI << 16) >> 16; 
	lYI  = (zdataI >> 16); 
	MagI = sqrt(lXI*lXI + lYI*lYI);

	

	pcos = (lXU * lXI + lYU * lYI)/(MagU * MagI);
	psin = (lYU * lXI - lXU * lYI)/(MagU * MagI);
	

	Urms = Elec_Display(zdataU);
	Irms = Elec_Display(zdataI);

	
	


	PowerValue[3] = pcos;

	PowerValue[2] = Urms * Irms * Ratio_P;

	PowerValue[1] = Urms * Irms * Ratio_P * psin;

	PowerValue[0] = Urms * Irms * Ratio_P * pcos;
	
	
}//PowerDisplay
#endif

/*
----------------------------------------------------
Name		PowerFactor
Function    功率因数
Edit by		fanzhong
Input	功率因数	
			指定谐波电压的FFT变换值
			指定谐波电流的FFT变换值
Output		No
Date		2018-01-20
----------------------------------------------------
*/
static void PowerFactor(float *buf,long zdataU,long zdataI)
{
	s16 lXU,lYU,lXI,lYI; 
	
	//float pcos,psin;
	float MagU,MagI;
	
	
	lXU  = (zdataU << 16) >> 16; 
	lYU  = (zdataU >> 16); 
	MagU = sqrt(lXU*lXU + lYU*lYU);
	

	lXI  = (zdataI << 16) >> 16; 
	lYI  = (zdataI >> 16); 
	MagI = sqrt(lXI*lXI + lYI*lYI);

	

	buf[0] = (lXU * lXI + lYU * lYI)/(MagU * MagI); //cos
	buf[1] = (lYU * lXI - lXU * lYI)/(MagU * MagI); //sin
	
	
}//PowerFactor

/*
----------------------------------------------------
Name		VectorSum_Display
Function    矢量和
Edit by		fanzhong
Input		No
Output		No
Date		2019-04-17
---------------------------------------------------- 
*/	
static long VectorSum_Display(long zdataA,long zdataB,long zdataC)
{
	s16 lXA,lYA,lXB,lYB,lXC,lYC;
	s16 lXSUM,lYSUM;
	long rt;

	lXA  = (zdataA << 16) >> 16; 
	lYA  = (zdataA >> 16); 

	lXB  = (zdataB << 16) >> 16; 
	lYB  = (zdataB >> 16); 

	lXC  = (zdataC << 16) >> 16; 
	lYC  = (zdataC >> 16); 

	lXSUM = lXA + lXB + lXC;
	lYSUM = lYA + lYB + lYC;

	rt = (uint16_t)(lYSUM);
	rt <<= 16;
	rt += (uint16_t)(lXSUM);

	return rt;
	
}//VectorSum_Display

/*
----------------------------------------------------
Name		RMS_ELECDisplay
Function    有效值  稳态电气参数值 定时性的去计算一次这些稳态数据
Edit by		fanzhong
Input		No
Output		No
Date		2018-01-20
---------------------------------------------------- 
*/
static void RMS_ELECDisplay(void)
{
	u8 flag;
	long i,m;
	float sbuf[2];
	float U0,I0;
	//u16 bt,j;
	
	for(i=0;i<3;i++){
		
		SystemPowerQuality->ElecU[i] = ADCFifo->RMS_buf[i];
		
		for(m=0;m<30;m++){
			SystemPowerQuality->ElecThdu[i][m] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][m]) / ADCFifo->fftratio[i]);
		}
	}
	SystemPowerQuality->Elec3U0 = ADCFifo->RMS_buf[3];
	
	
	
	
	for(i=0;i<3;i++){		
	
		SystemPowerQuality->ElecI[i] = ADCFifo->RMS_buf[i+4];
	
		
		for(m=0;m<30;m++){
			SystemPowerQuality->ElecThdi[i][m] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i+4][m]) / ADCFifo->fftratio[i+4]);
		}
	}
	SystemPowerQuality->Elec3I0 = ADCFifo->RMS_buf[7];
	
	U0I0calculate(&U0,&I0,
								ADCWholeVar->FFTOutBuf[0][1],ADCWholeVar->FFTOutBuf[1][1],ADCWholeVar->FFTOutBuf[2][1],
								ADCWholeVar->FFTOutBuf[4][1],ADCWholeVar->FFTOutBuf[5][1],ADCWholeVar->FFTOutBuf[6][1]);	
	
	
	

	//SystemPowerQuality->Phase[7] = Elec_PhaseAngle_Display(ADCWholeVar->FFTOutBuf[7][4]);
	
	
	for(i=0;i<3;i++){
		flag = 0;
		if(fabs(SystemPowerQuality->ElecU[i]) < DefinitionFilterV){
			SystemPowerQuality->ElecU[i] = 0;
			flag |= 0x01;
		}
		if(fabs(SystemPowerQuality->ElecI[i]) < DefinitionFilterI){
			SystemPowerQuality->ElecI[i] = 0;
			flag |= 0x02;
		}
		if(flag){
			SystemPowerQuality->ElecPowerFactor[i] = 1;
		}else{
			PowerFactor(sbuf,ADCWholeVar->FFTOutBuf[i][1],ADCWholeVar->FFTOutBuf[i+4][1]);
			SystemPowerQuality->ElecPowerFactor[i] = fabs(sbuf[0]);	
		}
		SystemPowerQuality->ElecApparentPower[i] = fabs(SystemPowerQuality->ElecU[i] * SystemPowerQuality->ElecI[i]);
		SystemPowerQuality->ElecActivePower[i] = fabs(SystemPowerQuality->ElecApparentPower[i] * sbuf[0]);
		SystemPowerQuality->ElecReactivePower[i] = fabs(SystemPowerQuality->ElecApparentPower[i] * sbuf[1]);
		
	}
	
	
	
	
	SystemPowerQuality->ElecApparentPower[3] = 0;
	for(i=0;i<3;i++){
		SystemPowerQuality->ElecApparentPower[3] += SystemPowerQuality->ElecApparentPower[i];
	}
	
	SystemPowerQuality->ElecActivePower[3] = 0;
	for(i=0;i<3;i++){
		SystemPowerQuality->ElecActivePower[3] += SystemPowerQuality->ElecActivePower[i];
	}
	
	SystemPowerQuality->ElecReactivePower[3] = 0;
	for(i=0;i<3;i++){
		SystemPowerQuality->ElecReactivePower[3] += SystemPowerQuality->ElecReactivePower[i];
	}
							
}//RMS_ELECDisplay

#if 0
/*
----------------------------------------------------
Name		PowerQuality_Task
Function    电能质量任务
Edit by		fanzhong
Input			No
Output		No
Date		2018-01-20
---------------------------------------------------- 
*/
void PowerQuality_Task(void)
{
	uint16_t j;
	uint16_t bt_back;
	
	bt_back = FIFO_Backpointer(ADCFifo.pt,4*256,ADCFifo.FifoSize);	
	
	for(j=0;j<1024;j++){		
		ADCWholeVar->FFTBuf[0][j] = ADCFifo.UAFIFO[bt_back];
		ADCWholeVar->FFTBuf[1][j] = ADCFifo.UBFIFO[bt_back];
		ADCWholeVar->FFTBuf[2][j] = ADCFifo.UCFIFO[bt_back];
		ADCWholeVar->FFTBuf[3][j] = ADCFifo.U0FIFO[bt_back];		
		ADCWholeVar->FFTBuf[4][j] = ADCFifo.IAFIFO[bt_back];
		ADCWholeVar->FFTBuf[5][j] = ADCFifo.IBFIFO[bt_back];
		ADCWholeVar->FFTBuf[6][j] = ADCFifo.ICFIFO[bt_back];
		ADCWholeVar->FFTBuf[7][j] = ADCFifo.I0FIFO[bt_back];		
		bt_back = FIFO_pointer(bt_back,1,ADCFifo.FifoSize);		
	}
	
	RMS_ELECDisplay();
	
}//PowerQuality_Task
#endif

/*
----------------------------------------------------
Name		FFT_VerifyDisplay
Function    校验值
Edit by		fanzhong
Input		No
Output		No
Date		2018-04-19
---------------------------------------------------- 
*/
void FFT_VerifyDisplay(void)
{
	
	u16 i,j,RMS_i;
	float RMS_buf[8];
	u16 bt_back;
	
	for(i=0;i<8;i++){
		RMS_buf[i] = 0;
	}
	bt_back = FIFO_Backpointer(ADCFifo->pt,4*256,ADCFifo->FifoSize);
	for(RMS_i=0; RMS_i<4; RMS_i++){		
		for(j=0;j<256;j++){
			for(i=0;i<8;i++){
				ADCWholeVar->VerifyBuf[i][j] = ADCFifo->ADCFIFO[i][bt_back];
			}				
			bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
		}
		
		
		for(i=0;i<8;i++){
			for(j=0;j<NPT; j++){
				lBUFIN[j] = (ADCWholeVar->VerifyBuf[i][j])<<16;
			}
			cr4_fft_256_stm32(lBUFOUT, lBUFIN, NPT);	
			RMS_buf[i] += Elec_Display(lBUFOUT[1]);
		}
	}
	
	for(i=0;i<8;i++){
		RMS_buf[i] /= 4;
	}
	
	for(i=0;i<4;i++){
		ADCFifo->fftratio[i] = RMS_buf[i]/100;
	}
	for(i=4;i<8;i++){
		ADCFifo->fftratio[i] = RMS_buf[i]/5;
	}
	
}//FFT_VerifyDisplay
#if 0
/*
----------------------------------------------------
Name		FundamentalResonanceDisplay
Function    基波谐振判断
Edit by		fanzhong
Input		 电压有效值
Output		1 谐振产生 0没有谐振
Date		2018-05-11
---------------------------------------------------- 
*/
u8 FundamentalResonanceDisplay(float Ua,float Ub,float Uc)
{
	u8 PhaseEx,rt;
	
	float PhaseVoltage,LhaseVoltage;
	PhaseVoltage = (SystemSet.VoltageSet.MaxRatedValue) / (SystemSet.RatioStruct.RatioPT);
	LhaseVoltage = (SystemSet.VoltageSet.MaxRatedValue) / ((SystemSet.RatioStruct.RatioPT)*radical3);	
	
	PhaseEx = 0;
	if(Ua > PhaseVoltage){
		PhaseEx |= 0x01;
	}
	if(Ub > PhaseVoltage){
		PhaseEx |= 0x02;
	}
	if(Uc > PhaseVoltage){
		PhaseEx |= 0x04;
	}
	rt = 0;
	switch(PhaseEx){
		case 0x01:
			if(((Ub < LhaseVoltage/10)&&(Ub > 0))&&
				((Uc < LhaseVoltage/10)&&(Uc > 0))){
					rt = 1;
			}
			break;
		case 0x02:
			if(((Ua < LhaseVoltage/10)&&(Ua > 0))&&
				((Uc < LhaseVoltage/10)&&(Uc > 0))){
					rt = 1;
			}
			break;
		case 0x04:
			if(((Ua < LhaseVoltage/10)&&(Ua > 0))&&
				((Ub < LhaseVoltage/10)&&(Ub > 0))){
					rt = 1;
			}
			break;
		case 0x03:
			if((Uc < LhaseVoltage/10)&&(Uc > 0)){
					rt = 1;
			}
			break;
		case 0x05:
			if((Ub < LhaseVoltage/10)&&(Ub > 0)){
					rt = 1;
			}
			break;
		case 0x06:
			if((Ua < LhaseVoltage/10)&&(Ua > 0)){
					rt = 1;
			}
			break;
	}
	return rt;
	
}//FundamentalResonanceDisplay

#endif

/*
----------------------------------------------------
Name			U0I0calculate
Function  计算3U0,3I0
Edit by		fanzhong
Input	
Output		No
Date		2018-05-21
----------------------------------------------------
*/
static void U0I0calculate(float *U0,float *I0,long Ua,long Ub,long Uc,long Ia,long Ib,long Ic)
{
	s16 lX,lY;
	s16 lXAdd,lYAdd;
	u32 data32;
	
	
	lXAdd = 0;
	lYAdd = 0;
	lX  = (Ua << 16) >> 16; 
	lY  = (Ua >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	lX  = (Ub << 16) >> 16; 
	lY  = (Ub >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	lX  = (Uc << 16) >> 16; 
	lY  = (Uc >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	data32 = (u32)lXAdd;
	data32 <<= 16;
	data32 += (u32)lYAdd;
	
	*U0 = Elec_Display(data32);
	
	
	
	lXAdd = 0;
	lYAdd = 0;
	lX  = (Ia << 16) >> 16; 
	lY  = (Ia >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	lX  = (Ib << 16) >> 16; 
	lY  = (Ib >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	lX  = (Ic << 16) >> 16; 
	lY  = (Ic >> 16); 
	
	lXAdd += lX;
	lYAdd += lY;
	
	data32 = (u32)lXAdd;
	data32 <<= 16;
	data32 += (u32)lYAdd;
	
	*I0 = Elec_Display(data32);
	
}//U0I0calculate

/*
----------------------------------------------------
Name		Calculate_Task
Function    计算任务
Edit by		fanzhong
Input		 No
Output	 No
Date		2018-07-25
---------------------------------------------------- 
*/
void Calculate_Task(void)
{
	//static u8 PowerQualityCount;
	//PowerQualityCount++;
	//if(PowerQualityCount>=100){
		//PowerQualityCount = 0;
		RMS_ELECDisplay();
		//DisplayNumCalculate();
		FrequencyCal();
	//}
	
}
#if 0
/*
----------------------------------------------------
Name		DisplayNumCalculate
Function    精确计算显示值
Edit by		fanzhong
Input		 No
Output	 No
Date		2018-10-24
---------------------------------------------------- 
*/
static void DisplayNumCalculate(void)
{
	u16 i,j,bt;
	float RMSbuf[8],RMSdata;
	bt = FIFO_Backpointer(ADCFifo->pt,20 * 256,ADCFifo->FifoSize);
	for(i=0;i<8;i++){
		RMSbuf[i] = 0;
	}

	for(i=0;i<20*32;i++){
		for(j=0;j<8;j++){
			RMSdata = ((float)(ADCFifo->ADCFIFO[j][bt]))/ADCFifo->rati0[j];
			RMSbuf[j] += (RMSdata * RMSdata);
		}
		bt = FIFO_pointer(bt,8,ADCFifo->FifoSize);
	}
	for(i=0;i<8;i++){
		RMSbuf[i] = RMSbuf[i] / (float)(20*32);
		ADCFifo->Display_buf[i] = sqrt(RMSbuf[i]);
		if(i<4){
			if(ADCFifo->Display_buf[i]<(float)(0.1)){
				ADCFifo->Display_buf[i] = 0;
			}
		}else{
			if(ADCFifo->Display_buf[i]<(float)(0.05)){
				ADCFifo->Display_buf[i] = 0;
			}
		}
	}

}//DisplayNumCalculate
#endif
/*
*********************************************************************************************************
*	函 数 名: ADC_Decompression
*	功能说明: 判断失压
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_Decompression(void)
{
	u8 i,t,rt;
	
	t = 0;
	for(i=0;i<3;i++){
		if(ADCFifo->RMS_buf[i] >= CriteriaValue.DecompressionValue){
			t++;
		}
	}	
	if(0 == t){		
			Decompression_LED_ON();	
			//Ground_LED_OFF();
			
			
			SystemPowerQuality->Frequency[0] = 0;
			SystemPowerQuality->Frequency[1] = 0;
			SystemPowerQuality->Frequency[2] = 0;
			//ADCParameter.GndStep = 0;
			//ADCParameter.GndTims = 0;	
		
			//ADCParameter.ExceedStep = 0;
			//ADCParameter.PTStep = 0;
			//ADCParameter.ShortStep = 0;
			rt = 1;
			//ADCParameter.ADCFaskArcGndTimes = 0;
			//if(ADCParameter.ADCFaskFlag != FaskFaultArcGnd){
			//	ADCParameter.ADCFaskFlag = 0;					
			//}
			
			
	}else{
			Decompression_LED_OFF();
			
			rt = 0;
	}
	
	return rt;
}//ADC_JudgePT

#if 1
/*
*********************************************************************************************************
*	函 数 名: ADC_JudgeGnd
*	功能说明: 判断是否过压
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_JudgeGnd(void)
{
	u8 flag;
	u16 i,j;
	static u8 ScanTims,flagbak;
	static u16 bt_back;
	static AlarmTimStruct AlarmTim;
	const u8 JudgeTime = 2;
	u8 ret;
	static u8 CheckCnt=0;
	
	switch(ADCParameter.GndStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				return 0;
				
			}
			flag = FaskGndHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);
			if(flag){
				if(0 == ScanTims){
					ScanTims++;
					AlarmTim = SysGetTime();
					flagbak = flag;
					bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);					
				}else if(ScanTims < JudgeTime){
					ScanTims++;
					if(flagbak != flag){
						ScanTims = 0;
					}else{
						if(ScanTims >= JudgeTime){
							ScanTims = 0;							
							if(0 == FaultFifo_busy){
								FaultFifo[FaultFifo_pt]->DislayFlag = 0;
								Ground_LED_ON();
								//SiliconControl_ON();
								SiliconControl_flag = 1;
								#ifdef UseLocal_16Current
								M1TOM3BitTxFalg.Bits.S_E0 = 1;	
								#endif
								CanSampleFlag = 1;
								
								FaultFifo_busy = 1;
								ADCParameter.GndStep = 1;
								#if 1
								//=============CanFaultSel Clear
								CanFaultSelSave.CanId[0]=0;
								CanFaultSelSave.RMSValue[0]=0;
								CanFaultSelSave.CanId[1]=0;
								CanFaultSelSave.RMSValue[1]=0;
								CanFaultSelSave.CanId[2]=0;
								CanFaultSelSave.RMSValue[2]=0;
								#endif
								//============================
								FaultFifo[FaultFifo_pt]->Tim = AlarmTim;
								FaultFifo[FaultFifo_pt]->ValueFlag = flag;
								FaultFifo[FaultFifo_pt]->FaultStyle = GroundFault;
								s16 lXU,lYU; 
								float MagU;	
								for(i=0;i<8;i++){
									for(j=0;j<15;j++){
										FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
										lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
										lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
										MagU = sqrt(lXU*lXU + lYU*lYU);	
										if(0 == MagU){
											FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
											FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
										}else{
											FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
											FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
										}
									}
								}								
								
								switch(flag|0x80){			//原先只有弧光接地，现改为全部接地都合接地相
									case 0x81:
										SystemState.SwitchState.AphaseState = 0x01;
										SystemState.SwitchState.BphaseState = 0x00;
										SystemState.SwitchState.CphaseState = 0x00;
									  SystemState.SwitchState.DphaseState = 0x00;	
										break;
									case 0x82:
										SystemState.SwitchState.AphaseState = 0x00;
										SystemState.SwitchState.BphaseState = 0x01;
										SystemState.SwitchState.CphaseState = 0x00;
										SystemState.SwitchState.DphaseState = 0x00;
										break;
									case 0x84:
										SystemState.SwitchState.AphaseState = 0x00;
										SystemState.SwitchState.BphaseState = 0x00;
										SystemState.SwitchState.CphaseState = 0x01;
										SystemState.SwitchState.DphaseState = 0x00;
										break;
								}
							}
							
						}
					}
				}
			}
			break;
		
			
			
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				#if 0
				if(FaultFifo[FaultFifo_pt]->ValueFlag & 0x80){
					ADCParameter.GndStep = 2;
				}else{
					ADCParameter.GndStep = 3;
				}
				#else
				ADCParameter.GndStep=2;
				#endif

				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag |= 0x40;
				//===========
				
				//=============
				AllSelDisplay();
				FaultFifo_busy = 0;
				/*
				if(0xE0 == (FaultFifo[FaultFifo_pt]->DislayFlag & 0xE0)){
					FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
					FaultFifo_busy = 0;
					FaultFifo_pt++;
					if(FaultFifo_pt>= FaultFifoSIZE){
						FaultFifo_pt = 0;
					}
				}
				*/
				ADCParameter.GndTims = 0;
			}
			break;
		case 2:
			ADCParameter.GndTims++;
			
			//if(760 == ADCParameter.GndTims){
			if(ADCParameter.GndTims>=200){// 
				ADCParameter.GndStep=3;
				//SystemState.SwitchState.DphaseState = 0x01;
			}
			
			break;
			
		case	3:
			ret=CheckRightPhase(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);
		  if(ret==0){
					SystemState.SwitchState.DphaseState = 0x01;//合项正确
					ADCParameter.GndTims=0;
					ADCParameter.GndStep=5;
			}
			else{
				SystemState.SwitchState.AphaseState = 0x02;
				SystemState.SwitchState.BphaseState = 0x02;
				SystemState.SwitchState.CphaseState = 0x02;
				SystemState.SwitchState.DphaseState = 0x02;
				ADCParameter.GndStep = 4;
				ADCParameter.GndTims=0;
				CheckCnt++;
				if(CheckCnt>3){
					CheckCnt=0;
					ADCParameter.GndStep=9;	//多次出错
				}
				
			}
      break;
		case 4:
		
			
			flag = FaskGndHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);	//再次检测
		 if(flag){
				switch(flag|0x80){
					case 0x81:
						
						SystemState.SwitchState.AphaseState = 0x01;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x00;
						ADCParameter.GndTims=0;
						ADCParameter.GndStep=2;
						break;
					case 0x82:
						
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x01;
						SystemState.SwitchState.CphaseState = 0x00;
						ADCParameter.GndTims=0;
						ADCParameter.GndStep=2;
						break;
					case 0x84:
						
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x01;
						ADCParameter.GndTims=0;
						ADCParameter.GndStep=2;
						break;				
				}
			}
		 else{
			 ADCParameter.GndStep=9;
		 }
	 
			
			break;
			
		case 5:			
			
			//if(ADCParameter.GndTims >= 960){
			ADCParameter.GndTims++;
			if(ADCParameter.GndTims >= 1000){       //　5s后关断重新检测
				SystemState.SwitchState.AphaseState = 0x02;
				SystemState.SwitchState.BphaseState = 0x02;
				SystemState.SwitchState.CphaseState = 0x02;
				SystemState.SwitchState.DphaseState = 0x02;
				ADCParameter.GndTims=0;
				ADCParameter.GndStep = 6;
			}
				
				/*
				ADCParameter.GndStep = 3;
				flag = FaskGndHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);
				switch(flag){
					case 0x81:
						ADCParameter.GndStep = 4;
						SystemState.SwitchState.AphaseState = 0x01;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x00;
						SystemState.SwitchState.DphaseState = 0x01;
						break;
					case 0x82:
						ADCParameter.GndStep = 4;
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x01;
						SystemState.SwitchState.CphaseState = 0x00;
						SystemState.SwitchState.DphaseState = 0x01;
						break;
					case 0x84:
						ADCParameter.GndStep = 4;
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x01;
						SystemState.SwitchState.DphaseState = 0x01;
						break;				
				}			
				
			}*/
			break;
		case 6:
			ADCParameter.GndTims++;
		  if(ADCParameter.GndTims>=200){//1s
				ADCParameter.GndStep = 7;
				
			}	
			break;
		case 7:
			//if(0 == ADC_UNormal()){
			//	ADCParameter.GndStep = 0;					
			//}	

			flag = FaskGndHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);
		 if(flag){
				switch(flag|0x80){
					case 0x81:
						ADCParameter.GndStep = 8;
						SystemState.SwitchState.AphaseState = 0x01;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x00;
						SystemState.SwitchState.DphaseState = 0x01;
						GndPhase2HourFlag=1;
						GndPhase2HourCount=0;
						break;
					case 0x82:
						ADCParameter.GndStep = 8;
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x01;
						SystemState.SwitchState.CphaseState = 0x00;
						SystemState.SwitchState.DphaseState = 0x01;
						GndPhase2HourFlag=1;
					  GndPhase2HourCount=0;
						break;
					case 0x84:
						ADCParameter.GndStep = 8;
						SystemState.SwitchState.AphaseState = 0x00;
						SystemState.SwitchState.BphaseState = 0x00;
						SystemState.SwitchState.CphaseState = 0x01;
						SystemState.SwitchState.DphaseState = 0x01;
						GndPhase2HourFlag=1;
						GndPhase2HourCount=0;
						break;				
				}
			}
		 else{
			 ADCParameter.GndStep=9;
		 }
		 
			break;
		case 8:
			break;
		case 9:
			ADCParameter.GndStep=0;
			break;
		default:
			ADCParameter.GndStep = 0;
	}
	
	return ADCParameter.GndStep;
	

//	return 0;
}//ADC_JudgeGnd
#endif
/*
*********************************************************************************************************
*	函 数 名: ADC_JudgeExceedV
*	功能说明: 判断是否过压
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_JudgeExceedV(void)
{
	
	u16 i,j;
	u8 flag,flag1,flag2;
	static u8 flagBack,ScanTimes;
	static u16 bt_back;
	static AlarmTimStruct AlarmTim;
	const u8 JudgeTime = 4;
	//float RMSbuf[8];
	switch(ADCParameter.ExceedStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				//ADCParameter.ExceedFaultTimes = 0;
				return 0;
			}
			flag1 = FaskGndHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2], ADCFifo->RMS_buf[3]);
			flag2 = FaskSysExceedVHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2]);
			if(flag1 || flag2){
				ScanTimes = 0;
			}else{				
				flag = ADC_JudgeFastExceedV(ADCFifo->RMS_buf);
				if(0 == ScanTimes){
					if(flag){
						flagBack = flag;
						AlarmTim = SysGetTime();
						bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);
						ScanTimes++;
					}
				}else if(ScanTimes < JudgeTime){
					ScanTimes++;
					if(flagBack != flag){
						ScanTimes = 0;
					}else{
						if(ScanTimes >= JudgeTime){
							ScanTimes = 0;							
							if(0 == FaultFifo_busy){
								ADCParameter.ExceedStep = 1;
								FaultFifo_busy = 1;
																
								
								
								FaultFifo[FaultFifo_pt]->Tim = AlarmTim;
								FaultFifo[FaultFifo_pt]->ValueFlag = flag;
								FaultFifo[FaultFifo_pt]->FaultStyle = ExceedValueFault;
								s16 lXU,lYU; 
								float MagU;	
								for(i=0;i<8;i++){									
									for(j=0;j<15;j++){
										if(i<7){
											FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
										}else{
											FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[6]);
										}
										lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
										lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
										MagU = sqrt(lXU*lXU + lYU*lYU);	
										if(0 == MagU){
											FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
											FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
										}else{
											FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
											FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
										}
									}
									
								}
								
							}
							
						}
					}
				}
			}
			break;
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				ADCParameter.ExceedStep++;
				
				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}
				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				FaultFifo_busy = 0;
				FaultFifo_pt++;
				if(FaultFifo_pt>= FaultFifoSIZE){
					FaultFifo_pt = 0;
				}
				
			}
			break;
		case 2:
			//if(0 == ADC_UNormal()){
			//	ADCParameter.ExceedStep = 0;
			//	OverVoltage_LED_OFF();	
			//}
			break;	
		default:
			
			ADCParameter.ExceedStep = 0;
	}
	
	return ADCParameter.ExceedStep;

}//ADC_JudgeExceedV
/*
*********************************************************************************************************
*	函 数 名: ADC_JudgeFastExceedV
*	功能说明: 判断是否过压
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_JudgeFastExceedV(float *RMSbuf)
{
	u8 i,flag;
	flag = 0;
	for(i=0;i<3;i++){
		if(RMSbuf[i]>= ADCParameter.UMaxValue){
			flag |= (1<<i);
		}
	}
	return flag;
}//ADC_JudgeFastExceedV
/*
*********************************************************************************************************
*	函 数 名: ADC_JudgeSysExceedV
*	功能说明: 判断是否过压
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_JudgeSysExceedV(void)
{
	
	u16 i,j;
	u8 flag;
	//static u8 flagBack;
	static u16 bt_back;
	
	switch(ADCParameter.SysExceedStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				//ADCParameter.ExceedFaultTimes = 0;
				return 0;
			}			
			flag = FaskSysExceedVHalut(ADCFifo->RMS_buf[0], ADCFifo->RMS_buf[1], ADCFifo->RMS_buf[2]);
			if(flag){
				if(0 == FaultFifo_busy){
					ADCParameter.SysExceedStep = 1;
					FaultFifo_busy = 1;
					
					bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);
					FaultFifo[FaultFifo_pt]->Tim = SysGetTime();
					FaultFifo[FaultFifo_pt]->ValueFlag = flag;
					FaultFifo[FaultFifo_pt]->FaultStyle = SysExceedValueFault;
					s16 lXU,lYU; 
					float MagU;	
					for(i=0;i<8;i++){
						for(j=0;j<15;j++){
							FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
							lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
							lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
							MagU = sqrt(lXU*lXU + lYU*lYU);	
							if(0 == MagU){
								FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
								FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
							}else{
								FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
								FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
							}
						}
					}
					
				}
				
			}
			break;
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				ADCParameter.SysExceedStep++;
				
				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}
				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				FaultFifo_busy = 0;
				FaultFifo_pt++;
				if(FaultFifo_pt>= FaultFifoSIZE){
					FaultFifo_pt = 0;
				}
				
			}
			break;
		case 2:
			//if(0 == ADC_UNormal()){
			//	ADCParameter.SysExceedStep = 0;
					
			//}
			break;
				
		default:			
			ADCParameter.SysExceedStep = 0;
	}
	
	return ADCParameter.SysExceedStep;

}//ADC_JudgeSysExceedV




/*
*********************************************************************************************************
*	函 数 名: ADC_PTFault
*	功能说明: 判断PT故障
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_PTFault(void)
{
	u8 flag,flag1;	
	u16 i,j;
	//static u8 Step;
	static u16 bt_back;	
	static u8 ScanTims,flagbak;
	static AlarmTimStruct AlarmTim;
	const u8 JudgeTime = 4;
	
	switch(ADCParameter.PTStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				return 0;
			}
			flag1 = FaskGndHalut(ADCFifo->RMS_buf[0],ADCFifo->RMS_buf[1],ADCFifo->RMS_buf[2],ADCFifo->RMS_buf[3]);
			if(flag1){
				ScanTims = 0;
			}else{
				flag = ADC_PTFastFault(ADCFifo->RMS_buf);
				if(0 == ScanTims){
					if((0== (flag & 0x80))&&flag){
						ScanTims++;
						flagbak = flag;
						AlarmTim = SysGetTime();
						bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);
					}
				}else if(ScanTims < JudgeTime){
					if(flagbak == flag){
						ScanTims++;
					}else{
						ScanTims = 0;
					}
					if(ScanTims >= JudgeTime){
						
						ScanTims = 0;
						if(0 == FaultFifo_busy){
							ADCParameter.PTStep = 1;
							FaultFifo_busy = 1;
							

							
							FaultFifo[FaultFifo_pt]->Tim = AlarmTim;
							FaultFifo[FaultFifo_pt]->ValueFlag = flag;
							FaultFifo[FaultFifo_pt]->FaultStyle = PTFault;
							s16 lXU,lYU; 
							float MagU;	
							for(i=0;i<8;i++){
								for(j=0;j<15;j++){
									FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
									lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
									lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
									MagU = sqrt(lXU*lXU + lYU*lYU);	
									if(0 == MagU){
										FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
										FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
									}else{
										FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
										FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
									}
								}
							}
							
						}
						
					}
				}
			}			
			break;
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				ADCParameter.PTStep++;
				
				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}
				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				FaultFifo_busy = 0;
				FaultFifo_pt++;
				if(FaultFifo_pt>= FaultFifoSIZE){
					FaultFifo_pt = 0;
				}
				
			}
			break;
		case 2:
			//if(0 == ADC_UNormal()){
			//	Fault_LED_OFF();
			//	ADCParameter.PTStep = 0;
			//}
			break;
		default:
			ADCParameter.PTStep = 0;
	}
	return ADCParameter.PTStep;

}//ADC_PTFault
/*
*********************************************************************************************************
*	函 数 名: ADC_LowValueFault
*	功能说明: 判断PT故障
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_LowValueFault(void)
{
	u8 flag,flag1;	
	u16 i,j;
	//static u8 Step;
	static u16 bt_back;	
	static u8 ScanTims,flagbak;
	static AlarmTimStruct AlarmTim;
	const u8 JudgeTime = 4;
	
	switch(ADCParameter.LowStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				return 0;
			}
			flag1 = FaskGndHalut(ADCFifo->RMS_buf[0],ADCFifo->RMS_buf[1],ADCFifo->RMS_buf[2],ADCFifo->RMS_buf[3]);
			if(flag1){
				ScanTims = 0;
			}else{
				flag = ADC_PTFastFault(ADCFifo->RMS_buf);
				if(0 == ScanTims){
					if((flag & 0x80) && flag){
						ScanTims++;
						flagbak = flag;
						AlarmTim = SysGetTime();
						bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);
					}
				}else if(ScanTims < JudgeTime){
					if(flagbak == flag){
						ScanTims++;
					}else{
						ScanTims = 0;
					}
					if(ScanTims >= JudgeTime){
						
						ScanTims = 0;
						if(0 == FaultFifo_busy){
							ADCParameter.LowStep = 1;
							FaultFifo_busy = 1;
							

							
							FaultFifo[FaultFifo_pt]->Tim = AlarmTim;
							FaultFifo[FaultFifo_pt]->ValueFlag = flag;
							FaultFifo[FaultFifo_pt]->FaultStyle = PTFault;
							s16 lXU,lYU; 
							float MagU;	
							for(i=0;i<8;i++){
								for(j=0;j<15;j++){
									FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
									lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
									lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
									MagU = sqrt(lXU*lXU + lYU*lYU);	
									if(0 == MagU){
										FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
										FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
									}else{
										FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
										FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
									}
								}
							}
							
						}
						
					}
				}
			}			
			break;
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				ADCParameter.LowStep++;
				
				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}
				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				FaultFifo_busy = 0;
				FaultFifo_pt++;
				if(FaultFifo_pt>= FaultFifoSIZE){
					FaultFifo_pt = 0;
				}
				
			}
			break;
		case 2:
			//if(0 == ADC_UNormal()){
			//	Fault_LED_OFF();
			//	ADCParameter.PTStep = 0;
			//}
			break;
		default:
			ADCParameter.LowStep = 0;
	}
	return ADCParameter.LowStep;

}//ADC_LowValueFault
/*
*********************************************************************************************************
*	函 数 名: ADC_PTFastFault
*	功能说明: 判断PT故障
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_PTFastFault(float *Rmsbuf)
{
	u8 flag,i,flag2;
	
	if(0x03 != (ADCParameter.flag & 0x03)){
		return 0;
	}

	flag = 0;
	flag2 = 0;
	for(i=0;i<3;i++){
		if(Rmsbuf[i] < 30){			
			flag |= (1<<i);
		}else if(Rmsbuf[i] < ADCParameter.UnderVoltage){
			flag2 = 1;
			flag |= (1<<i);			
		}
	}	
	switch(flag){
		case 1:
			if(((Rmsbuf[1] > ADCParameter.NormalUMin) && (Rmsbuf[1] < ADCParameter.NormalUMax))&&
				 ((Rmsbuf[2] > ADCParameter.NormalUMin) && (Rmsbuf[2] < ADCParameter.NormalUMax))
				){					
					flag = 0x01;
			}
			break;
		case 2:
			if(((Rmsbuf[0] > ADCParameter.NormalUMin) && (Rmsbuf[0] < ADCParameter.NormalUMax))&&
				 ((Rmsbuf[2] > ADCParameter.NormalUMin) && (Rmsbuf[2] < ADCParameter.NormalUMax))
				){
					flag = 0x02;
			}
			break;
		case 4:
			if(((Rmsbuf[0] > ADCParameter.NormalUMin) && (Rmsbuf[0] < ADCParameter.NormalUMax))&&
				 ((Rmsbuf[1] > ADCParameter.NormalUMin) && (Rmsbuf[1] < ADCParameter.NormalUMax))
				){
					flag = 0x04;
			}
			break;
		case 3:
			if(((Rmsbuf[2] > ADCParameter.NormalUMin) && (Rmsbuf[2] < ADCParameter.NormalUMax))){
					flag = 0x03;
			}
			break;
		case 5:
			if(((Rmsbuf[1] > ADCParameter.NormalUMin) && (Rmsbuf[1] < ADCParameter.NormalUMax))){
					flag = 0x05;
			}
			break;
		case 6:
			if(((Rmsbuf[0] > ADCParameter.NormalUMin) && (Rmsbuf[0] < ADCParameter.NormalUMax))){
					flag = 0x06;
			}
			break;
		case 7:
			flag = 0x07;
		default:;
			flag = 0;
	}
	if((flag2) && (flag)){
		flag |= 0x80;
	}
	return flag;
}//ADC_PTFastFault
/*
*********************************************************************************************************
*	函 数 名: ADC_ShortCircuit
*	功能说明: 判断短路
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static u8 ADC_ShortCircuit(void)
{	
	u16 i,j;
	//static u8 Step;
	static u16 bt_back;
	static u8 ScanTims;
	const u8 JudgeTime = 4;

	switch(ADCParameter.ShortStep){
		case 0:
			if(0x03 != (ADCParameter.flag & 0x03)){
				return 0;
			}			
			if(ADCFifo->RMS_buf[7] > ADCParameter.MaxI0){

				if(0 == ScanTims){
					bt_back = FIFO_Backpointer(ADCFifo->bt,64,ADCFifo->FifoSize);
					FaultFifo[FaultFifo_pt]->Tim = SysGetTime();
					ScanTims++;
				}else if(ScanTims<JudgeTime){
					ScanTims++;
				}else{				
					if(0 == FaultFifo_busy){
						ADCParameter.ShortStep++;
						FaultFifo_busy = 1;
											
						FaultFifo[FaultFifo_pt]->FaultStyle = ShortFault;
						s16 lXU,lYU; 
						float MagU;	
						for(i=0;i<8;i++){
							for(j=0;j<15;j++){
								FaultFifo[FaultFifo_pt]->RMSValue[i][j] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][j]) / ADCFifo->fftratio[i]);
								lXU  = (ADCWholeVar->FFTOutBuf[i][j] << 16) >> 16; 
								lYU  = (ADCWholeVar->FFTOutBuf[i][j] >> 16); 
								MagU = sqrt(lXU*lXU + lYU*lYU);	
								if(0 == MagU){
									FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = 0;
									FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = 0;
								}else{
									FaultFifo[FaultFifo_pt]->VabuleSin[i][j] = ((float)lXU/MagU);
									FaultFifo[FaultFifo_pt]->VabuleCos[i][j] = ((float)lYU/MagU);
								}
							}
						}
						
					}
				}
			}else{
				ScanTims = 0;
			}
			break;
		case 1:
			if(FIFO_length(bt_back,ADCFifo->pt,ADCFifo->FifoSize)>=10*256){
				ADCParameter.ShortStep++;
				
				for(i=0;i<7;i++){
					FaultFifo[FaultFifo_pt]->ratio[i] = ADCFifo->rati0[i];
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
				}
				
				bt_back = FIFO_Backpointer(bt_back, 10*256, ADCFifo->FifoSize);
				for(j=0;j<20*256;j++){
					for(i=0;i<7;i++){						
						FaultFifo[FaultFifo_pt]->Wave[i][j] = (ADCFifo->ADCFIFO)[i][bt_back];						
					}
					bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
				}
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				FaultFifo_busy = 0;
				FaultFifo_pt++;
				if(FaultFifo_pt>= FaultFifoSIZE){
					FaultFifo_pt = 0;
				}
				
			}
			break;
		case 2:
			if((ADCFifo->RMS_buf[7] < ADCParameter.MaxI0) && (0 == ADC_UNormal())){
				ADCParameter.ShortStep = 0;			
			}			
			break;
		default:
			ADCParameter.ShortStep = 0;
	}
	return ADCParameter.ShortStep;
	
}//ADC_ShortCircuit


/*
*********************************************************************************************************
*	函 数 名: ADCParameterSet
*	功能说明: 有效值计算
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADCParameterSet(void)
{

	//u8 i;
	float fdata1;	
	
	fdata1 = (PoramData.VoltageSet.MaxRatedValue) / ((PoramData.RatioStruct.RatioPT)*radical3);	
	//ADCParameter.PTValue = fdata1/3;
	
	//ADCParameter.GndVSet.MinValue = fdata1*4/5;
	//ADCParameter.GndVSet.MaxValue = fdata1*6/5;	
	//ADCParameter.GndVSet.MaxU0 = fdata1/2;
	if(6000 == PoramData.VoltageSet.MaxRatedValue){		
		ADCParameter.SysOverVal = (float)(16000)/((PoramData.RatioStruct.RatioPT)*radical3);
	}else if(10000 == PoramData.VoltageSet.MaxRatedValue){
		ADCParameter.SysOverVal = (float)(26000)/((PoramData.RatioStruct.RatioPT)*radical3);
	}else{
		ADCParameter.SysOverVal = (float)(78000)/((PoramData.RatioStruct.RatioPT)*radical3);
	}
	ADCParameter.SysOverVal  = 115; 
	
	//for(i=0;i<3;i++){
	//	ADCParameter.InstantUADC[i] = (int16_t)(85 * radical2 * ADCFifo->rati0[i]);
	//}
	//ADCParameter.InstantUADC[3] = (int16_t)(10 * radical2 * ADCFifo->rati0[3]);

	
	ADCParameter.NormalUMax =(float) (fdata1 * (float)( 1.1));
	ADCParameter.NormalUMin = (float)(fdata1 * (float)( 0.9));
	ADCParameter.NormalU0Max = (float)(fdata1 * (float)( 0.1));
	ADCParameter.UnderVoltage = (float)(fdata1 * (float)( 0.9));
	ADCParameter.UMaxValue = fdata1 * PoramData.VoltageSet.Modulus;

	ADCParameter.MaxI0 = PoramData.ShortGNDSet.I0/PoramData.RatioStruct.RatioCT;
	
}//ADCParameterSet

/*
*********************************************************************************************************
*	函 数 名: ADC_UNormal
*	功能说明: 判断电压正常
*	形    参:  无
*	返 回 值: 0 : ok >= 1 err
*********************************************************************************************************
*/
static u8 ADC_UNormal(void)
{
	u8 i,rt;
	rt = 0;
	for(i=0; i<3; i++){
		if((ADCFifo->RMS_buf[i] < ADCParameter.NormalUMin)||
		   (ADCFifo->RMS_buf[i] > ADCParameter.NormalUMax)){
			 rt = 1;
			 break;
		}
	}
	if(i == 3){
		if(ADCFifo->RMS_buf[3] > ADCParameter.NormalU0Max){
			rt = 1;
		}
	}
	return rt;
}//ADC_UNormal

/*
*********************************************************************************************************
*	函 数 名:  FaskCalTask
*	功能说明:  中断
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void FaskCalTask(void)
{


	uint16_t bt;
	uint16_t i,j;
	while(FIFO_length(ADCFifo->bt,ADCFifo->pt,ADCFifo->FifoSize) >= 256){
		bt = ADCFifo->bt;
		ADCFifo->bt = FIFO_pointer(ADCFifo->bt,64,ADCFifo->FifoSize);
		for(j=0;j<64;j++){
			for(i=0;i<7;i++){				
				ADCWholeVar->FFTBuf[i][j] = ADCFifo->ADCFIFO[i][bt];				
			}
			bt = FIFO_pointer(bt,4,ADCFifo->FifoSize);
		}
		for(i=0;i<7;i++){
			for(j=0;j<64;j++){			
				lBUFIN[j] = (ADCWholeVar->FFTBuf[i][j])<<16;
			}
			//cr4_fft_256_stm32(lBUFOUT, lBUFIN, 256);
			cr4_fft_64_stm32(lBUFOUT, lBUFIN, 64);
			for(j=0;j<64;j++){	
				ADCWholeVar->FFTOutBuf[i][j] = lBUFOUT[j];			
			}
		}
		for(i=0;i<64;i++){
			ADCWholeVar->FFTOutBuf[7][i] = VectorSum_Display(ADCWholeVar->FFTOutBuf[4][i],ADCWholeVar->FFTOutBuf[5][i],ADCWholeVar->FFTOutBuf[6][i]);
		}
		for(i=0;i<7;i++){		
			ADCFifo->RMS_buf[i] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][1]) / ADCFifo->fftratio[i]);		
			if(i<4){
				if(ADCFifo->RMS_buf[i] < DefinitionFilterV){
					ADCFifo->RMS_buf[i] = 0;
				}
			}else{
				if(ADCFifo->RMS_buf[i] < DefinitionFilterI){
					ADCFifo->RMS_buf[i] = 0;
				}
			}
		}
		ADCFifo->RMS_buf[7] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[7][1]) / ADCFifo->fftratio[6]);
		
			
		ADCParameterSet();
		ADCParameter.flag &= ~(1<<JudgeDecompressionBit);
		if(0 == ADC_Decompression()){
			ADCParameter.flag |= (1<<JudgeDecompressionBit);
		}else{
			if(2 == ADCParameter.SysExceedStep){
				ADCParameter.SysExceedStep = 0;
			}
			if(2 == ADCParameter.ExceedStep){
				ADCParameter.ExceedStep = 0;
				
			}
			if(2 == ADCParameter.PTStep){
				ADCParameter.PTStep = 0;				
			}
			if(2 == ADCParameter.LowStep){
				ADCParameter.LowStep = 0;
			}
			if(8 == ADCParameter.GndStep){
				ADCParameter.GndStep = 0;
				Ground_LED_OFF();
			}
		}
		if(0 == ADC_UNormal()){
			if(2 == ADCParameter.SysExceedStep){
				ADCParameter.SysExceedStep = 0;
			}
			if(2 == ADCParameter.ExceedStep){
				ADCParameter.ExceedStep = 0;
				
			}
			if(2 == ADCParameter.PTStep){
				ADCParameter.PTStep = 0;
				
			}
			if(2 == ADCParameter.LowStep){
				ADCParameter.LowStep = 0;
			}
			if(8 == ADCParameter.GndStep){
				ADCParameter.GndStep = 0;
				Ground_LED_OFF();
				SystemState.SwitchState.AphaseState = 0x02;
				SystemState.SwitchState.BphaseState = 0x02;
				SystemState.SwitchState.CphaseState = 0x02;
				SystemState.SwitchState.DphaseState = 0x02;
				GndPhase2HourFlag=0;
				
			}			
			
		
			//ADCParameter.ADCFaskArcGndTimes = 0;
			//if(ADCParameter.ADCFaskFlag != FaskFaultArcGnd){
			//	ADCParameter.ADCFaskFlag = 0;					
			//}
		}
		//TestPinXor();
		ADC_JudgeGnd();
		ADC_JudgeExceedV();
		ADC_PTFault();
		ADC_LowValueFault();
		ADC_ShortCircuit();
		ADC_JudgeSysExceedV();
		//TestPinXor();
	}
}//FaskCalTask


static u8 CheckRightPhase(float UA,float UB, float UC,float U0)
{
	u8 rt=0;
	u8 cnt=0;
	if(UA<30)
		cnt++;
	if(UB<30)
		cnt++;
	if(UC<30)
		cnt++;
	if(cnt>=2)
		rt=1;
	return rt; 
}

/*
*********************************************************************************************************
*	函 数 名:  FaskGndHalut
*	功能说明:  中断
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
static u8 FaskGndHalut(float UA,float UB, float UC,float U0)
{
	u8 rt = 0;
	rt = 0;
	if(U0 > CriteriaValue.Gnd.Ul){
		if((UA > CriteriaValue.Gnd.MaxValue) && (UB > CriteriaValue.Gnd.MaxValue)){
			rt = 0x04;
			if(UC > CriteriaValue.Gnd.MinValue){
				rt |= 0x80;
			}
		}else if((UA > CriteriaValue.Gnd.MaxValue) && (UC > CriteriaValue.Gnd.MaxValue)){
			rt = 0x02;
			if(UB > CriteriaValue.Gnd.MinValue){
				rt |= 0x80;
			}
		}else if((UB > CriteriaValue.Gnd.MaxValue) && (UC > CriteriaValue.Gnd.MaxValue)){
			rt = 0x01;
			if(UA > CriteriaValue.Gnd.MinValue){
				rt |= 0x80;
			}
		}
	}
	return rt;	
}//FaskGndHalut

/*
*********************************************************************************************************
*	函 数 名:  FaskExceedVHalut
*	功能说明:  中断
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
static u8 FaskSysExceedVHalut(float UA,float UB,float UC)
{
	u8 rt;
	rt = 0;
	if(UA >  ADCParameter.SysOverVal){
		rt |= 0x01;
	}
	if(UB > ADCParameter.SysOverVal){
		rt |= 0x02;
	}
	if(UC > ADCParameter.SysOverVal){
		rt |= 0x04;
	}
	return rt;
}//FaskExceedVHalut

/*
*********************************************************************************************************
*	函 数 名:  FaskGnd3U0bt
*	功能说明:  中断
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
#if 0
static u16 FaskGnd3U0bt(u16 bt3U0)
{
	u16 i,bt,bt1;
	int16_t ADC3U0;
	
	bt = bt3U0;		
	ADC3U0 = fabs((int16_t)(CriteriaValue.Gnd.Ul * ADCFifo->rati0[3]*radical2));
	for(i=0;i<3*256;i++){
		if(fabs(ADCFifo->ADCFIFO[3][bt]) >= ADC3U0){
			break;
		}else{
			bt = FIFO_Backpointer(bt, 1, ADCFifo->FifoSize);
		}
	}
	ADC3U0 = abs(ADCFifo->ADCFIFO[3][bt]);
	bt1 = bt;
	for(i=0;i<64;i++){
		if(ADC3U0 < abs(ADCFifo->ADCFIFO[3][bt])){
			ADC3U0 = abs(ADCFifo->ADCFIFO[3][bt]);
			bt1 = bt;
		}
	}
	bt = bt1;
	return bt;
}//FaskGnd3U0bt
#endif
/*-----------------------------end--------------------------------------------------*/


