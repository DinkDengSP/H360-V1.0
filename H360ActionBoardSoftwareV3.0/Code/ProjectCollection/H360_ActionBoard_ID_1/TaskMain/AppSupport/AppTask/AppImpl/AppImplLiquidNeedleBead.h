/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:15:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:25:24
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_NEEDLE_BEAD_H_
#define __APP_IMPL_NEEDLE_BEAD_H_
#include "AppImplLiquidNeedleBeadVersion0.h"
#include "AppImplLiquidNeedleBeadVersion1.h"

//磁珠针泵阀复位
LH_ERR AppImplLiquidNeedleBeadReset(void);

//磁珠针清洗泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenClean(NEEDLE_CLEAN_OPT opt);

//磁珠针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseClean(NEEDLE_CLEAN_OPT opt);

//磁珠针灌注泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenPrime(NEEDLE_PRIME_OPT opt);

//磁珠针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleBeadClosePrime(NEEDLE_PRIME_OPT opt);

//磁珠针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleBeadOpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//磁珠针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleBeadCloseMaintain(NEEDLE_MAINTAIN_OPT opt);



#endif

