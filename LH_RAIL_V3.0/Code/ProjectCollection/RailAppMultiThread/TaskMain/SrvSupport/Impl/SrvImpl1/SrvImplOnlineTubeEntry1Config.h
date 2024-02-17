/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-10-11 17:40:51
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-17 14:31:37
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SRV_IMPL_ONLINE_TUBE_ENTRY1_CONFIG_H_
#define __SRV_IMPL_ONLINE_TUBE_ENTRY1_CONFIG_H_
#include "SrvImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX_INTRANCE_SENSOR = 0,//入口传感器
}RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX;

//输入节点入口传感器
#define INPUT_TUBE_ENTRY1_INTRANCE_SENSOR           SYSTEM_INPUT_ACTION_BOARD7_MCU_PR1_ARM1_PG14
//输入节点入口传感器有效电平
#define VAILD_INPUT_TUBE_ENTRY1_INTRANCE_SENSOR     GPIO_PIN_SET

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_START          = 0X00,//进样轨道直流电机启停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_DIR            = 0X01,//进样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_EMSTOP         = 0X02,//进样轨道直流电机急停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_START         = 0X03,//出样轨道直流电机启停
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_DIR           = 0X04,//出样轨道直流电机方向控制
    RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_EMSTOP        = 0X05,//出样轨道直流电机急停
}RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX;

//输出引脚 进样轨道直流电机启停
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START                 SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_SEN20_PA5
//输出引脚 进样轨道直流电机启停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START           GPIO_PIN_SET

//输出引脚 进样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR                   SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_SEN21_PA4
//输出引脚 进样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR             GPIO_PIN_SET

//输出引脚 进样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP                SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_SEN23_PF8
//输出引脚 进样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP          GPIO_PIN_RESET

//输出引脚 出样轨道直流电机启停
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START                SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_DC2_PG9
//输出引脚 出样轨道直流电机使能有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START          GPIO_PIN_SET

//输出引脚 出样轨道直流电机方向控制
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR                  SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_DC4_PI3
//输出引脚 出样轨道直流电机方向控制有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR            GPIO_PIN_SET

//输出引脚 出样轨道直流电机急停
#define OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP               SYSTEM_OUTPUT_ACTION_BOARD7_MCU_SLAVE_DC3_PD6
//输出引脚 出样轨道直流电机急停有效电平
#define VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP         GPIO_PIN_RESET


#endif

