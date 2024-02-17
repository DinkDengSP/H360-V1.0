/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:06:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:07:49 +0800
************************************************************************************************/ 
#include "BoardLed.h"

//LED初始化函数指针类型
typedef void (*BoardLedInitFuncPtr)(PIN_STATE initState);
//LED写入函数指针类型
typedef void (*BoardLedWriteFuncPtr)(PIN_STATE state);
//LED状态切换函数指针类型
typedef void (*BoardLedToogleFuncPtr)(void);


//RED
static void BoardLedInitRed(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PinInit(MCU_PIN_G_9, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinWriteSingle(MCU_PIN_G_9, ioState);
}
static void BoardLedWriteRed(PIN_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PinWriteSingle(MCU_PIN_G_9, ioState);
}
static void BoardLedToogleRed()
{
    MCU_PinToogleSingle(MCU_PIN_G_9);
}

//green
static void BoardLedInitGreen(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PinInit(MCU_PIN_D_6, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinWriteSingle(MCU_PIN_D_6, ioState);
}
static void BoardLedWriteGreen(PIN_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PinWriteSingle(MCU_PIN_D_6, ioState);
}
static void BoardLedToogleGreen()
{
    MCU_PinToogleSingle(MCU_PIN_D_6);
}

//yellow
static void BoardLedInitYellow(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    //初始化IO口
    MCU_PinInit(MCU_PIN_D_7, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_HIGH,0);
    MCU_PinWriteSingle(MCU_PIN_D_7, ioState);
}
static void BoardLedWriteYellow(PIN_STATE State)
{
    GPIO_PinState ioState;
    ioState = (State == PIN_STATE_INVALID) ? GPIO_PIN_SET:GPIO_PIN_RESET;
    MCU_PinWriteSingle(MCU_PIN_D_7, ioState);
}
static void BoardLedToogleYellow()
{
    MCU_PinToogleSingle(MCU_PIN_D_7);
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
void BoardLedInit(BOARD_LED index,PIN_STATE initState)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(initState > PIN_STATE_VALID)
    {
        return;
    }
    BoardLedInitFuncPtrArray[index](initState);
}

//LED设置状态
void BoardLedWrite(BOARD_LED index,PIN_STATE State)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(State > PIN_STATE_VALID)
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
