/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:08:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-08 14:50:35 +0800
************************************************************************************************/ 
#include "SrvState.h"

//NETX状态映射表
static const NETX_STATE_MAP_STR NetX_StateStrMapArray[] = {
    {NX_TCP_CLOSED          ,"NX_TCP_CLOSED"      },
    {NX_TCP_LISTEN_STATE    ,"NX_TCP_LISTEN_STATE"},
    {NX_TCP_SYN_SENT        ,"NX_TCP_SYN_SENT"    },
    {NX_TCP_SYN_RECEIVED    ,"NX_TCP_SYN_RECEIVED"},
    {NX_TCP_ESTABLISHED     ,"NX_TCP_ESTABLISHED" },
    {NX_TCP_CLOSE_WAIT      ,"NX_TCP_CLOSE_WAIT"  },
    {NX_TCP_FIN_WAIT_1      ,"NX_TCP_FIN_WAIT_1"  },
    {NX_TCP_FIN_WAIT_2      ,"NX_TCP_FIN_WAIT_2"  },
    {NX_TCP_CLOSING         ,"NX_TCP_CLOSING"     },
    {NX_TCP_TIMED_WAIT      ,"NX_TCP_TIMED_WAIT"  },
    {NX_TCP_LAST_ACK        ,"NX_TCP_LAST_ACK"    },
};
#define LENGTH_NETX_STATE_MAP_STR_MAP       (sizeof(NetX_StateStrMapArray)/sizeof(NetX_StateStrMapArray[0]))
//未知状态描述表
static const uint8_t NetX_DespStrUnknow[] = "NX_TCP_UNKNOW!!";

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SrvState_NetX_GetSocketState(ULONG state)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_NETX_STATE_MAP_STR_MAP;indexUtil++)
    {
        if(state == NetX_StateStrMapArray[indexUtil].state)
        {
            return (uint8_t*)(NetX_StateStrMapArray[indexUtil].despStr);
        }
    }
    return (uint8_t*)(NetX_DespStrUnknow);
}

//服务层讯息集合
static SRV_STATE* srvStateAllPtr = NULL;

//服务层状态初始化
void SrvStateInit(void)
{
    //重复初始化,释放内存
    if(srvStateAllPtr != NULL)
    {
        UserMemFree(SRV_STATE_USE_MEM_REGION,srvStateAllPtr);
    }
    //申请内存
    srvStateAllPtr = UserMemMallocWhileSuccess(SRV_STATE_USE_MEM_REGION,LENGTH_SRV_STATE);
    //设置状态为默认值
    srvStateAllPtr->socketBaseMsg.socketInitFlag = 0;
}


/*--------------------------------网络底层讯息相关接口------------------------------------------*/
//获取服务层状态集合指针
SRV_STATE* SrvStateGetAllStatePtr(void)
{
    return srvStateAllPtr;
}

//获取Socket内存池指针
NX_PACKET_POOL* SrvStateGetSocketMemPoolPtr(void)
{
    return srvStateAllPtr->socketBaseMsg.netMemPoolPtr;
}

//获取服务层网络底层信息指针
SRV_STATE_SOCKET_BASE* SrvStateGetSocketBaseStatePtr(void)
{
    return &(srvStateAllPtr->socketBaseMsg);
}

//设置Socket初始化完成
void SrvStateSetSocketBaseInitOver(void)
{
    srvStateAllPtr->socketBaseMsg.socketInitFlag = SRV_STATE_SOCKET_BASE_SOCKET_INIT_FLAG;
}

//检查基础Socket是否初始化完成
ErrorStatus SrvStateCheckSocketBaseInitOver(void)
{
    if(srvStateAllPtr->socketBaseMsg.socketInitFlag == SRV_STATE_SOCKET_BASE_SOCKET_INIT_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//检查网线是否插入
uint32_t SrvStateCheckSocketBaseCheckPhyState(void)
{
    //获取当前网线连接状态
    return srvStateAllPtr->socketBaseMsg.lanPhyConnectStateCurrent;
}

//获取服务层网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SrvStateGetMainSocketPtr(void)
{
    return &(srvStateAllPtr->socketBaseMsg.tcpSocketMain);
}

//获取服务层网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetMainSocketInfoPtr(void)
{
    return &(srvStateAllPtr->socketBaseMsg.socketInfoMain);
}

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetMainSocketStateByInfo(void)
{
    return srvStateAllPtr->socketBaseMsg.socketInfoMain.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SrvStateGetMainSocketStateCurrentByRecord(void)
{
    return srvStateAllPtr->socketBaseMsg.socketMainStateCurrent;
}

void SrvStateSetMainSocketStateCurrentByRecord(ULONG setVal)
{
    srvStateAllPtr->socketBaseMsg.socketMainStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SrvStateGetMainSocketStateLastByRecord(void)
{
    return srvStateAllPtr->socketBaseMsg.socketMainStateLast;
}
void SrvStateSetMainSocketStateLastByRecord(ULONG setVal)
{
    srvStateAllPtr->socketBaseMsg.socketMainStateLast = setVal;
}

//获取服务层网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SrvStateGetLogSocketPtr(void)
{
    return &(srvStateAllPtr->socketBaseMsg.tcpSocketLog);
}

//获取服务层网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetLogSocketInfoPtr(void)
{
    return &(srvStateAllPtr->socketBaseMsg.socketInfoLog);
}

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetLogSocketStateByInfo(void)
{
    return srvStateAllPtr->socketBaseMsg.socketInfoLog.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SrvStateGetLogSocketStateCurrentByRecord(void)
{
    return srvStateAllPtr->socketBaseMsg.socketLogStateCurrent;
}

void SrvStateSetLogSocketStateCurrentByRecord(ULONG setVal)
{
    srvStateAllPtr->socketBaseMsg.socketLogStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SrvStateGetLogSocketStateLastByRecord(void)
{
    return srvStateAllPtr->socketBaseMsg.socketLogStateLast;
}
void SrvStateSetLogSocketStateLastByRecord(ULONG setVal)
{
    srvStateAllPtr->socketBaseMsg.socketLogStateLast = setVal;
}
/*---------------------------------------------------------------------------------------------*/



