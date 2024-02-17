/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 16:54:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:11:05 +0800
************************************************************************************************/ 
#ifndef __SRV_STATE_DATA_TYPE_H_
#define __SRV_STATE_DATA_TYPE_H_
#include "SrvStateMacro.h"

//NETX网卡状态和描述映射
typedef struct NETX_STATE_MAP_STR
{
    ULONG state;
    uint8_t* despStr;
}NETX_STATE_MAP_STR;

//Socket的状态
typedef struct NX_SOCKET_INFO
{
    ULONG tcp_packets_sent;
    ULONG tcp_bytes_sent;
    ULONG tcp_packets_received;
    ULONG tcp_bytes_received;
    ULONG tcp_retransmit_packets;
    ULONG tcp_packets_queued;
    ULONG tcp_checksum_errors;
    ULONG tcp_socket_state;
    ULONG tcp_transmit_queue_depth;
    ULONG tcp_transmit_window;
    ULONG tcp_receive_window;
}NX_SOCKET_INFO;
#define LENGTH_NX_SOCKET_INFO       (sizeof(NX_SOCKET_INFO)/sizeof(uint8_t))

//LAN8700网络底层信息集合
typedef struct SRV_STATE_LAN8700_SOCKET
{
    //Socket初始化标记
    uint32_t socketInitFlag;
    //Socket内存池
    NX_PACKET_POOL* netMemPoolPtr;
    /*--------------------------网口PHY链接参数---------------------------*/
    //当前网线连接状态
    ULONG lanPhyConnectStateCurrent;
    //上一次的网线连接状态
    ULONG lanPhyConnectStateLast;
    /*--------------------------主网口相关参数---------------------------*/
    //主通讯Socket
    NX_TCP_SOCKET tcpSocketMain;
    //主通讯网口Socket状态集合
    NX_SOCKET_INFO socketInfoMain;
    //主通讯网口Socket的状态,上一次
    ULONG socketMainStateLast;
    //主通讯网口Socket的状态,当前
    ULONG socketMainStateCurrent;
    /*--------------------------日志网口相关参数---------------------------*/
    //日志通讯Socket
    NX_TCP_SOCKET tcpSocketLog;
    //日志通讯Socket状态集合
    NX_SOCKET_INFO socketInfoLog;
    //日志通讯Socket的状态,上一次
    ULONG socketLogStateLast;
    //日志通讯Socket的状态,当前
    ULONG socketLogStateCurrent;
}SRV_STATE_LAN8700_SOCKET;
#define LENGTH_SRV_STATE_LAN8700_SOCKET       (sizeof(SRV_STATE_LAN8700_SOCKET)/sizeof(uint8_t))


//W5500网络底层信息集合
typedef struct SRV_STATE_W5500_SOCKET
{
    //Socket初始化标记
    uint32_t socketInitFlag;
    //Socket内存池
    NX_PACKET_POOL* netMemPoolPtr;
    /*--------------------------网口PHY链接参数---------------------------*/
    //当前网线连接状态
    ULONG lanPhyConnectStateCurrent;
    //上一次的网线连接状态
    ULONG lanPhyConnectStateLast;
    /*--------------------------主网口相关参数---------------------------*/
    //主通讯Socket
    NX_TCP_SOCKET tcpSocketMain;
    //主通讯网口Socket状态集合
    NX_SOCKET_INFO socketInfoMain;
    //主通讯网口Socket的状态,上一次
    ULONG socketMainStateLast;
    //主通讯网口Socket的状态,当前
    ULONG socketMainStateCurrent;
    /*--------------------------日志网口相关参数---------------------------*/
    //日志通讯Socket
    NX_TCP_SOCKET tcpSocketLog;
    //日志通讯Socket状态集合
    NX_SOCKET_INFO socketInfoLog;
    //日志通讯Socket的状态,上一次
    ULONG socketLogStateLast;
    //日志通讯Socket的状态,当前
    ULONG socketLogStateCurrent;
}SRV_STATE_W5500_SOCKET;
#define LENGTH_SRV_STATE_W5500_SOCKET       (sizeof(SRV_STATE_W5500_SOCKET)/sizeof(uint8_t))

//系统状态讯息集合
typedef struct SRV_STATE
{
    //LAN8700 Socket层信息集合
    SRV_STATE_LAN8700_SOCKET socketLan8700Msg;
    //W5500 Socket层信息集合
    SRV_STATE_W5500_SOCKET socketW5500Msg;
}SRV_STATE;
#define LENGTH_SRV_STATE                    (sizeof(SRV_STATE)/sizeof(uint8_t))

#endif



