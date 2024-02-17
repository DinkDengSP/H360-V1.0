/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 10:52:20
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 11:12:12
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_TUBE_BAR_SCAN_H_
#define __APP_IMPL_TUBE_BAR_SCAN_H_
#include "AppImplTubeBarScanConfig.h"

//初始化条码状态机
void AppImplTubeBarScanFSM_Init(void);

//运行条码扫描状态机
void AppImplTubeBarScanFSM_Run(void);

//电机复位
LH_ERR AppImplTubeBarScanStepMotorReset(RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX motorIndex,int32_t *posAfterResetPtr);

//电机走步数
LH_ERR AppImplTubeBarScanStepMotorRunSteps(RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr);

//条码扫描复位
LH_ERR AppImplTubeBarScanReset(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr);

//执行一次扫码,并返回结果
LH_ERR AppImplTubeBarScanTrigOnce(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr,uint16_t* barcodeLengthPtr,uint8_t** barcodeBufferPtrPtr);

//单次打开条码枪
LH_ERR AppImplTubeBarScanScannerOpenOnce(uint8_t openTimeSecond,APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr,uint16_t* barcodeLengthPtr,uint8_t** barcodeBufferPtrPtr);

//持续打开条码枪
LH_ERR AppImplTubeBarScanScannerOpenAlways(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr);

//关闭条码枪
LH_ERR AppImplTubeBarScanScannerClose(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr);

//读缓存状态
LH_ERR AppImplTubeBarScanReadState(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr);

//清除缓存状态
LH_ERR AppImplTubeBarScanClearState(void);


#endif





