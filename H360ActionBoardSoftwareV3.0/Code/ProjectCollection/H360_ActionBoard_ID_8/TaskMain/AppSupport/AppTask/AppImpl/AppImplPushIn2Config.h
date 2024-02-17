/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-27 17:23:53
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-28 10:24:15
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushIn2Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_IN2_CONFIG_H_
#define __APP_IMPL_PUSH_IN2_CONFIG_H_
#include "AppImplBase.h"

/****************************************电机映射定义***************************************/
#define PUSH_IN_2_STEP_MOTOR_ID              STEP_MOTOR2
/*****************************************输入IO口定义*************************************/
//进样2原点传感器
#define INPUT_PUSHIN2_ORGIN                     BOARD_IN_MCU_ARM_SEN2_PI9
#define VALID_LEVEL_PUSHIN2_ORGIN               Bit_RESET

//进样2试管架存在传感器
#define INPUT_PUSHIN2_RACK_EXIST                BOARD_IN_MCU_ARM_SEN11_PI6
#define VALID_LEVEL_PUSHIN2_RACK_EXIST          Bit_RESET
/****************************************输出IO口定义************************************/
//暂时没有,输出IO都在上位机

/****************************************运行曲线定义************************************/
//进样2调试速度
#define CURVE_PUSHIN2_DEBUG                     SM_CURVE_0
//进样2推进速度
#define CURVE_PUSHIN2_PUSH                      SM_CURVE_1
//进样2回零速度
#define CURVE_PUSHIN2_RETURN_ZERO               SM_CURVE_2


#endif

