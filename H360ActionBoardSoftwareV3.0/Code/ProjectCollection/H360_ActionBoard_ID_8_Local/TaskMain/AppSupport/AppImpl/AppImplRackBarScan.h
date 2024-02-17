/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-08 16:14:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-01 09:26:17
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RACK_BAR_SCAN_H_
#define __APP_IMPL_RACK_BAR_SCAN_H_
#include "AppImplRackBarScanConfig.h"

#define SINGLE_BAR_CODE_LENGTH_MAX          32
//集成扫码结果
typedef struct RACK_BAR_SCAN_RESULT
{
    uint16_t rackID;
    uint8_t tube1BarCode[SINGLE_BAR_CODE_LENGTH_MAX];
    uint8_t tube2BarCode[SINGLE_BAR_CODE_LENGTH_MAX];
    uint8_t tube3BarCode[SINGLE_BAR_CODE_LENGTH_MAX];
    uint8_t tube4BarCode[SINGLE_BAR_CODE_LENGTH_MAX];
    uint8_t tube5BarCode[SINGLE_BAR_CODE_LENGTH_MAX];
}RACK_BAR_SCAN_RESULT;

//初始化功能状态机
void AppImplRackBarScanFSM_Init(void);

//运行功能状态机
void AppImplRackBarScanFSM_Run(void);

//读取指定输入状态
LH_ERR AppImplRackBarScanReadInput(RACK_BAR_SCAN_INPUT inputIndex,SENSOR_STATE* sensorStatePtr);

//写入指定输出
LH_ERR AppImplRackBarScanWriteOutput(RACK_BAR_SCAN_OUTPUT outputIndex,OUT_STATE setState);

//指定步进电机复位
LH_ERR AppImplRackBarScanStepMotorReset(RACK_BAR_SCAN_SM smIndex,int32_t* currentPos);

//指定步进电机走位
LH_ERR AppImplRackBarScanStepMotorRunSteps(RACK_BAR_SCAN_SM smIndex,int32_t runSteps,int32_t* currentPos);

//模块复位
LH_ERR AppImplRackBarScanModuleReset(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//节点卡住试管架
LH_ERR AppImplRackBarScanSwitchStateBlock(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//节点释放试管架
LH_ERR AppImplRackBarScanSwitchStateRelease(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//节点放行
LH_ERR AppImplRackBarScanReleaseRack(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//单次打开条码枪
LH_ERR AppImplRackBarScanOpenOnce(uint8_t openTimeSecond,uint16_t* barcodeLength,uint8_t** barcodeBuffer,APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//持续打开条码枪
LH_ERR AppImplRackBarScanOpenAlways(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//关闭条码枪
LH_ERR AppImplRackBarScanClose(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//集成扫码
LH_ERR AppImplRackBarScanIntegrate(RACK_BAR_SCAN_RESULT* barCodeScanResultPtr,APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//获取节点全部讯息
LH_ERR AppImplRackBarScanReadState(APP_STATE_RACK_BAR_SCAN** rackBarScanStatePtrPtr);

//清除节点缓存讯息
LH_ERR AppImplRackBarScanClearState(void);


#endif
