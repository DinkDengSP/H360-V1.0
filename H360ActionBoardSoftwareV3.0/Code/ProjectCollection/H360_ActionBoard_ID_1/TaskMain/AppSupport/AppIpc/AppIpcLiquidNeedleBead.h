/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:39:58
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-06 11:38:11
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcLiquidNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_LIQUID_NEEDLE_BEAD_H_
#define __APP_IPC_LIQUID_NEEDLE_BEAD_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_LiquidNeedleBeadDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_LiquidNeedleBeadGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_LiquidNeedleBeadWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//磁珠针泵阀复位,不等待完成
LH_ERR IPC_LiquidNeedleBeadResetWhileAck(void);
//磁珠针清洗泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleBeadOpenCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//磁珠针清洗泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleBeadCloseCleanWhileAck(NEEDLE_CLEAN_OPT opt);
//磁珠针灌注泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleBeadOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt);
//磁珠针灌注泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleBeadClosePrimeWhileAck(NEEDLE_PRIME_OPT opt);
//磁珠针清洗维护泵阀打开,不等待完成
LH_ERR IPC_LiquidNeedleBeadOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);
//磁珠针清洗维护泵阀关闭,不等待完成
LH_ERR IPC_LiquidNeedleBeadCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt);

/****************************************************发出指令并等待完成********************************************************/
//磁珠针泵阀复位,等待完成
LH_ERR IPC_LiquidNeedleBeadResetWhileReturn(void);
//磁珠针清洗泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleBeadOpenCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//磁珠针清洗泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleBeadCloseCleanWhileReturn(NEEDLE_CLEAN_OPT opt);
//磁珠针灌注泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleBeadOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//磁珠针灌注泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleBeadClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt);
//磁珠针清洗维护泵阀打开,等待完成
LH_ERR IPC_LiquidNeedleBeadOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);
//磁珠针清洗维护泵阀关闭,等待完成
LH_ERR IPC_LiquidNeedleBeadCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt);


#endif


