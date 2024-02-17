/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-10-26 13:06:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 13:19:36
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardLed.h"

//红灯LED配置
#define LED_RED_PORT                MCU_PIN_C_4
#define LED_RED_VALID_LEVEL         Bit_RESET

//绿灯LED配置
#define LED_GREEN_PORT              MCU_PIN_C_0
#define LED_GREEN_VALID_LEVEL       Bit_RESET

//黄灯LED配置
#define LED_YELLOW_PORT             MCU_PIN_A_7
#define LED_YELLOW_VALID_LEVEL      Bit_RESET


//LED状态初始化函数指针
typedef void (*BoardLedInitFuncPtr)(BOARD_LED_STATE initStatus);
//LED状态写入函数指针
typedef void (*BoardLedWriteFuncPtr)(BOARD_LED_STATE status);
//LED状态切换函数指针
typedef void (*BoardLedToogleFuncPtr)(void);

//RED
static void BoardLedInitRed(BOARD_LED_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (BitAction)((initStatus == BOARD_LED_DARK) ? (!LED_RED_VALID_LEVEL):LED_RED_VALID_LEVEL);
    //初始化IO口
    MCU_PortInit(LED_RED_PORT, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(LED_RED_PORT, ioStatus);
}

static void BoardLedWriteRed(BOARD_LED_STATE status)
{
    BitAction ioStatus;
    ioStatus = (BitAction)((status == BOARD_LED_DARK) ? (!LED_RED_VALID_LEVEL):LED_RED_VALID_LEVEL);
    MCU_PortWriteSingle(LED_RED_PORT, ioStatus);
}

static void BoardLedToogleRed()
{
    MCU_PortToogleSingle(LED_RED_PORT);
}

//green
static void BoardLedInitGreen(BOARD_LED_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (BitAction)((initStatus == BOARD_LED_DARK) ? (!LED_GREEN_VALID_LEVEL):LED_GREEN_VALID_LEVEL);
    //初始化IO口
    MCU_PortInit(LED_GREEN_PORT, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(LED_GREEN_PORT, ioStatus);
}

static void BoardLedWriteGreen(BOARD_LED_STATE status)
{
    BitAction ioStatus;
    ioStatus = (BitAction)((status == BOARD_LED_DARK) ? (!LED_GREEN_VALID_LEVEL):LED_GREEN_VALID_LEVEL);
    MCU_PortWriteSingle(LED_GREEN_PORT, ioStatus);
}

static void BoardLedToogleGreen()
{
    MCU_PortToogleSingle(LED_GREEN_PORT);
}

//yellow
static void BoardLedInitYellow(BOARD_LED_STATE initStatus)
{
    BitAction ioStatus;
    //确定写入状态
    ioStatus = (BitAction)((initStatus == BOARD_LED_DARK) ? (!LED_YELLOW_VALID_LEVEL):LED_YELLOW_VALID_LEVEL);
    //初始化IO口
    MCU_PortInit(LED_YELLOW_PORT, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);
    MCU_PortWriteSingle(LED_YELLOW_PORT, ioStatus);
}

static void BoardLedWriteYellow(BOARD_LED_STATE status)
{
    BitAction ioStatus;
    ioStatus = (BitAction)((status == BOARD_LED_DARK) ? (!LED_YELLOW_VALID_LEVEL):LED_YELLOW_VALID_LEVEL);
    MCU_PortWriteSingle(LED_YELLOW_PORT, ioStatus);
}

static void BoardLedToogleYellow(void)
{
    MCU_PortToogleSingle(LED_YELLOW_PORT);
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
void BoardLedInit(BOARD_LED index,BOARD_LED_STATE initStatus)
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
void BoardLedWrite(BOARD_LED index,BOARD_LED_STATE status)
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

