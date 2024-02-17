/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:39:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-03 16:21:08
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "MCU_Inc.h"

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
