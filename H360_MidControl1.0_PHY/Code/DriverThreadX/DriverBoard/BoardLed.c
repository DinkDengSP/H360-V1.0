/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:14:14 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 16:02:07 +0800
************************************************************************************************/ 
#include "BoardLed.h"

//LED初始化函数指针类型
typedef void (*BoardLedInitFuncPtr)(PORT_STATE initState);
//LED写入函数指针类型
typedef void (*BoardLedWriteFuncPtr)(PORT_STATE state);
//LED状态切换函数指针类型
typedef void (*BoardLedToogleFuncPtr)(void);


//RED
static void BoardLedInitRed(PORT_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_G_9, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_9, ioState);
}
static void BoardLedWriteRed(PORT_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PortWriteSingle(MCU_PIN_G_9, ioState);
}
static void BoardLedToogleRed()
{
    MCU_PortToogleSingle(MCU_PIN_G_9);
}

//green
static void BoardLedInitGreen(PORT_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_D_6, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_D_6, ioState);
}
static void BoardLedWriteGreen(PORT_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PortWriteSingle(MCU_PIN_D_6, ioState);
}
static void BoardLedToogleGreen()
{
    MCU_PortToogleSingle(MCU_PIN_D_6);
}

//yellow
static void BoardLedInitYellow(PORT_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_D_7, MCU_PORT_MODE_OUTPUT_PP, MCU_PORT_PULL_PULLUP, MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_D_7, ioState);
}
static void BoardLedWriteYellow(PORT_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PORT_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PortWriteSingle(MCU_PIN_D_7, ioState);
}
static void BoardLedToogleYellow()
{
    MCU_PortToogleSingle(MCU_PIN_D_7);
}


static const BoardLedInitFuncPtr BoardLedInitFuncPtrArray[] = {
    BoardLedInitRed,
    BoardLedInitGreen,
    BoardLedInitYellow,
};

static const BoardLedWriteFuncPtr BoardLedWriteFuncPtrArray[] = {
    BoardLedWriteRed,
    BoardLedWriteGreen,
    BoardLedWriteYellow,
};  

static const BoardLedToogleFuncPtr BoardLedToogleFuncPtrArray[] = {
    BoardLedToogleRed,
    BoardLedToogleGreen,
    BoardLedToogleYellow,
};


//led初始化
void BoardLedInit(BOARD_LED index,PORT_STATE initState)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(initState > PORT_STATE_VALID)
    {
        return;
    }
    BoardLedInitFuncPtrArray[index](initState);
}

//LED设置状态
void BoardLedWrite(BOARD_LED index,PORT_STATE State)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(State > PORT_STATE_VALID)
    {
        return;
    }
    BoardLedWriteFuncPtrArray[index](State);
}

//LED切换状态
void BoardLedToogle(BOARD_LED index)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    BoardLedToogleFuncPtrArray[index]();
}
