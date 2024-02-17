/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 15:37:32
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 11:12:24
 *FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceState.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_STATE_H_
#define __SERVICE_STATE_H_
#include "ServiceStateDefault.h"

//服务状态初始化
void ServiceStateInit(void);

/***************************************PushIn1**************************************/
//推进一次试管架是否存在状态
void ServiceStatePushIn1WriteExist(SENSOR_STATE_PUSHIN_EXIST value);
SENSOR_STATE_PUSHIN_EXIST ServiceStatePushIn1ReadExist(void);
//推进一次试管架是否到位状态
void ServiceStatePushIn1WriteInPlace(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn1ReadInPlace(void);
//进样托盘在位状态
void ServiceStatePushIn1WriteTrayExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn1ReadTrayExist(void);
//进样门开关状态
void ServiceStatePushIn1WriteDoorClose(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn1ReadDoorClose(void);
//进样1模块状态
void ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE value);
PUSH_IN_STATE ServiceStatePushIn1ReadCurrentModuleState(void);
//进样1上一次模块状态
void ServiceStatePushIn1WriteLastModuleState(PUSH_IN_STATE value);
PUSH_IN_STATE ServiceStatePushIn1ReadLastModuleState(void);

//进样1按钮状态
void ServiceStatePushIn1WritePushButtonState(RAIL_SENSOR_STATE buttonState);
RAIL_SENSOR_STATE ServiceStatePushIn1ReadPushButtonState(void);
/***************************************PushIn2**************************************/
//推进一次试管架是否存在状态
void ServiceStatePushIn2WriteExist(SENSOR_STATE_PUSHIN_EXIST value);
SENSOR_STATE_PUSHIN_EXIST ServiceStatePushIn2ReadExist(void);
//推进一次试管架是否到位状态
void ServiceStatePushIn2WriteInPlace(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn2ReadInPlace(void);
//进样托盘在位状态
void ServiceStatePushIn2WriteTrayExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn2ReadTrayExist(void);
//进样门开关状态
void ServiceStatePushIn2WriteDoorClose(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushIn2ReadDoorClose(void);
//进样2模块状态
void ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE value);
PUSH_IN_STATE ServiceStatePushIn2ReadCurrentModuleState(void);
//进样2上一次模块状态
void ServiceStatePushIn2WriteLastModuleState(PUSH_IN_STATE value);
PUSH_IN_STATE ServiceStatePushIn2ReadLastModuleState(void);
//进样2按钮状态
void ServiceStatePushIn2WritePushButtonState(RAIL_SENSOR_STATE buttonState);
RAIL_SENSOR_STATE ServiceStatePushIn2ReadPushButtonState(void);
/***************************************PushOut1**************************************/
//出样1存在状态
void ServiceStatePushOut1WriteRackExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut1ReadRackExist(void);
//出样1满状态
void ServiceStatePushOut1WriteRackFull(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut1ReadRackFull(void);
//出样1托盘状态
void ServiceStatePushOut1WriteTrayExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut1ReadTrayExist(void);
//出样1门状态
void ServiceStatePushOut1WriteDoorClose(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut1ReadDoorClose(void);
//出样1当前模块状态
void ServiceStatePushOut1WriteCurrentModuleState(PUSH_OUT_STATE value);
PUSH_OUT_STATE ServiceStatePushOut1ReadCurrentModuleState(void);
//出样1上一次模块状态
void ServiceStatePushOut1WriteLastModuleState(PUSH_OUT_STATE value);
PUSH_OUT_STATE ServiceStatePushOut1ReadLastModuleState(void);

/***************************************PushOut2**************************************/
//出样2存在状态
void ServiceStatePushOut2WriteRackExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut2ReadRackExist(void);
//出样2满状态
void ServiceStatePushOut2WriteRackFull(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut2ReadRackFull(void);
//出样2托盘状态
void ServiceStatePushOut2WriteTrayExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut2ReadTrayExist(void);
//出样2门状态
void ServiceStatePushOut2WriteDoorClose(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStatePushOut2ReadDoorClose(void);
//出样2当前模块状态
void ServiceStatePushOut2WriteCurrentModuleState(PUSH_OUT_STATE value);
PUSH_OUT_STATE ServiceStatePushOut2ReadCurrentModuleState(void);
//出样2上一次模块状态
void ServiceStatePushOut2WriteLastModuleState(PUSH_OUT_STATE value);
PUSH_OUT_STATE ServiceStatePushOut2ReadLastModuleState(void);

/***************************************RailHand**************************************/
//样本架机械手当前方向
void ServiceStateRailHandWriteCurrentDir(HAND_DIR value);
HAND_DIR ServiceStateRailHandReadCurrentDir(void);
//机械手上样本架是否存在
void ServiceStateRailHandWriteHandRackExist(SENSOR_STATE_HAND_RACK_EXIST value);
SENSOR_STATE_HAND_RACK_EXIST ServiceStateRailHandReadHandRackExist(void);
//机械手模块状态
void ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE value);
RAIL_HAND_STATE ServiceStateRailHandReadHandModuleState(void);
//机械手爪手状态
void ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE value);
RAIL_HAND_CLAW_STATE ServiceStateRailHandReadClawState(void);
/***************************************RailTrans**************************************/
//轨道传送带主区域
void ServiceStateRailTransWriteMainPos(RAIL_MODULE_POS value);
RAIL_MODULE_POS ServiceStateRailTransReadMainPos(void);
//轨道传送带子区域
void ServiceStateRailTransWriteSubPos(RAIL_SUB_MODULE value);
RAIL_SUB_MODULE ServiceStateRailTransReadSubPos(void);
//急诊仓位有无试管架
void ServiceStateRailTransWriteEmergencyCallRackExist(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStateRailTransReadEmergencyCallRackExist(void);
//急诊仓位们关好
void ServiceStateRailTransWriteEmergencyCallDoorClose(RAIL_SENSOR_STATE value);
RAIL_SENSOR_STATE ServiceStateRailTransReadEmergencyCallDoorClose(void);
//轨道模块状态
void ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE value);
RAIL_TRANS_STATE ServiceStateRailTransReadModuleState(void);


/***************************************RailBarScan**************************************/
//条码枪数据长度写入与长度获取
void ServiceStateRailBarScanWriteData(uint16_t barcodeLength,uint8_t* barcodeBufferPtr);
uint16_t ServiceStateRailBarScanReadDataLength(void);
void ServiceStateRailBarScanReadData(uint16_t readDataLength,uint8_t* recvDataPtr);
//条码枪缓存数据清空
void ServiceStateRailBarScanClearLastResult(void);

/* 清空集成扫码数据 */
void ServiceStateClearRailBarScanIntegrateResult();
/* 设置集成扫码数据 */
void ServiceStateSetRailBarScanIntegrateResult(uint16_t rackID,uint8_t tubeBarExist,uint8_t *barScanReault);
/* 读取集成扫码数据 */
SERVICE_STATE_BARSCAN_RESULT_INTEGRATE* ServiceStateReadRailBarScanIntegrateResult();


/***************************************RailComplex**************************************/
//轨道复合模块数量
void ServiceStateRailComplexWriteModuleCount(uint8_t value);
uint8_t ServiceStateRailComplexReadModuleCount(void);


/***************************************RailUtil**************************************/
//指定板卡的版本号
void ServiceStateUtilWriteBoardVersion(uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion);
void ServiceStateUtilReadBoardVersion(uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);

#endif




