/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:14:59
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:25:10
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_AUTO_H_
#define __APP_IMPL_LIQUID_AUTO_H_
#include "AppImplLiquidAutoVersion0.h"
#include "AppImplLiquidAutoVersion1.h"

//液路自动化状态机初始化
void AppImplLiquidAutoStateMachineInit(void);

//液路自动化状态机循环
void AppImplLiquidAutoStateMachineLoop(void);

//液路自动化单元初始化
LH_ERR AppImplLiquidAutoInit(void);

//强制执行一次抽废液
LH_ERR AppImplLiquidAutoForceTrigWasteLiquid(void);

//打开自动抽废液功能
LH_ERR AppImplLiquidAutoEnableWasteLiquidAutomatic(void);

//关闭自动抽废液功能
LH_ERR AppImplLiquidAutoDisableWasteLiquidAutomatic(void);

//查看自动抽废液状态
LH_ERR AppImplLiquidAutoCheckStateWasteLiquid(LIQUID_AUTOMATIC_STATE* statePtr);

//关闭三针清洗液稀释液路
LH_ERR AppImplLiquidAutoCloseLiquidDilutionThreeNeedleClean(void);

//打开三针清洗液稀释液路
LH_ERR AppImplLiquidAutoOpenLiquidDilutionThreeNeedleClean(DILUTION_CHANNEL channel);


#endif

