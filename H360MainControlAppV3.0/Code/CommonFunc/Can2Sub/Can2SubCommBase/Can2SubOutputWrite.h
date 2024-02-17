/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:45:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-04 15:38:21
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\Can2SubCommBase\Can2SubOutputWrite.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_OUTPUT_WRITE_H_
#define __CAN2_SUB_OUTPUT_WRITE_H_
#include "Can2SubOutputIndex.h"
#include "Can2ProcMain.h"

//写入指定下位机IO状态
typedef struct CAN2_SUB_OUT_WRITE
{
    //引脚编号
    CAN2_SUB_IO_OUTPUT outputPinIndex;
    //写入值
    BitAction setState;
}CAN2_SUB_OUT_WRITE;

//信道指令支持
typedef enum CAN2_CMD_OUTPUT
{
    CAN2_CMD_OUTPUT_WRITE_SINGLE = 0X00000000,
}CAN2_CMD_OUTPUT;

//指定的IO写入结构体初始化
void Can2SubOutWriteDataStructInit(CAN2_SUB_OUT_WRITE* writePin);

//写入系统内部某个IO状态
LH_ERR Can2SubOutWriteState(CAN2_SUB_OUT_WRITE* writePin);

#endif





