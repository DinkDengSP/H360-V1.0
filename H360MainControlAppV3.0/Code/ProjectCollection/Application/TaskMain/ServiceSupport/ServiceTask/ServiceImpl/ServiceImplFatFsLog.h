
#ifndef __SERVICE_IMPL_FATFS_LOG_H_
#define __SERVICE_IMPL_FATFS_LOG_H_
#include "ServiceImplBase.h"


//读写缓存最大长度
#define FATFS_READ_WRITE_BUFFER_MAX_LENGTH 512

#define FATFS_FILE_WRITE_FLUSH_THRESHOLD    100

//打开新文件还是旧文件
typedef enum OPEN_FILE
{
    OPEN_FILE_OLD,
    OPEN_FILE_NEW,
}OPEN_FILE;

//文件系统状态
typedef enum FATFS_STATE
{
    FATFS_STATE_CLOSE = 0,          //空闲,关闭
    FATFS_STATE_OPEN_WRITE,         //打开可写
    FATFS_STATE_OPEN_READ,          //打开可读
}FATFS_STATE;

//文件系统相关数据初始化
LH_ERR ServiceImplFatFsLog_DataInit();
//日志写入
LH_ERR ServiceImplFatFsLog_WriteFile(uint8_t *writeBufferPtr,uint32_t writeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//刷新日志
LH_ERR ServiceImplFatFsLog_FlushCacheData(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//获取文件列表
LH_ERR ServiceImplFatFsLog_GetFileList(uint8_t fileOffset,uint8_t **fileListBufferPtr,uint16_t *fileCountPtr,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//准备读日志
LH_ERR ServiceImplFatFsLog_PrepareReadFile(uint8_t *fileName,uint8_t nameLength,uint64_t readByteOffset,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读日志
LH_ERR ServiceImplFatFsLog_ReadFile(uint8_t *fileDataBuffer,uint16_t *readLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读日志结束,关闭文件
LH_ERR ServiceImplFatFsLog_EndReadFile(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//删除指定文件
LH_ERR ServiceImplFatFsLog_DeleteTargetFile(uint8_t *fileName,uint8_t nameLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//删除全部文件
LH_ERR ServiceImplFatFsLog_DeleteAllFile(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//获取指定文件大小
LH_ERR ServiceImplFatFsLog_GetTargetFileSize(uint8_t *fileName,uint8_t nameLength,uint64_t *fileSize,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);
#endif