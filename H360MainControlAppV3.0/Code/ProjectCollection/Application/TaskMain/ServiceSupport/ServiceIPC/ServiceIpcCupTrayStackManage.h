/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:24:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-18 13:41:57
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcCupTrayStackManage.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_CUP_TRAY_STACK_MANAGE_H_
#define __SERVICE_IPC_CUP_TRAY_STACK_MANAGE_H_
#include "ServiceIpcBase.h"


//IPC调用基础结构初始化,初始化调用,流程勿使用
void IPC_ServiceCupTrayStackManageDataInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceCupTrayStackManageGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//等待执行完成,阻塞
LH_ERR IPC_ServiceCupTrayStackManageWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);


/**********************************************************************非阻塞API*************************************************************************/
//查询杯栈管理门状态,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageQueryStateWhileAck(void);
//新杯栈初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitNewStackWhileAck(void);
//空杯栈初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitEmptyStackWhileAck(void);
//推手初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitPushWhileAck(void);
//夹手初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitClampWhileAck(void);
//全部初始化,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitAllWhileAck(void);
//上传一个新盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageUploadNewTrayWhileAck(void);
//回收一个旧盘,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageRecoveryEmptyTrayWhileAck(void);
//从新杯托盘推一个托盘到测试区,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManagePushTray2TestingWhileAck(void);
//垃圾桶解锁上升,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageLockUpWhileAck(GARBAGE_SELECT garbageSelect);
//垃圾桶释放下降,不等待指令完成,非堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageReleaseDownWhileAck(GARBAGE_SELECT garbageSelect);
//更新杯栈管理传感器状态
LH_ERR IPC_ServiceCupTrayStackManageReflushSensorStateWhileAck(void);



/**********************************************************************阻塞API*************************************************************************/
//查询杯栈管理门状态,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageQueryStateWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//新杯栈初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitNewStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//空杯栈初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitEmptyStackWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//推手初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitPushWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//夹手初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitClampWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//全部初始化,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageInitAllWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//上传一个新盘,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageUploadNewTrayWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//回收一个旧盘,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageRecoveryEmptyTrayWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//从新杯托盘推一个托盘到测试区,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManagePushTray2TestingWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//垃圾桶解锁上升,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageLockUpWhileReturn(GARBAGE_SELECT garbageSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//垃圾桶释放下降,等待指令完成,堵塞
LH_ERR IPC_ServiceCupTrayStackManageGarbageReleaseDownWhileReturn(GARBAGE_SELECT garbageSelect,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//更新杯栈管理传感器状态
LH_ERR IPC_ServiceCupTrayStackManageReflushSensorStateWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);




#endif



