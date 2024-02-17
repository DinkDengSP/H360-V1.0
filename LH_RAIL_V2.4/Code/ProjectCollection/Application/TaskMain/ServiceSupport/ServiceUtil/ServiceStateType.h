/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 15:38:33
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 11:09:19
 *FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceStateType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_STATE_TYPE_H_
#define __SERVICE_STATE_TYPE_H_
#include "ServiceStateTypeEnum.h"
#include "Can2SubRailBase.h"
#include "ServiceImplBaseConfig.h"
#include "BoardInc.h"

//轨道进样服务状态
typedef struct SERVICE_STATE_PUSH_IN
{
    //推进一次试管架存在状态
    SENSOR_STATE_PUSHIN_EXIST rackExistState;
    //试管架到位传感器状态
    RAIL_SENSOR_STATE rackInPlaceState;
    //托盘到位状态
    RAIL_SENSOR_STATE trayState;
    //门开关状态
    RAIL_SENSOR_STATE doorState;
    //当前进样模块状态
    PUSH_IN_STATE currentModuleState;
    //上一次进样模块状态
    PUSH_IN_STATE lastModuleState;
    //进样按钮状态
    RAIL_SENSOR_STATE pushInButtonState;
}SERVICE_STATE_PUSH_IN;
//数据结构长度
#define SERVICE_STATE_PUSH_IN_LENGTH    (sizeof(SERVICE_STATE_PUSH_IN)/sizeof(uint8_t))

//轨道出样服务状态
typedef struct SERVICE_STATE_PUSH_OUT
{
    //出样存在状态
    RAIL_SENSOR_STATE rackExistState;
    //出样满状态
    RAIL_SENSOR_STATE rackFullState;
    //出样托盘状态
    RAIL_SENSOR_STATE trayState;
    //出样门状态
    RAIL_SENSOR_STATE doorState;
    //当前出样模块状态
    PUSH_OUT_STATE currentModuleState;
    //上一次出样模块状态
    PUSH_OUT_STATE lastModuleState;
}SERVICE_STATE_PUSH_OUT;
//数据结构长度
#define SERVICE_STATE_PUSH_OUT_LENGTH    (sizeof(SERVICE_STATE_PUSH_OUT)/sizeof(uint8_t))

//轨道机械手服务状态
typedef struct SERVICE_STATE_RAIL_HAND
{
    //当前机械手方向
    HAND_DIR currentDir;
    //机械手上是否有试管架的传感器状态
    SENSOR_STATE_HAND_RACK_EXIST handHadRackExist;
    //模块状态
    RAIL_HAND_STATE moduleState;
    //机械手夹爪位置
    RAIL_HAND_CLAW_STATE handClawState;
}SERVICE_STATE_RAIL_HAND;
//数据结构长度
#define SERVICE_STATE_RAIL_HAND_LENGTH    (sizeof(SERVICE_STATE_RAIL_HAND)/sizeof(uint8_t))

//轨道传送带服务状态
typedef struct SERVICE_STATE_RAIL_TRANS
{
    //当前轨道主区域状态
    RAIL_MODULE_POS currentMainPos;
    //当前轨道的子区域状态
    RAIL_SUB_MODULE currentSubPos;
    //急诊位试管架存在传感器状态
    RAIL_SENSOR_STATE emergencyCallRackExistState;
    //急诊位试管架到位传感器状态
    RAIL_SENSOR_STATE emergencyCallRackInPlaceState;
    //模块状态
    RAIL_TRANS_STATE moduleState;
}SERVICE_STATE_RAIL_TRANS;
//数据结构长度
#define SERVICE_STATE_RAIL_TRANS_LENGTH    (sizeof(SERVICE_STATE_RAIL_TRANS)/sizeof(uint8_t))

//轨道条码扫描服务状态
typedef struct SERVICE_STATE_RAIL_BARSCAN
{
    //条码数据缓冲区
    uint8_t barScanResultDataBuffer[BAR_SCAN_MAX_DATA_LENGTH];
    //条码长度
    uint16_t barScanResultLength;
}SERVICE_STATE_RAIL_BARSCAN;
//数据结构长度
#define SERVICE_STATE_RAIL_BARSCAN_LENGTH    (sizeof(SERVICE_STATE_RAIL_BARSCAN)/sizeof(uint8_t))

//轨道复合模块服务状态
typedef struct SERVICE_STATE_RAIL_COMPLEX
{
    //轨道联机模块数量
    uint8_t railModuleCount;
}SERVICE_STATE_RAIL_COMPLEX;
//数据结构长度
#define SERVICE_STATE_RAIL_COMPLEX_LENGTH    (sizeof(SERVICE_STATE_RAIL_COMPLEX)/sizeof(uint8_t))


//轨道其余的辅助模块服务状态
typedef struct SERVICE_STATE_RAIL_UTIL
{
    //板卡版本号
    uint8_t boardMainVersion;
    uint8_t boardSubVersion;
    uint8_t boardDebugVersion;
    uint8_t boardSvnVersion;
}SERVICE_STATE_RAIL_UTIL;
//数据结构长度
#define SERVICE_STATE_RAIL_UTIL_LENGTH    (sizeof(SERVICE_STATE_RAIL_UTIL)/sizeof(uint8_t))

/* 存放IPC集成扫码结果 */
typedef struct SERVICE_STATE_BARSCAN_RESULT_INTEGRATE
{
    uint16_t rackID;
    uint8_t tubeBarExistFlag;
    uint8_t barCodeTube1[32];
    uint8_t barCodeTube2[32];
    uint8_t barCodeTube3[32];
    uint8_t barCodeTube4[32];
    uint8_t barCodeTube5[32];
}SERVICE_STATE_BARSCAN_RESULT_INTEGRATE;
//数据结构长度
#define SERVICE_STATE_BARSCAN_RESULT_INTEGRATE_LENGTH    (sizeof(SERVICE_STATE_BARSCAN_RESULT_INTEGRATE)/sizeof(uint8_t))

//轨道整体服务状态
typedef struct SERVICE_STATE
{
    SERVICE_STATE_PUSH_IN pushIn1State;//进样1状态
    SERVICE_STATE_PUSH_IN pushIn2State;//进样2状态
    SERVICE_STATE_PUSH_OUT pushOut1State;//出样1状态
    SERVICE_STATE_PUSH_OUT pushOut2State;//出样2状态
    SERVICE_STATE_RAIL_HAND railHandState;//轨道机械手状态
    SERVICE_STATE_RAIL_TRANS railTransState;//轨道传送带状态
    SERVICE_STATE_RAIL_BARSCAN raikBarScanState;//轨道条码扫描状态
    SERVICE_STATE_RAIL_COMPLEX railComplexState;//轨道复合模块状态
    SERVICE_STATE_RAIL_UTIL railUtilState;//轨道其余辅助支援模块状态
    SERVICE_STATE_BARSCAN_RESULT_INTEGRATE barScanResultIntegrate; //存放IPC集成扫码结果
}SERVICE_STATE;
//数据结构长度
#define SERVICE_STATE_LENGTH    (sizeof(SERVICE_STATE)/sizeof(uint8_t))






#endif




