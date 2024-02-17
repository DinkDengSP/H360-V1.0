/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:56:39 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:19:44 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER_UNIVERSAL_H_
#define __MCU_TIMER_UNIVERSAL_H_
#include "CoreUtil.h"
#include "MCU_TimerDriverConfig.h"

//通过频率计算定时器分频系数和计数值,频率上限是输入频率的二分之一,超过直接设置为二分之一
void MCU_TimerCalcClkDivCountForFreq(uint32_t timSrc,uint32_t freq,uint16_t* divResult,uint16_t* countResult);
//通过频率计算定时器分频系数和计数值,频率上线是输入频率的百分之一,主要是用于PWM输出,好设置占空比
void MCU_TimerCalcClkDivCountForFreqPwmOut(uint32_t timSrc,uint32_t freq,uint16_t* divResult,uint16_t* countResult);

//基础定时器回调函数
typedef void (*MCU_TimerBaseCallBack)(void);


//定时器基础配置
typedef struct MCU_TIMER_BASE16_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint16_t countUpValue;//计数溢出值
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_BASE16_CONFIG;

//定时器基础配置
typedef struct MCU_TIMER_BASE32_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint32_t countUpValue;//计数溢出值
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_BASE32_CONFIG;



#endif

