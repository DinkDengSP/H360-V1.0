/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:59
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 15:35:00
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcMixWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_MIX_WASH_H_
#define __SERVICE_IPC_MIX_WASH_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceMixWashDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceMixWashGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceMixWashWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//清洗混匀复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixWashResetAllWhileAck(void);
//清洗混匀集成混匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixWashRotateIntegrateWhileAck(MIX_MODE mode);



/**********************************************************************阻塞API*************************************************************************/
//清洗混匀复位,等待指令完成,堵塞
LH_ERR IPC_ServiceMixWashResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗混匀集成混匀,等待指令完成,堵塞
LH_ERR IPC_ServiceMixWashRotateIntegrateWhileReturn(MIX_MODE mode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



#endif



