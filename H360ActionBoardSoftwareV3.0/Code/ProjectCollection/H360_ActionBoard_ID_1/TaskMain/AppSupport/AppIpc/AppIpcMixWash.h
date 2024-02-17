/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:20
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:10:01
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcMixWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_MIX_WASH_H_
#define __APP_IPC_MIX_WASH_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_MixWashDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MixWashGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_MixWashWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//清洗混匀模块复位,带零位偏移,不等待完成
LH_ERR IPC_MixWashResetWhileAck(void);
//清洗混匀升起来,不等待完成
LH_ERR IPC_MixWashUpWhileAck(MIX_MODE mixMode);
//清洗混匀升降降下去,不等待完成
LH_ERR IPC_MixWashDownWhileAck(void);
//清洗混匀旋转混匀一次,不带自动升降,不等待完成
LH_ERR IPC_MixWashStartRotateWhileAck(MIX_MODE mixMode);
//清洗混匀集成混匀一次,不等待完成
LH_ERR IPC_MixWashRotateIntegrateWhileAck(MIX_MODE mixMode);

/****************************************************发出指令并等待完成********************************************************/
//清洗混匀模块复位,带零位偏移,等待完成
LH_ERR IPC_MixWashResetWhileReturn(void);
//清洗混匀升起来,等待完成
LH_ERR IPC_MixWashUpWhileReturn(MIX_MODE mixMode);
//清洗混匀升降降下去,等待完成
LH_ERR IPC_MixWashDownWhileReturn(void);
//清洗混匀旋转混匀一次,不带自动升降,等待完成
LH_ERR IPC_MixWashStartRotateWhileReturn(MIX_MODE mixMode);
//清洗混匀集成混匀一次,等待完成
LH_ERR IPC_MixWashRotateIntegrateWhileReturn(MIX_MODE mixMode);

#endif


