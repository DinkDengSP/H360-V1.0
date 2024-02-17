/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:53:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:24:02 +0800
************************************************************************************************/ 
#ifndef __MCU_WDG_H_
#define __MCU_WDG_H_
#include "CoreUtil.h"

//窗口看门狗喂狗回调函数
typedef void (*MCU_WWDG_EarlyFeedCallBack)(void);

//初始化IWDG，并使能IWDG,参数为喂狗周期
void MCU_IWDG_Init(uint16_t periodMs);

//独立看门狗喂狗
void MCU_IWDG_Feed(void);

//窗口看门狗初始化,具体参数看配置文件
void MCU_WWDG_Init(MCU_WWDG_EarlyFeedCallBack callBackPtr);

//窗口看门狗喂狗
void MCU_WWDG_Feed(void);



#endif

