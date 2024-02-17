/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-04-27 17:20:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-28 18:20:44
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushIn1Config.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PUSH_IN1_CONFIG_H_
#define __APP_IMPL_PUSH_IN1_CONFIG_H_
#include "AppImplBase.h"

/****************************************电机映射定义***************************************/
//样本针升降电机编号
#define PUSH_IN_1_STEP_MOTOR_ID              STEP_MOTOR1

/*****************************************输入IO口定义*************************************/
//进样1原点传感器
#define INPUT_PUSHIN1_ORGIN                     BOARD_IN_MCU_ARM_SEN1_PI10
#define VALID_LEVEL_PUSHIN1_ORGIN               Bit_RESET

//进样1试管架有无传感器,进样1有无,代表是否推到试管架
#define INPUT_PUSHIN1_RACK_EXIST                BOARD_IN_MCU_ARM_SEN10_PI7
#define VALID_LEVEL_PUSHIN1_RACK_EXIST          Bit_RESET

/****************************************输出IO口定义************************************/
//暂时没有,输出IO都在上位机

/****************************************运行曲线定义************************************/
#define CURVE_PUSHIN1_DEBUG                     SM_CURVE_0//进样1调试速度
#define CURVE_PUSHIN1_PUSH                      SM_CURVE_1//进样1推进速度
#define CURVE_PUSHIN1_RETURN_ZERO               SM_CURVE_2//进样1回零速度


#endif




