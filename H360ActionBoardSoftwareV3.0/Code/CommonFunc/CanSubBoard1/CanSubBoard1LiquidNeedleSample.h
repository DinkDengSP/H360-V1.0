/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 19:58:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-05 15:52:29
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1LiquidNeedleSample.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_LIQUID_NEEDLE_SAMPLE_H_
#define __CAN_SUB_BOARD1_LIQUID_NEEDLE_SAMPLE_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//样本针泵阀复位
LH_ERR CanSubBoard1LiquidNeedleSample_Reset(void);

//样本针清洗泵阀打开
LH_ERR CanSubBoard1LiquidNeedleSample_OpenClean(NEEDLE_CLEAN_OPT opt);

//样本针清洗泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleSample_CloseClean(NEEDLE_CLEAN_OPT opt);

//样本针强力清洗泵阀打开
LH_ERR CanSubBoard1LiquidNeedleSample_OpenPowerfulClean(void);

//样本针强力清洗泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleSample_ClosePowerfulClean(void);

//样本针灌注泵阀打开
LH_ERR CanSubBoard1LiquidNeedleSample_OpenPrime(NEEDLE_PRIME_OPT opt);

//样本针灌注泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleSample_ClosePrime(NEEDLE_PRIME_OPT opt);

//样本针清洗维护泵阀打开
LH_ERR CanSubBoard1LiquidNeedleSample_OpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//样本针清洗维护泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleSample_CloseMaintain(NEEDLE_MAINTAIN_OPT opt);

#endif


