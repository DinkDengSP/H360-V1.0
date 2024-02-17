/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:06:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 11:48:36
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_INOUT_H_
#define __BOARD_INOUT_H_
#include "BoardCommon.h"

//主板可以输出的信号总数
#define BOARD_IN_COUNT      32
//主板输入信号列表
typedef enum BOARD_IN_PIN
{
    BOARD_IN_ARM_SEN1_PH10,
    BOARD_IN_ARM_SEN2_PH9,
    BOARD_IN_ARM_SEN3_PH8,
    BOARD_IN_ARM_SEN4_PH7,
    BOARD_IN_ARM_SEN5_PH6,
    BOARD_IN_ARM_SEN6_PF11,
    BOARD_IN_ARM_SEN7_PB1,
    BOARD_IN_ARM_SEN8_PB0,
    BOARD_IN_ARM_SEN9_PC5,
    BOARD_IN_ARM_SEN10_PC4,
    BOARD_IN_ARM_SEN11_PH3,
    BOARD_IN_ARM_SEN12_PH2,
    BOARD_IN_ARM_SEN13_PA1,
    BOARD_IN_ARM_SEN14_PA0,
    BOARD_IN_ARM_SEN15_PB9,
    BOARD_IN_ARM_SEN16_PI4,
    BOARD_IN_ARM_SEN17_PI5,
    BOARD_IN_ARM_SEN18_PI6,
    BOARD_IN_ARM_SEN19_PI7,
    BOARD_IN_ARM_SEN20_PF10,
    BOARD_IN_ARM_SEN21_PF9,
    BOARD_IN_ARM_SEN22_PF8,
    BOARD_IN_ARM_SEN23_PF6,
    BOARD_IN_ARM_SEN24_PF7,
    BOARD_IN_ARM_SEN25_PI11,
    BOARD_IN_ARM_SEN26_PI10,
    BOARD_IN_ARM_SEN27_PI9,
    BOARD_IN_ARM_SEN28_PC13,
    BOARD_IN_ARM_SEN29_PI8,
    BOARD_IN_ARM_SEN30_PE2,
    BOARD_IN_ARM_SEN31_PE3,
    BOARD_IN_ARM_SEN32_PE4,
}BOARD_IN_PIN;


//主板可以输出的信号总数
#define BOARD_OUT_COUNT     20
//主板输出信号列表
typedef enum BOARD_OUT_PIN
{
    BOARD_OUT_ARM_OUT1_PG15,
    BOARD_OUT_ARM_OUT2_PB3,
    BOARD_OUT_ARM_OUT3_PB4,
    BOARD_OUT_ARM_OUT4_PG14,
    BOARD_OUT_ARM_OUT5_PG13,
    BOARD_OUT_ARM_OUT6_PG11,
    BOARD_OUT_ARM_OUT7_PI3,
    BOARD_OUT_ARM_OUT8_PA15,
    BOARD_OUT_ARM_OUT9_PD6,
    BOARD_OUT_ARM_OUT10_PG10,
    BOARD_OUT_ARM_OUT11_PG9,
    BOARD_OUT_ARM_OUT12_PD7,
    BOARD_OUT_ARM_OUT13_PG8,
    BOARD_OUT_ARM_OUT14_PI1,
    BOARD_OUT_ARM_OUT15_PI0,
    BOARD_OUT_ARM_OUT16_PG6,
    BOARD_OUT_ARM_OUT17_PG7,
    BOARD_OUT_ARM_OUT18_PH12,
    BOARD_OUT_ARM_OUT19_PI2,
    BOARD_OUT_ARM_OUT20_PD3,
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


