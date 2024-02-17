/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-02 13:43:06
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-08 14:27:55
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_EC20_H_
#define __BOARD_EC20_H_
#include "MCU_Inc.h"
#include "EC20_Inc.h"

//EC20连接的远程IP讯息
#define EC20_TCP_SERVICE_IP1                    47
#define EC20_TCP_SERVICE_IP2                    105
#define EC20_TCP_SERVICE_IP3                    54
#define EC20_TCP_SERVICE_IP4                    124
//EC20连接的远程端口号讯息
#define EC20_TCP_SERVICE_PORT                   9005

//宏定义转换EC20错误代码
#define EC20_MODULE_RESULT_CONVERT(ec20Result) (LH_ERR)(LH_ERR_CLOUD_EC20_BASE+((LH_ERR)ec20Result));

//EC20初始化
void BoardEC20_PortInit(void);

//EC20检测是否存在
LH_ERR BoardEC20_CheckExist(void);

//EC20重启
void BoardEC20_Restart(void);

//EC20检测模块ready
LH_ERR BoardEC20_CheckNetWorkReady(void);

//EC20使能网络连接
LH_ERR BoardEC20_EnableNetWork(void);

//EC20检测网络可用
LH_ERR BoardEC20_CheckNetWorkAvailable(void);

//EC20更新模块全部讯息
LH_ERR BoardEC20_ReflushModuleAllMsg(void);

//EC20打印模块全部讯息
void BoardEC20_PrintModuleAllMsg(void);

//获取信号强度
LH_ERR BoardEC20_GetSignalStrength(uint8_t* strength);

//EC20连接网络
LH_ERR BoardEC20_SocketConnect(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort);

//EC20断开网络
LH_ERR BoardEC20_SocketDisconnect(uint8_t socketPort);

//EC20发送一帧数据,不自动联机断开
LH_ERR BoardEC20_SocketSendMsgWithoutConnect(uint8_t socketPort,uint8_t* msgBuffer,uint16_t msgLength);

//EC20发送一帧数据,自动联机断开
LH_ERR BoardEC20_SocketSendMsgWithConnect(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort,uint8_t* msgBuffer,uint16_t msgLength);

//读取当前缓冲区等待读取的数据数量
LH_ERR BoardEC20_SocketGetLengthWaitRead(uint8_t socketPort,uint16_t* waitReadLength);

//清除当前缓冲区全部讯息
LH_ERR BoardEC20_SocketClearBuf(uint8_t socketPort);

//读取指定长度缓冲区数据
LH_ERR BoardEC20_SocketReadBufferByLength(uint8_t socketPort,uint8_t* recvBuffer,uint16_t readLengthSet,uint16_t* realReadLength);

#endif





