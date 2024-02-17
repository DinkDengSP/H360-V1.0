/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:06:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 21:15:33 +0800
************************************************************************************************/ 
#include "BoardBeep.h"

//蜂鸣器初始化
void BoardBeepInit(PIN_STATE initState)
{
    //PA6
    GPIO_PinState ioStatus;
    //确定写入状态
    ioStatus = (initState == PIN_STATE_INVALID) ? GPIO_PIN_RESET:GPIO_PIN_SET;
    //初始化IO口
    MCU_PinInit(MCU_PIN_A_6, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinWriteSingle(MCU_PIN_A_6, ioStatus);
}

//设置蜂鸣器状态
void BoardBeepSetState(PIN_STATE state)
{
    GPIO_PinState ioStatus;
    ioStatus = (state == PIN_STATE_INVALID) ?GPIO_PIN_RESET:GPIO_PIN_SET;
    MCU_PinWriteSingle(MCU_PIN_A_6, ioStatus);
}
