

#include "bsp.h"

#define W5500CS_SET()				GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define W5500CS_CLR()				GPIO_ResetBits(GPIOB, GPIO_Pin_12) 

#define W5500RST_SET()			GPIO_SetBits(GPIOD, GPIO_Pin_7)
#define W5500RST_CLR()			GPIO_ResetBits(GPIOD, GPIO_Pin_7) 

#define   W5500_NetIP					SystemSet.NetSet.IPadrr
#define   W5500_NetPort					SystemSet.NetSet.Port



uint8 W5500_mac[6] = {0,0x08,0xdc,0x11,0x12,0x11};
uint8 W5500_lip[4] = {192,168,1,106};
uint8 W5500_sub[4] = {255,255,255,0};
uint8 W5500_gw[4] = {192,168,1,1};
uint8 W5500_dns[4] = {8,8,8,8};
uint16 W5500_lport = 30000;

//#include "stm32f4xx.h"
//#include "Utility.h"
//#include "socket.h"
//#include "w5500_config.h"
//#include "w5500.h"
//#include <stdio.h>
//#include <string.h>
//#include "bsp_cpu_flash.h"
//#include "stm32f4xx_flash.h"


//#define	FLASH_ADR	0x08060000		//网络配置信息要写入的Flash地址
//#define	FLASH_DATA	0x5a5a5a5a		//要写入的测试数据

//网络配置结构体，此结构体直接写入Flash保存
//CONFIG_MSG  net_cfg1, net_cfg2 ,ConfigMsg;

//W5500重新配置标识，web配置网络信息时使用
uint8 reset_flag=0;

uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// 选择8个Socket每个Socket发送缓存的大小，在w5500.c的void sysinit()有设置过程
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// 选择8个Socket每个Socket接收缓存的大小，在w5500.c的void sysinit()有设置过程

//网络收发使用的缓冲区结构体
//TNET_BUF net_rbuf,net_tbuf;



uint16 W5500TXBufLen;
uint8 W5500TXBuf[W5500BufSize];

//w5500 SPI nss
void WIZ_CS(uint8 val)
{
    if (val == LOW)
    {
        W5500CS_CLR();
    }
    else if (val == HIGH)
    {
        W5500CS_SET();
    }
}


/*
*********************************************************************************************************
*	函 数 名: TcpClient_Task
*	功能说明: tcp client 主任务(放置在线程内)，负责建立socket连接，进行数据收发，监测网络状态并采取相应措施
*	形    参：s: socket 通道号，port: TCP服务器端口号
*	返 回 值: 无
*********************************************************************************************************
*/
void TcpClient_Task(SOCKET s)
{
#if 0
    uint8   gets;
    uint16  FSR_len=0, RSR_len=0, rev_len=0, send_len=0, i=0;
    uint8 	netbuf[2048];

    gets=getSn_SR(s);

    switch(gets) 										// 获取socket(s)的状态
    {
    case SOCK_INIT: 									// Socket(s)处于初始化完成(打开)状态
        net_tbuf.state=0;
        connect(s, net_cfg1.sip,net_cfg1.sport);		// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
        NetDBG("connect...\r\n");
        break;
    case SOCK_ESTABLISHED:								// Socket(s)处于连接建立状态
        net_tbuf.state=1;
        if(getSn_IR(s) & Sn_IR_CON)
        {
            setSn_IR(s, Sn_IR_CON); 					// Sn_IR的CON位置1，通知W5500连接已建立
        }

        RSR_len=getSn_RX_RSR(s);						// RSR_len=Socket(s)接收缓存中已接收和保存的数据大小
        if(RSR_len>0)
        {
            rev_len=recv(s,netbuf,RSR_len); 			// W5500接收来自服务器的数据，并通过SPI发送给MCU
            while(rev_len--)
            {
                net_rbuf.buf[net_rbuf.w]=netbuf[i++];
                net_rbuf.w=(net_rbuf.w+1)%NET_BUFSIZE;
            }
        }
        //先接收再发送
        i=0;
        FSR_len=getSn_TX_FSR(s);						//查询空闲发送缓冲区长度,每个sockes通道最大是2048
        while(net_tbuf.r!=net_tbuf.w)					//有数据需要发送
        {
            netbuf[i++]=net_tbuf.buf[net_tbuf.r];
            net_tbuf.r=(net_tbuf.r+1)%NET_BUFSIZE;
            if(i>=FSR_len)								//发送数据分片，不超过2048
                break;
        }

        if(i > 0)										//有效数据
        {
            send_len = send(s, netbuf, i);
            if(send_len != i)
            {
                disconnect(s);							// 如果出现发不出数据的情况一定是断网了
                NetDBG("send_len fail...send_len = %d\r\n",send_len);
            }
        }

        break;
    case SOCK_CLOSE_WAIT:								// Socket处于等待关闭状态
        net_tbuf.state=0;
        close(s);										// 关闭Socket0
        NetDBG("SOCK_CLOSE_WAIT\r\n");
        break;
    case SOCK_CLOSED:									// Socket处于关闭状态
        net_tbuf.state=0;
        socket(s,Sn_MR_TCP,net_cfg1.lport,Sn_MR_ND);	// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
        NetDBG("SOCK_CLOSED\r\n");
        break;
    }
#else
	uint8   gets;
	uint16  RSR_len=0;
	//uint8 	netbuf[2048];
	int8 rt;
	uint16 W5500RXBufLen;
	uint8 W5500RXBuf[2048];
	
	gets=getSn_SR(s);
	switch(gets){ 										// 获取socket(s)的状态		
		case SOCK_INIT: 									// Socket(s)处于初始化完成(打开)状态
			
			connect(s, W5500_NetIP,W5500_NetPort);		// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
			
			break;
		case SOCK_ESTABLISHED:								// Socket(s)处于连接建立状态
			
			if(getSn_IR(s) & Sn_IR_CON){			
				setSn_IR(s, Sn_IR_CON); 					// Sn_IR的CON位置1，通知W5500连接已建立
			}

			RSR_len=getSn_RX_RSR(s);						// RSR_len=Socket(s)接收缓存中已接收和保存的数据大小
			if(RSR_len>0){
				
				rt = recv(s,W5500RXBuf,RSR_len); 			// W5500接收来自服务器的数据，并通过SPI发送给MCU
				if(rt > 0){
					W5500RXBufLen = RSR_len;
					/*
					switch(Eepromdata.Mode){
						case Mode_ADCandNET:
							memcpy(CollectTxbuf,W5500RXBuf,W5500RXBufLen);
							CollectTxbufLen = W5500RXBufLen;							
							break;
						case Mode_NETandRF433:
							memcpy(RF433TX1Buff,W5500RXBuf,W5500RXBufLen);
							RF433TX1BuffLen = W5500RXBufLen;							
							break;
						case Mode_NETandRS485:
							memcpy(RS485TXbuf,W5500RXBuf,W5500RXBufLen);
							RS485TXbufLen = W5500RXBufLen;							
							break;
					}
					*/
				}
				
			}
			if(W5500TXBufLen){
				RSR_len = getSn_TX_FSR(s);
				if(RSR_len >= W5500TXBufLen){					
					send(s, W5500TXBuf, W5500TXBufLen);
					W5500TXBufLen = 0;					
				}
			}
			
			#if 0
			//先接收再发送
			i=0;
			FSR_len=getSn_TX_FSR(s);						//查询空闲发送缓冲区长度,每个sockes通道最大是2048
			

			if(i > 0)										//有效数据
			{
				send_len = send(s, netbuf, i);			
			}
			#endif

			break;
		case SOCK_CLOSE_WAIT:								// Socket处于等待关闭状态
			
			close(s);										// 关闭Socket0
			
			break;
		case SOCK_CLOSED:									// Socket处于关闭状态
			
			socket(s,Sn_MR_TCP,W5500_lport,Sn_MR_ND);	// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
			
			break;
	}
#endif
}
#if 0
/*
*********************************************************************************************************
*	函 数 名: UDP_Task
*	功能说明: UDP主任务(放置在线程内)
*	形    参：s: socket 通道号
*	返 回 值: 无
*********************************************************************************************************
*/
void UDP_Task(SOCKET s)
{
    uint16 FSR_len=0,RSR_len=0,rev_len=0,send_len=0,i=0;
    uint8 netbuf[2048];

    switch(getSn_SR(s)) 					// 获取socket0的状态
    {
    case SOCK_UDP:							// Socket处于初始化完成(打开)状态
        net_tbuf.state=1;
        if(getSn_IR(s) & Sn_IR_RECV)
        {
            setSn_IR(s, Sn_IR_RECV);		// Sn_IR的RECV位置1
        }

        if((RSR_len=getSn_RX_RSR(s))>8)
        {
            RSR_len-=8;
            memset(netbuf,0,2048);
            rev_len=recvfrom(s,netbuf, RSR_len, (uint8*)(&net_cfg1.sip),(uint16*)(&net_cfg1.sport));// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
            while(rev_len--)
            {
                net_rbuf.buf[net_rbuf.w]=netbuf[i++];
                net_rbuf.w=(net_rbuf.w+1)%NET_BUFSIZE;
            }
        }

        i=0;
        FSR_len=getSn_TX_FSR(s);			//查询空闲发送缓冲区长度,每个sockes通道最大是2048
        while(net_tbuf.r!=net_tbuf.w)		//有数据需要发送
        {
            netbuf[i++]=net_tbuf.buf[net_tbuf.r];
            net_tbuf.r=(net_tbuf.r+1)%NET_BUFSIZE;
            if(i>=FSR_len)					//发送数据分片，不超过2048
                break;
        }

        if(i)// 有数据发送
        {
            send_len=sendto(s,netbuf,i, (uint8*)(&net_cfg1.sip), net_cfg1.sport);
            if(send_len != i)
            {
				NetDBG("UDP Send fail\r\n");
            }
        }
        break;
    case SOCK_CLOSED:							// Socket处于关闭状态
        net_tbuf.state=0;
        socket(s,Sn_MR_UDP,net_cfg1.lport,0);	// 打开Socket0，并配置为UDP模式，打开一个本地端口
        NetDBG("SOCK_CLOSED\r\n");
        break;
    }

}


/*
*********************************************************************************************************
*	函 数 名: Net_Send
*	功能说明: 网络发送数据函数，TCP或UDP方式共用此函数(TCP、UDP互斥使用)
*	形    参：tbuf:发送数据缓冲区，len: 发送数据长度
*	返 回 值: 返回当前网络状态，1:正常，0:网络异常
*********************************************************************************************************
*/
uint16 Net_Send(uint8 *tbuf,uint16 len)
{
    if(net_tbuf.state)
    {
        while(len--)
        {
            net_tbuf.buf[net_tbuf.w]=*(tbuf++);
            net_tbuf.w=(net_tbuf.w+1)%NET_BUFSIZE;
        }
        return 1;
    }
    else
        return 0;
}

/*
*********************************************************************************************************
*	函 数 名: Net_Rev
*	功能说明: 网络接收数据函数，TCP或UDP方式共用此函数(TCP、UDP互斥使用)
*	形    参：rbuf:接收数据缓冲区
*	返 回 值: 接收到的数据长度
*********************************************************************************************************
*/
uint16 Net_Rev(uint8 *rbuf)
{
    uint16 ret=0;
    while(net_rbuf.r != net_rbuf.w )
    {
        *(rbuf++)=net_rbuf.buf[net_rbuf.r];
        net_rbuf.r=(net_rbuf.r+1)%NET_BUFSIZE;
        ret++;
    }
    return ret;
}
#endif

void W5500_task(void)
{
	static uint8_t Step;
	static uint16_t DelayTim;
	
	switch(Step){
		case 0:
			W5500RST_CLR();
			DelayTim = 0;
			Step++;
			break;
		case 1:
			DelayTim++;
			if(DelayTim>=10){				
				W5500RST_SET();
				DelayTim = 0;
				Step++;
			}
			break;
		case 2:
			DelayTim++;
			if(DelayTim>=50){
				DelayTim = 0;
				Step++;
			}
			break;
		case 3:
			
			setSHAR(W5500_mac);			
			setSUBR(W5500_sub);
			setGAR(W5500_gw);
			setSIPR(W5500_lip);

			setRTR(2000);												// 设置超时时间
			setRCR(3);													// 设置最大重新发送次数
			//Init. TX & RX Memory size
			sysinit(txsize, rxsize);
			/***** 打开W5500的Keepalive功能 *****/
	        setkeepalive(0);			
			Step++;
			break;
		case 4:
			TcpClient_Task(SOCK_TCPC);
			break;
	}
}//W5500_task
/*
---------------------------------------------------------------------
Name		W5500_Init
Function	W5500初始化
Input		No
OutPut	    No
Edit by fanzhong
Date		2019-05-17
---------------------------------------------------------------------
*/
void W5500_Init(void)
{
	
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);// 使能 SPI1 时钟
	
	//这里只针对 SPI 口初始化
	RCC_APB2PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//复位 SPI2
	RCC_APB2PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//停止复位 SPI2
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置 SPI 全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置 SPI 工作模式:主 SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //设置 SPI 的数据大小: 8 位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS 信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //预分频 256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据传输从 MSB 位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC 值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure); //根据指定的参数初始化外设 SPIx 寄存器
	SPI_Cmd(SPI2, ENABLE); //使能 SPI1
	W5500RST_SET();
	W5500CS_SET();
}//W5500_Init
/*
*********************************************************************************************************
*	函 数 名: W5500_spiWrite
*	功能说明: 向SPI总线发送一个字节。SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void W5500_spiWrite(uint8_t _ucByte)
{
	/* 等待发送缓冲区空 */	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	/* 发送一个字节 */	
	SPI_I2S_SendData(SPI2, _ucByte);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */	
	SPI_I2S_ReceiveData(SPI2);
	

}

/*
*********************************************************************************************************
*	函 数 名: W5500_spiRead
*	功能说明: 从SPI总线接收8个bit数据。 SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t W5500_spiRead(void)
{
	uint8_t read;

	/* 等待发送缓冲区空 */
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */	
	SPI_I2S_SendData(SPI2, 0);

	/* 等待数据接收完毕 */	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */	
	read = SPI_I2S_ReceiveData(SPI2);

	/* 返回读到的数据 */
	return read;

}



