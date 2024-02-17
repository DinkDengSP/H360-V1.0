/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:40:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-06 13:12:24
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppIpc\AppIpcNeedleWash.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_NEEDLE_WASH_H_
#define __APP_IPC_NEEDLE_WASH_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_NeedleWashDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_NeedleWashGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_NeedleWashWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//清洗针复位,带零位偏移,不等待完成
LH_ERR IPC_NeedleWashResetWhileAck(void);
//清洗针灌注,不等待完成
LH_ERR IPC_NeedleWashPrimeWhileAck(uint8_t primeCount);
//清洗针液路维护,不等待完成
LH_ERR IPC_NeedleWashMaintainCleanWhileAck(uint8_t maintainCount);
//清洗针执行一次清洗,不等待完成
LH_ERR IPC_NeedleWashBeadCleanWhileAck(uint8_t cleanConfig);

/****************************************************发出指令并等待完成********************************************************/
//清洗针复位,带零位偏移,等待完成
LH_ERR IPC_NeedleWashResetWhileReturn(void);
//清洗针灌注,等待完成
LH_ERR IPC_NeedleWashPrimeWhileReturn(uint8_t primeCount);
//清洗针液路维护,等待完成
LH_ERR IPC_NeedleWashMaintainCleanWhileReturn(uint8_t maintainCount);
//清洗针执行一次清洗,等待完成
LH_ERR IPC_NeedleWashBeadCleanWhileReturn(uint8_t cleanConfig);

#endif


