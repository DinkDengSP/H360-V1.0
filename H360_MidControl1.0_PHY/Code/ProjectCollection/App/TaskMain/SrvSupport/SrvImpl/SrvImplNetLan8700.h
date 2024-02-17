/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:05:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 10:36:16 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_LAN8700_H_
#define __SRV_IMPL_LAN8700_H_
#include "SrvImplBase.h"

//主通讯端口服务器缓存
#define TCP_SERVICE_LAN8700_MAIN_SOCKET_BUF_LENGTH     (32*1024)
//日志通讯端口服务器缓存
#define TCP_SERVICE_LAN8700_LOG_SOCKET_BUF_LENGTH      (32*1024)
//主通讯端口号
#define TCP_LAN8700_MAIN_SOCKET_PORT                   8088
//日志通讯端口号
#define TCP_LAN8700_LOG_SOCKET_PORT                    20711
//主通讯端口可以连接的设备总数
#define TCP_LAN8700_MAIN_SOCKET_PORT_MAX_CLIENT        1
//日志端口可以连接的设备总数
#define TCP_LAN8700_LOG_SOCKET_PORT_MAX_CLIENT         1
//发送最大超时时间
#define LAN8700_TRANS_TIME_OUT_MS                      TIME_OUT_MS_ETH_SEND
//PHY网络连接延时消抖时间
#define TIME_MS_LAN8700_PHY_DISCONNECT_WAIT_STABLE     1000
#define COUNT_MAX_LAN8700_PHY_DISCONNECT_WAIT_STABLE   (TIME_MS_LAN8700_PHY_DISCONNECT_WAIT_STABLE/SRV_TASK_NET_LAN8700_FSM_PERIOD_MS)

//网卡协议栈初始化
void SrvImplNetLan8700StackInit(void);

//所有通讯网口状态机
void SrvImplNetLan8700AllPortFSM(void);

//主通讯端口状态机
void SrvImplNetLan8700MainPortFSM(void);

//获取主通讯网口状态
ULONG SrvImplNetLan8700MainGetState(void);

//主通讯网口数据发送
uint32_t SrvImplNetLan8700MainPortSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//获取日志通讯网口状态
ULONG SrvImplNetLan8700LogGetState(void);

//日志网口数据发送
uint32_t SrvImplNetLan8700LogPortSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//日志通讯端口状态机
void SrvImplNetLan8700LogPortFSM(void);



#endif
