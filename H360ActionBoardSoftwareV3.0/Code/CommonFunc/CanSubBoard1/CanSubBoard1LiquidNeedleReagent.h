/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 19:58:26
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 12:00:12
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard1\CanSubBoard1LiquidNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD1_LIQUID_NEEDLE_REAGENT_H_
#define __CAN_SUB_BOARD1_LIQUID_NEEDLE_REAGENT_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//试剂针泵阀复位
LH_ERR CanSubBoard1LiquidNeedleReagent_Reset(void);

//试剂针清洗泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenClean(NEEDLE_CLEAN_OPT opt);

//试剂针清洗泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_CloseClean(NEEDLE_CLEAN_OPT opt);

//试剂针灌注泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenPrime(NEEDLE_PRIME_OPT opt);

//试剂针灌注泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_ClosePrime(NEEDLE_PRIME_OPT opt);

//试剂针清洗维护泵阀打开
LH_ERR CanSubBoard1LiquidNeedleReagent_OpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//试剂针清洗维护泵阀关闭
LH_ERR CanSubBoard1LiquidNeedleReagent_CloseMaintain(NEEDLE_MAINTAIN_OPT opt);


#endif


