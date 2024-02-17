

#include "ServiceImplFatFsLog.h"

//当前日志创建的时间
static MCU_RTC_TIMER currentLogFileCreatTime;
//当前文件名
static uint8_t currentLogFileName[FILE_NAME_MAX_LENGTH_BYTE];
//当前文件名长度
static uint8_t currentLogFileNameLength;

//当前在读的文件名
static uint8_t currentReadFileName[FILE_NAME_MAX_LENGTH_BYTE];
//当前在读的文件名长度
static uint8_t currentReadFileNameLength;
//当前文件系统状态
static FATFS_STATE currentFatFsState = FATFS_STATE_CLOSE;
//写入计数
static uint16_t writeCount = 0;



//文件系统读写缓存,要在AXIRAM区,并且4字节对齐
#pragma pack(4)
static uint8_t FsReadWriteBuf[FATFS_READ_WRITE_BUFFER_MAX_LENGTH]@".axi_sram";
#pragma pack()



//根据年月日,生成文件名
static LH_ERR ServiceImplFatFsLog_GenerateFileName(uint16_t year,uint8_t month,uint8_t day,uint8_t* fileName,uint8_t *fileNameLength)
{
    uint8_t index = 0;
    if(fileName == NULL)
    {
        return LH_ERR_NONE;
    }
    //年
    fileName[index++] = (year/1000)+'0';
    fileName[index++] = ((year%1000)/100)+'0';
    fileName[index++] = ((year%100)/10)+'0';
    fileName[index++] = (year%10)+'0';
    //月
    fileName[index++] = (month/10)+'0';
    fileName[index++] = (month%10)+'0';
    //日
    fileName[index++] = (day/10)+'0';
    fileName[index++] = (day%10)+'0';
    //后缀
    fileName[index++] = '.';
    fileName[index++] = 't';
    fileName[index++] = 'x';
    fileName[index++] = 't';
    *fileNameLength = index;
    return LH_ERR_NONE;
}

//文件系统相关数据初始化
LH_ERR ServiceImplFatFsLog_DataInit()
{
    MCU_RTC_TIMER currentRTCTime;
    //清除写入计数
    writeCount = 0;
    //清除创建日志时间
    currentLogFileCreatTime.year    = 0;
    currentLogFileCreatTime.month   = 0;
    currentLogFileCreatTime.day     = 0;
    currentLogFileCreatTime.hour    = 0;
    currentLogFileCreatTime.minute  = 0;
    currentLogFileCreatTime.second  = 0;
    //清除当前创建文件名
    UserMemSet(currentLogFileName,0,FILE_NAME_MAX_LENGTH_BYTE);
    currentLogFileNameLength = 0;
    //清除当前在读文件名
    UserMemSet(currentReadFileName,0,FILE_NAME_MAX_LENGTH_BYTE);
    currentReadFileNameLength = 0;
    //当前状态空闲
    currentFatFsState = FATFS_STATE_CLOSE;
    //开机创建文件
        MCU_RTC_GetDataTime(&currentRTCTime);
        //文件名转换
        ServiceImplFatFsLog_GenerateFileName(currentRTCTime.year,currentRTCTime.month,currentRTCTime.day,currentLogFileName,&currentLogFileNameLength);
        //更新创建文件时间
        currentLogFileCreatTime.year = currentRTCTime.year;
        currentLogFileCreatTime.month = currentRTCTime.month;
        currentLogFileCreatTime.day = currentRTCTime.day; 
        //创建文件
        BoardSD_CreatFile(currentLogFileName,currentLogFileNameLength);
    return LH_ERR_NONE;
}

//文件系统状态转换,判断当前状态是否可以切换到目标状态
static FATFS_STATE ServiceImplFatFsLog_StateSwitch(FATFS_STATE targetState)
{
    FATFS_STATE stateUtil = FATFS_STATE_CLOSE;
    switch (targetState)
    {
    //目标关闭状态
    case FATFS_STATE_CLOSE:
        switch (currentFatFsState)
            {
            //写状态->关闭,关闭文件
            case FATFS_STATE_OPEN_WRITE:
            //读状态->关闭,关闭文件
            case FATFS_STATE_OPEN_READ:
                BoardSD_CloseDirAndFile();
            //关闭状态
            case FATFS_STATE_CLOSE:
                stateUtil = FATFS_STATE_CLOSE;
                break;
            default:
                stateUtil = currentFatFsState;
                break;
            }
        break;
    //目标写状态
    case FATFS_STATE_OPEN_WRITE:
        switch (currentFatFsState)
            {
            //关闭状态->写
            case FATFS_STATE_CLOSE:
                BoardSD_OpenForWrite(currentLogFileName,currentLogFileNameLength);
            //写状态
            case FATFS_STATE_OPEN_WRITE:
                stateUtil = FATFS_STATE_OPEN_WRITE;
                break;
            //写状态只能由关闭状态进入,不能由读直接切换到写,要等读结束
            case FATFS_STATE_OPEN_READ:
                stateUtil = currentFatFsState;
                break;
            default:
                stateUtil = currentFatFsState;
                break;
            }
        break;
    //目标读状态
    case FATFS_STATE_OPEN_READ:
        switch (currentFatFsState)
            {
            //写状态->读,关闭文件,再打开文件
            case FATFS_STATE_OPEN_WRITE:
                BoardSD_CloseDirAndFile();
            //关闭状态->读,打开文件
            case FATFS_STATE_CLOSE:
                BoardSD_OpenDirAndFile(currentReadFileName,currentReadFileNameLength);
            //读状态
            case FATFS_STATE_OPEN_READ:
                stateUtil = FATFS_STATE_OPEN_READ;
                break;
            default:
                stateUtil = currentFatFsState;
                break;
            }
        break;
    default:
        stateUtil = currentFatFsState;
        break;
    }
    return stateUtil;
}
//检查下发的是新文件还是旧文件
static OPEN_FILE CheckTargetFileIsOldOrNew(uint8_t *targetFileName,uint8_t targetNameLength)
{
    uint8_t i = 0;
    if(targetFileName == currentReadFileName)
    {
        for(i=0;i<targetNameLength;i++)
        {
            if(targetFileName[i] == currentReadFileName[i])
            {
                continue;
            }
            else
            {
                return OPEN_FILE_NEW;
            }
        }
        return OPEN_FILE_OLD;
    }
    return OPEN_FILE_NEW;
}


//日志写入SD卡
LH_ERR ServiceImplFatFsLog_WriteFile(uint8_t *writeBufferPtr,uint32_t writeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t bufferOffset = 0;
    MCU_RTC_TIMER currentRTCTime;

    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //读写缓存位空,直接返回
    if(writeBufferPtr == NULL || writeLength == 0)
    {
        return errorCode;
    }
    //判断能否进入写状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_OPEN_WRITE) != FATFS_STATE_OPEN_WRITE)
    {
        //释放内存
        UserMemFree(MEM_AXI_SRAM,writeBufferPtr);
        return errorCode;
    }
    //进入写状态
    currentFatFsState = FATFS_STATE_OPEN_WRITE;
    MCU_RTC_GetDataTime(&currentRTCTime);
    //当系统时间和创建文件的时间不一致时,需先创建文件
    if(currentLogFileCreatTime.year != currentRTCTime.year || currentLogFileCreatTime.month != currentRTCTime.month || currentLogFileCreatTime.day != currentRTCTime.day )
    {
        //关闭之前文件
        BoardSD_CloseDirAndFile(currentLogFileName,currentLogFileNameLength);
        //文件名转换
        ServiceImplFatFsLog_GenerateFileName(currentRTCTime.year,currentRTCTime.month,currentRTCTime.day,currentLogFileName,&currentLogFileNameLength);
        //更新创建文件时间
        currentLogFileCreatTime.year = currentRTCTime.year;
        currentLogFileCreatTime.month = currentRTCTime.month;
        currentLogFileCreatTime.day = currentRTCTime.day; 
        //创建文件
        BoardSD_CreatFile(currentLogFileName,currentLogFileNameLength);
        //打开准备写入日志
        BoardSD_OpenForWrite(currentLogFileName,currentLogFileNameLength);
    }
    
    while(writeLength > FATFS_READ_WRITE_BUFFER_MAX_LENGTH)
    {
        //将内容复制到读写缓存
        UserMemCopy(FsReadWriteBuf,&writeBufferPtr[bufferOffset*FATFS_READ_WRITE_BUFFER_MAX_LENGTH],FATFS_READ_WRITE_BUFFER_MAX_LENGTH);
        bufferOffset++;
        writeLength -= FATFS_READ_WRITE_BUFFER_MAX_LENGTH;
        //日志写入
        BoardSD_WriteFile(FsReadWriteBuf,FATFS_READ_WRITE_BUFFER_MAX_LENGTH);
        writeCount++;
    }
    //将内容复制到读写缓存
    UserMemCopy(FsReadWriteBuf,&writeBufferPtr[bufferOffset*FATFS_READ_WRITE_BUFFER_MAX_LENGTH],writeLength);
    //释放内存
    UserMemFree(MEM_AXI_SRAM,writeBufferPtr);
    //日志写入
    errorCode = BoardSD_WriteFile(FsReadWriteBuf,writeLength);
    //刷新日志
    writeCount++;
    if(writeCount > FATFS_FILE_WRITE_FLUSH_THRESHOLD)
    {
       writeCount = 0;
       errorCode = BoardSD_FlushCacheData();
    }
    return errorCode;
}

//刷新日志
LH_ERR ServiceImplFatFsLog_FlushCacheData(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //如果当前状态是写入状态,则刷新一下
    if(currentFatFsState == FATFS_STATE_OPEN_WRITE)
    {
        writeCount = 0;
        errorCode = BoardSD_FlushCacheData();
    }
    return errorCode;
}

//获取文件列表
LH_ERR ServiceImplFatFsLog_GetFileList(uint8_t fileOffset,uint8_t **fileListBufferPtr,uint16_t *bufferLengthPtr,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t fileCount = 0;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进入关闭状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE) != FATFS_STATE_CLOSE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_UPLOAD_FILE_LIST;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    errorCode = BoardSD_ReadFileList(fileOffset,fileListBufferPtr,&fileCount);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_UPLOAD_FILE_LIST;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    //将文件数量放在头部
    *(uint8_t *)*fileListBufferPtr = fileOffset;
    // *(uint8_t*)((uint8_t *)*fileListBufferPtr+1) = fileCount;
    // *(uint8_t*)((uint8_t *)*fileListBufferPtr+2) = fileCount>>8;
    UserMemCopy((void*)((uint8_t *)*fileListBufferPtr+1),&fileCount,2);
    *bufferLengthPtr = fileCount*(FILE_NAME_MAX_LENGTH_BYTE+FILE_MAX_SIZE_IN_BYTE)+3;
    return errorCode;
}

//准备读日志
LH_ERR ServiceImplFatFsLog_PrepareReadFile(uint8_t *fileName,uint8_t nameLength,uint64_t readByteOffset,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //打开是新的文件
    if(CheckTargetFileIsOldOrNew(fileName,nameLength) == OPEN_FILE_NEW)
    {
        //先进入关闭状态
        ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE);
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    //保存文件名
    UserMemCopy(currentReadFileName,fileName,nameLength);
    currentReadFileNameLength = nameLength;
    //进入读状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_OPEN_READ) != FATFS_STATE_OPEN_READ)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_PREPARE_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //移动光标
    errorCode = BoardSD_FileSeek(readByteOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_PREPARE_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    //进入读状态
    currentFatFsState = FATFS_STATE_OPEN_READ;
    return errorCode;
}

//读日志
LH_ERR ServiceImplFatFsLog_ReadFile(uint8_t *fileDataBuffer,uint16_t *readLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进入读状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_OPEN_READ) != FATFS_STATE_OPEN_READ)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入读状态
    currentFatFsState = FATFS_STATE_OPEN_READ;
    errorCode  = BoardSD_ReadFile(currentReadFileName,currentReadFileNameLength,FsReadWriteBuf,FATFS_READ_WRITE_BUFFER_MAX_LENGTH,readLength);
    //读出错
    if(errorCode != LH_ERR_NONE)
    {
        ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE);
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    //拷贝数据
    UserMemCopy(fileDataBuffer,FsReadWriteBuf,*readLength);
    return errorCode;
}

//读日志结束,关闭文件
LH_ERR ServiceImplFatFsLog_EndReadFile(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进关闭状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE) != FATFS_STATE_CLOSE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_END_READ;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    return errorCode;
}

//删除指定文件
LH_ERR ServiceImplFatFsLog_DeleteTargetFile(uint8_t *fileName,uint8_t nameLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进关闭状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE) != FATFS_STATE_CLOSE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_DELETE_TARGET_FILE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    //删除指定文件
    errorCode = BoardSD_DeleteTargetFile(fileName,nameLength);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_DELETE_TARGET_FILE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    return errorCode;
}

//删除全部文件
LH_ERR ServiceImplFatFsLog_DeleteAllFile(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进关闭状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE) != FATFS_STATE_CLOSE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_DELETE_ALL_FILE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    //删除全部文件
    errorCode = BoardSD_DeleteAllFile();
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_DELETE_ALL_FILE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    return errorCode;
}


//获取指定文件大小
LH_ERR ServiceImplFatFsLog_GetTargetFileSize(uint8_t *fileName,uint8_t nameLength,uint64_t *fileSize,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //进关闭状态
    if(ServiceImplFatFsLog_StateSwitch(FATFS_STATE_CLOSE) != FATFS_STATE_CLOSE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_GET_FILE_SIZE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return LH_ERR_BOARD_MAIN_COMM_FR_STATE_SWITCH;
    }
    //进入关闭状态
    currentFatFsState = FATFS_STATE_CLOSE;
    //删除全部文件
    errorCode = BoardSD_GetTargetFileSize(fileName,nameLength,fileSize);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_FATFS_LOG_GET_FILE_SIZE;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        return errorCode;
    }
    return errorCode;
}