/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-09-24 11:59:48
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_CLAMP_CONFIG_H_
#define __APP_IMPL_TUBE_CLAMP_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_CLAMP_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_CLAMP_INPUT_INDEX_SM_ORIGIN_SENSOR= 0,//电机原点传感器
}RAIL_ONLINE_TUBE_CLAMP_INPUT_INDEX;

//电机原点传感器
#define INPUT_RAIL_ONLINE_TUBE_CLAMP_SM_UPDOWN_ORIGIN_SENSOR           BOARD_IN_MCU_SLAVE_ARM_SEN14_PB1
//电机原点传感器有效电平
#define VAILD_INPUT_RAIL_ONLINE_TUBE_CLAMP_SM_UPDOWN_ORIGIN_SENSOR     Bit_RESET

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_CLAMP_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_CLAMP_OUTPUT_INDEX_RESERVE  = 0,//保留
}RAIL_ONLINE_TUBE_CLAMP_OUTPUT_INDEX;


//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_CLAMP_SM_INDEX
{
    RAIL_ONLINE_TUBE_CLAMP_SM_UPDOWN = 0,//升降电机
}RAIL_ONLINE_TUBE_CLAMP_SM_INDEX;

//试管固定节点电机索引
#define TUBE_CLAMP_SM_MAIN                      STEP_MOTOR6
//试管固定节点电机运行速度曲线
#define S_CURVE_TUBE_CLAMP_SM_MAIN_DEBUG        SM_CURVE_0 //试管固定机构调试速度曲线
#define S_CURVE_TUBE_CLAMP_SM_MAIN_DOWN         SM_CURVE_1 //试管固定机构下降运行速度曲线
#define S_CURVE_TUBE_CLAMP_SM_MAIN_UP           SM_CURVE_2 //试管固定机构上升运行速度曲线





#endif
