/***************************飞音云电子****************************
**  工程名称：YS-SYN6288语音合成配套程序
**	CPU: STC89LE52
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-SYN6288语音合成模块
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.8.25
**  说明：。。
***************************飞音云电子*******************************/

//#include "includes.h"	
#include  <bsp.h>

#define UART6_BAUD			9600
#define UART6_TX_BUF_SIZE	1*256
#define UART6_RX_BUF_SIZE	1*256

/* 串口6的GPIO -- GPRS,WIFI */
#define UART6_TX_PORT      GPIOG
#define UART6_TX_PIN       GPIO_Pin_14
#define UART6_TX_CLK       RCC_AHB1Periph_GPIOG
#define UART6_TX_SOURCE    GPIO_PinSource14

#define UART6_RX_PORT      GPIOC
#define UART6_RX_PIN       GPIO_Pin_7
#define UART6_RX_CLK       RCC_AHB1Periph_GPIOC
#define UART6_RX_SOURCE    GPIO_PinSource7


uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* 发送缓冲区 */
uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* 接收缓冲区 */











/**************芯片设置命令*********************/
uint8_t SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//停止合成
uint8_t SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//暂停合成
uint8_t SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//恢复合成
uint8_t SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//状态查询
uint8_t SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//进入POWER DOWN 状态命令

/***********************************************/

/*===================================================================
*Name W_StringLen()
*Function 双字节数符串长度
*input 字符串指针
*output 字符串长度
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
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
            *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata)//,uint8_t len)

{
/****************需要发送的文本**********************************/ 
		 unsigned  char  Frame_Info[50];
     unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//定义校验字节
	   unsigned  int i=0; 
		 //HZ_Length =strlen((char*)HZdata); 			//需要发送文本的长度
		 HZ_Length=2*W_StringLen((char *)HZdata); 			//需要发送文本的长度 modify by seleen 2018-8-16
	   //HZ_Length=len;
 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01 | Music;//<<4 ;  //构造命令参数：背景音乐设定

/*******************校验码计算***************************************/		 
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	     {  
	         ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验		
	     }		 
/*******************发送帧信息***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  //PrintCom(Frame_Info,5+HZ_Length+1);
			//comSendBuf(COM6,Frame_Info,5+HZ_Length+1); 
			memcpy(g_TxBuf6,Frame_Info,5+HZ_Length+1);
			Syn6288SendData(5+HZ_Length+1); 
			 
}


/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
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
Function	初始化语音芯片
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
Function	Send数据发送TXBUF
Input		数据buf 数据长度
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


