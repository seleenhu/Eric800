
#include "mydefine.h"

uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10;
	return FLASH_Sector_11;
}//STMFLASH_GetFlashSector

void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
	FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return; //非法地址
	FLASH_Unlock(); //解锁
	FLASH_DataCacheCmd(DISABLE);//FLASH 擦除期间,必须禁止数据缓存
	addrx=WriteAddr; //写入的起始地址
	endaddr=WriteAddr+NumToWrite*4; //写入的结束地址
	if(addrx<0X1FFF0000) //只有主存储区,才需要执行擦除操作!!
	{
		while(addrx<endaddr) //扫清一切障碍.(对非 FFFFFFFF 的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)
			//有非 0XFFFFFFFF 的地方,要擦除这个扇区
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);
				//VCC=2.7~3.6V 之间!!
				if(status!=FLASH_COMPLETE)break;//发生错误了
			}else addrx+=4;
		}
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
			{
				break; //写入异常
			}
			WriteAddr+=4; pBuffer++;
		}
	}
	FLASH_DataCacheCmd(ENABLE); //FLASH 擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}//STMFLASH_Write



