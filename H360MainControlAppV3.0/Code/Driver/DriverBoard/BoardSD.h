

/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:11:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-22 13:39:57
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_SD_H_
#define __BOARD_SD_H_
#include "BoardCommon.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "UserMemManage.h"
#include "stdint.h"

// /* bsp.c文件已经将AXI SRAM设置为WT模式，写操作无需再手动操作Cache实现Clean */
// #define ENABLE_SD_DMA_CACHE_MAINTENANCE_WRITE  0

// /* 读操作需要手动操作Cache实现无效化操作 */
// #define ENABLE_SD_DMA_CACHE_MAINTENANCE_READ   1 

#define SD_TIMEOUT 1000
#define SD_DEFAULT_BLOCK_SIZE 512
//printf的缓冲区的长度
#define LENGTH_FATFS_LOG_BUFFER_WITH_FORMAT     512
//文件名最大长度
#define FILE_NAME_MAX_LENGTH_BYTE   16
//文件大小占用字节数
#define FILE_MAX_SIZE_IN_BYTE       4
//一次最多获取的文件数
#define FILE_UPLOAD_MAX_COUNT       100

//SD卡当前状态
typedef enum SD_STATE
{
    SD_NULL   = 0X00,//未插入
    SD_INSERT = 0X01,//已经插入
}SD_STATE;

//SD卡传输状态
typedef enum SD_TRANS_STATE
{
    SD_TRANS_STATE_IDLE = 0X00,//传输状态空闲
    SD_TRANS_STATE_BUSY = 0X01,//传输状态忙
}SD_TRANS_STATE;

//打印函数
#define SDCardPrintf(format,args...)            BoardSD_Printf(format,##args)

extern LH_ERR IPC_ServiceFatFsLogWriteFileWhileAck(uint8_t *writeBufferPtr,uint32_t writeLength);

//SD卡状态初始化
LH_ERR BoardSD_Init(void);

//读取指定块
LH_ERR BoardSD_ReadBlocks(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

//写入指定块
LH_ERR BoardSD_WriteBlocks(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

//读取指定块
LH_ERR BoardSD_ReadBlocksDMA(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

//写入指定块
LH_ERR BoardSD_WriteBlocksDMA(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

//擦除指定地址
LH_ERR BoardSD_Erase(uint32_t StartAddr, uint32_t EndAddr);

//获取内存卡状态
SD_TRANS_STATE BoardSD_GetCardState(void);

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

//获取SD卡是否已经插入
SD_STATE BoardSD_IsDetected(void);

//打印SD卡讯息
void BoardSD_PrintfCardMsg(void);

//串口发送字符串,带格式化
int BoardSD_Printf(const char *format, ...);

//FF调用,获取状态
DSTATUS SD_status(BYTE lun);

//FF调用,SD卡初始化
DSTATUS SD_initialize(BYTE lun);

//FF调用,读数据
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);

//FF调用,写数据
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);

//FF调用,IO控制
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff);

//根据文件名在Main文件夹中创建一个文件,要求文件名带相应后缀
LH_ERR BoardSD_CreatFile(uint8_t *fileName,uint8_t nameLength);

//写入前打开
LH_ERR BoardSD_OpenForWrite(uint8_t *fileName,uint8_t nameLength);

//刷新文件内容
LH_ERR BoardSD_FlushCacheData(void);

//根据文件名写内容
LH_ERR BoardSD_WriteFile(uint8_t *writeBuffer,uint16_t bufferLength);

//打开文件,退出时不关闭
LH_ERR BoardSD_OpenDirAndFile(uint8_t *fileName,uint8_t nameLength);

//关闭文件
LH_ERR BoardSD_CloseDirAndFile();

//移动光标到指定位置
LH_ERR BoardSD_FileSeek(uint64_t readByteOffset);

//根据文件名读内容
LH_ERR BoardSD_ReadFile(uint8_t *fileName,uint8_t nameLength,uint8_t *readBuffer,uint16_t readLength,uint16_t *readLengthPtr);

//获取文件列表
LH_ERR BoardSD_ReadFileList(uint8_t fileOffset,uint8_t **uploadFileList,uint16_t *uploadFileCount);

//删除指定文件
LH_ERR BoardSD_DeleteTargetFile(uint8_t *fileName,uint8_t nameLength);

//删除全部文件
LH_ERR BoardSD_DeleteAllFile();

//获取指定文件大小
LH_ERR BoardSD_GetTargetFileSize(uint8_t *fileName,uint8_t nameLength,uint64_t *fileSize);

#endif




