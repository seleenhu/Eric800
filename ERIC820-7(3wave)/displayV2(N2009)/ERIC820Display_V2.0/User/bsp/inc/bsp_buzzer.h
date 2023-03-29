#ifndef _BSP_BUSSER_H_
#define _BSP_BUSSER_H_

#define BuzzerON()		GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define BuzzerOFF()		GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define BuzzerPIN()		GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3)
#define BuzzerLongOn()	{BuzzerON(); BuzzerMode=1;}
extern uint8_t BuzzerMode;
extern void BuzzerTask(void);
#endif



