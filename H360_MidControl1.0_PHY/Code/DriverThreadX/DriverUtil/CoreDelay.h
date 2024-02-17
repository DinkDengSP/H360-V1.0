/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:11:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:51:58 +0800
************************************************************************************************/ 
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32h7xx_hal.h"
#include "ErrorCode.h"

//使用DWT定时器作为延时时钟
#define CORE_DELAY_MODE_DWT         0
//使用外部定时器作为延时时钟
#define CORE_DELAY_MODE_TIMER       1
//使用滴答定时器作为延时时钟
#define CORE_DELAY_MODE_SYSTICK     2

//用户实际使用的延时方式设定
#define CORE_DELAY_MODE_USER        CORE_DELAY_MODE_TIMER

//延时初始化
void CoreDelayInit(void);

//微秒延时
void CoreDelayUs(uint32_t nus);

//毫秒延时
void CoreDelayMs(uint32_t nms);

//最小系统时基定时
void CoreDelayMinTick(void);


#endif




