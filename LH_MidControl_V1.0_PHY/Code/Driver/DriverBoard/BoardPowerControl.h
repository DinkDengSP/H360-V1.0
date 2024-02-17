/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:58:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-03 17:19:54
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_POWER_CONTROL_H_
#define __BOARD_POWER_CONTROL_H_
#include "MCU_Inc.h"

//板上电源控制端口
typedef enum BOARD_POWER_NO
{
    BOARD_POWER_ALARM    = 0X00,
    BOARD_POWER_MODULE_1 = 0X01,
    BOARD_POWER_MODULE_2 = 0X02,
    BOARD_POWER_MODULE_3 = 0X03,
    BOARD_POWER_MODULE_4 = 0X04,
    BOARD_POWER_MODULE_5 = 0X05,
}BOARD_POWER_NO;

//板上电源状态
typedef enum BOARD_POWER_STATE
{
    BOARD_POWER_OFF = 0X00,
    BOARD_POWER_ON  = 0X01,
}BOARD_POWER_STATE;

//初始化板上电源控制,初始化默认电源全部关闭
void BoardPowerControlInit(void);

//写入指定板上电源控制
void BoardPowerControlWriteSingle(BOARD_POWER_NO index,BOARD_POWER_STATE state);



#endif


