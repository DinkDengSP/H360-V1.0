#include "SystemOutput.h"

//系统端口写入
LH_ERR SystemOutputWrite(SYSTEM_OUTPUT_INDEX outIndex,BitAction setValue)
{
    if(outIndex > SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT20_PD3)
    {
        return LH_ERR_BOARD_MAIN_COMM_SYSTEM_OUT_RANGE;
    }
    if(outIndex >= SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT1_PG15)
    {
        //主控板自身
        BoardOutWriteSingle((BOARD_OUT_PIN)(outIndex - SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT1_PG15),
                                setValue);
        return LH_ERR_NONE;
    }
    else
    {
        CAN2_SUB_OUT_WRITE writeSubBoardPin;
        writeSubBoardPin.outputPinIndex = (CAN2_SUB_IO_OUTPUT)(outIndex + (uint32_t)CAN2_SUB_OUT_PIN_BOARD8_CPLD_CDC1);
        writeSubBoardPin.setState = setValue;
        //下位机程序
        return Can2SubOutWriteState(&writeSubBoardPin);
    }
}
