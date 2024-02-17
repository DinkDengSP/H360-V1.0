/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 16:51:08
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_NEEDLE_BEAD_VERSION_0_H_
#define __APP_IMPL_LIQUID_NEEDLE_BEAD_VERSION_0_H_
#include "AppImplBase.h"
#include "AppLiquidUtilVersion0.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadResetVersion0(void);

//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenCleanVersion0(NEEDLE_CLEAN_OPT opt);

//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseCleanVersion0(NEEDLE_CLEAN_OPT opt);

//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrimeVersion0(NEEDLE_PRIME_OPT opt);

//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrimeVersion0(NEEDLE_PRIME_OPT opt);

//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintainVersion0(NEEDLE_MAINTAIN_OPT opt);

//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintainVersion0(NEEDLE_MAINTAIN_OPT opt);


#endif




