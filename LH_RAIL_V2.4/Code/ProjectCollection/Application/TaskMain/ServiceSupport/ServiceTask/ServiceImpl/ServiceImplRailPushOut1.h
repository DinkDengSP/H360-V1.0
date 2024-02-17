/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:25:39
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:25:21
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailPushOut1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_PUSH_OUT1_H_
#define __SERVICE_IMPL_RAIL_PUSH_OUT1_H_
#include "ServiceImplPushOut1Util.h"


//自动循环事件初始化
void ServiceImplRailPushOut1_EventInit(void);

//自动循环事件
void ServiceImplRailPushOut1_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//出样1复位
LH_ERR ServiceImplRailPushOut1Reset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1走步数
LH_ERR ServiceImplRailPushOut1MotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1读取指定端口
LH_ERR ServiceImplRailPushOut1InputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1写入指定端口
LH_ERR ServiceImplRailPushOut1OutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1推进一次然后回零
LH_ERR ServiceImplRailPushOut1PushOnceWithReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1推进一次不回零
LH_ERR ServiceImplRailPushOut1PushOnceWithOutReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1回零
LH_ERR ServiceImplRailPushOut1ReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1获取当前推进到第几次
LH_ERR ServiceImplRailPushOut1GetPushIndex(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1推架子到顶端
LH_ERR ServiceImplRailPushOut1PushMax(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1读取当前传感器状态
LH_ERR ServiceImplRailPushOut1GetSensorState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样1状态控制
LH_ERR ServiceImplRailPushOut1SetState(RAIL_OUTPUT_STATE railState,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif


