/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-10-11 17:37:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-22 14:49:40
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SRV_IMPL_LOCAL_RACK_ENTRY1_CONFIG_H_
#define __SRV_IMPL_LOCAL_RACK_ENTRY1_CONFIG_H_
#include "SrvImplBase.h"


//-------------------------------------------输入配置----------------------------------------------
//输入序号定义
typedef enum RAIL_LOCAL_RACK_ENTRY1_INPUT
{
    RAIL_LOCAL_RACK_ENTRY1_INPUT_EXIST         = 0X00,//进样检测光电
    RAIL_LOCAL_RACK_ENTRY1_INPUT_MOTOR_ORIGIN  = 0X01,//进样防夹电机原点
}RAIL_LOCAL_RACK_ENTRY1_INPUT;

//输入IO选定-进样检测光电
#define INPUT_RAIL_LOCAL_RACK_ENTRY1_INPUT_EXIST                  SYSTEM_INPUT_ACTION_BOARD8_MCU_SLAVE_ARM_SEN10_PC0
//输入IO的有效状态选定
#define VAILD_INPUT_RAIL_LOCAL_RACK_ENTRY1_INPUT_EXIST            GPIO_PIN_SET

//输入IO选定-进样防夹电机原点
#define INPUT_RAIL_LOCAL_RACK_ENTRY1_INPUT_MOTOR_ORIGIN           SYSTEM_INPUT_ACTION_BOARD8_MCU_SLAVE_ARM_SEN7_PH6
//输入IO的有效状态选定
#define VAILD_INPUT_RAIL_LOCAL_RACK_ENTRY1_INPUT_MOTOR_ORIGIN     GPIO_PIN_RESET


//-------------------------------------------输出配置----------------------------------------------
//输出序号定义
typedef enum RAIL_LOCAL_RACK_ENTRY1_OUTPUT
{
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_START             = 0X00,//进样轨道直流电机启停
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_DIR               = 0X01,//进样轨道直流电机方向控制
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_EMSTOP            = 0X02,//进样轨道直流电机急停
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_START            = 0X03,//出样轨道直流电机启停
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_DIR              = 0X04,//出样轨道直流电机方向控制
    RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP           = 0X05,//出样轨道直流电机急停
}RAIL_LOCAL_RACK_ENTRY1_OUTPUT;

//输出引脚-进样轨道直流电机启停
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_START                 SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_SEN20_PA5
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_START           GPIO_PIN_SET

//输出引脚-进样轨道直流电机方向控制
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_DIR                   SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_SEN21_PA4
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_DIR             GPIO_PIN_SET

//输出引脚-进样轨道直流电机急停
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_EMSTOP                SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_SEN23_PF8
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_IN_EMSTOP          GPIO_PIN_RESET

//输出引脚-出样轨道直流电机启停
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_START                SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_DC2_PG9
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_START          GPIO_PIN_SET

//输出引脚-出样轨道直流电机方向控制
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_DIR                  SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_DC4_PI3
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_DIR            GPIO_PIN_RESET

//输出引脚-出样轨道直流电机急停
#define OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP               SYSTEM_OUTPUT_ACTION_BOARD8_MCU_SLAVE_DC3_PD6
//输出引脚有效电平
#define VAILD_OUTPUT_RAIL_LOCAL_RACK_ENTRY1_OUT_RAIL_OUT_EMSTOP         GPIO_PIN_RESET


#endif

