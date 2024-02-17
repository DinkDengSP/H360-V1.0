#include "BoardSystemLed.h"


//函数指针类型
typedef void (*BoardSystemLedInitFuncPtr)(IO_SIGNAL_STATE initState);

typedef void (*BoardSystemLedWriteFuncPtr)(IO_SIGNAL_STATE state);

typedef void (*BoardSystemLedToogleFuncPtr)(void);

//RED
static void BoardSystemLedInitRed(IO_SIGNAL_STATE initState)
{
    BitAction ioState;
    //确定写入状态
    ioState = (initState == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_G_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_G_3, ioState);
}

static void BoardSystemLedWriteRed(IO_SIGNAL_STATE state)
{
    BitAction ioState;
    ioState = (state == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_G_3, ioState);
}

static void BoardSystemLedToogleRed()
{
    MCU_PortToogleSingle(MCU_PIN_G_3);
}

//green
static void BoardSystemLedInitGreen(IO_SIGNAL_STATE initState)
{
    BitAction ioState;
    //确定写入状态
    ioState = (initState == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_G_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_G_4, ioState);
}

static void BoardSystemLedWriteGreen(IO_SIGNAL_STATE state)
{
    BitAction ioState;
    ioState = (state == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_G_4, ioState);
}

static void BoardSystemLedToogleGreen()
{
    MCU_PortToogleSingle(MCU_PIN_G_4);
}

//yellow
static void BoardSystemLedInitYellow(IO_SIGNAL_STATE initState)
{
    BitAction ioState;
    //确定写入状态
    ioState = (initState == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_G_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_G_2, ioState);
}

static void BoardSystemLedWriteYellow(IO_SIGNAL_STATE state)
{
    BitAction ioState;
    ioState = (state == IO_SIGNAL_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_G_2, ioState);
}

static void BoardSystemLedToogleYellow(void)
{
    MCU_PortToogleSingle(MCU_PIN_G_2);
}

//初始化函数数组
static const BoardSystemLedInitFuncPtr BoardSystemLedInitFuncPtrArray[] = {
    BoardSystemLedInitRed,
    BoardSystemLedInitGreen,
    BoardSystemLedInitYellow,
};

//写入数值函数数组
static const BoardSystemLedWriteFuncPtr BoardSystemLedWriteFuncPtrArray[] = {
    BoardSystemLedWriteRed,
    BoardSystemLedWriteGreen,
    BoardSystemLedWriteYellow,
};  

//翻转状态函数数组
static const BoardSystemLedToogleFuncPtr BoardSystemLedToogleFuncPtrArray[] = {
    BoardSystemLedToogleRed,
    BoardSystemLedToogleGreen,
    BoardSystemLedToogleYellow,
};




//led初始化
void BoardSystemLedInit(BOARD_LED index,IO_SIGNAL_STATE initState)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(initState > IO_SIGNAL_VALID)
    {
        return;
    }
    BoardSystemLedInitFuncPtrArray[index](initState);
}

//LED设置状态
void BoardSystemLedWrite(BOARD_LED index,IO_SIGNAL_STATE state)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(state > IO_SIGNAL_VALID)
    {
        return;
    }
    BoardSystemLedWriteFuncPtrArray[index](state);
}

//LED切换状态
void BoardSystemLedToogle(BOARD_LED index)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    BoardSystemLedToogleFuncPtrArray[index]();
}




















