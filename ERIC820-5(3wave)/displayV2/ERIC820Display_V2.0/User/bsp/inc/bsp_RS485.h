#ifndef _BSP_RS485_H_
#define _BSP_RS485_H_

extern void bsp_InitRS485(void);
extern void IRQ_RS485(void);
extern void RS485Send(u8 *buf,u16 buf_len);
extern void RS485_Task(void);
#endif
