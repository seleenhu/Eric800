/*
*********************************************************************************************************
*
*	模块名称 : fatfs和sd卡的接口文件
*	文件名称 : diskio.c
*	版    本 : V1.0
*	说    明 : 支持SD卡
*
*	修改记录 :
*		版本号   日期         作者           说明
*       v1.0    2014-06-19   Eric2013        首发
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "diskio.h"		    /* FatFs lower layer API */
#include "bsp.h"


#define SECTOR_SIZE		512
/*-------------------------------------------------------------------------------------------*/
/* Inidialize a Drive                                                                        */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :		/* SD卡 */
			if (SD_Init() == SD_OK)
			{
				stat = RES_OK;
			}
			else
			{
				stat = STA_NODISK;
			}
			break;

		default :
			break;
	}

	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Get Disk Status                                                                           */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :
			stat = 0;
			break;

		default:
			stat = 0;
			break;
	}
	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Read Sector(s)                                                                            */
/*-------------------------------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res = RES_OK;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_ReadBlock(buff, sector << 9 , SECTOR_SIZE);
			}
			else
			{
				Status = SD_ReadMultiBlocks(buff, sector << 9 , SECTOR_SIZE, count);
			}
			
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

		#ifdef SD_DMA_MODE
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif

			res = RES_OK;
			break;
		}
		
		default:
			res = RES_PARERR;
			break;
	}
	
	return res;
}

/*-------------------------------------------------------------------------------------------*/
/* Write Sector(s)                                                                           */
/*-------------------------------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = RES_OK;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);
			}
			else
			{
				/* 此处存在疑问： 扇区个数如果写 count ，将导致最后1个block无法写入 */
				//Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);
				Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count + 1);
			}
			
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

		#ifdef SD_DMA_MODE
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			Status = SD_WaitWriteOperation();
			//Status = SD_WaitReadOperation();
			
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}
			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif
			
			res = RES_OK;
			break;
		}

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif


/*-------------------------------------------------------------------------------------------*/
/* Miscellaneous Functions                                                                   */
/*-------------------------------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case FS_SD :
		/* SD卡磁盘容量： SDCardInfo.CardCapacity */
		res = RES_OK;
		return res;
	}

	return RES_PARERR;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2013-01-01 00:00:00 */
	#if 1
	RTC_TimeTypeDef rtctim;
	RTC_DateTypeDef rtcdate;
	
	
	RTC_GetTime(RTC_Format_BIN,&rtctim);
	RTC_GetDate(RTC_Format_BIN,&rtcdate);
	
	return	  ((DWORD)(rtcdate.RTC_Year + 2000-1980) << 25)	/* Year = 2013 */
			| ((DWORD)rtcdate.RTC_Month << 21)				/* Month = 1 */
			| ((DWORD)rtcdate.RTC_Date << 16)				/* Day_m = 1*/
			| ((DWORD)rtctim.RTC_Hours << 11)				/* Hour = 0 */
			| ((DWORD)rtctim.RTC_Minutes << 5)				/* Min = 0 */
			| ((DWORD)rtctim.RTC_Seconds >> 1);				/* Sec = 0 */
	
	#else
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
 #endif
}
