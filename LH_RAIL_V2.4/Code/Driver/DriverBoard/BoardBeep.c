/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:05:59
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 11:09:48
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardBeep.h"

//蜂鸣器初始化
void BoardBeepInit(BEEP_STATE initState)
{
    //PH11
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initState == BEEP_OFF) ? Bit_RESET:Bit_SET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_H_11, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_H_11, ioStatus);
}

//设置蜂鸣器状态
void BoardBeepSetState(BEEP_STATE state)
{
    BitAction ioStatus;
    ioStatus = (state == BEEP_OFF) ?Bit_RESET:Bit_SET;
    MCU_PortWriteSingle(MCU_PIN_H_11, ioStatus);
}
