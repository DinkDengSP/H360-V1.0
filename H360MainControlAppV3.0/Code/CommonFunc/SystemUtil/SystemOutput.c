/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:46:00
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-04 15:37:48
**FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\SystemCommUtil\SystemOutput.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemOutput.h"


//系统端口写入
LH_ERR SystemOutputWrite(SYSTEM_OUTPUT_INDEX outIndex,BitAction setValue)
{
    if(outIndex <= SYSTEM_OUTPUT_LOW_BOARD5_MCU_YM_ARM4_PG15)
    {
        //下位机
        CAN2_SUB_OUT_WRITE writeSubBoardPin;
        writeSubBoardPin.outputPinIndex = (CAN2_SUB_IO_OUTPUT)(outIndex-SYSTEM_OUTPUT_LOW_BOARD1_CPLD_CDC1);
        writeSubBoardPin.setState = setValue;
        //下位机程序
        return Can2SubOutWriteState(&writeSubBoardPin);
    }
    else if(outIndex <= SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT20_PD3)
    {
        //设备主控
        BoardOutWriteSingle((BOARD_OUT_PIN)(outIndex - SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT1_PG15),
                                setValue);
        return LH_ERR_NONE;
    }
    else
    {
        return LH_ERR_BOARD_MAIN_COMM_SYSTEM_OUT_RANGE;
    }
}
