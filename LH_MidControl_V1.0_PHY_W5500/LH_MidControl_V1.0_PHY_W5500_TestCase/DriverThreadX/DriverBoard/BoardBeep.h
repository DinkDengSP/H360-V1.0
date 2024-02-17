/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:06:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 21:15:12 +0800
************************************************************************************************/ 
#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "DriverHeaderMCU.h"

//蜂鸣器初始化
void BoardBeepInit(PIN_STATE initState);

//设置蜂鸣器状态
void BoardBeepSetState(PIN_STATE state);

#endif
