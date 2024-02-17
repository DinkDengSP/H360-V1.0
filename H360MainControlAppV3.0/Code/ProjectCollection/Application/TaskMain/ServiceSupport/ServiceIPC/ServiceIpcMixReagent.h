/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 18:33:19
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcMixReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_MIX_REAGENT_H_
#define __SERVICE_IPC_MIX_REAGENT_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceMixReagentDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceMixReagentGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceMixReagentWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//试剂混匀复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixReagentResetAllWhileAck(void);
//试剂混匀集成混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixReagentRotateIntegrateWhileAck(MIX_MODE mode);




/**********************************************************************阻塞API*************************************************************************/
//试剂混匀复位,等待指令完成,堵塞
LH_ERR IPC_ServiceMixReagentResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//试剂混匀集成混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceMixReagentRotateIntegrateWhileReturn(MIX_MODE mode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



#endif



