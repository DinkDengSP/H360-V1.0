/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:35:46
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-24 16:51:41
 *FilePath: \App\TaskMain\ServiceSupport\ServiceIpc\ServiceIpcBarScan.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_BAR_SCAN_H_
#define __SERVICE_IPC_BAR_SCAN_H_
#include "ServiceIpcBase.h"

//初始化IPC调用数据结构
void IPC_BarScanDataInit(void);

//任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_BarScanGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//等待执行完成,阻塞
LH_ERR IPC_BarScanWaitLastReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

/****************************************************发出指令不等待完成*******************************************************/
//检测条码枪是否存在,不等待返回
LH_ERR IPC_RailBarScanCheckExistWhileAck(void);
//打开条码枪,不等待返回
LH_ERR IPC_RailBarScanOpenWhileAck(BARSCAN_OPEN_MODE barScanOpenMode);
//关闭条码枪,不等待返回
LH_ERR IPC_RailBarScanCloseWhileAck(void);
//读取条码枪扫码结果,不等待返回
LH_ERR IPC_RailBarScanReadScanResultWhileAck(BARSCAN_INDEX barScanIndex);

/****************************************************发出指令并等待完成********************************************************/
//检测条码枪是否存在,等待返回
LH_ERR IPC_RailBarScanCheckExistWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//打开条码枪,等待返回
LH_ERR IPC_RailBarScanOpenWhileReturn(BARSCAN_OPEN_MODE barScanOpenMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//关闭条码枪,等待返回
LH_ERR IPC_RailBarScanCloseWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//读取条码枪扫码结果,等待返回
LH_ERR IPC_RailBarScanReadScanResultWhileReturn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);
//读取条码枪扫码结果,等待返回
LH_ERR IPC_RailBarScanReadScanResultFromBufferWhileReturn(BARSCAN_INDEX barScanIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif

