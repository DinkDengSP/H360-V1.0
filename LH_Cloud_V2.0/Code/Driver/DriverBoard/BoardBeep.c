/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 14:48:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 14:57:17
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
    MCU_PortInit(MCU_PIN_A_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP,GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_A_5, ioStatus);
}

//设置蜂鸣器状态
void BoardBeepSetState(BEEP_STATE state)
{
    BitAction ioStatus;
    ioStatus = (state == BEEP_OFF) ?Bit_RESET:Bit_SET;
    MCU_PortWriteSingle(MCU_PIN_A_5, ioStatus);
}





