#include "BoardLed.h"

//函数指针类型
typedef void (*BoardLedInitFuncPtr)(OUTPUT_STATE initStatus);

typedef void (*BoardLedWriteFuncPtr)(OUTPUT_STATE status);

typedef void (*BoardLedToogleFuncPtr)(void);

//GREEN
static void BoardLedGreen(OUTPUT_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_B_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    MCU_PortWriteSingle(MCU_PIN_B_5, ioStatus);
}

static void BoardLedWriteGreen(OUTPUT_STATE status)
{
    BitAction ioStatus;
    ioStatus = (status == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_B_5, ioStatus);
}

static void BoardLedToogleGreen()
{
    MCU_PortToogleSingle(MCU_PIN_B_5);
}

//YELLOW
static void BoardLedYellow(OUTPUT_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_B_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    MCU_PortWriteSingle(MCU_PIN_B_4, ioStatus);
}

static void BoardLedWriteYellow(OUTPUT_STATE status)
{
    BitAction ioStatus;
    ioStatus = (status == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_B_4, ioStatus);
}

static void BoardLedToogleYellow()
{
    MCU_PortToogleSingle(MCU_PIN_B_4);
}


//RED
static void BoardLedRed(OUTPUT_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_B_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_Level_3);
    MCU_PortWriteSingle(MCU_PIN_B_3, ioStatus);
}

static void BoardLedWriteRed(OUTPUT_STATE status)
{
    BitAction ioStatus;
    ioStatus = (status == OUTPUT_INVALID) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_B_3, ioStatus);
}

static void BoardLedToogleRed()
{
    MCU_PortToogleSingle(MCU_PIN_B_3);
}


//初始化函数数组
static const BoardLedInitFuncPtr BoardLedInitFuncPtrArray[] = {
    BoardLedGreen,
    BoardLedYellow,
    BoardLedRed,
};

//写入数值函数数组
static const BoardLedWriteFuncPtr BoardLedWriteFuncPtrArray[] = {
    BoardLedWriteGreen,
    BoardLedWriteYellow,
    BoardLedWriteRed,
};  

//翻转状态函数数组
static const BoardLedToogleFuncPtr BoardLedToogleFuncPtrArray[] = {
    BoardLedToogleGreen,
    BoardLedToogleYellow,
    BoardLedToogleRed,
};




//led初始化
void BoardLedInit(BOARD_LED index,OUTPUT_STATE initStatus)
{
    if(index > BOARD_LED_RED)
    {
        return;
    }
    BoardLedInitFuncPtrArray[index](initStatus);
}

//LED设置状态
void BoardLedWrite(BOARD_LED index,OUTPUT_STATE status)
{
    if(index > BOARD_LED_RED)
    {
        return;
    }
    BoardLedWriteFuncPtrArray[index](status);
}

//LED切换状态
void BoardLedToogle(BOARD_LED index)
{
    if(index > BOARD_LED_RED)
    {
        return;
    }
    BoardLedToogleFuncPtrArray[index]();
}








