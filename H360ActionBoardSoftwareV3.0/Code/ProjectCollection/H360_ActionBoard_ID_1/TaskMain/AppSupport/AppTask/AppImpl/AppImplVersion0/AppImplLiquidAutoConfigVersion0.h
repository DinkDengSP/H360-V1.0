/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-14 21:22:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 20:49:23
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidAutoConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_AUTO_CONFIG_VERSION_0_H_
#define __APP_IMPL_LIQUID_AUTO_CONFIG_VERSION_0_H_
#include "AppImplBase.h"
#include "AppLiquidUtilVersion0.h"

/*************************************************废液相关信号********************************************************/
//废液箱上浮球
#define INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_0                             CAN_SUB_IN_PIN_BOARD1_MCU_SLAVE_ARM_SEN1_PH12
/*废液桶上液位传感器有效电平,上液位满的时候,浮子浮起来,浮子触发传感器,下液位浮子和上液位浮子的传感器有差异 */
#define VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_0                 Bit_SET

//V307废液箱废液排放阀
#define OUTPUT_LIQUID_VALVE_V307_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN26_PH3
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_0                    Bit_RESET

//DP1废液箱废液排放泵
#define OUTPUT_LIQUID_PUMP_DP1_VERSION_0                                  CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN25_PF10
//打开DP1的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_0                      Bit_RESET

/***********************************************三针清洗液稀释相关液路*********************************************************/
//DP101,中转稀释液到目标稀释液抽液泵
#define OUTPUT_LIQUID_PUMP_DP101_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1
//打开DP101的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP101_VERSION_0                    Bit_RESET

//DP102中转浓缩液抽液泵
#define OUTPUT_LIQUID_PUMP_DP102_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC2
//打开DP102的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP102_VERSION_0                    Bit_RESET

//DP103,中转稀释液抽液泵
#define OUTPUT_LIQUID_PUMP_DP103_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC3
//打开DP103的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_0                    Bit_RESET
    
//V231,纯水选通阀
#define OUTPUT_LIQUID_VALVE_V231_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC4
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V231_VERSION_0                    Bit_RESET

//V235,浓缩液1选通阀
#define OUTPUT_LIQUID_VALVE_V235_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC8
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_0                    Bit_RESET

//V236,浓缩液2选通阀
#define OUTPUT_LIQUID_VALVE_V236_VERSION_0                                CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC9
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V236_VERSION_0                    Bit_RESET

#endif


