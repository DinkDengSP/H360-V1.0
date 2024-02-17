/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:26:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 13:33:13 +0800
************************************************************************************************/ 
#ifndef __BOARD_SDCARD_H_
#define __BOARD_SDCARD_H_
#include "MCU_DrvHeader.h"

//SD卡是否启用DMA模式,DMA模式下,不需要CPU干涉数据传输,解放CPU,但是在缓冲区小的情况下
//需要执行数据的拷贝,可能性能还不如中断模式.中断模式下,会频繁触发SDMMC中断,具体怎么用,看使用者取舍
//SD接口的DMA只支持AXI_SRAM访问,所以如果加载文件系统的时候,文件系统缓存在别的RAM分区,就需要使用缓存中转
#define BOARD_SD_ENABLE_DMA                 1
//SD卡读写时间检查开关
#define BOARD_SD_OP_TIME_MEASURE_ENABLE     0
//SD卡通讯超时时间,单个块时间,如果同时多个快,驱动自动加倍
#define BOARD_SD_TIME_OUT_MS                200
//SD卡等待空闲超时时间
#define BOARD_SD_WAIT_IDLE_TIME_OUT         1000
//中断事件打印是否开启
#define BOARD_SD_INT_SHOW                   0
//DMA传输缓冲区的扇区数量
#if(BOARD_SD_ENABLE_DMA == 1)
//DMA传输使用的缓冲区大小
#define BOARD_SD_DMA_BUFFER_SECTOR_COUNT    8
//DMA传输的扇区大小
#define BOARD_SD_DMA_SECTOR_BYTE_SIZE       512
#endif

//SD卡状态初始化
LH_ERR BoardSD_Init(void);

//SD卡复位
void BoardSD_DeInit(void);

//多个扇区擦除
LH_ERR BoardSDCardEraseMultiBlock(uint32_t startBlockNo, uint32_t blockCount,uint32_t timeOutMs);

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
LH_ERR BoardSDCardWriteMultiBlock(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount);

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
LH_ERR BoardSDCardReadMultiBlock(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount);

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

//打印SD卡讯息
void BoardSD_ShowCardMsg(HAL_SD_CardInfoTypeDef *CardInfo);

//检查SD卡状态是否正常
LH_ERR BoardSD_CheckState(void);

#endif

