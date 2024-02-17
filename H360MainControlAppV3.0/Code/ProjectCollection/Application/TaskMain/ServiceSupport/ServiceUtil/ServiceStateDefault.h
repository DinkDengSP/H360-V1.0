/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 13:48:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-18 08:58:38
 *FilePath: \Application\TaskMain\ServiceSupport\ServiceUtil\ServiceStateDefault.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_STATE_DEFAULT_H_
#define __SERVICE_STATE_DEFAULT_H_
#include "ServiceStateType.h"

//辅助状态初始化默认值
extern const SERVICE_STATE_ASSIST stateAssistDefault;

//缓冲液注液初始化默认值
extern const SERVICE_STATE_BUFFER_SOLUTION_INJECT stateBufferSolutionInjectDefault;

//杯栈管理初始化默认值
extern const SERVICE_STATE_CUP_TRAY_STACK_MANAGE stateCupTrayStackManageDefault;

//新杯机械手初始化默认值
extern const SERVICE_STATE_HAND_NEW_CUP stateHandNewCupDefault;

//清洗机械手初始化默认值
extern const SERVICE_STATE_HAND_WASH stateHandWashDefault;

//样本针初始化默认值
extern const SERVICE_STATE_NEEDLE_SAMPLE stateNeedleSampleDefault;

//磁珠针初始化默认值
extern const SERVICE_STATE_NEEDLE_BEAD stateNeedleBeadDefault;

//试剂针初始化默认值
extern const SERVICE_STATE_NEEDLE_REAGENT stateNeedleReagentDefault;

//清洗针初始化默认值
extern const SERVICE_STATE_NEEDLE_WASH stateNeedleWashDefault;

//测量模块初始化默认值
extern const SERVICE_STATE_MEASURE_MODULE stateMeasureModuleDefault;

//反应盘初始化默认值
extern const SERVICE_STATE_PLATE_REACTION statePlateReactionDefault;

//清洗盘初始化默认值
extern const SERVICE_STATE_PLATE_WASH statePlateWashDefault;

//试剂盘初始化默认值
extern const SERVICE_STATE_PLATE_REAGENT statePlateReagentDefault;

//磁珠混匀初始化默认值
extern const SERVICE_STATE_MIX_BEAD stateMixBeadDefault;

//试剂混匀初始化默认值
extern const SERVICE_STATE_MIX_REAGENT stateMixReagentDefault;

//清洗混匀初始化默认值
extern const SERVICE_STATE_MIX_WASH stateMixWashDefault;

//液路自动化初始化默认值
extern const SERVICE_STATE_LIQUID_AUTO stateLiquidAutoDefault;

//温度控制初始化默认值
extern const SERVICE_STATE_TEMP_CONTROL stateTempControlDefault;



#endif




