/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:26:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:07:07 +0800
************************************************************************************************/ 
#ifndef __BOARD_POWER_CONTROL_H_
#define __BOARD_POWER_CONTROL_H_
#include "MCU_DrvHeader.h"

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


