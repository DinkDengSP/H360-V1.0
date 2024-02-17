/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 13:25:11
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\AppLogicSupport\AppIpc\AppIpcBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "AppImplBase.h"
#include "ServiceIpcBase.h"

//IPC调用结构体
typedef struct DATA_IPC_PUSH_IN1
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //进样状态
    SENSOR_STATE_PUSHIN_EXIST pushInState;
}DATA_IPC_PUSH_IN1;

//IPC调用结构体
typedef struct DATA_IPC_PUSH_IN2
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //进样状态
    SENSOR_STATE_PUSHIN_EXIST pushInState;
}DATA_IPC_PUSH_IN2;

//IPC调用结构体
typedef struct DATA_IPC_PUSH_OUT1
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //出样当前的序号
    uint8_t currentRackIndex;
}DATA_IPC_PUSH_OUT1;

//IPC调用结构体
typedef struct DATA_IPC_PUSH_OUT2
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //出样当前的序号
    uint8_t currentRackIndex;
}DATA_IPC_PUSH_OUT2;

//IPC调用结构体
typedef struct DATA_IPC_RAIL_TRANS
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //主轨道模块位置
    RAIL_MODULE_POS mainRailModuleNo;
    //实际当前轨道位置
    RAIL_SUB_MODULE subRailRegion;
}DATA_IPC_RAIL_TRANS;

//IPC调用结构体
typedef struct DATA_IPC_RAIL_HAND
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //机械手上是否有试管架
    SENSOR_STATE_HAND_RACK_EXIST handRackExistState;
    //机械手当前方向
    HAND_DIR handCurrentDir;
}DATA_IPC_RAIL_HAND;

//IPC调用结构体
typedef struct DATA_IPC_RAIL_COMPLEX
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //当前模块数量
    uint8_t currentModuleCount;
    //进样1和2的状态
    SENSOR_STATE_PUSHIN_EXIST pushIn1State;
    SENSOR_STATE_PUSHIN_EXIST pushIn2State;
}DATA_IPC_RAIL_COMPLEX;

//IPC调用结构体
typedef struct DATA_IPC_APP_PARAM
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    //读取到的参数值
    int32_t paramRead;
}DATA_IPC_APP_PARAM;

//条码扫描的长度限制
#define DATA_IPC_RAIL_COMPLEX_BARCODE_LENGTH_LIMIT   400
//IPC调用结构体
typedef struct DATA_IPC_BAR_SCAN
{
    //指令的状态
    STATE_IPC state; 
    //指令结果
    LH_ERR resultValue;
    //指令数据域操作互斥信号量
    OS_MUTEX* dataMutexPtr;
    uint8_t barcodeDataBufferPtr[DATA_IPC_RAIL_COMPLEX_BARCODE_LENGTH_LIMIT];
    uint16_t barcodeLength;
}DATA_IPC_BAR_SCAN;





#endif





