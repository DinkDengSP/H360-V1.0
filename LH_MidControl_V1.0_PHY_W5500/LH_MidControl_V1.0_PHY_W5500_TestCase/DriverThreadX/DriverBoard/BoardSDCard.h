/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:07:49 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:59:05 +0800
************************************************************************************************/ 
#ifndef __BOARD_SDCARD_H_
#define __BOARD_SDCARD_H_
#include "DriverHeaderMCU.h"
#include "BoardSDCardDriverConfig.h"

//SD卡状态初始化
ERROR_SUB BoardSD_Init(void);

//SD卡复位
void BoardSD_DeInit(void);

//多个扇区擦除
ERROR_SUB BoardSDCardEraseMultiBlock(uint32_t startBlockNo, uint32_t blockCount,uint32_t timeOutMs);

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardWriteMultiBlock(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount);

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
ERROR_SUB BoardSDCardReadMultiBlock(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount);

//获取内存卡讯息
ERROR_SUB BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

//打印SD卡讯息
void BoardSD_ShowCardMsg(HAL_SD_CardInfoTypeDef *CardInfo);

//检查SD卡状态是否正常
ERROR_SUB BoardSD_CheckState(void);

#endif
