

#include "bsp.h"


#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_AF_PORT                GPIO_AF_CAN1

#define CAN_GPIO_TX_PORT           GPIOA
#define CAN_TX_PIN                 GPIO_Pin_12
#define CAN_GPIO_TX_CLK            RCC_AHB1Periph_GPIOA
#define CAN_TX_SOURCE              GPIO_PinSource12

#define CAN_GPIO_RX_PORT           GPIOI
#define CAN_RX_PIN                 GPIO_Pin_9
#define CAN_GPIO_RX_CLK            RCC_AHB1Periph_GPIOI
#define CAN_RX_SOURCE              GPIO_PinSource9




CanTxMsg g_tCanTxMsg;	
CanRxMsg g_tCanRxMsg;	
uint16_t CanRxDelayTim;
uint16_t CanReadWaveDelayTim;
uint16_t CanReadRmsDelayTim;


CANBitUnion CanBitData;

typedef struct{
	u8 FaultFlag;
	u8 CanId;         	/*BIT0 rx*/	
	AlarmTimStruct Tim;	
	float ratio;	
	float RMSValue;
	int16_t Wave[20*256];
}FaultStruct;


static void CanRxDisplay(uint8_t SID,uint8_t *buf,uint16_t buf_len);
static void CanReadWaveReply(uint8_t bagNum);
static uint8_t CanRMSCompare(void);

//static void can_Init(void);
//static void can_NVIC_Config(void);
//static void SendCanMsg(void);

void CanOnlineSet(uint8_t sid);
void CanOnlineClr(uint8_t sid);
uint8_t CanOnlineJudge(uint8_t sid);
void Select3Big(void);
void SortFormBigToSmall(void);



/*
*********************************************************************************************************
*	函 数 名: SendCanMsg
*	功能说明: 发送一包数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t SendCanMsg(uint16_t iddata,uint8_t *buf,uint8_t buf_len)
{
	uint8_t rt;
	/* 向CAN网络发送一包数据, 数据区第1个字节指LED灯序号 */

	g_tCanTxMsg.StdId = iddata;
	g_tCanTxMsg.ExtId = 0x01;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.DLC = buf_len;
	memcpy(g_tCanTxMsg.Data,buf,buf_len);
	//CAN_Transmit(CAN1, &g_tCanTxMsg);
	rt = myCAN_Transmit(CAN1, &g_tCanTxMsg);
	return rt;
}
/*
*********************************************************************************************************
*	函 数 名: SendCanMsg
*	功能说明: 发送一包数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SendCanbuf(uint16_t iddata,uint8_t *buf,uint16_t buf_len)
{
/*
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
*/

	uint8_t rt;
	uint16_t i;
	i = 0;
	while(buf_len>8){
		do{
			rt = SendCanMsg(iddata,buf+i*8,8);		
			IWDG_ReloadCounter();
		}while(0 != rt);
		
		buf_len-=8;
		i++;
	}
	if(buf_len){
		do{
			rt = SendCanMsg(iddata,buf+i*8,buf_len);		
			IWDG_ReloadCounter();
		}while(0 != rt);
		
	}
	
}
void can_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;


	RCC_AHB1PeriphClockCmd(CAN_GPIO_TX_CLK|CAN_GPIO_RX_CLK, ENABLE);


	GPIO_PinAFConfig(CAN_GPIO_RX_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_GPIO_TX_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 

	
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN_GPIO_RX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_Init(CAN_GPIO_TX_PORT, &GPIO_InitStructure);

	
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	
	CAN_DeInit(CANx);
	
	/*
		TTCM = time triggered communication mode
		ABOM = automatic bus-off management 
		AWUM = automatic wake-up mode
		NART = no automatic retransmission
		RFLM = receive FIFO locked mode 
		TXFP = transmit FIFO priority		
	*/
	CAN_InitStructure.CAN_TTCM = DISABLE;			/* ???1ê±??′￥·￠?￡ê?￡¨2?éú3éê±??′á), T  */
	CAN_InitStructure.CAN_ABOM = DISABLE;			/* ???1×??ˉ×ü??1?±?1üàí */
	CAN_InitStructure.CAN_AWUM = DISABLE;			/* ???1×??ˉ??D??￡ê? */
	CAN_InitStructure.CAN_NART = DISABLE;			/* ???1?ù2??aê§?ò3?′íoóμ?×??ˉ??′?1|?ü */
	CAN_InitStructure.CAN_RFLM = DISABLE;			/* ???1?óê?FIFO?ó???￡ê? */
	CAN_InitStructure.CAN_TXFP = DISABLE;			/* ???1′?ê?FIFOó??è?? */
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	/* éè??CAN?a?y3￡1¤×÷?￡ê? */
	/* 
		CAN 波特率 = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);
		
		SJW = synchronisation_jump_width 
		BS = bit_segment
		
		本例中，设置CAN波特率为200Kbps		
		CAN 波特率 = 420000000 / 42 / (1 + 3 + 1) / = 200K		
	*/
	
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
	CAN_InitStructure.CAN_Prescaler = 42;
	CAN_Init(CANx, &CAN_InitStructure);
	
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		/* ??2¨?÷Dòo?￡?0-13￡?1214????2¨?÷ */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		/* ??2¨?÷?￡ê?￡?éè??ID?ú???￡ê? */
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	/* 32????2¨ */
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					/* ?ú??oóIDμ???16bit */
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					/* ?ú??oóIDμ?μí16bit */
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				/* ID?ú???μ??16bit */
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				/* ID?ú???μμí16bit */
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		/* ??2¨?÷°ó?¨FIFO 0 */
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				/* ê1?ü??2¨?÷ */
	CAN_FilterInit(&CAN_FilterInitStructure);

	
	g_tCanTxMsg.StdId = 0x321;
	g_tCanTxMsg.ExtId = 0x01;
	g_tCanTxMsg.RTR = CAN_RTR_DATA;
	g_tCanTxMsg.IDE = CAN_ID_STD;
	g_tCanTxMsg.DLC = 8;
}     


void can_NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	
}


void can_ISR(void)
{
	uint8_t i,j;
	
	CAN_Receive(CANx, CAN_FIFO0, &g_tCanRxMsg);

#if	0
	if ((g_tCanRxMsg.StdId == 0x321) && (g_tCanRxMsg.IDE == CAN_ID_STD))
	{
		
	}
#else	
	if(g_tCanRxMsg.IDE == CAN_ID_STD){
		for(i=0;i<iCanNum;i++){
			if((iCanMainId+i+1) == g_tCanRxMsg.StdId){
				for(j=0;j<g_tCanRxMsg.DLC;j++){
					Membuf->CalRxbuf[i][Membuf->CalRxbufPt[i]] = g_tCanRxMsg.Data[j];
					Membuf->CalRxbufPt[i] = FIFO_pointer(Membuf->CalRxbufPt[i], 1, iCanbufSize);
				}
			}
		}		
	}
#endif
	
}

void CAN1_RX0_IRQHandler(void)
{
	can_ISR();
}

void CAN1_Task(void)
{
	uint8_t flag;
	uint16_t j,bt,data16,mycrc;
	static uint8_t step,i;
	static uint16_t len;
	switch(step){
		case 0:		
			for(i=0;i<iCanNum;i++){
				flag = 0;
				while(FIFO_length(Membuf->CalRxbufBt[i], Membuf->CalRxbufPt[i], iCanbufSize)>=2){
					bt = Membuf->CalRxbufBt[i];
					data16 = Membuf->CalRxbuf[i][bt];
					bt = FIFO_pointer(bt, 1, iCanbufSize);
					data16 += (Membuf->CalRxbuf[i][bt]<<8);
					if(iCanHead == data16){
						flag = 1;
						break;
					}else{
						Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], 1, iCanbufSize);
					}
					 
				}
				if(flag){
					step++;
					CanRxDelayTim = 0;
					break;
				}
			}
			break;
		case 1:
			if(FIFO_length(Membuf->CalRxbufBt[i], Membuf->CalRxbufPt[i], iCanbufSize)>=5){
				bt = FIFO_pointer(Membuf->CalRxbufBt[i], 3, iCanbufSize);
				len = Membuf->CalRxbuf[i][bt];
				bt = FIFO_pointer(bt, 1, iCanbufSize);	
				len += (Membuf->CalRxbuf[i][bt]<<8);
				if(len < iCanMaxBagSize){
					step++;
					CanRxDelayTim = 0;
				}else{
					step = 0;
					Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], 1, iCanbufSize);
				}
			}else{				
				if(CanRxDelayTim>=50){					
					step = 0;
					Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], 1, iCanbufSize);
				}
			}
			break;
		case 2:
			if(FIFO_length(Membuf->CalRxbufBt[i], Membuf->CalRxbufPt[i], iCanbufSize)>=len+7){
				bt = Membuf->CalRxbufBt[i];
				for(j=0;j<len+7;j++){					
					Membuf->CalWhole[j] = Membuf->CalRxbuf[i][bt];
					bt = FIFO_pointer(bt, 1, iCanbufSize);
				}
				data16 = Membuf->CalWhole[len+6];
				data16 <<= 8;
				data16 += Membuf->CalWhole[len+5];
				mycrc = CRC_GetModbus16(Membuf->CalWhole,len+5);
				if(data16 == mycrc){
					Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], len+7, iCanbufSize);
					CanRxDisplay(i,Membuf->CalWhole, len+7);
				}else{					
					Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], 1, iCanbufSize);				
				}
				step = 0;
			}else{				
				if(CanRxDelayTim>=50){					
					step = 0;
					Membuf->CalRxbufBt[i] = FIFO_pointer(Membuf->CalRxbufBt[i], 1, iCanbufSize);
				}
			}
			break;
		default:
			step = 0;
	}
	if(CanSampleFlag){
		CanSampleFlag = 0;
		CanRmsRxFlag = 1;
		CanSelStart();
	}

#if 1

	CanRMSCompare();

#else

	
	if(0 == CanRmsRxFlag){
		flag = 0;
		for(j=0;j<128;j++){
			if(Membuf->CalTable[j] & 0x08){
				if(Membuf->CalTable[j] & 0x02){
					flag++;
				}
			}else{
				flag++;
			}
		}
		if(flag == 128){
			CanRmsRxFlag = 1;
		}
	}
	if((Membuf->CanReadChannel >= 1)&&(Membuf->CanReadChannel <= 128)){		
		if(Membuf->CalTable[Membuf->CanReadChannel-1] & 0x08){
			CanWaveRxFlag = 0;
			if(0 == CanReadWave(Membuf->CanReadChannel)){
				CanWaveRxFlag = 1;
				Membuf->CanReadChannel = 0;

			}
		}
	}
#endif
}//CAN1_Task

void CanRxDisplay(uint8_t SID,uint8_t *buf,uint16_t buf_len)
{
	
	uint16_t len;
	
	
	switch(buf[2]){
		case 0xE0:			
			break;
		case 0xE1:			
			memcpy((uint8_t *)&Membuf->CalRxRms[SID],buf+5,4);
			Membuf->CalTable[SID] |= 0x02;
			break;
		case 0xE2:
			Membuf->CalMaxWaveTble = SID;
			len = buf[4];
			len <<= 8;
			len += buf[3];
			if(1 == buf[6]){
				len = 7;
				memcpy((uint8_t *)&Membuf->Calratio,buf+len,sizeof(float));
				len += sizeof(float);
				Membuf->CalTim.year = buf[len++];
				Membuf->CalTim.month = buf[len++];
				Membuf->CalTim.day = buf[len++];
				Membuf->CalTim.hour= buf[len++];
				Membuf->CalTim.minute= buf[len++];
				Membuf->CalTim.second= buf[len++];
				memcpy((uint8_t *)&Membuf->CalTim.us,buf+len,sizeof(uint32_t));
				
			}else{
				memcpy((uint8_t *)&Membuf->CalWave+(buf[6]-2)*(len-2),buf+7,len-2);
			}			
			CanReadWaveReply(buf[6]);
			if(buf[6]==buf[5]){
				Membuf->CalTable[SID] |= 0x04;
			}			
			break;
		case 0xE3:
			Membuf->CalTable[SID] |= 0x08;
			break;
		case 0xE4:
			Membuf->CalTable[SID] |= 0x08;
			CanSBreath(SID+1);
			CanOnlineSet(SID+1);
			Membuf->CanBreathTime[SID] = 0;
			break;
		default:;
			
	}
	
}//CanRxDisplay

#if 0
void CanBreath(void)
{

	uint8_t i,len;
	uint16_t mycrc;
	AlarmTimStruct Readtime;
	static uint16_t DelayTim;
	
	DelayTim++;
	if(DelayTim >= 6000){
		DelayTim = 0;
		for(i=0;i<iCanNum;i++){
			Membuf->CalTable[i] &= ~0x08;
		}
		len = 0;
		Membuf->CalTxbuf[len++] = (uint8_t)iCanHead;
		Membuf->CalTxbuf[len++] = iCanHead>>8;
		Membuf->CalTxbuf[len++] = 0x63;
		len++;
		len++;
		Readtime = SysGetTime();
		Membuf->CalTxbuf[len++] = Readtime.year;
		Membuf->CalTxbuf[len++] = Readtime.month;
		Membuf->CalTxbuf[len++] = Readtime.day;
		Membuf->CalTxbuf[len++] = Readtime.hour;	
		Membuf->CalTxbuf[len++] = Readtime.minute;
		Membuf->CalTxbuf[len++] = Readtime.second;	
		memcpy(Membuf->CalTxbuf+len,(uint8_t *)&Readtime.us,sizeof(uint32_t));
		len += sizeof(uint32_t);
		Membuf->CalTxbuf[3] = (len-5);
		Membuf->CalTxbuf[4] = (len-5)>>8;
		mycrc = CRC_GetModbus16(Membuf->CalTxbuf, len);
		Membuf->CalTxbuf[len++] = mycrc;
		Membuf->CalTxbuf[len++] = mycrc>>8;
		SendCanbuf(iCanMainId,Membuf->CalTxbuf,len);
	}
}//CanBreath
#endif


void CanSelStart(void)
{

	uint8_t i,len,flag;
	uint16_t mycrc;
	
	flag = 0;
	for(i=0;i<iCanNum;i++){
		if(0 == CanOnlineJudge(i+1)){
			flag++;			
		}
	}
	if(0 == flag){
		FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
		Membuf->MaxCanID = 0;
		return;
	}
	
	for(i=0;i<iCanNum;i++){
		Membuf->CalTable[i] &= ~0x02;
	}
	len = 0;
	Membuf->CalTxbuf[len++] = (uint8_t)iCanHead;
	Membuf->CalTxbuf[len++] = iCanHead>>8;
	Membuf->CalTxbuf[len++] = 0x60;
	Membuf->CalTxbuf[len++] = 0x00;
	Membuf->CalTxbuf[len++] = 0x00;			
	mycrc = CRC_GetModbus16(Membuf->CalTxbuf, len);
	Membuf->CalTxbuf[len++] = mycrc;
	Membuf->CalTxbuf[len++] = mycrc>>8;
	SendCanbuf(iCanMainId,Membuf->CalTxbuf,len);
	CanReadRmsDelayTim = 0;
}//CanSelStart

uint8_t CanReadWave(uint8_t Sid)
{

	uint8_t i,len,rt;
	uint16_t mycrc;
	static uint8_t step=0;
	switch(step){
		case 0:
			for(i=0;i<iCanNum;i++){
				Membuf->CalTable[i] &= ~0x04;
			}
			len = 0;
			Membuf->CalTxbuf[len++] = (uint8_t)iCanHead;
			Membuf->CalTxbuf[len++] = iCanHead>>8;
			Membuf->CalTxbuf[len++] = 0x62;
			Membuf->CalTxbuf[len++] = 0x01;
			Membuf->CalTxbuf[len++] = 0x00;
			Membuf->CalTxbuf[len++] = Sid;	
			mycrc = CRC_GetModbus16(Membuf->CalTxbuf, len);
			Membuf->CalTxbuf[len++] = mycrc;
			Membuf->CalTxbuf[len++] = mycrc>>8;
			SendCanbuf(iCanMainId,Membuf->CalTxbuf,len);
			CanReadWaveDelayTim = 0;
			step++;
			rt = 1;
			break;
		case 1:
			if(Membuf->CalTable[Sid-1] & 0x04){
				CanReadWaveDelayTim = 0;
				step = 0;
				rt = 0;
			}else{
				//if(CanReadWaveDelayTim>=50){
				if(CanReadWaveDelayTim>=100){
					rt = 2;
					step=0;
				}else{
					rt = 1;
				}
			}
			break;
		default:
			rt = 0;
			step = 0;
	}
	return rt;
}//CanSelStart

static void CanReadWaveReply(uint8_t bagNum)
{

	uint8_t len;
	uint16_t mycrc;
	len = 0;
	Membuf->CalTxbuf[len++] = (uint8_t)iCanHead;
	Membuf->CalTxbuf[len++] = iCanHead>>8;
	Membuf->CalTxbuf[len++] = 0x65;
	Membuf->CalTxbuf[len++] = 0x01;
	Membuf->CalTxbuf[len++] = 0x00;
	Membuf->CalTxbuf[len++] = bagNum;	
	mycrc = CRC_GetModbus16(Membuf->CalTxbuf, len);
	Membuf->CalTxbuf[len++] = mycrc;
	Membuf->CalTxbuf[len++] = mycrc>>8;
	SendCanbuf(iCanMainId,Membuf->CalTxbuf,len);
	CanReadWaveDelayTim = 0;
}//CanSelStart



void CanSBreath(uint8_t Sid)
{
	uint8_t len;
	uint16_t mycrc;	
	AlarmTimStruct Readtime;
	len = 0;
	Membuf->CalTxbuf[len++] = (uint8_t)iCanHead;
	Membuf->CalTxbuf[len++] = iCanHead>>8;
	Membuf->CalTxbuf[len++] = 0x64;
	len++;
	len++;	
	Membuf->CalTxbuf[len++] = Sid;
	Readtime = SysGetTime();
	Membuf->CalTxbuf[len++] = Readtime.year;
	Membuf->CalTxbuf[len++] = Readtime.month;
	Membuf->CalTxbuf[len++] = Readtime.day;
	Membuf->CalTxbuf[len++] = Readtime.hour;	
	Membuf->CalTxbuf[len++] = Readtime.minute;
	Membuf->CalTxbuf[len++] = Readtime.second;	
	memcpy(Membuf->CalTxbuf+len,(uint8_t *)&Readtime.us,sizeof(uint32_t));
	len += sizeof(uint32_t);
	Membuf->CalTxbuf[3] = (len-5);
	Membuf->CalTxbuf[4] = (len-5)>>8;
	mycrc = CRC_GetModbus16(Membuf->CalTxbuf, len);
	Membuf->CalTxbuf[len++] = mycrc;
	Membuf->CalTxbuf[len++] = mycrc>>8;
	SendCanbuf(iCanMainId,Membuf->CalTxbuf,len);	
}//CanSBreath

void CanSBreathTimes(void)
{
	uint8_t i;
	for(i=0;i<iCanNum;i++){
		if(Membuf->CanBreathTime[i]<iCanBreathTime){
			Membuf->CanBreathTime[i]++;
		}else{			
			Membuf->CalTable[i] &= ~0x08;
			CanOnlineClr(i+1);			
		}
	}

}//CanSBreathTimes

void CanOnlineSet(uint8_t sid)
{
	uint8_t i,bit_i;
	if((sid>=1)&&(sid<=128)){
		sid -= 1;
		i = sid/8;
		bit_i = sid%8;
		Membuf->CanOnlineTable[i] |= (1<<bit_i);
	}
}

void CanOnlineClr(uint8_t sid)
{
	uint8_t i,bit_i;
	if((sid>=1)&&(sid<=128)){
		sid -= 1;
		i = sid/8;
		bit_i = sid%8;
		Membuf->CanOnlineTable[i] &= ~(1<<bit_i);
	}
}

uint8_t CanOnlineJudge(uint8_t sid)
{
	uint8_t rt,i,bit_i;
	if((sid>=1)&&(sid<=128)){
		sid -= 1;
		i = sid/8;
		bit_i = sid%8;
		if(Membuf->CanOnlineTable[i] & (1<<bit_i)){
			rt = 0;
		}else{
			rt = 1;
		}
	}else{
		rt = 2;
	}
	return rt;
}//CanOnlineJudge


static void CanFaultSave(void)
{
	float MaxRxRms;
	uint8_t i,m,n;
	//=============
	CanFaultSelSave.CanId[0]=0;
	CanFaultSelSave.RMSValue[0]=0;
	CanFaultSelSave.CanId[1]=0;
	CanFaultSelSave.RMSValue[1]=0;
	CanFaultSelSave.CanId[2]=0;
	CanFaultSelSave.RMSValue[2]=0;
	
	for(i=0;i<iCanNum;i++){
		if((Membuf->CalTable[i] & 0x08)==0)
			Membuf->CalRxRms[i]=0;		//不在线清除
	}
	
	Membuf->MaxCanID = 0;
	MaxRxRms = 0;
	for(i=0;i<iCanNum;i++){
		if(Membuf->CalTable[i] & 0x02){	
			Membuf->CalTable[i] &= ~0x02;
			if(MaxRxRms < Membuf->CalRxRms[i]){
				MaxRxRms = Membuf->CalRxRms[i];
				Membuf->MaxCanID = i+1;
				//CanFaultSelSave.FaultFlag[0]=128;
				CanFaultSelSave.CanId[0]=Membuf->MaxCanID;
				//CanFaultSelSave.Tim[0]=Membuf->CalTim;
				//CanFaultSelSave.ratio[0]=Membuf->Calratio;
				CanFaultSelSave.RMSValue[0]=Membuf->CalRxRms[i];
				}
			}
		}
	//==========
	m= Membuf->MaxCanID -1;
	Membuf->MaxCanID = 0;
	MaxRxRms = 0;	
	for(i=0;i<iCanNum;i++){
		if(i!=m){
			if(MaxRxRms < Membuf->CalRxRms[i]){
				MaxRxRms = Membuf->CalRxRms[i];
				Membuf->MaxCanID = i+1;
				//CanFaultSelSave.FaultFlag[1]=128;
				CanFaultSelSave.CanId[1]=Membuf->MaxCanID;
				//CanFaultSelSave.Tim[1]=Membuf->CalTim;
				//CanFaultSelSave.ratio[1]=Membuf->Calratio;
				CanFaultSelSave.RMSValue[1]=Membuf->CalRxRms[i];
				}
			
		}
	}	
	//============
	n= Membuf->MaxCanID -1;
	Membuf->MaxCanID = 0;
	MaxRxRms = 0;
	for(i=0;i<iCanNum;i++){
		if(i!=n && i!=m){
			if(MaxRxRms < Membuf->CalRxRms[i]){
				MaxRxRms = Membuf->CalRxRms[i];
				Membuf->MaxCanID = i+1;
				//CanFaultSelSave.FaultFlag[2]=128;
				CanFaultSelSave.CanId[2]=Membuf->MaxCanID;
				//CanFaultSelSave.Tim[2]=Membuf->CalTim;
				//CanFaultSelSave.ratio[2]=Membuf->Calratio;
				CanFaultSelSave.RMSValue[2]=Membuf->CalRxRms[i];
				}
			
		}
	}	
	
}

static uint8_t CanRMSCompare(void)
{
	uint8_t i,flag,flag1,rt;	
	//float MaxRxRms;
	static uint8_t step;
	static uint8_t Cnt=0;

	switch(step){
		case 0:
			rt = 0;
			if(CanRmsRxFlag){
				CanRmsRxFlag = 0;
				rt = 1;
				CanReadRmsDelayTim = 0;
				step++;
			}
			break;
		case 1:
			rt = 0;
			flag = 0;
			for(i=0;i<iCanNum;i++){
				if(0 == CanOnlineJudge(i+1)){
					flag++;			
				}
			}
			
			flag1 = 0;
			
			for(i=0;i<iCanNum;i++){		
				if(Membuf->CalTable[i] & 0x02){
					flag1++;
				}
			}
			if(CanReadRmsDelayTim<50){
				if((flag1 == flag)&&(flag)){
					/*
					Membuf->MaxCanID = 0;
					MaxRxRms = 0;
					for(i=0;i<iCanNum;i++){
						if(Membuf->CalTable[i] & 0x02){	
							Membuf->CalTable[i] &= ~0x02;
							if(MaxRxRms < Membuf->CalRxRms[i]){
								MaxRxRms = Membuf->CalRxRms[i];
								Membuf->MaxCanID = i+1;
							}
						}
					}
					*/
					CanFaultSave();
					step++;					
				}
				rt = 1;
			}else{
				if(flag1){
					/*
					Membuf->MaxCanID = 0;
					MaxRxRms = 0;
					for(i=0;i<iCanNum;i++){
						if(Membuf->CalTable[i] & 0x02){	
							Membuf->CalTable[i] &= ~0x02;
							if(MaxRxRms < Membuf->CalRxRms[i]){
								MaxRxRms = Membuf->CalRxRms[i];
								Membuf->MaxCanID = i+1;
							}
						}
					}
					*/
					CanFaultSave();
					step++;
					rt = 1;
					
				}else{
					Membuf->MaxCanID = 0;
					CanFaultSelSave.CanId[0]=0;
					step = 0;
					FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
					AllSelDisplay();
					rt = 2;
				}
			}
			break;
		case 2:
			/*
			rt = CanReadWave(Membuf->MaxCanID);
			if(0 == rt){
				FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
				step = 0;
				AllSelDisplay();
			}else if(2 == rt){
				FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
				Membuf->MaxCanID = 0;
				step = 0;
				AllSelDisplay();
			}
		*/
		 rt=CanReadWave(CanFaultSelSave.CanId[Cnt]);
		 if(0==rt){
			 //FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
			 CanFaultSelSave.FaultFlag[Cnt]=128;
			 //CanFaultSelSave.CanId[Cnt]=Membuf->MaxCanID;
			 CanFaultSelSave.Tim[Cnt]=Membuf->CalTim;
			 CanFaultSelSave.ratio[Cnt]=Membuf->Calratio;
			 //CanFaultSelSave.RMSValue[Cnt]=Membuf->CalRxRms[Membuf->MaxCanID-1];
			 memcpy((uint8_t *)CanFaultSelSave.Wave[Cnt],Membuf->CalWave,20*256*2);
			  Cnt++;
		 } else if(2== rt){
			 //FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20;
			// CanFaultSelSave.CanId[Cnt]=0;
			 Cnt++;
			 
		 }
		 
		  if(Cnt>=3){
				FaultFifo[FaultFifo_pt]->DislayFlag |= 0x20; //save
				Cnt=0;
				step=0;
				AllSelDisplay();
			}
			break;
		default:
			step = 0;
			rt = 0;
	}
	return rt;
}//CanRMSCompare


#ifdef UseLocal_16Current

void AllSelDisplay(void)
{
  uint8_t i;
	
	if(0xE0 == (FaultFifo[FaultFifo_pt]->DislayFlag & 0xE0)){
		FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
		
		
		//==============================================================
		for(i=0;i<3;i++){
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[i]=i;
		FaultAllSelFifo[FaultFifo_pt]->CanId[i]=0;
		FaultAllSelFifo[FaultFifo_pt]->Tim[i] = FaultSelFifo[FaultFifo_pt]->Tim;
		FaultAllSelFifo[FaultFifo_pt]->ratio[i]=FaultSelFifo[FaultFifo_pt]->fftratio[i];
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[i] = FaultSelFifo[FaultFifo_pt]->RMSValue[i];
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[i],(uint8_t *)FaultSelFifo[FaultFifo_pt]->Wave[i],20*128*2);				
		}
		//=========第一个
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0] = FaultSelFifo[FaultFifo_pt]->FaultFlag; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[0] = 0;
		FaultAllSelFifo[FaultFifo_pt]->Tim[0] = FaultSelFifo[FaultFifo_pt]->Tim;
		FaultAllSelFifo[FaultFifo_pt]->ratio[0] = FaultSelFifo[FaultFifo_pt]->fftratio[FaultSelFifo[FaultFifo_pt]->FaultFlag];
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[0] = FaultSelFifo[FaultFifo_pt]->RMSValue[FaultSelFifo[FaultFifo_pt]->FaultFlag];
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[0],(uint8_t *)FaultSelFifo[FaultFifo_pt]->Wave[FaultSelFifo[FaultFifo_pt]->FaultFlag],20*128*2);
		//=====剩下的15个选两个电流大的
		for(i=0;i<16;i++){
			if(i!=FaultSelFifo[FaultFifo_pt]->FaultFlag){
				if(FaultAllSelFifo[FaultFifo_pt]->RMSValue[1]>FaultAllSelFifo[FaultFifo_pt]->RMSValue[2]){
					if(FaultSelFifo[FaultFifo_pt]->RMSValue[i]>FaultAllSelFifo[FaultFifo_pt]->RMSValue[2]){
						FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2] = i; 
						FaultAllSelFifo[FaultFifo_pt]->CanId[2] = 0;
						FaultAllSelFifo[FaultFifo_pt]->Tim[2] = FaultSelFifo[FaultFifo_pt]->Tim;
						FaultAllSelFifo[FaultFifo_pt]->ratio[2] = FaultSelFifo[FaultFifo_pt]->fftratio[i];
						FaultAllSelFifo[FaultFifo_pt]->RMSValue[2] = FaultSelFifo[FaultFifo_pt]->RMSValue[i];
						memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],(uint8_t *)FaultSelFifo[FaultFifo_pt]->Wave[i],20*128*2);
						
					}
				
				}
				else{
					if(FaultSelFifo[FaultFifo_pt]->RMSValue[i]>FaultAllSelFifo[FaultFifo_pt]->RMSValue[1]){
						FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1] = i; 
						FaultAllSelFifo[FaultFifo_pt]->CanId[1] = 0;
						FaultAllSelFifo[FaultFifo_pt]->Tim[1] = FaultSelFifo[FaultFifo_pt]->Tim;
						FaultAllSelFifo[FaultFifo_pt]->ratio[1] = FaultSelFifo[FaultFifo_pt]->fftratio[i];
						FaultAllSelFifo[FaultFifo_pt]->RMSValue[1] = FaultSelFifo[FaultFifo_pt]->RMSValue[i];
						memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[1],(uint8_t *)FaultSelFifo[FaultFifo_pt]->Wave[i],20*128*2);
						
					}
					
				}
			}
		}
		
		
		
		//if(Membuf->MaxCanID){		//有Can
		 if(CanFaultSelSave.CanId[0]){		//有Can
			Select3Big();
			
		}
		
		FaultFifo_busy = 0;
		FaultFifo_pt++;
		if(FaultFifo_pt>= FaultFifoSIZE){
			FaultFifo_pt = 0;
		}
	}	


}//AllSelDisplay
#else
void AllSelDisplay(void)
{
  uint8_t i;
	
	if(0x60 == (FaultFifo[FaultFifo_pt]->DislayFlag & 0x60)){
		FaultFifo[FaultFifo_pt]->DislayFlag = 0xFF;
		
		
		//if(Membuf->MaxCanID){		//有Can
		/*
		 if(CanFaultSelSave.CanId[0]){		//有Can
			Select3Big();
			
		}
		*/
		
		for(i=0;i<3;i++){
			FaultAllSelFifo[FaultFifo_pt]->FaultFlag[i] = 128; 
			FaultAllSelFifo[FaultFifo_pt]->CanId[i] =  CanFaultSelSave.CanId[i];//Membuf->MaxCanID;
			FaultAllSelFifo[FaultFifo_pt]->Tim[i] = CanFaultSelSave.Tim[i];
			FaultAllSelFifo[FaultFifo_pt]->ratio[i] = CanFaultSelSave.ratio[i];
			FaultAllSelFifo[FaultFifo_pt]->RMSValue[i] = CanFaultSelSave.RMSValue[i];
			memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[i],CanFaultSelSave.Wave[i],20*256*2);
		}
		SortFormBigToSmall();
		
		
		FaultFifo_busy = 0;
		FaultFifo_pt++;
		if(FaultFifo_pt>= FaultFifoSIZE){
			FaultFifo_pt = 0;
		}
	}	


}//AllSelDisplay
#endif

//=====选择3个比较大的电流
void SortFormBigToSmall(void)
{

	FaultStruct Temp;
	//===if(a<b)
	if(FaultAllSelFifo[FaultFifo_pt]->RMSValue[0]<FaultAllSelFifo[FaultFifo_pt]->RMSValue[1]){
		// t
		
		Temp.FaultFlag=FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0]; 
		Temp.CanId=FaultAllSelFifo[FaultFifo_pt]->CanId[0];
		Temp.Tim=FaultAllSelFifo[FaultFifo_pt]->Tim[0];
		Temp.ratio=FaultAllSelFifo[FaultFifo_pt]->ratio[0];
		Temp.RMSValue=FaultAllSelFifo[FaultFifo_pt]->RMSValue[0];
		memcpy((uint8_t *)Temp.Wave,(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[0],20*256*2);
		//a=b
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0] = FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1]; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[0] =  FaultAllSelFifo[FaultFifo_pt]->CanId[1];
		FaultAllSelFifo[FaultFifo_pt]->Tim[0] =FaultAllSelFifo[FaultFifo_pt]->Tim[1];
		FaultAllSelFifo[FaultFifo_pt]->ratio[0] = FaultAllSelFifo[FaultFifo_pt]->ratio[1];
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[0] = FaultAllSelFifo[FaultFifo_pt]->RMSValue[1];
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[0],(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[1],20*256*2);
		//b=t
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1] = Temp.FaultFlag; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[1] =  Temp.CanId;
		FaultAllSelFifo[FaultFifo_pt]->Tim[1] =Temp.Tim;
		FaultAllSelFifo[FaultFifo_pt]->ratio[1] = Temp.ratio;
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[1] = Temp.RMSValue;
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[1],(uint8_t *)Temp.Wave,20*256*2);
		
	}
	
	//===if(a<c)
	if(FaultAllSelFifo[FaultFifo_pt]->RMSValue[0]<FaultAllSelFifo[FaultFifo_pt]->RMSValue[2]){
		// t
		Temp.FaultFlag=FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0]; 
		Temp.CanId=FaultAllSelFifo[FaultFifo_pt]->CanId[0];
		Temp.Tim=FaultAllSelFifo[FaultFifo_pt]->Tim[0];
		Temp.ratio=FaultAllSelFifo[FaultFifo_pt]->ratio[0];
		Temp.RMSValue=FaultAllSelFifo[FaultFifo_pt]->RMSValue[0];
		memcpy((uint8_t *)Temp.Wave,(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[0],20*256*2);
		//a=b
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[0] = FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2]; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[0] =  FaultAllSelFifo[FaultFifo_pt]->CanId[2];
		FaultAllSelFifo[FaultFifo_pt]->Tim[0] =FaultAllSelFifo[FaultFifo_pt]->Tim[2];
		FaultAllSelFifo[FaultFifo_pt]->ratio[0] = FaultAllSelFifo[FaultFifo_pt]->ratio[2];
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[0] = FaultAllSelFifo[FaultFifo_pt]->RMSValue[2];
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[0],(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],20*256*2);
		//b=t
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2] = Temp.FaultFlag; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[2] =  Temp.CanId;
		FaultAllSelFifo[FaultFifo_pt]->Tim[2] =Temp.Tim;
		FaultAllSelFifo[FaultFifo_pt]->ratio[2] = Temp.ratio;
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[2] = Temp.RMSValue;
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],(uint8_t *)Temp.Wave,20*256*2);
		
	}
	
	//====if(b<c)
	
		if(FaultAllSelFifo[FaultFifo_pt]->RMSValue[1]<FaultAllSelFifo[FaultFifo_pt]->RMSValue[2]){
		// t
		Temp.FaultFlag=FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1]; 
		Temp.CanId=FaultAllSelFifo[FaultFifo_pt]->CanId[1];
		Temp.Tim=FaultAllSelFifo[FaultFifo_pt]->Tim[1];
		Temp.ratio=FaultAllSelFifo[FaultFifo_pt]->ratio[1];
		Temp.RMSValue=FaultAllSelFifo[FaultFifo_pt]->RMSValue[1];
		memcpy((uint8_t *)Temp.Wave,(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[1],20*256*2);
		//a=b
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[1] = FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2]; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[1] =  FaultAllSelFifo[FaultFifo_pt]->CanId[2];
		FaultAllSelFifo[FaultFifo_pt]->Tim[1] =FaultAllSelFifo[FaultFifo_pt]->Tim[2];
		FaultAllSelFifo[FaultFifo_pt]->ratio[1] = FaultAllSelFifo[FaultFifo_pt]->ratio[2];
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[1] = FaultAllSelFifo[FaultFifo_pt]->RMSValue[2];
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[1],(uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],20*256*2);
		//b=t
		FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2] = Temp.FaultFlag; 
		FaultAllSelFifo[FaultFifo_pt]->CanId[2] =  Temp.CanId;
		FaultAllSelFifo[FaultFifo_pt]->Tim[2] =Temp.Tim;
		FaultAllSelFifo[FaultFifo_pt]->ratio[2] = Temp.ratio;
		FaultAllSelFifo[FaultFifo_pt]->RMSValue[2] = Temp.RMSValue;
		memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],(uint8_t *)Temp.Wave,20*256*2);
		
	}
	
}
//=====选择3个比较大的电流
void Select3Big(void)
{
	uint8_t i;
	SortFormBigToSmall();
	for(i=0;i<3;i++){
		if(CanFaultSelSave.RMSValue[i]>FaultAllSelFifo[FaultFifo_pt]->RMSValue[2]){
			FaultAllSelFifo[FaultFifo_pt]->FaultFlag[2] = 128; 
			FaultAllSelFifo[FaultFifo_pt]->CanId[2] =  CanFaultSelSave.CanId[i];//Membuf->MaxCanID;
			FaultAllSelFifo[FaultFifo_pt]->Tim[2] = CanFaultSelSave.Tim[i];
			FaultAllSelFifo[FaultFifo_pt]->ratio[2] = CanFaultSelSave.ratio[i];
			FaultAllSelFifo[FaultFifo_pt]->RMSValue[2] = CanFaultSelSave.RMSValue[i];
			memcpy((uint8_t *)FaultAllSelFifo[FaultFifo_pt]->Wave[2],CanFaultSelSave.Wave[i],20*256*2);
			SortFormBigToSmall();
		}
	}

}	

