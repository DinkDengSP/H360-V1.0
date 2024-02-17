/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 16:29:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-09 09:08:00
 *FilePath: \App\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplRailTrans.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_RAIL_TRANS_H_
#define __SERVICE_IMPL_RAIL_TRANS_H_
#include "ServiceImplRailTransUtil.h"

//急诊位是否存在试管架的状态
typedef enum EMERGENCY_REGION_RACK_STATE
{
    EMERGENCY_REGION_RACK_NOT_EXIST     = 0X00,//急诊区域不存在试管架
    EMERGENCY_REGION_RACK_EXIST         = 0X01,//急诊区域存在试管架
}EMERGENCY_REGION_RACK_STATE;

//自动循环事件初始化
void ServiceImplRailTrans_EventInit(void);

//自动循环事件
void ServiceImplRailTrans_EventRun(uint32_t periodTimeMs);

/*************************************轨道主控自身使用的指令*****************************************/
//轨道运转复位
LH_ERR ServiceImplRailTransReset(RAIL_RESET_CORRECT_FLAG resetFlag,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运转走步数
LH_ERR ServiceImplRailTransMotorRunSteps(uint8_t dir,uint32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运转读取指定端口
LH_ERR ServiceImplRailTransInputRead(uint8_t pinIndex,uint8_t* pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运转写入指定端口
LH_ERR ServiceImplRailTransOutPutWrite(uint8_t pinIndex,uint8_t pinValue,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到进样1
LH_ERR ServiceImplRailTransMove2PushIn1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到进样2
LH_ERR ServiceImplRailTransMove2PushIn2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到出样1
LH_ERR ServiceImplRailTransMove2PushOut1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到出样2
LH_ERR ServiceImplRailTransMove2PushOut2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到急诊区
LH_ERR ServiceImplRailTransMove2EmergencyCall(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到旋转位1
LH_ERR ServiceImplRailTransMove2Rotate1(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到旋转位2
LH_ERR ServiceImplRailTransMove2Rotate2(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到距离比较近的旋转位
LH_ERR ServiceImplRailTransMove2RotateAuto(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到试管架扫码位
LH_ERR ServiceImplRailTransMove2RackBarScan(RAIL_BAR_SCAN_POS barScanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定试管扫码位
LH_ERR ServiceImplRailTransMove2TubeBarScan(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS barScanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定模块待测位
LH_ERR ServiceImplRailTransMove2WaitTest(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定模块重测位
LH_ERR ServiceImplRailTransMove2RepeatTest(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO repeatTestNo,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定模块测试区放架位
LH_ERR ServiceImplRailTransMove2TestingPut(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定模块测试区取架位
LH_ERR ServiceImplRailTransMove2TestingCatch(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定模块测试区吸样位
LH_ERR ServiceImplRailTransMove2TestingAbsorb(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//获取急诊位状态
LH_ERR ServiceImplRailTransGetEmergencyState(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//急诊电子锁锁定
LH_ERR ServiceImplRailTransLockSet(RAIL_OUTPUT_STATE lockSet,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到指定试管扫码位
LH_ERR ServiceImplRailTransMove2BarScanIntegrate(uint8_t* barScanResult,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//轨道运行到集成扫码指定位置-起始,结束
LH_ERR ServiceImplRailTransMove2BarScanPosIntegrate(RAIL_BAR_SCAN_POS scanPos,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

#endif




