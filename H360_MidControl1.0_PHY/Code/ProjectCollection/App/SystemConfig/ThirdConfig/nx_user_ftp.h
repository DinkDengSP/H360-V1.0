/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-02 23:38:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-03 00:25:08 +0800
************************************************************************************************/ 
#ifndef NX_USER_FTP_H
#define NX_USER_FTP_H
#include "tx_user.h"

//FTP 服务器线程的优先级。 默认情况下，此值定义为 16，表示将优先级指定为 16
#define NX_FTP_SERVER_PRIORITY              30

//服务器可同时处理的最大客户端数。 默认情况下，此值为 4，表示同时支持 4 个客户端
#define NX_FTP_MAX_CLIENTS                  4

//服务器数据包池有效负载的最小大小（以字节为单位），包括 TCP、IP 和网络帧标头以及 HTTP 数据。 
//默认值为 256（FileX 中的最大文件名长度）+ 12 个字节（用于存储文件信息）+ NX_PHYSICAL_TRAILER
#define NX_FTP_SERVER_MIN_PACKET_PAYLOAD   (256 + 52 + NX_TCP_PACKET + NX_PHYSICAL_TRAILER)  

//指定内部服务将暂停的 ThreadX 时钟周期数。 默认值设置为 1 秒 (1 * NX_IP_PERIODIC_RATE)
#define NX_FTP_SERVER_TIMEOUT               NX_IP_PERIODIC_RATE

//指定不活动时保持客户端连接的秒数。 默认值设置为 240
#define NX_FTP_ACTIVITY_TIMEOUT             240

//指定服务器检查客户端活动的间隔（以秒为单位）。 默认值设置为 60
#define NX_FTP_TIMEOUT_PERIOD               60

//指定在重新传输服务器响应之前的初始超时（以秒为单位）。 默认值为 2
#define NX_FTP_SERVER_RETRY_SECONDS         2 

//指定服务器套接字上已排队的传输数据包的最大深度。 默认值为 20
#define NX_FTP_SERVER_TRANSMIT_QUEUE_DEPTH  20

//指定每个数据包的最大重试次数。 默认值为 10
#define NX_FTP_SERVER_RETRY_MAX             10

//指定在设置重试超时时要移位的位数。 默认值为 2，例如，每次重试超时都是上一次重试时间的两倍
#define NX_FTP_SERVER_RETRY_SHIFT           1

//如果定义了此选项，则可为 FileX 依赖项提供存根。 如果定义了此选项，
//则无需进行任何更改，FTP 客户端即可正常工作。 FTP 服务器将需要进行修改，
//或者用户必须创建几个 FileX 服务，FTP 服务器才能正常工作
//#define NX_FTP_NO_FILEX

//FTP 控制请求所需的服务类型。 默认情况下，此值定义为 NX_IP_NORMAL，表示正常的 IP 数据包服务
#define NX_FTP_CONTROL_TOS                  NX_IP_NORMAL

//FTP 数据请求所需的服务类型。 默认情况下，此值定义为 NX_IP_NORMAL，表示正常的 IP 数据包服务
#define NX_FTP_DATA_TOS                     NX_IP_NORMAL

//TCP 控制套接字窗口大小。 默认情况下，此值为 400 个字节
#define NX_FTP_CONTROL_WINDOW_SIZE          400

//TCP 数据套接字窗口大小。 默认情况下，此值为 2048 个字节
#define NX_FTP_DATA_WINDOW_SIZE             2048 

//指定数据包在被丢弃之前可通过的路由器数目。 默认值设置为 0x80
#define NX_FTP_TIME_TO_LIVE                 0x80

//指定客户端提供的“用户名”中允许的字节数。 默认值设置为 20
#define NX_FTP_USERNAME_SIZE                20

//指定客户端提供的“密码”中允许的字节数。 默认值设置为 20
#define NX_FTP_PASSWORD_SIZE                20

//为 FTP 请求启用分段。 默认情况下，此值为 NX_DONT_FRAGMENT，表示禁用 FTP TCP 分段
#define NX_FTP_FRAGMENT_OPTION              NX_DONT_FRAGMENT

//FTP服务器同级时间片轮转的轮转时间
#define NX_FTP_SERVER_TIME_SLICE            2

//数据报尾巴的保留字节数
#define NX_PHYSICAL_TRAILER                 4




#endif



