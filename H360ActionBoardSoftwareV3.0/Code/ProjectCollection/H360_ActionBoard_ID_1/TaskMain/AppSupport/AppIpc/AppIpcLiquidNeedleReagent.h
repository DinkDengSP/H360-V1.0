/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 12:13:14
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_LIQUID_NEEDLE_REAGENT_H_
#define __APP_IPC_LIQUID_NEEDLE_REAGENT_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_LiquidNeedleReagentDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidNeedleReagentGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_LiquidNeedleReagentWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//试剂针泵阀复位,不等待完成
LH_ERR IPC_LiquidNeedleReagentResetWhileAck(void);
//试剂针清洗泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleReagentOpenCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//试剂针清洗泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleReagentCloseCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//试剂针灌注泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleReagentOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt);
//试剂针灌注泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleReagentClosePrimeWhileAck(NEEDLE_PRIME_OPT opt);
//试剂针清洗维护泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleReagentOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);
//试剂针清洗维护泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleReagentCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);

/****************************************************发出指令并等待完成********************************************************/
//试剂针泵阀复位,等待完成
LH_ERR IPC_LiquidNeedleReagentResetWhileReturn(void);
//试剂针清洗泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleReagentOpenCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//试剂针清洗泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleReagentCloseCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//试剂针灌注泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleReagentOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//试剂针灌注泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleReagentClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//试剂针清洗维护泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleReagentOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);
//试剂针清洗维护泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleReagentCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);


#endif


