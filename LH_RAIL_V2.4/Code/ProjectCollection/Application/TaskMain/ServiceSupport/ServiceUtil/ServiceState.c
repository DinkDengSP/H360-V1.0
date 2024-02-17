/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 15:37:25
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 11:11:54
**FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceState.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceState.h"

static SERVICE_STATE* serviceStatePtr = NULL;

//服务状态初始化
void ServiceStateInit(void)
{
    //如果已经申请了内存,就释放掉
    if(serviceStatePtr != NULL)
    {
        UserMemFree(MEM_D2_SRAM1,serviceStatePtr);
    }
    serviceStatePtr = NULL;
    //申请内存
    do
    {
        serviceStatePtr = UserMemMalloc(MEM_D2_SRAM1,SERVICE_STATE_LENGTH);
        if(serviceStatePtr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (serviceStatePtr == NULL);

    //初始化,从默认配置中加载参数
    UserMemCopy(&(serviceStatePtr->pushIn1State),(void*)(&pushIn1StateDefault),SERVICE_STATE_PUSH_IN_LENGTH);
    UserMemCopy(&(serviceStatePtr->pushIn2State),(void*)(&pushIn2StateDefault),SERVICE_STATE_PUSH_IN_LENGTH);
    UserMemCopy(&(serviceStatePtr->pushOut1State),(void*)(&pushOut1StateDefault),SERVICE_STATE_PUSH_OUT_LENGTH);
    UserMemCopy(&(serviceStatePtr->pushOut2State),(void*)(&pushOut2StateDefault),SERVICE_STATE_PUSH_OUT_LENGTH);
    UserMemCopy(&(serviceStatePtr->railHandState),(void*)(&railHandStateDefault),SERVICE_STATE_RAIL_HAND_LENGTH);
    UserMemCopy(&(serviceStatePtr->railTransState),(void*)(&railTransStateDefault),SERVICE_STATE_RAIL_TRANS_LENGTH);
    UserMemCopy(&(serviceStatePtr->raikBarScanState),(void*)(&railBarScanStateDefault),SERVICE_STATE_RAIL_BARSCAN_LENGTH);
    UserMemCopy(&(serviceStatePtr->railComplexState),(void*)(&railComplexStateDefault),SERVICE_STATE_RAIL_COMPLEX_LENGTH);
    UserMemCopy(&(serviceStatePtr->railUtilState),(void*)(&railUtilStateDefault),SERVICE_STATE_RAIL_UTIL_LENGTH);
    UserMemCopy(&(serviceStatePtr->barScanResultIntegrate),(void*)(&barScanResultIntegrateDefault),SERVICE_STATE_BARSCAN_RESULT_INTEGRATE_LENGTH);
}

/***************************************PushIn1**************************************/
//推进一次试管架是否存在状态
void ServiceStatePushIn1WriteExist(SENSOR_STATE_PUSHIN_EXIST value)
{
    serviceStatePtr->pushIn1State.rackExistState = value;
}

SENSOR_STATE_PUSHIN_EXIST ServiceStatePushIn1ReadExist(void)
{
    return serviceStatePtr->pushIn1State.rackExistState;
}

//推进一次试管架是否到位状态
void ServiceStatePushIn1WriteInPlace(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn1State.rackInPlaceState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn1ReadInPlace(void)
{
    return serviceStatePtr->pushIn1State.rackInPlaceState;
}

//进样托盘在位状态
void ServiceStatePushIn1WriteTrayExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn1State.trayState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn1ReadTrayExist(void)
{
    return serviceStatePtr->pushIn1State.trayState;
}

//进样门开关状态
void ServiceStatePushIn1WriteDoorClose(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn1State.doorState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn1ReadDoorClose(void)
{
    return serviceStatePtr->pushIn1State.doorState;
}
//进样1当前模块状态
void ServiceStatePushIn1WriteCurrentModuleState(PUSH_IN_STATE value)
{
    serviceStatePtr->pushIn1State.currentModuleState = value;
}

PUSH_IN_STATE ServiceStatePushIn1ReadCurrentModuleState(void)
{
    return serviceStatePtr->pushIn1State.currentModuleState;
}
//进样1上一次模块状态
void ServiceStatePushIn1WriteLastModuleState(PUSH_IN_STATE value)
{
    serviceStatePtr->pushIn1State.lastModuleState = value;
}
PUSH_IN_STATE ServiceStatePushIn1ReadLastModuleState(void)
{
    return serviceStatePtr->pushIn1State.lastModuleState;
}

//进样1按钮状态
void ServiceStatePushIn1WritePushButtonState(RAIL_SENSOR_STATE buttonState)
{
    serviceStatePtr->pushIn1State.pushInButtonState = buttonState;
}
RAIL_SENSOR_STATE ServiceStatePushIn1ReadPushButtonState(void)
{
    return serviceStatePtr->pushIn1State.pushInButtonState;
}


/***************************************PushIn2**************************************/
//推进一次试管架是否存在状态
void ServiceStatePushIn2WriteExist(SENSOR_STATE_PUSHIN_EXIST value)
{
    serviceStatePtr->pushIn2State.rackExistState = value;
}

SENSOR_STATE_PUSHIN_EXIST ServiceStatePushIn2ReadExist(void)
{
    return serviceStatePtr->pushIn2State.rackExistState;
}

//推进一次试管架是否到位状态
void ServiceStatePushIn2WriteInPlace(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn2State.rackInPlaceState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn2ReadInPlace(void)
{
    return serviceStatePtr->pushIn2State.rackInPlaceState;
}

//进样托盘在位状态
void ServiceStatePushIn2WriteTrayExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn2State.trayState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn2ReadTrayExist(void)
{
    return serviceStatePtr->pushIn2State.trayState;
}

//进样门开关状态
void ServiceStatePushIn2WriteDoorClose(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushIn2State.doorState = value;
}

RAIL_SENSOR_STATE ServiceStatePushIn2ReadDoorClose(void)
{
    return serviceStatePtr->pushIn2State.doorState;
}

//进样2当前模块状态
void ServiceStatePushIn2WriteCurrentModuleState(PUSH_IN_STATE value)
{
    serviceStatePtr->pushIn2State.currentModuleState = value;
}
PUSH_IN_STATE ServiceStatePushIn2ReadCurrentModuleState(void)
{
    return serviceStatePtr->pushIn2State.currentModuleState;
}

//进样2上一次模块状态
void ServiceStatePushIn2WriteLastModuleState(PUSH_IN_STATE value)
{
    serviceStatePtr->pushIn2State.lastModuleState = value;
}
PUSH_IN_STATE ServiceStatePushIn2ReadLastModuleState(void)
{
    return serviceStatePtr->pushIn2State.lastModuleState;
}

//进样2按钮状态
void ServiceStatePushIn2WritePushButtonState(RAIL_SENSOR_STATE buttonState)
{
    serviceStatePtr->pushIn2State.pushInButtonState = buttonState;
}
RAIL_SENSOR_STATE ServiceStatePushIn2ReadPushButtonState(void)
{
    return serviceStatePtr->pushIn2State.pushInButtonState;
}


/***************************************PushOut1**************************************/
//出样1存在状态
void ServiceStatePushOut1WriteRackExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut1State.rackExistState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut1ReadRackExist(void)
{
    return serviceStatePtr->pushOut1State.rackExistState;
}

//出样1满状态
void ServiceStatePushOut1WriteRackFull(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut1State.rackFullState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut1ReadRackFull(void)
{
    return serviceStatePtr->pushOut1State.rackFullState;
}

//出样1托盘状态
void ServiceStatePushOut1WriteTrayExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut1State.trayState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut1ReadTrayExist(void)
{
    return serviceStatePtr->pushOut1State.trayState;
}

//出样1门状态
void ServiceStatePushOut1WriteDoorClose(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut1State.doorState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut1ReadDoorClose(void)
{
    return serviceStatePtr->pushOut1State.doorState;
}

//出样1当前模块状态
void ServiceStatePushOut1WriteCurrentModuleState(PUSH_OUT_STATE value)
{
    serviceStatePtr->pushOut1State.currentModuleState = value;
}

PUSH_OUT_STATE ServiceStatePushOut1ReadCurrentModuleState(void)
{
    return serviceStatePtr->pushOut1State.currentModuleState;
}

//出样1上一次模块状态
void ServiceStatePushOut1WriteLastModuleState(PUSH_OUT_STATE value)
{
    serviceStatePtr->pushOut1State.lastModuleState = value;
}
PUSH_OUT_STATE ServiceStatePushOut1ReadLastModuleState(void)
{
    return serviceStatePtr->pushOut1State.lastModuleState;
}


/***************************************PushOut2**************************************/
//出样2存在状态
void ServiceStatePushOut2WriteRackExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut2State.rackExistState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut2ReadRackExist(void)
{
    return serviceStatePtr->pushOut2State.rackExistState;
}

//出样2满状态
void ServiceStatePushOut2WriteRackFull(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut2State.rackFullState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut2ReadRackFull(void)
{
    return serviceStatePtr->pushOut2State.rackFullState;
}

//出样2托盘状态
void ServiceStatePushOut2WriteTrayExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut2State.trayState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut2ReadTrayExist(void)
{
    return serviceStatePtr->pushOut2State.trayState;
}

//出样2门状态
void ServiceStatePushOut2WriteDoorClose(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->pushOut2State.doorState = value;
}

RAIL_SENSOR_STATE ServiceStatePushOut2ReadDoorClose(void)
{
    return serviceStatePtr->pushOut2State.doorState;
}

//出样2当前模块状态
void ServiceStatePushOut2WriteCurrentModuleState(PUSH_OUT_STATE value)
{
    serviceStatePtr->pushOut2State.currentModuleState = value;
}
PUSH_OUT_STATE ServiceStatePushOut2ReadCurrentModuleState(void)
{
    return serviceStatePtr->pushOut2State.currentModuleState;
}

//出样2上一次模块状态
void ServiceStatePushOut2WriteLastModuleState(PUSH_OUT_STATE value)
{
    serviceStatePtr->pushOut2State.lastModuleState = value;
}
PUSH_OUT_STATE ServiceStatePushOut2ReadLastModuleState(void)
{
    return serviceStatePtr->pushOut2State.lastModuleState;
}


/***************************************RailHand**************************************/
//样本架机械手当前方向
void ServiceStateRailHandWriteCurrentDir(HAND_DIR value)
{
    serviceStatePtr->railHandState.currentDir = value;
}

HAND_DIR ServiceStateRailHandReadCurrentDir(void)
{
    return serviceStatePtr->railHandState.currentDir;
}

//机械手上样本架是否存在
void ServiceStateRailHandWriteHandRackExist(SENSOR_STATE_HAND_RACK_EXIST value)
{
    serviceStatePtr->railHandState.handHadRackExist = value;
}

SENSOR_STATE_HAND_RACK_EXIST ServiceStateRailHandReadHandRackExist(void)
{
    return serviceStatePtr->railHandState.handHadRackExist;
}

//机械手模块状态
void ServiceStateRailHandWriteHandModuleState(RAIL_HAND_STATE value)
{
    serviceStatePtr->railHandState.moduleState = value;
}
RAIL_HAND_STATE ServiceStateRailHandReadHandModuleState(void)
{
    return serviceStatePtr->railHandState.moduleState;
}

//机械手爪手状态
void ServiceStateRailHandWriteClawState(RAIL_HAND_CLAW_STATE clawState)
{
    serviceStatePtr->railHandState.handClawState = clawState;
}
RAIL_HAND_CLAW_STATE ServiceStateRailHandReadClawState(void)
{
    return serviceStatePtr->railHandState.handClawState;
}
/***************************************RailTrans**************************************/
//轨道传送带主区域
void ServiceStateRailTransWriteMainPos(RAIL_MODULE_POS value)
{
    serviceStatePtr->railTransState.currentMainPos = value;
}

RAIL_MODULE_POS ServiceStateRailTransReadMainPos(void)
{
    return serviceStatePtr->railTransState.currentMainPos;
}

//轨道传送带子区域
void ServiceStateRailTransWriteSubPos(RAIL_SUB_MODULE value)
{
    serviceStatePtr->railTransState.currentSubPos = value;
}

RAIL_SUB_MODULE ServiceStateRailTransReadSubPos(void)
{
    return serviceStatePtr->railTransState.currentSubPos;
}

//急诊仓位有无试管架
void ServiceStateRailTransWriteEmergencyCallRackExist(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->railTransState.emergencyCallRackExistState = value;
}

RAIL_SENSOR_STATE ServiceStateRailTransReadEmergencyCallRackExist(void)
{
    return serviceStatePtr->railTransState.emergencyCallRackExistState;
}

//急诊仓位们关好
void ServiceStateRailTransWriteEmergencyCallDoorClose(RAIL_SENSOR_STATE value)
{
    serviceStatePtr->railTransState.emergencyCallRackInPlaceState = value;
}

RAIL_SENSOR_STATE ServiceStateRailTransReadEmergencyCallDoorClose(void)
{
    return serviceStatePtr->railTransState.emergencyCallRackInPlaceState;
}

//轨道模块状态
void ServiceStateRailTransWriteModuleState(RAIL_TRANS_STATE value)
{
    serviceStatePtr->railTransState.moduleState = value;
}

RAIL_TRANS_STATE ServiceStateRailTransReadModuleState(void)
{
    return serviceStatePtr->railTransState.moduleState;
}


/***************************************RailBarScan**************************************/
//条码枪数据长度写入与长度获取
void ServiceStateRailBarScanWriteData(uint16_t barcodeLength,uint8_t* barcodeBufferPtr)
{
    serviceStatePtr->raikBarScanState.barScanResultLength = barcodeLength;
    if(barcodeLength == 0)
    {
        //清零
        UserMemSet((void*)(serviceStatePtr->raikBarScanState.barScanResultDataBuffer),0,BAR_SCAN_MAX_DATA_LENGTH);
        return;
    }
    UserMemCopy((void*)(serviceStatePtr->raikBarScanState.barScanResultDataBuffer),(void*)barcodeBufferPtr,barcodeLength);
}

//获取条码长度
uint16_t ServiceStateRailBarScanReadDataLength(void)
{
    return serviceStatePtr->raikBarScanState.barScanResultLength;
}
//从缓存区中读取指定长度数据
void ServiceStateRailBarScanReadData(uint16_t readDataLength,uint8_t* recvDataPtr)
{
    UserMemCopy((void*)recvDataPtr,(void*)(serviceStatePtr->raikBarScanState.barScanResultDataBuffer),readDataLength);
}

//条码枪缓存数据清空
void ServiceStateRailBarScanClearLastResult(void)
{
    UserMemCopy(&(serviceStatePtr->raikBarScanState),(void*)(&railBarScanStateDefault),SERVICE_STATE_RAIL_BARSCAN_LENGTH);
}
/* 清空集成扫码数据 */
void ServiceStateClearRailBarScanIntegrateResult()
{
    uint8_t i = 0;
    serviceStatePtr->barScanResultIntegrate.rackID = 0xffff;
    serviceStatePtr->barScanResultIntegrate.tubeBarExistFlag = 0;
    /* 条码1 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube1[i] = 0;
    }
    /* 条码2 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube2[i] = 0;
    }
    /* 条码3 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube3[i] = 0;
    }
    /* 条码4 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube4[i] = 0;
    }
    /* 条码5 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube5[i] = 0;
    }
}
/* 设置集成扫码数据 */
void ServiceStateSetRailBarScanIntegrateResult(uint16_t rackID,uint8_t tubeBarExist,uint8_t *barScanReault)
{
    uint8_t i = 0;
    serviceStatePtr->barScanResultIntegrate.rackID = rackID;
    serviceStatePtr->barScanResultIntegrate.tubeBarExistFlag = tubeBarExist;
    /* 条码1 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube1[i] = barScanReault[i];
    }
    /* 条码2 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube2[i] = barScanReault[32+i];
    }
    /* 条码3 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube3[i] = barScanReault[64+i];
    }
    /* 条码4 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube4[i] = barScanReault[96+i];
    }
    /* 条码5 */
    for(i=0;i<32;i++)
    {
        serviceStatePtr->barScanResultIntegrate.barCodeTube5[i] = barScanReault[128+i];
    }
}

/* 读取集成扫码数据 */
SERVICE_STATE_BARSCAN_RESULT_INTEGRATE* ServiceStateReadRailBarScanIntegrateResult()
{
    return &serviceStatePtr->barScanResultIntegrate;
}


/***************************************RailComplex**************************************/
//轨道复合模块数量
void ServiceStateRailComplexWriteModuleCount(uint8_t value)
{
    serviceStatePtr->railComplexState.railModuleCount = value;
}

uint8_t ServiceStateRailComplexReadModuleCount(void)
{
    return serviceStatePtr->railComplexState.railModuleCount;
}


/***************************************RailUtil**************************************/
//指定板卡的版本号
void ServiceStateUtilWriteBoardVersion(uint8_t mainVersion,uint8_t subVersion,uint8_t debugVersion,uint32_t svnVersion)
{
    serviceStatePtr->railUtilState.boardMainVersion = mainVersion;
    serviceStatePtr->railUtilState.boardSubVersion = subVersion;
    serviceStatePtr->railUtilState.boardDebugVersion = debugVersion;
    serviceStatePtr->railUtilState.boardSvnVersion = svnVersion;
}

void ServiceStateUtilReadBoardVersion(uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion)
{
    *mainVersion = serviceStatePtr->railUtilState.boardMainVersion;
    *subVersion = serviceStatePtr->railUtilState.boardSubVersion;
    *debugVersion = serviceStatePtr->railUtilState.boardDebugVersion;
    *svnVersion = serviceStatePtr->railUtilState.boardSvnVersion;
}
