/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:07:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 20:56:37 +0800
************************************************************************************************/ 
#ifndef __SRV_STATE_H_
#define __SRV_STATE_H_
#include "SrvStateDataType.h"
#include "SrvStateDefault.h"

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SrvState_NetX_GetSocketState(ULONG state);

//服务层状态初始化
void SrvStateInit(void);

//获取服务层状态集合指针
SRV_STATE* SrvStateGetAllStatePtr(void);

/*--------------------------------LAN8700网络底层讯息相关接口------------------------------------------*/
//获取服务层网络底层信息指针
SRV_STATE_SOCKET_LAN8700* SrvStateGetSocketLan8700BaseStatePtr(void);

//设置Socket初始化完成
void SrvStateSetSocketLan8700BaseInitOver(void);

//检查基础Socket是否初始化完成
ErrorStatus SrvStateCheckSocketLan8700BaseInitOver(void);

//检查网线是否插入
uint32_t SrvStateCheckSocketLan8700BaseCheckPhyState(void);

//获取Socket内存池指针
NX_PACKET_POOL* SrvStateGetSocketLan8700MemPoolPtr(void);

//获取服务层网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SrvStateGetLan8700MainSocketPtr(void);

//获取服务层网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetLan8700MainSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetLan8700MainSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SrvStateGetLan8700MainSocketStateCurrentByRecord(void);
void SrvStateSetMainLan8700SocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SrvStateGetLan8700MainSocketStateLastByRecord(void);
void SrvStateSetMainLan8700SocketStateLastByRecord(ULONG setVal);

//获取服务层网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SrvStateGetLan8700LogSocketPtr(void);

//获取服务层网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetLan8700LogSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetLan8700LogSocketStateByInfo(void);

//读写当前日志通讯Socket状态,内存记录的
ULONG SrvStateGetLan8700LogSocketStateCurrentByRecord(void);
void SrvStateSetLan8700LogSocketStateCurrentByRecord(ULONG setVal);

//读写上一次日志通讯Socket状态,内存记录的
ULONG SrvStateGetLan8700LogSocketStateLastByRecord(void);
void SrvStateSetLan8700LogSocketStateLastByRecord(ULONG setVal);
/*---------------------------------------------------------------------------------------------*/



#endif


