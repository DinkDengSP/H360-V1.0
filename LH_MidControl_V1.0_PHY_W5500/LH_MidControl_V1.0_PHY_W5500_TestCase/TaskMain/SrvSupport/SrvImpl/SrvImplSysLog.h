/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 19:50:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:50:57 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_SYS_LOG_H_
#define __SRV_IMPL_SYS_LOG_H_
#include "SrvImplBase.h"

//系统日志长度
#define BLOCK_SIZE_SYSTEM_LOG                           1024
//单行显示缓存数据长度
#define SYS_BUF_SHOW_SINGLE_LINE                        20

//任务全局对象初始化
void SrvImplSysLogVarInit(void);

//任务接收消息处理
void SrvImplSysLogMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplSysLogFSM_Init(void);

//任务状态机周期运行
void SrvImplSysLogFSM_RunPeriod(void);




#endif

