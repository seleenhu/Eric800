#ifndef _MYDEFINE_H_
#define	_MYDEFINE_H_


#include "bsp.h"

typedef struct{
	uint8_t bit0:1;
	uint8_t bit1:1;
	uint8_t bit2:1;
	uint8_t bit3:1;
	uint8_t bit4:1;
	uint8_t bit5:1;
	uint8_t bit6:1;
	uint8_t bit7:1;
}BITS81;
extern BITS81 TimData;	

#define F5mS	TimData.bit0
#define F10mS	TimData.bit1

#define iCanRxSize		1024
#define iCanComHad		0xCBFF
#define iCanSetComHad	0xDBFF



#define MCUSoft_RST()   do{__set_FAULTMASK(1);NVIC_SystemReset();}while(0)

#pragma pack (1)
typedef struct{
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
	u32 us;
}AlarmTimStruct;
#pragma pack ()

typedef struct{
	uint8_t Txbuf[2048];
	uint8_t Rxbuf[iCanRxSize];
	uint16_t RxbufPt;
	uint16_t RxbufBt;
	uint8_t SetRxbuf[iCanRxSize];
	uint16_t SetRxbufPt;
	uint16_t SetRxbufBt;
	uint8_t Displaybuf[iCanRxSize];

	uint8_t CanSendFlag;
	uint8_t CanWaveSampleFlag;
	uint8_t CanSendWaveBag;
	//uint8_t OnlineFlag;
	uint16_t OnlineTim;
	uint8_t FaultFlag;
	uint16_t Faultbt;
	float FaultRms;
	AlarmTimStruct FaultTim;
	int16_t FaultWave[256*20];
}CanComStaruct;
extern CanComStaruct *CanComData;




extern AlarmTimStruct SystemTime;

#define EepromAdress    0x0000
#define EepromPassWord  0xA602
extern uint8_t EepromSaveFlag;
typedef struct{
	uint16_t PassWordKey;	
	float ratio;
	uint16_t CanID;
}EepromStruct;
extern EepromStruct EepromData;

typedef struct{
	uint8_t Type; //π ’œ¿‡–Õ
	uint8_t Phase; // bit0:A  bit1:B bit2:C
	uint8_t Grid;
	AlarmTimStruct Tim;
	float RMSbuf[8];
	int16_t Wave[8][12*128];
}FaultStruct;
#define FaultbufSIZE    2
extern uint8_t FaultbufLen;
//extern FaultStruct Faultbuf[];
extern uint16_t uSCount;
enum FaultEnum{
	FaultRst = 0,
	FaultGnd,
	FaultExceedV,
	FaultSysExceedV,
	FaultPt,
	FaultLowV
};

extern uint8_t EEpromflag;
extern uint8_t Verifyflag;

#define Test1Set()	GPIO_SetBits(GPIOA, GPIO_Pin_9);
#define Test1Clr()	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
#define Test1Xor()	GPIO_ToggleBits(GPIOA, GPIO_Pin_9);

#define Test2Set()	GPIO_SetBits(GPIOA, GPIO_Pin_10);
#define Test2Clr()	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
#define Test2Xor()	GPIO_ToggleBits(GPIOA, GPIO_Pin_10);


void TestPinInit(void);


uint16_t FIFO_length(uint16_t bt,uint16_t pt,uint16_t len);
uint16_t FIFO_pointer(uint16_t pointer,uint16_t len,uint16_t sizelen);
uint16_t FIFO_Backpointer(uint16_t pointer,uint16_t len,uint16_t sizelen);
uint8_t FIFO_bufcmp(uint8_t *FIFObuf,uint8_t *buf,uint16_t bt,uint16_t len,uint16_t sizelen);
uint16_t CRC_GetModbus16(uint8_t *pdata, uint16_t len);
uint32_t AsciiToHex(uint8_t *Asciibuf,uint8_t Asciibuf_len);
void EepromDataInit(void);
uint8_t FlashWrite(void);




#endif
/*---------end-------------------------------*/


