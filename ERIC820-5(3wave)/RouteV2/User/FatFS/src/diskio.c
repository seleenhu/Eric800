/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "bsp.h"			/* 底层驱动， 提供SD, USB, NAND 驱动 */
//#include "usbh_bsp_msc.h"	/* 提供U盘的读写函数 */

//#define ff_printf	printf
#define ff_printf(...)

#define SECTOR_SIZE		512	/* SD卡扇区大小必须为512 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :
			stat = 0;
			break;

		case FS_NAND :
			stat = 0;
			break;
		case FS_SPIFLASH:
			stat = 0;
			break;
		default:
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
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

		case FS_NAND :		/* NAND Flash */
			if (NAND_Init() == NAND_OK)
			{
				stat = RES_OK;
			}
			else
			{
				/* 如果初始化失败，请执行低级格式化 */
				//printf("NAND_Init() Error!  \r\n");
				
				stat = RES_ERROR;
			}
			break;
		case FS_SPIFLASH:
			bsp_InitSFlash();
			stat = RES_OK;
			break;
			
		default:
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

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

		case FS_NAND :
			if (NAND_OK == NAND_ReadMultiSectors(buff, sector, 512, count))
			{
				res = RES_OK;
			}
			else
			{
				//printf("NAND_ReadMultiSectors() Error! sector = %d, count = %d \r\n", sector, count);
				res = RES_ERROR;
			}
			break;
		case FS_SPIFLASH:			
			res = RES_OK;			
			break;
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);

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
			}
			else
			{
				/* 此处存在疑问： 扇区个数如果写 count ，将导致最后1个block无法写入 */
				//Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);
				Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count + 1);

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
			}
			break;
		}

		case FS_NAND :
			if (NAND_OK == NAND_WriteMultiSectors((uint8_t *)buff, sector, 512, count))
			{
				res = RES_OK;
			}
			else
			{
				//printf("NAND_WriteMultiSectors() Error! sector = %d, count = %d \r\n", sector, count);
				res = RES_ERROR;
			}
			break;
		case FS_SPIFLASH:
			res = RES_PARERR;
			
			break;

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

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

	case FS_NAND :
		{
			//DRESULT res = RES_OK;

			res = RES_ERROR;
			switch (cmd)
			{
				case CTRL_SYNC :		/* Make sure that no pending write process */
					res = RES_OK;
					break;

				case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
					*(DWORD*)buff = 262144;
					res = RES_OK;
					break;

				case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
					*(WORD*)buff = 512;
					res = RES_OK;
					break;

				case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */\
					*(DWORD*)buff = 512;
					res = RES_OK;
					break;

				default:
					res = RES_PARERR;
					break;
			}
			return res;			
		}
	case FS_SPIFLASH:
		
		
		res = RES_PARERR;
		break;

	}
	return RES_PARERR;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。客户可以自行移植和系统的RTC关联起来
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2014-07-02 00:00:00 */
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
	return	  ((DWORD)(2014 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)7 << 21)				/* Month = 1 */
			| ((DWORD)2 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
#endif
}


