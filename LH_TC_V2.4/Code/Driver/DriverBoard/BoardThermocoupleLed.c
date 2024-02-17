#include "BoardThermocoupleLed.h"

//试剂盘热电偶红灯---PA2
//试剂盘热电偶绿灯---PH2
//反应盘热电偶红灯---PH3
//反应盘热电偶绿灯---PB10
//清洗盘热电偶红灯---PB11
//清洗盘热电偶绿灯---PH10
//环境热电偶红灯-----PH11
//环境热电偶绿灯-----PD12

void BoardThermocoupleLedInitAll(IO_SIGNAL_STATE initState)
{
    BitAction ioState;
    //初始化
    MCU_PortInit(MCU_PIN_A_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_H_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_H_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_B_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_B_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_H_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_H_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    MCU_PortInit(MCU_PIN_D_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);
    ioState = (initState == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_A_2, ioState);
    MCU_PortWriteSingle(MCU_PIN_H_2, ioState);
    MCU_PortWriteSingle(MCU_PIN_H_3, ioState);
    MCU_PortWriteSingle(MCU_PIN_B_10, ioState);
    MCU_PortWriteSingle(MCU_PIN_B_11, ioState);
    MCU_PortWriteSingle(MCU_PIN_H_10, ioState);
    MCU_PortWriteSingle(MCU_PIN_H_11, ioState);
    MCU_PortWriteSingle(MCU_PIN_D_12, ioState);
}


//环境
static void BoardThermocoupleLedEnvirmentRedSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_H_11, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_H_11, Bit_SET);
    }
}

//环境
static void BoardThermocoupleLedEnvirmentGreenSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_D_12, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_D_12, Bit_SET);
    }
}


//清洗盘
static void BoardThermocoupleLedPlateWashRedSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_B_11, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_B_11, Bit_SET);
    }
}

//清洗盘
static void BoardThermocoupleLedPlateWashGreenSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        //绿灯亮,红灯灭
        MCU_PortWriteSingle(MCU_PIN_H_10, Bit_RESET);
    }
    else
    {
        //红灯亮,绿灯灭
        MCU_PortWriteSingle(MCU_PIN_H_10, Bit_SET);
    }
}

//反应盘
static void BoardThermocoupleLedPlateReactionRedSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_H_3, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_H_3, Bit_SET);
    }
}

//反应盘
static void BoardThermocoupleLedPlateReactionGreenSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_B_10, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_B_10, Bit_SET);
    }
}

//试剂盘
static void BoardThermocoupleLedPlateReagentRedSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_A_2, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_A_2, Bit_SET);
    }
}

//试剂盘
static void BoardThermocoupleLedPlateReagentGreenSetState(IO_SIGNAL_STATE state)
{
    if(state == IO_SIGNAL_VALID)
    {
        MCU_PortWriteSingle(MCU_PIN_H_2, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_H_2, Bit_SET);
    }
}


void BoardThermocoupleLedSetState(LED_CHANNEL channel,IO_SIGNAL_STATE state)
{
    if(channel == LED_CHANNEL_RED_ENVIR)
    {
        BoardThermocoupleLedEnvirmentRedSetState(state);
    }
    else if(channel == LED_CHANNEL_GREEN_ENVIR)
    {
        BoardThermocoupleLedEnvirmentGreenSetState(state);
    }
    else if(channel == LED_CHANNEL_RED_PLATE_WASH)
    {
        BoardThermocoupleLedPlateWashRedSetState(state);
    }
    else if(channel == LED_CHANNEL_GREEN_PLATE_WASH)
    {
        BoardThermocoupleLedPlateWashGreenSetState(state);
    }
    else if(channel == LED_CHANNEL_RED_PLATE_REACTION)
    {
        BoardThermocoupleLedPlateReactionRedSetState(state);
    }
    else if(channel == LED_CHANNEL_GREEN_PLATE_REACTION)
    {
        BoardThermocoupleLedPlateReactionGreenSetState(state);
    }
    else if(channel == LED_CHANNEL_RED_PLATE_REAGENT)
    {
        BoardThermocoupleLedPlateReagentRedSetState(state);
    }
    else
    {
        BoardThermocoupleLedPlateReagentGreenSetState(state);
    }
}

void BoardThermocoupleLedSetError(CHANNEL_MAX31856 channel)
{
    if(channel == CHANNEL_MAX31856_PLATE_REAGENT)
    {
        MCU_PortWriteSingle(MCU_PIN_H_2, Bit_RESET);
        MCU_PortWriteSingle(MCU_PIN_A_2, Bit_RESET);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_REACTION)
    {
        MCU_PortWriteSingle(MCU_PIN_B_10, Bit_RESET);
        MCU_PortWriteSingle(MCU_PIN_H_3, Bit_RESET);
    }
    else if(channel == CHANNEL_MAX31856_PLATE_WASH)
    {
        MCU_PortWriteSingle(MCU_PIN_H_10, Bit_RESET);
        MCU_PortWriteSingle(MCU_PIN_B_11, Bit_RESET);
    }
    else
    {
        MCU_PortWriteSingle(MCU_PIN_D_12, Bit_RESET);
        MCU_PortWriteSingle(MCU_PIN_H_11, Bit_RESET);
    }
}
