/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:25:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-08 14:53:01 +0800
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

/*--------------------------------网络底层讯息相关接口------------------------------------------*/
//获取服务层网络底层信息指针
SRV_STATE_SOCKET_BASE* SrvStateGetSocketBaseStatePtr(void);

//设置Socket初始化完成
void SrvStateSetSocketBaseInitOver(void);

//检查基础Socket是否初始化完成
ErrorStatus SrvStateCheckSocketBaseInitOver(void);

//检查网线是否插入
uint32_t SrvStateCheckSocketBaseCheckPhyState(void);

//获取Socket内存池指针
NX_PACKET_POOL* SrvStateGetSocketMemPoolPtr(void);

//获取服务层网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SrvStateGetMainSocketPtr(void);

//获取服务层网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetMainSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetMainSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SrvStateGetMainSocketStateCurrentByRecord(void);
void SrvStateSetMainSocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SrvStateGetMainSocketStateLastByRecord(void);
void SrvStateSetMainSocketStateLastByRecord(ULONG setVal);

//获取服务层网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SrvStateGetLogSocketPtr(void);

//获取服务层网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SrvStateGetLogSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SrvStateGetLogSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SrvStateGetLogSocketStateCurrentByRecord(void);
void SrvStateSetLogSocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SrvStateGetLogSocketStateLastByRecord(void);
void SrvStateSetLogSocketStateLastByRecord(ULONG setVal);
/*---------------------------------------------------------------------------------------------*/


#endif



