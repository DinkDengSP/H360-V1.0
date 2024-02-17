/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-11-16 09:42:11
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParamIndex.h
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
typedef enum APP_PARAM_MAIN_INDEX
{
    APP_PARAM_MAIN_TUBE_ENTRY    = 0X00,//进管节点
    APP_PARAM_MAIN_TUBE_ROTATE   = 0X01,//转盘节点
    APP_PARAM_MAIN_TUBE_BAR_SCAN = 0X02,//扫码节点
    APP_PARAM_MAIN_TUBE_CLAMP    = 0X03,//测试位夹手
    APP_PARAM_MAIN_TUBE_EXIT     = 0X04,//出管节点
}APP_PARAM_MAIN_INDEX;

//子参数序号-进管节点
typedef enum APP_PARAM_SUB_TUBE_ENTRY_INDEX
{
    APP_PARAM_SUB_TUBE_ENTRY_INDEX_BELT_TYPE    = 0X00,//轨道传送带电机类型                              
}APP_PARAM_SUB_TUBE_ENTRY_INDEX;

//子参数序号-转盘节点
typedef enum APP_PARAM_SUB_TUBE_ROTATE_INDEX
{
    APP_PARAM_SUB_TUBE_ROTATE_INDEX_RESERVE                        = 0X00,//保留            
    APP_PARAM_SUB_TUBE_ROTATE_INDEX_STEP_MOTOR_RESET_CORRECT_STEP  = 0X01,//旋转电机复位修正
}APP_PARAM_SUB_TUBE_ROTATE_INDEX;

//子参数序号-扫码节点
typedef enum APP_PARAM_SUB_TUBE_BAR_SCAN_INDEX
{   
    APP_PARAM_SUB_TUBE_BAR_SCAN_INDEX_RESERVE                             = 0X00,//保留
}APP_PARAM_SUB_TUBE_BAR_SCAN_INDEX;

//子参数序号-测试位夹手
typedef enum APP_PARAM_SUB_TUBE_CLAMP_INDEX
{
    APP_PARAM_SUB_TUBE_CLAMP_INDEX_RESERVE                                 = 0X00,//保留
    APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP    = 0X01,//升降电机复位修正
    APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_DOWN_POS              = 0X02,//升降电机固定试管坐标
}APP_PARAM_SUB_TUBE_CLAMP_INDEX;

//子参数序号-出管节点
typedef enum APP_PARAM_SUB_TUBE_EXIT_INDEX
{
    APP_PARAM_SUB_TUBE_EXIT_INDEX_RESERVE                                  = 0X00,//保留
}APP_PARAM_SUB_TUBE_EXIT_INDEX;


#endif





