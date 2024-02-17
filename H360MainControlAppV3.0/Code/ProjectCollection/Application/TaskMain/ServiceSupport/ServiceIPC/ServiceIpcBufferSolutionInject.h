/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:03
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 17:47:42
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcBufferSolutionInject.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_BUFFER_SOLUTION_INJECT_H_
#define __SERVICE_IPC_BUFFER_SOLUTION_INJECT_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceBufferSolutionInjectDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceBufferSolutionInjectGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceBufferSolutionInjectWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//缓冲液注液复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceBufferSolutionInjectResetWhileAck(void);
//缓冲液注液,不等待指令完成,非堵塞
LH_ERR IPC_ServiceBufferSolutionInjectTrigWhileAck(BOTTLE_SELECT bottleSelect);
//缓冲液灌注,不等待指令完成,非堵塞
LH_ERR IPC_ServiceBufferSolutionInjectPrimeWhileAck(BOTTLE_SELECT bottleSelect);


/**********************************************************************阻塞API*************************************************************************/
//缓冲液注液复位,等待指令完成,堵塞
LH_ERR IPC_ServiceBufferSolutionInjectResetWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//缓冲液注液,等待指令完成,堵塞
LH_ERR IPC_ServiceBufferSolutionInjectTrigWhileReturn(BOTTLE_SELECT bottleSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//缓冲液灌注,等待指令完成,堵塞
LH_ERR IPC_ServiceBufferSolutionInjectPrimeWhileReturn(BOTTLE_SELECT bottleSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



#endif



