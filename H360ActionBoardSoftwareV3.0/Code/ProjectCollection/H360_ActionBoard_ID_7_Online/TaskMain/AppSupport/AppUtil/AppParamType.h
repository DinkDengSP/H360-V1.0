/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-20 17:28:00
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParamType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_TYPE_H_
#define __APP_PARAM_TYPE_H_
#include "BoardInc.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA

//入口节点
typedef struct APP_PARAM_TUBE_ENTRY
{
    int32_t railConveyorBeltMotorType;
    int32_t reserveArray[100];
}APP_PARAM_TUBE_ENTRY;
#define LENGTH_APP_PARAM_TUBE_ENTRY         (sizeof(APP_PARAM_TUBE_ENTRY)/sizeof(uint8_t))

//转盘节点
typedef struct APP_PARAM_TUBE_ROTATE
{
    int32_t reserve;
    int32_t rotateStepMotorCorrectStep; //旋转电机复位修正
    int32_t reserveArray[99];
}APP_PARAM_TUBE_ROTATE;
#define LENGTH_APP_PARAM_TUBE_ROTATE         (sizeof(APP_PARAM_TUBE_ROTATE)/sizeof(uint8_t))

//扫码节点
typedef struct APP_PARAM_TUBE_BAR_SCAN
{
    int32_t reserve;
    int32_t reserveArray[100];
}APP_PARAM_TUBE_BAR_SCAN;
#define LENGTH_APP_PARAM_TUBE_BAR_SCAN        (sizeof(APP_PARAM_TUBE_BAR_SCAN)/sizeof(uint8_t))

//测试试管夹紧
typedef struct APP_PARAM_TUBE_CLAMP
{
    int32_t reserve;
    int32_t updownStepMotorCorrectStep;//升降电机复位修正
    int32_t updownStepMotorClampTubeDownPos;//升降电机固定试管下降坐标
    int32_t reserveArray[98];
}APP_PARAM_TUBE_CLAMP;
#define LENGTH_APP_PARAM_TUBE_CLAMP        (sizeof(APP_PARAM_TUBE_CLAMP)/sizeof(uint8_t))

//输出节点
typedef struct APP_PARAM_TUBE_EXIT
{
    int32_t reserve;
    int32_t reserveArray[100];
}APP_PARAM_TUBE_EXIT;
#define LENGTH_APP_PARAM_TUBE_EXIT          (sizeof(APP_PARAM_TUBE_EXIT)/sizeof(uint8_t))



//系统参数所有线程的集合
typedef struct APP_PARAM
{
    uint32_t paramFlag;
    APP_PARAM_TUBE_ENTRY paramTubeEntry;
    APP_PARAM_TUBE_ROTATE paramTubeRotate;
    APP_PARAM_TUBE_BAR_SCAN paramTubeBarScan;
    APP_PARAM_TUBE_CLAMP paramTubeClamp;
    APP_PARAM_TUBE_EXIT paramTubeExit;
}APP_PARAM;
//系统参数长度
#define LENGTH_APP_PARAM    (sizeof(APP_PARAM)/sizeof(uint8_t))

#endif




