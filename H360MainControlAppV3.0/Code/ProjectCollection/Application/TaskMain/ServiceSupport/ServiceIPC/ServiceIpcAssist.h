/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:23:54
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-05 14:26:40
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcAssist.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_ASSIST_H_
#define __SERVICE_IPC_ASSIST_H_
#include "ServiceIpcBase.h"

//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceAssistDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceAssistGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceAssistWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//读取软件版本,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistReadSoftwareVersionWhileAck(VERSION_BOARD_ID boardID);
//读取系统状态,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistReadSystemStateWhileAck(uint8_t mainIndex,uint8_t subIndex);
//握手,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistHandShakeWhileAck(MODE_RUNNING modeRun);
//系统紧急急停,不等待指令完成,非堵塞
LH_ERR IPC_ServiceAssistSystemEmergencyStopWhileAck(void);
//设置下位机模块运行版本
LH_ERR IPC_ServiceAssistSetActionBoardModelVersionWhileAck(void);



/**********************************************************************阻塞API*************************************************************************/
//读取软件版本,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadSoftwareVersionWhileReturn(VERSION_BOARD_ID boardID,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读取系统状态,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadSystemStateWhileReturn(uint8_t mainIndex,uint8_t subIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//握手,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistHandShakeWhileReturn(MODE_RUNNING modeRun,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//系统紧急急停,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistSystemEmergencyStopWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读动作参数,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistReadActionParamWhileReturn(uint8_t mainIndex,uint8_t subIndex,int32_t *liquidDetectPos,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//设置下位机模块运行版本,等待指令完成,堵塞
LH_ERR IPC_ServiceAssistSetActionBoardModelVersionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



#endif



