/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:13:10
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-25 15:55:39
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplCupTrayManage.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_CUP_TRAY_MANAGE_H_
#define __APP_IMPL_CUP_TRAY_MANAGE_H_
#include "AppImplCupTrayManageConfig.h"

//杯栈管理状态机初始化
LH_ERR AppImplCupTrayManageStateMachineInit(void);

//杯栈管理状态机循环
LH_ERR AppImplCupTrayManageStateMachineLoop(void);

//设置杯栈门状态
LH_ERR AppImplCupTrayManageSetDoorState(STACK_MANAGE_DOOR_STATE doorState);

//新杯盘栈初始化
LH_ERR AppImplCupTrayManageNewStackInit(STACK_MANAGE_RESULT* resultData);

//空杯盘栈初始化
LH_ERR AppImplCupTrayManageEmptyStackInit(STACK_MANAGE_RESULT* resultData);

//推手初始化
LH_ERR AppImplCupTrayManagePushInit(STACK_MANAGE_RESULT* resultData);

//夹手初始化
LH_ERR AppImplCupTrayManageClampInit(STACK_MANAGE_RESULT* resultData);

//垃圾桶下降
LH_ERR AppImplCupTrayManageGarbageDownUnlock(GARBAGE_SELECT garbageNo,STACK_MANAGE_RESULT* resultData);

//垃圾桶上升
LH_ERR AppImplCupTrayManageGarbegeUpLock(GARBAGE_SELECT garbageNo,STACK_MANAGE_RESULT* resultData);

//杯栈模块整体初始化
LH_ERR AppImplCupTrayManageInitAll(STACK_MANAGE_RESULT* resultData);

//夹手释放
LH_ERR AppImplCupTrayManageClampOpen(STACK_MANAGE_RESULT* resultData);

//夹手夹紧
LH_ERR AppImplCupTrayManageClampClose(STACK_MANAGE_RESULT* resultData);

//新杯盘栈上传一个新盘
LH_ERR AppImplCupTrayManageUploadNewTray(STACK_MANAGE_RESULT* resultData);

//空杯盘栈从测试区回收一个空盘
LH_ERR AppImplCupTrayManageRecycleEmptyTray(STACK_MANAGE_RESULT* resultData);

//推手推一个新盘到测试位
LH_ERR AppImplCupTrayManagePushNewTrayToTest(STACK_MANAGE_RESULT* resultData);

//新杯盘栈上升指定层(门开到位状态)
LH_ERR AppImplCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData);

//新杯盘栈回零(门开到位状态)
LH_ERR AppImplCupTrayManageNewStackReturnZero(STACK_MANAGE_RESULT* resultData);

//空杯盘上升指定层(门开到位状态)
LH_ERR AppImplCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData);

//空杯盘栈回零(门开到位状态)
LH_ERR AppImplCupTrayManageEmptyStackReturnZero(STACK_MANAGE_RESULT* resultData);

//读取传感器讯息
LH_ERR AppImplCupTrayManageReadSensorState(STACK_MANAGE_RESULT* resultData);

//指定步进电机复位
LH_ERR AppImplCupTrayManageStepMotorReset(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index);

//指定步进电机走指定步数
LH_ERR AppImplCupTrayManageStepMotorRunSteps(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index,int32_t steps,int32_t* posAfterRun);




#endif



