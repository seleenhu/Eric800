
#include "bsp.h"
//#include "includes.h"	
//#include "emwin_eric800.h"
//#include "param.h"

MessgeFlagEnum MessgeFlag;

#if 0
ADCFifoStruct ADCFifo ={
												.pt = 0,												
												.bt = 0,
												.Instantbt = 0,
												.Faultbt = 0,
												.FifoSize = ADCFifo_SDRAM_SIZE/2,
												//.U_rati0 = 0.009627,
												//.I_rati0 = 0.000307,
												.rati0 = {68.3887863,68.3887863,68.3887863,194.454239,2021.42578,2027.51135,2023.09204,3802.24146},
												.fftratio = {68.73,69.27,67.99,196.59,2015.54,2021.48,2018.51,3800.27},												
												.UAFIFO = (int16_t *)(ADCFifo_SDRAM0),
												.UBFIFO = (int16_t *)(ADCFifo_SDRAM1),
												.UCFIFO = (int16_t *)(ADCFifo_SDRAM2),
												.U0FIFO = (int16_t *)(ADCFifo_SDRAM3),
												.IAFIFO = (int16_t *)(ADCFifo_SDRAM4),
												.IBFIFO = (int16_t *)(ADCFifo_SDRAM5),
												.ICFIFO = (int16_t *)(ADCFifo_SDRAM6),
												.I0FIFO = (int16_t *)(ADCFifo_SDRAM7),
											 };


#else
//ADCFifoStruct *ADCFifo = (ADCFifoStruct *)(ADCFifo_SDRAM);
#endif
u8 FaultFifo_pt;

FaultSaveStruct *FaultFifo[FaultSaveSIZE];
//FaultSelSaveStruct *FaultSelFifo[FaultSaveSIZE];
FaultAllSelSaveStruct *FaultAllSelFifo[FaultSaveSIZE];

ADCWholeStruct *ADCWholeVar = (ADCWholeStruct *)(ADCWhole_SDRAM);											 

Setstruct SystemSet = {												
												.NetSet = {
																	.IPadrr = {116,30,193,209},
																	.Port = 8801,
																	.Modbusadr = 0xBBFF,
																	.ModbudID = "ERIC0820211000000001"
																	},
												.RS485Set = {
																		.Baud = 9600,
																		.adr = 25
																		},
												.RS232Set = {
																		.Baud = 9600,																		
																		},
												.LcdSet = {
																	.LcdOnTim = 300,
																	.LcdFlag = 0,
																	},
												.SyntonySet= {
																	.SyntonyV = {{30,25,20,15},{30,25,20,15},{30,25,20,15}},
																	},
												.ShortGNDSet = {
																				.I0 = 20,
																			 },												
											};
//AlarmStruct *SystemAlarm = (AlarmStruct *)(SystemAlarm_SDRAM);											
PowerQualityStruct *SystemPowerQuality = (PowerQualityStruct *)(SystemPowerQuality_SDRAM);
ActualStruct *SystemActual = (ActualStruct *)(SystemActual_SDRAM);
RFStruct *RFReception = (RFStruct *)(RFReception_SDRAM);
											
SystemStateStruct 	SystemState;	
											
//UnionRamStruct UnionRam[8];

CupboardParameter Cupboard_P[20][12];
RelayStruct Relay[2] = {{.RelayId = 0x7A000002},
												{.RelayId = 0x7A000003},
											 };											
											
CriteriaStruct CriteriaValue ={
	.Disconnection = {.MaxValue = 80,/*60.59,*/
						.MinValue = 40,/*54.8,*/
						.ErrValue = 30,
						.MaxUl = 30},
	.Gnd = {
					.MaxValue =85,
					.Ul = 50,
					.MinValue = 5					
				 },
	.Resonance = {
								.MaxValue1_3 = 12.3,
								.MaxValue1_2 = 25,
								.MaxValue1 = 30,								
								.MaxValue3 = 33,
							},
	.DecompressionValue = 5
															};											

const float radical3 =  1.7320508;
const float radical2 =  1.4142135;			

const char Ver[]={"V1.06"};	
uint16_t ScreenCloseTim;

uint16_t uSCount;

AlarmTimStruct SystemTime;

BitUnion WholeBit;

ChipICStruct ChipICID;

MemStruct *Membuf = (MemStruct *)Mem_SDRAM;

//TestStruct *TestData = (TestStruct *)SDRAM_APP1_END;
//u32 TestPt;
/*
---------------------------------------------------------------------
Name		FIFO_length
Function	FIFO�ĳ���
Input		����ָ�룬д��ָ�룬�ܳ�
OutPut	FIFO�ĳ���
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
u16 FIFO_length(u16 bt,u16 pt,u16 len)
{
	if(pt>=bt){
		return (pt-bt);
	}
	return (len-(bt-pt));
}//FIFO_length
/*
---------------------------------------------------------------------
Name		FIFO_pointer
Function	FIFO��ָ�����
Input		ָ�룬�ܳ�
OutPut	ָ�뵱ǰֵ
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
u16 FIFO_pointer(u16 pointer,u16 len,u16 sizelen)
{
#if 1
	while(len--){
		pointer++;
		if(pointer>=sizelen){
			pointer = 0;
		}
	}
#else
	if((pointer+len) < sizelen){
		pointer += len;
	}else{
		pointer = (pointer+len) - sizelen;
	}
#endif
	
	return pointer;
}//FIFO_pointer

/*
---------------------------------------------------------------------
Name		FIFO_Backpointer
Function	FIFO���˵ĳ���
Input		ָ�룬�ܳ�
OutPut	ָ�뵱ǰֵ
Edit by fanzhong
Date		2018-09-05
---------------------------------------------------------------------
*/
u16 FIFO_Backpointer(u16 pointer,u16 len,u16 sizelen)
{
	if(pointer >= len){
		pointer = pointer - len;
	}else{
		pointer = sizelen - (len-pointer);
	}
	return pointer;
}//FIFO_Backpointer
/*
---------------------------------------------------------------------
Name		Get_pointer
Function	FIFO��ָ�����
Input		ָ�룬��������
OutPut	��Ҫ��ʱ��
Edit by fanzhong
Date		2018-1-17
---------------------------------------------------------------------
*/

u16 Get_pointer(u16 pointer,u16 CycleNum)
{
	if(pointer>=256*CycleNum)
	{
		pointer=pointer-256*CycleNum;
	}
	else
	{
		pointer=pointer+ADCFifo_SDRAM_SIZE/2-256*CycleNum;
	}
	return pointer;
	
}

/*
---------------------------------------------------------------------
Name		FIFO_bufcmp
Function	FIFO��ָ��buf�Ƚ�
Input		ָ�룬�ܳ�
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
u8 FIFO_bufcmp(u8 *FIFObuf,u8 *buf,u16 bt,u16 len,u16 sizelen)
{
	u16 i;
	i = 0;
	while(len--){
		if(FIFObuf[bt] != buf[i]){
			return 1;
		}
		i++;
		bt = FIFO_pointer(bt,1,sizelen);
	}
	return 0;
}//FIFO_bufcmp
#if 0
/*
---------------------------------------------------------------------
Name		HexToAscii
Function	16���Ƶ�ascii
Input		��ת������ֵ�buf�
OutPut	����
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
u8 HexToAscii(u32 hexdata,u8 *buf)
{
	u8 len;
	u8 i,data8;
	len = 0;
	while(hexdata){
		buf[len] = hexdata%10 + '0';
		hexdata /= 10;
		len++;
	}
	for(i=0; i<len/2; i++){
		data8 = buf[i];
		buf[i] = buf[len-1-i];
		buf[len-1-i] = data8;		
	}
	return len;
}//HexToAscii
#endif
/*
---------------------------------------------------------------------
Name		GetRevCrc_16
Function	����CRC16
Input		
				pData ����
				nLength ���ݳ���
				init ��ʼ������
				ptable ���ұ�
OutPut	CRC����
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
static u16 GetRevCrc_16(u8 * pData, u16 nLength,u16 init, const u16 *ptable)
{
  unsigned short cRc_16 = init;
  unsigned char temp;

  while(nLength-- > 0)
  {
    temp = cRc_16 & 0xFF; 
    cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ *pData++) & 0xFF];
  }

  return cRc_16;
}
/*
---------------------------------------------------------------------
Name		CRC_GetModbus16
Function	CRC����
Input		
				pData ����
				nLength ���ݳ���
				
OutPut	CRC����
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
u16 CRC_GetModbus16(u8 *pdata, u16 len)
{
	//MODBUS CRC-16
	const unsigned short g_McRctable_16[256] =
	{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
	};

  return GetRevCrc_16(pdata, len, 0xFFFF, g_McRctable_16);
	//return GetCrc_16(pdata, len, 0xFFFF, g_McRctable_16);
}

/*
u16 crc16_modbus(u8 *pdata,u16 length)
{
	u16 i,j;
	u16 crc16data = 0xFFFF;
	for(i=0; i<length; i++){
		crc16data ^= pdata[i];
		for(j=0;j<8;j++){
			if(1 == (crc16data & 0x01)){
				crc16data = (crc16data>>1)^0xA001;
			}else{
				crc16data = crc16data>>1;
			}
		}
	}
	return crc16data;
}
*/
/*
---------------------------------------------------------------------
Name		AsciiToHex
Function	ascii��16����
Input		��ת������ֵ�buf�
OutPut	����
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
u32 AsciiToHex(u8 *Asciibuf,u8 Asciibuf_len)
{
	u32 data32;
	data32 = 0;
	while(Asciibuf_len--){
		data32 = data32*10 + *Asciibuf -'0';
		Asciibuf++;
	}
	return data32;
}//AsciiToHex
/*
---------------------------------------------------------------------
Name		SysGetTime
Function	�õ�ϵͳʱ��
Input		No
OutPut	ϵͳʱ��
Edit by fanzhong
Date		2018-01-18
---------------------------------------------------------------------
*/
AlarmTimStruct SysGetTime(void)
{
	AlarmTimStruct SysTim;
	RTC_TimeTypeDef rtctim;
	RTC_DateTypeDef rtcdate;
	
	
	RTC_GetTime(RTC_Format_BIN,&rtctim);
	RTC_GetDate(RTC_Format_BIN,&rtcdate);
	
	SysTim.hour = rtctim.RTC_Hours;
	SysTim.minute = rtctim.RTC_Minutes;
	SysTim.second = rtctim.RTC_Seconds;
	
	SysTim.year = rtcdate.RTC_Year;
	SysTim.month = rtcdate.RTC_Month;
	SysTim.day = rtcdate.RTC_Date;
	
	SysTim.us = uSCount;
	SysTim.us *= 78125;
	SysTim.us /= 1000;	
	return SysTim;
}//AlarmTimStruct


/*
---------------------------------------------------------------------
Name		SysSetTime
Function	����ϵͳʱ��
Input		No
OutPut	No
Edit by seleen
Date		2018-05-25
---------------------------------------------------------------------
*/
void SysSetTime(void)
{
	RTC_TimeStructure.RTC_Hours=SystemTime.hour;
	RTC_TimeStructure.RTC_Minutes=SystemTime.minute;
	RTC_TimeStructure.RTC_Seconds=SystemTime.second;
	RTC_DateStructure.RTC_Year=SystemTime.year;
	RTC_DateStructure.RTC_Month=SystemTime.month;
	RTC_DateStructure.RTC_Date=SystemTime.day;
	RTC_DateStructure.RTC_WeekDay=RTC_CalcWeek(SystemTime.year,SystemTime.month,SystemTime.day);
	RTC_Config();
	SPIBitFlag.Bits.S_12 = 1;
}//SysSetTime


/*
---------------------------------------------------------------------
Name		RFTemSend
Function	����RF433��ʱ���͵��¶�
Input		i :��� j:�����е�λ��  �洢���ݵ�ָ��
OutPut	��
Edit by seleen hu
Date		2018-03-20
---------------------------------------------------------------------
*/

void RFTemSend(u8 i,u8 j,struct _RFTem *RFbuf)
{
	uint8_t DataType;
	Cupboard_P[i][j].OnlineFlag=1;
	Cupboard_P[i][j].KeepTim = 0;
	DataType=Cupboard_P[i][j].Type;
	Cupboard_P[i][j].TimeStamp=RFbuf->TimeStamp;
	//*(&Cupboard_P[i][j].Value.TempBusA+4*DataType)=RFbuf->TemValue;
	*(&(Cupboard_P[i][j].Value.TempBusA)+DataType)=RFbuf->TemValue;
}

/*
---------------------------------------------------------------------
Name		RFProtectorTemSend
Function	����RF433��������ʱ����
Input		i :��� j:�����е�λ��  �洢���ݵ�ָ��
OutPut	��
Edit by seleen hu
Date		2018-03-20
---------------------------------------------------------------------
*/

void RFProtectorTemSend(u8 i,u8 j,struct _ProtectorTem *RFbuf)
{
	Cupboard_P[i][j].OnlineFlag=1;
	Cupboard_P[i][j].KeepTim = 0;
	Cupboard_P[i][j].TimeStamp=RFbuf->Tim;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageTA=RFbuf->TemA;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageTB=RFbuf->TemB;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageTC=RFbuf->TemC;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageHA=RFbuf->HumA;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageHB=RFbuf->HumB;
	Cupboard_P[i][j].Value.ProtectorData.OverVoltageHC=RFbuf->HumC;
	Cupboard_P[i][j].Value.ProtectorData.LeakgaeCurrent=RFbuf->LeakageCurrent;
	Cupboard_P[i][j].Value.ProtectorData.BatteryValue=RFbuf->BatteryVoltage;
}
/*
---------------------------------------------------------------------
Name		RFProtectorActionSend
Function	����RF433������ͻ������
Input		i :��� j:�����е�λ��  �洢���ݵ�ָ��
OutPut	��
Edit by seleen hu
Date		2018-03-20
---------------------------------------------------------------------
*/
void RFProtectorActionSend(u8 i,u8 j,struct _ProtectorAction *RFbuf)
{
	uint8_t PhaseNo;
	Cupboard_P[i][j].TimeStamp=RFbuf->Tim;
	PhaseNo=RFbuf->Phase;
	switch(PhaseNo)
	{
		case PhaseA:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeAD=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntAD++;
			break;
		case PhaseB:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeBD=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntBD++;
			break;
		case PhaseC:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeCD=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntCD++;
			break;
		case PhaseAB:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeAB=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntAB++;
			break;
		case PhaseBC:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeBC=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntBC++;
			break;
		case PhaseAC:
			Cupboard_P[i][j].Value.ProtectorData.ActionTimeAC=RFbuf->ActionTime;
			Cupboard_P[i][j].Value.ProtectorData.ActionCntAC++;
			break;
		
		default:
		break;
			
	}
}

#if 0
	

/*
*********************************************************************************************************
*	�� �� ��: SaveSDFault
*	����˵��: ��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SaveSDFault(void)
{
	u8 i;
	u16 len;
	
	
	
	FIL fp;
	FRESULT Frt;
	FRESULT result;
	FATFS fs;
	UINT ByteWritten;
	u8 *upt8,*upt8_1;
	const char lisname[] = ".txt";
	char *buf;
	
	buf = (char *)Membuf->SDbuf;
	for(i=0;i<30;i++){
		if((FaultFifo[i]->DislayFlag) & 0x02){			
			switch(FaultFifo[i]->FaultStyle){
				case GroundFaultA:
				case GroundFaultB:
				case GroundFaultC:
				case GroundFaultArcA:
				case GroundFaultArcB:
				case GroundFaultArcC:
					buf[0] = 'G';
					buf[1] = 'A';
					len = sprintf(buf+2,"%d",SDGroundFault_index+1);
					len += 2;
					upt8 = &SDGroundFault_index;
					upt8_1 = &SDGroundFault_sum;
					break;
				case ExceedValueFaultA:
				case ExceedValueFaultB:
				case ExceedValueFaultC:
					buf[0] = 'S';
					buf[1] = 'V';
					buf[2] = 'A';
					len = sprintf(buf+3,"%d",SDInstantaneousVoltageFault_index+1);	
					len += 3;
					upt8 = &SDInstantaneousVoltageFault_index;
					upt8_1 = &SDInstantaneousVoltageFault_sum;
					break;
				default:
					buf[0] = 'X';
					buf[1] = 'A';
					len = sprintf(buf+2,"%d",SDInstantaneousXFault_index+1);	
					len += 2;
					upt8 = &SDInstantaneousXFault_index;
					upt8_1 = &SDInstantaneousXFault_sum;

			}
			buf[len++] = lisname[0];
			buf[len++] = lisname[1];
			buf[len++] = lisname[2];
			buf[len++] = lisname[3];
			buf[len++] = 0;

			result = f_mount(&fs, "0:/", 0);
			if(FR_OK == result){
							
				Frt = f_open(&fp,buf,FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
				if(FR_OK == Frt){

					Frt = f_write(&fp,(char *)(FaultFifo[i]),sizeof(FaultSaveStruct),&ByteWritten);	
					(FaultFifo[i]->DislayFlag) &= ~0x02;
					(*upt8)++;
					if((*upt8) >=	SDindexSize){
						(*upt8) = 0;
					}
					if((*upt8_1) < SDindexSize){
						(*upt8_1)++;
					}
				}
				f_close(&fp);	


			}

		}
	}

	if(SystemAlarm->VoltageAlarm.DislayFlag & 0x02){
		result = f_mount(&fs, "0:/", 0);
		if(FR_OK == result){
			buf[0] = 'V';
			buf[1] = 'A';
			len = sprintf(buf+2,"%d",SDExceedValue_index+1);
			buf[len+2] = lisname[0];
			buf[len+3] = lisname[1];
			buf[len+4] = lisname[2];
			buf[len+5] = lisname[3];
			buf[len+6] = 0;
				
			Frt = f_open(&fp,buf,FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
			if(FR_OK == Frt){
				Frt = f_write(&fp,(char *)&(SystemAlarm->VoltageAlarm),sizeof(struct _VoltageAlarm),&ByteWritten);
				SystemAlarm->VoltageAlarm.DislayFlag &= ~0x02;
				
				SDExceedValue_index++;
				if(SDExceedValue_index >= SDindexSize){
					SDExceedValue_index = 0;
				}
				if(SDExceedValue_sum < SDindexSize){
					SDExceedValue_sum++;
				}
			}
			f_close(&fp);
		}
			
	}
	/*
	if(SystemAlarm->GroundAlarm.DislayFlag & 0x02){
		result = f_mount(&fs, "0:/", 1);
		if(FR_OK == result){
			buf[0] = 'G';
			buf[1] = 'A';
			len = sprintf(buf+2,"%d",SDGroundFault_index+1);
			buf[len+2] = lisname[0];
			buf[len+3] = lisname[1];
			buf[len+4] = lisname[2];
			buf[len+5] = lisname[3];
			buf[len+6] = 0;
			//save3_loop:
			Frt = f_open(&fp,buf,FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
			if(FR_OK == Frt){
				#if 1
				//f_lseek(&fp,f_size(&fp));
				Frt = f_write(&fp,(char *)&(SystemAlarm->GroundAlarm),sizeof(struct _GroundAlarm),&ByteWritten);
				if(FR_OK != Frt){
						f_close(&fp);	
						//goto save3_loop;
				}
				#else
				f_write(&fp,(SystemAlarm->GroundAlarm.Wave),sizeof(int16_t)*8*20*256,&ByteWritten);
				f_write(&fp,&(SystemAlarm->GroundAlarm.Tim),sizeof(AlarmTimStruct)-2,&ByteWritten);							
				f_write(&fp,SystemAlarm->GroundAlarm.RMSValue,sizeof(u32)*8,&ByteWritten);							
				f_write(&fp,&SystemAlarm->GroundAlarm.ValueFlag,sizeof(u8),&ByteWritten);
				#endif
				SystemAlarm->GroundAlarm.DislayFlag &= ~0x02;							
				f_close(&fp);
				SDGroundFault_index++;
				if(SDGroundFault_index >= SDindexSize){
					SDGroundFault_index = 0;
				}
				if(SDGroundFault_sum < SDindexSize){
					SDGroundFault_sum++;
				}
			}
		}
			
	}
	*/
	if(SystemAlarm->ShortAlarm.DislayFlag & 0x02){
		result = f_mount(&fs, "0:/", 1);
		if(FR_OK == result){					
			buf[0] = 'S';
			buf[1] = 'A';
			len = sprintf(buf+2,"%d",SDShortFault_index+1);
			buf[len+2] = lisname[0];
			buf[len+3] = lisname[1];
			buf[len+4] = lisname[2];
			buf[len+5] = lisname[3];
			buf[len+6] = 0;
			
			Frt = f_open(&fp,buf,FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
			if(FR_OK == Frt){					
				Frt = f_write(&fp,(char *)&(SystemAlarm->ShortAlarm),sizeof(struct _ShortAlarm),&ByteWritten);
									
				SystemAlarm->ShortAlarm.DislayFlag &= ~0x02;							
				
				SDShortFault_index++;
				if(SDShortFault_index >= SDindexSize){
					SDShortFault_index = 0;
				}
				if(SDShortFault_sum < SDindexSize){
					SDShortFault_sum++;
				}
			}
			f_close(&fp);
		}
		
	}

	if(SystemAlarm->PTAlarm.DislayFlag & 0x02){
		result = f_mount(&fs, "0:/", 1);
		if(FR_OK == result){
			len = 0;
			if(SystemAlarm->PTAlarm.ValueFlag & 0x80){
				buf[len++] = 'U';
				buf[len++] = 'V';
				buf[len++] = 'A';						
				upt8 = &SDUnderValFault_index;
				upt8_1 = &SDUnderValFault_sum;
			}else{
				buf[len++] = 'T';
				buf[len++] = 'A';
				upt8 = &SDPTFault_index;
				upt8_1 = &SDPTFault_sum;						
			}
			len += sprintf(buf+len,"%d",(*upt8)+1);
			buf[len++] = lisname[0];
			buf[len++] = lisname[1];
			buf[len++] = lisname[2];
			buf[len++] = lisname[3];
			buf[len++] = 0;
		
			Frt = f_open(&fp,buf,FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
			if(FR_OK == Frt){
				
				Frt = f_write(&fp,(char *)&(SystemAlarm->PTAlarm),sizeof(struct _PTAlarm),&ByteWritten);					
				
				SystemAlarm->PTAlarm.DislayFlag &= ~0x02;						
				
				(*upt8)++;
				if((*upt8) >= SDindexSize){
					(*upt8) = 0;
				}
				if(*upt8_1 < SDindexSize){
					(*upt8_1)++;
				}
			}
			f_close(&fp);
		}
		
	}
}//SaveSDFault
#endif
/*
*********************************************************************************************************
*	�� �� ��: ReadSDFault
*	����˵��: �Ķ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ReadSDFault(void)
{
	uint8_t rt;
	uint8_t fileIndex;
	static uint8_t NumRead;
	if(StartReadSD_VA_Flag==1)
	{
		if(0 == SDExceedValue_sum){
			StartReadSD_VA_Flag = 0;
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(VA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
		}else{
			fileIndex = FIFO_Backpointer(SDExceedValue_index, VA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x03,fileIndex);
			if(0 == rt){
				StartReadSD_VA_Flag = 0;
				//=======��ȡ����
				WM_SendMessageNoPara(WM_GetClientWindow(VA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			}
		}

	}
	else
	//==================��ȡSD���ӵ�����
	if(StartReadSD_GA_Flag==1)
	{
		if(SDGroundFault_sum){
			if(0 == NumRead){
				fileIndex = FIFO_Backpointer(SDGroundFault_index, GA_WavePosition+1, NandFileSize);
				rt = ModuleComReadFault(0x01,fileIndex);
				if(0 == rt){
					NumRead++;
					//StartReadSD_GA_Flag=0;
					//=======��ȡ����
					//WM_SendMessageNoPara(WM_GetClientWindow(GA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
				}
			}else{
				fileIndex = FIFO_Backpointer(SDGroundFault_index, GA_WavePosition+1, NandFileSize);
				rt = ModuleComReadFault(0x07,fileIndex);
				if(0 == rt){
					NumRead = 0;
					StartReadSD_GA_Flag=0;
					//=======��ȡ����
					WM_SendMessageNoPara(WM_GetClientWindow(GA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
				}
			}
		}else{
			StartReadSD_GA_Flag=0;
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(GA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
		}

	}	
	else
	if(StartReadSD_GASEL_Flag==1)
	{
		if(SDGroundFault_sum){
			fileIndex = FIFO_Backpointer(SDGroundFault_index, GA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x07,fileIndex);
			if(0 == rt){
				StartReadSD_GASEL_Flag=0;
				//=======��ȡ����
			//	WM_SendMessageNoPara(WM_GetClientWindow(GA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
				WM_SendMessageNoPara(WM_GetClientWindow(SelGndCurrentW_hWin), WM_DisplayWave);
			}
		}else{
			StartReadSD_GASEL_Flag=0;
			//=======��ȡ����
			//WM_SendMessageNoPara(WM_GetClientWindow(GA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			WM_SendMessageNoPara(WM_GetClientWindow(SelGndCurrentW_hWin), WM_DisplayWave);
		}

	}
	else
	//==================��ȡSD����·����
	if(StartReadSD_SA_Flag==1)
	{
		if(SDShortFault_sum){
			fileIndex = FIFO_Backpointer(SDShortFault_index, SA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x06,fileIndex);
			if(0 == rt){
				StartReadSD_SA_Flag=0;
				//=======��ȡ����
				WM_SendMessageNoPara(WM_GetClientWindow(SA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			}
		}else{
			StartReadSD_SA_Flag=0;
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(SA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
		
		}
	}
	else
	//==================��ȡSD����������ѹ����
	if(StartReadSD_SVA_Flag==1)
	{	if(SDInstantaneousVoltageFault_sum){
			fileIndex = FIFO_Backpointer(SDInstantaneousVoltageFault_index, SVA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x02,fileIndex);
			if(0 == rt){
				StartReadSD_SVA_Flag=0;
				//=======��ȡ����
				WM_SendMessageNoPara(WM_GetClientWindow(RA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			}
		}else{
			StartReadSD_SVA_Flag=0;
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(RA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
		
		}
	}
	else
	//==================��ȡSD��PT��������
	if(StartReadSD_TA_Flag==1)
	{
		if(SDPTFault_sum){
			fileIndex = FIFO_Backpointer(SDPTFault_index, TA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x04,fileIndex);
			if(0 == rt){
				StartReadSD_TA_Flag=0;
				//=======��ȡ����
				WM_SendMessageNoPara(WM_GetClientWindow(TA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			}
		}else{
			StartReadSD_TA_Flag=0;
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(TA_hWin), WM_DisplayWave); //????????? WM_SetValue ??			
			
		}
	}
	else
	//==================��ȡSD��Ƿ��ѹ
	if(StartReadSD_UVA_Flag==1)
	{	if(SDUnderValFault_sum){
			fileIndex = FIFO_Backpointer(SDUnderValFault_index, UVA_WavePosition+1, NandFileSize);
			rt = ModuleComReadFault(0x05,fileIndex);
			if(0 == rt){
				StartReadSD_UVA_Flag=0;	
				//=======��ȡ����
				WM_SendMessageNoPara(WM_GetClientWindow(UA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
			}
		}else{
			StartReadSD_UVA_Flag=0;	
			//=======��ȡ����
			WM_SendMessageNoPara(WM_GetClientWindow(UA_hWin), WM_DisplayWave); //????????? WM_SetValue ??
		
		}
	}
}

void PlayAudio(void)
{
	static uint8_t SoundFlag;
	static uint16_t SaveTime;
	uint8_t hour,minute;
	uint16_t Time;
	SystemTime=SysGetTime();
	hour=SystemTime.hour;
	minute=SystemTime.minute;
	if(SystemSet.LcdSet.LcdFlag==1)
	{
		if(SoundFlag==0)
		{	
			SoundFlag=1;
			SYN_FrameInfo(0,"��ѹΣ�գ���ע�ⰲȫ");
			SaveTime=hour*60+minute;
		}
	}
	
	if(SoundFlag==1)
	{
		Time=hour*60+minute;
		if(Time<SaveTime)
			Time=(hour+24)*60+minute;
		if((Time-SaveTime)>=120)
			SoundFlag=0;	
	}
		
}
/*
*********************************************************************************************************
*	�� �� ��: UnicodeToUtf8
*	����˵��: �ַ�ת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int UnicodeToUtf8(char* pInput, char *pOutput)  
{  
	int len = 0; 
	while (*(u16 *)pInput)			
	{
		
		char low = *pInput;
		pInput++;
		char high = *pInput;
		//int w=high<<8;
		unsigned  wchar = (high<<8)+low;
 
		if (wchar <= 0x7F ) 
		{   
			pOutput[len] = (char)wchar;  
			len++;
		}  
		else if (wchar >=0x80 && wchar <= 0x7FF)  
		{  
			pOutput[len] = 0xc0 |((wchar >> 6)&0x1f);  
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  
			len++;
		}  
		else if (wchar >=0x800 && wchar < 0xFFFF)
		{
			pOutput[len] = 0xe0 | ((wchar >> 12)&0x0f);  
			len++;
			pOutput[len] = 0x80 | ((wchar >> 6) & 0x3f);  
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  
			len++;
		}
		else 
		{
			return -1;
		}
		pInput++;
	}	
	pOutput[len]= 0;
	return len;  
}//UnicodeToUtf8


/*
*********************************************************************************************************
*	�� �� ��: GetChipID
*	����˵��: ��ȡоƬID
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void	GetChipID(void)
{	
	ChipICID.ChipID[0] = *(__IO u32 *)(0x1FFF7A10);
	ChipICID.ChipID[1] = *(__IO u32 *)(0x1FFF7A14);
	ChipICID.ChipID[2] = *(__IO u32 *)(0x1FFF7A18);
	
	ChipICID.ChipFlashSize = *(__IO u32 *)(0x1FFF7A22);
	
}//GetChipID
/*
---------------------------------------------------------------------
Name		ScreenCloseDisplay
Function	��������
Input		No
OutPut	0 OK  1 ʱ�䵽
Edit by fanzhong
Date		2019-06-14
---------------------------------------------------------------------
*/
uint8_t ScreenCloseDisplay(void)
{
	uint8_t rt;
	rt = 0;
	if(ScreenCloseTim>=iScreenCloseTim){
		rt = 1;
	}
	return rt;
}//ScreenCloseDisplay


#if 0
/*
---------------------------------------------------------------------
Name		MemInit
Function	�ڴ�����ʼ��
Input		No
OutPut		No
Edit by fanzhong
Date		2018-10-12
---------------------------------------------------------------------
*/
void MemInit(void)
{
	Membuf = (MemStruct *)(Mem_SDRAM);
	Membuf->flag = 0;
}//MemInit

/*
---------------------------------------------------------------------
Name		MemGet
Function	�ڴ�ȡ��
Input		No
OutPut		��ַ
Edit by fanzhong
Date		2018-10-12
---------------------------------------------------------------------
*/
void *MemGet(void)
{
	void *pt;
	u8 i;
	for(i=0;i<16;i++){
		if(0 == ((Membuf->flag) & (1<<i))){
			(Membuf->flag) |= (1<<i);
			break;
		}
	}
	if(i<16){
		pt = (void *)(Membuf->Membuf[i]);
	}else{
		pt = 0;
	}
	return pt;	
}//MemGet
/*
---------------------------------------------------------------------
Name		MemPut
Function	�ڴ�黹
Input		��ַ
OutPut		No
Edit by fanzhong
Date		2018-10-12
---------------------------------------------------------------------
*/
void MemPut(void *pt)
{
	u8 i;
	for(i=0;i<16;i++){
		if(pt == Membuf->Membuf[i]){
			(Membuf->flag) &= ~(1<<i);
			break;
		}
	}	
}//MemPut
#endif


/*----------------------end----------------------------------------------*/

