#ifndef _BSP_SYN6288_H_
#define _BSP_SYN6288_H_



extern void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);//,uint8_t len);

extern void YS_SYN_Set(uint8_t *Info_data,uint8_t len);

void bsp_InitSyn6288(void);
void  Syn6288SendData(u16 buflen);
#endif
//----------------end------------------------------
