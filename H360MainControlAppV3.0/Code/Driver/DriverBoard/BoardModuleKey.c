/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:06:55
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 11:57:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardModuleKey.h"

//PE5 BOARD0
//PE6 BOARD1

//模块号识别初始化
void BoardModuleID_Init(void)
{
    //PE5 6初始化
    MCU_PortInit(MCU_PIN_E_5, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortInit(MCU_PIN_E_6, MCU_PORT_MODE_INPUT, MCU_PORT_PULL_PULLDOWN, MCU_PORT_SPEED_FREQ_HIGH,0);
    BoardModuleRead();
}

//读取当前模块号
BOARD_MODULE_ID BoardModuleRead(void)
{
    //读取状态
    BitAction board0Value = MCU_PortReadSingle(MCU_PIN_E_5);
    BitAction board1Value = MCU_PortReadSingle(MCU_PIN_E_6);
    if((board0Value == Bit_RESET)&&(board1Value == Bit_RESET))
    {
        return BOARD_MODULE_ID_1;
    }
    else if((board0Value == Bit_SET)&&(board1Value == Bit_RESET))
    {
        return BOARD_MODULE_ID_2;
    }
    else if((board0Value == Bit_RESET)&&(board1Value == Bit_SET))
    {
        return BOARD_MODULE_ID_3;
    }
    else 
    {
        return BOARD_MODULE_ID_4;
    }
}
