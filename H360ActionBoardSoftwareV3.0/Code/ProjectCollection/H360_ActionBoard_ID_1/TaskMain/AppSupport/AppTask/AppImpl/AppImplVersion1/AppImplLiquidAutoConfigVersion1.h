/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-14 21:22:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 20:53:55
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidAutoConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_AUTO_CONFIG_VERSION_1_H_
#define __APP_IMPL_LIQUID_AUTO_CONFIG_VERSION_1_H_
#include "AppImplBase.h"

/*************************************************废液相关信号********************************************************/
//废液箱上浮球
#define INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1                             CAN_SUB_IN_PIN_BOARD1_MCU_SLAVE_ARM_SEN1_PH12
/*废液桶上液位传感器有效电平,上液位满的时候,浮子浮起来,浮子触发传感器,下液位浮子和上液位浮子的传感器有差异 */
#define VALID_LEVEL_INPUT_WASTE_TANK_LEVEL_HIGH_VERSION_1                 Bit_SET

//V307废液箱废液排放阀
#define OUTPUT_LIQUID_VALVE_V307_VERSION_1                                CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN26_PH3
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V307_VERSION_1                    Bit_RESET

//DP1废液箱废液排放泵
#define OUTPUT_LIQUID_PUMP_DP1_VERSION_1                                  CAN_SUB_OUT_PIN_BOARD1_MCU_SLAVE_SEN25_PF10
//打开DP1的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP1_VERSION_1                      Bit_RESET

/********************************************液路自动化稀释*****************************************/
//DP101 配置瓶到目标瓶抽液泵
#define OUTPUT_LIQUID_PUMP_DP101_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1
//打开DP101的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP101_VERSION_1            Bit_RESET

//DP304 浓缩液抽液到配置瓶泵
#define OUTPUT_LIQUID_PUMP_DP304_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC24
//打开DP304的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP304_VERSION_1            Bit_RESET

//DP103 纯水抽液到配置瓶抽液泵
#define OUTPUT_LIQUID_PUMP_DP103_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC3
//打开DP103的电平
#define VALID_LEVEL_OUTPUT_LIQUID_PUMP_DP103_VERSION_1            Bit_RESET

//V235 浓缩液纯水选通阀,关闭选通纯水,打开选通浓缩液
#define OUTPUT_LIQUID_VALVE_V235_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC8
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1            Bit_RESET
//三通阀切换到浓缩液的电平
#define CONCENTRATE_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1      Bit_RESET
//三通阀切换到纯水的电平
#define PURE_WATER_LEVEL_OUTPUT_LIQUID_VALVE_V235_VERSION_1       Bit_SET

//V240 浓缩液选瓶阀,关闭选通瓶1,打开选通瓶2
#define OUTPUT_LIQUID_VALVE_V240_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC17
//打开三通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1            Bit_RESET
//三通阀切换到瓶1的电平
#define BOTTLE_1_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1         Bit_SET
//三通阀切换到瓶2的电平
#define BOTTLE_2_LEVEL_OUTPUT_LIQUID_VALVE_V240_VERSION_1         Bit_RESET

//纯水水箱导通阀
#define OUTPUT_LIQUID_VALVE_V237_VERSION_1                        CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC14
//打开两通阀的电平
#define VALID_LEVEL_OUTPUT_LIQUID_VALVE_V237_VERSION_1            Bit_RESET



#endif


