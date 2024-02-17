/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:26:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:01:15 +0800
************************************************************************************************/ 
#ifndef __BOARD_LED_H_
#define __BOARD_LED_H_
#include "MCU_DrvHeader.h"

//LED灯序号
typedef enum BOARD_LED
{
    BOARD_LED_RED,
    BOARD_LED_GREEN,
    BOARD_LED_YELLOW,
}BOARD_LED;

//LED初始化
void BoardLedInit(BOARD_LED index,PORT_STATE initStatus);

//LED设置状态
void BoardLedWrite(BOARD_LED index,PORT_STATE status);

//LED切换状态
void BoardLedToogle(BOARD_LED index);

#endif
