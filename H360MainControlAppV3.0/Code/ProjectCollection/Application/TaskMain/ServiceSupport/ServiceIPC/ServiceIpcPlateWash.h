/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:33:48
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 18:35:03
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcPlateWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_PLATE_WASH_H_
#define __SERVICE_IPC_PLATE_WASH_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServicePlateWashDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServicePlateWashGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServicePlateWashWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//清洗盘复位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateWashResetAllWhileAck(void);
//清洗盘移动指定数量个杯位,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateWashMoveAnyCupsWhileAck(uint8_t cups);
//清洗盘移动到指定序号的位置,不等待指令完成,非堵塞
LH_ERR IPC_ServicePlateWashMove2IndexWhileAck(uint8_t targetCupIndex);


/**********************************************************************阻塞API*************************************************************************/
//清洗盘复位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateWashResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗盘移动指定数量个杯位,等待指令完成,堵塞
LH_ERR IPC_ServicePlateWashMoveAnyCupsWhileReturn(uint8_t cups,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗盘移动到指定序号的位置,等待指令完成,堵塞
LH_ERR IPC_ServicePlateWashMove2IndexWhileReturn(uint8_t targetCupIndex,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



#endif



