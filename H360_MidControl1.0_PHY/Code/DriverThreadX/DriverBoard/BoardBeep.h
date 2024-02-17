/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:25:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:00:36 +0800
************************************************************************************************/ 
#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "MCU_DrvHeader.h"

//BEEP状态
typedef enum BEEP_STATE
{
    BEEP_OFF,
    BEEP_ON,
}BEEP_STATE;

//蜂鸣器初始化
void BoardBeepInit(BEEP_STATE initState);

//设置蜂鸣器状态
void BoardBeepSetState(BEEP_STATE state);

#endif
