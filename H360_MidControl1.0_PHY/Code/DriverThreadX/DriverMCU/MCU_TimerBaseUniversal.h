/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:27:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 10:40:26 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER_BASE_UNIVERSAL_H_
#define __MCU_TIMER_BASE_UNIVERSAL_H_
#include "CoreUtil.h"

//TIMER7用于基础计时模式
#define MCU_TIMER7_MODE_BASE        0
//TIMER7用于DAC触发模式
#define MCU_TIMER7_MODE_DAC         1

//TIMER6用于基础计时模式
#define MCU_TIMER6_MODE_BASE        0
//TIMER6用于DAC触发模式
#define MCU_TIMER6_MODE_DAC         1

//通过基础定时器的计数频率计算分频和计数值
//计算的前提是HCLK= SYSCLK/2 PCLK= HCLK/2 Timer时钟2倍频
void MCU_BaseTimerUniversalCalcParam(float countFreq,uint16_t* clkDivPtr,uint16_t* countPtr);

//基础定时器回调函数
typedef void (*MCU_TimerBaseCallBack)(void);

//定时器基础配置
typedef struct MCU_TIMER_BASE_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint16_t countUpValue;//计数溢出值
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_BASE_CONFIG;


#endif


