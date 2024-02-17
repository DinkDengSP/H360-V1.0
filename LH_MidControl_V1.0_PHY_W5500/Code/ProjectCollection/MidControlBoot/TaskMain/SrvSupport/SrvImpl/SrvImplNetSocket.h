/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-10 19:09:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 15:36:04 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_NET_SOCKET_H_
#define __SRV_IMPL_NET_SOCKET_H_
#include "SrvImplBase.h"


//主通讯端口服务器缓存
#define TCP_SERVICE_SOCKET_BUF_LENGTH_MAIN     4096
//日志通讯端口服务器缓存
#define TCP_SERVICE_SOCKET_BUF_LENGTH_LOG      4096
//主通讯端口号
#define TCP_SOCKET_PORT_MAIN                   8088
//日志通讯端口号
#define TCP_SOCKET_PORT_LOG                    20711
//主通讯端口可以连接的设备总数
#define TCP_SOCKET_PORT_MAIN_MAX_CLIENT        1
//日志端口可以连接的设备总数
#define TCP_SOCKET_PORT_LOG_MAX_CLIENT         1
//发送最大超时时间
#define W5500_TRANS_TIME_OUT_MS                1000
//PHY网络连接延时消抖时间
#define TIME_MS_PHY_DISCONNECT_WAIT_STABLE     3000
//PHY连接检测周期
#define TIME_MS_PHY_DISCONNECT_CHECK_PERIOD    1000
//检测网线连接稳定时间
#define COUNT_MAX_PHY_DISCONNECT_WAIT_STABLE   (TIME_MS_PHY_DISCONNECT_WAIT_STABLE/TIME_MS_PHY_DISCONNECT_CHECK_PERIOD)


//网卡协议栈初始化
void SrvImplNetSocketStackInit(void);

//所有通讯网口状态机
void SrvImplNetSocketPhyLinkFSM(void);

//主通讯端口状态机
void SrvImplNetSocketMainPortFSM(void);

//获取主通讯网口状态
ULONG SrvImplNetSocketMainGetState(void);

//主通讯网口数据发送
uint32_t SrvImplNetSocketMainPortSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//获取日志通讯网口状态
ULONG SrvImplNetSocketLogGetState(void);

//日志网口数据发送
uint32_t SrvImplNetSocketLogPortSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//日志通讯端口状态机
void SrvImplNetSocketLogPortFSM(void);


#endif

