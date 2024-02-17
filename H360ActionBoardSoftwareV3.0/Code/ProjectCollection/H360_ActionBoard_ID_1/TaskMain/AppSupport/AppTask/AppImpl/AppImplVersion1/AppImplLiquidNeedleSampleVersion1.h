/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:33:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-16 19:51:17
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplLiquidNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_LIQUID_NEEDLE_SAMPLE_VERSION_1_H_
#define __APP_IMPL_LIQUID_NEEDLE_SAMPLE_VERSION_1_H_
#include "AppImplBase.h"


//样本针泵阀复位
LH_ERR AppImplLiquidNeedleSampleResetVersion1(void);

//样本针清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenCleanVersion1(NEEDLE_CLEAN_OPT opt);

//样本针清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseCleanVersion1(NEEDLE_CLEAN_OPT opt);

//样本针强力清洗泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPowerfulCleanVersion1(void);

//样本针强力清洗泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePowerfulCleanVersion1(void);

//样本针灌注泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenPrimeVersion1(NEEDLE_PRIME_OPT opt);

//样本针灌注泵阀关闭
LH_ERR AppImplLiquidNeedleSampleClosePrimeVersion1(NEEDLE_PRIME_OPT opt);

//样本针清洗维护泵阀打开
LH_ERR AppImplLiquidNeedleSampleOpenMaintainVersion1(NEEDLE_MAINTAIN_OPT opt);

//样本针清洗维护泵阀关闭
LH_ERR AppImplLiquidNeedleSampleCloseMaintainVersion1(NEEDLE_MAINTAIN_OPT opt);


#endif




