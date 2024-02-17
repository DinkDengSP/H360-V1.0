/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 19:43:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:50:03 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_SERIAL_PORT_RECV_H_
#define __SRV_IMPL_SERIAL_PORT_RECV_H_
#include "SrvImplBase.h"

//任务全局对象初始化
void SrvImplSerialPortRecvVarInit(void);

//任务接收消息处理
void SrvImplSerialPortRecvMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplSerialPortRecvFSM_Init(void);

//任务状态机周期运行
void SrvImplSerialPortRecvFSM_RunPeriod(void);




#endif

