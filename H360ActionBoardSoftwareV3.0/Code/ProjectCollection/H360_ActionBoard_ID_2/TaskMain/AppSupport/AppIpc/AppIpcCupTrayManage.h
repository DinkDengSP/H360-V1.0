/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:16:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-10 18:24:36
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppIpc\AppIpcCupTrayManage.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_CUP_TRAY_MANAGE_H_
#define __APP_IPC_CUP_TRAY_MANAGE_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_CupTrayManageDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_CupTrayManageGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_CupTrayManageWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//设置杯栈门状态,不等待执行完成
LH_ERR IPC_CupTrayManageSetDoorStateWhileAck(STACK_MANAGE_DOOR_STATE doorState);
//新杯盘栈初始化,不等待执行完成
LH_ERR IPC_CupTrayManageNewStackInitWhileAck(void);
//空杯盘栈初始化,不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackInitWhileAck(void);
//推手初始化,不等待执行完成
LH_ERR IPC_CupTrayManagePushInitWhileAck(void);
//夹手初始化,不等待执行完成
LH_ERR IPC_CupTrayManageClampInitWhileAck(void);
//垃圾桶下降,不等待执行完成
LH_ERR IPC_CupTrayManageGarbageDownUnlockWhileAck(GARBAGE_SELECT garbageNo);
//垃圾桶上升,不等待执行完成
LH_ERR IPC_CupTrayManageGarbegeUpLockWhileAck(GARBAGE_SELECT garbageNo);
//杯栈模块整体初始化,不等待执行完成
LH_ERR IPC_CupTrayManageInitAllWhileAck(void);
//夹手释放,不等待执行完成
LH_ERR IPC_CupTrayManageClampOpenWhileAck(void);
//夹手夹紧,不等待执行完成
LH_ERR IPC_CupTrayManageClampCloseWhileAck(void);
//新杯盘栈上传一个新盘,不等待执行完成
LH_ERR IPC_CupTrayManageUploadNewTrayWhileAck(void);
//空杯盘栈从测试区回收一个空盘,不等待执行完成
LH_ERR IPC_CupTrayManageRecycleEmptyTrayWhileAck(void);
//推手推一个新盘到测试位,不等待执行完成
LH_ERR IPC_CupTrayManagePushNewTrayToTestWhileAck(void);
//新杯盘栈上升指定层(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageNewStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers);
//新杯盘栈回零(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageNewStackReturnZeroWhileAck(void);
//空杯盘上升指定层(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileAck(uint8_t layers);
//空杯盘栈回零(门开到位状态),不等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackReturnZeroWhileAck(void);
//读取传感器讯息,不等待执行完成
LH_ERR IPC_CupTrayManageReadSensorStateWhileAck(void);

/****************************************************发出指令并等待完成********************************************************/
//设置杯栈门状态,等待执行完成
LH_ERR IPC_CupTrayManageSetDoorStateWhileReturn(STACK_MANAGE_DOOR_STATE doorState);
//新杯盘栈初始化,等待执行完成
LH_ERR IPC_CupTrayManageNewStackInitWhileReturn(void);
//空杯盘栈初始化,等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackInitWhileReturn(void);
//推手初始化,等待执行完成
LH_ERR IPC_CupTrayManagePushInitWhileReturn(void);
//夹手初始化,等待执行完成
LH_ERR IPC_CupTrayManageClampInitWhileReturn(void);
//垃圾桶下降,等待执行完成
LH_ERR IPC_CupTrayManageGarbageDownUnlockWhileReturn(GARBAGE_SELECT garbageNo);
//垃圾桶上升,等待执行完成
LH_ERR IPC_CupTrayManageGarbegeUpLockWhileReturn(GARBAGE_SELECT garbageNo);
//杯栈模块整体初始化,等待执行完成
LH_ERR IPC_CupTrayManageInitAllWhileReturn(void);
//夹手释放,等待执行完成
LH_ERR IPC_CupTrayManageClampOpenWhileReturn(void);
//夹手夹紧,等待执行完成
LH_ERR IPC_CupTrayManageClampCloseWhileReturn(void);
//新杯盘栈上传一个新盘,等待执行完成
LH_ERR IPC_CupTrayManageUploadNewTrayWhileReturn(void);
//空杯盘栈从测试区回收一个空盘,等待执行完成
LH_ERR IPC_CupTrayManageRecycleEmptyTrayWhileReturn(void);
//推手推一个新盘到测试位,等待执行完成
LH_ERR IPC_CupTrayManagePushNewTrayToTestWhileReturn(void);
//新杯盘栈上升指定层(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageNewStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers);
//新杯盘栈回零(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageNewStackReturnZeroWhileReturn(void);
//空杯盘上升指定层(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackUpSpecialLayerWithDoorOpenStateWhileReturn(uint8_t layers);
//空杯盘栈回零(门开到位状态),等待执行完成
LH_ERR IPC_CupTrayManageEmptyStackReturnZeroWhileReturn(void);
//读取传感器讯息,等待执行完成
LH_ERR IPC_CupTrayManageReadSensorStateWhileReturn(void);



#endif



