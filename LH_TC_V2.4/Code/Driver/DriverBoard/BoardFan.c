#include "BoardFan.h"

//使能信号
#define FAN_VALID_BIT_LEVEL     Bit_RESET

//风扇1初始化
static void BoardFan1Init(IO_SIGNAL_STATE initState)
{
    //初始化IO引脚
    MCU_PortInit(MCU_PIN_E_7,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Low_Speed);
    if(initState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_E_7,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_E_7,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}

//风扇1设置状态
static void BoardFan1SetState(IO_SIGNAL_STATE setState)
{
    if(setState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_E_7,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_E_7,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}

//风扇2初始化
static void BoardFan2Init(IO_SIGNAL_STATE initState)
{
    MCU_PortInit(MCU_PIN_G_1,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Low_Speed);
    if(initState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_G_1,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_G_1,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}

//风扇2设置状态
static void BoardFan2SetState(IO_SIGNAL_STATE setState)
{
    if(setState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_G_1,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_G_1,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}

//风扇3初始化
static void BoardFan3Init(IO_SIGNAL_STATE initState)
{
    MCU_PortInit(MCU_PIN_G_0,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Low_Speed);
    if(initState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_G_0,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_G_0,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}

//风扇3设置状态
static void BoardFan3SetState(IO_SIGNAL_STATE setState)
{
    if(setState == IO_SIGNAL_VALID)
    {
        //有效
         MCU_PortWriteSingle(MCU_PIN_G_0,FAN_VALID_BIT_LEVEL);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_G_0,(BitAction)(!FAN_VALID_BIT_LEVEL));
    }
}


//独立风扇初始化
void BoardFanInitAll(IO_SIGNAL_STATE initState)
{
    BoardFan1Init(initState);
    BoardFan2Init(initState);
    BoardFan3Init(initState);
}

//设置独立风扇的状态
void BoardFanSetState(BOARD_FAN fanIndex,IO_SIGNAL_STATE setState)
{
    if(fanIndex == BOARD_FAN1)
    {
        BoardFan1SetState(setState);
    }
    else if(fanIndex == BOARD_FAN2)
    {
        BoardFan2SetState(setState);
    }
    else if(fanIndex == BOARD_FAN3)
    {
        BoardFan3SetState(setState);
    }
    else
    {
        return;
    }
}
