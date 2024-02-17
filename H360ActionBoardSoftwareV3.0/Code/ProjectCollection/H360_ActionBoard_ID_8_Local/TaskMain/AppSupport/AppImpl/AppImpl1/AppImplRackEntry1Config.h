/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:16:28
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-14 09:48:16
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_ENTRY1_CONFIG_H_
#define __APP_IMPL_RACK_ENTRY1_CONFIG_H_
#include "AppImplBase.h"

//节点等待释放的最长等待时间
#define TIME_MS_MAX_RACK_ENTRY1_WAIT_RACK_MOVE_OUT           6000
//节点确认释放的有效时间
#define TIME_MS_STABLE_RACK_ENTRY1_WAIT_RACK_MOVE_OUT        1200
//节点释放到下一个节点的最长等待时间
#define TIME_MS_MAX_RACK_ENTRY1_RELEASE_ARRIVE_NEXT_NODE     4000
//节点到下一节点的稳定时间
#define TIME_MS_STABLE_RACK_ENTRY1_RELEASE_ARRIVE_NEXT_NODE  200

//-------------------------------------------输入配置----------------------------------------------
//输入序号定义
typedef enum RACK_ENTRY1_INPUT
{
    RACK_ENTRY1_INPUT_EXIST         = 0X00,//进样检测光电
    RACK_ENTRY1_INPUT_MOTOR_ORIGIN  = 0X01,//进样防夹电机原点
}RACK_ENTRY1_INPUT;

//输入IO选定-进样检测光电
#define INPUT_RACK_ENTRY1_INPUT_EXIST                  BOARD_IN_MCU_SLAVE_ARM_SEN10_PC0
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_ENTRY1_INPUT_EXIST            Bit_SET

//输入IO选定-进样防夹电机原点
#define INPUT_RACK_ENTRY1_INPUT_MOTOR_ORIGIN           BOARD_IN_MCU_SLAVE_ARM_SEN7_PH6
//输入IO的有效状态选定
#define VAILD_INPUT_RACK_ENTRY1_INPUT_MOTOR_ORIGIN     Bit_RESET


//-------------------------------------------输出配置----------------------------------------------
//输出序号定义
typedef enum RACK_ENTRY1_OUTPUT
{
    RACK_ENTRY1_OUT_RAIL_IN_START             = 0X00,//进样轨道直流电机启停
    RACK_ENTRY1_OUT_RAIL_IN_DIR               = 0X01,//进样轨道直流电机方向控制
    RACK_ENTRY1_OUT_RAIL_IN_EMSTOP            = 0X02,//进样轨道直流电机急停
    RACK_ENTRY1_OUT_RAIL_OUT_START            = 0X03,//出样轨道直流电机启停
    RACK_ENTRY1_OUT_RAIL_OUT_DIR              = 0X04,//出样轨道直流电机方向控制
    RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP           = 0X05,//出样轨道直流电机急停
}RACK_ENTRY1_OUTPUT;

//输出引脚-进样轨道直流电机启停
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_START                 BOARD_OUT_MCU_SLAVE_SEN20_PA5
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_START           Bit_SET

//输出引脚-进样轨道直流电机方向控制
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_DIR                   BOARD_OUT_MCU_SLAVE_SEN21_PA4
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_DIR             Bit_SET

//输出引脚-进样轨道直流电机急停
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_EMSTOP                BOARD_OUT_MCU_SLAVE_SEN23_PF8
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_IN_EMSTOP          Bit_RESET

//输出引脚-出样轨道直流电机启停
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_START                BOARD_OUT_MCU_SLAVE_DC2_RUBBISH_MOTOR1_PG9
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_START          Bit_SET

//输出引脚-出样轨道直流电机方向控制
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_DIR                  BOARD_OUT_MCU_SLAVE_DC4_RUBBISH_MOTOR3_PI3
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_DIR            Bit_RESET

//输出引脚-出样轨道直流电机急停
#define OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP               BOARD_OUT_MCU_SLAVE_DC3_RUBBISH_MOTOR2_PD6
//输出引脚有效电平
#define VAILD_OUTPUT_RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP         Bit_RESET



//-------------------------------------------电机配置----------------------------------------------
//电机序号
typedef enum RACK_ENTRY1_SM
{
    RACK_ENTRY1_SM_CLAMP    = 0X00,//进样防夹
}RACK_ENTRY1_SM;

//试管架防卡电机
#define RACK_ENTRY1_SM_CLAMP_ID                  STEP_MOTOR8
//试管架防卡电机的运行速度曲线
#define S_CURVE_RACK_ENTRY1_SM_CLAMP_DEBUG       SM_CURVE_0//调试曲线
#define S_CURVE_RACK_ENTRY1_SM_CLAMP_RETURN_ZERO SM_CURVE_1//回零曲线
#define S_CURVE_RACK_ENTRY1_SM_CLAMP_ACTION      SM_CURVE_2//运动到工作位置
#define S_CURVE_RACK_ENTRY1_SM_CLAMP_IDLE        SM_CURVE_3//运动到空闲位置

#endif
