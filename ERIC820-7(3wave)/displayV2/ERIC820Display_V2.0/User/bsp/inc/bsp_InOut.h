#ifndef _BSP_INOUT_H_
#define _BSP_INOUT_H_

//#define Earth_LED_ON()				GPIO_SetBits(GPIOH, GPIO_Pin_14)
//#define Earth_LED_OFF()				GPIO_ResetBits(GPIOH, GPIO_Pin_14)
//#define Earth_LED_STATE()			GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_14)
#if 0

#define Com_LED_ON()				GPIO_SetBits(GPIOH, GPIO_Pin_14)
#define Com_LED_OFF()				GPIO_ResetBits(GPIOH, GPIO_Pin_14)
#define Com_LED_XOR()				GPIO_ToggleBits(GPIOH, GPIO_Pin_14)
#define Com_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_14)

#define OverVoltage_LED_ON()				GPIO_SetBits(GPIOH, GPIO_Pin_15)
#define OverVoltage_LED_OFF()				GPIO_ResetBits(GPIOH, GPIO_Pin_15)
#define OverVoltage_LED_STATE()			GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_15)

#define OverTem_LED_ON()				GPIO_SetBits(GPIOI, GPIO_Pin_3)
#define OverTem_LED_OFF()				GPIO_ResetBits(GPIOI, GPIO_Pin_3)
#define OverTem_LED_STATE()			GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_3)

#define Decompression_LED_ON()				  GPIO_SetBits(GPIOI, GPIO_Pin_2)
#define Decompression_LED_OFF()				GPIO_ResetBits(GPIOI, GPIO_Pin_2)
#define Decompression_LED_STATE()			GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_2)

#define Fault_LED_ON()				  GPIO_SetBits(GPIOI, GPIO_Pin_1)
#define Fault_LED_OFF()					GPIO_ResetBits(GPIOI, GPIO_Pin_1)
#define Fault_LED_STATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_1)

#define PutOP_LED_ON()				  GPIO_SetBits(GPIOI, GPIO_Pin_0)
#define PutOP_LED_OFF()					GPIO_ResetBits(GPIOI, GPIO_Pin_0)
#define PutOP_LED_STATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_0)
#else
#define Com_LED_ON()				GPIO_SetBits(GPIOH, GPIO_Pin_6)
#define Com_LED_OFF()				GPIO_ResetBits(GPIOH, GPIO_Pin_6)
#define Com_LED_XOR()				GPIO_ToggleBits(GPIOH, GPIO_Pin_6)
#define Com_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_6)

#define PutOP_LED_ON()				  	GPIO_SetBits(GPIOH, GPIO_Pin_8)
#define PutOP_LED_OFF()					GPIO_ResetBits(GPIOH, GPIO_Pin_8)
#define PutOP_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_8)

#define Power_LED_ON()				  	GPIO_SetBits(GPIOH, GPIO_Pin_9)
#define Power_LED_OFF()					GPIO_ResetBits(GPIOH, GPIO_Pin_9)
#define Power_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_9)

#define Ground_LED_ON()				  	GPIO_SetBits(GPIOH, GPIO_Pin_10)
#define Ground_LED_OFF()					GPIO_ResetBits(GPIOH, GPIO_Pin_10)
#define Ground_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_10)

#define Alarm_LED_ON()				  	GPIO_SetBits(GPIOH, GPIO_Pin_11)
#define Alarm_LED_OFF()					GPIO_ResetBits(GPIOH, GPIO_Pin_11)
#define Alarm_LED_STATE()				GPIO_ReadOutputDataBit(GPIOH, GPIO_Pin_11)

#define Decompression_LED_ON()				GPIO_SetBits(GPIOH, GPIO_Pin_12)
#define Decompression_LED_OFF()				GPIO_ResetBits(GPIOH, GPIO_Pin_12)
#define Decompression_LED_STATE()			GPIO_ReadOutputDataBit(GPIOH,GPIO_Pin_12)
#endif


//#define InputPin1()							GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_7)
//#define InputPin2()							GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_6)
//#define InputPin3()							GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_8)
//#define InputPin4()							GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
//#define InputPin5()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
//#define InputPin6()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
//#define InputPin7()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
//#define InputPin8()							GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_9)
//#define InputPin9()							GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_10)
//#define InputPin10()						GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_11)
//#define InputPin11()						GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
//#define InputPin12()						GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
//#define InputPin13()						GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13)


#define Out1PinSet()				 ModuleCombuf->OutState |= (1<<0)
#define Out1PinReset()				 ModuleCombuf->OutState &= ~(1<<0)
#define Out1PinSTATE()				 ModuleCombuf->OutState ^= (1<<0)

#define Out2PinSet()				 ModuleCombuf->OutState |= (1<<1)
#define Out2PinReset()				ModuleCombuf->OutState &= ~(1<<1)
#define Out2PinSTATE()				ModuleCombuf->OutState ^= (1<<1)

#define Out3PinSet()				  ModuleCombuf->OutState |= (1<<2)
#define Out3PinReset()				ModuleCombuf->OutState &= ~(1<<2)
#define Out3PinSTATE()				ModuleCombuf->OutState ^= (1<<2)

#define Out4PinSet()				 ModuleCombuf->OutState |= (1<<3)
#define Out4PinReset()				ModuleCombuf->OutState &= ~(1<<3)
#define Out4PinSTATE()				ModuleCombuf->OutState ^= (1<<3)

#define Out5PinSet()				 ModuleCombuf->OutState |= (1<<4)
#define Out5PinReset()				ModuleCombuf->OutState &= ~(1<<4)
#define Out5PinSTATE()				ModuleCombuf->OutState ^= (1<<4)

#define Out6PinSet()				ModuleCombuf->OutState |= (1<<5)
#define Out6PinReset()				ModuleCombuf->OutState &= ~(1<<5)
#define Out6PinSTATE()				ModuleCombuf->OutState ^= (1<<5)

#define Out7PinSet()				 ModuleCombuf->OutState |= (1<<6)
#define Out7PinReset()				ModuleCombuf->OutState &= ~(1<<6)
#define Out7PinSTATE()				ModuleCombuf->OutState ^= (1<<6)

#define Out8PinSet()				ModuleCombuf->OutState |= (1<<7)
#define Out8PinReset()				ModuleCombuf->OutState &= ~(1<<7)
#define Out8PinSTATE()				ModuleCombuf->OutState ^= (1<<7)

#define Out9PinSet()				ModuleCombuf->OutState |= (1<<8)
#define Out9PinReset()				ModuleCombuf->OutState &= ~(1<<8)
#define Out9PinSTATE()				ModuleCombuf->OutState ^= (1<<8)

#define Out10PinSet()				ModuleCombuf->OutState |= (1<<9)
#define Out10PinReset()				ModuleCombuf->OutState &= ~(1<<9)
#define Out10PinSTATE()				ModuleCombuf->OutState ^= (1<<9)

#define Out11PinSet()				ModuleCombuf->OutState |= (1<<10)
#define Out11PinReset()				ModuleCombuf->OutState &= ~(1<<10)
#define Out11PinSTATE()				ModuleCombuf->OutState ^= (1<<10)

#define Out12PinSet()				ModuleCombuf->OutState |= (1<<11)
#define Out12PinReset()				ModuleCombuf->OutState &= ~(1<<11)
#define Out12PinSTATE()				ModuleCombuf->OutState ^= (1<<11)


#define TestPinSet()					GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define TestPinReset()				GPIO_ResetBits(GPIOB, GPIO_Pin_2)
//#define TestPinXor()				  GPIO_ToggleBits(GPIOB, GPIO_Pin_2)

//#define Test1PinSet()					GPIO_SetBits(GPIOE, GPIO_Pin_2)
//#define Test1PinReset()				GPIO_ResetBits(GPIOE, GPIO_Pin_2)
//#define Test1PinXor()				  GPIO_ToggleBits(GPIOE, GPIO_Pin_2)

//#define Test2PinSet()					GPIO_SetBits(GPIOI, GPIO_Pin_4)
//#define Test2PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_4)
//#define Test2PinXor()				  GPIO_ToggleBits(GPIOI, GPIO_Pin_4)





#define IRInputPin()						GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_4)

extern u16 ExternalIO;
#define PutOPState()			(ExternalIO & 0x0001)      /*投运状态*/

#define PhaseAStateIN()			((ExternalIO >> 1) & 0x01) /*相位开关状态*/
#define PhaseBStateIN()			((ExternalIO >> 2) & 0x01) /*相位开关状态*/
#define PhaseCStateIN()			((ExternalIO >> 3) & 0x01) /*相位开关状态*/

#define PhaseRSTIN()			((ExternalIO >> 4) & 0x01) /*相位开关复位状态*/

#define SinglePhaseSwitchIN()	((ExternalIO >> 5) & 0x01) /*单相智能开关复位状态*/

#define CurrentLimitingFuseAIN()	((ExternalIO >> 6) & 0x01) /*限流熔丝状态*/
#define CurrentLimitingFuseBIN()	((ExternalIO >> 7) & 0x01) /*限流熔丝状态*/
#define CurrentLimitingFuseCIN()	((ExternalIO >> 8) & 0x01) /*限流熔丝状态*/


#define SwitchAON()						Out1PinSet()
#define SwitchAOFF()					Out1PinReset()
#define SwitchASTATE()					Out1PinSTATE()

#define SwitchBON()						Out2PinSet()
#define SwitchBOFF()					Out2PinReset()
#define SwitchBSTATE()					Out2PinSTATE()

#define SwitchCON()						Out3PinSet()
#define SwitchCOFF()					Out3PinReset()
#define SwitchCSTATE()					Out3PinSTATE()

#define SinglePhaseSwitchON()	Out4PinSet()
#define SinglePhaseSwitchOFF()  Out4PinReset()

#define ProtectorON()			Out5PinSet()
#define ProtectorOFF()		    Out5PinReset()

#define AlarmOverTemON()		Out6PinSet()
#define AlarmOverTemOFF()		Out6PinReset()

#define AlarmGndON()			Out7PinSet()
#define AlarmGndOFF()		    Out7PinReset()

#define AlarmOverVoltageON()	Out8PinSet()
#define AlarmOverVoltageOFF()	Out8PinReset()

#define AlarmLowVoltageON()		Out9PinSet()
#define AlarmLowVoltageOFF()	Out9PinReset()

#define AlarmDecompressionON()	Out10PinSet()
#define AlarmDecompressionOFF()	Out10PinReset()

#define AlarmPTON()				Out11PinSet()
#define AlarmPTOFF()			Out11PinReset()

#define BlackOutON()			Out12PinSet()
#define BlackOutOFF()			Out12PinReset()

extern void InOutDisplay(void);
#endif



