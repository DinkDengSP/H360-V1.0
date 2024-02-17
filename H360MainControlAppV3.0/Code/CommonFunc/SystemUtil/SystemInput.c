/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:46:00
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-04 15:37:38
**FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\SystemCommUtil\SystemInput.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SystemInput.h"

//读取系统输入
LH_ERR SystemInputRead(SYSTEM_INPUT_INDEX inputIndex,BitAction* inputValue)
{
    LH_ERR errorCode;
    BitAction inputValueLocal;
    if(inputIndex <= SYSTEM_INPUT_LOW_BOARD5_MCU_YM_ARM2_PB5)
    {
        //设备下位机
        CAN2_SUB_INPUT can2Input;
        //初始化
        Can2SubPortInDataStructInit(&can2Input);
        can2Input.inputPinIndex = (CAN2_SUB_IN_PIN_INDEX)(inputIndex - SYSTEM_INPUT_LOW_BOARD1_CPLD_NOP1);
        //读取状态
        errorCode = Can2SubPortInReadState(&can2Input);
        if(errorCode == LH_ERR_NONE)
        {
            *inputValue = can2Input.pinValue;
        }
        return errorCode;
    }
    else if(inputIndex <= SYSTEM_INPUT_MAIN_BOARD_ARM_SEN32_PE4)
    {
        //设备主控板
        inputValueLocal = BoardInReadSingle((BOARD_IN_PIN)(inputIndex - SYSTEM_INPUT_MAIN_BOARD_ARM_SEN1_PH10));
        *inputValue = inputValueLocal;
        return LH_ERR_NONE;
    }
    else
    {
        return LH_ERR_BOARD_MAIN_COMM_SYSTEM_IN_RANGE;
    }
}















