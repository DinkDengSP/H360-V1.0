/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:27:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 10:41:37 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER7_H_
#define __MCU_TIMER7_H_
#include "MCU_TimerBaseUniversal.h"

//选择定时器使用模式
#define MCU_TIMER7_MODE_USER        MCU_TIMER7_MODE_BASE
//是否打印定时器中断讯息
#define MCU_TIMER7_INT_MSH_SHOW     0

//定时器周期中断函数回调
void MCU_Timer7_PeriodElapsedCallback(void);

//注意,定时器主频为APB1上,为APB频率的2倍
//分频系数和计数值 TIM_TRGO_RESET TIM_TRGO_UPDATE
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr);

//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax);

//定时器启动
void MCU_Timer7Start(void);

//定时器停止
void MCU_Timer7Stop(void);

//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void);




#endif



