/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-17 15:17:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-17 15:17:33 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER7_H_
#define __MCU_TIMER7_H_
#include "MCU_TimerUniversal.h"

/*-------------------------------------公共函数--------------------------------*/
//底层复位
void HAL_TIM7_Base_MspDeInit(void);

//底层初始化
void HAL_TIM7_Base_MspInit(void);

//定时器周期中断函数
void HAL_TIM7_PeriodElapsedCallback(void);

//注意,定时器主频为APB1上,为APB频率的2倍
//分频系数和计数值 TIM_TRGO_RESET TIM_TRGO_UPDATE
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax,uint32_t TIM_TRGO_Set,MCU_TimerBaseCallBack callBackFuncPtr);

//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax);

//定时器启动
void MCU_Timer7Start(void);

//定时器停止
void MCU_Timer7Stop(void);

//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void);



#endif


