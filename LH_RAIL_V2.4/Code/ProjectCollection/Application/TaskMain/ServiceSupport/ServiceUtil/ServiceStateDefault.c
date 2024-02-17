/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:00:36
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-28 13:36:43
**FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceStateDefault.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceStateDefault.h"

//进样1默认状态
const SERVICE_STATE_PUSH_IN pushIn1StateDefault = {
    SENSOR_STATE_PUSHIN_EXIST_NONE,//推进一次试管架存在状态
    RAIL_SENSOR_INVALID,//试管架到位传感器状态
    RAIL_SENSOR_INVALID,//托盘到位状态
    RAIL_SENSOR_INVALID,//门开关状态
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//当前模块状态
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//上一次模块状态
    RAIL_SENSOR_INVALID,//进样1按钮状态无效
};

//进样2默认状态
const SERVICE_STATE_PUSH_IN pushIn2StateDefault = {
    SENSOR_STATE_PUSHIN_EXIST_NONE,//推进一次试管架存在状态
    RAIL_SENSOR_INVALID,//试管架到位传感器状态
    RAIL_SENSOR_INVALID,//托盘到位状态
    RAIL_SENSOR_INVALID,//门开关状态
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//当前模块状态
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//上一次模块状态
    RAIL_SENSOR_INVALID,//进样2按钮状态无效
};

//出样1默认状态
const SERVICE_STATE_PUSH_OUT pushOut1StateDefault = {
    RAIL_SENSOR_INVALID,//出样存在状态
    RAIL_SENSOR_INVALID,//出样满状态
    RAIL_SENSOR_INVALID,//出样托盘状态
    RAIL_SENSOR_INVALID,//出样门状态
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//当前出样模块状态
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//上一次出样模块状态
};

//出样2默认状态
const SERVICE_STATE_PUSH_OUT pushOut2StateDefault = {
    RAIL_SENSOR_INVALID,//出样存在状态
    RAIL_SENSOR_INVALID,//出样满状态
    RAIL_SENSOR_INVALID,//出样托盘状态
    RAIL_SENSOR_INVALID,//出样门状态
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//当前出样模块状态
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST,//上一次出样模块状态
};

//轨道机械手默认状态
const SERVICE_STATE_RAIL_HAND railHandStateDefault = {
    HAND_DIR_NONE,//当前机械手方向
    SENSOR_STATE_HAND_RACK_EXIST_NONE,//机械手上是否有试管架的传感器状态
    RAIL_HAND_STATE_IDLE,//模块状态
    RAIL_HAND_CLAW_STATE_BACK,//机械手夹爪收回
};

//轨道传送带默认状态
const SERVICE_STATE_RAIL_TRANS railTransStateDefault = {
    RAIL_MODULE_POS_COMMON,//当前轨道主区域状态
    RAIL_SUB_MODULE_COMMON_UNKNOW,//当前轨道的子区域状态
    RAIL_SENSOR_INVALID,//急诊位试管架存在传感器状态
    RAIL_SENSOR_INVALID,//急诊位试管架到位传感器状态
    RAIL_TRANS_STATE_IDLE,//模块状态
};

//轨道条码枪默认数据
const SERVICE_STATE_RAIL_BARSCAN railBarScanStateDefault = {
    {0},//条码数据缓冲区
    0,//条码长度
};

//轨道复合状态默认状态
const SERVICE_STATE_RAIL_COMPLEX railComplexStateDefault = {
    4,//轨道联机模块数量
};

//轨道辅助功能默认状态
const SERVICE_STATE_RAIL_UTIL railUtilStateDefault = {
    0,//板卡版本号
};
//集成扫码默认结果
const SERVICE_STATE_BARSCAN_RESULT_INTEGRATE barScanResultIntegrateDefault = {
    0,
    0,
    {0},
    {0},
    {0},
    {0},
    {0},
};
