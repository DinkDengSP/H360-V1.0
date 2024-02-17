/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:06:36
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 13:30:38
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppIpc\AppIpcMixReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_MIX_REAGENT_H_
#define __APP_IPC_MIX_REAGENT_H_
#include "AppIpcBase.h"


//初始化IPC调用结构
void IPC_MixReagentDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MixReagentGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_MixReagentWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//试剂混匀模块复位,带零位偏移,不等待完成
LH_ERR IPC_MixReagentResetWhileAck(void);
//试剂混匀升起来,不等待完成
LH_ERR IPC_MixReagentUpWhileAck(MIX_MODE mixMode);
//试剂混匀升降降下去,不等待完成
LH_ERR IPC_MixReagentDownWhileAck(void);
//试剂混匀旋转混匀一次,不带自动升降,不等待完成
LH_ERR IPC_MixReagentStartRotateWhileAck(MIX_MODE mixMode);
//试剂混匀集成混匀一次,不等待完成
LH_ERR IPC_MixReagentRotateIntegratedWhileAck(MIX_MODE mixMode);


/****************************************************发出指令并等待完成********************************************************/
//试剂混匀模块复位,带零位偏移,等待完成
LH_ERR IPC_MixReagentResetWhileReturn(void);
//试剂混匀升起来,等待完成
LH_ERR IPC_MixReagentUpWhileReturn(MIX_MODE mixMode);
//试剂混匀升降降下去,等待完成
LH_ERR IPC_MixReagentDownWhileReturn(void);
//试剂混匀旋转混匀一次,不带自动升降,等待完成
LH_ERR IPC_MixReagentStartRotateWhileReturn(MIX_MODE mixMode);
//试剂混匀集成混匀一次,等待完成
LH_ERR IPC_MixReagentRotateIntegratedWhileReturn(MIX_MODE mixMode);


#endif

