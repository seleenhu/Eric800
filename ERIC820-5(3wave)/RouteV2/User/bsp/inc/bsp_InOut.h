#ifndef _BSP_INOUT_H_
#define _BSP_INOUT_H_



//#define Com_LED_ON()				(LEDStatue |= (1<<0))
//#define Com_LED_OFF()				(LEDStatue &= ~(1<<0))
//#define Com_LED_XOR()				(LEDStatue ^= (1<<0))
//#define Com_LED_STATE()				(LEDStatue &= (1<<0))

#define PutOP_LED_ON()				  	(LEDStatue |= (1<<1))
#define PutOP_LED_OFF()					(LEDStatue &= ~(1<<1))
#define PutOP_LED_STATE()				(LEDStatue &= (1<<1))

//#define OverTem_LED_ON()				(LEDStatue |= (1<<2))
//#define OverTem_LED_OFF()				(LEDStatue &= ~(1<<2))
//#define OverTem_LED_STATE()				(LEDStatue &= (1<<2))

#define Ground_LED_ON()					(LEDStatue |= (1<<3))
#define Ground_LED_OFF()				(LEDStatue &= ~(1<<3))
#define Ground_LED_STATE()				(LEDStatue &= (1<<3))

//#define Fault_LED_ON()				  	(LEDStatue |= (1<<4))
//#define Fault_LED_OFF()					(LEDStatue &= ~(1<<4))
//#define Fault_LED_STATE()				(LEDStatue &= (1<<4))

#define Decompression_LED_ON()				(LEDStatue |= (1<<5))
#define Decompression_LED_OFF()				(LEDStatue &= ~(1<<5))
#define Decompression_LED_STATE()			(LEDStatue &= (1<<5))



#define InputPin1()							GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_13)
#define InputPin2()							GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define InputPin3()							GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)
#define InputPin4()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)
#define InputPin5()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13)
#define InputPin6()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14)
#define InputPin7()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define InputPin8()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define InputPin9()							GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)
#define InputPin10()						GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define InputPin11()						GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)
#define InputPin12()						GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)
#define InputPin13()						GPIO_ReadInputDataBit(GPIOH, GPIO_Pin_14)


#define Out1PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_7)
#define Out1PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_7)
#define Out1PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_7)

#define Out2PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_6)
#define Out2PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_6)
#define Out2PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_6)

#define Out3PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_5)
#define Out3PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_5)
#define Out3PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_5)

#define Out4PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_4)
#define Out4PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_4)
#define Out4PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_4)

#define Out5PinSet()				  GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define Out5PinReset()				GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define Out5PinSTATE()				GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)

#define Out6PinSet()				  GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define Out6PinReset()				GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define Out6PinSTATE()				GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8)

#define Out7PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_11)
#define Out7PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_11)
#define Out7PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_11)

#define Out8PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_10)
#define Out8PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_10)
#define Out8PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_10)

#define Out9PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_9)
#define Out9PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_9)
#define Out9PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_9)

#define Out10PinSet()				  GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define Out10PinReset()				GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define Out10PinSTATE()				GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)

#define Out11PinSet()				 GPIO_SetBits(GPIOI, GPIO_Pin_8)
#define Out11PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_8)
#define Out11PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_8)

#define Out12PinSet()				GPIO_SetBits(GPIOE, GPIO_Pin_3)
#define Out12PinReset()				GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define Out12PinSTATE()				GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3)


#define TestPinSet()					GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define TestPinReset()				GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define TestPinXor()				  GPIO_ToggleBits(GPIOB, GPIO_Pin_2)

//#define Test1PinSet()					GPIO_SetBits(GPIOE, GPIO_Pin_2)
//#define Test1PinReset()				GPIO_ResetBits(GPIOE, GPIO_Pin_2)
//#define Test1PinXor()				  GPIO_ToggleBits(GPIOE, GPIO_Pin_2)

//#define Test2PinSet()					GPIO_SetBits(GPIOI, GPIO_Pin_4)
//#define Test2PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_4)
//#define Test2PinXor()				  GPIO_ToggleBits(GPIOI, GPIO_Pin_4)





#define LEDStatue ComData.LEDState

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



