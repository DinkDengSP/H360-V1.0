/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-09 18:33:03
 *FilePath: \DeviceMainApplication\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcHandWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_HAND_WASH_H_
#define __SERVICE_IPC_HAND_WASH_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceHandWashDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceHandWashGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceHandWashWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//清洗机械手复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashResetAllWhileAck(void);
//清洗机械手旋转到清洗盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotatePlateWashWhileAck(void);
//清洗机械手旋转到反应盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotatePlateReactionWhileAck(void);
//清洗机械手旋转到测量模块,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashRotateMeasureModuleWhileAck(void);
//清洗机械手从反应盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchPlateReactionWhileAck(void);
//清洗机械手往反应盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutPlateReactionWhileAck(void);
//清洗机械手从清洗盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchPlateWashWhileAck(void);
//清洗机械手往清洗盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutPlateWashWhileAck(void);
//清洗机械手穷测量模块抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashCatchMeasureModuleWhileAck(void);
//清洗机械手往测量模块放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandWashPutMeasureModuleWhileAck(void);





/**********************************************************************阻塞API*************************************************************************/
//清洗机械手复位,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手旋转到清洗盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotatePlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手旋转到反应盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotatePlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手旋转到测量模块,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashRotateMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手从反应盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手往反应盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手从清洗盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchPlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手往清洗盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutPlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手穷测量模块抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashCatchMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//清洗机械手往测量模块放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandWashPutMeasureModuleWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);






#endif



