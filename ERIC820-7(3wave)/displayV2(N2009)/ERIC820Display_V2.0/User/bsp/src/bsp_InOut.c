
#include "bsp.h"			/* 底层硬件驱动 */
//u16 ExternalIO;
//static u8 InPutScan(u16 *InputState);
//static void PhaseSwitchDisplay(void);
//static void TemPorterDisplay(void);
/*
---------------------------------------------------------------------
Name		InOutDisplay
Function	输入输出处理
Input		No
OutPut	No
Edit by fanzhong
Date		2018-08-20
---------------------------------------------------------------------
*/
void InOutDisplay(void)
{
	static u8 IRTimH,IRTimL,OutTim;

		
	if(ModuleCombuf->LEDState & (1<<1)){
		PutOP_LED_ON();
	}else{
		PutOP_LED_OFF();
	}
	if(ModuleCombuf->LEDState & (1<<3)){
		Ground_LED_ON();
	}else{
		Ground_LED_OFF();
	}
	if(ModuleCombuf->LEDState & (1<<5)){
		Decompression_LED_ON();
	}else{
		Decompression_LED_OFF();
	}
	if(IRInputPin()){
		IRTimL = 0;
		if(IRTimH<10){
			IRTimH++;
		}else{
			SystemSet.LcdSet.LcdFlag = 1;
		}
	}else{
		IRTimH= 0;
		if(IRTimL<10){
			IRTimL++;
		}else{
			SystemSet.LcdSet.LcdFlag = 0;
			
		}
	}
	if(0x01 == SystemState.SwitchState.AphaseState){
		if(0 == ModuleComSet10Fault(0x00,0x00,0x01)){
			SystemState.SwitchState.AphaseState = 0;
			SystemState.SwitchState.BphaseState = 0;
			SystemState.SwitchState.CphaseState = 0;
		}
		
	}else if(0x01 == SystemState.SwitchState.BphaseState){
		if(0 == ModuleComSet10Fault(0x00,0x00,0x02)){
			SystemState.SwitchState.AphaseState = 0;
			SystemState.SwitchState.BphaseState = 0;
			SystemState.SwitchState.CphaseState = 0;
		}
	}else if(0x01 == SystemState.SwitchState.CphaseState){
		if(0 == ModuleComSet10Fault(0x00,0x00,0x03)){
			SystemState.SwitchState.AphaseState = 0;
			SystemState.SwitchState.BphaseState = 0;
			SystemState.SwitchState.CphaseState = 0;
		}
	}else if(0x01 == SystemState.SwitchState.DphaseState){
		if(0 == ModuleComSet10Fault(0x00,0x00,0x04)){
			SystemState.SwitchState.AphaseState = 0;
			SystemState.SwitchState.BphaseState = 0;
			SystemState.SwitchState.CphaseState = 0;
		}
	 	
	}else if((0x02 == SystemState.SwitchState.AphaseState)||
			 (0x02 == SystemState.SwitchState.BphaseState)||
			 (0x02 == SystemState.SwitchState.CphaseState)||
			 (0x02 == SystemState.SwitchState.DphaseState)){
		 if(0 == ModuleComSet10Fault(0x00,0x00,0x05)){
			SystemState.SwitchState.AphaseState = 0;
			SystemState.SwitchState.BphaseState = 0;
			SystemState.SwitchState.CphaseState = 0;
			SystemState.SwitchState.DphaseState = 0; 
		}
	}else if(ModuleCombuf->ResetFlag){
		if(0 == ModuleComSet10Fault(0x00,0x00,0x05)){
			ModuleCombuf->ResetFlag = 0;
		}
	}
	
	if(OutTim >= 50){		
		if(0 == ModuleComSet13Fault((ModuleCombuf->OutState & 0x7FFF),0x00)){
			OutTim = 0;			
		}
	}else{
		OutTim++;
	}

	//TemPorterDisplay();
	
}//InOutDisplay
#if 0
/*
---------------------------------------------------------------------
Name		TemPorterDisplay
Function	温度保护器，输出处理
Input		No
OutPut	No
Edit by fanzhong
Date		2018-08-28
---------------------------------------------------------------------
*/
static void TemPorterDisplay(void)
{
	uint8_t i,j,flag;
	uint8_t Temflag,Poroterflag;

	Temflag = 0;
	Poroterflag = 0;
	for(i=0;i<20;i++){
		for(j=0;j<12;j++){
			if(Cupboard_P[i][j].OnlineFlag){
				switch(Cupboard_P[i][j].Type){
					case Bus_A:
						if(Cupboard_P[i][j].Value.TempBusA > SystemSet.LineTempMax*10){ 
							
						  Temflag++;
						}
						break;
					case Bus_B:
						if(Cupboard_P[i][j].Value.TempBusB > SystemSet.LineTempMax*10){
						  
						  Temflag++;
						} 
						break;
					case Bus_C:
						if(Cupboard_P[i][j].Value.TempBusC > SystemSet.LineTempMax*10){
						  Temflag++;
						} 
						break;
					case TouchUp_A:
						if(Cupboard_P[i][j].Value.TempTouchUpA > SystemSet.LineTempMax*10){
						  Temflag++;
						}
						break;
					case TouchUp_B:
						if(Cupboard_P[i][j].Value.TempTouchUpB > SystemSet.LineTempMax*10){
						  Temflag++;
						}
						break;
					case TouchUp_C:
						if(Cupboard_P[i][j].Value.TempTouchUpC > SystemSet.LineTempMax*10){
						  Temflag++;
						}
						break;
					case TouchDown_A:
						if(Cupboard_P[i][j].Value.TempTouchDownA > SystemSet.LineTempMax*10){
						  Temflag++;
						}					
						break;
					case TouchDown_B:
						if(Cupboard_P[i][j].Value.TempTouchDownB > SystemSet.LineTempMax*10){
						  Temflag++;
						}
						break;
					case TouchDown_C:
						if(Cupboard_P[i][j].Value.TempTouchDownC > SystemSet.LineTempMax*10){
						  Temflag++;
						}
						break;
					case OverVoltage_P:
						flag = 0;
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA < SystemSet.TempSet.MinValue*100) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA > SystemSet.TempSet.MaxValue*100)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB < SystemSet.TempSet.MinValue*100) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB > SystemSet.TempSet.MaxValue*100)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC < SystemSet.TempSet.MinValue*100) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC > SystemSet.TempSet.MaxValue*100)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA < SystemSet.HumiditySet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA > SystemSet.HumiditySet.MaxValue)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB < SystemSet.HumiditySet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB > SystemSet.HumiditySet.MaxValue)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC < SystemSet.HumiditySet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC > SystemSet.HumiditySet.MaxValue)){
						   flag++;
						} 
						if(Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent > SystemSet.LeakageCurrentSet.LeakageValue){
						   flag++;
						}
						if(flag){
							Poroterflag++;
						}
						break;
					
				}
			}
		}
	}
	if(Temflag){
		AlarmOverTemON();
	}else{
		AlarmOverTemOFF();
	}
	if(Poroterflag){
		ProtectorON();
	}else{
		ProtectorOFF();
	}
}//TemPorterDisplay
#endif


