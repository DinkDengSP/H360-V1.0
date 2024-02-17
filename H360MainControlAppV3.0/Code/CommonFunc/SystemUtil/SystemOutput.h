/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:46:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-04 15:37:50
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\SystemCommUtil\SystemOutput.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_OUTPUT_H_
#define __SYSTEM_OUTPUT_H_
#include "SystemOutputIndex.h"
#include "BoardInc.h"
#include "Can2SubOutputWrite.h"


//系统端口写入
LH_ERR SystemOutputWrite(SYSTEM_OUTPUT_INDEX outIndex,BitAction setValue);

#endif





