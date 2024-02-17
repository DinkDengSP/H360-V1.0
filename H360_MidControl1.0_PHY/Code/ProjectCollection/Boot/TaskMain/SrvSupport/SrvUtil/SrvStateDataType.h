/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:07:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 11:43:22 +0800
************************************************************************************************/ 
#ifndef __SRV_STATE_DATA_TYPE_H_
#define __SRV_STATE_DATA_TYPE_H_
#include "SrvStatemacro.h"

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

//网络消息来源
typedef enum NET_DATA_SRC
{
    NET_DATA_SRC_LAN8700 = 0X00,
    NET_DATA_SRC_W5500 = 0X01,
}NET_DATA_SRC;

//主通讯端口数据包结构体
typedef struct NX_DATA_RECV
{
    NET_DATA_SRC netSrc;    //数据源
    ULONG peerIpAddress;    //发送数据的远端IP
    ULONG peerPort;         //发送数据的远端端口
    ULONG recvDataLength;   //接收数据长度
    uint8_t* recvDatBufPtr; //接收数据指针
}NX_DATA_RECV;
#define LENGTH_NX_DATA_RECV    (sizeof(NX_DATA_RECV)/sizeof(uint8_t))

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
typedef struct SRV_STATE_SOCKET_LAN8700
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
}SRV_STATE_SOCKET_LAN8700;
#define LENGTH_SRV_STATE_SOCKET_LAN8700       (sizeof(SRV_STATE_SOCKET_LAN8700)/sizeof(uint8_t))


//网络底层信息集合
typedef struct SRV_STATE_SOCKET_W5500
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
#if(NETX_FILE_TRANS_MODE_W5500 == NETX_FILE_TRANS_MODE_TFTP)
    /*--------------------------TFTP相关参数---------------------------*/
    //TFTP服务器对象
    NX_TFTP_SERVER serverTftp;
    //TFTP创建服务器结果
    uint32_t serverTftpCreateResult;
    //TFTP创建服务器时间
    uint32_t serverTftpCreateTimeTick;
    //TFTP是否启动状态
    uint32_t serverTftpStartState;
    uint32_t serverTftpLastStartTick;
#endif
    /*--------------------------FTP相关参数---------------------------*/
#if(NETX_FILE_TRANS_MODE_W5500 == NETX_FILE_TRANS_MODE_FTP)
    NX_FTP_SERVER serverFtp;
    //FTP创建服务器结果
    uint32_t serverFtpCreateResult;
    //FTP创建服务器时间
    uint32_t serverFtpCreateTimeTick;
    //FTP是否启动状态
    uint32_t serverFtpStartState;
    uint32_t serverFtpLastStartTick;
#endif
}SRV_STATE_SOCKET_W5500;
#define LENGTH_SRV_STATE_SOCKET_W5500       (sizeof(SRV_STATE_SOCKET_W5500)/sizeof(uint8_t))

//系统状态讯息集合
typedef struct SRV_STATE
{
    //LAN8700网络Socket层信息集合
    SRV_STATE_SOCKET_LAN8700 socketLan8700Msg;
}SRV_STATE;
#define LENGTH_SRV_STATE                    (sizeof(SRV_STATE)/sizeof(uint8_t))

#endif



