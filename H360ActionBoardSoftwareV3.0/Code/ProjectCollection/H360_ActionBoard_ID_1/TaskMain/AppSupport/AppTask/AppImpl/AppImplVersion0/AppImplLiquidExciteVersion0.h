/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:36
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 17:19:20
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidExcite.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_EXCITE_VERSION_0_H_
#define __APP_IMPL_LIQUID_EXCITE_VERSION_0_H_
#include "AppImplBase.h"
#include "AppLiquidUtilVersion0.h"

//激发液泵阀复位
LH_ERR AppImplLiquidExciteResetVersion0(void);

//激发液选通指定瓶
LH_ERR AppImplLiquidExciteOpenSelectInjectVersion0(BOTTLE_LIQUID_EXCITE bottle);

//激发液选通清洗维护
LH_ERR AppImplLiquidExciteOpenSelectMaintainVersion0(BOTTLE_LIQUID_EXCITE bottle);

//激发液泵阀关闭
LH_ERR AppImplLiquidExciteCloseVersion0(void);

//激发液开关废液排放
LH_ERR AppImplLiquidExciteSetWasteLiquidVersion0(LIQUID_EXCITE_WASTE_STATE state);

#endif




