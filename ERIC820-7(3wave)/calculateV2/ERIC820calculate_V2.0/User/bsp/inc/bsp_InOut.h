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



#define InputPin1()							GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)
#define InputPin2()							GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)
#define InputPin3()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define InputPin4()							GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)
#define InputPin5()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define InputPin6()							GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define InputPin7()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14)
#define InputPin8()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13)
//#define InputPin9()							GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)
//#define InputPin10()						GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)



#define Out1PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_11)
#define Out1PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_11)
#define Out1PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_11)

#define Out2PinSet()				  GPIO_SetBits(GPIOI, GPIO_Pin_10)
#define Out2PinReset()				GPIO_ResetBits(GPIOI, GPIO_Pin_10)
#define Out2PinSTATE()				GPIO_ReadOutputDataBit(GPIOI, GPIO_Pin_10)

#define Out3PinSet()				  GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define Out3PinReset()				GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define Out3PinSTATE()				GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)


#define Out4PinSet()				  GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define Out4PinReset()				GPIO_ResetBits(GPIOD, GPIO_Pin_3)
#define Out4PinSTATE()				GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_3)




#define LEDStatue ComData.LEDState

extern u16 ExternalIO;
#define PutOPState()			(ExternalIO & 0x0001)      /*投运状态*/

#define PhaseAStateIN()			((ExternalIO >> 1) & 0x01) /*相位开关状态*/
#define PhaseBStateIN()			((ExternalIO >> 2) & 0x01) /*相位开关状态*/
#define PhaseCStateIN()			((ExternalIO >> 3) & 0x01) /*相位开关状态*/
#define PhaseDStateIN()			((ExternalIO >> 4) & 0x01)

#define PhaseRSTIN()			((ExternalIO >> 5) & 0x01) /*相位开关复位状态*/

//#define SinglePhaseSwitchIN()	((ExternalIO >> 5) & 0x01) /*单相智能开关复位状态*/

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


#define SwitchDON()						Out4PinSet()
#define SwitchDOFF()					Out4PinReset()
#define SwitchDSTATE()					Out4PinSTATE()



extern void InOutDisplay(void);
#endif



