/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:28:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 16:25:41
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailHand.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_HAND_H_
#define __SERVICE_IMPL_RAIL_HAND_H_
#include "ServiceImplRailHandUtil.h"

//自动循环事件初始化
void ServiceImplRailHand_EventInit(void);

//自动循环事件
void ServiceImplRailHand_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//机械手旋转复位
LH_ERR ServiceImplRailHandRotateReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手前后复位
LH_ERR ServiceImplRailHandBackFrontReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手升降复位
LH_ERR ServiceImplRailHandUpDownReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手三维复位
LH_ERR ServiceImplRailHandModuleReset(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手指定电机复位
LH_ERR ServiceImplRailHandMotorReset(uint8_t motorIndex,RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手指定电机走步数
LH_ERR ServiceImplRailHandStepMotorRunSteps(uint8_t motorIndex,uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手读取指定端口
LH_ERR ServiceImplRailHandInputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手写入指定端口
LH_ERR ServiceImplRailHandOutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手抓取进样1
LH_ERR ServiceImplRailHandCatchPushIn1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手抓取进样2
LH_ERR ServiceImplRailHandCatchPushIn2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手放下出样1
LH_ERR ServiceImplRailHandPutPushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手放下出样2
LH_ERR ServiceImplRailHandPutPushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手旋转进样方向
LH_ERR ServiceImplRailHandRotate2PushIn(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手旋转到进样方向
LH_ERR ServiceImplRailHandRotate2EmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手旋转到指定方向
LH_ERR ServiceImplRailHandRotate2SpecialDir(HAND_DIR targetDir,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手从急诊位抓取试管架
LH_ERR ServiceImplRailHandCatchEmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手从指定模块待测位抓取试管架
LH_ERR ServiceImplRailHandCatchWaitTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手往指定模块待测位放置试管架
LH_ERR ServiceImplRailHandPutWaitTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手从指定模块重测位抓取试管架
LH_ERR ServiceImplRailHandCatchRepeatTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//机械手往指定模块重测位放置试管架
LH_ERR ServiceImplRailHandPutRepeatTest(RAIL_MODULE_POS moduleNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//获取机械手当前状态
LH_ERR ServiceImplRailHandGetState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道机械手夹爪到出样1
LH_ERR ServiceImplRailHandClawReachPushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道机械手夹爪到出样2
LH_ERR ServiceImplRailHandClawReachPushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道机械手收回夹爪
LH_ERR ServiceImplRailHandTakeClawBack(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif




