/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-01-14 10:10:57
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-14 15:03:50
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardInOut.h"

//输入初始化函数指针
typedef void (*BoardInInitFuncPtr)(void);
//读取输入值状态
typedef BitAction (*BoardInReadSingleFuncPtr)(void);
//输出初始化函数指针
typedef void (*BoardOutInitFuncPtr)(void);
//输出指定值
typedef void (*BoardOutWriteSingleFuncPtr)(BitAction value);

/******************************************************输入初始化****************************************************/
//输入引脚初始化，已经在各个模块初始化
static void BoardInInit_ARM_NOP(void)
{
    asm("nop");
}


/******************************************************读取输入状态**********************************************************/
static BitAction BoardInReadSingle_ARM_SEN1_PD0(void)
{
    return MCU_PortReadSingle(MCU_PIN_D_0);
}

/******************************************************输出初始化,****************************************************/
//输出引脚初始化，已经在各个模块初始化
static void BoardOutInit_ARM_NOP(void)
{
    asm("nop");
}

/******************************************************写入输出状态**********************************************************/
void BoardOutWriteSingle_ARM_DCOUT1_PE7(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_E_7,value);
}

void BoardOutWriteSingle_ARM_DCOUT5_PE8(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_E_8,value);
}

void BoardOutWriteSingle_ARM_DCOUT4_PE11(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_E_11,value);
}

void BoardOutWriteSingle_ARM_DCOUT3_PG0(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_G_0,value);
}

void BoardOutWriteSingle_ARM_DCOUT2_PG1(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_G_1,value);
}

void BoardOutWriteSingle_ARM_PWM4_PH12(BitAction value)
{
    MCU_PortWriteSingle(MCU_PIN_H_12,value);
}

/******************************************函数指针数组************************************************************/

//函数指针数组
static const BoardInInitFuncPtr BoardInInitFuncPtrArray[] = {
   BoardInInit_ARM_NOP,
};

//函数指针数组
static const BoardInReadSingleFuncPtr BoardInReadSingleFuncPtrArray[] = {
    BoardInReadSingle_ARM_SEN1_PD0,
};

//函数指针数组
static const BoardOutInitFuncPtr BoardOutInitFuncPtrArray[] = {
    BoardOutInit_ARM_NOP,
    BoardOutInit_ARM_NOP,
    BoardOutInit_ARM_NOP,
    BoardOutInit_ARM_NOP,
    BoardOutInit_ARM_NOP,
    BoardOutInit_ARM_NOP,
};

//函数指针数组
static const BoardOutWriteSingleFuncPtr BoardOutWriteSingleFuncPtrArray[] = {
    BoardOutWriteSingle_ARM_DCOUT1_PE7,
    BoardOutWriteSingle_ARM_DCOUT5_PE8,
    BoardOutWriteSingle_ARM_DCOUT4_PE11,
    BoardOutWriteSingle_ARM_DCOUT3_PG0,
    BoardOutWriteSingle_ARM_DCOUT2_PG1,
    BoardOutWriteSingle_ARM_PWM4_PH12,
};

//输入初始化
void BoardInInit(BOARD_IN_PIN inputPin)
{
    if(inputPin >= BOARD_IN_COUNT)
    {
        return;
    }
    BoardInInitFuncPtrArray[inputPin]();
}

//读取指定输入
BitAction BoardInReadSingle(BOARD_IN_PIN inputPin)
{
    if(inputPin >= BOARD_IN_COUNT)
    {
        return Bit_RESET;
    }
    return BoardInReadSingleFuncPtrArray[inputPin]();
}

//输出初始化
void BoardOutInit(BOARD_OUT_PIN outPin)
{
    if(outPin >= BOARD_OUT_COUNT)
    {
        return;
    }
    BoardOutInitFuncPtrArray[outPin]();
}

//写入指定输出
void BoardOutWriteSingle(BOARD_OUT_PIN outPin,BitAction value)
{
    if(outPin >= BOARD_OUT_COUNT)
    {
        return;
    }
    BoardOutWriteSingleFuncPtrArray[outPin](value);
}

//一次初始化系统内部所有输入输出
void BoardInOutAllInitOnce(void)
{
    uint16_t i = 0; 
    for(i = 0; i < BOARD_IN_COUNT; i++)
    {
        BoardInInit((BOARD_IN_PIN)i);
    }
    for(i = 0; i < BOARD_OUT_COUNT; i++)
    {
        BoardOutInit((BOARD_OUT_PIN)i);
    }
}











