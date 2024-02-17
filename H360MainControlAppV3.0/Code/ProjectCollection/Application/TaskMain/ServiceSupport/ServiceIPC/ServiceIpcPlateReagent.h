/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:33:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 10:21:21
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcPlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_PLATE_REAGENT_H_
#define __SERVICE_IPC_PLATE_REAGENT_H_
#include "ServiceIpcBase.h"

//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServicePlateReagentDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServicePlateReagentGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServicePlateReagentWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



/**********************************************************************非阻塞API*************************************************************************/
//试剂盘复位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentResetAllWhileAck(void);
//试剂盘走一定数量杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentRunAnyCupsWhileAck(uint8_t cupsCount);
//试剂盘走到指定序号杯位带偏移,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentRun2IndexWithOffsetWhileAck(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset);
//试剂盘走到指定序号杯位的扫码位并扫码,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentRun2IndexScanAndQRScanWhileAck(uint8_t targetIndex);
//试剂盘当前位置扫码,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentBarScanOnceWhileAck(void);
//试剂盘清除指定序号杯位试剂的高度信息,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentClearLiquidDetectDataWhileAck(uint8_t targetIndex,uint8_t clearConfig);
//试剂盘设置状态空闲,可以相应按键,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReagentSetIdleWhileAck(void);


/**********************************************************************阻塞API*************************************************************************/
//试剂盘复位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘走一定数量杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentRunAnyCupsWhileReturn(uint8_t cupsCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘走到指定序号杯位带偏移,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentRun2IndexWithOffsetWhileReturn(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘走到指定序号杯位的扫码位并扫码,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentRun2IndexScanAndQRScanWhileReturn(uint8_t targetIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘当前位置扫码,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentBarScanOnceWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘清除指定序号杯位试剂的高度信息,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentClearLiquidDetectDataWhileReturn(uint8_t targetIndex,uint8_t clearConfig,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂盘设置状态空闲,可以相应按键,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReagentSetIdleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

#endif



