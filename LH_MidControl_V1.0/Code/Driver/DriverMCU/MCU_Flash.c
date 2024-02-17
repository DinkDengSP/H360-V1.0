/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-03 14:52:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-03 16:05:30
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_Flash.h"

//读出字
uint32_t MCU_FlashReadWord(uint32_t faddr)
{
    return *(__IO uint32_t *)faddr; 
}

//获取某个地址所在的flash扇区,仅用于BANK1！！
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t MCU_FlashGetFlashBank1Sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1_BANK1)
        return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2_BANK1)
        return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3_BANK1)
        return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4_BANK1)
        return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5_BANK1)
        return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6_BANK1)
        return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7_BANK1)
        return FLASH_SECTOR_6;
	return FLASH_SECTOR_7;	
}

//获取某个地址所在的flash扇区,仅用于BANK2！！
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t MCU_FlashGetFlashBank2Sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1_BANK2)
        return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2_BANK2)
        return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3_BANK2)
        return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4_BANK2)
        return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5_BANK2)
        return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6_BANK2)
        return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7_BANK2)
        return FLASH_SECTOR_6;
	return FLASH_SECTOR_7;	
}

//通过地址计算扇区编号
static uint8_t MCU_FlashCalcBank(uint32_t flashAddr)
{
    if(flashAddr < ADDR_FLASH_SECTOR_0_BANK2)
    {
        return FLASH_BANK_1;
    }
    else
    {
        return FLASH_BANK_2;
    }
}

//擦除指定地址的扇区，外部需要关闭中断
uint8_t MCU_FlashErase(uint32_t flashAddr)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    
    uint32_t SectorError = 0;
    uint8_t result = 0;
    HAL_StatusTypeDef FlashStatus;
    //计算bank
    uint8_t bank = MCU_FlashCalcBank(flashAddr);
    //计算扇区号码
    uint32_t sector;
    if(bank == FLASH_BANK_1)sector = MCU_FlashGetFlashBank1Sector(flashAddr);
    else sector = MCU_FlashGetFlashBank2Sector(flashAddr);
    //擦除对象
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;           //擦除类型，扇区擦除 
    FlashEraseInit.Sector = sector;                               //要擦除的扇区
    FlashEraseInit.Banks = bank;                                  //操作BANK
    FlashEraseInit.NbSectors = 1;                                 //一次只擦除一个扇区
    FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;          //电压范围，VCC=2.7~3.6V之间!!
 	HAL_FLASH_Unlock();
    if(bank == FLASH_BANK_1)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    }
    else
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
    }
    FlashStatus = HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError);
    if(FlashStatus != HAL_OK) 
    {
        result = 1;
    }
    else
    {
        SCB_CleanInvalidateDCache();                              //清除无效的D-Cache
        FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME, bank);        //等待上次操作完成
    }
 	
 	HAL_FLASH_Lock();
    return result;
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32H7的扇区实在太大,没办法本地保存扇区数据,所以本函数写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FF0F000~0X1FF0F41F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 	  
uint8_t MCU_FlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
    //计算bank和扇区
    uint8_t bank = MCU_FlashCalcBank(WriteAddr);;
    //计算扇区号码
    uint32_t sector;
    //非法地址
    if(WriteAddr<MCU_FLASH_BASE||WriteAddr%4)return 1;
    /* 关中断 */
    __set_PRIMASK(1);  			
    //解锁	
    HAL_FLASH_Unlock();   
    if(bank == FLASH_BANK_1)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    }
    else
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
    }
    //写入的起始地址          
	addrx=WriteAddr;		
    //写入的结束地址		
	endaddr=WriteAddr+NumToWrite*4;	
    //处理前置信息
    if(addrx<0X1FF00000)
    {
        //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        while(addrx<endaddr)		
        {
            //有非0XFFFFFFFF的地方,要擦除这个扇区
            if(MCU_FlashReadWord(addrx)!=0XFFFFFFFF)
            {   
                bank = MCU_FlashCalcBank(addrx);
                if(bank == FLASH_BANK_1)sector = MCU_FlashGetFlashBank1Sector(addrx);
                else sector = MCU_FlashGetFlashBank2Sector(addrx);
                //擦除类型，扇区擦除 
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;     
                //要擦除的扇区  
                FlashEraseInit.Sector=sector;   
                //操作BANK1
                FlashEraseInit.Banks=bank;			
                //一次只擦除一个扇区			
                FlashEraseInit.NbSectors=1;                  
                //电压范围，VCC=2.7~3.6V之间!!           
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3; 
                //发生错误了	     
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
                {
                    break;
                }
                //清除无效的D-Cache
                SCB_CleanInvalidateDCache();                            
            }
            else 
                addrx+=4;
            //等待上次操作完成
            FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME,bank);    
        }
    }
    //等待上次操作完成
    FlashStatus=FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME,bank);
    //写数据       
    if(FlashStatus==HAL_OK)
    {
        while(WriteAddr < endaddr)
        {
            //写入数据
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,WriteAddr,(uint64_t)pBuffer)!=HAL_OK)
            { 
                //写入异常
                break;	
            }
            WriteAddr+=32;
            pBuffer+=8;
        } 
    }
    //上锁
	HAL_FLASH_Lock();  
    /* 开中断 */
    __set_PRIMASK(0);  	
    //操作异常
    if(WriteAddr < endaddr)  
    {
        return 1;//出错
    }   
    //完成
    return 0; 
}

//擦除使用的FLASH空间，查看是否插成功
//特别注意:因为STM32H7的扇区实在太大,没办法本地保存扇区数据,所以本函数写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FF0F000~0X1FF0F41F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 	  
uint8_t MCU_FlashEraseUsedFlash(uint32_t WriteAddr,uint32_t NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
    //计算bank和扇区
    uint8_t bank = MCU_FlashCalcBank(WriteAddr);;
    //计算扇区号码
    uint32_t sector;
    //非法地址
    if(WriteAddr<MCU_FLASH_BASE||WriteAddr%4)return 1;
    /* 关中断 */
    __set_PRIMASK(1);  			
    //解锁	
    HAL_FLASH_Unlock();  
    if(bank == FLASH_BANK_1)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    }
    else
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
    }
    //写入的起始地址          
	addrx=WriteAddr;		
    //写入的结束地址		
	endaddr=WriteAddr+NumToWrite*4;	
    //处理前置信息
    if(addrx<0X1FF00000)
    {
        //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        while(addrx<endaddr)		
        {
            //有非0XFFFFFFFF的地方,要擦除这个扇区
            if(MCU_FlashReadWord(addrx)!=0XFFFFFFFF)
            {   
                bank = MCU_FlashCalcBank(addrx);
                if(bank == FLASH_BANK_1)sector = MCU_FlashGetFlashBank1Sector(addrx);
                else sector = MCU_FlashGetFlashBank2Sector(addrx);
                //擦除类型，扇区擦除 
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;     
                //要擦除的扇区  
                FlashEraseInit.Sector=sector;   
                //操作BANK1
                FlashEraseInit.Banks=bank;			
                //一次只擦除一个扇区			
                FlashEraseInit.NbSectors=1;                  
                //电压范围，VCC=2.7~3.6V之间!!           
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3; 
                //发生错误了	     
                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
                {
                    break;
                }
                //清除无效的D-Cache
                SCB_CleanInvalidateDCache();                            
            }
            else 
                addrx+=4;
            //等待上次操作完成
            FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME,bank);    
        }
    }
    //等待上次操作完成
    FlashStatus=FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME,bank);
    //擦除完毕，重新读取
    addrx = WriteAddr;
    //读取数据是否擦除成功    
    if(FlashStatus==HAL_OK)
    {
        while(addrx<endaddr)
        {
            if(MCU_FlashReadWord(addrx)!=0XFFFFFFFF)
            { 
                break;
            }
            addrx+=4;
            //等待上次操作完成
            FLASH_WaitForLastOperation(MCU_FLASH_WAITE_TIME,bank);   
        } 
    }
    //上锁
	HAL_FLASH_Lock();
    /* 开中断 */
     __set_PRIMASK(0);
    if(addrx < endaddr)
    {
        return 1;
    }
    //完成
    return 0; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void MCU_FlashRead(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)
{
    uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
        //读取4个字节.
		pBuffer[i]=MCU_FlashReadWord(ReadAddr);
        //偏移4个字节.
		ReadAddr+=4;	
	}
} 

//比较Flash空间和buffer空间的数据是否对应的上
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* buffer, uint32_t size)
{
    uint32_t i;
	uint8_t ucIsEqu;	/* 相等标志 */
	uint8_t ucByte;
	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
    if (flashAddr + size > MCU_FLASH_BASE + MCU_FLASH_SIZE)
    {
        /*　函数参数错误　*/
        return MCU_FLASH_CMP_PARAM_ERR;		
    }
	/* 长度为0时返回正确 */
    if (size == 0)
    {
        /* Flash内容和待写入的数据相等 */
        return MCU_FLASH_CMP_EQU;		
    }
    /* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	ucIsEqu = 1;			
	for (i = 0; i < size; i++)
	{
		ucByte = *(uint8_t *)flashAddr;

		if (ucByte != *buffer)
		{
			if (ucByte != 0xFF)
			{
                /* 需要擦除后再写 */
				return MCU_FLASH_CMP_ERASE;		
			}
			else
			{
				ucIsEqu = 0;	/* 不相等，需要写 */
			}
		}
		flashAddr++;
		buffer++;
	}
    /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	if (ucIsEqu == 1)
	{
		return MCU_FLASH_CMP_EQU;	
	}
	else
	{
        /* Flash不需要擦除，直接写 */
		return MCU_FLASH_CMP_WRITE;	
	}
}	