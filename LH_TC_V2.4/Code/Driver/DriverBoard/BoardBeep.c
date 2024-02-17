#include "BoardBeep.h"

//蜂鸣器端口是PI7
#define VALID_LEVEL_BEEP        Bit_SET

//蜂鸣器初始化
void BoardBeepInit(IO_SIGNAL_STATE initState)
{
    MCU_PortInit(MCU_PIN_I_7,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_UP,GPIO_Fast_Speed);
    if(initState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_I_7,VALID_LEVEL_BEEP);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_I_7,(BitAction)(!VALID_LEVEL_BEEP));
    }
}

//蜂鸣器设置状态
void BoardBeepWriteState(IO_SIGNAL_STATE setState)
{
    if(setState == IO_SIGNAL_VALID)
    {
        //有效
        MCU_PortWriteSingle(MCU_PIN_I_7,VALID_LEVEL_BEEP);
    }
    else
    {
        //无效
        MCU_PortWriteSingle(MCU_PIN_I_7,(BitAction)(!VALID_LEVEL_BEEP));
    }
}


//蜂鸣器切换状态
void BoardBeepToogleState(void)
{
    MCU_PortToogleSingle(MCU_PIN_I_7);
}

















