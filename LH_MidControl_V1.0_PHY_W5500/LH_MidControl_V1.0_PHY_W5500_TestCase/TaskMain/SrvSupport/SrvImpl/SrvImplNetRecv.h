/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 20:53:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:53:21 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_NET_RECV_H_
#define __SRV_IMPL_NET_RECV_H_
#include "SrvImplBase.h"

//任务全局对象初始化
void SrvImplNetRecvVarInit(void);

//任务接收消息处理
void SrvImplNetRecvMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplNetRecvFSM_Init(void);

//任务状态机周期运行
void SrvImplNetRecvFSM_RunPeriod(void);




#endif

