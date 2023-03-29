#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "Types.h"

//#define NetDBG printf
#define NetDBG 

#define ON					  1
#define OFF					  0
#define HIGH				  1
#define LOW					  0

#define CONFIG_MSG_LEN        sizeof(CONFIG_MSG) - 4 // the 4 bytes OP will not save to EEPROM
#define MAX_BUF_SIZE		  1460
#define KEEP_ALIVE_TIME	      30	// 30sec
// SRAM address range is 0x2000 0000 ~ 0x2000 BFFF (48KB)
#define SOCK_BUF_ADDR 		  0x20000000
#define AppBackAddress        0x08020000 //from 128K
#define ConfigAddr			  0x0800FC00
#define NORMAL_STATE          0
#define NEW_APP_IN_BACK       1 //there is new app in back address
#define CONFIGTOOL_FW_UP      2 //configtool update f/w in app

//#pragma pack(1)
typedef struct _CONFIG_MSG //36字节
{
    uint8 	mac[6];		//mac 地址
    uint8 	lip[4];		//本机ip
    uint8 	sub[4];		//子网掩码
    uint8 	gw[4];		//网关
    uint8 	dns[4];		//dns
	uint8 	sip[4];		//服务器ip
	uint16 	lport;		//本机port
	uint16  sport;		//服务器port
	uint8   sw_ver[2];  //软件版本
	uint32  chksum;		//配置信息校验和
} CONFIG_MSG;
//#pragma pack()


#define DEVICE_ID "W5500"

#define FW_VER_HIGH   	1
#define FW_VER_LOW    	0


//#define WIZ_SCS			    	GPIO_Pin_12	// out
//#define WIZ_SCLK				GPIO_Pin_13	// out
//#define WIZ_MISO				GPIO_Pin_14	// in
//#define WIZ_MOSI				GPIO_Pin_15	// out

//#define WIZ_RESET		    	GPIO_Pin_5	// out
//#define WIZ_INT			    	GPIO_Pin_6	// in
//#define NET_BUFSIZE				4096

//typedef struct _NET_BUF					//网络收发缓冲区结构 
//{
//	uint32 state;		//当前网络状态
//	uint16 r;			//缓冲区读指针
//	uint16 w;			//缓冲区写指针
//	uint8  buf[NET_BUFSIZE];	//缓冲区		
//}TNET_BUF;

#define W5500BufSize 2048
extern uint16 W5500TXBufLen;
extern uint8 W5500TXBuf[];



/* Private function prototypes -----------------------------------------------*/
void WIZ_CS(uint8 val);
//uint8 SPI2_SendByte(uint8 byte);
//void WIZ_SPI_Init(void);
//void GPIO_Configuration(void);
//void NVIC_Configuration(void);
//void Set_default(void);
//void Reset_W5500(void);
//uint8 Save_NetCfg(CONFIG_MSG *p);
//uint8 Load_NetCfg(CONFIG_MSG *p);

void W5500_Init(void);
void W5500_spiWrite(uint8 _ucByte);
uint8 W5500_spiRead(void);
void TcpClient_Task(SOCKET s);
//void UDP_Task(SOCKET s);

//uint16 Net_Send(uint8 *tbuf,uint16 len);
//uint16 Net_Rev(uint8 *rbuf);

void TcpClient_Task(SOCKET s);
void W5500_task(void);
#endif


