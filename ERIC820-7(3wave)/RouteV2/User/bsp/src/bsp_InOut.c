
#include "bsp.h"			/* 底层硬件驱动 */
u16 ExternalIO;
//uint16_t LEDStatue;
static u8 InPutScan(u16 *InputState);
static void PhaseSwitchDisplay(void);
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
	
	u8 rt;
	//static uint16_t LEDStateBackup; 	

	
	rt = InPutScan(&ExternalIO);
	if(0 ==rt){
		if(PutOPState()){
			PutOP_LED_OFF();
			ADCParameter.flag &= ~(1<<JudgePutOPBit);
		}else{
			PutOP_LED_ON();
			ADCParameter.flag |= (1<<JudgePutOPBit);
		}
		
		if(PhaseAStateIN()){
			SystemState.SwitchState.phaseAction &= ~(1<<0);
		}else{
			SystemState.SwitchState.phaseAction |= (1<<0);
		}
		if(PhaseBStateIN()){
			SystemState.SwitchState.phaseAction &= ~(1<<1);
		}else{
			SystemState.SwitchState.phaseAction |= (1<<1);
		}
		if(PhaseCStateIN()){
			SystemState.SwitchState.phaseAction &= ~(1<<2);
		}else{
			SystemState.SwitchState.phaseAction |= (1<<2);
		}

		if(0 == PhaseRSTIN()){
			SystemState.SwitchState.AphaseState = 2;
			SystemState.SwitchState.BphaseState = 2;
			SystemState.SwitchState.CphaseState = 2;
		}
				
	}
	if(0x01 == ComData.PhaseSwitch){
		SystemState.SwitchState.AphaseState = 1;
		SystemState.SwitchState.BphaseState = 0;
		SystemState.SwitchState.CphaseState = 0;
	}else if(0x02 == ComData.PhaseSwitch){
		SystemState.SwitchState.AphaseState = 0;
		SystemState.SwitchState.BphaseState = 1;
		SystemState.SwitchState.CphaseState = 0;
	}else if(0x03 == ComData.PhaseSwitch){
		SystemState.SwitchState.AphaseState = 0;
		SystemState.SwitchState.BphaseState = 0;
		SystemState.SwitchState.CphaseState = 1;
	}else if(0x04 == ComData.PhaseSwitch){
		SystemState.SwitchState.AphaseState = 2;
		SystemState.SwitchState.BphaseState = 2;
		SystemState.SwitchState.CphaseState = 2;
	}
	ComData.PhaseSwitch = 0;

	PhaseSwitchDisplay();
	
}//InOutDisplay

/*
---------------------------------------------------------------------
Name		InPutScan
Function	输入扫描
Input		输入状态
OutPut	No
Edit by fanzhong
Date		2018-08-24
---------------------------------------------------------------------
*/
static u8 InPutScan(u16 *InputState)
{
	u8 rt;
	u16 InputData;
	static u8 InputTimes;
	static u16 InputDataBackup;
	
	rt = 1;
	InputData = 0;
	if(InputPin1()){
		InputData |= (1<<0);
	}
	if(InputPin2()){
		InputData |= (1<<1);
	}
	if(InputPin3()){
		InputData |= (1<<2);
	}
	if(InputPin4()){
		InputData |= (1<<3);
	}
	if(InputPin5()){
		InputData |= (1<<4);
	}
	if(InputPin6()){
		InputData |= (1<<5);
	}
	if(InputPin7()){
		InputData |= (1<<6);
	}
	if(InputPin8()){
		InputData |= (1<<7);
	}
	if(InputPin9()){
		InputData |= (1<<8);
	}
	if(InputPin10()){
		InputData |= (1<<9);
	}
	if(InputPin11()){
		InputData |= (1<<10);
	}
	if(InputPin12()){
		InputData |= (1<<11);
	}
	if(InputPin13()){
		InputData |= (1<<12);
	}
	if(0 == InputTimes){
		InputDataBackup = InputData;
		InputTimes++;
	}else if(InputTimes < 5){
		InputTimes++;
		if(InputDataBackup != InputData){
			InputTimes = 0;
		}
	}else{
		InputTimes = 0;
		if(InputDataBackup == InputData){
			*InputState = InputData;
			rt = 0;
		}
	}
	return rt;	
}//InPutScan


/*
---------------------------------------------------------------------
Name		PhaseSwitchDisplay
Function	相位开关输出处理
Input		No
OutPut	No
Edit by fanzhong
Date		2018-08-28
---------------------------------------------------------------------
*/
static void PhaseSwitchDisplay(void)
{
#if 1
	if(1 == SystemState.SwitchState.AphaseState){
		if((0 == SwitchASTATE())&&(0 == SwitchBSTATE())&&(0 == SwitchCSTATE())){
			SwitchAON();
			SwitchBOFF();
			SwitchCOFF();
			AlarmGndON();
		}
	}else if(1 == SystemState.SwitchState.BphaseState){
		if((0 == SwitchASTATE())&&(0 == SwitchBSTATE())&&(0 == SwitchCSTATE())){
			SwitchAOFF();
			SwitchBON();
			SwitchCOFF();
			AlarmGndON();
		}
	}else if(1 == SystemState.SwitchState.CphaseState){
		if((0 == SwitchASTATE())&&(0 == SwitchBSTATE())&&(0 == SwitchCSTATE())){
			SwitchAOFF();
			SwitchBOFF();
			SwitchCON();
			AlarmGndON();
		}
	}else if((2 == SystemState.SwitchState.AphaseState) ||
			 (2 == SystemState.SwitchState.BphaseState) ||
			 (2 == SystemState.SwitchState.CphaseState)){
		SwitchAOFF();
		SwitchBOFF();
		SwitchCOFF();
		AlarmGndOFF();
	}
	SystemState.SwitchState.AphaseState = 0;
	SystemState.SwitchState.BphaseState = 0;
	SystemState.SwitchState.CphaseState = 0;
#else
	static u8  SwitchTimS;
	static u16 DelayTim;
	const u8 DelayTimC = 40;
	if(0 == DelayTim){		
		if(1 == SystemState.SwitchState.AphaseState){
			if(0 == SystemState.SwitchState.phaseAction){	
				if(SwitchTimS<3){
					PhaseAStateON();
					DelayTim = DelayTimC;
					SwitchTimS++;
				}else{
					SystemState.SwitchState.AphaseState = 0;
					SwitchTimS = 0;
				}
			}else{
				SystemState.SwitchState.AphaseState = 0;
				SwitchTimS = 0;
			}
		}else if(1 == SystemState.SwitchState.BphaseState){
			if(0 == SystemState.SwitchState.phaseAction){
				if(SwitchTimS<3){
					PhaseBStateON();
					DelayTim = DelayTimC;
					SwitchTimS++;
				}else{
					SystemState.SwitchState.BphaseState = 0;
					SwitchTimS = 0;
				}
			}else{
				SystemState.SwitchState.BphaseState = 0;
				SwitchTimS = 0;
			}
		}else if(1 == SystemState.SwitchState.CphaseState){
			if(0 == SystemState.SwitchState.phaseAction){
				if(SwitchTimS<3){
					PhaseCStateON();
					DelayTim = DelayTimC;
					SwitchTimS++;
				}else{
					SystemState.SwitchState.CphaseState = 0;
					SwitchTimS = 0;
				}
			}else{
				SystemState.SwitchState.CphaseState = 0;
				SwitchTimS = 0;
			}
		}else if((2 == SystemState.SwitchState.AphaseState)||
						 (2 == SystemState.SwitchState.BphaseState)||
						 (2 == SystemState.SwitchState.CphaseState)){
				if(SystemState.SwitchState.phaseAction){
					if(SwitchTimS<3){
						PhaseAStateOFF();
						PhaseBStateOFF();
						PhaseCStateOFF();
						DelayTim = DelayTimC;
						SwitchTimS++;						
					}else{
						SystemState.SwitchState.AphaseState = 0;
						SystemState.SwitchState.BphaseState = 0;
						SystemState.SwitchState.CphaseState = 0;
						SwitchTimS = 0;
					}						
				}else{
					SystemState.SwitchState.AphaseState = 0;
					SystemState.SwitchState.BphaseState = 0;
					SystemState.SwitchState.CphaseState = 0;
					SwitchTimS = 0;
				}
		}
	}else{
		DelayTim--;
	}
#endif
}//PhaseSwitchDisplay

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
						if(Cupboard_P[i][j].Value.TempBusA < SystemSet.LineTempMax){ 
							
						  Temflag++;
						}
						break;
					case Bus_B:
						if(Cupboard_P[i][j].Value.TempBusB < SystemSet.LineTempMax){
						  
						  Temflag++;
						} 
						break;
					case Bus_C:
						if(Cupboard_P[i][j].Value.TempBusC< SystemSet.LineTempMax){
						  Temflag++;
						} 
						break;
					case TouchUp_A:
						if(Cupboard_P[i][j].Value.TempTouchUpA < SystemSet.LineTempMax){
						  Temflag++;
						}
						break;
					case TouchUp_B:
						if(Cupboard_P[i][j].Value.TempTouchUpB < SystemSet.LineTempMax){
						  Temflag++;
						}
						break;
					case TouchUp_C:
						if(Cupboard_P[i][j].Value.TempTouchUpC < SystemSet.LineTempMax){
						  Temflag++;
						}
						break;
					case TouchDown_A:
						if(Cupboard_P[i][j].Value.TempTouchDownA < SystemSet.LineTempMax){
						  Temflag++;
						}					
						break;
					case TouchDown_B:
						if(Cupboard_P[i][j].Value.TempTouchDownB < SystemSet.LineTempMax){
						  Temflag++;
						}
						break;
					case TouchDown_C:
						if(Cupboard_P[i][j].Value.TempTouchDownC < SystemSet.LineTempMax){
						  Temflag++;
						}
						break;
					case OverVoltage_P:
						flag = 0;
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA < SystemSet.TempSet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA > SystemSet.TempSet.MaxValue)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB < SystemSet.TempSet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB > SystemSet.TempSet.MaxValue)){
						   flag++;
						}
						if((Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC < SystemSet.TempSet.MinValue) ||
						   (Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC > SystemSet.TempSet.MaxValue)){
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

