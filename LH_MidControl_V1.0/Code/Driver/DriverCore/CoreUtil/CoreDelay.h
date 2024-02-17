/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-26 13:23:10
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-07-27 19:01:45
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32h7xx_hal.h"
#include "SEGGER_RTT.h"

//延时函数初始化
void CoreDelayInit(void);

//延时指定微妙,带自动溢出切片
void CoreDelayUs(uint32_t nus);

//延时指定毫秒
void CoreDelayMs(uint32_t nms);

//延时操作系统最小周期
void CoreDelayMinTick(void);

#endif




