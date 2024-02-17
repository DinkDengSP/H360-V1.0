/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-01 15:46:13
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 15:50:18
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplConfig\ServiceImplRailTransConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_TRANS_CONFIG_H_
#define __SERVICE_IMPL_RAIL_TRANS_CONFIG_H_
#include "SystemInput.h"
#include "SystemOutput.h"

/**************************************************轨道原点资源表**************************************************/
////////////////////////////////////////////////输入/////////////////////////////////////////////////
//轨道原点
#define INPUT_RAIL_TRANS_ORGIN_SENSOR                   SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN9_PE2
#define VALID_LEVEL_INPUT_RAIL_TRANS_ORGIN_SENSOR       Bit_RESET//挡住

//进样方向,辅助定位传感器
#define INPUT_RAIL_TRANS_UTIL_ADJUST_PUSHIN             SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN6_PE5
#define VALID_LEVEL_INPUT_RAIL_TRANS_UTIL_ADJUST_PUSHIN Bit_SET

//急诊方向,辅助定位传感器
#define INPUT_RAIL_TRANS_UTIL_ADJUST_EMERGENCY              SYSTEM_INPUT_ACTION_BOARD8_MCU_ARM_SEN7_PE4
#define VALID_LEVEL_INPUT_RAIL_TRANS_UTIL_ADJUST_EMERGENCY  Bit_SET


/**************************************************急诊资源表**************************************************/
////////////////////////////////////////////////输入/////////////////////////////////////////////////
//急诊到位
#define INPUT_RAIL_EMERGENCY_CALL_IN_PLACE             SYSTEM_INPUT_RAIL_BOARD_ARM_SEN5_PH6
#define VALID_LEVEL_INPUT_RAIL_EMERGENCY_CALL_IN_PLACE Bit_RESET//挡住
//急诊有无
#define INPUT_RAIL_EMERGENCY_CALL_EXIST                SYSTEM_INPUT_RAIL_BOARD_ARM_SEN2_PH9
#define VALID_LEVEL_INPUT_RAIL_EMERGENCY_CALL_EXIST    Bit_SET//光电

////////////////////////////////////////////////输出/////////////////////////////////////////////////
//急诊绿灯
#define OUTPUT_RAIL_EMERGENCY_LED_GREEN                SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT1_PG15
//急诊黄灯
#define OUTPUT_RAIL_EMERGENCY_LED_YELLOW               SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT2_PB3
//急诊电子锁
#define OUTPUT_RAIL_EMERGENCY_LOCK_CALL                SYSTEM_OUTPUT_RAIL_BOARD_ARM_OUT18_PH12  



#endif


