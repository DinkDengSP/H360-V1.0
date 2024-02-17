/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-05 15:45:31
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 17:48:42
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_SD_H_
#define __BOARD_SD_H_
#include "MCU_Inc.h"

//SD卡通讯超时时间
#define BOARD_SD_TIME_OUT   1000

//SD卡相关讯息
extern HAL_SD_CardInfoTypeDef currentSDCardInfo;

//SD卡状态初始化
LH_ERR BoardSD_Init(void);

//多个扇区擦除
LH_ERR BoardSDCardEraseMultiBlock(uint32_t startBlockNo, uint32_t blockCount,uint32_t timeOutMs);

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
LH_ERR BoardSDCardWriteMultiBlock(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount);

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
LH_ERR BoardSDCardReadMultiBlock(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount);

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

//打印SD卡讯息
void BoardSD_PrintfCardMsg(HAL_SD_CardInfoTypeDef *CardInfo);

#endif
