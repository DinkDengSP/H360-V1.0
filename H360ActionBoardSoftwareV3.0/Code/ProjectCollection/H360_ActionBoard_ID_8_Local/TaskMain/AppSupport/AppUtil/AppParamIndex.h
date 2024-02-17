/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:09:55
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-24 15:15:22
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_INDEX_H_
#define __APP_PARAM_INDEX_H_
#include "BoardInc.h"

//app参数主序号枚举
typedef enum APP_PARAM_MAIN
{
    APP_PARAM_MAIN_RACK_STORE_IN                            = 0X00,//参数类型-进样仓
    APP_PARAM_MAIN_RACK_STORE_OUT                           = 0X01,//参数类型-出样仓
    APP_PARAM_MAIN_RACK_ENTRY                               = 0X02,//参数类型-轨道进样节点
    APP_PARAM_MAIN_RACK_BAR_SCAN                            = 0X03,//参数类型-轨道进样扫码节点
    APP_PARAM_MAIN_RACK_TEST                                = 0X04,//参数类型-轨道进样测试节点
    APP_PARAM_MAIN_RACK_IN_SWITCH                           = 0X05,//参数类型-轨道进样换轨节点
    APP_PARAM_MAIN_RACK_WAIT_OUT1                           = 0X06,//参数类型-轨道出样等待节点1
    APP_PARAM_MAIN_RACK_WAIT_OUT2                           = 0X07,//参数类型-轨道出样等待节点2
    APP_PARAM_MAIN_RACK_OUT_SWITCH                          = 0X08,//参数类型-轨道出样换轨节点
}APP_PARAM_MAIN;

//子参数-进样仓
typedef enum APP_PARAM_SUB_RACK_STORE_IN
{
    APP_PARAM_SUB_RACK_STORE_IN_PUSH_MOTOR_RESET_CORRECT    = 0X01,//推进电机复位修正
    APP_PARAM_SUB_RACK_STORE_IN_TO_END                      = 0X02,//推进电机推进架子走一次的步数
    APP_PARAM_SUB_RACK_STORE_IN_STEP_COMPENSATE_TO_ENTRY    = 0X03,//进样进入入口的补偿
}APP_PARAM_SUB_RACK_STORE_IN;

//子参数-出样仓
typedef enum APP_PARAM_SUB_RACK_STORE_OUT
{
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT       = 0X01,//出样前后电机复位修正
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_START_POS                 = 0X02,//出样前后电机工作起点
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP                 = 0X03,//出样前后电机推进一次的步数
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_KEY   = 0X04,//出样前后电机推到靠近按键托盘末端最大推进次数
    APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_ACTION_POS              = 0X05,//出样旋转电机工作位置坐标
    APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS                = 0X06,//出样旋转电机空闲位置坐标
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_RAIL  = 0X07,//出样前后电机推到靠近轨道托盘末端最大推进次数
    APP_PARAM_SUB_RACK_STORE_OUT_PUSH_STEP_SWITCH_TO_STORE      = 0X08,//将试管架从出样换轨出口推到出样仓入口的步数
}APP_PARAM_SUB_RACK_STORE_OUT;

//子参数-轨道进样节点
typedef enum APP_PARAM_SUB_RACK_ENTRY
{
    APP_PARAM_SUB_RACK_ENTRY_ACTION_POS                     = 0X01,//防堵电机顶住前端的工作坐标
    APP_PARAM_SUB_RACK_ENTRY_IDLE_POS                       = 0X02,//防堵电机释放的工作坐标
    APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE             = 0X03,//轨道传送带电机类型
}APP_PARAM_SUB_RACK_ENTRY;

//子参数-轨道进样扫码节点
typedef enum APP_PARAM_SUB_RACK_BAR_SCAN
{
    APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_ACTION_POS      = 0X01,//扫码旋转电机挡住试管架的坐标
    APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_IDLE_POS        = 0X02,//扫码旋转电机释放试管架的坐标
    APP_PARAM_SUB_RACK_BAR_SCAN_PUSH_MOTOR_START_POS        = 0X03,//扫码电机扫码起点坐标
}APP_PARAM_SUB_RACK_BAR_SCAN;

//子参数-轨道进样测试节点
typedef enum APP_PARAM_SUB_RACK_TEST
{
    APP_PARAM_SUB_RACK_TEST_RESET_CORRECT_AND_TUBE5_POS     = 0X01,//测试换管电机复位修正与试管5位置
}APP_PARAM_SUB_RACK_TEST;

//子参数-轨道进样换轨节点
typedef enum APP_PARAM_SUB_RACK_IN_SWITCH
{
    APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS                = 0X01,//进样换轨电机换轨初始位置(试管架进入位置)
    APP_PARAM_SUB_RACK_IN_SWITCH_MOVE_OUT_POS               = 0X02,//进样换轨电机换轨目标位置(试管架离开位置)
}APP_PARAM_SUB_RACK_IN_SWITCH;

//子参数-轨道出样等待节点1
typedef enum APP_PARAM_SUB_RACK_WAIT_OUT1
{
    APP_PARAM_SUB_RACK_WAIT_OUT1_ACTION_POS                 = 0X01,//旋转电机挡住试管架位置
    APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS                   = 0X02,//旋转电机释放试管架位置
}APP_PARAM_SUB_RACK_WAIT_OUT1;

//子参数-轨道出样等待节点2
typedef enum APP_PARAM_SUB_RACK_WAIT_OUT2
{
    APP_PARAM_SUB_RACK_WAIT_OUT2_ACTION_POS                 = 0X01,//旋转电机挡住试管架位置
    APP_PARAM_SUB_RACK_WAIT_OUT2_IDLE_POS                   = 0X02,//旋转电机释放试管架位置
}APP_PARAM_SUB_RACK_WAIT_OUT2;

//子参数-轨道出样换轨节点
typedef enum APP_PARAM_SUB_RACK_OUT_SWITCH
{
    APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS               = 0X01,//进样换轨电机换轨初始位置(试管架进入位置)
    APP_PARAM_SUB_RACK_OUT_SWITCH_MOVE_OUT_POS              = 0X02,//进样换轨电机换轨目标位置(试管架离开位置)
}APP_PARAM_SUB_RACK_OUT_SWITCH;



#endif
