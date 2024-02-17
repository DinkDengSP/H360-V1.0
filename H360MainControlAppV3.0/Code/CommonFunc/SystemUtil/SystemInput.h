/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:46:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-04 15:37:42
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\SystemCommUtil\SystemInput.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SYSTEM_INPUT_H_
#define __SYSTEM_INPUT_H_
#include "SystemInputIndex.h"
#include "BoardInc.h"
#include "Can2SubInputRead.h"

//读取系统输入
LH_ERR SystemInputRead(SYSTEM_INPUT_INDEX inputIndex,BitAction* inputValue);

#endif




