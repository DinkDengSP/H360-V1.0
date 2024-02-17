/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:25:35
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 18:37:15
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcPlateReaction.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_PLATE_REACTION_H_
#define __SERVICE_IPC_PLATE_REACTION_H_
#include "ServiceIpcBase.h"

//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServicePlateReactionDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServicePlateReactionGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServicePlateReactionWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



/**********************************************************************非阻塞API*************************************************************************/
//反应盘复位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReactionResetAllWhileAck(void);
//反应盘正向走指定数量个杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReactionRunAnyCupsPositiveWhileAck(uint8_t cupsCount);
//反应盘正向走到指定杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReactionRun2IndexPositiveWhileAck(uint8_t cupIndex);
//反应盘反向走指定数量个杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReactionRunAnyCupsNegativeWhileAck(uint8_t cupsCount);
//反应盘反向走到指定杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateReactionRun2IndexNegativeWhileAck(uint8_t cupIndex);



/**********************************************************************阻塞API*************************************************************************/
//反应盘复位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReactionResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//反应盘正向走指定数量个杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReactionRunAnyCupsPositiveWhileReturn(uint8_t cupsCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//反应盘正向走到指定杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReactionRun2IndexPositiveWhileReturn(uint8_t cupIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//反应盘反向走指定数量个杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReactionRunAnyCupsNegativeWhileReturn(uint8_t cupsCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//反应盘反向走到指定杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateReactionRun2IndexNegativeWhileReturn(uint8_t cupIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);





#endif



