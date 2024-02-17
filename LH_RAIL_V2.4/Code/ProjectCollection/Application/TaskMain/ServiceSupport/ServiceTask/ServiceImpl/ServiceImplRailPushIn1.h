/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:25:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:25:07
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailPushIn1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_PUSH_IN1_H_
#define __SERVICE_IMPL_RAIL_PUSH_IN1_H_
#include "ServiceImplPushIn1Util.h"

//自动循环事件初始化
void ServiceImplRailPushIn1_EventInit(void);

//自动循环事件
void ServiceImplRailPushIn1_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//进样1复位
LH_ERR ServiceImplRailPushIn1Reset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1走步数
LH_ERR ServiceImplRailPushIn1MotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1读取指定端口
LH_ERR ServiceImplRailPushIn1InputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1写入指定端口
LH_ERR ServiceImplRailPushIn1OutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1推进一次
LH_ERR ServiceImplRailPushIn1PushOnce(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1推进多次直到触发
LH_ERR ServiceImplRailPushIn1PushMultiWhileTrig(uint16_t retryMax,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//进样1获取传感器状态
LH_ERR ServiceImplRailPushIn1GetSensorState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);



#endif


