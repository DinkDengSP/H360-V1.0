/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-11-16 14:42:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_EXIT_CONFIG_H_
#define __APP_IMPL_TUBE_EXIT_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_EXIT_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_EXIT_INPUT_INDEX_EXIT_SENSOR = 0,//出口传感器
}RAIL_ONLINE_TUBE_EXIT_INPUT_INDEX;

//输出节点出口传感器
#define INPUT_TUBE_EXIT_EXIT_SENSOR                BOARD_IN_MCU_PR2_ARM2_PG10
//输出节点出口传感器有效电平
#define VAILD_INPUT_TUBE_EXIT_EXIT_SENSOR          Bit_SET


//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_EXIT_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_EXIT_OUTPUT_INDEX_RESERVE  = 0,//保留
}RAIL_ONLINE_TUBE_EXIT_OUTPUT_INDEX;

//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_EXIT_SM_INDEX
{
    RAIL_ONLINE_TUBE_EXIT_SM_INDEX_RESERVE    = 0,//保留
}RAIL_ONLINE_TUBE_EXIT_SM_INDEX;



#endif
