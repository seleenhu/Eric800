/*
*********************************************************************************************************
*
*	模块名称 : 应用程序参数模块
*	文件名称 : param.c
*	版    本 : V1.0
*	说    明 : 读取和保存应用程序的参数
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-01-01 armfly  正式发布
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"


uint32_t const DefineId[20][12]={
		'A'<<24|1001,'A'<<24|1002,'A'<<24|1003,'A'<<24|1005,'A'<<24|1006,'A'<<24|1008,'A'<<24|1009,'A'<<24|1010,'A'<<24|1011,'A'<<24|1012,'A'<<24|1014,'B'<<24|0x00000b,//'A'<<24|0x00000c,'A'<<24|0x00000d,'A'<<24|0x00000e,'A'<<24|0x00000f,
		'A'<<24|1015,'A'<<24|1016,'A'<<24|1017,'A'<<24|1018,'A'<<24|1019,'A'<<24|1020,'A'<<24|1021,'A'<<24|1022,'A'<<24|1023,'A'<<24|1025,'A'<<24|1026,'B'<<24|0x000001,//'A'<<24|0x00001c,'A'<<24|0x00001d,'A'<<24|0x00001e,'A'<<24|0x00001f,
		'A'<<24|1027,'A'<<24|1028,'A'<<24|1029,'A'<<24|1030,'A'<<24|0x000024,'A'<<24|0x000025,'A'<<24|0x000026,'A'<<24|0x000027,'A'<<24|0x000028,'A'<<24|0x000029,'A'<<24|0x00002a,'B'<<24|0x00002b,//'A'<<24|0x00002c,'A'<<24|0x00002d,'A'<<24|0x00002e,'A'<<24|0x00002f,
		'A'<<24|0x000030,'A'<<24|0x000031,'A'<<24|0x000032,'A'<<24|0x000033,'A'<<24|0x000034,'A'<<24|0x000035,'A'<<24|0x000036,'A'<<24|0x000037,'A'<<24|0x000038,'A'<<24|0x000039,'A'<<24|0x00003a,'B'<<24|0x00003b,//'A'<<24|0x00003c,'A'<<24|0x00003d,'A'<<24|0x00003e,'A'<<24|0x00003f,
		'A'<<24|0x000040,'A'<<24|0x000041,'A'<<24|0x000042,'A'<<24|0x000043,'A'<<24|0x000044,'A'<<24|0x000045,'A'<<24|0x000046,'A'<<24|0x000047,'A'<<24|0x000048,'A'<<24|0x000049,'A'<<24|0x00004a,'B'<<24|0x00004b,//'A'<<24|0x00004c,'A'<<24|0x00004d,'A'<<24|0x00004e,'A'<<24|0x00004f,
		'A'<<24|0x000050,'A'<<24|0x000051,'A'<<24|0x000052,'A'<<24|0x000053,'A'<<24|0x000054,'A'<<24|0x000055,'A'<<24|0x000056,'A'<<24|0x000057,'A'<<24|0x000058,'A'<<24|0x000059,'A'<<24|0x00005a,'B'<<24|0x00005b,//'A'<<24|0x00005c,'A'<<24|0x00005d,'A'<<24|0x00005e,'A'<<24|0x00005f,
		'A'<<24|0x000060,'A'<<24|0x000061,'A'<<24|0x000062,'A'<<24|0x000063,'A'<<24|0x000064,'A'<<24|0x000065,'A'<<24|0x000066,'A'<<24|0x000067,'A'<<24|0x000068,'A'<<24|0x000069,'A'<<24|0x00006a,'B'<<24|0x00006b,//'A'<<24|0x00006c,'A'<<24|0x00006d,'A'<<24|0x00006e,'A'<<24|0x00006f,
		'A'<<24|0x000070,'A'<<24|0x000071,'A'<<24|0x000072,'A'<<24|0x000073,'A'<<24|0x000074,'A'<<24|0x000075,'A'<<24|0x000076,'A'<<24|0x000077,'A'<<24|0x000078,'A'<<24|0x000079,'A'<<24|0x00007a,'B'<<24|0x00007b,//'A'<<24|0x00007c,'A'<<24|0x00007d,'A'<<24|0x00007e,'A'<<24|0x00007f,
		'A'<<24|0x000080,'A'<<24|0x000081,'A'<<24|0x000082,'A'<<24|0x000083,'A'<<24|0x000084,'A'<<24|0x000085,'A'<<24|0x000086,'A'<<24|0x000087,'A'<<24|0x000088,'A'<<24|0x000089,'A'<<24|0x00008a,'B'<<24|0x00008b,//'A'<<24|0x00008c,'A'<<24|0x00008d,'A'<<24|0x00008e,'A'<<24|0x00008f,
		'A'<<24|0x000090,'A'<<24|0x000091,'A'<<24|0x000092,'A'<<24|0x000093,'A'<<24|0x000094,'A'<<24|0x000095,'A'<<24|0x000096,'A'<<24|0x000097,'A'<<24|0x000098,'A'<<24|0x000099,'A'<<24|0x00009a,'B'<<24|0x00009b,//'A'<<24|0x00009c,'A'<<24|0x00009d,'A'<<24|0x00009e,'A'<<24|0x00009f,
		'A'<<24|0x0000a0,'A'<<24|0x0000a1,'A'<<24|0x0000a2,'A'<<24|0x0000a3,'A'<<24|0x0000a4,'A'<<24|0x0000a5,'A'<<24|0x0000a6,'A'<<24|0x0000a7,'A'<<24|0x0000a8,'A'<<24|0x0000a9,'A'<<24|0x0000aa,'B'<<24|0x0000ab,//'A'<<24|0x0000ac,'A'<<24|0x0000ad,'A'<<24|0x0000ae,'A'<<24|0x0000af,
		'A'<<24|0x0000b0,'A'<<24|0x0000b1,'A'<<24|0x0000b2,'A'<<24|0x0000b3,'A'<<24|0x0000b4,'A'<<24|0x0000b5,'A'<<24|0x0000b6,'A'<<24|0x0000b7,'A'<<24|0x0000b8,'A'<<24|0x0000b9,'A'<<24|0x0000ba,'B'<<24|0x0000bb,//'A'<<24|0x0000bc,'A'<<24|0x0000bd,'A'<<24|0x0000be,'A'<<24|0x0000bf,
		'A'<<24|0x0000c0,'A'<<24|0x0000c1,'A'<<24|0x0000c2,'A'<<24|0x0000c3,'A'<<24|0x0000c4,'A'<<24|0x0000c5,'A'<<24|0x0000c6,'A'<<24|0x0000c7,'A'<<24|0x0000c8,'A'<<24|0x0000c9,'A'<<24|0x0000ca,'B'<<24|0x0000cb,//'A'<<24|0x0000cc,'A'<<24|0x0000cd,'A'<<24|0x0000ce,'A'<<24|0x0000cf,
		'A'<<24|0x0000d0,'A'<<24|0x0000d1,'A'<<24|0x0000d2,'A'<<24|0x0000d3,'A'<<24|0x0000d4,'A'<<24|0x0000d5,'A'<<24|0x0000d6,'A'<<24|0x0000d7,'A'<<24|0x0000d8,'A'<<24|0x0000d9,'A'<<24|0x0000da,'B'<<24|0x0000db,//'A'<<24|0x0000dc,'A'<<24|0x0000dd,'A'<<24|0x0000de,'A'<<24|0x0000df,
		'A'<<24|0x0000e0,'A'<<24|0x0000e1,'A'<<24|0x0000e2,'A'<<24|0x0000e3,'A'<<24|0x0000e4,'A'<<24|0x0000e5,'A'<<24|0x0000e6,'A'<<24|0x0000e7,'A'<<24|0x0000e8,'A'<<24|0x0000e9,'A'<<24|0x0000ea,'B'<<24|0x0000eb,//'A'<<24|0x0000ec,'A'<<24|0x0000ed,'A'<<24|0x0000ee,'A'<<24|0x0000ef,
		'A'<<24|0x0000f0,'A'<<24|0x0000f1,'A'<<24|0x0000f2,'A'<<24|0x0000f3,'A'<<24|0x0000f4,'A'<<24|0x0000f5,'A'<<24|0x0000f6,'A'<<24|0x0000f7,'A'<<24|0x0000f8,'A'<<24|0x0000f9,'A'<<24|0x0000fa,'B'<<24|0x0000fb,//'A'<<24|0x0000fc,'A'<<24|0x0000fd,'A'<<24|0x0000fe,'A'<<24|0x0000ff,
		'A'<<24|0x000100,'A'<<24|0x000101,'A'<<24|0x000102,'A'<<24|0x000103,'A'<<24|0x000104,'A'<<24|0x000105,'A'<<24|0x000106,'A'<<24|0x000107,'A'<<24|0x000108,'A'<<24|0x000109,'A'<<24|0x00010a,'B'<<24|0x00010b,//'A'<<24|0x00010c,'A'<<24|0x00010d,'A'<<24|0x00010e,'A'<<24|0x00010f,
		'A'<<24|0x000110,'A'<<24|0x000111,'A'<<24|0x000112,'A'<<24|0x000113,'A'<<24|0x000114,'A'<<24|0x000115,'A'<<24|0x000116,'A'<<24|0x000117,'A'<<24|0x000118,'A'<<24|0x000119,'A'<<24|0x00011a,'B'<<24|0x00011b,//'A'<<24|0x00011c,'A'<<24|0x00011d,'A'<<24|0x00011e,'A'<<24|0x00011f,
		'A'<<24|0x000120,'A'<<24|0x000121,'A'<<24|0x000122,'A'<<24|0x000123,'A'<<24|0x000124,'A'<<24|0x000125,'A'<<24|0x000126,'A'<<24|0x000127,'A'<<24|0x000128,'A'<<24|0x000129,'A'<<24|0x00012a,'B'<<24|0x00012b,//'A'<<24|0x00012c,'A'<<24|0x00012d,'A'<<24|0x00012e,'A'<<24|0x00012f,
		'A'<<24|0x000130,'A'<<24|0x000131,'A'<<24|0x000132,'A'<<24|0x000133,'A'<<24|0x000134,'A'<<24|0x000135,'A'<<24|0x000136,'A'<<24|0x000137,'A'<<24|0x000138,'A'<<24|0x000139,'A'<<24|0x00013a,'B'<<24|0x00013b//,'A'<<24|0x00013c,'A'<<24|0x00013d,'A'<<24|0x00013e,'A'<<24|0x00013f
};
//PARAM_T g_tParam;

/* 将16KB 一个扇区的空间预留出来做为参数区 For MDK */
//const uint8_t para_flash_area[16*1024] __attribute__((at(ADDR_FLASH_SECTOR_3)));

/*
*********************************************************************************************************
*	函 数 名: LoadParam
*	功能说明: 从Flash读参数到g_tParam
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LoadParam(void)
{
#if 0
#ifdef PARAM_SAVE_TO_FLASH
	/* 读取CPU Flash中的参数 */
	//bsp_ReadCpuFlash(PARAM_ADDR, (uint8_t *)&g_tParam, sizeof(PARAM_T));
	
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* 读取EEPROM中的参数 */
	ee_ReadBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif

	/* 填充缺省参数 */
	if (g_tParam.ParamVer != PARAM_VER)
	{
		g_tParam.ParamVer = PARAM_VER;

		g_tParam.ucBackLight = 200;

		/* uip  本机IP地址 */
		g_tParam.uip_ip[0] = 192;
		g_tParam.uip_ip[1] = 168;
		g_tParam.uip_ip[2] = 1;
		g_tParam.uip_ip[3] = 85;

		/* uip  子网掩码 */
		g_tParam.uip_net_mask[0] = 255;
		g_tParam.uip_net_mask[1] = 255;
		g_tParam.uip_net_mask[2] = 255;
		g_tParam.uip_net_mask[3] = 0;

		/* uip  默认网关 */
		g_tParam.uip_gateway[0] = 192;
		g_tParam.uip_gateway[1] = 168;
		g_tParam.uip_gateway[2] = 1;
		g_tParam.uip_gateway[3] = 1;

		/* lwip  本机IP地址 */
		g_tParam.lwip_ip[0] = 192;
		g_tParam.lwip_ip[1] = 168;
		g_tParam.lwip_ip[2] = 1;
		g_tParam.lwip_ip[3] = 86;

		/* lwip  子网掩码 */
		g_tParam.lwip_net_mask[0] = 255;
		g_tParam.lwip_net_mask[1] = 255;
		g_tParam.lwip_net_mask[2] = 255;
		g_tParam.lwip_net_mask[3] = 0;

		/* lwip  默认网关 */
		g_tParam.lwip_gateway[0] = 192;
		g_tParam.lwip_gateway[1] = 168;
		g_tParam.lwip_gateway[2] = 1;
		g_tParam.lwip_gateway[3] = 1;

		g_tParam.ucRadioMode     = 0;				/* 1 = AM 或 0 = FM */
		g_tParam.ucRadioListType = 0;				/* 电台列表类型。0武汉地区或1全国 */
		g_tParam.ucIndexFM       = 0;				/* 当前FM电台索引 */
		g_tParam.ucIndexAM       = 0;				/* 当前电台索引 */
		g_tParam.ucRadioVolume   = 30;				/* 音量 */
		g_tParam.ucSpkOutEn      = 1;				/* 扬声器输出使能 */

		/* 485 通信波特率 */
		g_tParam.Baud485 = 9600;

		SaveParam();							/* 将新参数写入Flash */
	}
#endif
	u8 i,j;
#if 0
	ADCFifo->bt = 0;
	ADCFifo->pt = 0;
	ADCFifo->Calbt = 0;
	ADCFifo->Faultbt = 0;
	ADCFifo->Instantbt = 0;
	ADCFifo->FifoSize = ADCFifo_SDRAM_SIZE/2;
	ADCFifo->RMS_buf[0] = 0;
	ADCFifo->RMS_buf[1] = 0;
	ADCFifo->RMS_buf[2] = 0;
	ADCFifo->RMS_buf[3] = 0;
	ADCFifo->RMS_buf[4] = 0;
	ADCFifo->RMS_buf[5] = 0;
	ADCFifo->RMS_buf[6] = 0;
	ADCFifo->RMS_buf[7] = 0;

	ADCFifo->rati0[0] = 68.3887863;
	ADCFifo->rati0[1] = 68.3887863;
	ADCFifo->rati0[2] = 68.3887863;
	ADCFifo->rati0[3] = 194.454239;
	ADCFifo->rati0[4] = 2021.42578;
	ADCFifo->rati0[5] = 2027.51135;
	ADCFifo->rati0[6] = 2023.09204;
	ADCFifo->rati0[7] = 3802.24146;
	
	ADCFifo->fftratio[0] = 68.73;
	ADCFifo->fftratio[1] = 69.27;
	ADCFifo->fftratio[2] = 67.99;
	ADCFifo->fftratio[3] = 196.59;
	ADCFifo->fftratio[4] = 2015.54;
	ADCFifo->fftratio[5] = 2021.48;
	ADCFifo->fftratio[6] = 2018.51;
	ADCFifo->fftratio[7] = 3800.27;	
#endif	
	sf_ReadBuffer((uint8_t *)&SystemSet, PARAM_ADDR, sizeof(Setstruct));	
	if(PARAM_VER == SystemSet.ParamVer){		
		g_tTP.usAdcX1 = SystemSet.usAdcX1;
		g_tTP.usAdcY1 = SystemSet.usAdcY1;
		g_tTP.usAdcX2 = SystemSet.usAdcX2;
		g_tTP.usAdcY2 = SystemSet.usAdcY2;
		g_tTP.usAdcX3 = SystemSet.usAdcX3;
		g_tTP.usAdcY3 = SystemSet.usAdcY3;
		g_tTP.usAdcX4 = SystemSet.usAdcX4;
		g_tTP.usAdcY4 = SystemSet.usAdcY4;

		g_tTP.usLcdX1 = SystemSet.usLcdX1;
		g_tTP.usLcdY1 = SystemSet.usLcdY1;
		g_tTP.usLcdX2 = SystemSet.usLcdX2;
		g_tTP.usLcdY2 = SystemSet.usLcdY2;
		g_tTP.usLcdX3 = SystemSet.usLcdX3;
		g_tTP.usLcdY3 = SystemSet.usLcdY3;
		g_tTP.usLcdX4 = SystemSet.usLcdX4;
		g_tTP.usLcdY4 = SystemSet.usLcdY4;

		g_tTP.XYChange = SystemSet.XYChange;

		g_LcdDirection = SystemSet.TouchDirection;

		//memcpy(ADCFifo->rati0,SystemSet.ADCParameter.ratio,8*sizeof(float));
		//memcpy(ADCFifo->fftratio,SystemSet.ADCParameter.fftratio,8*sizeof(float));
		SystemSet.ParamSaveFlag = 0x00;
	}else{
		SystemSet.BackLightTime = 1;
		
		SystemSet.NetSet.IPadrr[0] = 118;
		SystemSet.NetSet.IPadrr[1] = 31;
		SystemSet.NetSet.IPadrr[2] = 67;
		SystemSet.NetSet.IPadrr[3] = 82;		
		SystemSet.NetSet.Port = 8802;
		SystemSet.NetSet.Modbusadr = 0xBBFF;
		memcpy(SystemSet.NetSet.ModbudID,(void *)"ERIC0820190900000004",20);
		SystemSet.NetSet.NetSelectFlag = 0;
		SystemSet.VoltageSet.MaxRatedValue = 10000;
		SystemSet.VoltageSet.Modulus = 1.2;

		SystemSet.RS485Set.Baud = 9600;
		SystemSet.RS485Set.adr = 1;
		
		SystemSet.RS232Set.Baud = 9600;

		SystemSet.ShortGNDSet.I0 = 20;
		for(i=0;i<20;i++){
			for(j=0;j<12;j++){
				SystemSet.CupboardID[i][j] = DefineId[i][j];
			}
		}
		
		
		
//=====================
		
		
		Setting.HumiditySet.MaxValue=90;
		Setting.HumiditySet.MinValue=50;
		Setting.TempSet.MaxValue=80;
		Setting.TempSet.MinValue=15;
		Setting.LeakageCurrentSet.LeakageValue=20;
		SystemSet.LineTempMax=100;
		Setting.RatioStruct.RatioCT=100;
		Setting.RatioStruct.RatioPT=100;
		SystemSet.DriveTestFlag=0;//中断传动试验
		SystemSet.DistanceFlag=1;//初始远方模式
		
		
		Setting.Passwords[0]=1;
		Setting.Passwords[1]=2;
		Setting.Passwords[2]=3;
		Setting.Passwords[3]=4;
		
		
		SystemSet.SafeSoundTime=15;
		SystemSet.TouchToneFlag=0;
		
		SystemSet.NetSet.NetSelectFlag=0;//0:4G
		
		
		
		for(i=0;i<20;i++)
		{
			
			SystemSet.ActionCnAD[i]=0;
			SystemSet.ActionCnBD[i]=0;
			SystemSet.ActionCnCD[i]=0;
			SystemSet.ActionCnAB[i]=0;
			SystemSet.ActionCnBC[i]=0;
			SystemSet.ActionCnAC[i]=0;
			SystemSet.ActionTimeAD[i]=0;
			SystemSet.ActionTimeBD[i]=0;	
			SystemSet.ActionTimeCD[i]=0;
			SystemSet.ActionTimeAB[i]=0;
			SystemSet.ActionTimeBC[i]=0;
			SystemSet.ActionTimeAC[i]=0;
			
		}
		
		memset(SystemSet.LineNamebuf,0,50);
		SystemSet.LineNamebuf[0] = 0x08;
		SystemSet.LineNamebuf[1] = 0x54;
		SystemSet.LineNamebuf[2] = 0xA5;
		SystemSet.LineNamebuf[3] = 0x80;
		SystemSet.LineNamebuf[4] = 0x9B;
		SystemSet.LineNamebuf[5] = 0x4F;
		SystemSet.LineNamebuf[6] = 0x35;
		SystemSet.LineNamebuf[7] = 0x75;	
		SystemSet.LineNamebuf[8] = 0x31;
		SystemSet.LineNamebuf[9] = 0x00;
		SystemSet.LineNamebuf[10] = 0x23;
		SystemSet.LineNamebuf[11] = 0x00;
		SystemSet.LineNamebuf[12] = 0xBF;
		SystemSet.LineNamebuf[13] = 0x7E;
		//memcpy(SystemSet.ADCParameter.ratio,ADCFifo->rati0,8*sizeof(float));
		//memcpy(SystemSet.ADCParameter.fftratio,ADCFifo->fftratio,8*sizeof(float));
		SystemSet.ParamSaveFlag = FlagSystemState;

		SystemSet.CupboardSIZE = 20;
	}
	if(SystemSet.CupboardSIZE>20){
		SystemSet.CupboardSIZE = 20;
	}	
	for(i=0;i<16;i++){
		if((SystemSet.SelRatioCTBuf[i] >= 5000)||(0 == SystemSet.SelRatioCTBuf[i])){
			SystemSet.SelRatioCTBuf[i] = 100;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: SaveParam
*	功能说明: 将全局变量g_tParam 写入到CPU内部Flash
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SaveParam(void)
{
#ifdef PARAM_SAVE_TO_FLASH
	/* 将全局的参数变量保存到 CPU Flash */
	//bsp_WriteCpuFlash(PARAM_ADDR, (unsigned char *)&g_tParam, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* 将全局的参数变量保存到EEPROM */
	ee_WriteBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_SPIFLASH 
	/* 将全局的参数变量保存到 SPI Flash */
	
	SystemSet.ParamVer = PARAM_VER;
	sf_WriteBuffer((uint8_t *)&SystemSet, PARAM_ADDR, sizeof(Setstruct));
#endif

}
/*
*********************************************************************************************************
*	函 数 名: SaveParamTouch
*	功能说明: 将全局变量g_tParam 写入到CPU内部Flash
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SaveParamTouch(void)
{
#ifdef PARAM_SAVE_TO_FLASH
	/* 将全局的参数变量保存到 CPU Flash */
	//bsp_WriteCpuFlash(PARAM_ADDR, (unsigned char *)&g_tParam, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* 将全局的参数变量保存到EEPROM */
	ee_WriteBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_SPIFLASH 
	/* 将全局的参数变量保存到 SPI Flash */
	
	SystemSet.ParamVer = PARAM_VER+1;
	sf_WriteBuffer((uint8_t *)&SystemSet, PARAM_ADDR, sizeof(Setstruct));
#endif

}
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
