/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 21:43:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-18 18:06:41
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_TEST_FLOW_H_
#define __APP_IPC_TEST_FLOW_H_
#include "AppIpcBase.h"

//IPC调用基础结构初始化
void IPC_AppTestFlowInit(void);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_AppTestFlowGetLastCommandStateAndResult(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel,LH_ERR* errorParam);

//等待执行完成,阻塞
LH_ERR IPC_AppTestFlowWaitLastReturn(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel);

/*********************************************发送指令不等待完成********************************************************************************/
//模块保留指令
LH_ERR IPC_AppTestFlowReserveWhileAck(void);


/*********************************************发送指令并等待完成********************************************************************************/
//模块保留指令
LH_ERR IPC_AppTestFlowReserveWhileReturn(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel);



#endif





