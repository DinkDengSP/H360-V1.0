/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:50:50
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-13 10:31:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_ROTATE_CONFIG_H_
#define __APP_IMPL_TUBE_ROTATE_CONFIG_H_
#include "AppImplBase.h"

//-------------------------------------------模块运行配置----------------------------------------------
//旋转电机旋转一次固定的步数
#define STEPS_TUBE_ROTATE_TUBE_ROTATE_ONCE                12000
//转盘上连续多次检测不到传感器讯号则复位通过,设置这个次数,应该是一圈
#define COUNT_ROTATE_SENSOR_INVALID_CLEARUP               32
//检测连续多次转盘信号不变化导致堵塞的阈值
#define COUNT_ROTATE_SENSOR_BLOCK                         8
//转盘复位清空的最大次数
#define COUNT_MAX_ROTATE_CLEARUP                          32*8
//旋转电机等待试管流出最大时间 毫秒
#define MS_TUBE_ROTATE_WAIT_TUBE_OUT_MAX_TIME             2000
//等待转盘稳定后读取传感器讯号
#define TIME_MS_WAIT_PLATE_STABLE                         200

//-------------------------------------------输入输出配置----------------------------------------------
//输入引脚读取索引
typedef enum RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX
{
    RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_SM_ORIGIN_SENSOR     = 0,//电机原点传感器    
    RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_INPUT_POS_SENSOR     = 1,//输入位传感器
    RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_BARSCAN_POS_SENSOR   = 2,//扫码位传感器
    RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_TEST_POS_SENSOR      = 3,//测试位传感器
    RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_OUTPUT_POS_SENSOR    = 4,//输出位传感器
}RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX;

//旋转节点电机原点传感器
#define TUBE_ENTRY_RAIL_ONLINE_TUBE_ROTATE_SM_ORIGIN_SENSOR                 BOARD_IN_MCU_SLAVE_ARM_SEN11_PC1
//旋转节点输入位传感器有效电平
#define VAILD_TUBE_ENTRY_RAIL_ONLINE_TUBE_ROTATE_SM_ORIGIN_SENSOR           Bit_RESET

//旋转节点输入位传感器
#define INPUT_TUBE_ROTATE_INPUT_POS_SENSOR                                  BOARD_IN_MCU_FRUBBISH_OVER1_PI0
//旋转节点输入位传感器有效电平          
#define VAILD_INPUT_TUBE_ROTATE_INPUT_POS_SENSOR                            Bit_SET

//旋转节点扫码位传感器          
#define INPUT_TUBE_ROTATE_BARSCAN_POS_SENSOR                                BOARD_IN_MCU_SLAVE_ARM_SEN12_PC2
//旋转节点扫码位传感器有效电平          
#define VAILD_INPUT_TUBE_ROTATE_BARSCAN_POS_SENSOR                          Bit_SET

//旋转节点测试位传感器          
#define INPUT_TUBE_ROTATE_TEST_POS_SENSOR                                   BOARD_IN_MCU_FRUBBISH2_PH14
//旋转节点测试位传感器有效电平          
#define VAILD_INPUT_TUBE_ROTATE_TEST_POS_SENSOR                             Bit_SET

//旋转节点输出位传感器          
#define INPUT_TUBE_ROTATE_OUTPUT_POS_SENSOR                                 BOARD_IN_MCU_PR2_ARM1_PG11
//旋转节点输出位传感器有效电平          
#define VAILD_INPUT_TUBE_ROTATE_OUTPUT_POS_SENSOR                           Bit_SET

//输出引脚写出索引
typedef enum RAIL_ONLINE_TUBE_ROTATE_OUTPUT_INDEX
{
    RAIL_ONLINE_TUBE_ROTATE_OUTPUT_INDEX_RESERVE  = 0,//保留
}RAIL_ONLINE_TUBE_ROTATE_OUTPUT_INDEX;

//-------------------------------------------步进电机配置----------------------------------------------
//电机索引列表
typedef enum RAIL_ONLINE_TUBE_ROTATE_SM_INDEX
{
    RAIL_ONLINE_TUBE_ROTATE_SM_INDEX_MAIN = 0,//旋转电机
}RAIL_ONLINE_TUBE_ROTATE_SM_INDEX;

//旋转节点电机
#define TUBE_ROTATE_SM_MAIN                     STEP_MOTOR5

//旋转节点电机调试速度曲线
#define S_CURVE_TUBE_ROTATE_DEBUG               SM_CURVE_0 //旋转电机调试速度
//旋转节点电机运行速度曲线
#define S_CURVE_TUBE_ROTATE_MAIN_NORMAL         SM_CURVE_1 //旋转电机转动一格速度曲线
//旋转节点电机找原点速度曲线
#define S_CURVE_TUBE_ROTATE_MAIN_FIND_ORIGIN    SM_CURVE_2 //旋转找原点速度曲线



#endif

