/***************************�����Ƶ���****************************
**  �������ƣ�YS-SYN6288�����ϳ����׳���
**	CPU: STC89LE52
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-SYN6288�����ϳ�ģ��
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2012.8.25
**  ˵��������
***************************�����Ƶ���*******************************/

//#include "includes.h"	
#include  <bsp.h>

#define UART6_BAUD			9600
#define UART6_TX_BUF_SIZE	1*256
#define UART6_RX_BUF_SIZE	1*256

/* ����6��GPIO -- GPRS,WIFI */
#define UART6_TX_PORT      GPIOG
#define UART6_TX_PIN       GPIO_Pin_14
#define UART6_TX_CLK       RCC_AHB1Periph_GPIOG
#define UART6_TX_SOURCE    GPIO_PinSource14

#define UART6_RX_PORT      GPIOC
#define UART6_RX_PIN       GPIO_Pin_7
#define UART6_RX_CLK       RCC_AHB1Periph_GPIOC
#define UART6_RX_SOURCE    GPIO_PinSource7


uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ���ͻ����� */
uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ���ջ����� */











/**************оƬ��������*********************/
uint8_t SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//ֹͣ�ϳ�
uint8_t SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//��ͣ�ϳ�
uint8_t SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//�ָ��ϳ�
uint8_t SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//״̬��ѯ
uint8_t SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//����POWER DOWN ״̬����

/***********************************************/

/*===================================================================
*Name W_StringLen()
*Function ˫�ֽ�����������
*input �ַ���ָ��
*output �ַ�������
*Edit by Seleen 2018-8-16
====================================================================*/
static uint8_t W_StringLen(char *str)
{
	uint8_t len=0;
	while(*(u16 *)(str+2*len))
	{
		len++;
		if(len>=255)
			break;		
	}
	return len;
}

/***********************************************************
* ��    �ƣ�  YS-SYN6288 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���SYN6288оƬ���кϳɲ���
* ��ڲ�����Music(��������ѡ��):0�ޱ������֡�1-15����ر�������
            *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo��0�����������ӿƼ�������
**********************************************************/
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata)//,uint8_t len)

{
/****************��Ҫ���͵��ı�**********************************/ 
		 unsigned  char  Frame_Info[50];
     unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//����У���ֽ�
	   unsigned  int i=0; 
		 //HZ_Length =strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���
		 HZ_Length=2*W_StringLen((char *)HZdata); 			//��Ҫ�����ı��ĳ��� modify by seleen 2018-8-16
	   //HZ_Length=len;
 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01 | Music;//<<4 ;  //����������������������趨

/*******************У�������***************************************/		 
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(Frame_Info[i]);		//�Է��͵��ֽڽ������У��	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	         ecc=ecc^(HZdata[i]); 				//�Է��͵��ֽڽ������У��		
	     }		 
/*******************����֡��Ϣ***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  //PrintCom(Frame_Info,5+HZ_Length+1);
			//comSendBuf(COM6,Frame_Info,5+HZ_Length+1); 
			memcpy(g_TxBuf6,Frame_Info,5+HZ_Length+1);
			Syn6288SendData(5+HZ_Length+1); 
			 
}


/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ���� 
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��					 
* ���÷�����ͨ�������Ѿ�������������������á� 
**********************************************************/

void YS_SYN_Set(uint8_t *Info_data,uint8_t len)
{
	uint8_t Com_Len;
	//Com_Len =strlen(Info_data);
	Com_Len=len;
	//PrintCom(Info_data,Com_Len);
	//comSendBuf(COM6,Info_data,Com_Len);
	memcpy(g_TxBuf6,Info_data,Com_Len);
	Syn6288SendData(Com_Len);
}



/*
---------------------------------------------------------------------
Name		InitSyn6288
Function	��ʼ������оƬ
Input		No
OutPut	No
Edit by seleen 
Date		2018-8-16
---------------------------------------------------------------------
*/
void	bsp_InitSyn6288(void)
{
	
		//GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
		USART_Init(USART6, &USART_InitStructure);
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
		DMA_DeInit(DMA2_Stream2);
		while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
	
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART6->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_RxBuf6;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = UART6_RX_BUF_SIZE;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2, &DMA_InitStructure);
		
		USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
		
		DMA_Cmd(DMA2_Stream2, ENABLE);	
		
		DMA_DeInit(DMA2_Stream7);
		while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);		
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART6->DR);
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_TxBuf6;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_BufferSize = UART6_TX_BUF_SIZE;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream7, &DMA_InitStructure);	
		
				
		USART_Cmd(USART6, ENABLE);
		USART_ClearFlag(USART6, USART_FLAG_TC);
		
		
		
}


/*
---------------------------------------------------------------------
Name		Syn6288SendData
Function	Send���ݷ���TXBUF
Input		����buf ���ݳ���
OutPut	No
Edit by 
Date		2018-08-16
---------------------------------------------------------------------
*/
void  Syn6288SendData(u16 buflen)
{	
	
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(DMA2_Stream7, DISABLE);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	
	DMA_SetCurrDataCounter(DMA2_Stream7,buflen); 
	DMA_Cmd(DMA2_Stream7, ENABLE); 	
	while(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);
	
	
}


