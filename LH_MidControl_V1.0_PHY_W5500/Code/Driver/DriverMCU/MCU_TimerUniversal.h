/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-17 15:18:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-17 15:18:57 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER_UNIVERSAL_H_
#define __MCU_TIMER_UNIVERSAL_H_
#include "CoreUtil.h"

//基础定时器回调函数
typedef void (*MCU_TimerBaseCallBack)(void);

//定时器基础配置
typedef struct MCU_TIMER_BASE_CONFIG
{
    uint16_t freqDivParam;
    uint16_t countUpValue;
    MCU_TimerBaseCallBack callBackFuncPtr;
}MCU_TIMER_BASE_CONFIG;


#endif


