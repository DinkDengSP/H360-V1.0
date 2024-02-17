/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-27 14:03:55
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 16:05:05
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppUtil\AppLiquidUtilConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_LIQUID_UTIL_CONFIG_VERSION_0_H_
#define __APP_LIQUID_UTIL_CONFIG_VERSION_0_H_
#include "CanSubSystemInc.h"

/*************************************A B 清洗针公用***************************************************/
//清洗盘,A液 B液排放泵,DP2
#define OUTPUT_LIQUID_PUMP_DP2_VERSION_0                      CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN23_PF8
//打开DP2的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP2_VERSION_0          Bit_RESET

/***********************************************缓冲液注液*********************************************/
//缓冲液选瓶V202
#define OUTPUT_LIQUID_VALVE_V202_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC12
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V202_VERSION_0        Bit_RESET
//三通阀切换到A瓶的电平
#define BOTTLE_SELECT_A_OUTPUT_LIQUID_VALVE_V202_VERSION_0    Bit_SET
//三通阀切换到B瓶的电平
#define BOTTLE_SELECT_B_OUTPUT_LIQUID_VALVE_V202_VERSION_0    Bit_RESET

//缓冲液选通V201
#define OUTPUT_LIQUID_VALVE_V201_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC13
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V201_VERSION_0        Bit_RESET

/***********************************************激发液注液*********************************************/
//激发液选瓶V204
#define OUTPUT_LIQUID_VALVE_V204_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC10
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V204_VERSION_0        Bit_RESET
//三通阀切换到A瓶的电平
#define BOTTLE_SELECT_A_OUTPUT_LIQUID_VALVE_V204_VERSION_0    Bit_SET
//三通阀切换到B瓶的电平
#define BOTTLE_SELECT_B_OUTPUT_LIQUID_VALVE_V204_VERSION_0    Bit_RESET

//激发液选通V203
#define OUTPUT_LIQUID_VALVE_V203_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC11
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V203_VERSION_0        Bit_RESET

//激发液纯水维护选通纯水
#define OUTPUT_LIQUID_VALVE_V308_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN17_PC4
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V308_VERSION_0        Bit_RESET
/***************************************************清洗针*****************************************/
//清洗针清洗液注液针1,注液吸液三通阀 V301
#define OUTPUT_LIQUID_VALVE_V301_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC33
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V301_VERSION_0        Bit_RESET
//三通阀切换到吸液状态的电平,泵和清洗液桶导通
#define ABSORB_OUTPUT_LIQUID_VALVE_V301_VERSION_0             Bit_SET
//三通阀切换到注液状态的电平,泵和清洗针导通
#define INJECT_OUTPUT_LIQUID_VALVE_V301_VERSION_0             Bit_RESET

//清洗针清洗液注液针2,注液吸液三通阀 V302
#define OUTPUT_LIQUID_VALVE_V302_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC32
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V302_VERSION_0        Bit_RESET
//三通阀切换到吸液状态的电平,泵和清洗液桶导通
#define ABSORB_OUTPUT_LIQUID_VALVE_V302_VERSION_0             Bit_SET
//三通阀切换到注液状态的电平,泵和清洗针导通
#define INJECT_OUTPUT_LIQUID_VALVE_V302_VERSION_0             Bit_RESET

//清洗针清洗液注液针3,注液吸液三通阀 V303
#define OUTPUT_LIQUID_VALVE_V303_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC31
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V303_VERSION_0        Bit_RESET
//三通阀切换到吸液状态的电平,泵和清洗液桶导通
#define ABSORB_OUTPUT_LIQUID_VALVE_V303_VERSION_0             Bit_SET
//三通阀切换到注液状态的电平,泵和清洗针导通
#define INJECT_OUTPUT_LIQUID_VALVE_V303_VERSION_0             Bit_RESET

//清洗针清洗液注液针4,注液吸液三通阀 V304
#define OUTPUT_LIQUID_VALVE_V304_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC30
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V304_VERSION_0        Bit_RESET
//三通阀切换到吸液状态的电平,泵和清洗液桶导通
#define ABSORB_OUTPUT_LIQUID_VALVE_V304_VERSION_0             Bit_SET
//三通阀切换到注液状态的电平,泵和清洗针导通
#define INJECT_OUTPUT_LIQUID_VALVE_V304_VERSION_0             Bit_RESET

//清洗针清洗液注液针5,注液吸液三通阀 V305
#define OUTPUT_LIQUID_VALVE_V305_VERSION_0                    CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC29
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V305_VERSION_0        Bit_RESET 
//三通阀切换到吸液状态的电平,泵和清洗液桶导通
#define ABSORB_OUTPUT_LIQUID_VALVE_V305_VERSION_0             Bit_SET
//三通阀切换到注液状态的电平,泵和清洗针导通
#define INJECT_OUTPUT_LIQUID_VALVE_V305_VERSION_0             Bit_RESET

//清洗针清洗液纯水三通阀 V310
#define OUTPUT_LIQUID_VALVE_V310_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC28
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V310_VERSION_0            Bit_RESET
//三通阀切换到正常运行状态,阀和清洗液导通
#define LIQUID_CLEAN_OUTPUT_LIQUID_VALVE_V310_VERSION_0           Bit_SET
//三通阀切换到清洗维护状态,阀和纯水导通
#define LIQUID_PURE_WATER_OUTPUT_LIQUID_VALVE_V310_VERSION_0      Bit_RESET

/***************************************************样本针*****************************************/
//样本针内针清洗泵
#define OUTPUT_LIQUID_PUMP_DP307_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC25
//打开DP307的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP307_VERSION_0            Bit_RESET

//样本针内针清洗阀
#define OUTPUT_LIQUID_VALVE_V221_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC34
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V221_VERSION_0            Bit_RESET

//样本针外壁清洗阀
#define OUTPUT_LIQUID_VALVE_V218_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC22
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V218_VERSION_0            Bit_RESET

//样本针强洗泵
#define OUTPUT_LIQUID_PUMP_DP304_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC24
//打开DP304的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_0            Bit_RESET

//样本针强洗阀
#define OUTPUT_LIQUID_VALVE_V215_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC19
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V215_VERSION_0            Bit_RESET
/***************************************************磁珠针*****************************************/
//磁珠针内壁清洗阀
#define OUTPUT_LIQUID_VALVE_V220_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC35
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V220_VERSION_0            Bit_RESET

//磁珠针外壁清洗阀
#define OUTPUT_LIQUID_VALVE_V217_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC21
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V217_VERSION_0            Bit_RESET
/***************************************************试剂针*****************************************/
//试剂针内壁清洗阀
#define OUTPUT_LIQUID_VALVE_V219_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC36
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V219_VERSION_0            Bit_RESET

//试剂针外壁清洗阀
#define OUTPUT_LIQUID_VALVE_V216_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC20
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V216_VERSION_0            Bit_RESET
/***************************************************公用模块*****************************************/
//三针内针水维护选通阀
#define OUTPUT_LIQUID_VALVE_V309_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC18
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V309_VERSION_0            Bit_RESET
//三通阀切换到正常运行状态,阀和清洗液导通
#define LIQUID_CLEAN_OUTPUT_LIQUID_VALVE_V309_VERSION_0           Bit_SET
//三通阀切换到清洗维护状态,阀和纯水导通
#define LIQUID_PURE_WATER_OUTPUT_LIQUID_VALVE_V309_VERSION_0      Bit_RESET

//三针外壁清洗泵
#define OUTPUT_LIQUID_PUMP_DP305_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC27
//打开DP305的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP305_VERSION_0            Bit_RESET

//试剂针磁珠针内壁清洗泵
#define OUTPUT_LIQUID_PUMP_DP306_VERSION_0                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC26
//打开DP305的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP306_VERSION_0            Bit_RESET
/***************************************************抽废液*****************************************/

/***************************************************三针稀释*****************************************/

/************************************************通用延时*****************************************/
//泵操作等待稳定时间
#define TIME_MS_WAIT_STABLE_VALVE_VERSION_0                   30
//阀操作等待稳定时间
#define TIME_MS_WAIT_STABLE_PUMP_VERSION_0                    30





#endif




