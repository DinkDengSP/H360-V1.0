/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 21:42:09
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-18 18:06:27
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppIpcTestFlow.h"
#include "AppTaskConfig.h"

//标志左移位数
#define EVENT_OFFSET_LEFT_SHIFT_APP_TEST_FLOW   0
//指令集
typedef enum IPC_CMD_APP_TEST_FLOW
{
    IPC_CMD_APP_TEST_FLOW_RESERVE           = 0x00,//保留指令
}IPC_CMD_APP_TEST_FLOW;

//IPC结构
static IPC_DATA_BASE ipcDataAppTestFlow;


//IPC调用基础结构初始化
void IPC_AppTestFlowInit(void)
{
    IPC_DataInit(&ipcDataAppTestFlow,"mutexIpcAppTestFlow",&queueAppTaskTestFlow,&ipcDataEventGroupAck,&ipcDataEventGroupComplete,
                    EVENT_OFFSET_LEFT_SHIFT_APP_TEST_FLOW);
}

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_AppTestFlowGetLastCommandStateAndResult(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel,LH_ERR* errorParam)
{
    return IPC_BaseGetLastCommandStateAndResult(&ipcDataAppTestFlow,errorParam,errorLevel,errorCode);
}

//等待执行完成,阻塞
LH_ERR IPC_AppTestFlowWaitLastReturn(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel)
{
    return IPC_BaseWaitLastReturn(&ipcDataAppTestFlow,errorLevel,errorCode);
}

/*********************************************发送指令不等待完成********************************************************************************/
//模块保留指令
LH_ERR IPC_AppTestFlowReserveWhileAck(void)
{
    //动作API发送任务给动作API,等待ACK不等待返回
    return IPC_BaseSendCommandWhileAck(&ipcDataAppTestFlow,IPC_CMD_APP_TEST_FLOW_RESERVE,(int32_t)(0),(int32_t)(0),
                                                (int32_t)(0),(int32_t)(0),(int32_t)(0),NULL,0);
}


/*********************************************发送指令并等待完成********************************************************************************/
//模块保留指令
LH_ERR IPC_AppTestFlowReserveWhileReturn(TCP_ERR* errorCode,TCP_ERR_LEVEL* errorLevel)
{
    //动作API发送任务给动作API,等待返回
    return IPC_BaseSendCommandWhileReturn(&ipcDataAppTestFlow,IPC_CMD_APP_TEST_FLOW_RESERVE,(int32_t)(0),(int32_t)(0),
                                            (int32_t)(0),(int32_t)(0),(int32_t)(0),NULL,0,errorLevel,errorCode);
}


