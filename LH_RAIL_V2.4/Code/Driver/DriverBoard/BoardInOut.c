/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:06:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 11:41:52
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardInOut.h"
#include "BoardInOutConfig.h"

//输入初始化函数指针
typedef void (*BoardInInitFuncPtr)(void);
//读取输入值状态
typedef BitAction (*BoardInReadSingleFuncPtr)(void);
//输出初始化函数指针
typedef void (*BoardOutInitFuncPtr)(void);
//输出指定值
typedef void (*BoardOutWriteSingleFuncPtr)(BitAction value);


/******************************************************输入初始化****************************************************/
static void BoardInInit_ARM_SEN1_PH10(void)
{
    MCU_PortInit(MCU_PIN_H_10, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN1_PH10], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN2_PH9(void)
{
    MCU_PortInit(MCU_PIN_H_9, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN2_PH9], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN3_PH8(void)
{
    MCU_PortInit(MCU_PIN_H_8, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN3_PH8], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN4_PH7(void)
{
    MCU_PortInit(MCU_PIN_H_7, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN4_PH7], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN5_PH6(void)
{
    MCU_PortInit(MCU_PIN_H_6, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN5_PH6], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN6_PF11(void)
{
    MCU_PortInit(MCU_PIN_F_11, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN6_PF11], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN7_PB1(void)
{
    MCU_PortInit(MCU_PIN_B_1, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN7_PB1], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN8_PB0(void)
{
    MCU_PortInit(MCU_PIN_B_0, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN8_PB0], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN9_PC5(void)
{
    MCU_PortInit(MCU_PIN_C_5, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN9_PC5], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN10_PC4(void)
{
    MCU_PortInit(MCU_PIN_C_4, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN10_PC4], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN11_PH3(void)
{
    MCU_PortInit(MCU_PIN_H_3, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN11_PH3], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN12_PH2(void)
{
    MCU_PortInit(MCU_PIN_H_2, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN12_PH2], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN13_PA1(void)
{
    MCU_PortInit(MCU_PIN_A_1, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN13_PA1], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN14_PA0(void)
{
    MCU_PortInit(MCU_PIN_A_0, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN14_PA0], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN15_PB9(void)
{
    MCU_PortInit(MCU_PIN_B_9, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN15_PB9], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN16_PI4(void)
{
    MCU_PortInit(MCU_PIN_I_4, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN16_PI4], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN17_PI5(void)
{
    MCU_PortInit(MCU_PIN_I_5, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN17_PI5], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN18_PI6(void)
{
    MCU_PortInit(MCU_PIN_I_6, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN18_PI6], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN19_PI7(void)
{
    MCU_PortInit(MCU_PIN_I_7, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN19_PI7], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN20_PF10(void)
{
    MCU_PortInit(MCU_PIN_F_10, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN20_PF10], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN21_PF9(void)
{
    MCU_PortInit(MCU_PIN_F_9, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN21_PF9], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN22_PF8(void)
{
    MCU_PortInit(MCU_PIN_F_8, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN22_PF8], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN23_PF6(void)
{
    MCU_PortInit(MCU_PIN_F_6, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN23_PF6], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN24_PF7(void)
{
    MCU_PortInit(MCU_PIN_F_7, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN24_PF7], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN25_PI11(void)
{
    MCU_PortInit(MCU_PIN_I_11, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN25_PI11], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN26_PI10(void)
{
    MCU_PortInit(MCU_PIN_I_10, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN26_PI10], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN27_PI9(void)
{
    MCU_PortInit(MCU_PIN_I_9, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN27_PI9], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN28_PC13(void)
{
    MCU_PortInit(MCU_PIN_C_13, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN28_PC13], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN29_PI8(void)
{
    MCU_PortInit(MCU_PIN_I_8, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN29_PI8], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN30_PE2(void)
{
    MCU_PortInit(MCU_PIN_E_2, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN30_PE2], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN31_PE3(void)
{
    MCU_PortInit(MCU_PIN_E_3, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN31_PE3], MCU_PORT_SPEED_FREQ_HIGH,0);
}

static void BoardInInit_ARM_SEN32_PE4(void)
{
    MCU_PortInit(MCU_PIN_E_4, MCU_PORT_MODE_INPUT, boardInInitPullConfigArray[BOARD_IN_ARM_SEN32_PE4], MCU_PORT_SPEED_FREQ_HIGH,0);
}


/******************************************************读取输入状态**********************************************************/
static BitAction BoardInReadSingle_ARM_SEN1_PH10(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_10);
}

static BitAction BoardInReadSingle_ARM_SEN2_PH9(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_9);
}

static BitAction BoardInReadSingle_ARM_SEN3_PH8(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_8);
}

static BitAction BoardInReadSingle_ARM_SEN4_PH7(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_7);
}

static BitAction BoardInReadSingle_ARM_SEN5_PH6(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_6);
}

static BitAction BoardInReadSingle_ARM_SEN6_PF11(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_11);
}

static BitAction BoardInReadSingle_ARM_SEN7_PB1(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_B_1);
}

static BitAction BoardInReadSingle_ARM_SEN8_PB0(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_B_0);
}

static BitAction BoardInReadSingle_ARM_SEN9_PC5(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_C_5);
}

static BitAction BoardInReadSingle_ARM_SEN10_PC4(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_C_4);
}

static BitAction BoardInReadSingle_ARM_SEN11_PH3(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_3);
}

static BitAction BoardInReadSingle_ARM_SEN12_PH2(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_H_2);
}

static BitAction BoardInReadSingle_ARM_SEN13_PA1(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_A_1);
}

static BitAction BoardInReadSingle_ARM_SEN14_PA0(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_A_0);
}

static BitAction BoardInReadSingle_ARM_SEN15_PB9(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_B_9);
}

static BitAction BoardInReadSingle_ARM_SEN16_PI4(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_4);
}

static BitAction BoardInReadSingle_ARM_SEN17_PI5(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_5);
}

static BitAction BoardInReadSingle_ARM_SEN18_PI6(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_6);
}

static BitAction BoardInReadSingle_ARM_SEN19_PI7(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_7);
}

static BitAction BoardInReadSingle_ARM_SEN20_PF10(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_10);
}

static BitAction BoardInReadSingle_ARM_SEN21_PF9(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_9);
}

static BitAction BoardInReadSingle_ARM_SEN22_PF8(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_8);
}

static BitAction BoardInReadSingle_ARM_SEN23_PF6(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_6);
}

static BitAction BoardInReadSingle_ARM_SEN24_PF7(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_F_7);
}

static BitAction BoardInReadSingle_ARM_SEN25_PI11(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_11);
}

static BitAction BoardInReadSingle_ARM_SEN26_PI10(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_10);
}

static BitAction BoardInReadSingle_ARM_SEN27_PI9(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_9);
}

static BitAction BoardInReadSingle_ARM_SEN28_PC13(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_C_13);
}

static BitAction BoardInReadSingle_ARM_SEN29_PI8(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_I_8);
}

static BitAction BoardInReadSingle_ARM_SEN30_PE2(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_E_2);
}

static BitAction BoardInReadSingle_ARM_SEN31_PE3(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_E_3);
}

static BitAction BoardInReadSingle_ARM_SEN32_PE4(void)
{
    //读取当前状态
    return MCU_PortReadSingle(MCU_PIN_E_4);
}

/******************************************************输出初始化,输出全部上拉****************************************************/
//初始化,全部默认高电平
static void BoardOutInit_ARM_OUT1_PG15(void)
{
    MCU_PortInit(MCU_PIN_G_15, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT1_PG15], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_15,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT1_PG15]);
}

static void BoardOutInit_ARM_OUT2_PB3(void)
{
    MCU_PortInit(MCU_PIN_B_3, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT2_PB3], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_B_3,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT2_PB3]);
}

static void BoardOutInit_ARM_OUT3_PB4(void)
{
    MCU_PortInit(MCU_PIN_B_4, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT3_PB4], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_B_4,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT3_PB4]);
}

static void BoardOutInit_ARM_OUT4_PG14(void)
{
    MCU_PortInit(MCU_PIN_G_14, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT4_PG14], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_14,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT4_PG14]);
}

static void BoardOutInit_ARM_OUT5_PG13(void)
{
    MCU_PortInit(MCU_PIN_G_13, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT5_PG13], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_13,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT5_PG13]);
}

static void BoardOutInit_ARM_OUT6_PG11(void)
{
    MCU_PortInit(MCU_PIN_G_11, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT6_PG11], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_11,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT6_PG11]);
}

static void BoardOutInit_ARM_OUT7_PI3(void)
{
    MCU_PortInit(MCU_PIN_I_3, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT7_PI3], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_I_3,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT7_PI3]);
}

static void BoardOutInit_ARM_OUT8_PA15(void)
{
    MCU_PortInit(MCU_PIN_A_15, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT8_PA15], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_A_15,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT8_PA15]);
}

static void BoardOutInit_ARM_OUT9_PD6(void)
{
    MCU_PortInit(MCU_PIN_D_6, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT9_PD6], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_D_6,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT9_PD6]);
}

static void BoardOutInit_ARM_OUT10_PG10(void)
{
    MCU_PortInit(MCU_PIN_G_10, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT10_PG10], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_10,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT10_PG10]);
}

static void BoardOutInit_ARM_OUT11_PG9(void)
{
    MCU_PortInit(MCU_PIN_G_9, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT11_PG9], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_9,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT11_PG9]);
}

static void BoardOutInit_ARM_OUT12_PD7(void)
{
    MCU_PortInit(MCU_PIN_D_7, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT12_PD7], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_D_7,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT12_PD7]);
}

static void BoardOutInit_ARM_OUT13_PG8(void)
{
    MCU_PortInit(MCU_PIN_G_8, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT13_PG8], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_8,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT13_PG8]);
}

static void BoardOutInit_ARM_OUT14_PI1(void)
{
    MCU_PortInit(MCU_PIN_I_1, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT14_PI1], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_I_1,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT14_PI1]);
}

static void BoardOutInit_ARM_OUT15_PI0(void)
{
    MCU_PortInit(MCU_PIN_I_0, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT15_PI0], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_I_0,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT15_PI0]);
}

static void BoardOutInit_ARM_OUT16_PG6(void)
{
    MCU_PortInit(MCU_PIN_G_6, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT16_PG6], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_6,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT16_PG6]);
}

static void BoardOutInit_ARM_OUT17_PG7(void)
{
    MCU_PortInit(MCU_PIN_G_7, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT17_PG7], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_G_7,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT17_PG7]);
}

static void BoardOutInit_ARM_OUT18_PH12(void)
{
    MCU_PortInit(MCU_PIN_H_12, MCU_PORT_MODE_OUTPUT_PP, boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT18_PH12], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_H_12,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT18_PH12]);
}

static void BoardOutInit_ARM_OUT19_PI2(void)
{
    MCU_PortInit(MCU_PIN_I_2, MCU_PORT_MODE_OUTPUT_PP, 
                    boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT19_PI2], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_I_2,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT19_PI2]);
}

static void BoardOutInit_ARM_OUT20_PD3(void)
{
    MCU_PortInit(MCU_PIN_D_3, MCU_PORT_MODE_OUTPUT_PP, 
                    boardOutInitPullConfigArray[BOARD_OUT_ARM_OUT20_PD3], MCU_PORT_SPEED_FREQ_HIGH,0);
    MCU_PortWriteSingle(MCU_PIN_D_3,boardOutInitLevelConfigArray[BOARD_OUT_ARM_OUT20_PD3]);
}

/******************************************************写入输出状态**********************************************************/
static void BoardOutWriteSingle_ARM_OUT1_PG15(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_15,setValue);
}

static void BoardOutWriteSingle_ARM_OUT2_PB3(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_B_3,setValue);
}

static void BoardOutWriteSingle_ARM_OUT3_PB4(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_B_4,setValue);
}

static void BoardOutWriteSingle_ARM_OUT4_PG14(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_14,setValue);
}

static void BoardOutWriteSingle_ARM_OUT5_PG13(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_13,setValue);
}

static void BoardOutWriteSingle_ARM_OUT6_PG11(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_11,setValue);
}

static void BoardOutWriteSingle_ARM_OUT7_PI3(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_I_3,setValue);
}

static void BoardOutWriteSingle_ARM_OUT8_PA15(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_A_15,setValue);
}

static void BoardOutWriteSingle_ARM_OUT9_PD6(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_D_6,setValue);
}

static void BoardOutWriteSingle_ARM_OUT10_PG10(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_10,setValue);
}

static void BoardOutWriteSingle_ARM_OUT11_PG9(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_9,setValue);
}

static void BoardOutWriteSingle_ARM_OUT12_PD7(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_D_7,setValue);
}

static void BoardOutWriteSingle_ARM_OUT13_PG8(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_8,setValue);
}

static void BoardOutWriteSingle_ARM_OUT14_PI1(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_I_1,setValue);
}

static void BoardOutWriteSingle_ARM_OUT15_PI0(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_I_0,setValue);
}

static void BoardOutWriteSingle_ARM_OUT16_PG6(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_6,setValue);
}

static void BoardOutWriteSingle_ARM_OUT17_PG7(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_G_7,setValue);
}

static void BoardOutWriteSingle_ARM_OUT18_PH12(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_H_12,setValue);
}

static void BoardOutWriteSingle_ARM_OUT19_PI2(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_I_2,setValue);
}

static void BoardOutWriteSingle_ARM_OUT20_PD3(BitAction setValue)
{
    MCU_PortWriteSingle(MCU_PIN_D_3,setValue);
}


/******************************************函数指针数组************************************************************/

//函数指针数组
static const BoardInInitFuncPtr BoardInInitFuncPtrArray[] = {
    BoardInInit_ARM_SEN1_PH10,
    BoardInInit_ARM_SEN2_PH9,
    BoardInInit_ARM_SEN3_PH8,
    BoardInInit_ARM_SEN4_PH7,
    BoardInInit_ARM_SEN5_PH6,
    BoardInInit_ARM_SEN6_PF11,
    BoardInInit_ARM_SEN7_PB1,
    BoardInInit_ARM_SEN8_PB0,
    BoardInInit_ARM_SEN9_PC5,
    BoardInInit_ARM_SEN10_PC4,
    BoardInInit_ARM_SEN11_PH3,
    BoardInInit_ARM_SEN12_PH2,
    BoardInInit_ARM_SEN13_PA1,
    BoardInInit_ARM_SEN14_PA0,
    BoardInInit_ARM_SEN15_PB9,
    BoardInInit_ARM_SEN16_PI4,
    BoardInInit_ARM_SEN17_PI5,
    BoardInInit_ARM_SEN18_PI6,
    BoardInInit_ARM_SEN19_PI7,
    BoardInInit_ARM_SEN20_PF10,
    BoardInInit_ARM_SEN21_PF9,
    BoardInInit_ARM_SEN22_PF8,
    BoardInInit_ARM_SEN23_PF6,
    BoardInInit_ARM_SEN24_PF7,
    BoardInInit_ARM_SEN25_PI11,
    BoardInInit_ARM_SEN26_PI10,
    BoardInInit_ARM_SEN27_PI9,
    BoardInInit_ARM_SEN28_PC13,
    BoardInInit_ARM_SEN29_PI8,
    BoardInInit_ARM_SEN30_PE2,
    BoardInInit_ARM_SEN31_PE3,
    BoardInInit_ARM_SEN32_PE4,
};

//函数指针数组
static const BoardInReadSingleFuncPtr BoardInReadSingleFuncPtrArray[] = {
    BoardInReadSingle_ARM_SEN1_PH10,
    BoardInReadSingle_ARM_SEN2_PH9,
    BoardInReadSingle_ARM_SEN3_PH8,
    BoardInReadSingle_ARM_SEN4_PH7,
    BoardInReadSingle_ARM_SEN5_PH6,
    BoardInReadSingle_ARM_SEN6_PF11,
    BoardInReadSingle_ARM_SEN7_PB1,
    BoardInReadSingle_ARM_SEN8_PB0,
    BoardInReadSingle_ARM_SEN9_PC5,
    BoardInReadSingle_ARM_SEN10_PC4,
    BoardInReadSingle_ARM_SEN11_PH3,
    BoardInReadSingle_ARM_SEN12_PH2,
    BoardInReadSingle_ARM_SEN13_PA1,
    BoardInReadSingle_ARM_SEN14_PA0,
    BoardInReadSingle_ARM_SEN15_PB9,
    BoardInReadSingle_ARM_SEN16_PI4,
    BoardInReadSingle_ARM_SEN17_PI5,
    BoardInReadSingle_ARM_SEN18_PI6,
    BoardInReadSingle_ARM_SEN19_PI7,
    BoardInReadSingle_ARM_SEN20_PF10,
    BoardInReadSingle_ARM_SEN21_PF9,
    BoardInReadSingle_ARM_SEN22_PF8,
    BoardInReadSingle_ARM_SEN23_PF6,
    BoardInReadSingle_ARM_SEN24_PF7,
    BoardInReadSingle_ARM_SEN25_PI11,
    BoardInReadSingle_ARM_SEN26_PI10,
    BoardInReadSingle_ARM_SEN27_PI9,
    BoardInReadSingle_ARM_SEN28_PC13,
    BoardInReadSingle_ARM_SEN29_PI8,
    BoardInReadSingle_ARM_SEN30_PE2,
    BoardInReadSingle_ARM_SEN31_PE3,
    BoardInReadSingle_ARM_SEN32_PE4,
};

//函数指针数组
static const BoardOutInitFuncPtr BoardOutInitFuncPtrArray[] = {
    BoardOutInit_ARM_OUT1_PG15,
    BoardOutInit_ARM_OUT2_PB3,
    BoardOutInit_ARM_OUT3_PB4,
    BoardOutInit_ARM_OUT4_PG14,
    BoardOutInit_ARM_OUT5_PG13,
    BoardOutInit_ARM_OUT6_PG11,
    BoardOutInit_ARM_OUT7_PI3,
    BoardOutInit_ARM_OUT8_PA15,
    BoardOutInit_ARM_OUT9_PD6,
    BoardOutInit_ARM_OUT10_PG10,
    BoardOutInit_ARM_OUT11_PG9,
    BoardOutInit_ARM_OUT12_PD7,
    BoardOutInit_ARM_OUT13_PG8,
    BoardOutInit_ARM_OUT14_PI1,
    BoardOutInit_ARM_OUT15_PI0,
    BoardOutInit_ARM_OUT16_PG6,
    BoardOutInit_ARM_OUT17_PG7,
    BoardOutInit_ARM_OUT18_PH12,
    BoardOutInit_ARM_OUT19_PI2,
    BoardOutInit_ARM_OUT20_PD3,
};

//函数指针数组
static const BoardOutWriteSingleFuncPtr BoardOutWriteSingleFuncPtrArray[] = {
    BoardOutWriteSingle_ARM_OUT1_PG15,
    BoardOutWriteSingle_ARM_OUT2_PB3,
    BoardOutWriteSingle_ARM_OUT3_PB4,
    BoardOutWriteSingle_ARM_OUT4_PG14,
    BoardOutWriteSingle_ARM_OUT5_PG13,
    BoardOutWriteSingle_ARM_OUT6_PG11,
    BoardOutWriteSingle_ARM_OUT7_PI3,
    BoardOutWriteSingle_ARM_OUT8_PA15,
    BoardOutWriteSingle_ARM_OUT9_PD6,
    BoardOutWriteSingle_ARM_OUT10_PG10,
    BoardOutWriteSingle_ARM_OUT11_PG9,
    BoardOutWriteSingle_ARM_OUT12_PD7,
    BoardOutWriteSingle_ARM_OUT13_PG8,
    BoardOutWriteSingle_ARM_OUT14_PI1,
    BoardOutWriteSingle_ARM_OUT15_PI0,
    BoardOutWriteSingle_ARM_OUT16_PG6,
    BoardOutWriteSingle_ARM_OUT17_PG7,
    BoardOutWriteSingle_ARM_OUT18_PH12,
    BoardOutWriteSingle_ARM_OUT19_PI2,
    BoardOutWriteSingle_ARM_OUT20_PD3,
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
