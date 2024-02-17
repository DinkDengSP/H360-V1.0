/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:25:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 14:59:11
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_NEEDLE_REAGENT_H_
#define __SERVICE_IPC_NEEDLE_REAGENT_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceNeedleReagentDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceNeedleReagentGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceNeedleReagentWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//试剂针复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentResetAllWhileAck(void);
//试剂针旋转到指定位置,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentRotatePosWhileAck(NEEDLE_REAGENT_POS targetPos);
//试剂针清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentCleanWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//试剂针灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentPrimeWhileAck(NEEDLE_PRIME_OPT primeOpt);
//试剂针吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentAbsorbWhileAck(uint16_t absorbCountMinUL);
//试剂针注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentInjectWhileAck(float cupLiquidUl);
//试剂针注液带混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentInjectMixWhileAck(float cupLiquidUl,uint8_t mixCount);
//试剂针清洗维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentMainTainWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//试剂针探液校准,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentAdjustWhileAck(CMD_SOURCE_TEST_FLOW cmdSource);
//试剂针试剂余量探测,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleReagentLiquidResidueCheckWhileAck(void);




/**********************************************************************阻塞API*************************************************************************/
//试剂针复位,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针旋转到指定位置,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentRotatePosWhileReturn(NEEDLE_REAGENT_POS targetPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentCleanWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentPrimeWhileReturn(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentAbsorbWhileReturn(uint16_t absorbCountMinUL,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针注液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentInjectWhileReturn(float cupLiquidUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针注液带混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentInjectMixWhileReturn(float cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针清洗维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentMainTainWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针探液校准,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentAdjustWhileReturn(CMD_SOURCE_TEST_FLOW cmdSource,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂针试剂余量探测,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleReagentLiquidResidueCheckWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);




#endif



