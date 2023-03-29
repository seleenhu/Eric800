#ifndef _BSP_4G_H_
#define _BSP_4G_H_


#define   NetIP						SystemSet.NetSet.IPadrr
#define   NetPort					SystemSet.NetSet.Port
#define   NetModbusID			SystemSet.NetSet.ModbudID
#define   NetModbusAdr		SystemSet.NetSet.Modbusadr


#define		Power_4G_ON()		GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define   Power_4G_OFF()	GPIO_ResetBits(GPIOF,GPIO_Pin_8)

#define RX4Gbuff_len		2048
#define Buf4GRxSize     (RX4Gbuff_len-DMA_GetCurrDataCounter(DMA1_Stream3))
extern u8 	RX4Gbuff[];
extern u16 RX4G_bt;								
#define TX4Gbuff_len		2048
extern u8 TX4Gbuff[];
extern u8 TX4Gbuff2[];


extern u8 	RX4GDisplaybuff[];
extern u16 RX4GDisplaybuff_len;





extern u8 Mode4G;
extern u8 Step4G;
#define  POWER_4G_ON 							0
#define  RST_4G 								1
#define  Connect_4G_Login 						2
#define  Connect_4G_Normal 						3	
#define  Connect_4G_breath						4
#define  Connect_4G_Tem							5
#define  Connect_4G_TemaP						6
#define  Connect_4G_quality						7
#define  Connect_4G_XBAG						8
#define  Connect_4G_exceedV						9
#define  Connect_4G_PTBag						10
#define  Connect_4G_Short						11
#define  Connect_ProtectorAction				12
#define  Connect_4G_SystemSet					13
#define  Connect_4G_SystemSetDown				14
#define  Connect_4G_ReviceDisplay				15
#define  Connect_4G_Protector					16
#define  POWER_4G_OFF 							17

extern u8 Com4GLedState;
//static void  U4GSend(u8 *buf,u16 buflen);
extern void  U4GSendTxbuf(u16 buflen);


extern void	Init4G(void);
extern void Display4G(void);
#endif
//----------------end------------------------------
