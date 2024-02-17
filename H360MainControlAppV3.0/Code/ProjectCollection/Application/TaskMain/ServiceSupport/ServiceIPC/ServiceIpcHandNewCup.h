/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-03 00:08:47
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcHandNewCup.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_HAND_NEW_CUP_H_
#define __SERVICE_IPC_HAND_NEW_CUP_H_
#include "ServiceIpcBase.h"

//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceHandNewCupDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceHandNewCupGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceHandNewCupWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//新杯机械手复位,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupResetAllWhileAck(void);
//新杯机械手移动到新杯栈,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2NewStackWhileAck(uint8_t leftRight,uint8_t frontBack);
//新杯机械手移动到反应盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2PlateReactionWhileAck(void);
//新杯机械手移动到垃圾桶,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupMove2GarbageWhileAck(GARBAGE_SELECT garbageNo);
//新杯机械手从新杯栈抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromNewStackWhileAck(void);
//新杯机械手往反应盘放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToPlateReactionWhileAck(void);
//新杯机械手从反应盘抓杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromPlateReactionWhileAck(void);
//新杯机械手往垃圾桶放杯,不等待指令完成,非堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToGarbageWhileAck(GARBAGE_SELECT garbageNo);




/**********************************************************************阻塞API*************************************************************************/
//新杯机械手复位,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupResetAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手移动到新杯栈,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2NewStackWhileReturn(uint8_t leftRight,uint8_t frontBack,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手移动到反应盘,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2PlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手移动到垃圾桶,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupMove2GarbageWhileReturn(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手从新杯栈抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromNewStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手往反应盘放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手从反应盘抓杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupCatchCupFromPlateReactionWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯机械手往垃圾桶放杯,等待指令完成,堵塞
LH_ERR IPC_ServiceHandNewCupPutCupToGarbageWhileReturn(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


#endif



