
/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:25:20
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 14:25:29
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_NEEDLE_SAMPLE_H_
#define __SERVICE_IPC_NEEDLE_SAMPLE_H_
#include "ServiceIpcBase.h"



//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceNeedleSampleDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceNeedleSampleGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceNeedleSampleWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



/**********************************************************************非阻塞API*************************************************************************/
//样本针复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleResetAllWhileAck(void);
//样本针旋转到指定位置,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleRotatePosWhileAck(NEEDLE_SAMPLE_POS targetPos);
//样本针清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleCleanWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//样本针强力清洗,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleForceCleanWhileAck(uint8_t cleanCount);
//样本针灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSamplePrimeWhileAck(NEEDLE_PRIME_OPT primeOpt);
//样本针强力灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleForcePrimeWhileAck(void);
//样本针吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbRackWhileAck(uint16_t absorbCountMinUL,TUBE_TYPE tubeType,uint8_t newSampleTube);
//样本针反应盘吸液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbPlateReactionWhileAck(uint16_t absorbCountMinUL,float cupLiquidAfterAbsorb);
//样本针注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWhileAck(float cupLiquidUl);
//样本针注液带混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectMixWhileAck(float cupLiquidUl,uint8_t mixCount);
//样本针管路维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleMainTainWhileAck(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime);
//样本针校准,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleAdjustWhileAck(CMD_SOURCE_TEST_FLOW cmdSource);
//样本针携带维护,不等待指令完成,非堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWithMaintainWhileAck(uint8_t maintainCount,uint16_t injectUl);




/**********************************************************************阻塞API*************************************************************************/
//样本针复位,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针旋转到指定位置,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleRotatePosWhileReturn(NEEDLE_SAMPLE_POS targetPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleCleanWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针强力清洗,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleForceCleanWhileReturn(uint8_t cleanCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSamplePrimeWhileReturn(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针强力灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleForcePrimeWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbRackWhileReturn(uint16_t absorbCountMinUL,TUBE_TYPE tubeType,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针反应盘吸液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAbsorbPlateReactionWhileReturn(uint16_t absorbCountMinUL,float cupLiquidAfterAbsorb,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针注液,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWhileReturn(float cupLiquidUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针注液带混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectMixWhileReturn(float cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针管路维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleMainTainWhileReturn(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针校准,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleAdjustWhileReturn(CMD_SOURCE_TEST_FLOW cmdSource,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//样本针携带维护,等待指令完成,堵塞
LH_ERR IPC_ServiceNeedleSampleInjectWithMaintainWhileReturn(uint8_t maintainCount,uint16_t injectUl,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);




#endif



