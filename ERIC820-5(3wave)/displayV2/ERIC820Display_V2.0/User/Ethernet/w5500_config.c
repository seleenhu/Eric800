

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


//#define	FLASH_ADR	0x08060000		//����������ϢҪд���Flash��ַ
//#define	FLASH_DATA	0x5a5a5a5a		//Ҫд��Ĳ�������

//�������ýṹ�壬�˽ṹ��ֱ��д��Flash����
//CONFIG_MSG  net_cfg1, net_cfg2 ,ConfigMsg;

//W5500�������ñ�ʶ��web����������Ϣʱʹ��
uint8 reset_flag=0;

uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ͻ���Ĵ�С����w5500.c��void sysinit()�����ù���
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ջ���Ĵ�С����w5500.c��void sysinit()�����ù���

//�����շ�ʹ�õĻ������ṹ��
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
*	�� �� ��: TcpClient_Task
*	����˵��: tcp client ������(�������߳���)��������socket���ӣ����������շ����������״̬����ȡ��Ӧ��ʩ
*	��    �Σ�s: socket ͨ���ţ�port: TCP�������˿ں�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TcpClient_Task(SOCKET s)
{
#if 0
    uint8   gets;
    uint16  FSR_len=0, RSR_len=0, rev_len=0, send_len=0, i=0;
    uint8 	netbuf[2048];

    gets=getSn_SR(s);

    switch(gets) 										// ��ȡsocket(s)��״̬
    {
    case SOCK_INIT: 									// Socket(s)���ڳ�ʼ�����(��)״̬
        net_tbuf.state=0;
        connect(s, net_cfg1.sip,net_cfg1.sport);		// ����Sn_CRΪCONNECT������TCP������������������
        NetDBG("connect...\r\n");
        break;
    case SOCK_ESTABLISHED:								// Socket(s)�������ӽ���״̬
        net_tbuf.state=1;
        if(getSn_IR(s) & Sn_IR_CON)
        {
            setSn_IR(s, Sn_IR_CON); 					// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
        }

        RSR_len=getSn_RX_RSR(s);						// RSR_len=Socket(s)���ջ������ѽ��պͱ�������ݴ�С
        if(RSR_len>0)
        {
            rev_len=recv(s,netbuf,RSR_len); 			// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
            while(rev_len--)
            {
                net_rbuf.buf[net_rbuf.w]=netbuf[i++];
                net_rbuf.w=(net_rbuf.w+1)%NET_BUFSIZE;
            }
        }
        //�Ƚ����ٷ���
        i=0;
        FSR_len=getSn_TX_FSR(s);						//��ѯ���з��ͻ���������,ÿ��sockesͨ�������2048
        while(net_tbuf.r!=net_tbuf.w)					//��������Ҫ����
        {
            netbuf[i++]=net_tbuf.buf[net_tbuf.r];
            net_tbuf.r=(net_tbuf.r+1)%NET_BUFSIZE;
            if(i>=FSR_len)								//�������ݷ�Ƭ��������2048
                break;
        }

        if(i > 0)										//��Ч����
        {
            send_len = send(s, netbuf, i);
            if(send_len != i)
            {
                disconnect(s);							// ������ַ��������ݵ����һ���Ƕ�����
                NetDBG("send_len fail...send_len = %d\r\n",send_len);
            }
        }

        break;
    case SOCK_CLOSE_WAIT:								// Socket���ڵȴ��ر�״̬
        net_tbuf.state=0;
        close(s);										// �ر�Socket0
        NetDBG("SOCK_CLOSE_WAIT\r\n");
        break;
    case SOCK_CLOSED:									// Socket���ڹر�״̬
        net_tbuf.state=0;
        socket(s,Sn_MR_TCP,net_cfg1.lport,Sn_MR_ND);	// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض˿�
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
	switch(gets){ 										// ��ȡsocket(s)��״̬		
		case SOCK_INIT: 									// Socket(s)���ڳ�ʼ�����(��)״̬
			
			connect(s, W5500_NetIP,W5500_NetPort);		// ����Sn_CRΪCONNECT������TCP������������������
			
			break;
		case SOCK_ESTABLISHED:								// Socket(s)�������ӽ���״̬
			
			if(getSn_IR(s) & Sn_IR_CON){			
				setSn_IR(s, Sn_IR_CON); 					// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
			}

			RSR_len=getSn_RX_RSR(s);						// RSR_len=Socket(s)���ջ������ѽ��պͱ�������ݴ�С
			if(RSR_len>0){
				
				rt = recv(s,W5500RXBuf,RSR_len); 			// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
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
			//�Ƚ����ٷ���
			i=0;
			FSR_len=getSn_TX_FSR(s);						//��ѯ���з��ͻ���������,ÿ��sockesͨ�������2048
			

			if(i > 0)										//��Ч����
			{
				send_len = send(s, netbuf, i);			
			}
			#endif

			break;
		case SOCK_CLOSE_WAIT:								// Socket���ڵȴ��ر�״̬
			
			close(s);										// �ر�Socket0
			
			break;
		case SOCK_CLOSED:									// Socket���ڹر�״̬
			
			socket(s,Sn_MR_TCP,W5500_lport,Sn_MR_ND);	// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض˿�
			
			break;
	}
#endif
}
#if 0
/*
*********************************************************************************************************
*	�� �� ��: UDP_Task
*	����˵��: UDP������(�������߳���)
*	��    �Σ�s: socket ͨ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UDP_Task(SOCKET s)
{
    uint16 FSR_len=0,RSR_len=0,rev_len=0,send_len=0,i=0;
    uint8 netbuf[2048];

    switch(getSn_SR(s)) 					// ��ȡsocket0��״̬
    {
    case SOCK_UDP:							// Socket���ڳ�ʼ�����(��)״̬
        net_tbuf.state=1;
        if(getSn_IR(s) & Sn_IR_RECV)
        {
            setSn_IR(s, Sn_IR_RECV);		// Sn_IR��RECVλ��1
        }

        if((RSR_len=getSn_RX_RSR(s))>8)
        {
            RSR_len-=8;
            memset(netbuf,0,2048);
            rev_len=recvfrom(s,netbuf, RSR_len, (uint8*)(&net_cfg1.sip),(uint16*)(&net_cfg1.sport));// W5500��������Զ����λ�������ݣ���ͨ��SPI���͸�MCU
            while(rev_len--)
            {
                net_rbuf.buf[net_rbuf.w]=netbuf[i++];
                net_rbuf.w=(net_rbuf.w+1)%NET_BUFSIZE;
            }
        }

        i=0;
        FSR_len=getSn_TX_FSR(s);			//��ѯ���з��ͻ���������,ÿ��sockesͨ�������2048
        while(net_tbuf.r!=net_tbuf.w)		//��������Ҫ����
        {
            netbuf[i++]=net_tbuf.buf[net_tbuf.r];
            net_tbuf.r=(net_tbuf.r+1)%NET_BUFSIZE;
            if(i>=FSR_len)					//�������ݷ�Ƭ��������2048
                break;
        }

        if(i)// �����ݷ���
        {
            send_len=sendto(s,netbuf,i, (uint8*)(&net_cfg1.sip), net_cfg1.sport);
            if(send_len != i)
            {
				NetDBG("UDP Send fail\r\n");
            }
        }
        break;
    case SOCK_CLOSED:							// Socket���ڹر�״̬
        net_tbuf.state=0;
        socket(s,Sn_MR_UDP,net_cfg1.lport,0);	// ��Socket0��������ΪUDPģʽ����һ�����ض˿�
        NetDBG("SOCK_CLOSED\r\n");
        break;
    }

}


/*
*********************************************************************************************************
*	�� �� ��: Net_Send
*	����˵��: ���緢�����ݺ�����TCP��UDP��ʽ���ô˺���(TCP��UDP����ʹ��)
*	��    �Σ�tbuf:�������ݻ�������len: �������ݳ���
*	�� �� ֵ: ���ص�ǰ����״̬��1:������0:�����쳣
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
*	�� �� ��: Net_Rev
*	����˵��: ����������ݺ�����TCP��UDP��ʽ���ô˺���(TCP��UDP����ʹ��)
*	��    �Σ�rbuf:�������ݻ�����
*	�� �� ֵ: ���յ������ݳ���
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

			setRTR(2000);												// ���ó�ʱʱ��
			setRCR(3);													// ����������·��ʹ���
			//Init. TX & RX Memory size
			sysinit(txsize, rxsize);
			/***** ��W5500��Keepalive���� *****/
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
Function	W5500��ʼ��
Input		No
OutPut	    No
Edit by fanzhong
Date		2019-05-17
---------------------------------------------------------------------
*/
void W5500_Init(void)
{
	
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);// ʹ�� SPI1 ʱ��
	
	//����ֻ��� SPI �ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λ SPI2
	RCC_APB2PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λ SPI2
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //���� SPI ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //���� SPI ����ģʽ:�� SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //���� SPI �����ݴ�С: 8 λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS �ź���Ӳ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //Ԥ��Ƶ 256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ���� MSB λ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC ֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure); //����ָ���Ĳ�����ʼ������ SPIx �Ĵ���
	SPI_Cmd(SPI2, ENABLE); //ʹ�� SPI1
	W5500RST_SET();
	W5500CS_SET();
}//W5500_Init
/*
*********************************************************************************************************
*	�� �� ��: W5500_spiWrite
*	����˵��: ��SPI���߷���һ���ֽڡ�SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void W5500_spiWrite(uint8_t _ucByte)
{
	/* �ȴ����ͻ������� */	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	/* ����һ���ֽ� */	
	SPI_I2S_SendData(SPI2, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */	
	SPI_I2S_ReceiveData(SPI2);
	

}

/*
*********************************************************************************************************
*	�� �� ��: W5500_spiRead
*	����˵��: ��SPI���߽���8��bit���ݡ� SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t W5500_spiRead(void)
{
	uint8_t read;

	/* �ȴ����ͻ������� */
	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */	
	SPI_I2S_SendData(SPI2, 0);

	/* �ȴ����ݽ������ */	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */	
	read = SPI_I2S_ReceiveData(SPI2);

	/* ���ض��������� */
	return read;

}



