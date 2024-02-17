/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 16:50:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-28 10:41:30 +0800
************************************************************************************************/ 
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "ErrorCode.h"
#include "stm32h7xx_hal.h"

//延时初始化
void CoreDelayInit(void);

//微秒延时
void CoreDelayUs(uint32_t nus);

//毫秒延时
void CoreDelayMs(uint32_t nms);

//最小系统时基定时
void CoreDelayMinTick(void);


#endif



