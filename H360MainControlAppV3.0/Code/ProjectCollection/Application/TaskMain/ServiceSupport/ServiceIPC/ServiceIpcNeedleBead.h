/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:25:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 15:00:02
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_NEEDLE_BEAD_H_
#define __SERVICE_IPC_NEEDLE_BEAD_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceNeedleBeadDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceNeedleBeadGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceNeedleBeadWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//磁珠针复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadResetAllWhileAck(void);
//磁珠针旋转到指定位置,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadRotatePosWhileAck(NEEDLE_BEAD_POS targetPos);
//磁珠针清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//磁珠针灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT primeOpt);
//磁珠针吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadAbsorbWhileAck(uint16_t absorbCountMinUL);
//磁珠针注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadInjectWhileAck(float cupLiquidUl);
//磁珠针注液带混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadInjectMixWhileAck(float cupLiquidUl,uint8_t mixCount);
//磁珠针清洗维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadMainTainWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//磁珠针探液校准,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadAdjustWhileAck(CMD_SOURCE_TEST_FLOW cmdSource);
//磁珠针试剂余量检查,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleBeadLiquidResidueCheckWhileAck(void);







/**********************************************************************阻塞API*************************************************************************/
//磁珠针复位,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针旋转到指定位置,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadRotatePosWhileReturn(NEEDLE_BEAD_POS targetPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadAbsorbWhileReturn(uint16_t absorbCountMinUL,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针注液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadInjectWhileReturn(float cupLiquidUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针注液带混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadInjectMixWhileReturn(float cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针清洗维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadMainTainWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针探液校准,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadAdjustWhileReturn(CMD_SOURCE_TEST_FLOW cmdSource,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠针试剂余量检查,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleBeadLiquidResidueCheckWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);




#endif



