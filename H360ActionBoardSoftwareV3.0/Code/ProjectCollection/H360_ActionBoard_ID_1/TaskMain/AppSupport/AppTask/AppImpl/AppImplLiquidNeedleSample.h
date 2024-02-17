/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-16 17:15:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:25:39
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_NEEDLE_SAMPLE_H_
#define __APP_IMPL_NEEDLE_SAMPLE_H_
#include "AppImplLiquidNeedleSampleVersion0.h"
#include "AppImplLiquidNeedleSampleVersion1.h"

//样本针泵阀复位
LH_ERR AppImplLiquidNeedleSampleReset(void);

//样本针清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenClean(NEEDLE_CLEAN_OPT opt);

//样本针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseClean(NEEDLE_CLEAN_OPT opt);

//样本针强力清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPowerfulClean(void);

//样本针强力清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePowerfulClean(void);

//样本针灌注泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPrime(NEEDLE_PRIME_OPT opt);

//样本针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePrime(NEEDLE_PRIME_OPT opt);

//样本针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//样本针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseMaintain(NEEDLE_MAINTAIN_OPT opt);



#endif

