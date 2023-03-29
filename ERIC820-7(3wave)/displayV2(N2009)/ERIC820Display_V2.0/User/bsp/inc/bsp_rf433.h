
#ifndef _BSP_RF433_H_
#define _BSP_RF433_H_

typedef enum{
	SaveIdle = 0,
	SaveTem,
	SaveProtectorTime,
	SaveProtectorCurrentTime,
	SaveProtectorSudden
}RF433Enum;

typedef struct{
	uint8_t i;
	uint8_t j;
	RF433Enum flag;	
}RF433Struct;

extern RF433Struct RF433SD[16];

extern void InitRF433(void);
extern void RF433Task(void);
//extern void ISR_RF433(void);
extern void TemExcessJudge(void);
//extern uint8_t RFRXJudge(CupboardParameter CupboardData);

#endif

