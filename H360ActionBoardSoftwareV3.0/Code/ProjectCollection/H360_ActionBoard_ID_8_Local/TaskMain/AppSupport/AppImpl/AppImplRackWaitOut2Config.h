/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:17:43
 *LastEditors: DengXiaoJun
 *LastEditTime: 2022-01-14 09:47:13
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_WAIT_OUT2_CONFIG_H_
#define __APP_IMPL_RACK_WAIT_OUT2_CONFIG_H_
#include "AppImplBase.h"

//节点等待释放从有到无的时间
#define TIME_MS_RACK_WAIT_OUT2_WAIT_RACK_MOVE_OUT       5000
//节点等待试管架稳定释放时间
#define TIME_MS_RACK_WAIT_OUT2_WAIT_RELEASE_STABLE      1000
//等待传感器运行到下个节点最长时间
#define TIME_OUT_MS_RACK_WAIT_OUT2_ARRIVE_NEXT_NODE     6000
//在下个节点稳定触发的时间
#define TIME_STABLE_MS_RACK_WAIT_OUT2_ARRIVE_NEXT_NODE  200

//-------------------------------------------输入配置----------------------------------------------
//输入序号
typedef enum RACK_WAIT_OUT2_INPUT
{
    RACK_WAIT_OUT2_INPUT_EXIST          = 0X00,//检测试管架是否存在
    RACK_WAIT_OUT2_INPUT_MOTOR_ORIGIN   = 0X01,//旋转电机原点
}RACK_WAIT_OUT2_INPUT;

//输入IO选定-检测试管架是否存在
#define INPUT_RACK_WAIT_OUT2_INPUT_EXIST                BOARD_IN_MCU_PR2_ARM2_PG10
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_WAIT_OUT2_INPUT_EXIST          Bit_SET

//输入IO选定-旋转电机原点
#define INPUT_RACK_WAIT_OUT2_INPUT_MOTOR_ORIGIN         BOARD_IN_MCU_FRUBBISH_OVER1_PI0
//输入IO的有效状态选定  
#define VAILD_INPUT_RACK_WAIT_OUT2_INPUT_MOTOR_ORIGIN   Bit_RESET

//-------------------------------------------输出配置----------------------------------------------
//输出序号
typedef enum RACK_WAIT_OUT2_OUTPUT
{
    RACK_WAIT_OUT2_OUTPUT_RESERVE                = 0X00,//保留
}RACK_WAIT_OUT2_OUTPUT;


//-------------------------------------------电机配置----------------------------------------------
//电机序号
typedef enum RACK_WAIT_OUT2_SM
{
    RACK_WAIT_OUT2_SM_MAIN  = 0X00,//阻挡电机
}RACK_WAIT_OUT2_SM;

//出样等待位2阻挡电机
#define RACK_WAIT_OUT2_SM_MAIN_ID               STEP_MOTOR6
//出样等待位2阻挡电机的运行速度曲线
#define S_CURVE_RACK_WAIT_OUT2_SM_MAIN_DEBUG        SM_CURVE_0//调试曲线
#define S_CURVE_RACK_WAIT_OUT2_SM_MAIN_BRAKE        SM_CURVE_1//阻挡电机挡住状态
#define S_CURVE_RACK_WAIT_OUT2_SM_MAIN_RELEASE      SM_CURVE_2//阻挡电机释放状态
#define S_CURVE_RACK_WAIT_OUT2_SM_MAIN_RETURN_ZERO  SM_CURVE_3//限位电机回零

#endif
