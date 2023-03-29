/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.c
*	��    �� : V1.0
*	˵    �� : ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*		V1.1    2013-06-09 armfly  FiFo�ṹ����TxCount��Ա�����������жϻ�������; ���� ��FiFo�ĺ���
*		V1.2	2014-09-29 armfly  ����RS485 MODBUS�ӿڡ����յ����ֽں�ֱ��ִ�лص�������
*		V1.3	2015-07-23 armfly  ���� UART_T �ṹ�Ķ�дָ�뼸����Ա������������ __IO ����,�����Ż���
*					�ᵼ�´��ڷ��ͺ���������
*		V1.4	2015-08-04 armfly  ���UART4����bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  �����޸Ĳ����ʵĽӿں���
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* ����1��GPIO --- RS323 */
#define UART1_TX_PORT      GPIOA
#define UART1_TX_PIN       GPIO_Pin_9
#define UART1_TX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_TX_SOURCE    GPIO_PinSource9

#define UART1_RX_PORT      GPIOA
#define UART1_RX_PIN       GPIO_Pin_10
#define UART1_RX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_RX_SOURCE    GPIO_PinSource10









static void InitHardUart(void);

#define RS232bufSize   512
uint8_t RS232RXbuf[RS232bufSize];
uint16_t RS232bt;
#define  RS232Pt     (RS232bufSize-DMA_GetCurrDataCounter(DMA2_Stream5))

uint8_t RS232TXbuf[RS232bufSize];
uint16_t RS232TXbuflen;

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	//UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */

	InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

	//RS485_InitTXE();	/* ����RS485оƬ�ķ���ʹ��Ӳ��������Ϊ������� */

	//ConfigUartNVIC();	/* ���ô����ж� */
}


/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;



	/* ��1���� ����GPIO */
	#if 1	/* TX = PA9   RX = PA10 */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* �� PA9 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

		/* �� PA10 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#else	/* TX = PB6   RX = PB7  */
		/* �� GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* �� PB6 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* �� PB7 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = 9600;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	DMA_DeInit(DMA2_Stream5);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RS232RXbuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RS232bufSize;
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
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);

	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	DMA_Cmd(DMA2_Stream5, ENABLE);
	

	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */

	RS232bt = RS232Pt;


}










/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 0	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	comSendChar(COM1, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	return ch;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 0	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}
/*
*********************************************************************************************************
*	�� �� ��: RS232Task
*	����˵��: RS232������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS232Task(void)
{
	uint8_t i,flag;
	uint16_t bt,Data16;
	static uint8_t Step,DelayTime;
	static uint16_t btbackup;
	DelayTime++;
	switch(Step){
		case 0:
			while(FIFO_length(RS232bt, RS232Pt, RS232bufSize)>=2){
				if(0 == FIFO_bufcmp(RS232RXbuf, "AT", RS232bt, 2, RS232bufSize)){
					Step++;
					
					DelayTime = 0;
					btbackup = RS232bt;
					RS232bt = FIFO_pointer(RS232bt,2,RS232bufSize);
					break;
				}else{
					RS232bt = FIFO_pointer(RS232bt,1,RS232bufSize);
				}
			}			
			break;
		case 1:
			flag = 0;
			while(FIFO_length(RS232bt, RS232Pt, RS232bufSize)>=2){
				if(0 == FIFO_bufcmp(RS232RXbuf, "\r\n", RS232bt, 2, RS232bufSize)){
					Step++;
					flag = 1;
					RS232bt = FIFO_pointer(RS232bt,2,RS232bufSize);	
					break;
				}else{
					RS232bt = FIFO_pointer(RS232bt,1,RS232bufSize);
				}
			}
			if(flag){
				RS232TXbuflen = 0;
				bt = btbackup;
				while(bt != RS232bt){
					RS232TXbuf[RS232TXbuflen++] = RS232RXbuf[bt];
					bt = FIFO_pointer(bt,1,RS232bufSize);
				}
				RS232TXbuf[RS232TXbuflen++] = '\0';
				printf("%s",RS232TXbuf);
				bt = btbackup; 
				if(0 == FIFO_bufcmp(RS232RXbuf, "AT+EQUIPID=", bt, 11, RS232bufSize)){
					bt = FIFO_pointer(bt,11,RS232bufSize);
					
					while(bt != RS232bt){
						if(isalnum(RS232RXbuf[bt])){
							btbackup = bt;
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
					}
					flag = 0;
					if(FIFO_length(bt, RS232bt, RS232bufSize) >= 22){						
						for(i=0;i<20;i++){
							if(0 == isalnum(RS232RXbuf[bt])){
								break;
							}
							bt = FIFO_pointer(bt,1,RS232bufSize);
						}
						if(i == 20){
							flag = 1;
						}
						if(flag){
							bt = btbackup;
							for(i=0;i<20;i++){
								SystemSet.NetSet.ModbudID[i] = RS232RXbuf[bt];
								bt = FIFO_pointer(bt,1,RS232bufSize);
							}
							SystemSet.ParamSaveFlag |= FlagSystemStateSave;
							
						}
						
						
					}
					if(flag){
						printf("+EQUIPID:SET OK\r\n");
					}else{
						printf("+EQUIPID:SET Eorror\r\n");
					}
					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+EQUIPID?", bt, 11, RS232bufSize)){
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+EQUIPID:");
					memcpy(RS232TXbuf+RS232TXbuflen,SystemSet.NetSet.ModbudID,20);
					RS232TXbuflen += 20;
					RS232TXbuf[RS232TXbuflen++] = '\r';
					RS232TXbuf[RS232TXbuflen++] = '\n';
					RS232TXbuf[RS232TXbuflen++] = '\0';
					printf("%s",RS232TXbuf);
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+TOUCH=1", bt, 10, RS232bufSize)){
					SaveParamTouch();
					MCUSoft_RST();
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+CLRFAULT=1", bt, 10, RS232bufSize)){
					ModuleComSet13Fault(0x8000,0x01);					
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+CLRFAULT: OK\r\n");
					printf("%s",RS232TXbuf);					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+VERSION?", bt, 11, RS232bufSize)){
					RS232TXbuflen = sprintf((char *)RS232TXbuf,"+VERSION:\r\n");
					memcpy((char *)(RS232TXbuf+RS232TXbuflen),Ver,5);
					RS232TXbuflen += 5;
					RS232TXbuflen += sprintf((char *)(RS232TXbuf+RS232TXbuflen),"\r\nEdit by fanzhong\r\nDate:2019-10-11\r\n\0");
					printf("%s",RS232TXbuf);
					
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+SETNAME=", bt, 11, RS232bufSize)){
					bt = FIFO_pointer(bt,11,RS232bufSize);

					while(bt != RS232bt){
						if(isxdigit(RS232RXbuf[bt])){							
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
					}					
					i = 0;
					while(i<20*2){
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 = RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 = RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 = RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}
							Data16 <<= 4;
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						if(isxdigit(RS232RXbuf[bt])){
							if(isdigit(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'0';
							}else if(islower(RS232RXbuf[bt])){
								Data16 += RS232RXbuf[bt]-'a'+10;
							}else{
								Data16 += RS232RXbuf[bt]-'A'+10;
							}							
						}else{
							break;
						}
						bt = FIFO_pointer(bt,1,RS232bufSize);
						memcpy(SystemSet.LineNamebuf+i,&Data16,2);
						i += 2;
					}
					if(i){
						printf("+SETNAME:SET OK\r\n");
						SystemSet.LineNamebuf[i++] = 0;
						SystemSet.LineNamebuf[i++] = 0;
						SystemSet.ParamSaveFlag = FlagSystemState;
					}else{
						printf("+SETNAME:SET Eorror\r\n");
					}
				}else if(0 == FIFO_bufcmp(RS232RXbuf, "AT+TSET", bt, 7, RS232bufSize)){
					//ModuleTxLen = sprintf((char *)ModuleTxBuf,"This is test\r\n");					
					//ModuleTxStart();
					if(Messge_idle == MessgeFlag){
						MessgeFlag = Messge_Start;
					}
					//ModuleComSet10Fault(0x02,0x00,0x00);
				}
				Step = 0;
			}else if(DelayTime >= 20){
				
				RS232bt = RS232Pt;
				Step = 0;
			}			
			break;
	}
}//RS232Task


/***************************** (END OF FILE) *********************************/
