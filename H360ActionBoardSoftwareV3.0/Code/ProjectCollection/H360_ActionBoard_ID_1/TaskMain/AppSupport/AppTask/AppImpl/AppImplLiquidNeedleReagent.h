/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:15:30
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:25:32
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_NEEDLE_REAGENT_H_
#define __APP_IMPL_NEEDLE_REAGENT_H_
#include "AppImplLiquidNeedleReagentVersion0.h"
#include "AppImplLiquidNeedleReagentVersion1.h"

//试剂针泵阀复位
LH_ERR AppImplLiquidNeedleReagentReset(void);

//试剂针清洗泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenClean(NEEDLE_CLEAN_OPT opt);

//试剂针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseClean(NEEDLE_CLEAN_OPT opt);

//试剂针灌注泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenPrime(NEEDLE_PRIME_OPT opt);

//试剂针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleReagentClosePrime(NEEDLE_PRIME_OPT opt);

//试剂针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleReagentOpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//试剂针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleReagentCloseMaintain(NEEDLE_MAINTAIN_OPT opt);




#endif

