/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:02:25
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-02 13:37:42
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BASE_H_
#define __SERVICE_IMPL_BASE_H_
#include "os.h"
#include "Can1ProcMain.h"
#include "SystemCmd.h"
#include "ServiceState.h"
#include "ServiceParam.h"
#include "ServiceImplBaseConfig.h"

//使能调试信息输出
#define SYSTEM_DEBUG_MSG_SHOW_ENABLE           1


//这个打印信息是动作任务在响应应用任务调用时候的输出信息
#if(SYSTEM_DEBUG_MSG_SHOW_ENABLE == 1)
//输出任务调试讯息
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       ActionPrintf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)                                                                                                           
//输出任务结束讯息
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      ActionPrintf("%s - 0X%08X\r\n",endStr,userErrorCode)
//输出错误信息
#define SystemDebugShowMsgError(errorCode)                                                               ActionPrintf("!!Action Error Func : %s , Line : %d , ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode)
#else
//不输出信息调试
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       asm("nop")
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      asm("nop")
#define SystemDebugShowMsgError(errorCode)                                                               asm("nop")
#endif


//RailPushIn1任务
extern const uint8_t* RailPushIn1DebugStartMsg;
extern const uint8_t* RailPushIn1DebugEndMsg;

//RailPushIn2任务
extern const uint8_t* RailPushIn2DebugStartMsg;
extern const uint8_t* RailPushIn2DebugEndMsg;

//RailPushOut1任务
extern const uint8_t* RailPushOut1DebugStartMsg;
extern const uint8_t* RailPushOut1DebugEndMsg;

//RailPushOut2任务
extern const uint8_t* RailPushOut2DebugStartMsg;
extern const uint8_t* RailPushOut2DebugEndMsg;

//RailTrans任务
extern const uint8_t* RailTransDebugStartMsg;
extern const uint8_t* RailTransDebugEndMsg;

//RailHand任务
extern const uint8_t* RailHandDebugStartMsg;
extern const uint8_t* RailHandDebugEndMsg;

//RailComplex任务
extern const uint8_t* RailComplexDebugStartMsg;
extern const uint8_t* RailComplexDebugEndMsg;

//BarScan任务
extern const uint8_t* BarScanDebugStartMsg;
extern const uint8_t* BarScanDebugEndMsg;

//Heart任务
extern const uint8_t* HeartDebugStartMsg;
extern const uint8_t* HeartDebugEndMsg;

//Debug任务
extern const uint8_t* DebugDebugStartMsg;
extern const uint8_t* DebugDebugEndMsg;

//模块输入单元映射
typedef struct MODULE_INPUT_MAP_UNIT
{
    SYSTEM_INPUT_INDEX inputIndex;
    BitAction validState;
}MODULE_INPUT_MAP_UNIT;

//模块输出单元映射
typedef struct MODULE_OUTPUT_MAP_UNIT
{
    SYSTEM_OUTPUT_INDEX outputIndex;
    BitAction validState;
}MODULE_OUTPUT_MAP_UNIT;

//模块电机复位函数指针类型
typedef LH_ERR (*STEP_MOTOR_RESET_FUNC_PTR)(RAIL_RESET_CORRECT_FLAG resetFlag);
//模块步进电机映射
typedef struct MODULE_STEP_MOTOR_MAP_UNIT
{
    CAN2_SUB_SM_INDEX motorIndex;
    STEP_MOTOR_RESET_FUNC_PTR resetFuncPtr;
}MODULE_STEP_MOTOR_MAP_UNIT;


//模块读取指定输入
LH_ERR ServiceImplModuleInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT* arrayPtr,RAIL_SENSOR_STATE* state);

//模块写入指定输出
LH_ERR ServiceImplModuleOutputWrite(uint8_t index,RAIL_OUTPUT_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT* arrayPtr);

//模块指定电机复位
LH_ERR ServiceImplModuleMotorReset(uint8_t index,RAIL_RESET_CORRECT_FLAG resetCorrectFlag,uint8_t indexMax,MODULE_STEP_MOTOR_MAP_UNIT* arrayPtr);

//模块指定电机走位
LH_ERR ServiceImplModuleMotorRunSteps(uint8_t index,uint8_t dir,uint32_t steps,uint8_t indexMax,int32_t* posAfterRun,MODULE_STEP_MOTOR_MAP_UNIT* arrayPtr);

//辅助通用功能实现
LH_ERR ServiceImplBase_UtilCommon(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//自动循环心跳
void ServiceImplHeart_EventInit(void);

//自动循环心跳
void ServiceImplHeart_EventRun(uint32_t periodTimeMs);



#endif


