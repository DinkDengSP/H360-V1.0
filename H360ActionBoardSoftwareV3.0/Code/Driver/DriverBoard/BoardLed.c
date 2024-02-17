#include "BoardLed.h"


//函数指针类型
typedef void (*BoardLedInitFuncPtr)(BOARD_LED_STATUS initStatus);

typedef void (*BoardLedWriteFuncPtr)(BOARD_LED_STATUS status);

typedef void (*BoardLedToogleFuncPtr)(void);

//RED
static void BoardLedInitRed(BOARD_LED_STATUS initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_C_7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_C_7, ioStatus);
}

static void BoardLedWriteRed(BOARD_LED_STATUS status)
{
    BitAction ioStatus;
    ioStatus = (status == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_C_7, ioStatus);
}

static void BoardLedToogleRed()
{
    MCU_PortToogleSingle(MCU_PIN_C_7);
}

//green
static void BoardLedInitGreen(BOARD_LED_STATUS initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_G_8, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_G_8, ioStatus);
}

static void BoardLedWriteGreen(BOARD_LED_STATUS status)
{
    BitAction ioStatus;
    ioStatus = (status == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_G_8, ioStatus);
}

static void BoardLedToogleGreen()
{
    MCU_PortToogleSingle(MCU_PIN_G_8);
}

//yellow
static void BoardLedInitYellow(BOARD_LED_STATUS initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (initStatus == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    //初始化IO口
    MCU_PortInit(MCU_PIN_C_6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(MCU_PIN_C_6, ioStatus);
}

static void BoardLedWriteYellow(BOARD_LED_STATUS status)
{
    BitAction ioStatus;
    ioStatus = (status == BOARD_LED_DARK) ? Bit_SET:Bit_RESET;
    MCU_PortWriteSingle(MCU_PIN_C_6, ioStatus);
}

static void BoardLedToogleYellow(void)
{
    MCU_PortToogleSingle(MCU_PIN_C_6);
}

//初始化函数数组
static const BoardLedInitFuncPtr BoardLedInitFuncPtrArray[] = {
    BoardLedInitRed,
    BoardLedInitGreen,
    BoardLedInitYellow,
};

//写入数值函数数组
static const BoardLedWriteFuncPtr BoardLedWriteFuncPtrArray[] = {
    BoardLedWriteRed,
    BoardLedWriteGreen,
    BoardLedWriteYellow,
};  

//翻转状态函数数组
static const BoardLedToogleFuncPtr BoardLedToogleFuncPtrArray[] = {
    BoardLedToogleRed,
    BoardLedToogleGreen,
    BoardLedToogleYellow,
};




//led初始化
void BoardLedInit(BOARD_LED index,BOARD_LED_STATUS initStatus)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(initStatus > BOARD_LED_LIGHT)
    {
        return;
    }
    BoardLedInitFuncPtrArray[index](initStatus);
}

//LED设置状态
void BoardLedWrite(BOARD_LED index,BOARD_LED_STATUS status)
{
    if(index > BOARD_LED_YELLOW)
    {
        return;
    }
    if(status > BOARD_LED_LIGHT)
    {
        return;
    }
    BoardLedWriteFuncPtrArray[index](status);
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




















