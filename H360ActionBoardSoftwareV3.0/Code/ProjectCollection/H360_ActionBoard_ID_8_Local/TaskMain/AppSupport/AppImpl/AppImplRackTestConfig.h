/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:17:25
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-07 10:30:35
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_TEST_CONFIG_H_
#define __APP_IMPL_RACK_TEST_CONFIG_H_
#include "AppImplBase.h"

//换管电机转一圈的步数 8细分
#define STEPS_RACK_TEST_MAIN_MOTOR_ONE_CYCLE    1600
//换管电机位置定义
#define STEP_RACK_TUBE_SWITCH_5_TO_1_OFFSET     266
#define STEP_RACK_TUBE_SWITCH_1_TO_2_OFFSET     266
#define STEP_RACK_TUBE_SWITCH_2_TO_3_OFFSET     266
#define STEP_RACK_TUBE_SWITCH_3_TO_4_OFFSET     266
#define STEP_RACK_TUBE_SWITCH_4_TO_5_OFFSET_A   266
#define STEP_RACK_TUBE_SWITCH_4_TO_5_OFFSET_B   270//536-266
//切换到释放位置的坐标
#define POS_RACK_TEST_RELEASE_RACK              1280
//换管电机复位找原点的单次运转步数
#define STEP_RACK_TEST_MAIN_MOTOR_RESET_MICRO   10   
//等待试管架移动稳定
#define TIME_MS_RACK_TEST_WAIT_MOVE_STBLE       250


//-------------------------------------------输入配置----------------------------------------------
//输入序号
typedef enum RACK_TEST_INPUT
{
    RACK_TEST_INPUT_EXIST           = 0X00,//试管架是否存在
    RACK_TEST_INPUT_FULL            = 0X01,//试管架是否满
    RACK_TEST_INPUT_MOTOR_ORIGIN    = 0X02,//旋转电机原点
    RACK_TEST_INPUT_ENCODER         = 0X03,//测试区码盘信号
}RACK_TEST_INPUT;

//输入IO选定-试管架是否存在
#define INPUT_RACK_TEST_INPUT_EXIST                 BOARD_IN_MCU_SLAVE_ARM_SEN8_PB11
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_TEST_INPUT_EXIST           Bit_SET

//输入IO选定-试管架是否满
#define INPUT_RACK_TEST_INPUT_FULL                  BOARD_IN_MCU_SLAVE_ARM_SEN9_PB10
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_TEST_INPUT_FULL            Bit_SET

//输入IO选定-旋转电机原点
#define INPUT_RACK_TEST_INPUT_MOTOR_ORIGIN          BOARD_IN_MCU_SLAVE_ARM_SEN5_PH8
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_TEST_INPUT_MOTOR_ORIGIN    Bit_RESET

//输入IO选定-测试区码盘信号
#define INPUT_RACK_TEST_INPUT_ENCODER               BOARD_IN_MCU_SLAVE_ARM_SEN1_PH12
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_TEST_INPUT_ENCODER         Bit_SET


//-------------------------------------------输出配置----------------------------------------------
//输出序号
typedef enum RACK_TEST_OUTPUT
{
    RACK_TEST_OUTPUT_RESERVE                = 0X00,//保留
}RACK_TEST_OUTPUT;


//-------------------------------------------电机配置----------------------------------------------
//电机序号
typedef enum RACK_TEST_SM
{
    RACK_TEST_SM_MAIN  = 0X00,//测试区换管电机
}RACK_TEST_SM;

//测试区换管电机
#define RACK_TEST_SM_MAIN_ID                    STEP_MOTOR9
//测试区换管电机电机的运行速度曲线
#define S_CURVE_RACK_TEST_SM_MAIN_DEBUG         SM_CURVE_0//调试曲线
#define S_CURVE_RACK_TEST_SM_MAIN_SWITCH        SM_CURVE_1//切换曲线
#define S_CURVE_RACK_TEST_SM_MAIN_RETURN_ZERO   SM_CURVE_2//回零曲线
#define S_CURVE_RACK_TEST_SM_MAIN_MICRO         SM_CURVE_3//微步曲线
#define S_CURVE_RACK_TEST_SM_MAIN_RUN_RELEASE   SM_CURVE_4//换管运行到释放位置
#define S_CURVE_RACK_TEST_SM_MAIN_SWITCH4_TO_5  SM_CURVE_5//切换曲线4到5试管

#endif




