/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-02 23:38:29 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-03 00:25:00 +0800
************************************************************************************************/ 
#ifndef NX_USER_TFTP_H
#define NX_USER_TFTP_H
#include "tx_user.h"

//如果定义了此选项，则可为 FileX 依赖项提供存根。 如果定义了此选项，则无需进行任何更改，
//TFTP 客户端即可正常工作。 TFTP 服务器需要修改，或者用户必须创建几个 FileX 服务才能正常工作
//#define NX_TFTP_NO_FILEX

//服务器线程的优先级。 默认情况下，此值定义为 16，指定优先级为 16
#define NX_TFTP_SERVER_PRIORITY             30

//错误字符串中的最大字符数。 默认情况下，此值为 64
#define NX_TFTP_ERROR_STRING_MAX            64

//服务器可同时处理的最大客户端数。 默认情况下，此值为 10，表示同时支持 10 个客户端
#define NX_TFTP_MAX_CLIENTS                 10

//TFTP UDP 请求所需的服务类型。 默认情况下，此值定义为 NX_IP_NORMAL，以指示正常的 IP 数据包服务
#define NX_TFTP_TYPE_OF_SERVICE             NX_IP_NORMAL

//为 TFTP UDP 请求启用分段。 默认情况下，此值为 NX_DONT_FRAGMENT，表示禁用 TFTP UDP 分段
#define NX_TFTP_FRAGMENT_OPTION             NX_DONT_FRAGMENT

//指定数据包在被丢弃之前可通过的路由器数目。 默认值设置为 0x80
#define NX_TFTP_TIME_TO_LIVE                0x80

//此选项允许 TFTP 客户端应用程序指定 TFTP 客户端 UDP 套接字端口。 默认为 NX_ANY_PORT
#define NX_TFTP_SOURCE_PORT                 NX_ANY_PORT

//数据报尾巴的保留字节数
#define NX_PHYSICAL_TRAILER                 4

//在向相同优先级的其他线程生成内容之前，要运行 TFTP 服务器的时间片。 默认值为 2
#define NX_TFTP_SERVER_TIME_SLICE           2

////指定服务器套接字上已排队的传输数据包的最大深度。 默认值为 20
#define NX_TFTP_QUEUE_DEPTH                 5

//默认情况下，数据以定长512字节传输，如果服务器支持扩展选项，可以使用blksize选项协商数据长度
#define NX_TFTP_FILE_TRANSFER_MAX           512         

//TFTP数据包最大长度
#define NX_TFTP_PACKET_SIZE                (NX_UDP_PACKET + NX_TFTP_FILE_TRANSFER_MAX + NX_PHYSICAL_TRAILER) 

//支持 TFTP 服务器的计时器检查每个 TFTP 客户端会话以了解最近的活动（ACK 或数据包）。 如果会话超时
//在达到最大次数后过期，则假定连接已断开。 服务器清除客户端请求，关闭所有打开的文件，并支持连接请求
//供下一客户端使用。 默认设置为“已禁用”
//#define NX_TFTP_SERVER_RETRANSMIT_ENABLE

#ifdef  NX_TFTP_SERVER_RETRANSMIT_ENABLE
//指定 TFTP 服务器计时器条目函数检查客户端连接以接收任何数据包的时间间隔。 默认值为 20（计时器时钟周期）
#define NX_TFTP_SERVER_TIMEOUT_PERIOD      20
//这是从客户端接收有效 ACK 或数据包的超时值。 默认值为 200（计时器时钟周期）
#define NX_TFTP_SERVER_RETRANSMIT_TIMEOUT  200
//指定更新客户端会话重新传输超时的最大次数。 此后，服务器将关闭该会话
#define NX_TFTP_SERVER_MAX_RETRIES         5

#else
//指定服务器从客户端接收重复 ACK 或数据包（会将其删除）的最大次数，期间不会向客户端发送错误消息并关闭会话。 
//如果定义了 NX_TFTP_SERVER_RETRANSMIT_ENABLE，则不起作用
#define NX_TFTP_MAX_CLIENT_RETRANSMITS     2

#endif 

 
















#endif


