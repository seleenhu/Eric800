/*
*********************************************************************************************************
*	                                  
*	ģ������ : CAN������ʾ����
*	�ļ����� : can_network.c
*	��    �� : V1.3
*	˵    �� : ��ʾ���ʵ�ֶ��CAN�ڵ㣨�ڵ�ĳ�����ͬ��֮���ͨ�š�
*	�޸ļ�¼ :
*		�汾��  ����       ����      ˵��
*		v1.3    2015-01-29 Eric2013  �׷�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "mydefine.h"
#if 0
/* CAN1�����ţ�ʱ�Ӷ��� */
#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_0
#define CAN_TX_PIN                 GPIO_Pin_1
#define CAN_GPIO_TX_PORT           GPIOD
#define CAN_GPIO_TX_CLK            RCC_AHB1Periph_GPIOD
#define CAN_GPIO_RX_PORT           GPIOD
#define CAN_GPIO_RX_CLK            RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT                GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource0
#define CAN_TX_SOURCE              GPIO_PinSource1

/* ����ȫ�ֱ��� */
CanTxMsg g_tCanTxMsg;	/* ���ڷ��� */
CanRxMsg g_tCanRxMsg;	/* ���ڽ��� */
#endif


static stc_can_rxframe_t       stcRxFrame;
static stc_can_txframe_t       stcTxFrame;
AlarmTimStruct SystemTime;	//��ʱ����Ϊ�˱���
static uint8_t ptbTXFlag=false;

uint16_t CanDelayTim;

uint16_t SaveFaultPt;

//uint8_t CanBreathTimes;

#define iCanBreathTime	4000

void CAN_RxIrqCallBack(void);

/*
*********************************************************************************************************
*	�� �� ��: SendCanMsg
*	����˵��: ����һ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t SendCanMsg(uint16_t iddata,uint8_t *buf,uint8_t buf_len)
{
	uint8_t rt;
	/* ��CAN���緢��һ������, ��������1���ֽ�ָLED����� */
	
	stcTxFrame.StdID = iddata;
	
	
	//stcTxFrame.ExtID = 0x01;
	/*
	stcTxFrame.Control_f.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	*/
	stcTxFrame.Control_f.DLC = buf_len;
	memcpy(stcTxFrame.Data,buf,buf_len);
	CAN_SetFrame(&stcTxFrame);
	
	
  rt=CAN_TransmitCmd(CanPTBTxCmd);

  CAN_IrqCmd(CanRxIrqEn, Enable);
	/*
	rt = CAN_Transmit(CAN1, &g_tCanTxMsg);	
	*/
	return rt;
}

/*
*********************************************************************************************************
*	�� �� ��: SendCanMsg
*	����˵��: ����һ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t SendCan1Msg(uint16_t iddata,uint8_t *buf,uint8_t buf_len)
{
	uint8_t rt=0;
	/* ��CAN���緢��һ������, ��������1���ֽ�ָLED����� */
#if 0
	g_tCanTxMsg.StdId = iddata;
	g_tCanTxMsg.ExtId = 0x01;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.DLC = buf_len;
	memcpy(g_tCanTxMsg.Data,buf,buf_len);
	rt = myCAN_Transmit(CAN1, &g_tCanTxMsg);
	
	return rt;
	#endif
	
	stcTxFrame.StdID = iddata;
	//stcTxFrame.ExtID = 0x01;
	
	/*
	stcTxFrame.Control_f.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	*/
	stcTxFrame.Control_f.DLC = buf_len;
	memcpy(stcTxFrame.Data,buf,buf_len);
	CAN_SetFrame(&stcTxFrame);
	
	
  CAN_TransmitCmd(CanPTBTxCmd);

  CAN_IrqCmd(CanRxIrqEn, Enable);
	
	/*
	rt = CAN_Transmit(CAN1, &g_tCanTxMsg);	
	*/
	return rt;
	
}
/*
*********************************************************************************************************
*	�� �� ��: SendCanMsg
*	����˵��: ����һ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t SendCanbuf(uint16_t iddata,uint8_t *buf,uint16_t buf_len)
{
	uint8_t rt;
	uint16_t i;
	uint32_t timeoutcnt;
	
	//uint16_t DelayTim;
	i = 0;
	rt = 0;
	while(buf_len>8){
		//DelayTim = 0;
		/*
		do{
			rt = SendCan1Msg(iddata,buf+i*8,8);		
		//	IWDG_ReloadCounter();
			//DelayTim++;
		}while(0 != rt);		
		*/
		SendCan1Msg(iddata,buf+i*8,8);
		//========wait CAN send complete
		timeoutcnt=0;
		while(ptbTXFlag==false){
			timeoutcnt++;
			if(timeoutcnt>200000)
				break;
		}
		ptbTXFlag=false;	
		buf_len-=8;
		i++;
	}
	if((buf_len)&&(0 == rt)){
		/*
		do{
			rt = SendCan1Msg(iddata,buf+i*8,buf_len);		
			//IWDG_ReloadCounter();			
		}while(0 != rt);
		*/
		SendCan1Msg(iddata,buf+i*8,buf_len);
		//==========wait CAN send complete
		timeoutcnt=0;
		while(ptbTXFlag==false){
			timeoutcnt++;
			if(timeoutcnt>200000)
				break;
			
		}
		ptbTXFlag=false;	
	}
	return rt;

/*


#if 1
	uint8_t rt;
	uint16_t i;
	i = 0;
	while(buf_len>8){
		
		SendCanMsg(iddata,buf+i*8,8);
		do{
			rt = CAN_TransmitStatus(CAN1,0);
			IWDG_ReloadCounter();
		}while(rt != CAN_TxStatus_Ok);
		
		buf_len-=8;
		i++;
	}
	if(buf_len){
		do{
			rt = CAN_TransmitStatus(CAN1,0);
			IWDG_ReloadCounter();
		}while(rt != CAN_TxStatus_Ok);
		SendCanMsg(iddata,buf+i*8,buf_len);
	}
#else
	uint16_t i,Delay;
	i = 0;
	while(buf_len>8){		
		SendCanMsg(iddata,buf+i*8,8);
		buf_len-=8;
		i++;
		if((buf_len>8)||buf_len){
			for(Delay=0;Delay<100;Delay++){
				IWDG_ReloadCounter();
			}
		}
	}
	if(buf_len){
		SendCanMsg(iddata,buf+i*8,buf_len);
	}
#endif
*/
	
}


/*
*********************************************************************************************************
*	�� �� ��: can_Init
*	����˵��: ����CANӲ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void can_Init(void)
{
	#ifdef stm32f407
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* CAN GPIOs ����*/
	/* ʹ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_TX_CLK|CAN_GPIO_RX_CLK, ENABLE);

	/* ����ӳ��ΪCAN����  */
	GPIO_PinAFConfig(CAN_GPIO_RX_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_GPIO_TX_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 

	/* ���� CAN RX �� TX ���� */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN_GPIO_RX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_Init(CAN_GPIO_TX_PORT, &GPIO_InitStructure);

	/* CAN����*/  
	/* ʹ��CANʱ�� */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/* ��λCAN�Ĵ��� */
	CAN_DeInit(CANx);
	
	/*
		TTCM = time triggered communication mode
		ABOM = automatic bus-off management 
		AWUM = automatic wake-up mode
		NART = no automatic retransmission
		RFLM = receive FIFO locked mode 
		TXFP = transmit FIFO priority		
	*/
	CAN_InitStructure.CAN_TTCM = DISABLE;			/* ��ֹʱ�䴥��ģʽ��������ʱ���), T  */
	CAN_InitStructure.CAN_ABOM = DISABLE;			/* ��ֹ�Զ����߹رչ��� */
	CAN_InitStructure.CAN_AWUM = DISABLE;			/* ��ֹ�Զ�����ģʽ */
	CAN_InitStructure.CAN_NART = DISABLE;			/* ��ֹ�ٲö�ʧ��������Զ��ش����� */
	CAN_InitStructure.CAN_RFLM = DISABLE;			/* ��ֹ����FIFO����ģʽ */
	CAN_InitStructure.CAN_TXFP = DISABLE;			/* ��ֹ����FIFO���ȼ� */
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	/* ����CANΪ��������ģʽ */
	
	/* 
		CAN ������ = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);
		
		SJW = synchronisation_jump_width 
		BS = bit_segment
		
		�����У�����CAN������Ϊ200Kbps		
		CAN ������ = 420000000 / 42 / (1 + 3 + 1) / = 200K		
	*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
	CAN_InitStructure.CAN_Prescaler = 42;
	CAN_Init(CANx, &CAN_InitStructure);
	
	/* ����CAN�˲���0 */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		/* �˲�����ţ�0-13����14���˲��� */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		/* �˲���ģʽ������ID����ģʽ */
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	/* 32λ�˲� */
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					/* �����ID�ĸ�16bit */
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					/* �����ID�ĵ�16bit */
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				/* ID����ֵ��16bit */
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x00000;				/* ID����ֵ��16bit */
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		/* �˲�����FIFO 0 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				/* ʹ���˲��� */
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* ��䷢�Ͳ����� Ҳ������ÿ�η��͵�ʱ���� */
	g_tCanTxMsg.StdId = 0x321;
	g_tCanTxMsg.ExtId = 0x01;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.DLC = 1;
	CanComData.RxbufPt = 0;
	CanComData.RxbufBt = 0;
	CanComData.SetRxbufBt = 0;
	CanComData.SetRxbufPt = 0;
	#endif
	
	stc_can_init_config_t   stcCanInitCfg;
  stc_can_filter_t        stcFilter;
  
	
  stc_irq_regi_conf_t     stcIrqRegiConf;
	
	MEM_ZERO_STRUCT(stcCanInitCfg);
  MEM_ZERO_STRUCT(stcFilter);
  MEM_ZERO_STRUCT(stcTxFrame);
  MEM_ZERO_STRUCT(stcRxFrame);
	
//<<Enable can peripheral clock and buffer(ram)
  PWC_RamOpMdConfig(HighSpeedMd);
  PWC_RamPwrdownCmd(PWC_RAMPWRDOWN_CAN, Enable);
  PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_CAN, Enable);
		
//<<CAN GPIO config
  PORT_SetFunc(PortB, Pin12, Func_Can1_Rx, Disable);
  PORT_SetFunc(PortB, Pin13, Func_Can1_Tx, Disable);
  //PORT_ResetBits(PortD, Pin15);
  //PORT_OE(PortD, Pin15, Enable);
//<<Can bit time config
  stcCanInitCfg.stcCanBt.PRESC = 5u-1u;//1u-1u;
  stcCanInitCfg.stcCanBt.SEG_1 = 5u-2u;
  stcCanInitCfg.stcCanBt.SEG_2 = 3u-1u;
  stcCanInitCfg.stcCanBt.SJW   = 3u-1u;

  stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10u;
  stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16u-1u;

  stcCanInitCfg.enCanRxBufAll  = CanRxNormal;
  stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;
  stcCanInitCfg.enCanSAck      = CanSelfAckEnable;
  stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;

  CAN_Init(&stcCanInitCfg);
	
	//<<Can filter config
  stcFilter.enAcfFormat = CanAllFrames;
  stcFilter.enFilterSel = CanFilterSel1;
  stcFilter.u32CODE     = 0x00000000u;	/*�����ID*/
  stcFilter.u32MASK     = 0x1FFFFFFFu;
	
	//stcFilter.u32MASK     = 0x00000000u;	/*ID����ֵ*/
  CAN_FilterConfig(&stcFilter, Enable);
	
		
	//<<Can Irq Enable
  CAN_IrqCmd(CanRxIrqEn, Enable);

  stcIrqRegiConf.enIRQn = Int000_IRQn;
  stcIrqRegiConf.enIntSrc = INT_CAN_INT;
  stcIrqRegiConf.pfnCallback = &CAN_RxIrqCallBack;
  enIrqRegistration(&stcIrqRegiConf);
  NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
  NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
  NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	CAN_IrqCmd(CanTxPrimaryIrqEn,Enable);
}     
#if 0
/*
*********************************************************************************************************
*	�� �� ��: can_NVIC_Config
*	����˵��: ����CAN�ж�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/  
void can_NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* CAN FIFO0 ��Ϣ�����ж�ʹ�� */ 
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	
}
#endif

#if 0
/*
*********************************************************************************************************
*	�� �� ��: can_ISR
*	����˵��: CAN�жϷ������. ��������� stm32f4xx_it.c�б�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void can_ISR(void)
{
	uint8_t i;
	CAN_Receive(CANx, CAN_FIFO0, &g_tCanRxMsg);
	if(g_tCanRxMsg.IDE == CAN_ID_STD){		
		if(iCanMainID == g_tCanRxMsg.StdId){
			for(i=0;i<g_tCanRxMsg.DLC;i++){
				CanComData.Rxbuf[CanComData.RxbufPt] = g_tCanRxMsg.Data[i];
				CanComData.RxbufPt = FIFO_pointer(CanComData.RxbufPt, 1, iCanRxSize);
			}
		
		}else if(iCanSetID == g_tCanRxMsg.StdId){
			for(i=0;i<g_tCanRxMsg.DLC;i++){
				CanComData.SetRxbuf[CanComData.SetRxbufPt] = g_tCanRxMsg.Data[i];
				CanComData.SetRxbufPt = FIFO_pointer(CanComData.SetRxbufPt, 1, iCanRxSize);
			}
		
		}
		
	}
}//can_ISR
#endif
/*
*********************************************************************************************************
*	�� �� ��: CAN_RxIrqCallBack
*	����˵��: CAN�жϷ������ص�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 


void CAN_RxIrqCallBack(void)
{
	 uint8_t i;
    if(true == CAN_IrqFlgGet(CanRxIrqFlg))
    {
        CAN_IrqFlgClr(CanRxIrqFlg);
        //CAN_IrqCmd(CanRxIrqEn, Disable);

        CAN_Receive(&stcRxFrame);
			
		if(iCanMainID == stcRxFrame.StdID){
			for(i=0;i<stcRxFrame.Cst.Control_f.DLC;i++){
				CanComData.Rxbuf[CanComData.RxbufPt] = stcRxFrame.Data[i];
				CanComData.RxbufPt = FIFO_pointer(CanComData.RxbufPt, 1, iCanRxSize);
			}
		
		}else if(iCanSetID == stcRxFrame.StdID){ 
			for(i=0;i<stcRxFrame.Cst.Control_f.DLC;i++){
				CanComData.SetRxbuf[CanComData.SetRxbufPt] = stcRxFrame.Data[i];
				CanComData.SetRxbufPt = FIFO_pointer(CanComData.SetRxbufPt, 1, iCanRxSize);
			}
		
		}
				

       // u8RxFlag = true;
    }
		
	if(true == CAN_IrqFlgGet(CanTxPrimaryIrqFlg))
    {
        CAN_IrqFlgClr(CanTxPrimaryIrqFlg);
        ptbTXFlag = true;
    }	

}
/*
*********************************************************************************************************
*	�� �� ��: can_Task
*	����˵��: CAN������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void can_Task(void)
{
	uint16_t i,bt,data16;
	uint8_t flag;
	static uint8_t step;
	static uint16_t len;
	switch(step){
		case 0:	
			flag = 0;
			while(FIFO_length(CanComData.RxbufBt, CanComData.RxbufPt, iCanRxSize)>=2){
				bt = CanComData.RxbufBt;
				data16 = CanComData.Rxbuf[bt];
				bt = FIFO_pointer(bt, 1, iCanRxSize);
				data16 |= (CanComData.Rxbuf[bt]<<8);
				if(iCanComHad == data16){
					step++;
					CanDelayTim = 0;
					flag = 1;
					break;
				}else{
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, 1, iCanRxSize);
				}
			}
			if(0 == flag){
				break;
			}
		case 1:
			flag = 0;
			if(FIFO_length(CanComData.RxbufBt, CanComData.RxbufPt, iCanRxSize)>=5){
				bt = FIFO_pointer(CanComData.RxbufBt, 3, iCanRxSize);
				len = CanComData.Rxbuf[bt];
				bt = FIFO_pointer(bt, 1, iCanRxSize);
				len |= (CanComData.Rxbuf[bt]<<8);
				if(len < 32){
					CanDelayTim = 0;
					step++;
					flag = 1;
				}else{
					step = 0;
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, 1, iCanRxSize);
				}
				
			}else{				
				if(CanDelayTim>=50){					
					step = 0;
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, 1, iCanRxSize);
				}
			}
			if(0 == flag){
				break;
			}
		case 2:
			if(FIFO_length(CanComData.RxbufBt, CanComData.RxbufPt, iCanRxSize)>=len+7){
				bt = CanComData.RxbufBt;
				for(i=0;i<len+7;i++){
					CanComData.Displaybuf[i] = CanComData.Rxbuf[bt];
					bt = FIFO_pointer(bt, 1, iCanRxSize);
				}
				data16 = CanComData.Displaybuf[len+6];
				data16 <<= 8;
				data16 += CanComData.Displaybuf[len+5];
				bt = CRC_GetModbus16(CanComData.Displaybuf, len+5);
				if(data16 == bt){
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, len+7, iCanRxSize);
					len = 0;
					//flag = 0;
					switch(CanComData.Displaybuf[2]){
						case 0x60:							
							//CanComData.Faultbt = AD_pt;
						  SaveFaultPt=AD_pt;
							//Test1Set();
							CanComData.FaultFlag = 1;
							CanComData.CanSendFlag &= ~0x01;
							CanComData.CanWaveSampleFlag = 0;
							CanComData.OnlineTim = iCanBreathTime;
							ReadRtcTime();
							CanComData.FaultTim = SystemTime;
							
							break;
						case 0x61:							
							break;
						case 0x62:
							if(CanComData.Displaybuf[5]==(EepromData.CanID&0x0FF)){
								//flag = 1;
								/*
								CanComData.Txbuf[len++] = (uint8_t)iCanComHad;
								CanComData.Txbuf[len++] = iCanComHad>>8;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[2]|0x80;
								len++;
								len++;								
								memcpy(CanComData.Txbuf+len,(uint8_t *)CanComData.FaultWave,256*12*2);
								len	+=	256*12*2;
								CanComData.Txbuf[3] = (len-5);
								CanComData.Txbuf[4] = (len-5)>>8;
								bt = CRC_GetModbus16(CanComData.Txbuf, len);
								CanComData.Txbuf[len++] = bt;
								CanComData.Txbuf[len++] = bt>>8;
								*/
								CanComData.CanSendFlag |= 0x01;
								CanComData.OnlineTim = iCanBreathTime;
							}
							break;
						case 0x63:
#if 0
							memcpy((uint8_t *)&SystemTime,CanComData.Displaybuf+5,sizeof(AlarmTimStruct));
							My_RTC_Init(1);
							CanComData.Txbuf[len++] = (uint8_t)iCanComHad;
							CanComData.Txbuf[len++] = iCanComHad>>8;
							CanComData.Txbuf[len++] = CanComData.Displaybuf[2]|0x80;
							len++;
							len++;
							CanComData.Txbuf[3] = (len-5);
							CanComData.Txbuf[4] = (len-5)>>8;
							bt = CRC_GetModbus16(CanComData.Txbuf, len);
							CanComData.Txbuf[len++] = bt;
							CanComData.Txbuf[len++] = bt>>8;
							CanComData.OnlineFlag = 1;
							bsp_LedOn(iGreenLed);
							bsp_LedOff(iRedLed);
							//CanBreathTimes = 0;
							CanComData.OnlineTim = 12000;
#endif							
							break;
						case 0x64:
							if((EepromData.CanID&0x00FF) ==  CanComData.Displaybuf[5]){
								memcpy((uint8_t *)&SystemTime,CanComData.Displaybuf+6,sizeof(AlarmTimStruct));
								
								//My_RTC_Init(1);
								SetRtcTime();
								
								//CanComData.OnlineFlag = 1;
								GreenLedOn();
								RedLedOff();
								
								//CanBreathTimes = 0;
								CanComData.OnlineTim = iCanBreathTime;
							}
							break;
						case 0x65:
							if(CanComData.CanSendFlag & 0x01){
								CanComData.CanSendWaveBag = CanComData.Displaybuf[5];
							}
							CanComData.OnlineTim = iCanBreathTime;
							break;
						default:
							len = 0;							
					}
					if((len)&&(0 == CanComData.CanSendFlag)){						
						SendCanbuf(EepromData.CanID,CanComData.Txbuf,len);						
					}
				}else{
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, 1, iCanRxSize);
				}
				step = 0;
			}else{				
				if(CanDelayTim>=50){					
					step = 0;
					CanComData.RxbufBt = FIFO_pointer(CanComData.RxbufBt, 1, iCanRxSize);
				}
			}
			break;
		default:
			step = 0;
	}

}//can_Task
/*
*********************************************************************************************************
*	�� �� ��: can_Task
*	����˵��: CAN������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void can_SetTask(void)
{
	uint16_t i,bt,data16;
	//uint8_t *bt8;
	static uint8_t step,DelayTim;
	static uint16_t len;
	switch(step){
		case 0:	
			
			while(FIFO_length(CanComData.SetRxbufBt, CanComData.SetRxbufPt, iCanRxSize)>=2){
				bt = CanComData.SetRxbufBt;
				data16 = CanComData.SetRxbuf[bt];
				bt = FIFO_pointer(bt, 1, iCanRxSize);
				data16 |= (CanComData.SetRxbuf[bt]<<8);
				if(iCanSetComHad == data16){
					step++;
					DelayTim = 0;
					break;
				}else{
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, 1, iCanRxSize);
				}
			}
			break;
		case 1:
			if(FIFO_length(CanComData.SetRxbufBt, CanComData.SetRxbufPt, iCanRxSize)>=5){
				bt = FIFO_pointer(CanComData.SetRxbufBt, 3, iCanRxSize);
				len = CanComData.SetRxbuf[bt];
				bt = FIFO_pointer(bt, 1, iCanRxSize);
				len |= (CanComData.SetRxbuf[bt]<<8);
				if(len < 32){
					DelayTim = 0;
					step++;
				}else{
					step = 0;
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, 1, iCanRxSize);
				}
				
			}else{
				DelayTim++;
				if(DelayTim>=50){
					DelayTim = 0;
					step = 0;
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, 1, iCanRxSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(CanComData.SetRxbufBt, CanComData.SetRxbufPt, iCanRxSize)>=len+7){
				bt = CanComData.SetRxbufBt;
				for(i=0;i<len+7;i++){
					CanComData.Displaybuf[i] = CanComData.SetRxbuf[bt];
					bt = FIFO_pointer(bt, 1, iCanRxSize);
				}
				data16 = CanComData.Displaybuf[len+6];
				data16 <<= 8;
				data16 += CanComData.Displaybuf[len+5];
				bt = CRC_GetModbus16(CanComData.Displaybuf, len+5);
				if(data16 == bt){
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, len+7, iCanRxSize);
					switch(CanComData.Displaybuf[2]){
						case 0x60:
							if(1 == CanComData.Displaybuf[5]){
								
								VerifyFlag = 1;
								
								len = 0;
								CanComData.Txbuf[len++] = (uint8_t)iCanSetComHad;
								CanComData.Txbuf[len++] = iCanSetComHad>>8;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[2] | 0x80;
								len++;
								len++;
								CanComData.Txbuf[len++] = 1;
								CanComData.Txbuf[3] = (len-5);
								CanComData.Txbuf[4] = (len-5)>>8;
								bt = CRC_GetModbus16(CanComData.Txbuf, len);
								CanComData.Txbuf[len++] = bt;
								CanComData.Txbuf[len++] = bt>>8;
							}
							break;
						case 0x61:
							if(1 == CanComData.Displaybuf[5]){
								
								len = 0;
								CanComData.Txbuf[len++] = (uint8_t)iCanSetComHad;
								CanComData.Txbuf[len++] = iCanSetComHad>>8;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[2] | 0x80;
								len++;
								len++;
								memcpy(CanComData.Txbuf+len,(uint8_t *)(&ADCFifo.RMSbuf),sizeof(float));
								len += sizeof(float);							
								CanComData.Txbuf[3] = (len-5);
								CanComData.Txbuf[4] = (len-5)>>8;
								bt = CRC_GetModbus16(CanComData.Txbuf, len);
								CanComData.Txbuf[len++] = bt;
								CanComData.Txbuf[len++] = bt>>8;
							}
							break;
						case 0x62:
							if((CanComData.Displaybuf[5]>=1)&&(CanComData.Displaybuf[5]<=128)){
								EepromData.CanID = 0x300+CanComData.Displaybuf[5];
								EepromSaveFlag = 1;
								
								len = 0;
								CanComData.Txbuf[len++] = (uint8_t)iCanSetComHad;
								CanComData.Txbuf[len++] = iCanSetComHad>>8;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[2] | 0x80;
								len++;
								len++;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[5];											
								CanComData.Txbuf[3] = (len-5);
								CanComData.Txbuf[4] = (len-5)>>8;
								bt = CRC_GetModbus16(CanComData.Txbuf, len);
								CanComData.Txbuf[len++] = bt;
								CanComData.Txbuf[len++] = bt>>8;
							}
							break;
						case 0x63:
							if(1 == CanComData.Displaybuf[5]){	
								len = 0;
								CanComData.Txbuf[len++] = (uint8_t)iCanSetComHad;
								CanComData.Txbuf[len++] = iCanSetComHad>>8;
								CanComData.Txbuf[len++] = CanComData.Displaybuf[2] | 0x80;
								len++;
								len++;
								CanComData.Txbuf[len++] = EepromData.CanID;											
								CanComData.Txbuf[3] = (len-5);
								CanComData.Txbuf[4] = (len-5)>>8;
								bt = CRC_GetModbus16(CanComData.Txbuf, len);
								CanComData.Txbuf[len++] = bt;
								CanComData.Txbuf[len++] = bt>>8;
							}
							break;
						default:
							len = 0;							
					}
					if(len){
						SendCanbuf(iCanSetID,CanComData.Txbuf,len);
					}
				}else{
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, 1, iCanRxSize);
				}
				step = 0;
			}else{
				DelayTim++;
				if(DelayTim>=50){
					DelayTim = 0;
					step = 0;
					CanComData.SetRxbufBt = FIFO_pointer(CanComData.SetRxbufBt, 1, iCanRxSize);
				}
			}
			break;
		default:
			step = 0;
	}

	

}//can_Task


void CanBreath(void)
{
	uint8_t len;
	uint16_t mycrc;
	static uint16_t DelayTim;
	if(0 == CanComData.OnlineTim){
		//CanComData.OnlineFlag = 1;
		if(0 == DelayTim){
			len = 0;
			CanComData.Txbuf[len++] = (uint8_t)iCanComHad;
			CanComData.Txbuf[len++] = iCanComHad>>8;
			CanComData.Txbuf[len++] = 0x64|0x80;
			len++;
			len++;
			CanComData.Txbuf[3] = (len-5);
			CanComData.Txbuf[4] = (len-5)>>8;
			mycrc = CRC_GetModbus16(CanComData.Txbuf, len);
			CanComData.Txbuf[len++] = mycrc;
			CanComData.Txbuf[len++] = mycrc>>8;
			
			SendCanbuf(EepromData.CanID,CanComData.Txbuf,len);
			DelayTim++;
		}else{
			DelayTim++;
			if(DelayTim >= 2000){
				DelayTim = 0;	
				GreenLedOff();
				RedLedOn();
				#ifdef Release		
				MCUSoft_RST();
				while(1);
				#endif
				//can_Init();
				
			}
		}
	}else{
		DelayTim = 0;
		CanComData.OnlineTim--;
	}		
}//CanBreath


void CanSendWave(void)
{
	uint16_t mycrc;
	static uint8_t step,bagSize,bagNum,SendTimes;
	static uint16_t len,DelayTim;
	switch(step){
		case 0:
			if((CanComData.CanSendFlag & 0x01) && CanComData.CanWaveSampleFlag){				
				step++;	
				bagSize = 21;
				bagNum = 0;
				SendTimes = 0;
			}
			if(0 == step){
				break;
			}
		case 1:			
			if(bagNum<bagSize){			
				len = 0;
				CanComData.Txbuf[len++] = (uint8_t)iCanComHad;
				CanComData.Txbuf[len++] = iCanComHad>>8;
				CanComData.Txbuf[len++] = 0xE2;
				len++;
				len++;	
				CanComData.Txbuf[len++] = bagSize;
				CanComData.Txbuf[len++] = bagNum+1;
				if(0 == bagNum){					
					memcpy(CanComData.Txbuf+len,((uint8_t *)&EepromData.ratio),sizeof(float));
					len += sizeof(float);					
					memcpy(CanComData.Txbuf+len,((uint8_t *)&CanComData.FaultTim),sizeof(AlarmTimStruct));
					len += sizeof(AlarmTimStruct);
				}else{
					memcpy(CanComData.Txbuf+len,((uint8_t *)CanComData.FaultWave)+(bagNum-1)*512,512);
					len	+=	512;
				}
				CanComData.Txbuf[3] = (len-5);
				CanComData.Txbuf[4] = (len-5)>>8;
				mycrc = CRC_GetModbus16(CanComData.Txbuf, len);
				CanComData.Txbuf[len++] = mycrc;
				CanComData.Txbuf[len++] = mycrc>>8;
				SendCanbuf(EepromData.CanID, CanComData.Txbuf, len);
				DelayTim = 0;
				CanComData.CanSendWaveBag = 0;
				step++;
			}else{
				CanComData.CanSendFlag &= ~0x01;
				step = 0;
			}
			break;
		case 2:
			if(bagNum+1 == CanComData.CanSendWaveBag){
				bagNum++;
				step--;
				SendTimes = 0;
			}else{
				DelayTim++;
				if(DelayTim>=100){					
					SendTimes++;
					if(SendTimes>=5){
						CanComData.CanSendFlag &= ~0x01;
						step = 0;
					}else{
						step--;
					}
				}
			}			
			break;
		default:
			step = 0;			
	}
}//CanSendWave
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
