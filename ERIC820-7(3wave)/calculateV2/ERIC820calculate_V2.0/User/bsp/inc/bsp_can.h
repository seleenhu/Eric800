#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#define iCanHead    0xCBFF
#define iCanNum		128
#define iCanbufSize 256*20*2*2
#define iCanMaxBagSize    (256*20*2+256*2)
#define iCanMainId	0x300
#define iCanWavebufSize 256*20*2

typedef union{
	uint8_t UnionData;
	struct{
		uint8_t bit0:1;
		uint8_t bit1:1;
		uint8_t bit2:1;
		uint8_t bit3:1;
		uint8_t bit4:1;
		uint8_t bit5:1;
		uint8_t bit6:1;		
		uint8_t bit7:1;
		
	}Bits;	
}CANBitUnion;

extern CANBitUnion CanBitData;
#define CanSampleFlag	CanBitData.Bits.bit0
#define CanRmsRxFlag	CanBitData.Bits.bit1
#define CanWaveRxFlag	CanBitData.Bits.bit2

	


extern uint16_t CanRxDelayTim;
extern uint16_t CanReadWaveDelayTim;
extern uint16_t CanReadRmsDelayTim;


void can_Init(void);
void can_NVIC_Config(void);
uint8_t SendCanMsg(uint16_t iddata,uint8_t *buf,uint8_t buf_len);
void SendCanbuf(uint16_t iddata,uint8_t *buf,uint16_t buf_len);
//void CanBreath(void);
void CanSelStart(void);
uint8_t CanReadWave(uint8_t Sid);
void CanSBreath(uint8_t Sid);
void CAN1_Task(void);
void AllSelDisplay(void);
void CanSBreathTimes(void);



#endif




