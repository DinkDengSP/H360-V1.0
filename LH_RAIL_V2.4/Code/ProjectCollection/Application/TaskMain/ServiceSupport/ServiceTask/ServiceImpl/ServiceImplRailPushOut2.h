/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:27:18
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:25:30
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailPushOut2.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_PUSH_OUT2_H_
#define __SERVICE_IMPL_RAIL_PUSH_OUT2_H_
#include "ServiceImplPushOut2Util.h"


//自动循环事件初始化
void ServiceImplRailPushOut2_EventInit(void);

//自动循环事件
void ServiceImplRailPushOut2_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//出样2复位
LH_ERR ServiceImplRailPushOut2Reset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2走步数
LH_ERR ServiceImplRailPushOut2MotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2读取指定端口
LH_ERR ServiceImplRailPushOut2InputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2写入指定端口
LH_ERR ServiceImplRailPushOut2OutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2推进一次然后回零
LH_ERR ServiceImplRailPushOut2PushOnceWithReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2推进一次不回零
LH_ERR ServiceImplRailPushOut2PushOnceWithOutReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2回零
LH_ERR ServiceImplRailPushOut2ReturnZero(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2获取当前推进到第几次
LH_ERR ServiceImplRailPushOut2GetPushIndex(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2推架子到顶端
LH_ERR ServiceImplRailPushOut2PushMax(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2读取当前传感器状态
LH_ERR ServiceImplRailPushOut2GetSensorState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//出样2状态控制
LH_ERR ServiceImplRailPushOut2SetState(RAIL_OUTPUT_STATE railState,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif




