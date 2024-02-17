/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:25:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-14 15:08:27
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplBarScan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BAR_SCAN_H_
#define __SERVICE_IMPL_BAR_SCAN_H_
#include "ServiceImplBase.h"


//自动循环事件初始化
void ServiceImplBarScan_EventInit(void);

//自动循环事件
void ServiceImplBarScan_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//检测条码枪是否存在
LH_ERR ServiceImplRailBarScanCheckExist(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//打开条码枪
LH_ERR ServiceImplRailBarScanOpen(BARSCAN_OPEN_MODE openMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//关闭条码枪
LH_ERR ServiceImplRailBarScanClose(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取条码枪数据
LH_ERR ServiceImplRailBarScanReadData(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取条码枪数据
LH_ERR ServiceImplRailBarScanReadDataFromBuffer(BARSCAN_INDEX barScanIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


