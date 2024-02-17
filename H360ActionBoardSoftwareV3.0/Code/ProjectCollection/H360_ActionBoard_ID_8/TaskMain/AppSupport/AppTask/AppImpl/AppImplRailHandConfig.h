/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-27 17:24:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-10 13:25:52
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplRailHandConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RAIL_HAND_CONFIG_H_
#define __APP_IMPL_RAIL_HAND_CONFIG_H_
#include "AppImplBase.h"

/****************************************电机映射定义***************************************/
//机械手升降
#define RAIL_HAND_STEP_MOTOR_UPDOWN_ID                              STEP_MOTOR3
//机械手前后
#define RAIL_HAND_STEP_MOTOR_BACKFRONT_ID                           STEP_MOTOR4
//机械手旋转
#define RAIL_HAND_STEP_MOTOR_ROTATE_ID                              STEP_MOTOR5
/*****************************************输入IO口定义*************************************/
//机械手升降原点传感器
#define INPUT_HAND_UPDOWN_ORGIN                         BOARD_IN_MCU_ARM_SEN3_PC13
#define VALID_LEVEL_HAND_UPDOWN_ORGIN                   Bit_RESET

//机械手前后原点传感器
#define INPUT_HAND_BACKFRONT_ORGIN                      BOARD_IN_MCU_ARM_SEN4_PI8
#define VALID_LEVEL_HAND_BACKFRONT_ORGIN                Bit_RESET

//机械手旋转原点传感器
#define INPUT_HAND_ROTATE_ORGIN                         BOARD_IN_MCU_ARM_SEN5_PE6 
#define VALID_LEVEL_HAND_ROTATE_ORGIN                   Bit_RESET

//手上是否存在光电传感器
#define INPUT_HAND_RACK_EXIST                            BOARD_IN_MCU_ARM_SEN8_PE3
#define VALID_LEVEL_INPUT_HAND_RACK_EXIST                Bit_SET

/****************************************输出IO口定义************************************/

/****************************************运行曲线定义************************************/
//机械手升降调试速度
#define CURVE_HAND_UPDOWN_DEBUG                     SM_CURVE_0
//机械手升降升速度
#define CURVE_HAND_UPDOWN_UP                        SM_CURVE_1
//机械手升降降速度
#define CURVE_HAND_UPDOWN_DOWN                      SM_CURVE_2

//机械手前后调试速度
#define CURVE_HAND_BACKFRONT_DEBUG                  SM_CURVE_0
//机械手前后前速度
#define CURVE_HAND_BACKFRONT_FRONT                  SM_CURVE_1
//机械手前后后速度
#define CURVE_HAND_BACKFRONT_BACK                   SM_CURVE_2

//机械手旋转调试速度
#define CURVE_HAND_ROTATE_DEBUG                     SM_CURVE_0
//机械手旋转带试管架速度
#define CURVE_HAND_ROTATE_WITH_RACK                 SM_CURVE_1
//机械手旋转不带试管架速度
#define CURVE_HAND_ROTATE_WITHOUT_RACK              SM_CURVE_2

/**************************************延迟时间*********************************************/
#define TIME_MS_HAND_WAIT_RACK_STABLE               100

/**************************************模块参数定义*********************************************/
//机械手扫码开始升降下降到最高位置的比例。
#define RAIL_HAND_BAR_SCAN_START_DOWN_HIGH_MAX_SCALE               (0.33f)
//机械手前后在旋转时候的辅助旋转偏移
#define HAND_INDENPENDENT_BACKFRONT_UTIL_ROTATE_OFFSET              3000
#endif

