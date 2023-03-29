

#include "mydefine.h"


BITS81 TimData;										
uint16_t uSCount;

		
const char EEPromBuf[16*1024] __attribute__ ((at(0x0800C000)));

EepromStruct EepromData = {
							.PassWordKey = EepromPassWord,						
							.ratio = 250.071213,
						  };

uint8_t EepromSaveFlag;





uint8_t FaultbufLen;
CanComStaruct *CanComData = (CanComStaruct *)(0x10000000);

//FaultStruct Faultbuf[2] __attribute__ ((at(0x10000000)));
//FaultStruct Faultbuf1;


/*
---------------------------------------------------------------------
Name		FIFO_length
Function	FIFOµÄ³¤¶È
Input		¶Á³öÖ¸Õë£¬Ð´ÈëÖ¸Õë£¬×Ü³¤
OutPut	FIFOµÄ³¤¶È
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
uint16_t FIFO_length(uint16_t bt,uint16_t pt,uint16_t len)
{
	if(pt>=bt){
		return (pt-bt);
	}
	return (len-(bt-pt));
}//FIFO_length
/*
---------------------------------------------------------------------
Name		FIFO_pointer
Function	FIFOµÄÖ¸ÕëÏà¼û
Input		Ö¸Õë£¬×Ü³¤
OutPut	Ö¸Õëµ±Ç°Öµ
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
uint16_t FIFO_pointer(uint16_t pointer,uint16_t len,uint16_t sizelen)
{
#if 0
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
Function	FIFOºóÍËµÄ³¤¶È
Input		Ö¸Õë£¬×Ü³¤
OutPut	Ö¸Õëµ±Ç°Öµ
Edit by fanzhong
Date		2018-09-05
---------------------------------------------------------------------
*/
uint16_t FIFO_Backpointer(uint16_t pointer,uint16_t len,uint16_t sizelen)
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
Name		FIFO_bufcmp
Function	FIFOÓëÖ¸¶¨buf±È½Ï
Input		Ö¸Õë£¬×Ü³¤
OutPut	0 OK  >=1 Err
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
uint8_t FIFO_bufcmp(uint8_t *FIFObuf,uint8_t *buf,uint16_t bt,uint16_t len,uint16_t sizelen)
{
	uint16_t i;
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
Function	16½øÖÆµ½ascii
Input		±»×ª»»µÄÊýÖµ£buf¬
OutPut	³¤¶È
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
Function	ÄæÐòCRC16
Input		
				pData Êý¾Ý
				nLength Êý¾Ý³¤¶È
				init ³õÊ¼»¯Êý¾Ý
				ptable ²éÕÒ±í
OutPut	CRCÊý¾Ý
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
static uint16_t GetRevCrc_16(uint8_t * pData, uint16_t nLength,uint16_t init, const uint16_t *ptable)
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
Function	CRC¼ÆËã
Input		
				pData Êý¾Ý
				nLength Êý¾Ý³¤¶È
				
OutPut	CRCÊý¾Ý
Edit by fanzhong
Date		2017-12-04
---------------------------------------------------------------------
*/
uint16_t CRC_GetModbus16(uint8_t *pdata, uint16_t len)
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
Function	asciiµ½16½øÖÆ
Input		±»×ª»»µÄÊýÖµ£buf¬
OutPut	³¤¶È
Edit by fanzhong
Date		2017-11-30
---------------------------------------------------------------------
*/
uint32_t AsciiToHex(uint8_t *Asciibuf,uint8_t Asciibuf_len)
{
	uint32_t data32;
	data32 = 0;
	while(Asciibuf_len--){
		data32 = data32*10 + *Asciibuf -'0';
		Asciibuf++;
	}
	return data32;
}//AsciiToHex

/*
****************************************************
o¡¥¨ºy??3?¡êoFlashWrite
1|?¨¹?¨¨¨º?¡êoD¡ä¨¨?FLASH          
¨º?¨¨?2?¨ºy¡êoD¡ä¨¨?2?¨ºy
¨º?3?2?¨ºy¡êoNo
¡¤¦Ì ?? ?¦Ì¡êo0 3¨¦1|   >= 1 2?3¨¦1|
¡Á¡é¨°a¨º???¡êo
¡Á¡Â    ??¡êofanzhong
¨¨?    ?¨²¡êo2018-11-13
DT??????¡êo
****************************************************
*/
uint8_t FlashWrite(void)
{
	uint8_t *pt;
	uint16_t  i;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	if(FLASH_COMPLETE != FLASH_EraseSector(((uint16_t)0x0018),VoltageRange_3)){
		
		return 1;
	}	

	pt = (uint8_t *)(&EepromData);
	for(i=0;i<sizeof(EepromStruct);i++){		
		if(FLASH_COMPLETE != FLASH_ProgramByte(0x0800C000+i,pt[i])){
			return 2;
		}
	}
	FLASH_Lock();
	
	return 0;
}//FlashWrite

/*
****************************************************
o¡¥¨ºy??3?¡êoEepromDataInit
1|?¨¹?¨¨¨º?¡êo?¨¢FLASH          
¨º?¨¨?2?¨ºy¡êoNo
¨º?3?2?¨ºy¡êoNo
¡¤¦Ì ?? ?¦Ì¡êoNo
¡Á¡é¨°a¨º???¡êo
¡Á¡Â    ??¡êofanzhong
¨¨?    ?¨²¡êo2018-11-13
DT??????¡êo
****************************************************
*/
void EepromDataInit(void)
{
	memcpy((uint8_t *)(&EepromData),EEPromBuf,sizeof(EepromStruct));
	if(EepromPassWord != EepromData.PassWordKey){
		EepromData.PassWordKey = EepromPassWord;		
		EepromData.ratio =  244.93515;
		EepromData.CanID = 0x301;
	}
}//EepromDataInit

void TestPinInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ´ò¿ªGPIOÊ±ÖÓ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/*
		ÅäÖÃËùÓÐµÄLEDÖ¸Ê¾µÆGPIOÎªÍÆÍìÊä³öÄ£Ê½
		ÓÉÓÚ½«GPIOÉèÖÃÎªÊä³öÊ±£¬GPIOÊä³ö¼Ä´æÆ÷µÄÖµÈ±Ê¡ÊÇ0£¬Òò´Ë»áÇý¶¯LEDµãÁÁ.
		ÕâÊÇÎÒ²»Ï£ÍûµÄ£¬Òò´ËÔÚ¸Ä±äGPIOÎªÊä³öÇ°£¬ÏÈ¹Ø±ÕLEDÖ¸Ê¾µÆ
	*/
		

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ÉèÎªÊä³ö¿Ú */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ÉèÎªÍÆÍìÄ£Ê½ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ÉÏÏÂÀ­µç×è²»Ê¹ÄÜ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO¿Ú×î´óËÙ¶È */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	Test1Clr();
	Test2Clr();
	//Test1Set();
	//Test2Set();
}//TestPinInit
/*----------------------end----------------------------------------------*/

