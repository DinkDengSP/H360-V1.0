/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:10:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-11-19 11:24:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_DATA_TYPE_H_
#define __APP_STATE_DATA_TYPE_H_
#include "AppStateMacro.h"


/*------------------------------------------各个模块信号集合-------------------------------------*/
//本机轨道进样仓传感器状态集合
typedef struct RACK_STORE_IN_SENSOR_SET
{
    SENSOR_STATE storeInplaceSensor;//进样末端试管架检测光电
}RACK_STORE_IN_SENSOR_SET;

//出样仓传感器状态集合
typedef struct RACK_STORE_OUT_SENSOR_SET
{
    SENSOR_STATE closeToKeyTaryCloseToKeySensor;//靠近按键托盘靠近轨道光电传感器
    SENSOR_STATE closeToKeyTaryCloseToRailSensor;//靠近按键托盘靠近按键光电传感器
    SENSOR_STATE closeToRailTaryCloseToKeySensor;//靠近轨道托盘靠近按键光电传感器
    SENSOR_STATE closeToRailTaryCloseToRailSensor;//靠近轨道托盘靠近轨道光电传感器
}RACK_STORE_OUT_SENSOR_SET;

//进样换轨传感器集合
typedef struct RACK_IN_SWITCH_SENSOR_SET
{
    SENSOR_STATE rackComeInState;//样本架进入换轨传感器
    SENSOR_STATE rackMoveOutState;//样本架离开换轨传感器
}RACK_IN_SWITCH_SENSOR_SET;

//传感器状态集合
typedef struct RACK_ENTRY_SENSOR_SET
{
    SENSOR_STATE rackExistSensor;//检测试管架是否存在光电
}RACK_ENTRY_SENSOR_SET;

//输入信号集合
typedef struct RACK_BAR_SCAN_SENSOR_SET
{
    SENSOR_STATE rackExistSensor;//试管架是否存在检测
}RACK_BAR_SCAN_SENSOR_SET;

//输入信号集合
typedef struct RACK_TEST_SENSOR_SET
{
    SENSOR_STATE rackExistSensor;//试管架是否存在检测
    SENSOR_STATE rackFullSensor;//测试区试管架满检测
    SENSOR_STATE encoderSensor;//码盘信号检测
}RACK_TEST_SENSOR_SET;

//输入讯号集合
typedef struct RACK_OUT_SWITCH_SENSOR_SET
{
    SENSOR_STATE rackComeInState;//样本架进入换轨传感器
    SENSOR_STATE rackMoveOutState;//样本架离开换轨传感器
}RACK_OUT_SWITCH_SENSOR_SET;

//输入传感器集合
typedef struct WAIT_OUT1_SENSOR_SET
{
    SENSOR_STATE rackExistSensor;//试管架是否存在检测
}WAIT_OUT1_SENSOR_SET;

//输入状态集合
typedef struct WAIT_OUT2_SENSOR_SET
{
    SENSOR_STATE rackExistSensor;//试管架是否存在检测
}WAIT_OUT2_SENSOR_SET;

/*-------------------------------------------各模块状态集合---------------------------------------------*/
//状态集合-进样仓
typedef struct APP_STATE_RACK_STORE_IN
{
    //传感器状态集合
    RACK_STORE_IN_SENSOR_SET rackStoreInSensorSet;
}APP_STATE_RACK_STORE_IN;
#define LENGTH_APP_STATE_RACK_STORE_IN                      (sizeof(APP_STATE_RACK_STORE_IN)/sizeof(uint8_t))

//状态集合-出样仓
typedef struct APP_STATE_RACK_STORE_OUT
{
    //传感器状态集合
    RACK_STORE_OUT_SENSOR_SET rackStoreOutSensorSet;
    //当前推进次数
    uint8_t currentPushCount;
    //推手旋转状态
    PUSH_ROTATE_STATE rotateState;
}APP_STATE_RACK_STORE_OUT;
#define LENGTH_APP_STATE_RACK_STORE_OUT                     (sizeof(APP_STATE_RACK_STORE_OUT)/sizeof(uint8_t))

//状态集合-轨道进样节点
typedef struct APP_STATE_RACK_ENTRY
{
    //传感器状态集合
    RACK_ENTRY_SENSOR_SET rackEntrySensorSet;
    //发生试管架进入事件标志
    EVENT_FLAG rackComeInEventFlag;
    //当前节点状态
    RAIL_NODE_STATE nodeState;
}APP_STATE_RACK_ENTRY;
#define LENGTH_APP_STATE_RACK_ENTRY                         (sizeof(APP_STATE_RACK_ENTRY)/sizeof(uint8_t))

//状态集合-轨道进样扫码节点
typedef struct APP_STATE_RACK_BAR_SCAN
{
    //传感器状态集合
    RACK_BAR_SCAN_SENSOR_SET rackBarScanSensorSet;
    //发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //节点状态
    RAIL_NODE_STATE nodeState;
}APP_STATE_RACK_BAR_SCAN;
#define LENGTH_APP_STATE_RACK_BAR_SCAN                      (sizeof(APP_STATE_RACK_BAR_SCAN)/sizeof(uint8_t))

//状态集合-轨道进样测试节点
typedef struct APP_STATE_RACK_TEST
{
    //传感器状态集合
    RACK_TEST_SENSOR_SET rackTestSensorSet;
    //是否发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //当前正在测试的试管号
    TEST_TUBE_NO currentTubeNo;
}APP_STATE_RACK_TEST;
#define LENGTH_APP_STATE_RACK_TEST                          (sizeof(APP_STATE_RACK_TEST)/sizeof(uint8_t))

//状态集合-轨道进样换轨节点
typedef struct APP_STATE_RACK_IN_SWITCH
{
    //换轨传感器集合
    RACK_IN_SWITCH_SENSOR_SET rackInSwitchSensorSet;
    //是否发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //换轨当前状态
    RAIL_SWITCH_STATE railSwitchState;
}APP_STATE_RACK_IN_SWITCH;
#define LENGTH_APP_STATE_RACK_IN_SWITCH                     (sizeof(APP_STATE_RACK_IN_SWITCH)/sizeof(uint8_t))

//状态集合-轨道出样等待节点1
typedef struct APP_STATE_RACK_WAIT_OUT1
{
    //传感器状态集合
    WAIT_OUT1_SENSOR_SET rackWaitOut1SensorSet;
    //是否发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //节点当前状态
    RAIL_NODE_STATE nodeState;
}APP_STATE_RACK_WAIT_OUT1;
#define LENGTH_APP_STATE_RACK_WAIT_OUT1                     (sizeof(APP_STATE_RACK_WAIT_OUT1)/sizeof(uint8_t))

//状态集合-轨道出样等待节点2
typedef struct APP_STATE_RACK_WAIT_OUT2
{
    //传感器状态集合
    WAIT_OUT2_SENSOR_SET rackWaitOut2SensorSet;
    //是否发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //节点当前状态
    RAIL_NODE_STATE nodeState;
}APP_STATE_RACK_WAIT_OUT2;
#define LENGTH_APP_STATE_RACK_WAIT_OUT2                     (sizeof(APP_STATE_RACK_WAIT_OUT2)/sizeof(uint8_t))

//状态集合-轨道出样换轨节点
typedef struct APP_STATE_RACK_OUT_SWITCH
{
    //传感器状态集合
    RACK_OUT_SWITCH_SENSOR_SET rackOutSwitchSensorSet;
    //是否发生试管架进入事件
    EVENT_FLAG rackComeInEventFlag;
    //换轨状态
    RAIL_SWITCH_STATE railSwitchState;
}APP_STATE_RACK_OUT_SWITCH;
#define LENGTH_APP_STATE_RACK_OUT_SWITCH                    (sizeof(APP_STATE_RACK_OUT_SWITCH)/sizeof(uint8_t))

//状态集合-系统全部
typedef struct APP_STATE
{
    //进样仓状态
    APP_STATE_RACK_STORE_IN  stateRackStoreIn;
    //出样仓状态
    APP_STATE_RACK_STORE_OUT stateRackStoreOut;
    //入口节点状态
    APP_STATE_RACK_ENTRY     stateRackEntry;
    //扫码节点状态
    APP_STATE_RACK_BAR_SCAN  stateRackBarScan;
    //测试节点状态
    APP_STATE_RACK_TEST      stateRackTest;
    //进样换轨节点状态
    APP_STATE_RACK_IN_SWITCH stateRackInSwitch;
    //出样等待1状态
    APP_STATE_RACK_WAIT_OUT1 stateRackWaitOut1;
    //出样等待2状态
    APP_STATE_RACK_WAIT_OUT2 stateRackWaitOut2;
    //出样换轨状态
    APP_STATE_RACK_OUT_SWITCH stateRackOutSwitch;
}APP_STATE;
#define LENGTH_APP_STATE                                    (sizeof(APP_STATE)/sizeof(uint8_t))


#endif




