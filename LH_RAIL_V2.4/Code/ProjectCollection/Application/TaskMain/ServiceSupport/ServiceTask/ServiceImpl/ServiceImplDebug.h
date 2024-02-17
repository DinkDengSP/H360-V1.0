/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:25:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 10:48:31
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplDebug.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_DEBUG_H_
#define __SERVICE_IMPL_DEBUG_H_
#include "ServiceImplBarScan.h"

//自动循环事件初始化
void ServiceImplDebug_EventInit(void);

//自动循环事件
void ServiceImplDebug_EventRun(uint32_t periodTimeMs);


/*************************************轨道主控自身使用的指令*****************************************/

//通用调试电机复位
LH_ERR ServiceImplDebugMotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//通用调试电机走步数
LH_ERR ServiceImplDebugMotorRunSteps(uint8_t motorIndex,uint8_t dir,uint32_t steps,uint8_t curveS,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取指定调试序号的IO口输入
LH_ERR ServiceImplDebugInputRead(uint16_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定调试序号的IO口输出
LH_ERR ServiceImplDebugOutputWrite(uint16_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取指定系统参数
LH_ERR ServiceImplDebugReadSystemParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定系统参数
LH_ERR ServiceImplDebugWriteSystemParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取版本号
LH_ERR ServiceImplDebugReadBoardVersion(uint8_t boardNo,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//读取指定电机参数
LH_ERR ServiceImplDebugReadMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t* paramResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//写入指定电机参数
LH_ERR ServiceImplDebugWriteMotorParam(uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//通讯握手
LH_ERR ServiceImplDebugHandShake(uint8_t shakeMode,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//系统急停
LH_ERR ServiceImplDebugEmergencyStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);





#endif


