/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:09:40
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-24 15:11:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppParamDefault.h"

//默认参数配置--进样仓
const APP_PARAM_RACK_STORE_IN paramRackStoreInDefault = {
    0,  //推进电机复位修正
    0,  //推进电机推架子到末端最大步数
    0,  //进样进入入口的补偿
    {0},//保留参数
};

//默认参数配置--出样仓
const APP_PARAM_RACK_STORE_OUT paramRackStoreOutDefault = {
    0,  //出样前后电机复位修正
    0,  //出样前后电机工作起点
    0,  //出样前后电机推进一次的步数
    20,  //出样前后电机推到靠近按键托盘末端最大推进次数
    0,  //出样旋转电机工作位置坐标
    0,  //出样旋转电机空闲位置坐标
    10, //出样前后电机推到靠近轨道托盘末端最大推进次数
    200,//将试管架从换轨出口推到出样仓入口的步数
    {0},//保留参数
};

//默认参数配置--轨道进样节点
const APP_PARAM_RACK_ENTRY paramRackEntryDefault = {
    0,  //防堵电机顶住前端的工作坐标
    0,  //防堵电机释放的工作坐标
    0,//轨道传送带电机类型
    {0},//保留参数
};

//默认参数配置--轨道进样扫码节点
const APP_PARAM_RACK_BAR_SCAN paramRackBarScanDefault = {
    0,  //扫码限位电机挡住试管架的坐标
    0,  //扫码限位电机释放试管架的坐标
    0,  //扫码移动电机扫码起点坐标
    {0},//保留参数
};

//默认参数配置--轨道进样测试节点
const APP_PARAM_RACK_TEST paramRackTestDefault = {
    0,  //测试换管电机复位修正
    {0},//保留参数
};

//默认参数配置--轨道进样换轨节点
const APP_PARAM_RACK_IN_SWITCH paramRackInSwitchDefault = {
    0,  //进样换轨电机换轨初始位置(试管架进入位置)
    0,  //进样换轨电机换轨目标位置(试管架离开位置)
    {0},//保留参数
};

//默认参数配置--轨道出样等待节点1
const APP_PARAM_RACK_WAIT_OUT1 paramRackWaitOut1Default = {
    0,  //限位电机挡住试管架位置
    0,  //限位电机释放试管架位置
    {0},//保留参数
};

//默认参数配置--轨道出样等待节点2
const APP_PARAM_RACK_WAIT_OUT2 paramRackWaitOut2Default = {
    0,  //限位电机挡住试管架位置
    0,  //限位电机释放试管架位置
    {0},//保留参数
};

//默认参数配置--轨道出样换轨节点
const APP_PARAM_RACK_OUT_SWITCH paramRackOutSwitchDefault = {
    0,  //出样换轨电机换轨初始位置(试管架进入位置)
    0,  //出样换轨电机换轨目标位置(试管架离开位置)
    {0},//保留参数
};
