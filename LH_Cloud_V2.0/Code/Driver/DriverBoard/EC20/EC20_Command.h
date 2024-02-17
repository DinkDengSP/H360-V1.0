/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-04 09:04:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-03-22 13:47:44
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __EC20_COMMAND_H_
#define __EC20_COMMAND_H_
#include "string.h"

//EC20模块指令,检查模块是否存在
#define EC20_CMD_CHECK_MODULE_EXIST                 "AT"
//EC20模块指令,设置模块波特率
#define EC20_CMD_SET_MODULE_BAUD                    "AT+IPR="
//EC20模块指令,保存模块当前配置信息
#define EC20_CMD_SAVE_USER_CONFIG                   "AT&W"
//EC20模块指令,打开模块AT指令回显,将发送的数据打印出来
#define EC20_CMD_CLOSE_AT_ECHO                      "ATE0"
//EC20模块指令,关闭模块AT指令回显,不将发送的数据打印出来
#define EC20_CMD_OPEN_AT_ECHO                       "ATE1"
//EC20模块指令,检查模块是否已经准备好,可以接受指令
#define EC20_CMD_CHECK_MODULE_READY                 "AT+CPAS"
//EC20模块指令,检查SIM卡是否已经解锁或者SIM卡等待解锁
#define EC20_CMD_CHECK_SIM_CARD_PIN_READY           "AT+CPIN?"
//EC20模块指令,检查SIM卡是否功能初始化完成
#define EC20_CMD_CHECK_SIM_CARD_INIT_READY          "AT+QINISTAT"
//设置模块网络注册格式
#define EC20_CMD_SET_NOTWORK_REGISTER_FORMAT        "AT+CREG=2"
//EC20模块指令,检查模块是否已经注册上网络
#define EC20_CMD_CHECK_NETWORK_REGISTERED           "AT+CREG?"
//EC20模块指令,检查模块GPS是否已经附着
#define EC20_CMD_CHECK_GPS_ATTACH                   "AT+CGATT?"
//EC20模块指令,查询模块厂家编码
#define EC20_CMD_QUERY_MANUFACTURER_STR             "AT+GMI"
//EC20模块指令,查询模块类型编码
#define EC20_CMD_QUERY_MODULE_TYPE_STR              "AT+GMM"
//EC20模块指令,查询模块软件版本号编码
#define EC20_CMD_QUERY_SOFTWARE_VERSION_STR         "AT+GMR"
//EC20模块指令,查询模块入网许可IMEI号码
#define EC20_CMD_QUERY_MODULE_IMEI_STR              "AT+GSN"
//EC20模块指令,查询SIM卡入网许可IMSI号码
#define EC20_CMD_QUERY_SIM_IMSI_STR                 "AT+CIMI"
//EC20模块指令,查询SIM卡唯一ID ICCID
#define EC20_CMD_QUERY_SIM_ICCID_STR                "AT+QCCID"
//EC20模块指令,查询当前模块注册的网络名称
#define EC20_CMD_QUERY_NET_NAME                     "AT+QSPN"
//EC20模块指令,查询当前模块注册的网络类型
#define EC20_CMD_QUERY_NET_TYPE                     "AT+QNWINFO"
//EC20模块指令,查询当前信号强度
#define EC20_CMD_QUERY_SIGNAL_STRENGTH              "AT+CSQ"
//默认设置GPRS接入APN,上下文1,IPV4协议,CMNET中国移动
#define EC20_CMD_SET_GPRS_APN_POINT                 "AT+QICSGP=1,1,\"CMNET\""
//关闭GPRS移动场景
#define EC20_CMD_DEACT_GPRS                         "AT+QIDEACT=1"
//打开GPRS移动场景
#define EC20_CMD_ACT_GPRS                           "AT+QIACT=1"
//ping一下服务器,看看能否联网
#define EC20_CMD_PING_SERVER                         "AT+QPING=1,\"47.103.19.144\""
//网络连接 1标识辅佐的网络为1
#define EC20_CMD_SOCKET_CONNECT                     "AT+QIOPEN=1"
//网络断开
#define EC20_CMD_SOCKET_DISCONNECT                  "AT+QICLOSE="
//网络数据包发送
#define EC20_CMD_SOCKET_SEND_DATA                   "AT+QISEND="
//检查当前网络需要读取的数据长度
#define EC20_CMD_SOCKET_CHECK_RECV_LENGTH           "AT+QIRD="
//读取指定的数据长度的数据,如果不够就读取全部的数据
#define EC20_CMD_SOCKET_READ_DATA                   "AT+QIRD="
//清除缓冲区,将数据全部读取出来
#define EC20_CMD_SOCKET_CLEAR_BUF                   "AT+QIRD="
//查询当前连接状态,默认带上下文
#define EC20_CMD_CHECK_CONNECT_STATE                "AT+QISTATE=1,"

#endif






