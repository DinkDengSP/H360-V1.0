/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
**LastEditors: DengXiaoJun
**LastEditTime: 2021-09-26 16:48:04
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_BAR_SCAN_CONFIG_H_
#define __APP_IMPL_TUBE_BAR_SCAN_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------模块运行使用配置---------------------------------------------
//条码扫码单次最大持续时间,毫秒
#define TIME_BAR_SCAN_ONCE_MAX_DURTION_TIME_MS            2000

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_BAR_SCAN_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_BAR_SCAN_INPUT_INDEX_RESERVE    = 0,//保留
}RAIL_ONLINE_TUBE_BAR_SCAN_INPUT_INDEX;

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_BAR_SCAN_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_BAR_SCAN_OUTPUT_INDEX_RESERVE  = 0,//保留
}RAIL_ONLINE_TUBE_BAR_SCAN_OUTPUT_INDEX;

//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX
{
    RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX_ROTATE = 0,//扫码旋转电机
}RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX;

//扫码节点电机索引
#define TUBE_BAR_SCAN_SM_ROTATE                  STEP_MOTOR4

//扫码节点电机调试速度曲线
#define S_CURVE_TUBE_BAR_SCAN_DEBUG              SM_CURVE_0 //电机调试速度曲线
//扫码节点电机运行速度曲线
#define S_CURVE_TUBE_BAR_SCAN_ROTATE_SCAN        SM_CURVE_1 //电机扫码速度曲线





#endif
