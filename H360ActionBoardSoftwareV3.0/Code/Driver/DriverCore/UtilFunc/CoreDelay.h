/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved./
 *Author: DengXiaoJun
 *Date: 2020-03-24 17:52:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-27 15:17:27
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\Driver\DriverCore\UtilFunc\CoreDelay.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32f4xx.h"

//延时函数初始化
void CoreDelayInit(void);

//延时函数延时毫秒
void CoreDelayMs(uint16_t nms);

//延时函数延时微秒
void CoreDelayUs(uint16_t nus);

//精确延时函数
void CoreDelayMsSensitivity(uint16_t nms);

//延时操作系统最小时间基数
void CoreDelayMinTick(void);

#endif




