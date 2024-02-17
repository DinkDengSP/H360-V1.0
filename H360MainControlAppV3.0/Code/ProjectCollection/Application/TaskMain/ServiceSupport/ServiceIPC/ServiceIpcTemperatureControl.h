/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:34:08
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-26 20:46:47
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcTemperatureControl.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_TEMPERATURE_CONTROL_H_
#define __SERVICE_IPC_TEMPERATURE_CONTROL_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceTemperatureControlDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceTemperatureControlGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceTemperatureControlWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//读取反应盘温度,不等待指令完成,非堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateReactionWhileAck(void);
//读取试剂盘温度,不等待指令完成,非堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateReagentWhileAck(void);
//读取清洗盘温度,不等待指令完成,非堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateWashWhileAck(void);
//读取环境温度,不等待指令完成,非堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelEnvirWhileAck(void);
//读取温控板板上温度,不等待指令完成,非堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelBoardWhileAck(void);
//读取制冷机状态,不等待返回
LH_ERR IPC_ServiceTemperatureControlReadCoolerStateWhileAck(void);
//设置制冷机状态,不等待返回
LH_ERR IPC_ServiceTemperatureControlSetCoolerStateWhileAck(uint8_t setState);

/**********************************************************************阻塞API*************************************************************************/
//读取反应盘温度,等待指令完成,堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读取试剂盘温度,等待指令完成,堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateReagentWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读取清洗盘温度,等待指令完成,堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelPlateWashWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读取环境温度,等待指令完成,堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelEnvirWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//读取温控板板上温度,等待指令完成,堵塞
LH_ERR IPC_ServiceTemperatureControlReadChannelBoardWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//设置制冷机状态,等待返回
LH_ERR IPC_ServiceTemperatureControlSetCoolerStateWhileReturn(uint8_t setState,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
/******************************************************************从数组中读取浮点型温度数据**********************************************************************/
float IPC_ServiceTemperatureControlReadTempByBuffer(uint8_t* bufferPtr);

#endif



