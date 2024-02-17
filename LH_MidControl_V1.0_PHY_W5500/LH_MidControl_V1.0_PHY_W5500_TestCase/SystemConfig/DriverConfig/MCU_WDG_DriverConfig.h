/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:34:46 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 14:38:02 +0800
************************************************************************************************/ 
#ifndef __MCU_WDG_DRIVER_CONFIG__
#define __MCU_WDG_DRIVER_CONFIG__
#include "stm32h7xx.h"

/*-----------------------------------------------------------窗口看门狗配置-------------------------------------------------------*/
//窗口看门狗最大0X7F,每次喂狗的值
#define MCU_WWDG_RELOAD_VAL                             0X7F
//窗口值,上窗口值最低不能小于等待0X40,最大0X7F,计数值在0X40和MCU_WWDG_WINDOW_VAL之间
//才能喂狗,否则将会导致中断
#define MCU_WWDG_WINDOW_VAL                             (MCU_WWDG_RELOAD_VAL -1)

#endif



