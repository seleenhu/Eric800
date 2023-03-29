


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



ADCParameterStruct ADCParameter;
bubblestrcut bubblebuf[8][ibubblebufSIZE];


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

//static u8 ADC_Decompression(void);
//static u8 ADC_JudgeGnd(void);
//static u8 ADC_JudgeExceedV(void);
//static u8 ADC_JudgeFastExceedV(float *RMSbuf);
//static u8 ADC_PTFault(void);
//static u8 ADC_PTFastFault(float *Rmsbuf);
//static u8 ADC_ShortCircuit(void);
//static u8 ADC_JudgeSysExceedV(void);

//static u8 ADC_UNormal(void);	

//static u8 FaskSysExceedVHalut(float UA,float UB,float UC);
//static u8 FaskGndHalut(float UA,float UB, float UC,float U0);
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
#if 0
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
#endif
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
	float RMS_buf[16];
	u16 bt_back;
	
	for(i=0;i<8;i++){
		RMS_buf[i] = 0;
	}
	bt_back = FIFO_Backpointer(ADCFifo->pt,8*128,ADCFifo->FifoSize);
	for(RMS_i=0; RMS_i<4; RMS_i++){		
		for(j=0;j<256;j++){
			for(i=0;i<16;i++){
				ADCWholeVar->VerifyBuf[i][j] = ADCFifo->ADCFIFO[i][bt_back];
			}				
			bt_back = FIFO_pointer(bt_back,1,ADCFifo->FifoSize);
		}
		
		
		for(i=0;i<16;i++){
			for(j=0;j<NPT; j++){
				lBUFIN[j] = (ADCWholeVar->VerifyBuf[i][j])<<16;
			}
			cr4_fft_256_stm32(lBUFOUT, lBUFIN, NPT);	
			RMS_buf[i] += Elec_Display(lBUFOUT[2]);
		}
	}
	
	for(i=0;i<16;i++){
		RMS_buf[i] /= 4;
	}
	
	for(i=0;i<16;i++){	
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
#if 0
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
		//FrequencyCal();
	//}
	
}
#endif
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
	while(FIFO_length(ADCFifo->bt,ADCFifo->pt,ADCFifo->FifoSize) >= 128){
		bt = ADCFifo->bt;
		ADCFifo->bt = FIFO_pointer(ADCFifo->bt,32,ADCFifo->FifoSize);
		for(j=0;j<64;j++){
			for(i=0;i<16;i++){				
				ADCWholeVar->FFTBuf[i][j] = ADCFifo->ADCFIFO[i][bt];				
			}
			bt = FIFO_pointer(bt,2,ADCFifo->FifoSize);
		}
		for(i=0;i<16;i++){
			for(j=0;j<64;j++){			
				lBUFIN[j] = (ADCWholeVar->FFTBuf[i][j])<<16;
			}
			//cr4_fft_256_stm32(lBUFOUT, lBUFIN, 256);
			cr4_fft_64_stm32(lBUFOUT, lBUFIN, 64);
			for(j=0;j<64;j++){	
				ADCWholeVar->FFTOutBuf[i][j] = lBUFOUT[j];			
			}
		}
		for(i=0;i<16;i++){		
			ADCFifo->RMS_buf[i] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][1]) / ADCFifo->fftratio[i]);
			if(ADCFifo->RMS_buf[i] < DefinitionFilterI){
				ADCFifo->RMS_buf[i] = 0;
			}
		}
		
	}


}//FaskCalTask

/*
*********************************************************************************************************
*	函 数 名:  SelRouteCalTask
*	功能说明:  中断
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void SelRouteCalTask(void)
{
#if 1
	uint8_t i,j,channel_i;
	uint16_t bt; 
	float RMS_buf[8][16],max_RMS;
	uint8_t channel_buf[8];
	uint8_t channel_Num[16],max_Num;

	static uint8_t Step;
	switch(Step){
		case 0:
			if(ADCFifo->CalFlag){
				ADCFifo->CalFlag = 0;
				Step++;
			}
			break;
		case 1:
			if(FIFO_length(ADCFifo->Calbt, ADCFifo->pt, ADCFifo->FifoSize) >= 32){				
				for(channel_i = 0;channel_i <8;channel_i++){
					bt = FIFO_Backpointer(ADCFifo->Calbt,96-channel_i*4,ADCFifo->FifoSize);				
					for(j=0;j<64;j++){
						for(i=0;i<16;i++){				
							ADCWholeVar->FFTBuf[i][j] = ADCFifo->ADCFIFO[i][bt];				
						}
						bt = FIFO_pointer(bt,2,ADCFifo->FifoSize);
					}
					for(i=0;i<16;i++){
						for(j=0;j<64;j++){			
							lBUFIN[j] = (ADCWholeVar->FFTBuf[i][j])<<16;
						}
						//cr4_fft_256_stm32(lBUFOUT, lBUFIN, 256);
						cr4_fft_64_stm32(lBUFOUT, lBUFIN, 64);
						for(j=0;j<64;j++){	
							ADCWholeVar->FFTOutBuf[i][j] = lBUFOUT[j];			
						}
					}
					for(i=0;i<16;i++){		
						RMS_buf[channel_i][i] = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][1]) / ADCFifo->fftratio[i]);
						if(RMS_buf[channel_i][i] < DefinitionFilterI){
							RMS_buf[channel_i][i] = 0;
						}
					}
				}
				for(i=0;i<8;i++){
					max_RMS = RMS_buf[i][0];
					channel_buf[i] = 0;
					for(channel_i=1;channel_i<16;channel_i++){		
						if(max_RMS < RMS_buf[i][channel_i]){
							max_RMS = RMS_buf[i][channel_i];
							channel_buf[i] = channel_i;
						}
					}		
				}
				for(i=0;i<16;i++){
					channel_Num[i] = 0;
				}
				
				for(i=0;i<8;i++){
					channel_Num[channel_buf[i]] += 1;
				}
				max_Num = channel_Num[0];
				channel_i = 0;
				for(i=1;i<16;i++){
					if(max_Num < channel_Num[i]){
						max_Num = channel_Num[i];
						channel_i = i;
					}
				}
				ComData.SelNum = channel_i;				
				ModuleUartTXTask(0x03);
				max_RMS = RMS_buf[0][channel_i];
				max_Num = 0;
				for(i=0;i<8;i++){
					if(max_RMS < RMS_buf[i][channel_i]){
						max_RMS = RMS_buf[i][channel_i];
						max_Num = i;
					}
					
				}
				FaultFifo[FaultFifo_pt]->FaultFlag = channel_i;
				for(i=0;i<16;i++){
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
					FaultFifo[FaultFifo_pt]->RMSValue[i] = RMS_buf[max_Num][i];
				}
				Step++;
			}
			break;
		case 2:
	 		if(FIFO_length(ADCFifo->Calbt, ADCFifo->pt, ADCFifo->FifoSize) >= 9*128){
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				bt = FIFO_Backpointer(ADCFifo->Calbt,11*128,ADCFifo->FifoSize);
				SaveWave(bt, 20*128);
				Step = 0;
	 		}
			break;
		default:
			Step = 0;
	
		
	}
#else
	uint8_t i,j,channel_i;
	uint16_t bt; 
	float RMS_buf[8][16],max_RMS;
	uint8_t channel_buf[8];
	uint8_t channel_Num[16],max_Num;

	static uint8_t Step;
	switch(Step){
		case 0:
			if(ADCFifo->CalFlag){
				ADCFifo->CalFlag = 0;
				Step++;
			}
			break;
		case 1:
			if(FIFO_length(ADCFifo->Calbt, ADCFifo->pt, ADCFifo->FifoSize) >= 32){				
				for(channel_i = 0;channel_i <8;channel_i++){
					bt = FIFO_Backpointer(ADCFifo->Calbt,96-channel_i*4,ADCFifo->FifoSize);				
					for(j=0;j<64;j++){
						for(i=0;i<16;i++){				
							ADCWholeVar->FFTBuf[i][j] = ADCFifo->ADCFIFO[i][bt];				
						}
						bt = FIFO_pointer(bt,2,ADCFifo->FifoSize);
					}
					for(i=0;i<16;i++){
						for(j=0;j<64;j++){			
							lBUFIN[j] = (ADCWholeVar->FFTBuf[i][j])<<16;
						}
						//cr4_fft_256_stm32(lBUFOUT, lBUFIN, 256);
						cr4_fft_64_stm32(lBUFOUT, lBUFIN, 64);
						for(j=0;j<64;j++){	
							ADCWholeVar->FFTOutBuf[i][j] = lBUFOUT[j];			
						}
					}
					for(i=0;i<16;i++){
						bubblebuf[channel_i][i].flag = i;
						bubblebuf[channel_i][i].Avrflag = channel_i;
						bubblebuf[channel_i][i].fdata = fabs(Elec_Display(ADCWholeVar->FFTOutBuf[i][1]) / ADCFifo->fftratio[i]);
						if(bubblebuf[channel_i][i].fdata < DefinitionFilterI){
							bubblebuf[channel_i][i].fdata = 0;
						}
						RMS_buf[channel_i][i] = bubblebuf[channel_i][i].fdata;
					}
				}				
				for(i=0;i<8;i++){
					bubbleCal(bubblebuf[i], 16);
				}				
				for(i=0;i<16;i++){
					channel_Num[i] = 0;					
				}
				for(i=0;i<8;i++){					
					channel_Num[bubblebuf[i][0].flag]++;					
				}
				max_Num = 0;
				channel_i = 0;
				for(i=0;i<16;i++){
					if(max_Num<channel_Num[i]){
						max_Num = channel_Num[i];
						channel_i = i;
					}
				}
				
				FaultFifo[FaultFifo_pt]->FaultFlag = channel_i;				
				for(i=0;i<16;i++){
					FaultFifo[FaultFifo_pt]->fftratio[i] = ADCFifo->fftratio[i];
					FaultFifo[FaultFifo_pt]->RMSValue[i] = bubblebuf[i][0].fdata;
				}
				Step++;
			}
			break;
		case 2:
	 		if(FIFO_length(ADCFifo->Calbt, ADCFifo->pt, ADCFifo->FifoSize) >= 9*128){
				FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
				bt = FIFO_Backpointer(ADCFifo->Calbt,11*128,ADCFifo->FifoSize);
				SaveWave(bt, 20*128);
				Step = 0;
	 		}
			break;
		default:
			Step = 0;
	
		
	}
#endif
}//SelRouteCalTask

void bubbleCal(bubblestrcut *buf,uint8_t buf_len)
{
	uint8_t i,j;
	bubblestrcut temdata;
	for(i=0;i<buf_len-1;i++)
	{
		for(j=0;j<buf_len-i-1;j++)
		{
			if(buf[j].fdata < buf[j+1].fdata)
			{
				temdata = buf[j];
				buf[j] =  buf[j+1];
				buf[j+1] = temdata;
			}
		}
	}
}//bubbleCal




/*-----------------------------end--------------------------------------------------*/


