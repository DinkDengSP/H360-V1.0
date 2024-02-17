/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:32:49 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 15:54:49 +0800
************************************************************************************************/ 
#include "BoardAT24CXX.h"

//设备互斥信号量
static TX_MUTEX mutexAT24CXX;

//初始化
void BoardAT24CXX_Init(void)
{
    //创建设备互斥信号量
    tx_mutex_create(&mutexAT24CXX,"mutexAT24CXX",TX_INHERIT);
    //端口初始化
    MCU_IIC2_PortInit();
}

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
LH_ERR BoardAT24CXX_WaitIdle(uint16_t retryCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
    while(retryCount != 0)
    {
        halState = MCU_IIC2_CheckDeviceReady(ADDR_DEVICE_AT24CXX_USE);
        if(halState == HAL_OK)
        {
            //释放设备互斥信号量
            tx_mutex_put(&mutexAT24CXX);
            return LH_ERR_NONE;
        }
        //还要等
        retryCount--;
    }
    //释放设备互斥信号量
    tx_mutex_put(&mutexAT24CXX);
    //显示错误
    LowLevelPrintf("%s TimeOut\r\n",__func__);
    return LH_ERR_BOARD_AT24CXX_WAIT_IDLE;
}

//写入页数据
LH_ERR BoardAT24CXX_WritePage(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
    //检查空闲
    if(LH_ERR_NONE != BoardAT24CXX_WaitIdle(COUNT_MAX_AT24CXX_WAIT_IDLE))
    {
        //没检查到空闲,释放设备互斥信号量
        tx_mutex_put(&mutexAT24CXX);
        return LH_ERR_BOARD_AT24CXX_WAIT_IDLE;
    }
#if(BYTE_SIZE_AT24CXX_USE > BYTE_SIZE_AT24C02)
    halState = MCU_IIC2_WriteBytesWithAddr(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_16BIT,addr,writeBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE*byteCount);
#else
    halState = MCU_IIC2_WriteBytesWithAddr(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_8BIT,addr,writeBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE*byteCount);
#endif
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        //如果发生错误,不需要等待有效,直接释放设备互斥量
        tx_mutex_put(&mutexAT24CXX);
        return LH_ERR_BOARD_AT24CXX_WRITE_PAGE;
    }
    //检查空闲
    if(LH_ERR_NONE != BoardAT24CXX_WaitIdle(COUNT_MAX_AT24CXX_WAIT_IDLE))
    {
        //没检查到空闲,释放设备互斥信号量
        tx_mutex_put(&mutexAT24CXX);
        return LH_ERR_BOARD_AT24CXX_WAIT_IDLE;
    }
    //完成,空闲
    tx_mutex_put(&mutexAT24CXX);
    return LH_ERR_NONE;
}

//读取页数据
LH_ERR BoardAT24CXX_ReadPage(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
#if(BYTE_SIZE_AT24CXX_USE > BYTE_SIZE_AT24C02)
    halState = MCU_IIC2_ReadBytesWithAddr(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_16BIT,addr,readBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE*byteCount);
#else
    halState = MCU_IIC2_ReadBytesWithAddr(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_8BIT,addr,readBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE*byteCount);
#endif
    //完成,空闲
    tx_mutex_put(&mutexAT24CXX);
    if(halState != HAL_OK)
    {
        LowLevelShowHalError(halState);
        return LH_ERR_BOARD_AT24CXX_READ;
    }
    return LH_ERR_NONE;
}


//写入数据
LH_ERR BoardAT24CXX_WriteMultiByte(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //第一页的页偏移地址
    uint8_t firstPageOffset = addr % PAGE_SIZE_AT24CXX_USE;
    //第一页剩余空间
    uint8_t firstPageRemain = PAGE_SIZE_AT24CXX_USE - firstPageOffset;
    //计算几个整页
    uint8_t numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
    //计算最后的单页多少字节
    uint8_t numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
    //刚好从某一页的起始地址开始
    if(firstPageOffset == 0) 
    {
        //不足一页的情况下直接写
        if(numOfPage == 0) 
        {
            errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, byteCount);
            if(errorSubCode != LH_ERR_NONE)
            {
                //完成,空闲
                tx_mutex_put(&mutexAT24CXX);
                //错误显示
                LowLevelShowErrorSubCode(errorSubCode);
                return errorSubCode;
            }
        }
        //存在多页面
        else  
        {
            //先写入多页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                writeBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, numOfSingle);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    }
    //并不对其某一页的开始地址
    else 
    {
        //不足一页的情况下直接写
        if(numOfPage == 0) 
        {
            //存在一种情况,两个字节刚好跨页
            if(byteCount > firstPageRemain)
            {
                //写入上一页残留
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,firstPageRemain);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                //地址和计数增加
                writeBufPtr += firstPageRemain;
                byteCount -= firstPageRemain;
                addr += firstPageRemain;
                //没写完的写到下一页
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,byteCount);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
            else
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,byteCount);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
        //存在多页面
        else
        {
            byteCount -= firstPageRemain;
            numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
            numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;	
            //第一页
            if(firstPageRemain != 0)
            {  
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, firstPageRemain);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr += firstPageRemain;
                writeBufPtr += firstPageRemain;
            } 
            //后续整页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                writeBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, numOfSingle);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    } 
    //完成,空闲
    tx_mutex_put(&mutexAT24CXX);
    return LH_ERR_NONE; 
}


//读取数据
LH_ERR BoardAT24CXX_ReadMultiByte(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //第一页的页偏移地址
    uint8_t firstPageOffset = addr % PAGE_SIZE_AT24CXX_USE;
    //第一页剩余空间
    uint8_t firstPageRemain = PAGE_SIZE_AT24CXX_USE - firstPageOffset;
    //计算几个整页
    uint8_t numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
    //计算最后的单页多少字节
    uint8_t numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
    //刚好从某一页的起始地址开始
    if(firstPageOffset == 0) 
    {
        //不足一页的情况下直接读
        if(numOfPage == 0) 
        {
            errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, byteCount);
            if(errorSubCode != LH_ERR_NONE)
            {
                //完成,空闲
                tx_mutex_put(&mutexAT24CXX);
                //错误显示
                LowLevelShowErrorSubCode(errorSubCode);
                return errorSubCode;
            }
        }
        //存在多页面
        else  
        {
            //先读入多页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                readBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, numOfSingle);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    }
    //并不对其某一页的开始地址
    else 
    {
        //不足一页的情况下直接读
        if(numOfPage == 0) 
        {
            //存在一种情况,两个字节刚好跨页
            if(byteCount > firstPageRemain)
            {
                //读入上一页残留
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,firstPageRemain);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                //地址和计数增加
                readBufPtr += firstPageRemain;
                byteCount -= firstPageRemain;
                addr += firstPageRemain;
                //没读完的读到下一页
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,byteCount);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
            else
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,byteCount);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
        //存在多页面
        else
        {
            byteCount -= firstPageRemain;
            numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
            numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;	
            //第一页
            if(firstPageRemain != 0)
            {  
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, firstPageRemain);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr += firstPageRemain;
                readBufPtr += firstPageRemain;
            } 
            //后续整页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                readBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, numOfSingle);
                if(errorSubCode != LH_ERR_NONE)
                {
                    //完成,空闲
                    tx_mutex_put(&mutexAT24CXX);
                    //错误显示
                    LowLevelShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    } 
    //完成,空闲
    tx_mutex_put(&mutexAT24CXX);
    return LH_ERR_NONE; 
}

//自检
LH_ERR BoardAT24CXX_Check(void)
{
    LH_ERR errorSub = LH_ERR_NONE;
    uint32_t srcDat = 0;
    uint32_t writeDat = MCU_RandomGetNextUnsigned(0,0XFFFFFFFF);
    uint32_t readDat = 0;
    uint16_t testAddr = MCU_RandomGetNextUnsigned(0,BYTE_SIZE_AT24CXX_USE);
    //自检开始
    LowLevelPrintf("%s Start,Addr: 0X%08X\r\n",__func__,testAddr);
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
    //读取源
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != LH_ERR_NONE)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入新的数据
    errorSub = BoardAT24CXX_WriteMultiByte(testAddr,(uint8_t*)(&writeDat),4);
    if(errorSub != LH_ERR_NONE)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取出来
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != LH_ERR_NONE)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != writeDat)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return LH_ERR_BOARD_AT24CXX_CHECK_FRIST;
    }
    //原始数据写回去
    errorSub = BoardAT24CXX_WriteMultiByte(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != LH_ERR_NONE)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置异常值
    readDat = 0XAA5555AA;
    //读取出来
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != LH_ERR_NONE)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != srcDat)
    {
        //释放信号量
        tx_mutex_put(&mutexAT24CXX);
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return LH_ERR_BOARD_AT24CXX_CHECK_SECOND;
    }
    //释放信号量
    tx_mutex_put(&mutexAT24CXX);
    //成功
    LowLevelPrintf("%s Success Over\r\n",__func__);
    return LH_ERR_NONE;
}

#if(BOARD_AT24CXX_ERASE_CHIP_ENABLE == 1)
static uint8_t boardAT24CXX_EraseChipUtilArray[PAGE_SIZE_AT24CXX_USE] = {0};
//全片擦除
LH_ERR BoardAT24CXX_ChipErase(void)
{
    uint16_t indexPage = 0;
    uint16_t indexByte = 0;
    LH_ERR errorSub = LH_ERR_NONE;
    LowLevelPrintf("%s Start\r\n",__func__);
    for(indexPage = 0; indexPage < (BYTE_SIZE_AT24CXX_USE/PAGE_SIZE_AT24CXX_USE);indexPage++)
    {
        //擦除设置为BOARD_AT24CXX_ERASE_CHIP_INIT_VAL
        UserMemSet((void*)boardAT24CXX_EraseChipUtilArray,BOARD_AT24CXX_ERASE_CHIP_INIT_VAL,PAGE_SIZE_AT24CXX_USE);
        //写入指定块
        errorSub = BoardAT24CXX_WriteMultiByte(indexPage*PAGE_SIZE_AT24CXX_USE,(uint8_t*)(boardAT24CXX_EraseChipUtilArray),PAGE_SIZE_AT24CXX_USE);
        if(errorSub != LH_ERR_NONE)
        {
            return errorSub;
        }
        //写入完成,清除数组
        UserMemClear((void*)boardAT24CXX_EraseChipUtilArray,PAGE_SIZE_AT24CXX_USE);
        //读取指定块
        errorSub = BoardAT24CXX_ReadMultiByte(indexPage*PAGE_SIZE_AT24CXX_USE,(uint8_t*)(boardAT24CXX_EraseChipUtilArray),PAGE_SIZE_AT24CXX_USE);
        if(errorSub != LH_ERR_NONE)
        {
            return errorSub;
        }
        //数据比对
        for(indexByte = 0;indexByte < PAGE_SIZE_AT24CXX_USE;indexByte++)
        {
            if(boardAT24CXX_EraseChipUtilArray[indexByte] != BOARD_AT24CXX_ERASE_CHIP_INIT_VAL)
            {
                return LH_ERR_BOARD_AT24CXX_CHIP_ERASE;
            }
        }
    }
    //成功
    LowLevelPrintf("%s Success Over\r\n",__func__);
    return LH_ERR_NONE;
}

#endif