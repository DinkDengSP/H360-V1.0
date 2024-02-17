/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:17:02
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-24 09:47:05
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_STORE_OUT_CONFIG_H_
#define __APP_IMPL_RACK_STORE_OUT_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//出样仓输入序号定义
typedef enum RACK_STORE_OUT_INPUT
{
    RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY     = 0X00,//靠近按键托盘靠近轨道光电传感器
    RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL    = 0X01,//靠近按键托盘靠近按键光电传感器
    RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY    = 0X02,//靠近轨道托盘靠近按键光电传感器
    RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL   = 0X03,//靠近轨道托盘靠近轨道光电传感器
    RACK_STORE_OUT_INPUT_MOTOR_PUSH_ORIGIN          = 0X04,//出样仓推进步进电机原点
    RACK_STORE_OUT_INPUT_MOTOR_ROTATE_ORIGIN        = 0X05,//出样仓旋转步进电机原点
}RACK_STORE_OUT_INPUT;

//输入IO选定-靠近按键托盘靠近轨道光电传感器
#define INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY                       BOARD_IN_MCU_ARM_SEN13_PI4
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY                 Bit_SET

//输入IO选定-靠近按键托盘靠近按键光电传感器
#define INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL                      BOARD_IN_MCU_ARM_SEN12_PI5
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL                Bit_SET

//输入IO选定-靠近轨道托盘靠近按键光电传感器
#define INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY                      BOARD_IN_MCU_ARM_SEN11_PI6
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY                Bit_SET

//输入IO选定-靠近轨道托盘靠近轨道光电传感器
#define INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL                     BOARD_IN_MCU_ARM_SEN10_PI7
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL               Bit_SET

//输入IO选定-出样仓推进步进电机原点
#define INPUT_RACK_STORE_OUT_INPUT_MOTOR_PUSH_ORIGIN                            BOARD_IN_MCU_ARM_SEN5_PE6
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_MOTOR_PUSH_ORIGIN                      Bit_RESET

//输入IO选定-出样仓旋转步进电机原点
#define INPUT_RACK_STORE_OUT_INPUT_MOTOR_ROTATE_ORIGIN                          BOARD_IN_MCU_ARM_SEN1_PI10
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_STORE_OUT_INPUT_MOTOR_ROTATE_ORIGIN                    Bit_RESET


//-------------------------------------------输出配置----------------------------------------------
//出样仓输出序号定义
typedef enum RACK_STORE_OUT_OUTPUT
{
    RACK_STORE_OUT_OUTPUT_RESERVE = 0X00,//保留序号
}RACK_STORE_OUT_OUTPUT;


//-------------------------------------------电机配置----------------------------------------------
//出样仓电机序号
typedef enum RACK_STORE_OUT_SM
{
    RACK_STORE_OUT_SM_PUSH   = 0X00,//推手前后
    RACK_STORE_OUT_SM_ROTATE = 0X01,//推手旋转
}RACK_STORE_OUT_SM;

//出样推进电机
#define RACK_STORE_OUT_SM_PUSH_ID                       STEP_MOTOR2
//出样推进电机的运行速度曲线
#define S_CURVE_RACK_STORE_OUT_SM_PUSH_DEBUG            SM_CURVE_0//出样仓推手调试曲线
#define S_CURVE_RACK_STORE_OUT_SM_PUSH_TO_PREPARE       SM_CURVE_1//快速运行到出样准备位置
#define S_CURVE_RACK_STORE_OUT_SM_PUSH_PUSH             SM_CURVE_2//出样仓推手推进
#define S_CURVE_RACK_STORE_OUT_SM_PUSH_RETURN_ZERO      SM_CURVE_3//出样仓推手回零

//出样旋转电机
#define RACK_STORE_OUT_SM_ROTATE_ID                     STEP_MOTOR1
//出样旋转电机的运行速度曲线
#define S_CURVE_RACK_STORE_OUT_SM_ROTATE_DEBUG          SM_CURVE_0//出样仓推手旋转调试曲线
#define S_CURVE_RACK_STORE_OUT_SM_ROTATE_TO_ACTION      SM_CURVE_1//出样仓推手旋转运行到工作位置
#define S_CURVE_RACK_STORE_OUT_SM_ROTATE_TO_IDLE        SM_CURVE_2//出样仓推手旋转运行到空闲位置
#define S_CURVE_RACK_STORE_OUT_SM_ROTATE_RETURN_ZERO    SM_CURVE_3//出样仓推手旋转回零

#endif
