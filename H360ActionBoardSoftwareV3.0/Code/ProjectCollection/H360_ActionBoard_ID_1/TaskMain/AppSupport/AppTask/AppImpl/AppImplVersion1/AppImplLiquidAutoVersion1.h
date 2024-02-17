/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:44:39
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidAuto.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_AUTO_VERSION_1_H_
#define __APP_IMPL_LIQUID_AUTO_VERSION_1_H_
#include "AppImplLiquidAutoConfigVersion1.h"

//液路自动化状态机初始化
void AppImplLiquidAutoStateMachineInitVersion1(void);

//液路自动化状态机循环
void AppImplLiquidAutoStateMachineLoopVersion1(void);

//液路自动化单元初始化
LH_ERR AppImplLiquidAutoInitVersion1(void);

//强制执行一次抽废液
LH_ERR AppImplLiquidAutoForceTrigWasteLiquidVersion1(void);

//打开自动抽废液功能
LH_ERR AppImplLiquidAutoEnableWasteLiquidAutomaticVersion1(void);

//关闭自动抽废液功能
LH_ERR AppImplLiquidAutoDisableWasteLiquidAutomaticVersion1(void);

//查看自动抽废液状态
LH_ERR AppImplLiquidAutoCheckStateWasteLiquidVersion1(LIQUID_AUTOMATIC_STATE* statePtr);

//关闭三针清洗液稀释液路
LH_ERR AppImplLiquidAutoCloseLiquidDilutionThreeNeedleCleanVersion1(void);

//打开三针清洗液稀释液路
LH_ERR AppImplLiquidAutoOpenLiquidDilutionThreeNeedleCleanVersion1(DILUTION_CHANNEL channel);

#endif





