/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:14:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:00:57 +0800
************************************************************************************************/ 
#include "BoardBeep.h"

//蜂鸣器初始化
void BoardBeepInit(BEEP_STATE initState)
{
    //PA6
    GPIO_PinState ioStatus;
    //确定写入状态
    ioStatus = (initState == BEEP_OFF) ? GPIO_PIN_RESET:GPIO_PIN_SET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_A_6, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_A_6, ioStatus);
}

//设置蜂鸣器状态
void BoardBeepSetState(BEEP_STATE state)
{
    GPIO_PinState ioStatus;
    ioStatus = (state == BEEP_OFF) ?GPIO_PIN_RESET:GPIO_PIN_SET;
    MCU_PortWriteSingle(MCU_PIN_A_6, ioStatus);
}
