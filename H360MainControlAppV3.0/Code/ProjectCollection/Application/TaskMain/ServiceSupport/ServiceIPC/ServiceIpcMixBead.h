/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 18:37:39
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcMixBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_MIX_BEAD_H_
#define __SERVICE_IPC_MIX_BEAD_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceMixBeadDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceMixBeadGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceMixBeadWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

/**********************************************************************非阻塞API*************************************************************************/
//磁珠摇匀复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixBeadResetAllWhileAck(void);
//磁珠摇匀集成摇匀,不等待指令完成,非堵塞
LH_ERR IPC_ServiceMixBeadRotateIntegrateWhileAck(MIX_MODE mode);



/**********************************************************************阻塞API*************************************************************************/
//磁珠摇匀复位,等待指令完成,堵塞
LH_ERR IPC_ServiceMixBeadResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//磁珠摇匀集成摇匀,等待指令完成,堵塞
LH_ERR IPC_ServiceMixBeadRotateIntegrateWhileReturn(MIX_MODE mode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


#endif



