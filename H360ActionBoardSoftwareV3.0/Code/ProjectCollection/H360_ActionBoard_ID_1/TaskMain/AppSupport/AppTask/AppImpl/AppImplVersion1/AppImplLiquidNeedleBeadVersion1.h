/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:46:59
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_NEEDLE_BEAD_VERSION_1_H_
#define __APP_IMPL_LIQUID_NEEDLE_BEAD_VERSION_1_H_
#include "AppImplBase.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadResetVersion1(void);

//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenCleanVersion1(NEEDLE_CLEAN_OPT opt);

//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseCleanVersion1(NEEDLE_CLEAN_OPT opt);

//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrimeVersion1(NEEDLE_PRIME_OPT opt);

//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrimeVersion1(NEEDLE_PRIME_OPT opt);

//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintainVersion1(NEEDLE_MAINTAIN_OPT opt);

//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintainVersion1(NEEDLE_MAINTAIN_OPT opt);


#endif




