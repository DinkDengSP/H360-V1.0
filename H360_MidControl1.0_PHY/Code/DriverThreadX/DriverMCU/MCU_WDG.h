/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 14:05:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 14:05:27 +0800
************************************************************************************************/ 
#ifndef __MCU_WDG_H_
#define __MCU_WDG_H_
#include "CoreUtil.h"

//独立看门狗初始化,最长喂狗时间,0XFFF = 4096
void MCU_IWDG_Init(uint16_t timeMsMaxFeed);

//独立看门狗喂狗
void MCU_IWDG_Feed(void);

//窗口看门狗初始化
void MCU_WWDG_Init(void);

//窗口看门狗喂狗
void MCU_WWDG_Feed(void);


#endif

