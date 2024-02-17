/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:02:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 15:31:40
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplCupTrayStackManage.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_CUP_TRAY_STACK_MANAGE_H_
#define __SERVICE_IMPL_CUP_TRAY_STACK_MANAGE_H_
#include "ServiceImplCupTrayStackManageConfig.h"

//自动运行状态机初始化
LH_ERR ServiceImplCupTrayStackManage_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplCupTrayStackManage_StateMachineLoop(void);

//读取当前杯栈管理的门的状态
LH_ERR ServiceImplCupTrayStackManage_QueryState(STACK_MANAGE_DOOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯栈初始化,返回当前有几个新盘以及新盘传感器的状态
LH_ERR ServiceImplCupTrayStackManage_InitNewStack(uint8_t* newTrayCount,SENSOR_STATE* newTraySensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//空杯栈初始化,返回当前空杯栈的数目以及正在测试杯盘传感器的状态
LH_ERR ServiceImplCupTrayStackManage_InitEmptyStack(uint8_t* emptyTrayCount,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//推手初始化,返回新盘传感器和正在测试杯盘传感器的状态
LH_ERR ServiceImplCupTrayStackManage_InitPush(SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//夹手初始化
LH_ERR ServiceImplCupTrayStackManage_InitClamp(SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//杯栈管理模块整体初始化
LH_ERR ServiceImplCupTrayStackManage_InitAll(uint8_t* newTrayCount,uint8_t* emptyTrayCount,SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,SENSOR_STATE* garbage1Exist,
                                                SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//夹手打开
LH_ERR ServiceImplCupTrayStackManage_ClampOpen(SENSOR_STATE* testTrayExistSensor,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//夹手关闭
LH_ERR ServiceImplCupTrayStackManage_ClampClose(SENSOR_STATE* testTrayExistSensor,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新杯盘管理模块上传一个新杯盘
LH_ERR ServiceImplCupTrayStackManage_UploadNewTray(uint8_t* newTrayCount,SENSOR_STATE* newTraySensorState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//空杯盘管理模块回收一个空杯盘
LH_ERR ServiceImplCupTrayStackManage_RecoveryEmptyTray(uint8_t* emptyTrayCount,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//推手从新杯盘区推一个盘子到测试区
LH_ERR ServiceImplCupTrayStackManage_PushTray2Testing(SENSOR_STATE* newTraySensorState,SENSOR_STATE* testingTrayInplaceState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//锁定升起指定垃圾桶
LH_ERR ServiceImplCupTrayStackManage_GarbageLockUp(GARBAGE_SELECT garbageSelect,SENSOR_STATE* garbage1Exist,SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                    CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//解锁降下指定垃圾桶
LH_ERR ServiceImplCupTrayStackManage_GarbageReleaseDown(GARBAGE_SELECT garbageSelect,SENSOR_STATE* garbage1Exist,SENSOR_STATE* garbage2Exist,CAN1_REPORT_ERR_CODE* commErrorCode,
                                                                CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//门打开到位之后,新盘升起来指定层方便放盘
LH_ERR ServiceImplCupTrayStackManage_NewTrayRiseupWhileDoorOpenInPlace(uint8_t trayCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//新盘管理回零
LH_ERR ServiceImplCupTrayStackManage_NewTrayReturnZero(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//门打开到位之后,空盘升起来指定层方便放盘子
LH_ERR ServiceImplCupTrayStackManage_EmptyTrayRiseupWhileDoorOpenInPlace(uint8_t trayCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//空盘管理回零
LH_ERR ServiceImplCupTrayStackManage_EmptyTrayReturnZero(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplCupTrayStackManage_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplCupTrayStackManage_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplCupTrayStackManage_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplCupTrayStackManage_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//更新系统传感器状态
LH_ERR ServiceImplCupTrayStackManage_ReflushSensorState(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);


#endif


