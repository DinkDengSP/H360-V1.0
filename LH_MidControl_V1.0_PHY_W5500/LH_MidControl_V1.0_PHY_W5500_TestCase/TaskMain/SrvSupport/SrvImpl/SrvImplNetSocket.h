/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:55:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:56:29 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_NET_SOCKET_H_
#define __SRV_IMPL_NET_SOCKET_H_
#include "SrvImplBase.h"

/*********************************LAN8700 Socket相关配置与函数************************************/
//主通讯端口服务器缓存
#define TCP_SERVICE_LAN8700_MAIN_SOCKET_BUF_LENGTH     (32*1024)
//日志通讯端口服务器缓存
#define TCP_SERVICE_LAN8700_LOG_SOCKET_BUF_LENGTH      (32*1024)
//主通讯端口号
#define TCP_LAN8700_MAIN_SOCKET_PORT                   12460
//日志通讯端口号
#define TCP_LAN8700_LOG_SOCKET_PORT                    20711
//主通讯端口可以连接的设备总数
#define TCP_LAN8700_MAIN_SOCKET_PORT_MAX_CLIENT        1
//日志端口可以连接的设备总数
#define TCP_LAN8700_LOG_SOCKET_PORT_MAX_CLIENT         1
//发送最大超时时间
#define LAN8700_TRANS_TIME_OUT_MS                      TIME_OUT_MS_ETH_SEND
//PHY网络连接延时消抖时间
#define TIME_MS_LAN8700_PHY_DISCONNECT_WAIT_STABLE     3000
//PHY连接检测周期
#define TIME_MS_LAN8700_PHY_DISCONNECT_CHECK_PERIOD    1000
//检测网线连接稳定时间
#define COUNT_MAX_LAN8700_PHY_DISCONNECT_WAIT_STABLE   (TIME_MS_LAN8700_PHY_DISCONNECT_WAIT_STABLE/TIME_MS_LAN8700_PHY_DISCONNECT_CHECK_PERIOD)

/*********************************W5500 Socket相关配置与函数************************************/
//主通讯端口服务器缓存
#define TCP_SERVICE_W5500_MAIN_SOCKET_BUF_LENGTH     (32*1024)
//日志通讯端口服务器缓存
#define TCP_SERVICE_W5500_LOG_SOCKET_BUF_LENGTH      (32*1024)
//主通讯端口号
#define TCP_W5500_MAIN_SOCKET_PORT                   12460
//日志通讯端口号
#define TCP_W5500_LOG_SOCKET_PORT                    20711
//主通讯端口可以连接的设备总数
#define TCP_W5500_MAIN_SOCKET_PORT_MAX_CLIENT        1
//日志端口可以连接的设备总数
#define TCP_W5500_LOG_SOCKET_PORT_MAX_CLIENT         1
//发送最大超时时间
#define W5500_TRANS_TIME_OUT_MS                      TIME_OUT_MS_ETH_SEND
//PHY网络连接延时消抖时间
#define TIME_MS_W5500_PHY_DISCONNECT_WAIT_STABLE     3000
//PHY连接检测周期
#define TIME_MS_W5500_PHY_DISCONNECT_CHECK_PERIOD    1000
//检测网线连接稳定时间
#define COUNT_MAX_W5500_PHY_DISCONNECT_WAIT_STABLE   (TIME_MS_W5500_PHY_DISCONNECT_WAIT_STABLE/TIME_MS_W5500_PHY_DISCONNECT_CHECK_PERIOD)



/*----------------------------------外部辅助函数--------------------------------------------------*/
//获取主通讯网口状态
ULONG SrvImplNetLan8700SocketMainGetState(void);
//获取日志通讯网口状态
ULONG SrvImplNetLan8700SocketLogGetState(void);
//主通讯网口数据发送
uint32_t SrvImplNetLan8700SocketMainSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplNetLan8700SocketLogSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//获取主通讯网口状态
ULONG SrvImplNetW5500SocketMainGetState(void);
//获取日志通讯网口状态
ULONG SrvImplNetW5500SocketLogGetState(void);
//主通讯网口数据发送
uint32_t SrvImplNetW5500SocketMainSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplNetW5500SocketLogSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
/*----------------------------------任务调度的核心函数---------------------------------------------*/
//任务全局对象初始化
void SrvImplNetSocketVarInit(void);

//任务接收消息处理
void SrvImplNetSocketMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplNetSocketFSM_Init(void);

//任务状态机周期运行
void SrvImplNetSocketFSM_RunPeriod(void);




#endif

