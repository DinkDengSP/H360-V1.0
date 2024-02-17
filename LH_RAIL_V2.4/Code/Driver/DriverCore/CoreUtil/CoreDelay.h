/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-09 14:43:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-12 13:51:59
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

//延时函数初始化
void CoreDelayInit(void);

//延时指定时间
void CoreDelayMs(uint32_t nms);

//延时指定时间
void CoreDelayUs(uint16_t nus);

//延时最小时钟单元
void CoreDelayMinTick(void);

/*SysTick定时器中断函数*/
void SysTick_Handler(void);

#endif



