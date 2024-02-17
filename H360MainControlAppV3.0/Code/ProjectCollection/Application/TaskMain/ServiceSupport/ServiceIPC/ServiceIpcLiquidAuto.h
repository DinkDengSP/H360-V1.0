/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:30
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-15 15:59:08
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcLiquidAuto.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_LIQUID_AUTO_H_
#define __SERVICE_IPC_LIQUID_AUTO_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceLiquidAutoDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceLiquidAutoGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceLiquidAutoWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//使能自动抽废液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnableWasteDischargeAutoWhileAck(void);
//使能自动三针清洗液稀释,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnableThreeNeedleCleanDiluentAutoWhileAck(void);
//使能自动存水补充,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnablePureWaterSupplementAutoWhileAck(void);
//禁止自动抽废液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisableWasteDischargeAutoWhileAck(void);
//禁止自动三针清洗液稀释,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisableThreeNeedleCleanDiluentAutoWhileAck(void);
//禁止纯水自动补充,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoDisablePureWaterSupplementAutoWhileAck(void);
//液路自动化强制三针清洗液稀释一次,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnforceThreeNeedleCleanDiluentOnceWhileAck(void);
//液路自动化强制补充存水一次,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoEnforcePureWaterSupplementOnceWhileAck(void);
//设定三针稀释浓缩液瓶号,不等待指令完成,非堵塞
LH_ERR IPC_ServiceLiquidAutoSetThreeNeedleAutoDiluentConcentrateBottleWhileAck(CONCENTRATE_BOTTLE_SELECT concentrateBottle);



/**********************************************************************阻塞API*************************************************************************/
//使能自动抽废液,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnableWasteDischargeAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//使能自动三针清洗液稀释,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnableThreeNeedleCleanDiluentAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//使能自动存水补充,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnablePureWaterSupplementAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//禁止自动抽废液,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisableWasteDischargeAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//禁止自动三针清洗液稀释,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisableThreeNeedleCleanDiluentAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//禁止纯水自动补充,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoDisablePureWaterSupplementAutoWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//液路自动化强制三针清洗液稀释一次,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnforceThreeNeedleCleanDiluentOnceWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//液路自动化强制补充存水一次,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoEnforcePureWaterSupplementOnceWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//设定三针稀释浓缩液瓶号,等待指令完成,堵塞
LH_ERR IPC_ServiceLiquidAutoSetThreeNeedleAutoDiluentConcentrateBottleWhileReturn(CONCENTRATE_BOTTLE_SELECT concentrateBottle,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
#endif



