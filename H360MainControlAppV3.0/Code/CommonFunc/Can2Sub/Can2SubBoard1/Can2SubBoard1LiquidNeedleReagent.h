/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 19:58:26
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 20:30:28
 *FilePath: \Code\CommonFunc\Can2SubBoard1\Can2SubBoard1LiquidNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BOARD1_LIQUID_NEEDLE_REAGENT_H_
#define __CAN2_SUB_BOARD1_LIQUID_NEEDLE_REAGENT_H_
#include "Can2ProcMain.h"
#include "Can2SubCommBaseInc.h"

//试剂针泵阀复位
LH_ERR Can2SubBoard1LiquidNeedleReagent_Reset(void);

//试剂针清洗泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleReagent_OpenClean(NEEDLE_CLEAN_OPT opt);

//试剂针清洗泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleReagent_CloseClean(NEEDLE_CLEAN_OPT opt);

//试剂针灌注泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleReagent_OpenPrime(NEEDLE_PRIME_OPT opt);

//试剂针灌注泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleReagent_ClosePrime(NEEDLE_PRIME_OPT opt);

//试剂针清洗维护泵阀打开
LH_ERR Can2SubBoard1LiquidNeedleReagent_OpenMaintain(NEEDLE_MAINTAIN_OPT opt);

//试剂针清洗维护泵阀关闭
LH_ERR Can2SubBoard1LiquidNeedleReagent_CloseMaintain(NEEDLE_MAINTAIN_OPT opt);


#endif


