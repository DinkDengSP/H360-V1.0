/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 13:37:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-16 12:12:56
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32f0xx.h"
#include "os.h"

//初始化延时系统
void CoreDelayInit(void);

//延时指定毫秒
void CoreDelayMs(uint32_t nms);

//延时最小时钟单元
void CoreDelayMinTick(void);

//延时指定微秒
void CoreDelayUs(uint32_t nus);

/*SysTick定时器中断函数*/
void CoreSystickIRQ_Handler(void);

#endif





