/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:25:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-08 14:54:53 +0800
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

//主通讯端口数据包结构体
typedef struct NX_DATA_MAIN_PORT
{
    uint8_t* recvDatBufPtr; //接收数据指针
    ULONG recvDataLength;   //接收数据长度
    ULONG peerIpAddress;    //发送数据的远端IP
    ULONG peerPort;         //发送数据的远端端口
}NX_DATA_MAIN_PORT;
#define LENGTH_NX_DATA_MAIN_PORT    (sizeof(NX_DATA_MAIN_PORT)/sizeof(uint8_t))

//日志信息
typedef struct SYS_LOG_MSG
{
    uint32_t msgLength;     //消息长度
    uint8_t* msgBufferPtr;  //消息指针
}SYS_LOG_MSG;
#define LENGTH_SYS_LOG_MSG          (sizeof(SYS_LOG_MSG)/sizeof(uint8_t))

//主网口通讯信息
typedef struct SYS_NET_BUF
{
    uint16_t msgLength;     //消息长度
    uint8_t* msgBufferPtr;  //消息指针
}SYS_NET_BUF;
#define LENGTH_SYS_NET_BUF          (sizeof(SYS_NET_BUF)/sizeof(uint8_t))


//网络底层信息集合
typedef struct SRV_STATE_SOCKET_BASE
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
}SRV_STATE_SOCKET_BASE;
#define LENGTH_SRV_STATE_SOCKET_BASE       (sizeof(SRV_STATE_SOCKET_BASE)/sizeof(uint8_t))

//系统状态讯息集合
typedef struct SRV_STATE
{
    //网络Socket层信息集合
    SRV_STATE_SOCKET_BASE socketBaseMsg;
}SRV_STATE;
#define LENGTH_SRV_STATE                    (sizeof(SRV_STATE)/sizeof(uint8_t))


#endif



