/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 19:58:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-04 13:47:19
 *FilePath: \Applicationd:\SVN_Cloud\H200\EmbeddedSoftware\H360\H360MainControlAppV2.2\Code\CommonFunc\Can2SubBoard1\Can2SubBoard1LiquidNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_LIQUID_NEEDLE_SAMPLE_H_
#define __CAN2_SUB_BOARD1_LIQUID_NEEDLE_SAMPLE_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//样本针泵阀复位
LH_ERR Can2SubBoard1LiquidNeedleSample_Reset(void);

//样本针清洗泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleSample_OpenClean(NEEDLE_CLEAN_OPT opt);

//样本针清洗泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleSample_CloseClean(NEEDLE_CLEAN_OPT opt);

//样本针强力清洗泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleSample_OpenPowerfulClean(void);

//样本针强力清洗泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleSample_ClosePowerfulClean(void);

//样本针灌注泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleSample_OpenPrime(NEEDLE_PRIME_OPT opt);

//样本针灌注泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleSample_ClosePrime(NEEDLE_PRIME_OPT opt);

//样本针清洗维护泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleSample_OpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//样本针清洗维护泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleSample_CloseMaintain(NEEDLE_MAINTAIN_OPT opt);

#endif


