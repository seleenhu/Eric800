#ifndef _BSP_RS485_H_
#define _BSP_RS485_H_

#define RS485_RX_EN()			GPIO_SetBits(GPIOH,GPIO_Pin_15)
#define RS485_TX_EN()			GPIO_ResetBits(GPIOH,GPIO_Pin_15)

#define RS485_RX_PIN()			GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_0)

#define RS485_TX_SET()			GPIO_SetBits(GPIOI,GPIO_Pin_1)
#define RS485_TX_CLR()			GPIO_ResetBits(GPIOI,GPIO_Pin_1)

extern void bsp_InitRS485(void);
extern void RS485_DMA2_Stream7_IRQn(void);
#endif

