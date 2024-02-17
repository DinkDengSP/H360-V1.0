/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:16:45
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 19:23:19
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_OUT_SWITCH_CONFIG_H_
#define __APP_IMPL_RACK_OUT_SWITCH_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------输入配置----------------------------------------------
//输入序号
typedef enum RACK_OUT_SWITCH_INPUT
{
    RACK_OUT_SWITCH_INPUT_COME_IN             = 0X00,//换轨进入传感器
    RACK_OUT_SWITCH_INPUT_MOVE_OUT            = 0X01,//换轨离去传感器
    RACK_OUT_SWITCH_INPUT_MOTOR_ORIGIN        = 0X02,//出样换轨电机原点
}RACK_OUT_SWITCH_INPUT;

//输入IO选定-换轨进入传感器
#define INPUT_RACK_OUT_SWITCH_INPUT_COME_IN             BOARD_IN_MCU_SLAVE_ARM_SEN16_PC5
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_OUT_SWITCH_INPUT_COME_IN       Bit_SET

//输入IO选定-换轨离去传感器
#define INPUT_RACK_OUT_SWITCH_INPUT_MOVE_OUT            BOARD_IN_MCU_ARM_SEN4_PI8
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_OUT_SWITCH_INPUT_MOVE_OUT      Bit_SET

//输入IO选定-出样换轨电机原点
#define INPUT_RACK_OUT_SWITCH_INPUT_MOTOR_ORIGIN        BOARD_IN_MCU_SLAVE_ARM_SEN15_PB0
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_OUT_SWITCH_INPUT_MOTOR_ORIGIN  Bit_RESET

//输出序号
typedef enum RACK_OUT_SWITCH_OUTPUT
{
    RACK_OUT_SWITCH_OUTPUT_RESERVE                      = 0X00,//保留
}RACK_OUT_SWITCH_OUTPUT;


//-------------------------------------------电机配置----------------------------------------------
//电机序号
typedef enum RACK_OUT_SWITCH_SM
{
    RACK_OUT_SWITCH_SM_MAIN  = 0X00,//换轨电机
}RACK_OUT_SWITCH_SM;

//换轨电机
#define RACK_OUT_SWITCH_SM_MAIN_ID                      STEP_MOTOR4
//换轨电机的运行速度曲线
#define S_CURVE_RACK_OUT_SWITCH_SM_MAIN_DEBUG           SM_CURVE_0//调试曲线
#define S_CURVE_RACK_OUT_SWITCH_SM_MAIN_RETURN_ZERO     SM_CURVE_1//回零曲线
#define S_CURVE_RACK_OUT_SWITCH_SM_MAIN_SWITCH          SM_CURVE_2//换轨状态

#endif
