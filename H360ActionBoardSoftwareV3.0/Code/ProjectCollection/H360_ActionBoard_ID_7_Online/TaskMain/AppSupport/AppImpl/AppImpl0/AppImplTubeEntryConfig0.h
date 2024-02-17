/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-12-20 09:29:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-12-20 09:31:06 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TUBE_ENTRY0_CONFIG0_H_
#define __APP_IMPL_TUBE_ENTRY0_CONFIG0_H_
#include "AppImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_ENTRY0_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY0_INPUT_INDEX_INTRANCE_SENSOR = 0,//入口传感器
}RAIL_ONLINE_TUBE_ENTRY0_INPUT_INDEX;

//输入节点入口传感器
#define INPUT_TUBE_ENTRY0_INTRANCE_SENSOR           BOARD_IN_MCU_PR1_ARM1_PG14
//输入节点入口传感器有效电平
#define VAILD_INPUT_TUBE_ENTRY0_INTRANCE_SENSOR     Bit_SET

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_IN_OUTPUT_START          = 0,//进样轨道直流电机启停
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_IN_OUTPUT_DIR            = 1,//进样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_IN_OUTPUT_SPEED          = 2,//进样轨道直流电机速度控制
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_IN_OUTPUT_EMSTOP         = 3,//进样轨道直流电机急停
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_IN_OUTPUT_ALARM_RELEASE  = 4,//进样轨道直流电机报警解除
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_OUT_OUTPUT_ENABLE        = 5,//出样轨道直流电机使能
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_OUT_OUTPUT_DIR           = 6,//出样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_OUT_OUTPUT_SPEED         = 7,//出样轨道直流电机速度控制
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_OUT_OUTPUT_EMSTOP        = 8,//出样轨道直流电机急停
    RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX_RAIL_OUT_OUTPUT_ALARM_RELEASE = 9,//出样轨道直流电机报警解除
}RAIL_ONLINE_TUBE_ENTRY0_OUTPUT_INDEX;

//输出引脚 进样轨道直流电机启停
#define OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_START                 BOARD_OUT_MCU_SLAVE_SEN20_PA5
//输出引脚 进样轨道直流电机启停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_START           Bit_RESET

//输出引脚 进样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_DIR                   BOARD_OUT_MCU_SLAVE_SEN21_PA4
//输出引脚 进样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_DIR             Bit_SET

//输出引脚 进样轨道直流电机速度控制
#define OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_SPEED                 BOARD_OUT_MCU_SLAVE_SEN24_PF9
//输出引脚 进样轨道直流电机速度控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_SPEED           Bit_RESET

//输出引脚 进样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_EMSTOP                BOARD_OUT_MCU_SLAVE_SEN23_PF8
//输出引脚 进样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_EMSTOP          Bit_SET

//输出引脚 进样轨道直流电机报警解除
#define OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_ALARM_RELEASE         BOARD_OUT_MCU_SLAVE_SEN22_PA3
//输出引脚 进样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_IN_OUTPUT_ALARM_RELEASE   Bit_RESET


//输出引脚 出样轨道直流电机使能
#define OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_START                BOARD_OUT_MCU_SLAVE_DC2_RUBBISH_MOTOR1_PG9
//输出引脚 出样轨道直流电机使能有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_START          Bit_RESET

//输出引脚 出样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_DIR                  BOARD_OUT_MCU_SLAVE_DC4_RUBBISH_MOTOR3_PI3
//输出引脚 出样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_DIR            Bit_SET

//输出引脚 出样轨道直流电机速度控制
#define OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_SPEED                BOARD_OUT_MCU_SLAVE_DC5_RUBBISH_MOTOR4_PI2
//输出引脚 出样轨道直流电机速度控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_SPEED          Bit_RESET

//输出引脚 出样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_EMSTOP               BOARD_OUT_MCU_SLAVE_DC3_RUBBISH_MOTOR2_PD6
//输出引脚 出样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_EMSTOP         Bit_SET

//输出引脚 出样轨道直流电机报警解除
#define OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_ALARM_RELEASE        BOARD_OUT_MCU_SLAVE_SEN26_PH3
//输出引脚 出样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY0_RAIL_OUT_OUTPUT_ALARM_RELEASE  Bit_RESET


//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_ENTRY0_SM_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY0_SM_INDEX_RESERVE    = 0,//保留
}RAIL_ONLINE_TUBE_ENTRY0_SM_INDEX;


#endif