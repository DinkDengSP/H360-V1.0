/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-01-14 10:12:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-14 15:03:23
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_INOUT_H_
#define __BOARD_INOUT_H_
#include "MCU_Inc.h"

//主板可以输出的信号总数,和主控板对应，
#define BOARD_IN_COUNT      1
//主板输入信号列表
typedef enum BOARD_IN_PIN
{
    BOARD_IN_ARM_SEN1_PD0, //浮球输入
}BOARD_IN_PIN;


//主板可以输出的信号总数 和主控板对应
#define BOARD_OUT_COUNT     6
//主板输出信号列表
typedef enum BOARD_OUT_PIN
{
    BOARD_OUT_ARM_DCOUT1_PE7, //风扇1
    BOARD_OUT_ARM_DCOUT5_PE8,  //循环风扇和循环泵
    BOARD_OUT_ARM_DCOUT4_PE11, //制冷指示灯
    BOARD_OUT_ARM_DCOUT3_PG0,   //风扇3
    BOARD_OUT_ARM_DCOUT2_PG1,   //风扇2
    BOARD_OUT_ARM_PWM4_PH12,   //玻璃加热
}BOARD_OUT_PIN;

//输入初始化
void BoardInInit(BOARD_IN_PIN inputPin);

//读取指定输入
BitAction BoardInReadSingle(BOARD_IN_PIN inputPin);

//输出初始化
void BoardOutInit(BOARD_OUT_PIN outPin);

//写入指定输出
void BoardOutWriteSingle(BOARD_OUT_PIN outPin,BitAction value);

//一次初始化系统内部所有输入输出
void BoardInOutAllInitOnce(void);

#endif
