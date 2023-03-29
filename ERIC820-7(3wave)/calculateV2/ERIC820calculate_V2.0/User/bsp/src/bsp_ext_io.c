#include "bsp.h"

static void ConfirgureIO(void);
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitExtIO
*	����˵��: ������չIO��ص�GPIO. �ϵ�ֻ��ִ��һ�Ρ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitExtIO(void)
{
	ConfirgureIO();
}
/*
*********************************************************************************************************
*	�� �� ��: ConfirgureIO
*	����˵��: ������չIO��ص�GPIO. �ϵ�ֻ��ִ��һ�Ρ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfirgureIO(void)
{
	

	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/***************************����IO��ʱ��*********************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK, ENABLE);

	/*----------------------USART GPIO------------------------------------------*/
	  /* �� PA9 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

		/* �� PA10 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		
		
		
	/*----------------------USART GPIO END------------------------------------------*/

	
	
	
	/*--------------------------FMC GPIO ���� -----------------------------------------------------*/
	 /*
	 FMC ��������
	 
	 FMC_D0 <-> PD14	 FMC_D1 <-> PD15
	 FMC_D2 <-> PD0	   FMC_D3 <-> PD1
	 FMC_D4 <-> PE7	   FMC_D5 <-> PE8
	 FMC_D6 <-> PE9	   FMC_D7 <-> PE10
	 
	 FMC_D8 <-> PE11	 FMC_D9 <-> PE12
	 FMC_D10 <-> PE13	 FMC_D11 <-> PE14
	 FMC_D12 <-> PE15	 FMC_D13 <-> PD8
	 FMC_D14 <-> PD9	 FMC_D15 <-> PD10	 
	 
	 FMC_A0 <-> PF0    FMC_A1 <-> PF1
	 FMC_A2 <-> PF2    FMC_A3 <-> PF3
	 FMC_A4 <-> PF4    FMC_A5 <-> PF5
	 FMC_A6 <-> PF12   FMC_A7 <-> PF13
	 FMC_A8 <-> PF14   FMC_A9 <-> PF15
	 FMC_A10 <-> PG0   FMC_A11 <-> PG1
	 FMC_A12 <-> PG2
	 
	*/
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0  |GPIO_Pin_1  |
															GPIO_Pin_8 |GPIO_Pin_9   |
	                            GPIO_Pin_10 |GPIO_Pin_14 |
															GPIO_Pin_15;

	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_7  |GPIO_Pin_8  |
															GPIO_Pin_9  |GPIO_Pin_10  |
															GPIO_Pin_11  |GPIO_Pin_12  |
															GPIO_Pin_13  |GPIO_Pin_14  |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);

	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0  |	GPIO_Pin_1  |
															GPIO_Pin_2  | GPIO_Pin_3  |
															GPIO_Pin_4  |	GPIO_Pin_5  |
															GPIO_Pin_12  |	GPIO_Pin_13 |
															GPIO_Pin_14  | GPIO_Pin_15;															
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FMC);

	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0  |	GPIO_Pin_1 | GPIO_Pin_2;														
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	
	/*
		SDRAM
		
		FMC_A14/BA0	<-> PG4  FMC_A15/BA1	<-> PG5		
		FMC_SDCKE0	<-> PH2  FMC_SDCLK	<-> PG8
		FMC_SDNWE 	<-> PH5  FMC_SDNE0 	<-> PH3
		FMC_SDNCAS	<-> PG15 FMC_SDNRAS	<-> PF11
		FMC_NBL0 <-> 	PE0  FMC_NBL1 <-> 	PE1
		FMC_NBL2 <-> 	PI4  FMC_NBL3 <-> 	PI5
	*/
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_11;

	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4  |GPIO_Pin_5  |
															GPIO_Pin_8  |GPIO_Pin_15;

	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource5, GPIO_AF_FMC);

	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2  |
															GPIO_Pin_3  |GPIO_Pin_5;

	GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	//GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_FMC);
	//GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_FMC);
	
	//GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;

	//GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	/*
	AD7606
	
	AD_RANGE <-> PG10      AD_CONVST <-> PC6
	AD_RESET <-> PG7			 AD7606_BUSY <-> PB1
	
	FMC_NE4 <-> PG12       FMC_NWE <-> PD5
	FMC_NOE <-> PD4				 FMC_SDCLK <-> PG8
	*/
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_12;

	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;

	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
/*---------------------------FMC����豸IO END-----------------------------------*/	

/*---------------------------I2C GPIO ����---------------------------------------*/

	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	

/*---------------------------I2C GPIO ���� END------------------------------------*/

/*---------------------------SPI GPIO ����-------------------------------------------*/

	/*
	SPI2_NSS<->PB12
	SPI2_SCK<->PB10
	SPI2_MOSI<->PC3
	SPI2_MISO<->PC2
	
	SPI3_NSS<->PA4
	SPI3_SCK<->PB3
	SPI3_MOSI<->PD6
	SPI3_MISO<->PB4
	
	SPI4_NSS<->PE4
	SPI4_SCK<->PE2
	SPI4_MOSI<->PE6
	SPI4_MISO<->PE5
	*/



	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	


	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);	

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);

#if 1
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);	


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);	


	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF5_SPI3);	

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif


	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOE, &GPIO_InitStruct);	


	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_UP;	
	GPIO_Init(GPIOE, &GPIO_InitStruct);


/*---------------------------SPI GPIO ���� END---------------------------------------*/




/*---------------------------	ģ��ͨѶ GPIO ����-----------------------------------------------*/
/*
PA2 <-> USART2_TX
PA3 <-> USART2_RX

PF6 <-> UART7_RX
PF7 <-> UART7_TX
*/

	/* �� PA2 ӳ��Ϊ USART2_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	/* �� PA3 ӳ��Ϊ USART2_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* ���� USART Tx RxΪ���ù��� */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	/* �� PF7 ӳ��Ϊ UART7_TX */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_UART7);

	/* �� PF6 ӳ��Ϊ UART7_RX */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_UART7);

	/* ���� USART Tx RxΪ���ù��� */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);

/*---------------------------	ģ��ͨѶ GPIO ���� END-----------------------------------------------*/


/*--------------------------- SD GPIO ����-----------------------------------------------*/
/*
PC12 <-> SDIO-CK
PD2 <-> SDIO-CMD
PC8 <-> SDIO-D0
PC9 <-> SDIO-D1
PC10 <-> SDIO-D2
PC11 <-> SDIO-D3
*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);


	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //| GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
/*--------------------------- SD GPIO ���� END-------------------------------------------*/
/*---------------------------RS485 GPIO ����---------------------------------------*/
	/*
	PH15 <-> EN
	PI0 <-> RX
	PI1 <-> TX
	*/

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOI, &GPIO_InitStruct);
	
/*---------------------------RS485 GPIO ���� END---------------------------------------*/
/*---------------------------CAN GPIO ����---------------------------------------*/
	/*
	PA12 <-> CAN1_TX
	PI9  <-> CAN1_RX	
	*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_CAN1);


	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOI, &GPIO_InitStruct);
/*---------------------------CAN GPIO ���� END---------------------------------------*/
/*-------------------------IO GPIO����--------------------------------------------------*/
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOG, &GPIO_InitStruct);


	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOH, &GPIO_InitStruct);	
	GPIO_ResetBits(GPIOH,GPIO_Pin_9);// 0
	

	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOI, &GPIO_InitStruct);	
	
/*-------------------------IO GPIO���� END--------------------------------------------------*/
}//void

/***************************** END *********************************/
