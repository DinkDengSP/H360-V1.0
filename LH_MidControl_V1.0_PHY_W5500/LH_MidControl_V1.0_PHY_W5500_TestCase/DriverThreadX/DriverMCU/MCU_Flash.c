/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:03:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:37:56 +0800
************************************************************************************************/ 
#include "MCU_Flash.h"
#include "LowLevelShow.h"

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

//读出字
uint32_t MCU_FlashReadWord(uint32_t addr)
{
    return *(__IO uint32_t *)addr; 
}

//从指定地址开始读出指定长度的数据
void MCU_FlashReadBuffer(uint32_t readAddr,uint32_t *bufferPtr,uint32_t dwordNumber)
{
    uint32_t indexUtil = 0;
	for(indexUtil = 0;indexUtil < dwordNumber;indexUtil++)
	{
        //读取4个字节.
		bufferPtr[indexUtil] = MCU_FlashReadWord(readAddr);
        //偏移4个字节.
		readAddr += 4;	
	}
}  

//比较Flash空间和buffer空间的数据是否对应的上,决定该空间是否需要擦除
MCU_FLASH_CMP_RESULT MCU_FlashCompare(uint32_t flashAddr, uint8_t* bufferPtr, uint32_t size)
{
    uint32_t indexUtil;
	uint8_t flashEqualFlag;	
	uint8_t readByteTemp;
	//如果偏移地址超过芯片容量，输出错误
    if (flashAddr + size > MCU_FLASH_BASE + MCU_FLASH_BANK_SIZE)
    {
        /*　函数参数错误　*/
        return MCU_FLASH_CMP_PARAM_ERR;		
    }
	//长度为0时返回正确
    if (size == 0)
    {
        /* Flash内容和待写入的数据相等 */
        return MCU_FLASH_CMP_EQU;		
    }
    /* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	flashEqualFlag = 1;			
	for (indexUtil = 0; indexUtil < size; indexUtil++)
	{
		readByteTemp = *(uint8_t *)flashAddr;

		if (readByteTemp != *bufferPtr)
		{
			if (readByteTemp != 0xFF)
			{
                //需要擦除后再写
				return MCU_FLASH_CMP_ERASE;		
			}
			else
			{
                //不相等,需要写,但是不需要擦除
				flashEqualFlag = 0;	
			}
		}
		flashAddr++;
		bufferPtr++;
	}
    /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	if (flashEqualFlag == 1)
	{
		return MCU_FLASH_CMP_EQU;	
	}
	else
	{
        //Flash不需要擦除，直接写
		return MCU_FLASH_CMP_WRITE;	
	}
} 	

//通过Flash地址计算扇区号码和bank号码
static void MCU_FlashCalcBankAndSectorWithAddr(uint32_t flashAddr,uint8_t* bankNoPtr,uint32_t* sectorNoPtr)
{
    //计算bank
    *bankNoPtr = MCU_FlashCalcBank(flashAddr);
    if(*bankNoPtr == FLASH_BANK_1)
    {
        *sectorNoPtr = MCU_FlashGetFlashBank1Sector(flashAddr);
    }
    else 
    {
        *sectorNoPtr = MCU_FlashGetFlashBank2Sector(flashAddr);
    }
}

//清除指定FLASH区域全部错误
static void MCU_FlashClearAllErrorByBankNo(uint8_t bankNo)
{
    if(bankNo == FLASH_BANK_1)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
    }
    else
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);
    }
}

//擦除指定扇区
static ERROR_SUB MCU_FlashEraseSector(uint8_t bankNo,uint32_t sectorNo)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef flashState;
    //擦除对象
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; //擦除类型，扇区擦除 
    FlashEraseInit.Sector = sectorNo;                   //要擦除的扇区
    FlashEraseInit.Banks = bankNo;                      //操作BANK
    FlashEraseInit.NbSectors = 1;                       //一次只擦除一个扇区
    FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;//电压范围，VCC=2.7~3.6V之间!!
    /* 关中断 */
    __set_PRIMASK(1);
    //Flash解锁
 	flashState = HAL_FLASH_Unlock();
    if(flashState != HAL_OK)
    {
        //Flash重新上锁
        HAL_FLASH_Lock();
        /* 开中断 */
        __set_PRIMASK(0);
        //日志显示
        LowLevelShowHalError(flashState);
        return ERROR_SUB_MCU_FLASH_UNLOCK;
    }
    //清除指定区域全部错误
    MCU_FlashClearAllErrorByBankNo(bankNo);
    //发出擦除指令
    flashState = HAL_FLASHEx_Erase(&FlashEraseInit,&sectorNo);
    //清除无效的D-Cache
    SCB_CleanInvalidateDCache();   
    if(flashState != HAL_OK)
	{
        //Flash重新上锁
        HAL_FLASH_Lock();
        /* 开中断 */
        __set_PRIMASK(0);
        //日志显示
        LowLevelShowHalError(flashState);
        return ERROR_SUB_MCU_FLASH_ERASE_CMD;
    }    
    //没有异常,等待擦除操作完成
    flashState = FLASH_WaitForLastOperation(MCU_FLASH_WAIT_OP_TIME,bankNo);   
    if(flashState != HAL_OK)
	{
        //Flash重新上锁
        HAL_FLASH_Lock();
        /* 开中断 */
        __set_PRIMASK(0);
        //日志显示
        LowLevelShowHalError(flashState);
        return ERROR_SUB_MCU_FLASH_ERASE_TIME_OUT;
    }    
    //Flash重新上锁
    HAL_FLASH_Lock();
    /* 开中断 */
    __set_PRIMASK(0);
    return ERROR_SUB_OK;
}

//FLASH写入,不带擦除
static ERROR_SUB MCU_FlashWriteNoCheck(uint32_t startAddr,uint32_t *bufferPtr,uint32_t dwordNumber)
{
    HAL_StatusTypeDef flashState;
    //地址计数辅助          
	uint32_t addressUtil = startAddr;		
    //地址计数结束		
	uint32_t endAddr = startAddr + (dwordNumber*4);
    /* 关中断 */
    __set_PRIMASK(1);
    //Flash解锁
 	flashState = HAL_FLASH_Unlock();	
    if(flashState != HAL_OK)
	{
        //Flash重新上锁
        HAL_FLASH_Lock();
        /* 开中断 */
        __set_PRIMASK(0);
        //日志显示
        LowLevelShowHalError(flashState);
        return ERROR_SUB_MCU_FLASH_UNLOCK;
    }  
    while(addressUtil < endAddr)
    {
        flashState = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,addressUtil,(uint64_t)bufferPtr);
        if(flashState != HAL_OK)
	    {
            //Flash重新上锁
            HAL_FLASH_Lock();
            /* 开中断 */
            __set_PRIMASK(0);
            //日志显示
            LowLevelShowHalError(flashState);
            return ERROR_SUB_MCU_FLASH_PAGE_WRITE;
        }    
        addressUtil += 32;
        bufferPtr += 8;
    } 
    //Flash重新上锁
    HAL_FLASH_Lock();
    /* 开中断 */
    __set_PRIMASK(0);
    return ERROR_SUB_OK;
}

//擦除指定地址的扇区，外部需要关闭中断
ERROR_SUB MCU_FlashEraseSectorByAddr(uint32_t flashAddr)
{
    //计算bank
    uint8_t bankNo = 0;
    //计算扇区号码
    uint32_t sectorNo =0;
    //计算bank编号和扇区编号
    MCU_FlashCalcBankAndSectorWithAddr(flashAddr,&bankNo,&sectorNo);
    //擦除
    return MCU_FlashEraseSector(bankNo,sectorNo);
}

//擦除使用的FLASH空间，查看是否擦成功
ERROR_SUB MCU_FlashEraseRangeWithCheck(uint32_t startAddr,uint32_t dwordNumber)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //地址计数辅助          
	uint32_t addressUtil = startAddr;		
    //地址计数结束		
	uint32_t endAddr = startAddr + (dwordNumber*4);	
    //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
    while(addressUtil < endAddr)		
    {
        //有非0XFFFFFFFF的地方,要擦除这个扇区
        if(MCU_FlashReadWord(addressUtil)!=0XFFFFFFFF)
        {   
            errorCodeSub = MCU_FlashEraseSectorByAddr(addressUtil);
            if(errorCodeSub != ERROR_SUB_OK)
            {
                LowLevelShowErrorSubCode(errorCodeSub);
                return errorCodeSub;
            }                    
        }
        addressUtil += 4;   
    }
    //擦除完毕，重新读取
    addressUtil = startAddr;
    while(addressUtil<endAddr)
    {
        //擦除完成后检验对应地址是不是空数据
        if(MCU_FlashReadWord(addressUtil) != 0XFFFFFFFF)
        { 
            errorCodeSub = ERROR_SUB_MCU_FLASH_ERASE_CHECK;
            LowLevelShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        addressUtil+=4;
    } 
    return ERROR_SUB_OK;
}

//从指定地址开始写入指定长度的数据  	  
ERROR_SUB MCU_FlashWrite(uint32_t startAddr,uint32_t *bufferPtr,uint32_t dwordNumber)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //非法地址
    if((startAddr < MCU_FLASH_BASE)||(startAddr%4))
    {
        errorCodeSub = ERROR_SUB_MCU_FLASH_ADDR_LIMIT;
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //擦除指定区域
    errorCodeSub = MCU_FlashEraseRangeWithCheck(startAddr,dwordNumber);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    } 
    //写入数据
    errorCodeSub = MCU_FlashWriteNoCheck(startAddr,bufferPtr,dwordNumber);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    } 
    return ERROR_SUB_OK;
}
