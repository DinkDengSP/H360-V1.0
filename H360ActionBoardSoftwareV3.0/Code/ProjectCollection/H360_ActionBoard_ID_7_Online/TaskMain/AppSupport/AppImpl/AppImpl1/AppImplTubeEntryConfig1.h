/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 10:14:14
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_ENTRY_CONFIG1_H_
#define __APP_IMPL_TUBE_ENTRY_CONFIG1_H_
#include "AppImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX_INTRANCE_SENSOR = 0,//入口传感器
}RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX;

//输入节点入口传感器
#define INPUT_TUBE_ENTRY1_INTRANCE_SENSOR           BOARD_IN_MCU_PR1_ARM1_PG14
//输入节点入口传感器有效电平
#define VAILD_INPUT_TUBE_ENTRY1_INTRANCE_SENSOR     Bit_SET

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_START          = 0,//进样轨道直流电机启停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_DIR            = 1,//进样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_EMSTOP         = 2,//进样轨道直流电机急停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_START         = 3,//出样轨道直流电机启停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_DIR           = 4,//出样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_EMSTOP        = 5,//出样轨道直流电机急停
}RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX;

//输出引脚 进样轨道直流电机启停
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START                 BOARD_OUT_MCU_SLAVE_SEN20_PA5
//输出引脚 进样轨道直流电机启停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START           Bit_SET

//输出引脚 进样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR                   BOARD_OUT_MCU_SLAVE_SEN21_PA4
//输出引脚 进样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR             Bit_SET

//输出引脚 进样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP                BOARD_OUT_MCU_SLAVE_SEN23_PF8
//输出引脚 进样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP          Bit_RESET

//输出引脚 出样轨道直流电机使能
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START                BOARD_OUT_MCU_SLAVE_DC2_RUBBISH_MOTOR1_PG9
//输出引脚 出样轨道直流电机使能有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START          Bit_SET

//输出引脚 出样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR                  BOARD_OUT_MCU_SLAVE_DC4_RUBBISH_MOTOR3_PI3
//输出引脚 出样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR            Bit_SET

//输出引脚 出样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP               BOARD_OUT_MCU_SLAVE_DC3_RUBBISH_MOTOR2_PD6
//输出引脚 出样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP         Bit_RESET


//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX_RESERVE    = 0,//保留
}RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX;


#endif
