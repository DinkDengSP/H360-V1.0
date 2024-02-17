/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-04 09:05:38
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-23 18:29:07
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __EC20_UTIL_H_
#define __EC20_UTIL_H_
#include "EC20_Port.h"
#include "EC20_Type.h"
#include "EC20_Command.h"
#include "EC20_Config.h"

//EC20的模块信息
extern EC20_MESSAGE ec20Message;
//设备操作互斥信号量
extern OS_MUTEX mutexMCU_EC20;

/****************************辅助模块运转的小方法**************************/
//EC20发送设置波特率指令
EC20_RESULT EC20_SetUartBaud(uint32_t baud);
//EC20保存用户配置讯息
EC20_RESULT EC20_SaveCurrentUserConfig(void);
//EC20开关回显
EC20_RESULT EC20_OpenCloseUartCmdEcho(EC20_FUNC echoFunc);
//EC20检测是否存在,指定波特率
EC20_RESULT EC20Util_CheckModuleExistSub(uint32_t baud);
//检测EC20模块是否准备好
EC20_RESULT EC20Util_CheckModuleReady(void);
//检测EC20卡是否准备好
EC20_RESULT EC20Util_CheckSIMCardReady(void);
//设置网络注册格式
EC20_RESULT EC20_SetNetWorkRegisterFormat(void);
//检测EC20网络注册是否完成
EC20_RESULT EC20Util_CheckNetRegisterReady(void);
//获取EC20网络注册基站信息,LAC基站位置区域码以及CID基站编号
EC20_RESULT EC20Util_CheckNetBaseStationMsg(void);
//检测EC20的GPS附着是否完成
EC20_RESULT EC20Util_CheckGprsAttachReady(void);
//配置GPRS数据网点
EC20_RESULT EC20_SetGprsConfig(void);
//关闭GPRS移动场景
EC20_RESULT EC20_DisableGprs(void);
//打开GPRS移动场景
EC20_RESULT EC20_EnableGprs(void);
//测试服务器能否ping通,可以看有没有网络
EC20_RESULT EC20_CheckNetPingServer(void);
//发送指令获取厂家信息
EC20_RESULT EC20_GetManufacturerDesp(void);
//发送指令获取模块类型信息
EC20_RESULT EC20_GetModuleTypeDesp(void);
//发送指令获取软件版本讯息
EC20_RESULT EC20_GetSoftwareVersionDesp(void);
//发送指令获取IMEI
EC20_RESULT EC20_GetIMEICodeDesp(void);
//发送指令获取SIM卡IMSI串号
EC20_RESULT EC20_GetIMSICodeDesp(void);
//发送指令获取SIM卡ICCID讯息
EC20_RESULT EC20_GetICCIDCodeDesp(void);
//发送指令获取网络全名,短名,供应商讯息
EC20_RESULT EC20_GetNetNameDesp(void);
//发送指令获取网络类型和波段选择
EC20_RESULT EC20_GetNetTypeDesp(void);
//获取信号强度,前一个是信号强度,后一个是误码率
EC20_RESULT EC20_GetSignalStrength(void);
//TCPIP连接网络
EC20_RESULT EC20_SocketConnectServicePort(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort);
//TCPIP网络断开
EC20_RESULT EC20_SocketDisconnectServicePort(uint8_t socketPort);
//TCPIP发送数据
EC20_RESULT EC20_SocketSendBufferMsg(uint8_t socketPort,uint8_t* msgBuffer,uint16_t msgLength);
//TCPIP检查当前当前缓冲区数据长度
EC20_RESULT EC20_SocketCheckWaitReadDataLength(uint8_t socketPort,uint16_t* needReadCount);
//TCPIP读取当前缓冲区数据
EC20_RESULT EC20_SocketReadRecvData(uint8_t socketPort,uint16_t needReadLength,uint8_t* readBuffer,uint16_t* realReadLength);
//TCPIP清空当前接收缓冲区
EC20_RESULT EC20_SocketClearRecvBuffer(uint8_t socketPort);
//获取当前连接状态
EC20_RESULT EC20_SocketCheckConnectState(uint8_t socketNumber,EC20_CONNECT_STATE state);
//查询上次连接是否断开
EC20_RESULT EC20_SocketCheckIsClose(uint8_t* disconnectFlag);

/**************************外部常用的集成方法***********************************/
//初始化EC20端口
void EC20Util_PortInit(void);
//控制电源
void EC20Util_PowerControl(EC20_POWER_MODE powerMode);
//模块复位
void EC20Util_ModuleReset(void);
//EC20检测是否存在,指定波特率
EC20_RESULT EC20Util_CheckModuleExist(void);


#endif

