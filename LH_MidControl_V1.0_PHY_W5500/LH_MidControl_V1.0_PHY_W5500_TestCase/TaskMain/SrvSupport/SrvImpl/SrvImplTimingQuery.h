/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 19:42:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:52:07 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_TIMING_QUERY_H_
#define __SRV_IMPL_TIMING_QUERY_H_
#include "SrvImplBase.h"

//任务全局对象初始化
void SrvImplTimingQueryVarInit(void);

//任务接收消息处理
void SrvImplTimingQueryMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplTimingQueryFSM_Init(void);

//任务状态机周期运行
void SrvImplTimingQueryFSM_RunPeriod(void);




#endif

