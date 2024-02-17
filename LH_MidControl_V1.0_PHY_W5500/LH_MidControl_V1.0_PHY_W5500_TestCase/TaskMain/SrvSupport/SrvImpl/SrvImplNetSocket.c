/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:55:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:46:39 +0800
************************************************************************************************/ 
#include "SrvImplNetSocket.h"
#include "SrvTaskConfig.h"
#include "SrvParam.h"
#include "SrvState.h"

//LAN8700函数导入
//网卡协议栈初始化
extern void SrvImplNetLan8700StackInit(void);

//W5500函数导入
//网卡协议栈初始化
extern void SrvImplNetW5500StackInit(void);

//任务全局对象初始化
void SrvImplNetSocketVarInit(void)
{
    //初始化LAN8700网卡协议栈
    SrvImplNetLan8700StackInit();
    //初始化W5500协议栈
    SrvImplNetW5500StackInit();
}

//任务接收消息处理
void SrvImplNetSocketMsgProcess(void* taskMsgPtr)
{
    
}

//任务状态机初始化
void SrvImplNetSocketFSM_Init(void)
{
    
}

//LAN8700函数导入
//PHY网线链接检测状态机
extern void SrvImplNetLan8700PhyLinkFSM(void);
//主通讯端口状态机
extern void SrvImplNetLan8700MainPortFSM(void);
//日志通讯端口状态机
extern void SrvImplNetLan8700LogPortFSM(void);

//W5500函数导入
//PHY网线链接检测状态机
extern void SrvImplNetW5500PhyLinkFSM(void);
//主通讯端口状态机
extern void SrvImplNetW5500MainPortFSM(void);
//日志通讯端口状态机
extern void SrvImplNetW5500LogPortFSM(void);

//任务状态机周期运行
void SrvImplNetSocketFSM_RunPeriod(void)
{
    //LAN8700PHY网线链接检测状态机
    SrvImplNetLan8700PhyLinkFSM();
    //LAN8700主通讯端口状态机
    SrvImplNetLan8700MainPortFSM();
    //LAN8700日志通讯端口状态机
    SrvImplNetLan8700LogPortFSM();

    //W5500PHY网线链接检测状态机
    SrvImplNetW5500PhyLinkFSM();
    //W5500主通讯端口状态机
    SrvImplNetW5500MainPortFSM();
    //W5500日志通讯端口状态机
    SrvImplNetW5500LogPortFSM();
}

