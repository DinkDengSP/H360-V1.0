/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 17:19:27
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_NEEDLE_REAGENT_VERSION_0_H_
#define __APP_IMPL_LIQUID_NEEDLE_REAGENT_VERSION_0_H_
#include "AppImplBase.h"
#include "AppLiquidUtilVersion0.h"

//试剂针泵阀复位
LH_ERR AppImplLiquidNeedleReagentResetVersion0(void);

//试剂针清洗泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenCleanVersion0(NEEDLE_CLEAN_OPT opt);

//试剂针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseCleanVersion0(NEEDLE_CLEAN_OPT opt);

//试剂针灌注泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenPrimeVersion0(NEEDLE_PRIME_OPT opt);

//试剂针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleReagentClosePrimeVersion0(NEEDLE_PRIME_OPT opt);

//试剂针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenMaintainVersion0(NEEDLE_MAINTAIN_OPT opt);

//试剂针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseMaintainVersion0(NEEDLE_MAINTAIN_OPT opt);



#endif




