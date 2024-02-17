/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:55:11
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 14:42:55
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_3\TaskMain\AppSupport\AppUtil\AppStateType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_TYPE_H_
#define __APP_STATE_TYPE_H_
#include "AppStateMacro.h"

/*-----------------------------------------传感器集合-------------------------------------------*/
typedef struct TUBE_ENTRY_SENSOR_SET
{
    SENSOR_STATE intranceState;//入口传感器状态
}TUBE_ENTRY_SENSOR_SET;

typedef struct TUBE_ROTATE_SENSOR_SET
{
    SENSOR_STATE inputState;//入口位置状态
    SENSOR_STATE barScanState;//扫码位置状态
    SENSOR_STATE testState;//测试位置状态
    SENSOR_STATE outputState;//出口位置状态
}TUBE_ROTATE_SENSOR_SET;

//传感器状态集合
typedef struct TUBE_EXIT_SENSOR_SET
{
    SENSOR_STATE tubeExitState;//出口传感器状态
}TUBE_EXIT_SENSOR_SET;

/*-----------------------------------------状态集合----------------------------------------------*/
//状态集合-入口节点
typedef struct APP_STATE_TUBE_ENTRY
{
    TUBE_ENTRY_SENSOR_SET tubeEntrySensorSet;
    EVENT_FLAG tubeEntryEventFlag; //进管事件
}APP_STATE_TUBE_ENTRY;
#define LENGTH_APP_STATE_TUBE_ENTRY           (sizeof(APP_STATE_TUBE_ENTRY)/sizeof(uint8_t))

//状态集合-转盘节点
typedef struct APP_STATE_TUBE_ROTATE
{
    TUBE_ROTATE_SENSOR_SET tubeRotateSensorSet;
    uint32_t reserve;
}APP_STATE_TUBE_ROTATE;
#define LENGTH_APP_STATE_TUBE_ROTATE          (sizeof(APP_STATE_TUBE_ROTATE)/sizeof(uint8_t))

//状态集合-扫码节点
typedef struct APP_STATE_TUBE_BAR_SCAN
{
    uint32_t reserve;
}APP_STATE_TUBE_BAR_SCAN;
#define LENGTH_APP_STATE_TUBE_BAR_SCAN        (sizeof(APP_STATE_TUBE_BAR_SCAN)/sizeof(uint8_t))

//状态集合-测试试管夹紧
typedef struct APP_STATE_TUBE_CLAMP
{
    TUBE_CLAMP_STATE tubeClampState; //试管加紧状态
}APP_STATE_TUBE_CLAMP;
#define LENGTH_APP_STATE_TUBE_CLAMP        (sizeof(APP_STATE_TUBE_CLAMP)/sizeof(uint8_t))

//状态集合-输出节点
typedef struct APP_STATE_TUBE_EXIT
{
    TUBE_EXIT_SENSOR_SET tubeExtiSensorSet;
    EVENT_FLAG tubExitEventFlag; //出管事件;
}APP_STATE_TUBE_EXIT;
#define LENGTH_APP_STATE_TUBE_EXIT          (sizeof(APP_STATE_TUBE_EXIT)/sizeof(uint8_t))

//状态集合-系统状态
typedef struct APP_STATE
{
    APP_STATE_TUBE_ENTRY stateTubeEntry;
    APP_STATE_TUBE_ROTATE stateTubeRotate;
    APP_STATE_TUBE_BAR_SCAN stateTubeBarScan;
    APP_STATE_TUBE_CLAMP stateTubeClamp;
    APP_STATE_TUBE_EXIT stateTubeExit; 
}APP_STATE;
#define LENGTH_APP_STATE                        (sizeof(APP_STATE)/sizeof(uint8_t))




#endif

