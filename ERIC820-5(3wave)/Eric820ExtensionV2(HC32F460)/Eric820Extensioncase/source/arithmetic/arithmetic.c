


#include "mydefine.h"	


//#define PI     3.141592653589793

const float radical3 =  1.7320508;
const float radical2 =  1.4142135;	

uint8_t VerifyFlag;
uint16_t DebugBuf[256];
uint8_t StartCalRmsFlag;


/*
---------------------------------------------------------------------
Name		CalRMS
Function	计算RMS
Input		FIFO bt
OutPut		RMS
Edit by fanzhong
Date		2018-11-12
---------------------------------------------------------------------
*/	
void CalRMS(uint16_t bt)
{
	uint16_t j;
	//int16_t bt1;
	int16_t Indata;
	float buf;

	
	bt = FIFO_Backpointer(bt, ADC_Period*ADC_N/2, ADCbufSize/2);
	bt *=2;
	buf = 0;	
	for(j=0;j<32;j++){		
		//bt1 = FIFO_pointer(bt, ADC_N-1, ADCbufSize);
		//Indata = ADCFifo.AD_Value[bt] - ADCFifo.AD_Value[bt1];
		//Indata = ADCFifo.AD_Value[bt1] - ADCFifo.AD_Value[bt];
		Indata = ADCFifo.AD_Value[bt+1] - ADCFifo.AD_Value[bt];
		
		buf += (Indata*Indata);		
		bt = FIFO_pointer(bt, ADC_N*8, ADCbufSize);
		
	}
	
	buf /= 32;		
	
	buf = sqrt(buf);
	buf = buf / EepromData.ratio;
	ADCFifo.RMSbuf = buf;
	if(ADCFifo.RMSbuf < (float)(0.05)){
		ADCFifo.RMSbuf = 0;
	}
	
}//CalRMS


void CalVerify(uint16_t bt)
{

	uint16_t j;
	int16_t Indata;
	//uint16_t bt1;
	float Verifbuf;	
	
	bt = FIFO_Backpointer(bt, 10*ADC_Period*ADC_N/2, ADCbufSize/2);
	bt *=2;
	Verifbuf = 0;
	
	for(j=0;j<ADC_Period*10;j++){		
		//bt1 = FIFO_pointer(bt, ADC_N-1, ADCbufSize);		
		//Indata = ADCFifo.AD_Value[bt] - ADCFifo.AD_Value[bt1];
		Indata = ADCFifo.AD_Value[bt+1] - ADCFifo.AD_Value[bt];
		Verifbuf += (Indata*Indata);		
		bt = FIFO_pointer(bt, ADC_N, ADCbufSize);
	}	
	Verifbuf /= ADC_Period*10;		
	Verifbuf = sqrt(Verifbuf);
	
	EepromData.ratio = Verifbuf/5;	
	EepromSaveFlag = 1;
		

}//CalVerify

/*
---------------------------------------------------------------------
Name		SelCalRMS
Function	计算RMS
Input		FIFO bt
OutPut		RMS
Edit by fanzhong
Date		2018-11-12
---------------------------------------------------------------------
*/
void SelCalRMS(void)
{
	uint16_t i,j,len;
	#if 1
	static uint16_t bt;//bt1;
	static int16_t Indata;
	static float buf;
	#else
	uint16_t bt;//bt1;
	int16_t Indata;
	float buf;
	#endif
		

	static uint8_t step;

	switch(step){
		case 0:
		//	if(CanComData.FaultFlag){
				if(StartCalRmsFlag==1){
				//CanComData.Faultbt = FIFO_Backpointer(CanComData.Faultbt, ((ADC_Period+ADC_Period/4)*ADC_N)/2, ADCbufSize/2);
				CanComData.Faultbt = FIFO_Backpointer(CanComData.Faultbt, 128, ADCbufSize/2);
				//Test1Set();
				bt = CanComData.Faultbt*2;
				//=======计算128个点	
				memcpy(DebugBuf,&ADCFifo.AD_Value[bt],256*2);
				for(j=0;j<(ADC_Period/2);j++){
					//bt1 = FIFO_pointer(bt, ADC_N-1, ADCbufSize);		
					//Indata = ADCFifo.AD_Value[bt] - ADCFifo.AD_Value[bt1];
					
					Indata = ADCFifo.AD_Value[bt+1] - ADCFifo.AD_Value[bt];
					
					buf += (Indata*Indata);					
					bt = FIFO_pointer(bt, ADC_N, ADCbufSize);
				}			
				
				
				buf /= (ADC_Period/2);
				buf = sqrt(buf);
				buf /= EepromData.ratio;				
				
				CanComData.FaultRms = buf;
				
				step++;
				
				len = 0;
				CanComData.Txbuf[len++] = (uint8_t)iCanComHad;
				CanComData.Txbuf[len++] = iCanComHad>>8;
				CanComData.Txbuf[len++] = 0xE1;
				len++;
				len++;	
				//====test
				//CanComData.FaultRms=2.0;
				memcpy(CanComData.Txbuf+len,(uint8_t *)&CanComData.FaultRms,4);
				
				
				len	+=	4;
				CanComData.Txbuf[3] = (len-5);
				CanComData.Txbuf[4] = (len-5)>>8;
				bt = CRC_GetModbus16(CanComData.Txbuf, len);
				CanComData.Txbuf[len++] = bt;
				CanComData.Txbuf[len++] = bt>>8;
				SendCanbuf(EepromData.CanID,CanComData.Txbuf,len);
			}
			break;
		case 1:
			if(FIFO_length(CanComData.Faultbt, AD_pt, ADCbufSize/2)>=(11 * ADC_Period * ADC_N)/2){
				bt = FIFO_Backpointer(CanComData.Faultbt, (10 * ADC_Period * ADC_N)/2, ADCbufSize/2);
				bt *=2;
				for(i=0;i<20 * ADC_Period;i++){
					//bt1 = FIFO_pointer(bt, ADC_N-1, ADCbufSize);
					//CanComData.FaultWave[i] = ADCFifo.AD_Value[bt] - ADCFifo.AD_Value[bt1];
					CanComData.FaultWave[i] = ADCFifo.AD_Value[bt+1] - ADCFifo.AD_Value[bt];
					bt = FIFO_pointer(bt, ADC_N, ADCbufSize);
				}
				//Test1Clr();
				step = 0;
				CanComData.FaultFlag = 0;
				StartCalRmsFlag=0;
				CanComData.CanWaveSampleFlag = 1;
				
			
			}
			break;
		default:
			step = 0;
	}
}//SelCalRMS


/*-----------------------------end--------------------------------------------------*/


